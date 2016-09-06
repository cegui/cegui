/***********************************************************************
    created:    Sat Jan 31 2009
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
#include "CEGUI/RendererModules/OpenGL/GLXPBTextureTarget.h"
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
const float OpenGLGLXPBTextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
// internal attribute array used to get pbuffer configs
int pbAttrs[] =
{
    GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
    GLX_DOUBLEBUFFER, GL_FALSE,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    None
};

//----------------------------------------------------------------------------//
OpenGLGLXPBTextureTarget::OpenGLGLXPBTextureTarget(OpenGLRendererBase& owner) :
    OpenGLTextureTarget(owner),
    d_pbuffer(0)
{
    if (!GLXEW_VERSION_1_3)
        CEGUI_THROW(InvalidRequestException("System does not support GLX >= 1.3 "
            "required by CEGUI pbuffer usage under GLX"));

    d_dpy = glXGetCurrentDisplay();

    selectFBConfig();
    createContext();
    initialiseTexture();

    // set default size (and cause initialisation of the pbuffer)
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));

    // set some states as a one-time thing (because we use a separate context)
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
OpenGLGLXPBTextureTarget::~OpenGLGLXPBTextureTarget()
{
    if (d_pbuffer)
        glXDestroyPbuffer(d_dpy, d_pbuffer);
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::activate()
{
    enablePBuffer();

    // we clear the blend mode here so the next setupRenderingBlendMode call
    // is forced to update states for our local context.
    d_owner.setupRenderingBlendMode(BM_INVALID);

    OpenGLTextureTarget::activate();
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::deactivate()
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
void OpenGLGLXPBTextureTarget::clear()
{
    enablePBuffer();
    glDisable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);
    disablePBuffer();
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::declareRenderSize(const Sizef& sz)
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
void OpenGLGLXPBTextureTarget::initialisePBuffer()
{
    int creation_attrs[] =
    {
        GLX_PBUFFER_WIDTH, static_cast<int>(d_area.getWidth()),
        GLX_PBUFFER_HEIGHT, static_cast<int>(d_area.getHeight()),
        GLX_LARGEST_PBUFFER, True,
        GLX_PRESERVED_CONTENTS, True,
        None
    };

    // release any existing pbuffer
    if (d_pbuffer)
        glXDestroyPbuffer(d_dpy, d_pbuffer);

    d_pbuffer = glXCreatePbuffer(d_dpy, d_fbconfig, creation_attrs);

    if (!d_pbuffer)
        CEGUI_THROW(RendererException(
            "pbuffer creation error:  glXCreatePbuffer() failed"));

    // get the real size of the buffer that was created
    GLuint actual_width, actual_height;
    glXQueryDrawable(d_dpy, d_pbuffer, GLX_WIDTH, &actual_width);
    glXQueryDrawable(d_dpy, d_pbuffer, GLX_HEIGHT, &actual_height);
    d_area.setSize(Sizef(actual_width, actual_height));

    // ensure CEGUI::Texture is wrapping real GL texture and has correct size
    d_CEGUITexture->setOpenGLTexture(d_texture, d_area.getSize());
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::enablePBuffer() const
{
    // switch to using the pbuffer
    d_prevDisplay = glXGetCurrentDisplay();
    d_prevDrawable = glXGetCurrentDrawable();
    d_prevContext = glXGetCurrentContext();

    if (!glXMakeCurrent(d_dpy, d_pbuffer, d_context))
        std::cerr << "Failed to switch to pbuffer for rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::disablePBuffer() const
{
    // switch back to rendering to previous set up
    if (!glXMakeCurrent(d_prevDisplay, d_prevDrawable, d_prevContext))
        std::cerr << "Failed to switch from pbuffer rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::initialiseTexture()
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
void OpenGLGLXPBTextureTarget::selectFBConfig()
{
    int cfgcnt;
    GLXFBConfig* glxcfgs;

    glxcfgs = glXChooseFBConfig(d_dpy, DefaultScreen(d_dpy), pbAttrs, &cfgcnt);
    if (!glxcfgs)
        CEGUI_THROW(RendererException(
            "pbuffer creation failure, can't get suitable configuration."));

    d_fbconfig = glxcfgs[0];
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::createContext()
{
    d_context = glXCreateNewContext(d_dpy, d_fbconfig, GLX_RGBA_TYPE,
                                    glXGetCurrentContext(), true);

    if (!d_context)
        CEGUI_THROW(RendererException(
            "Failed to create GLX context for pbuffer."));
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::grabTexture()
{
    if (d_pbuffer)
    {
        glXDestroyPbuffer(d_dpy, d_pbuffer);
        d_pbuffer = 0;
    }

    OpenGLTextureTarget::grabTexture();
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::restoreTexture()
{
    OpenGLTextureTarget::restoreTexture();
    initialiseTexture();
    initialisePBuffer();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
