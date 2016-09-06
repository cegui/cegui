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
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/ImageCodec.h"
#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLTexture::OpenGLTexture(OpenGLRendererBase& owner, const String& name) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_owner(owner),
    d_name(name)
{
    initInternalPixelFormatFields(PF_RGBA);
    generateOpenGLTexture();
}

//----------------------------------------------------------------------------//
OpenGLTexture::OpenGLTexture(OpenGLRendererBase& owner, const String& name,
                             const String& filename,
                             const String& resourceGroup) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner),
    d_name(name)
{
    initInternalPixelFormatFields(PF_RGBA);
    generateOpenGLTexture();
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OpenGLTexture::OpenGLTexture(OpenGLRendererBase& owner, const String& name,
                             const Sizef& size) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner),
    d_name(name)
{
    initInternalPixelFormatFields(PF_RGBA);
    generateOpenGLTexture();
    setTextureSize(size);
}

//----------------------------------------------------------------------------//
OpenGLTexture::OpenGLTexture(OpenGLRendererBase& owner, const String& name,
                             GLuint tex, const Sizef& size) :
    d_ogltexture(tex),
    d_size(size),
    d_grabBuffer(0),
    d_dataSize(size),
    d_owner(owner),
    d_name(name)
{
    initInternalPixelFormatFields(PF_RGBA);
    updateCachedScaleValues();
}
//----------------------------------------------------------------------------//
GLint OpenGLTexture::internalFormat() const
{
    if (OpenGLInfo::getSingleton().isSizedInternalFormatSupported())
    {
        const char* err = "Invalid or unsupported OpenGL pixel format.";
        switch (d_format)
        {
        case GL_RGBA:
            switch (d_subpixelFormat)
            {
            case GL_UNSIGNED_BYTE:
                return GL_RGBA8;
            case GL_UNSIGNED_SHORT_4_4_4_4:
                return GL_RGBA4;
            default:
                CEGUI_THROW(RendererException(err));
            }
        case GL_RGB:
            switch (d_subpixelFormat)
            {
            case GL_UNSIGNED_BYTE:
                return GL_RGB8;
            case GL_UNSIGNED_SHORT_5_6_5:
                return GL_RGB565;
            default:
                CEGUI_THROW(RendererException(err));
            }
        default:  CEGUI_THROW(RendererException(err));
        }
    }
    else
        return d_format;
}

//----------------------------------------------------------------------------//
void OpenGLTexture::initInternalPixelFormatFields(const PixelFormat fmt)
{
    d_isCompressed = false;

    switch (fmt)
    {
    case PF_RGBA:
        d_format = GL_RGBA;
        d_subpixelFormat = GL_UNSIGNED_BYTE;
        break;

    case PF_RGB:
        d_format = GL_RGB;
        d_subpixelFormat = GL_UNSIGNED_BYTE;
        break;

    case PF_RGB_565:
        d_format = GL_RGB;
        d_subpixelFormat = GL_UNSIGNED_SHORT_5_6_5;
        break;

    case PF_RGBA_4444:
        d_format = GL_RGBA;
        d_subpixelFormat = GL_UNSIGNED_SHORT_4_4_4_4;
        break;

    case PF_RGB_DXT1:
        d_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    case PF_RGBA_DXT1:
        d_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    case PF_RGBA_DXT3:
        d_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    case PF_RGBA_DXT5:
        d_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not used.
        d_isCompressed = true;
        break;

    default:
        CEGUI_THROW(RendererException(
            "invalid or unsupported CEGUI::PixelFormat."));
    }
}

//----------------------------------------------------------------------------//
OpenGLTexture::~OpenGLTexture()
{
    cleanupOpenGLTexture();
}

//----------------------------------------------------------------------------//
const String& OpenGLTexture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& OpenGLTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& OpenGLTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& OpenGLTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void OpenGLTexture::loadFromFile(const String& filename,
    const String& resourceGroup)
{
    // Note from PDT:
    // There is somewhat tight coupling here between OpenGLTexture and the
    // ImageCodec classes - we have intimate knowledge of how they are
    // implemented and that knowledge is relied upon in an unhealthy way; this
    // should be addressed at some stage.

    // load file to memory via resource provider
    RawDataContainer texFile;
    System::getSingleton().getResourceProvider()->
        loadRawDataContainer(filename, texFile, resourceGroup);

    // get and check existence of CEGUI::System (needed to access ImageCodec)
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException(
            "CEGUI::System object has not been created: "
            "unable to access ImageCodec."));

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    System::getSingleton().getResourceProvider()->
        unloadRawDataContainer(texFile);

    if (!res)
        // It's an error
        CEGUI_THROW(RendererException(
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void OpenGLTexture::loadFromMemory(const void* buffer, const Sizef& buffer_size,
                    PixelFormat pixel_format)
{
    if (!isPixelFormatSupported(pixel_format))
        CEGUI_THROW(InvalidRequestException(
            "Data was supplied in an unsupported pixel format."));

    initInternalPixelFormatFields(pixel_format);
    setTextureSize_impl(buffer_size);

    // store size of original data we are loading
    d_dataSize = buffer_size;
    updateCachedScaleValues();

    blitFromMemory(buffer, Rectf(Vector2f(0, 0), buffer_size));
}

//----------------------------------------------------------------------------//
void OpenGLTexture::loadUncompressedTextureBuffer(const Rectf& dest_area,
                                                  const GLvoid* buffer) const
{
    GLint old_pack;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &old_pack);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    static_cast<GLint>(dest_area.left()),
                    static_cast<GLint>(dest_area.top()),
                    static_cast<GLsizei>(dest_area.getWidth()),
                    static_cast<GLsizei>(dest_area.getHeight()),
                    d_format, d_subpixelFormat, buffer);

    glPixelStorei(GL_UNPACK_ALIGNMENT, old_pack);
}

//----------------------------------------------------------------------------//
void OpenGLTexture::loadCompressedTextureBuffer(const Rectf& dest_area,
                                                const GLvoid* buffer) const
{
    const GLsizei image_size = getCompressedTextureSize(dest_area.getSize());

    glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 
                              static_cast<GLint>(dest_area.left()),
                              static_cast<GLint>(dest_area.top()),
                              static_cast<GLsizei>(dest_area.getWidth()),
                              static_cast<GLsizei>(dest_area.getHeight()),
                              d_format, image_size, buffer);
}

//----------------------------------------------------------------------------//
GLsizei OpenGLTexture::getCompressedTextureSize(const Sizef& pixel_size) const
{
    GLsizei blocksize = 16;
    if (d_format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT ||
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
void OpenGLTexture::setTextureSize(const Sizef& sz)
{
    initInternalPixelFormatFields(PF_RGBA);

    setTextureSize_impl(sz);

    d_dataSize = d_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void OpenGLTexture::setTextureSize_impl(const Sizef& sz)
{
    const Sizef size(d_owner.getAdjustedTextureSize(sz));
    d_size = size;

    // make sure size is within boundaries
    GLfloat maxSize;
    glGetFloatv(GL_MAX_TEXTURE_SIZE, &maxSize);
    if ((size.d_width > maxSize) || (size.d_height > maxSize))
        CEGUI_THROW(RendererException("size too big"));

    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // set texture to required size
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);

    if (d_isCompressed)
    {
        const GLsizei image_size = getCompressedTextureSize(size);
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, d_format,
                               static_cast<GLsizei>(size.d_width),
                               static_cast<GLsizei>(size.d_height),
                               0, image_size, 0);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat(),
                     static_cast<GLsizei>(size.d_width),
                     static_cast<GLsizei>(size.d_height),
                     0, d_format, d_subpixelFormat, 0);
    }

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLTexture::grabTexture()
{
    // if texture has already been grabbed, do nothing.
    if (d_grabBuffer)
        return;

    std::size_t buffer_size(0);
    if (OpenGLInfo::getSingleton().isUsingOpenglEs())
    {
        /* OpenGL ES 3.1 or below doesn't support
           "glGetTexImage"/"glGetCompressedTexImage", so we need to emulate it
           with "glReadPixels", which will return the data in (umcompressed)
           format (GL_RGBA, GL_UNSIGNED_BYTE). */
        buffer_size = static_cast<std::size_t>(d_dataSize.d_width)
          *static_cast<std::size_t>(d_dataSize.d_height) *4;
        d_isCompressed = false;
        d_format = GL_RGBA;
        d_subpixelFormat = GL_UNSIGNED_BYTE;
    }
    else // Desktop OpenGL
        buffer_size = static_cast<std::size_t>(d_size.d_width)
          *static_cast<std::size_t>(d_size.d_height) *4;
    d_grabBuffer = new uint8[buffer_size];

    blitToMemory(d_grabBuffer);

    glDeleteTextures(1, &d_ogltexture);
}

//----------------------------------------------------------------------------//
void OpenGLTexture::restoreTexture()
{
    if (!d_grabBuffer)
        return;

    generateOpenGLTexture();
    setTextureSize_impl(d_size);

    Sizef blit_size;
    /* In OpenGL ES we used "glReadPixels" to grab the texture, reading just the
       relevant rectangle. */
    blit_size = OpenGLInfo::getSingleton().isUsingOpenglEs() ? d_dataSize : d_size;
    blitFromMemory(d_grabBuffer, Rectf(Vector2f(0, 0), blit_size));

    // free the grabbuffer
    delete [] d_grabBuffer;
    d_grabBuffer = 0;
}

//----------------------------------------------------------------------------//
void OpenGLTexture::blitFromMemory(const void* sourceData, const Rectf& area)
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // do the real work of getting the data into the texture
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);

    if (d_isCompressed)
        loadCompressedTextureBuffer(area, sourceData);
    else
        loadUncompressedTextureBuffer(area, sourceData);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLTexture::blitToMemory(void* targetData)
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
void OpenGLTexture::updateCachedScaleValues()
{
    d_texelScaling.d_x = d_size.d_width  == 0.f  ?  0.f  :  1.f / d_size.d_width;
    d_texelScaling.d_y = d_size.d_height == 0.f  ?  0.f  :  1.f / d_size.d_height;
}

//----------------------------------------------------------------------------//
void OpenGLTexture::generateOpenGLTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    glGenTextures(1, &d_ogltexture);

    // set some parameters for this texture.
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // FIXME: This hack was needed to fix #980 in a way that maintains binary
    // compatibility in v0-8 branch.
    if (d_owner.getIdentifierString().find("CEGUI::OpenGLRenderer -") == 0)
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLTexture::cleanupOpenGLTexture()
{
    // if the grabbuffer is not empty then free it
    if (d_grabBuffer)
    {
        delete[] d_grabBuffer;
        d_grabBuffer = 0;
    }
    // otherwise delete any OpenGL texture associated with this object.
    else
    {
        glDeleteTextures(1, &d_ogltexture);
        d_ogltexture = 0;
    }
}

//----------------------------------------------------------------------------//
GLuint OpenGLTexture::getOpenGLTexture() const
{
    return d_ogltexture;
}

//----------------------------------------------------------------------------//
void OpenGLTexture::setOpenGLTexture(GLuint tex, const Sizef& size)
{
    if (d_ogltexture != tex)
    {
        // cleanup the current state first.
        cleanupOpenGLTexture();

        d_ogltexture = tex;
    }

    d_dataSize = d_size = size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
bool OpenGLTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    switch (fmt)
    {
    case PF_RGBA:
    case PF_RGB:
    case PF_RGBA_4444:
    case PF_RGB_565:
        return true;

    case PF_RGB_DXT1:
    case PF_RGBA_DXT1:
    case PF_RGBA_DXT3:
    case PF_RGBA_DXT5:
        return d_owner.isS3TCSupported();

    default:
        return false;
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
