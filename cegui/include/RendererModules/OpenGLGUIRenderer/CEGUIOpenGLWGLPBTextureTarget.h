/***********************************************************************
    filename:   CEGUIOpenGLWGLPBTextureTarget.h
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
#ifndef _CEGUIOpenGLWGLPBTextureTarget_h_
#define _CEGUIOpenGLWGLPBTextureTarget_h_

#include "CEGUIOpenGLTextureTarget.h"

#include "wglext.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    OpenGLWGLPBTextureTarget - allows rendering to an OpenGL texture via the
    pbuffer extensions in the Windows OpenGL implementation.
*/
class OPENGL_GUIRENDERER_API OpenGLWGLPBTextureTarget :
    public OpenGLTextureTarget
{
public:
    /*!
    \brief
        OpenGLGLXPBTextureTarget default constructor.  Constructs an OpenGL
        texture target that draws using the pbuffer support in Windows.
    */
    OpenGLWGLPBTextureTarget();

    /*!
    \brief
        OpenGLWGLPBTextureTarget destructor.
    */
    virtual ~OpenGLWGLPBTextureTarget();

    // interface implementation
    virtual void clearRenderList(void);
    virtual void declareRenderSize(const Size& sz);


protected:
    /*!
    \brief
        Check support for, and initialise extensions required to use this
        RenderTarget implementation.
    */
    void initialiseExtensions();
    /*!
    \brief
        Handles creation / initialisation of the pbuffer.
    */
    void initialisePBuffer();
    /*!
    \brief
        Cleans up the pbuffer resources.
    */
    void releasePBuffer();
    /*!
    \brief
        Switches all rendering to the pbuffer.
    */
    void enablePBuffer();
    /*!
    \brief
        Switches rendering back to whatever was being used prior to the pbuffer.
    */
    void disablePBuffer();
    /*!
    \brief
        Initialise the OpenGL texture that the pbuffer content gets loaded to.
    */
    void initialiseTexture();

    // override state management funcs from base
    virtual void beginRenderState();
    virtual void endRenderState();

    // data members to hold the pointers to the WGL extension functions we use.
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
    PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
    PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
    PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
    PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;

    //! Holds the pixel format we use when creating the pbuffer.
    int d_pixfmt;
    //! Handle to the pbuffer itself.
    HPBUFFERARB d_pbuffer;
    //! Handle to the rendering context for the pbuffer.
    HGLRC d_context;
    //! Handle to the Windows device context for the pbuffer.
    HDC d_hdc;
    //! Handle to the rendering context in use when we switched to the pbuffer.
    HGLRC d_prevContext;
    //! Handle to the device context in use when we switched to the pbuffer.
    HDC d_prevDC;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOpenGLWGLPBTextureTarget_h_
