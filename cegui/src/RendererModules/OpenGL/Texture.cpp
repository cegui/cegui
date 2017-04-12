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
    d_grabBuffer(nullptr),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_owner(owner),
    d_name(name)
{
}

//----------------------------------------------------------------------------//
OpenGLTexture::~OpenGLTexture()
{
    cleanupOpenGLTexture();
}

//----------------------------------------------------------------------------//
void OpenGLTexture::initialise() 
{
    initInternalPixelFormatFields(PixelFormat::Rgba);
    generateOpenGLTexture();
}

//----------------------------------------------------------------------------//
void OpenGLTexture::initialise(const String& filename, const String& resourceGroup)
{
    initInternalPixelFormatFields(PixelFormat::Rgba);
    generateOpenGLTexture();
    loadFromFile(filename, resourceGroup);
}
//----------------------------------------------------------------------------//
GLint OpenGLTexture::getTextureFormat() const
{
    if (OpenGLInfo::getSingleton().isSizedInternalFormatSupported())
    {
        const char* err = "Invalid or unsupported OpenGL pixel format.";
        switch (d_pixelDataFormat)
        {
        case GL_RGBA:
            switch (d_pixelDataType)
            {
            case GL_UNSIGNED_BYTE:
                return GL_RGBA8;
            case GL_UNSIGNED_SHORT_4_4_4_4:
                return GL_RGBA4;
            default:
                throw RendererException(err);
            }
        case GL_RGB:
            switch (d_pixelDataType)
            {
            case GL_UNSIGNED_BYTE:
                return GL_RGB8;
            case GL_UNSIGNED_SHORT_5_6_5:
                return GL_RGB565;
            default:
                throw RendererException(err);
            }
        default:
            throw RendererException(err);
        }
    }

    return d_pixelDataFormat;
}
 
//----------------------------------------------------------------------------//
void OpenGLTexture::initialise(const Sizef& size)
{
    initInternalPixelFormatFields(PixelFormat::Rgba);
    generateOpenGLTexture();
    setTextureSize(size);
}

//----------------------------------------------------------------------------//
void OpenGLTexture::initialise(GLuint tex, const Sizef& size) 
{
    d_ogltexture = tex;
    d_size = size;
    d_dataSize = size;
    initInternalPixelFormatFields(PixelFormat::Rgba);
    updateCachedScaleValues();
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
const glm::vec2& OpenGLTexture::getTexelScaling() const
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
    CEGUI::System& system = System::getSingleton();

    system.getResourceProvider()->
        loadRawDataContainer(filename, texFile, resourceGroup);

    Texture* res = system.getImageCodec().load(texFile, this);

    // unload file data buffer
    System::getSingleton().getResourceProvider()->
        unloadRawDataContainer(texFile);

    if (!res)
        // It's an error
        throw RendererException(
            system.getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'.");
}

//----------------------------------------------------------------------------//
void OpenGLTexture::loadFromMemory(const void* buffer, const Sizef& buffer_size,
                    PixelFormat pixel_format)
{
    if (!isPixelFormatSupported(pixel_format))
        throw InvalidRequestException(
            "Data was supplied in an unsupported pixel format.");

    initInternalPixelFormatFields(pixel_format);
    setTextureSize_impl(buffer_size);

    // store size of original data we are loading
    d_dataSize = buffer_size;
    updateCachedScaleValues();

    blitFromMemory(buffer, Rectf(glm::vec2(0, 0), buffer_size));
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
                    d_pixelDataFormat, d_pixelDataType, buffer);

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
                              d_pixelDataFormat, image_size, buffer);
}

//----------------------------------------------------------------------------//
GLsizei OpenGLTexture::getCompressedTextureSize(const Sizef& pixel_size) const
{
    GLsizei blocksize = 16;
    return (
        static_cast<GLsizei>(
        std::ceil(pixel_size.d_width / 4) *
        std::ceil(pixel_size.d_height / 4) *
        blocksize));
}

//----------------------------------------------------------------------------//
void OpenGLTexture::setTextureSize(const Sizef& sz)
{
    initInternalPixelFormatFields(PixelFormat::Rgba);

    setTextureSize_impl(sz);

    d_dataSize = d_size;
    updateCachedScaleValues();
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
        d_pixelDataFormat = GL_RGBA;
        d_pixelDataType = GL_UNSIGNED_BYTE;
    }
    else // Desktop OpenGL
        buffer_size = static_cast<std::size_t>(d_size.d_width)
          *static_cast<std::size_t>(d_size.d_height) *4;
    d_grabBuffer = new std::uint8_t[buffer_size];

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

    /* In OpenGL ES we used "glReadPixels" to grab the texture, reading just the
       relevant rectangle. */
    Sizef blit_size = OpenGLInfo::getSingleton().isUsingOpenglEs() ? d_dataSize : d_size;
    blitFromMemory(d_grabBuffer, Rectf(glm::vec2(0, 0), blit_size));

    // free the grabbuffer
    delete[] d_grabBuffer;
    d_grabBuffer = nullptr;
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

void OpenGLTexture::updateCachedScaleValues()
{
    // Update the scale of a texel based on the absolute size
    d_texelScaling.x = (d_size.d_width != 0.0f) ? (1.0f / d_size.d_width) : 0.0f;
    d_texelScaling.y = (d_size.d_height != 0.0f) ? (1.0f / d_size.d_height) : 0.0f;
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

    setTextureEnvironment();

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}
//----------------------------------------------------------------------------//
void OpenGLTexture::setTextureEnvironment()
{
}

//----------------------------------------------------------------------------//
void OpenGLTexture::cleanupOpenGLTexture()
{
    // if the grabbuffer is not empty then free it
    if (d_grabBuffer)
    {
        delete[] d_grabBuffer;
        d_grabBuffer = nullptr;
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
    case PixelFormat::Rgba:
    case PixelFormat::Rgb:
    case PixelFormat::Rgba4444:
    case PixelFormat::Rgb565:
        return true;

    case PixelFormat::RgbDxt1:
    case PixelFormat::RgbaDxt1:
    case PixelFormat::RgbaDxt3:
    case PixelFormat::RgbaDxt5:
        return OpenGLInfo::getSingleton().isS3tcSupported();

    default:
        return false;
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
