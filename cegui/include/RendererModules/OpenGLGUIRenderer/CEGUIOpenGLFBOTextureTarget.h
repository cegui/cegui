/***********************************************************************
    filename:   CEGUIOpenGLFBOTextureTarget.h
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
#ifndef _CEGUIOpenGLFBOTextureTarget_h_
#define _CEGUIOpenGLFBOTextureTarget_h_

#define GL_GLEXT_PROTOTYPES
#include "CEGUIOpenGLTextureTarget.h"
#include "openglrenderer.h"

#include "glext.h"

#if defined(__linux__)
#   include <GL/glx.h>
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    OpenGLFBOTextureTarget - allows rendering to an OpenGL texture via FBO.
*/
class OPENGL_GUIRENDERER_API OpenGLFBOTextureTarget : public OpenGLTextureTarget
{
public:
    /*!
    \brief
        OpenGLFBOTextureTarget default constructor.  Constructs an OpenGL
        texture target that draws using the frame buffer object extension.
    */
    OpenGLFBOTextureTarget();

    /*!
    \brief
        OpenGLFBOTextureTarget destructor.
    */
    virtual ~OpenGLFBOTextureTarget();

    // interface implementation
    virtual void clearRenderList(void);
    virtual void declareRenderSize(const Size& sz);

protected:
    /*!
    \brief
        Set up the actual texture that the FBO will render to.
    */
    void initialiseRenderTexture();

    /*!
    \brief
        Initialise the OpenGL extension function pointers.
    */
    void initialiseOpenGLEXTFunctions();

    // override state management funcs from base
    virtual void beginRenderState();
    virtual void endRenderState();

    // pointers to the extension functions used
    PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus;
    PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers;
    PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers;
    PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer;
    PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;

    //! Frame buffer object.
    GLuint d_frameBuffer;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOpenGLFBOTextureTarget_h_
