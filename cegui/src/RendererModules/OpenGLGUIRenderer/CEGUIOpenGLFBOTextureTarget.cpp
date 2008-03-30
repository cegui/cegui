/***********************************************************************
    filename:   CEGUIOpenGLFBOTextureTarget.cpp
    created:    Tue Feb 12 2008
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
#include "CEGUIOpenGLFBOTextureTarget.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLFBOTextureTarget::OpenGLFBOTextureTarget()
{
    if (!OpenGLRenderer::isExtensionSupported("GL_EXT_framebuffer_object"))
        throw InvalidRequestException("Hardware does not support FBO");

    initialiseOpenGLEXTFunctions();

    // create the FBO
    glGenFramebuffers(1, &d_frameBuffer);
    // setup area.  This will cause the initial texture to be generated.
    declareRenderSize(Size(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
OpenGLFBOTextureTarget::~OpenGLFBOTextureTarget()
{
    glDeleteFramebuffers(1, &d_frameBuffer);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::clearRenderList(void)
{
    if (d_quadlist.empty())
        return;

    OpenGLRenderTarget::clearRenderList();

    // also need to clear texture surface itself
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, d_frameBuffer);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    // switch back to rendering to view port
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::beginRenderState()
{
    // switch to rendering to the texture
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, d_frameBuffer);
    // now call base implementation
    OpenGLRenderTarget::beginRenderState();
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::endRenderState()
{
    // now call base implementation
    OpenGLRenderTarget::endRenderState();
    // switch back to rendering to view port
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::declareRenderSize(const Size& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    d_area.setSize(getAdjustedTextureSize(sz));
    initialiseRenderTexture();
    clearRenderList();
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::initialiseRenderTexture()
{
    // switch to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, d_frameBuffer);
    // create OpenGL texture as needed
    if (d_texture == 0)
        glGenTextures(1, &d_texture);

    glBindTexture(GL_TEXTURE_2D, d_texture);
    // setup some texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // set the texture to be 2d and of the required dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 static_cast<GLsizei>(d_area.d_right),
                 static_cast<GLsizei>(d_area.d_bottom),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // attach the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                           GL_TEXTURE_2D, d_texture, 0);

    // TODO: Check for completeness and then maybe try some alternative stuff?

    // switch from frame buffer back to using view port
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::initialiseOpenGLEXTFunctions()
{
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)
        OpenGLRenderer::getGLProcAddr("glGenFramebuffersEXT");
    glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)
        OpenGLRenderer::getGLProcAddr("glDeleteFramebuffersEXT");
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)
        OpenGLRenderer::getGLProcAddr("glBindFramebufferEXT");
    glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)
        OpenGLRenderer::getGLProcAddr("glFramebufferTexture2DEXT");
    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)
        OpenGLRenderer::getGLProcAddr("glCheckFramebufferStatusEXT");
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
