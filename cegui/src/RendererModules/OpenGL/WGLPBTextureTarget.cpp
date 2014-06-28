/***********************************************************************
    created:    Sun Feb 1 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/RendererModules/OpenGL/WGLPBTextureTarget.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/GeometryBuffer.h"

#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"

#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float OpenGLWGLPBTextureTarget::DEFAULT_SIZE = 128;

//----------------------------------------------------------------------------//
// internal attribute array used to get pbuffer configs
int pbAttrs[] =
{
    WGL_DRAW_TO_PBUFFER_ARB, true,
    WGL_SUPPORT_OPENGL_ARB, true,
    WGL_DOUBLE_BUFFER_ARB, false,
    WGL_RED_BITS_ARB, 8,
    WGL_GREEN_BITS_ARB, 8,
    WGL_BLUE_BITS_ARB, 8,
    WGL_ALPHA_BITS_ARB, 8,
    0, 0
};

//----------------------------------------------------------------------------//
OpenGLWGLPBTextureTarget::OpenGLWGLPBTextureTarget(OpenGLRendererBase& owner) :
    OpenGLTextureTarget(owner),
    d_pixfmt(0),
    d_pbuffer(0),
    d_context(0),
    d_hdc(0),
    d_prevContext(0),
    d_prevDC(0)
{
    if (!WGLEW_ARB_pbuffer)
        CEGUI_THROW(RendererException("WGL_ARB_pbuffer extension is needed to "
            "use OpenGLWGLPBTextureTarget!"));

    HDC hdc = wglGetCurrentDC();

    uint fmtcnt;
    wglChoosePixelFormatARB(hdc, pbAttrs, 0, 1, &d_pixfmt, &fmtcnt);

    if (!fmtcnt)
        CEGUI_THROW(RendererException(
            "pbuff creation failure, no suitable pixel formats."));

    initialiseTexture();

    // set default size (and cause initialisation of the pbuffer)
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
OpenGLWGLPBTextureTarget::~OpenGLWGLPBTextureTarget()
{
    releasePBuffer();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::activate()
{
    enablePBuffer();
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_FOG_COORDINATE_ARRAY);
    glDisableClientState(GL_EDGE_FLAG_ARRAY);

    // we clear the blend mode here so the next setupRenderingBlendMode call
    // is forced to update states for our local context.
    d_owner.setupRenderingBlendMode(BM_INVALID);

    OpenGLTextureTarget::activate();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::deactivate()
{
    // grab what we rendered into the texture
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     0, 0,
                     static_cast<GLsizei>(d_area.right()),
                     static_cast<GLsizei>(d_area.bottom()), 0);

    disablePBuffer();

    // Clear the blend mode again so the next setupRenderingBlendMode call
    // is forced to update states for the main / previous context.
    d_owner.setupRenderingBlendMode(BM_INVALID);

    OpenGLTextureTarget::deactivate();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::clear()
{
    // save the old clear colour
    GLfloat old_colour[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, old_colour);
    // save old scissor state
    GLboolean old_scissor;
    glGetBooleanv(GL_SCISSOR_TEST, &old_scissor);

    enablePBuffer();
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    disablePBuffer();
    
    // restore old scissor state
    if (GL_TRUE == old_scissor)
        glEnable(GL_SCISSOR_TEST);
    // restore the old clear colour
    glClearColor(old_colour[0], old_colour[1], old_colour[2], old_colour[3]);
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::declareRenderSize(const Sizef& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) &&
        (d_area.getHeight() >= sz.d_height))
            return;

    setArea(Rectf(d_area.getPosition(), d_owner.getAdjustedTextureSize(sz)));
    initialisePBuffer();
    clear();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::initialisePBuffer()
{
    int creation_attrs[] =
    {
        WGL_PBUFFER_LARGEST_ARB, true,
        0
    };

    releasePBuffer();

    HDC hdc = wglGetCurrentDC();
    d_pbuffer = wglCreatePbufferARB(hdc, d_pixfmt, 
                                    static_cast<int>(d_area.getWidth()),
                                    static_cast<int>(d_area.getHeight()),
                                    creation_attrs);

    if (!d_pbuffer)
        CEGUI_THROW(RendererException(
            "pbuffer creation failure, wglCreatePbufferARB() call failed."));

    d_hdc = wglGetPbufferDCARB(d_pbuffer);

    if (!d_hdc)
        CEGUI_THROW(RendererException(
            "pbuffer creation failure, wglGetPbufferDCARB() call failed."));

    d_context= wglCreateContext(d_hdc);

    if (!d_hdc)
        CEGUI_THROW(RendererException(
            "pbuffer creation failure, wglCreateContext() call failed."));

    if(!wglShareLists(wglGetCurrentContext(), d_context))
        CEGUI_THROW(RendererException(
            "pbuffer creation failure, wglShareLists() call failed."));

    // extract the actual size of the created bufer
    int actual_width, actual_height;
    wglQueryPbufferARB(d_pbuffer, WGL_PBUFFER_WIDTH_ARB, &actual_width);
    wglQueryPbufferARB(d_pbuffer, WGL_PBUFFER_HEIGHT_ARB, &actual_height);
    d_area.setSize(Sizef(static_cast<float>(actual_width),
                          static_cast<float>(actual_height)));

    // ensure CEGUI::Texture is wrapping real GL texture and has correct size
    d_CEGUITexture->setOpenGLTexture(d_texture, d_area.getSize());
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::releasePBuffer()
{
    if (d_pbuffer)
    {
        wglDeleteContext(d_context);
        wglReleasePbufferDCARB(d_pbuffer, d_hdc);
        wglDestroyPbufferARB(d_pbuffer);
    }
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::enablePBuffer() const
{
    // store old details
    d_prevContext = wglGetCurrentContext();
    d_prevDC = wglGetCurrentDC();

    // switch to rendering to the pbuffer
    if (!wglMakeCurrent(d_hdc, d_context))
        std::cerr << "Failed to switch to pbuffer for rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::disablePBuffer() const
{
    // change back to previous set up
    if (!wglMakeCurrent(d_prevDC, d_prevContext))
        std::cerr << "Failed to switch from pbuffer rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::initialiseTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // create and setup texture which pbuffer content will be loaded to
    glGenTextures(1, &d_texture);
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::grabTexture()
{
    releasePBuffer();
    OpenGLTextureTarget::grabTexture();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::restoreTexture()
{
    OpenGLTextureTarget::restoreTexture();
    initialiseTexture();
    initialisePBuffer();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
