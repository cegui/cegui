/***********************************************************************
    created:    Sun Jan 11 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
     Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team

     Permission is hereby granted, free of charge, to any person obtaining
     a copy of this software and associated documentation files (the
     "Software"), to deal in the Software without restriction, including
     without limitation the rights to use, copy, modify, merge, publish,
     distribute, sublicense, and/or sell copies of the Software, and to
     permit persons to whom the Software is furnished to do so, subject to
     the following conditions:

     The above copyright notice and this permission notice shall be
     included in all copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
     IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
     OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
     ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
     OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/RendererModules/OpenGL/GLTexture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/ImageCodec.h"

#include "CEGUI/RendererModules/OpenGL/GL.h"

#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGL1Texture::OpenGL1Texture(OpenGLRendererBase& owner, const String& name)
    : OpenGLTexture(owner, name)
{
}

//----------------------------------------------------------------------------//
void OpenGL1Texture::initInternalPixelFormatFields(const PixelFormat fmt)
{
    d_isCompressed = false;

    switch(fmt)
    {
    case PixelFormat::RGBA:
        d_format = GL_RGBA;
        d_subpixelFormat = GL_UNSIGNED_BYTE;
        break;

    case PixelFormat::RGB:
        d_format = GL_RGB;
        d_subpixelFormat = GL_UNSIGNED_BYTE;
        break;

    case PixelFormat::RGB_565:
        d_format = GL_RGB;
        d_subpixelFormat = GL_UNSIGNED_SHORT_5_6_5;
        break;

    case PixelFormat::RGBA_4444:
        d_format = GL_RGBA;
        d_subpixelFormat = GL_UNSIGNED_SHORT_4_4_4_4;
        break;

    case PixelFormat::RGB_DXT1:
        d_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    case PixelFormat::RGBA_DXT1:
        d_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    case PixelFormat::RGBA_DXT3:
        d_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    case PixelFormat::RGBA_DXT5:
        d_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    default:
        throw RendererException(
                        "invalid or unsupported CEGUI::PixelFormat.");
    }
}

//----------------------------------------------------------------------------//
OpenGL1Texture::~OpenGL1Texture()
{
}

//----------------------------------------------------------------------------//
GLsizei OpenGL1Texture::getCompressedTextureSize(const Sizef& pixel_size) const
{
    GLsizei blocksize = 16;

    if(d_format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT ||
            d_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
    {
        blocksize = 8;
    }

    return (
               static_cast<GLsizei>(
                   std::ceil(pixel_size.d_width / 4) *
                   std::ceil(pixel_size.d_height / 4) *
                   blocksize));
}

//----------------------------------------------------------------------------//
void OpenGL1Texture::setTextureSize_impl(const Sizef& sz)
{
    const Sizef size(d_owner.getAdjustedTextureSize(sz));
    d_size = size;

    // make sure size is within boundaries
    GLfloat maxSize;
    glGetFloatv(GL_MAX_TEXTURE_SIZE, &maxSize);

    if((size.d_width > maxSize) || (size.d_height > maxSize))
        throw RendererException("size too big");

    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // set texture to required size
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);

    if(d_isCompressed)
    {
        const GLsizei image_size = getCompressedTextureSize(size);
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, d_format,
                               static_cast<GLsizei>(size.d_width),
                               static_cast<GLsizei>(size.d_height),
                               0, image_size, nullptr);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, d_format,
                     static_cast<GLsizei>(size.d_width),
                     static_cast<GLsizei>(size.d_height),
                     0, d_format, d_subpixelFormat, nullptr);
    }

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGL1Texture::blitToMemory(void* targetData)
{
    if (OpenGLInfo::getSingleton().isUsingOpenglEs())
    {
        /* OpenGL ES 3.1 or below doesn't support
        "glGetTexImage"/"glGetCompressedTexImage", so we need to emulate it
        using "glReadPixels". */

        GLint old_pack;
        glGetIntegerv(GL_PACK_ALIGNMENT, &old_pack);
        GLuint texture_framebuffer(0);
        GLint framebuffer_old(0), pack_alignment_old(0);
        glGenFramebuffers(1, &texture_framebuffer);
        GLenum framebuffer_target(0), framebuffer_param(0);
        if (OpenGLInfo::getSingleton()
            .isSeperateReadAndDrawFramebufferSupported())
        {
            framebuffer_param = GL_READ_FRAMEBUFFER_BINDING;
            framebuffer_target = GL_READ_FRAMEBUFFER;
        }
        else
        {
            framebuffer_param = GL_FRAMEBUFFER_BINDING;
            framebuffer_target = GL_FRAMEBUFFER;
        }
        glGetIntegerv(framebuffer_param, &framebuffer_old);
        glBindFramebuffer(framebuffer_target, texture_framebuffer);
        glFramebufferTexture2D(framebuffer_target, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d_ogltexture, 0);
        GLint read_buffer_old(0), pixel_pack_buffer_old(0);
        if (OpenGLInfo::getSingleton().isReadBufferSupported())
        {
            glGetIntegerv(GL_READ_BUFFER, &read_buffer_old);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &pixel_pack_buffer_old);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        }
        glGetIntegerv(GL_PACK_ALIGNMENT, &pack_alignment_old);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, static_cast<GLsizei>(d_dataSize.d_width),
            static_cast<GLsizei>(d_dataSize.d_height), GL_RGBA, GL_UNSIGNED_BYTE, targetData);
        glPixelStorei(GL_PACK_ALIGNMENT, pack_alignment_old);
        if (OpenGLInfo::getSingleton().isReadBufferSupported())
        {
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_pack_buffer_old);
            glReadBuffer(read_buffer_old);
        }
        glBindFramebuffer(framebuffer_target, framebuffer_old);
        glDeleteFramebuffers(1, &texture_framebuffer);

    }
    else // Desktop OpenGL
    {
        // save existing config
        GLuint old_tex;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

        glBindTexture(GL_TEXTURE_2D, d_ogltexture);

        if (d_isCompressed)
        {
            glGetCompressedTexImage(GL_TEXTURE_2D, 0, targetData);
        }
        else
        {
            GLint old_pack;
            glGetIntegerv(GL_PACK_ALIGNMENT, &old_pack);

            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glGetTexImage(GL_TEXTURE_2D, 0, d_format, d_subpixelFormat, targetData);

            glPixelStorei(GL_PACK_ALIGNMENT, old_pack);
        }

        // restore previous config.
        glBindTexture(GL_TEXTURE_2D, old_tex);
    }
}

//----------------------------------------------------------------------------//
void OpenGL1Texture::setTextureEnvironment()
{
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

