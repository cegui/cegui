/***********************************************************************
    filename:   CEGUIOpenGLGLXPBTextureTarget.cpp
    created:    Sat Feb 16 2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIOpenGLGLXPBTextureTarget.h"
#include "CEGUIExceptions.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
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
OpenGLGLXPBTextureTarget::OpenGLGLXPBTextureTarget() :
    d_pbuffer(0)
{
    d_dpy = glXGetCurrentDisplay();

    checkRequirements();
    selectFBConfig();
    createContext();
    initialiseTexture();

    // set default size and initialise the pbuffer
    d_area.setSize(Size(DEFAULT_SIZE, DEFAULT_SIZE));
    initialisePBuffer();
}

//----------------------------------------------------------------------------//
OpenGLGLXPBTextureTarget::~OpenGLGLXPBTextureTarget()
{
    if (d_pbuffer)
        glXDestroyPbuffer(d_dpy, d_pbuffer);
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::clearRenderList(void)
{
    if (d_quadlist.empty())
        return;

    OpenGLRenderTarget::clearRenderList();

    // clear pbuffer content as well as queued quads
    enablePBuffer();
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    disablePBuffer();
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::declareRenderSize(const Size& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    d_area.setSize(getAdjustedTextureSize(sz));
    initialisePBuffer();
    clearRenderList();
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::beginRenderState()
{
    enablePBuffer();
    // now call base implementation
    OpenGLRenderTarget::beginRenderState();
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::endRenderState()
{
    // grab what we rendered into the texture
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     0, 0,
                     static_cast<GLsizei>(d_area.d_right),
                     static_cast<GLsizei>(d_area.d_bottom), 0);

    // now call base implementation
    OpenGLRenderTarget::endRenderState();

    // switch back to any previous context etc
    disablePBuffer();
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::initialisePBuffer()
{
    int creation_attrs[] =
    {
        GLX_PBUFFER_WIDTH, d_area.getWidth(),
        GLX_PBUFFER_HEIGHT, d_area.getHeight(),
        GLX_LARGEST_PBUFFER, True,
        GLX_PRESERVED_CONTENTS, True,
        None
    };

    // release any existing pbuffer
    if (d_pbuffer)
        glXDestroyPbuffer(d_dpy, d_pbuffer);

    d_pbuffer = glXCreatePbuffer(d_dpy, d_fbconfig, creation_attrs);

    if (!d_pbuffer)
        throw GenericException(
            "pbuffer creation error:  glXCreatePbuffer() failed");

    // get the real size of the buffer that was created
    GLuint actual_width, actual_height;
    glXQueryDrawable(d_dpy, d_pbuffer, GLX_WIDTH, &actual_width);
    glXQueryDrawable(d_dpy, d_pbuffer, GLX_HEIGHT, &actual_height);
    d_area.setSize(Size(actual_width, actual_height));
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::enablePBuffer()
{
    // switch to using the pbuffer
    d_prevDisplay = glXGetCurrentDisplay();
    d_prevDrawable = glXGetCurrentDrawable();
    d_prevContext = glXGetCurrentContext();

    if (!glXMakeCurrent(d_dpy, d_pbuffer, d_context))
        std::cerr << "Failed to switch to pbuffer for rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::disablePBuffer()
{
    // switch back to rendering to previous set up
    if (!glXMakeCurrent(d_prevDisplay, d_prevDrawable, d_prevContext))
        std::cerr << "Failed to switch from pbuffer rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::initialiseTexture()
{
    // create and setup texture which pbuffer content will be loaded to
    glGenTextures(1, &d_texture);
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::checkRequirements() const
{
    if (!glXQueryExtension(d_dpy, 0, 0))
        throw InvalidRequestException("GLX extension is required.");

    int glx_maj, glx_min;
    if (!glXQueryVersion(d_dpy, &glx_maj, &glx_min))
        throw InvalidRequestException("Failed to query GLX version.");

    if (!((glx_maj == 1) && (glx_min >= 3)) || (glx_maj > 1))
        throw InvalidRequestException("GLX version 1.3 or better required.");
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::selectFBConfig()
{
    int cfgcnt;
    GLXFBConfig* glxcfgs;

    glxcfgs = glXChooseFBConfig(d_dpy, DefaultScreen(d_dpy), pbAttrs, &cfgcnt);
    if (!glxcfgs)
        throw GenericException(
            "pbuffer creation failure - can't get suitable configuration.");

    d_fbconfig = glxcfgs[0];
}

//----------------------------------------------------------------------------//
void OpenGLGLXPBTextureTarget::createContext()
{
    d_context = glXCreateNewContext(d_dpy, d_fbconfig, GLX_RGBA_TYPE,
                                    glXGetCurrentContext(), true);

    if (!d_context)
        throw GenericException("Failed to create GLX context for pbuffer.");
}

//----------------------------------------------------------------------------//
} // End of  CEGUI namespace section
