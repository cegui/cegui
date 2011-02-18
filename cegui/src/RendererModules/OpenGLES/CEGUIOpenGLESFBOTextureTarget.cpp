/***********************************************************************
    filename:   CEGUIOpenGLESFBOTextureTarget.cpp
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
#include "CEGUIOpenGLESFBOTextureTarget.h"
#include "CEGUIExceptions.h"
#include "CEGUIRenderQueue.h"
#include "CEGUIGeometryBuffer.h"

#include "CEGUIOpenGLESRenderer.h"
#include "CEGUIOpenGLESTexture.h"

#ifndef __APPLE__
#include "egl/egl.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

// Where should it be?
typedef GLboolean (GL_APIENTRY *PFNGLISRENDERBUFFEROES)(GLuint renderbuffer);
typedef void (GL_APIENTRY *PFNGLBINDRENDERBUFFEROES)(GLenum target, GLuint renderbuffer);
typedef void (GL_APIENTRY *PFNGLDELETERENDERBUFFERSOES)(GLsizei n, const GLuint *renderbuffers);
typedef void (GL_APIENTRY *PFNGLGENRENDERBUFFERSOES)(GLsizei n, GLuint *renderbuffers);
typedef void (GL_APIENTRY *PFNGLRENDERBUFFERSTORAGEOES)(GLenum target, GLenum internalformat,GLsizei width, GLsizei height);
typedef void (GL_APIENTRY *PFNGLGETRENDERBUFFERPARAMETERIVOES)(GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GL_APIENTRY *PFNGLISFRAMEBUFFEROES)(GLuint framebuffer);
typedef void (GL_APIENTRY *PFNGLBINDFRAMEBUFFEROES)(GLenum target, GLuint framebuffer);
typedef void (GL_APIENTRY *PFNGLDELETEFRAMEBUFFERSOES)(GLsizei n, const GLuint *framebuffers);
typedef void (GL_APIENTRY *PFNGLGENFRAMEBUFFERSOES)(GLsizei n, GLuint *framebuffers);
typedef GLenum (GL_APIENTRY *PFNGLCHECKFRAMEBUFFERSTATUSOES)(GLenum target);
typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DOES)(GLenum target, GLenum attachment,GLenum textarget, GLuint texture,GLint level);
typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERRENDERBUFFEROES)(GLenum target, GLenum attachment,GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GL_APIENTRY *PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)(GLenum target, GLenum attachment,GLenum pname, GLint *params);
typedef void (GL_APIENTRY *PFNGLGENERATEMIPMAPOES)(GLenum target);

PFNGLISRENDERBUFFEROES glIsRenderbufferEXT = NULL;
PFNGLBINDRENDERBUFFEROES glBindRenderbufferEXT = NULL;
PFNGLDELETERENDERBUFFERSOES glDeleteRenderbuffersEXT = NULL;
PFNGLGENRENDERBUFFERSOES glGenRenderbuffersEXT = NULL;
PFNGLRENDERBUFFERSTORAGEOES glRenderbufferStorageEXT = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVOES glGetRenderbufferParameterivEXT = NULL;
PFNGLISFRAMEBUFFEROES glIsFramebufferEXT = NULL;
PFNGLBINDFRAMEBUFFEROES glBindFramebufferEXT = NULL;
PFNGLDELETEFRAMEBUFFERSOES glDeleteFramebuffersEXT = NULL;
PFNGLGENFRAMEBUFFERSOES glGenFramebuffersEXT = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSOES glCheckFramebufferStatusEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DOES glFramebufferTexture2DEXT = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEROES glFramebufferRenderbufferEXT = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES glGetFramebufferAttachmentParameterivEXT = NULL;
PFNGLGENERATEMIPMAPOES glGenerateMipmapEXT = NULL;	
	
//----------------------------------------------------------------------------//
const float OpenGLESFBOTextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
OpenGLESFBOTextureTarget::OpenGLESFBOTextureTarget(OpenGLESRenderer& owner) :
    OpenGLESRenderTarget(owner),
    d_texture(0)
{
    // this essentially creates a 'null' CEGUI::Texture
    d_CEGUITexture = &static_cast<OpenGLESTexture&>(
        d_owner.createTexture(d_texture, d_area.getSize()));

    initialiseRenderTexture();

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Size(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
OpenGLESFBOTextureTarget::~OpenGLESFBOTextureTarget()
{
    glDeleteFramebuffersEXT(1, &d_frameBuffer);
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
void OpenGLESFBOTextureTarget::declareRenderSize(const Size& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    setArea(Rect(d_area.getPosition(), d_owner.getAdjustedTextureSize(sz)));
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

    const Size sz(d_area.getSize());

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
	if (!OpenGLESRenderer::IsGLExtensionSupported("GL_OES_framebuffer_object"))
		throw InvalidRequestException("This platform does not support FBO");

#ifndef __APPLE__
	glIsRenderbufferEXT = (PFNGLISRENDERBUFFEROES)eglGetProcAddress("glIsRenderbufferOES") ;
	glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEROES)eglGetProcAddress("glBindRenderbufferOES");
	glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSOES)eglGetProcAddress("glDeleteRenderbuffersOES");
	glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSOES)eglGetProcAddress("glGenRenderbuffersOES");
	glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEOES)eglGetProcAddress("glRenderbufferStorageOES");
	glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVOES)eglGetProcAddress("glGetRenderbufferParameterivOES");
	glIsFramebufferEXT = (PFNGLISFRAMEBUFFEROES)eglGetProcAddress("glIsFramebufferOES");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEROES)eglGetProcAddress("glBindFramebufferOES");
	glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSOES)eglGetProcAddress("glDeleteFramebuffersOES");
	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSOES)eglGetProcAddress("glGenFramebuffersOES");
	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSOES)eglGetProcAddress("glCheckFramebufferStatusOES");
	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DOES)eglGetProcAddress("glFramebufferTexture2DOES");
	glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEROES)eglGetProcAddress("glFramebufferRenderbufferOES");
	glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
	glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPOES)eglGetProcAddress("glGenerateMipmapOES");
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
