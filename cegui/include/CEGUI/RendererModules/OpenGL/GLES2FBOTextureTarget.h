/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
                David Reepmeyer (added GLES2/GLES3)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIGLES2FBOTextureTarget_h_
#define _CEGUIGLES2FBOTextureTarget_h_

#include "CEGUI/RendererModules/OpenGL/TextureTarget.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4250)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class GLES2Texture;
class GLES2Renderer;
class OpenGLBaseStateChangeWrapper;

//! ~GLES2FBOTextureTarget - allows rendering to an OpenGL texture via FBO.
class OPENGL_GUIRENDERER_API GLES2FBOTextureTarget : public OpenGLTextureTarget
{
public:
    GLES2FBOTextureTarget(GLES2Renderer& owner);
    virtual ~GLES2FBOTextureTarget();

    // overrides from GLES2RenderTarget
    void activate() override;
    void deactivate() override;
    // implementation of TextureTarget interface
    void clear() override;
    void declareRenderSize(const Sizef& sz) override;
    // specialise functions from GLES2TextureTarget
    void grabTexture() override;
    void restoreTexture() override;

protected:
    //! default size of created texture objects
    static const float DEFAULT_SIZE;

    //! allocate and set up the texture used with the FBO.
    void initialiseRenderTexture();
    //! resize the texture
    void resizeRenderTexture();
    //! Checks for OpenGL framebuffer completeness
    void checkFramebufferStatus();

    //! Frame buffer object.
    GLuint d_frameBuffer;
    //! Stencil buffer renderbuffer object
    GLuint d_stencilBufferRBO;
    //! Frame buffer object that was bound before we bound this one
    GLuint d_previousFrameBuffer;
    //! OpenGL state changer
    OpenGLBaseStateChangeWrapper* d_glStateChanger;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIOpenGLFBOTextureTarget_h_
