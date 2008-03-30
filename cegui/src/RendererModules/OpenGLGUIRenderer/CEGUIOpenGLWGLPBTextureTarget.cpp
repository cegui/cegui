/***********************************************************************
    filename:   CEGUIOpenGLWGLPBTextureTarget.cpp
    created:    Mon Feb 18 2008
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
#include "CEGUIOpenGLWGLPBTextureTarget.h"
#include "CEGUIExceptions.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
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
OpenGLWGLPBTextureTarget::OpenGLWGLPBTextureTarget() :
    d_pixfmt(0),
    d_pbuffer(0),
    d_context(0),
    d_hdc(0),
    d_prevContext(0),
    d_prevDC(0)
{
    initialiseExtensions();

    HDC hdc = wglGetCurrentDC();

    uint fmtcnt;
    wglChoosePixelFormatARB(hdc, pbAttrs, 0, 1, &d_pixfmt, &fmtcnt);

    if (!fmtcnt)
        throw InvalidRequestException("pbuff creation failure - "
                                      "no suitable pixel formats.");

    d_area.setSize(Size(DEFAULT_SIZE, DEFAULT_SIZE));
    initialisePBuffer();
    initialiseTexture();
}

//----------------------------------------------------------------------------//
OpenGLWGLPBTextureTarget::~OpenGLWGLPBTextureTarget()
{
    releasePBuffer();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::clearRenderList(void)
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
void OpenGLWGLPBTextureTarget::declareRenderSize(const Size& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    d_area.setSize(getAdjustedTextureSize(sz));
    initialisePBuffer();
    clearRenderList();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::beginRenderState()
{
    enablePBuffer();
    // now call base implementation
    OpenGLRenderTarget::beginRenderState();
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::endRenderState()
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
void OpenGLWGLPBTextureTarget::initialisePBuffer()
{
    int creation_attrs[] =
    {
        WGL_PBUFFER_LARGEST_ARB, true,
        0
    };

    releasePBuffer();

    HDC hdc = wglGetCurrentDC();
    d_pbuffer = wglCreatePbufferARB(hdc, d_pixfmt, d_area.getWidth(),
                                    d_area.getHeight(), creation_attrs);

    if (!d_pbuffer)
        throw InvalidRequestException("pbuff creation failure - "
                                      "wglCreatePbufferARB() call failed.");

    d_hdc = wglGetPbufferDCARB(d_pbuffer);

    if (!d_hdc)
        throw InvalidRequestException("pbuff creation failure - "
                                      "wglGetPbufferDCARB() call failed.");

    d_context= wglCreateContext(d_hdc);

    if (!d_hdc)
        throw InvalidRequestException("pbuff creation failure - "
                                      "wglCreateContext() call failed.");

    if(!wglShareLists(wglGetCurrentContext(), d_context))
        throw InvalidRequestException("pbuff creation failure - "
        "wglShareLists() call failed.");

    // extract the actual size of the created bufer
    int actual_width, actual_height;
    wglQueryPbufferARB(d_pbuffer, WGL_PBUFFER_WIDTH_ARB, &actual_width);
    wglQueryPbufferARB(d_pbuffer, WGL_PBUFFER_HEIGHT_ARB, &actual_height);
    d_area.setSize(Size(actual_width, actual_height));
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
void OpenGLWGLPBTextureTarget::enablePBuffer()
{
    // store old details
    d_prevContext = wglGetCurrentContext();
    d_prevDC = wglGetCurrentDC();

    // switch to rendering to the pbuffer
    if (!wglMakeCurrent(d_hdc, d_context))
        std::cerr << "Failed to switch to pbuffer for rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::disablePBuffer()
{
    // change back to previous set up
    if (!wglMakeCurrent(d_prevDC, d_prevContext))
        std::cerr << "Failed to switch from pbuffer rendering" << std::endl;
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::initialiseTexture()
{
    // create and setup texture which pbuffer content will be loaded to
    glGenTextures(1, &d_texture);
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

//----------------------------------------------------------------------------//
void OpenGLWGLPBTextureTarget::initialiseExtensions()
{
    const char* (APIENTRY *getwglexts)(HDC) = (const char* (APIENTRY *)(HDC))
        wglGetProcAddress("wglGetExtensionsStringARB");

    if (!getwglexts)
        throw InvalidRequestException("Unable to determine available WGL "
                                      "extensions");

    std::string wglexts = getwglexts(wglGetCurrentDC());

    if (!OpenGLRenderer::isExtensionInString("WGL_ARB_pbuffer", wglexts))
        throw InvalidRequestException("Required extension WGL_ARB_pbuffer "
                                      "is not supported.");

    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)
        wglGetProcAddress("wglChoosePixelFormatARB");

    wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)
        wglGetProcAddress("wglReleasePbufferDCARB");

    wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)
        wglGetProcAddress("wglDestroyPbufferARB");

    wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)
        wglGetProcAddress("wglQueryPbufferARB");

    wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)
        wglGetProcAddress("wglGetPbufferDCARB");

    wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)
        wglGetProcAddress("wglCreatePbufferARB");

}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
