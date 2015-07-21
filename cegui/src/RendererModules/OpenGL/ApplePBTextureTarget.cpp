/***********************************************************************
    created:    Sun Feb 1 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/ApplePBTextureTarget.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/GeometryBuffer.h"

#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float OpenGLApplePBTextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
static CGLPixelFormatAttribute fmtAttrs[] =
{
    kCGLPFAAccelerated,
    kCGLPFAPBuffer,
    kCGLPFAColorSize, static_cast<CGLPixelFormatAttribute>(24),
    kCGLPFAAlphaSize, static_cast<CGLPixelFormatAttribute>(8),
    static_cast<CGLPixelFormatAttribute>(0)
};

//----------------------------------------------------------------------------//
OpenGLApplePBTextureTarget::OpenGLApplePBTextureTarget(OpenGLRendererBase& owner) :
    OpenGLTextureTarget(owner),
    d_pbuffer(0),
    d_context(0)
{
    if (!GLEW_APPLE_pixel_buffer)
        CEGUI_THROW(RendererException("GL_APPLE_pixel_buffer extension is "
            "needed to use OpenGLApplePBTextureTarget!"));

    initialiseTexture();

    CGLError err;
    CGLContextObj cctx = CGLGetCurrentContext();
    if (err = CGLGetVirtualScreen(cctx, &d_screen))
        CEGUI_THROW(RendererException(
            "CGLGetVirtualScreen failed: " + String(CGLErrorString(err))));

    GLint fmt_count;
    CGLPixelFormatObj pix_fmt;
    if (err = CGLChoosePixelFormat(fmtAttrs, &pix_fmt, &fmt_count))
        CEGUI_THROW(RendererException(
            "CGLChoosePixelFormat failed: " + String(CGLErrorString(err))));

    err = CGLCreateContext(pix_fmt, cctx, &d_context);
    CGLDestroyPixelFormat(pix_fmt);

    if (err)
        CEGUI_THROW(RendererException(
            "CGLCreateContext failed: " + String(CGLErrorString(err))));

    // set default size (and cause initialisation of the pbuffer)
    CEGUI_TRY
    {
        declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
    }
    CEGUI_CATCH(...)
    {
        CGLDestroyContext(d_context);
        CEGUI_RETHROW;
    }

    // set these states one-time since we have our own context
    enablePBuffer();
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_FOG_COORDINATE_ARRAY);
    glDisableClientState(GL_EDGE_FLAG_ARRAY);
    glClearColor(0,0,0,0);
    disablePBuffer();
}

//----------------------------------------------------------------------------//
OpenGLApplePBTextureTarget::~OpenGLApplePBTextureTarget()
{
    if (d_context)
        CGLDestroyContext(d_context);

    if (d_pbuffer)
        CGLDestroyPBuffer(d_pbuffer);
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::activate()
{
    enablePBuffer();

    // we clear the blend mode here so the next setupRenderingBlendMode call
    // is forced to update states for our local context.
    d_owner.setupRenderingBlendMode(BM_INVALID);

    OpenGLTextureTarget::activate();
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::deactivate()
{
    glFlush();
    OpenGLTextureTarget::deactivate();

    // Clear the blend mode again so the next setupRenderingBlendMode call
    // is forced to update states for the main / previous context.
    d_owner.setupRenderingBlendMode(BM_INVALID);

    disablePBuffer();
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::clear()
{
    enablePBuffer();
    glDisable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);
    disablePBuffer();
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::declareRenderSize(const Sizef& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) &&
        (d_area.getHeight() >= sz.d_height))
            return;

    setArea(Rectf(d_area.getPosition(), d_owner.getAdjustedTextureSize(sz)));

    // dump any previous pbuffer
    if (d_pbuffer)
    {
        CGLDestroyPBuffer(d_pbuffer);
        d_pbuffer = 0;
    }

    CGLError err;
    if (err = CGLCreatePBuffer(d_area.getWidth(), d_area.getHeight(),
                                GL_TEXTURE_2D, GL_RGBA, 0, &d_pbuffer))
    {
        CEGUI_THROW(RendererException(
            "CGLCreatePBuffer failed: " + String(CGLErrorString(err))));
    }

    if (err = CGLSetPBuffer(d_context, d_pbuffer, 0, 0, d_screen))
        CEGUI_THROW(RendererException(
            "CGLSetPBuffer failed: " + String(CGLErrorString(err))));

    clear();

    // make d_texture use the pbuffer as it's data source
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    glBindTexture(GL_TEXTURE_2D, d_texture);
    err = CGLTexImagePBuffer(CGLGetCurrentContext(), d_pbuffer, GL_FRONT);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);

    if (err)
        CEGUI_THROW(RendererException(
            "CGLTexImagePBuffer failed: " + String(CGLErrorString(err))));

    // ensure CEGUI::Texture is wrapping real GL texture and has correct size
    d_CEGUITexture->setOpenGLTexture(d_texture, d_area.getSize());
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::initialiseTexture()
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
void OpenGLApplePBTextureTarget::enablePBuffer() const
{
    if (CGLGetCurrentContext() != d_context)
    {
        d_prevContext = CGLGetCurrentContext();
        CGLSetCurrentContext(d_context);
    }
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::disablePBuffer() const
{
    if (CGLGetCurrentContext() == d_context)
        CGLSetCurrentContext(d_prevContext);
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::grabTexture()
{
    if (d_pbuffer)
    {
        CGLDestroyPBuffer(d_pbuffer);
        d_pbuffer = 0;
    }

    OpenGLTextureTarget::grabTexture();
}

//----------------------------------------------------------------------------//
void OpenGLApplePBTextureTarget::restoreTexture()
{
    const Sizef sz(d_area.getSize());
    d_area.setSize(Sizef(0.0f, 0.0f));

    OpenGLTextureTarget::restoreTexture();
    initialiseTexture();
    declareRenderSize(sz);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
