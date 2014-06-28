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
#include "CEGUI/RendererModules/OpenGLES/FBOTextureTarget.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/PropertyHelper.h"

#include "CEGUI/RendererModules/OpenGLES/Renderer.h"
#include "CEGUI/RendererModules/OpenGLES/Texture.h"

#ifndef __APPLE__
#include "EGL/egl.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

// Where should it be?
typedef GLboolean (GL_APIENTRY *PFNGLISRENDERBUFFEROES)(GLuint);
typedef void (GL_APIENTRY *PFNGLBINDRENDERBUFFEROES)(GLenum, GLuint);
typedef void (GL_APIENTRY *PFNGLDELETERENDERBUFFERSOES)(GLsizei, const GLuint*);
typedef void (GL_APIENTRY *PFNGLGENRENDERBUFFERSOES)(GLsizei, GLuint*);
typedef void (GL_APIENTRY *PFNGLRENDERBUFFERSTORAGEOES)(GLenum, GLenum, GLsizei, GLsizei);
typedef void (GL_APIENTRY *PFNGLGETRENDERBUFFERPARAMETERIVOES)(GLenum, GLenum, GLint*);
typedef GLboolean (GL_APIENTRY *PFNGLISFRAMEBUFFEROES)(GLuint);
typedef void (GL_APIENTRY *PFNGLBINDFRAMEBUFFEROES)(GLenum, GLuint);
typedef void (GL_APIENTRY *PFNGLDELETEFRAMEBUFFERSOES)(GLsizei, const GLuint*);
typedef void (GL_APIENTRY *PFNGLGENFRAMEBUFFERSOES)(GLsizei, GLuint*);
typedef GLenum (GL_APIENTRY *PFNGLCHECKFRAMEBUFFERSTATUSOES)(GLenum);
typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DOES)(GLenum, GLenum, GLenum, GLuint, GLint);
typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERRENDERBUFFEROES)(GLenum, GLenum, GLenum, GLuint);
typedef void (GL_APIENTRY *PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)(GLenum, GLenum, GLenum, GLint*);
typedef void (GL_APIENTRY *PFNGLGENERATEMIPMAPOES)(GLenum);

PFNGLISRENDERBUFFEROES glIsRenderbufferEXT = 0;
PFNGLBINDRENDERBUFFEROES glBindRenderbufferEXT = 0;
PFNGLDELETERENDERBUFFERSOES glDeleteRenderbuffersEXT = 0;
PFNGLGENRENDERBUFFERSOES glGenRenderbuffersEXT = 0;
PFNGLRENDERBUFFERSTORAGEOES glRenderbufferStorageEXT = 0;
PFNGLGETRENDERBUFFERPARAMETERIVOES glGetRenderbufferParameterivEXT = 0;
PFNGLISFRAMEBUFFEROES glIsFramebufferEXT = 0;
PFNGLBINDFRAMEBUFFEROES glBindFramebufferEXT = 0;
PFNGLDELETEFRAMEBUFFERSOES glDeleteFramebuffersEXT = 0;
PFNGLGENFRAMEBUFFERSOES glGenFramebuffersEXT = 0;
PFNGLCHECKFRAMEBUFFERSTATUSOES glCheckFramebufferStatusEXT = 0;
PFNGLFRAMEBUFFERTEXTURE2DOES glFramebufferTexture2DEXT = 0;
PFNGLFRAMEBUFFERRENDERBUFFEROES glFramebufferRenderbufferEXT = 0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES glGetFramebufferAttachmentParameterivEXT = 0;
PFNGLGENERATEMIPMAPOES glGenerateMipmapEXT = 0;	
	
//----------------------------------------------------------------------------//
const float OpenGLESFBOTextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
uint OpenGLESFBOTextureTarget::s_textureNumber = 0;

//----------------------------------------------------------------------------//
OpenGLESFBOTextureTarget::OpenGLESFBOTextureTarget(OpenGLESRenderer& owner) :
    OpenGLESRenderTarget<TextureTarget>(owner),
    d_texture(0)
{
    // this essentially creates a 'null' CEGUI::Texture
    d_CEGUITexture = &static_cast<OpenGLESTexture&>(
        d_owner.createTexture(generateTextureName(), d_texture, d_area.getSize()));

    initialiseRenderTexture();

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
String OpenGLESFBOTextureTarget::generateTextureName()
{
    String tmp("_gles_tt_tex_");
    tmp.append(PropertyHelper<uint>::toString(s_textureNumber++));

    return tmp;
}

//----------------------------------------------------------------------------//
OpenGLESFBOTextureTarget::~OpenGLESFBOTextureTarget()
{
    glDeleteFramebuffersEXT(1, &d_frameBuffer);
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::declareRenderSize(const Sizef& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    setArea(Rectf(d_area.getPosition(), d_owner.getAdjustedTextureSize(sz)));
    resizeRenderTexture();
}

//----------------------------------------------------------------------------//
bool OpenGLESFBOTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::activate()
{
    // switch to rendering to the texture
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &d_oldFbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_OES, d_frameBuffer);

    OpenGLESRenderTarget::activate();
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::deactivate()
{
    OpenGLESRenderTarget::deactivate();

    // switch back to rendering to default buffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_OES, d_oldFbo);
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::clear()
{
    // save old clear colour
    GLfloat old_col[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, old_col);
	
	GLint old_fbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &old_fbo);

    // switch to our FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_OES, d_frameBuffer);
    // Clear it.
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    // switch back to rendering to view port
    glBindFramebufferEXT(GL_FRAMEBUFFER_OES, old_fbo);

    // restore previous clear colour
    glClearColor(old_col[0], old_col[1], old_col[2], old_col[3]);
}

//----------------------------------------------------------------------------//
Texture& OpenGLESFBOTextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::initialiseRenderTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));
	
	GLint old_fbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &old_fbo);

    // create FBO
    glGenFramebuffersEXT(1, &d_frameBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_OES, d_frameBuffer);

    // set up the texture the FBO will draw to
    glGenTextures(1, &d_texture);
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 static_cast<GLsizei>(DEFAULT_SIZE),
                 static_cast<GLsizei>(DEFAULT_SIZE),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
                              GL_TEXTURE_2D, d_texture, 0);

    // TODO: Check for completeness and then maybe try some alternative stuff?

    // switch from our frame buffer back to using default buffer.
    glBindFramebufferEXT(GL_FRAMEBUFFER_OES, old_fbo);

    // ensure the CEGUI::Texture is wrapping the gl texture and has correct size
    d_CEGUITexture->setOpenGLESTexture(d_texture, d_area.getSize());

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::resizeRenderTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    const Sizef sz(d_area.getSize());

    // set the texture to the required size
    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 static_cast<GLsizei>(sz.d_width),
                 static_cast<GLsizei>(sz.d_height),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    clear();

    // ensure the CEGUI::Texture is wrapping the gl texture and has correct size
    d_CEGUITexture->setOpenGLESTexture(d_texture, sz);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
bool OpenGLESFBOTextureTarget::isRenderingInverted() const
{
    return true;
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::initializedFBOExtension()
{
	if (!OpenGLESRenderer::isGLExtensionSupported("GL_OES_framebuffer_object"))
	    CEGUI_THROW(InvalidRequestException("This platform does not support FBO"));

#ifndef __APPLE__
	glIsRenderbufferEXT =
        (PFNGLISRENDERBUFFEROES)eglGetProcAddress("glIsRenderbufferOES") ;
	glBindRenderbufferEXT =
        (PFNGLBINDRENDERBUFFEROES)eglGetProcAddress("glBindRenderbufferOES");
	glDeleteRenderbuffersEXT =
        (PFNGLDELETERENDERBUFFERSOES)eglGetProcAddress("glDeleteRenderbuffersOES");
	glGenRenderbuffersEXT =
        (PFNGLGENRENDERBUFFERSOES)eglGetProcAddress("glGenRenderbuffersOES");
	glRenderbufferStorageEXT =
        (PFNGLRENDERBUFFERSTORAGEOES)eglGetProcAddress("glRenderbufferStorageOES");
	glGetRenderbufferParameterivEXT =
        (PFNGLGETRENDERBUFFERPARAMETERIVOES)eglGetProcAddress("glGetRenderbufferParameterivOES");
	glIsFramebufferEXT =
        (PFNGLISFRAMEBUFFEROES)eglGetProcAddress("glIsFramebufferOES");
	glBindFramebufferEXT =
        (PFNGLBINDFRAMEBUFFEROES)eglGetProcAddress("glBindFramebufferOES");
	glDeleteFramebuffersEXT =
        (PFNGLDELETEFRAMEBUFFERSOES)eglGetProcAddress("glDeleteFramebuffersOES");
	glGenFramebuffersEXT =
        (PFNGLGENFRAMEBUFFERSOES)eglGetProcAddress("glGenFramebuffersOES");
	glCheckFramebufferStatusEXT =
        (PFNGLCHECKFRAMEBUFFERSTATUSOES)eglGetProcAddress("glCheckFramebufferStatusOES");
	glFramebufferTexture2DEXT =
        (PFNGLFRAMEBUFFERTEXTURE2DOES)eglGetProcAddress("glFramebufferTexture2DOES");
	glFramebufferRenderbufferEXT =
        (PFNGLFRAMEBUFFERRENDERBUFFEROES)eglGetProcAddress("glFramebufferRenderbufferOES");
	glGetFramebufferAttachmentParameterivEXT =
        (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
	glGenerateMipmapEXT =
        (PFNGLGENERATEMIPMAPOES)eglGetProcAddress("glGenerateMipmapOES");
#else
	glIsRenderbufferEXT = ::glIsRenderbufferOES;
	glBindRenderbufferEXT = ::glBindRenderbufferOES;
	glDeleteRenderbuffersEXT = ::glDeleteRenderbuffersOES;
	glGenRenderbuffersEXT = ::glGenRenderbuffersOES;
	glRenderbufferStorageEXT = ::glRenderbufferStorageOES;
	glGetRenderbufferParameterivEXT = ::glGetRenderbufferParameterivOES;
	glIsFramebufferEXT = ::glIsFramebufferOES;
	glBindFramebufferEXT = ::glBindFramebufferOES;
	glDeleteFramebuffersEXT = ::glDeleteFramebuffersOES;
	glGenFramebuffersEXT = ::glGenFramebuffersOES;
	glCheckFramebufferStatusEXT = ::glCheckFramebufferStatusOES;
	glFramebufferTexture2DEXT = ::glFramebufferTexture2DOES;
	glFramebufferRenderbufferEXT = ::glFramebufferRenderbufferOES;
	glGetFramebufferAttachmentParameterivEXT = ::glGetFramebufferAttachmentParameterivOES;
	glGenerateMipmapEXT = ::glGenerateMipmapOES;
#endif

}
//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

//----------------------------------------------------------------------------//
// Implementation of template base class
#include "./RenderTarget.inl"

