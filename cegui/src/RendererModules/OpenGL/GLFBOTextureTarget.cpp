/***********************************************************************
    created:    Sun Jan 11 2009
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/GLFBOTextureTarget.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/GeometryBuffer.h"

#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float OpenGLFBOTextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
OpenGLFBOTextureTarget::OpenGLFBOTextureTarget(OpenGLRendererBase& owner) :
    OpenGLTextureTarget(owner)
{
    if (!GLEW_EXT_framebuffer_object)
        CEGUI_THROW(InvalidRequestException("Hardware does not support FBO"));

    // no need to initialise d_previousFrameBuffer here, it will be
    // initialised in activate()

    initialiseRenderTexture();

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
OpenGLFBOTextureTarget::~OpenGLFBOTextureTarget()
{
    glDeleteFramebuffersEXT(1, &d_frameBuffer);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::declareRenderSize(const Sizef& sz)
{
    setArea(Rectf(d_area.getPosition(), d_owner.getAdjustedTextureSize(sz)));
    resizeRenderTexture();
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::activate()
{
    // remember previously bound FBO to make sure we set it back
    // when deactivating
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,
            reinterpret_cast<GLint*>(&d_previousFrameBuffer));

    // switch to rendering to the texture
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, d_frameBuffer);

    OpenGLTextureTarget::activate();
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::deactivate()
{
    OpenGLTextureTarget::deactivate();

    // switch back to rendering to the previously bound framebuffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, d_previousFrameBuffer);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::clear()
{
    const Sizef sz(d_area.getSize());
    // Some drivers crash when clearing a 0x0 RTT. This is a workaround for
    // those cases.
    if (sz.d_width < 1.0f || sz.d_height < 1.0f)
        return;

    // save old clear colour
    GLfloat old_col[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, old_col);

    // remember previously bound FBO to make sure we set it back
    GLuint previousFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,
            reinterpret_cast<GLint*>(&previousFBO));

    // switch to our FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, d_frameBuffer);
    // Clear it.
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    // switch back to rendering to the previously bound FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFBO);

    // restore previous clear colour
    glClearColor(old_col[0], old_col[1], old_col[2], old_col[3]);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::initialiseRenderTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // create FBO
    glGenFramebuffersEXT(1, &d_frameBuffer);

    // remember previously bound FBO to make sure we set it back
    GLuint previousFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,
            reinterpret_cast<GLint*>(&previousFBO));

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, d_frameBuffer);

    // set up the texture the FBO will draw to
    glGenTextures(1, &d_texture);
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 static_cast<GLsizei>(DEFAULT_SIZE),
                 static_cast<GLsizei>(DEFAULT_SIZE),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, d_texture, 0);

    // TODO: Check for completeness and then maybe try some alternative stuff?

    // switch from our frame buffer back to the previously bound buffer.
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFBO);

    // ensure the CEGUI::Texture is wrapping the gl texture and has correct size
    d_CEGUITexture->setOpenGLTexture(d_texture, d_area.getSize());

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::resizeRenderTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // Some drivers (hint: Intel) segfault when glTexImage2D is called with
    // any of the dimensions being 0. The downside of this workaround is very
    // small. We waste a tiny bit of VRAM on cards with sane drivers and
    // prevent insane drivers from crashing CEGUI.
    Sizef sz(d_area.getSize());
    if (sz.d_width < 1.0f || sz.d_height < 1.0f)
    {
        sz.d_width = 1.0f;
        sz.d_height = 1.0f;
    }

    // set the texture to the required size
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 static_cast<GLsizei>(sz.d_width),
                 static_cast<GLsizei>(sz.d_height),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    clear();

    // ensure the CEGUI::Texture is wrapping the gl texture and has correct size
    d_CEGUITexture->setOpenGLTexture(d_texture, sz);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::grabTexture()
{
    glDeleteFramebuffersEXT(1, &d_frameBuffer);
    d_frameBuffer = 0;

    OpenGLTextureTarget::grabTexture();
}

//----------------------------------------------------------------------------//
void OpenGLFBOTextureTarget::restoreTexture()
{
    OpenGLTextureTarget::restoreTexture();

    initialiseRenderTexture();
    resizeRenderTexture();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
