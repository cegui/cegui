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
#include "CEGUI/RendererModules/OpenGLES/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/ImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner, const String& name) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_owner(owner),
    d_name(name)
{
    initPixelFormatFields(PF_RGBA);
    generateOpenGLESTexture();
}

//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner, const String& name,
                                 const String& filename,
                                 const String& resourceGroup) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner),
    d_name(name)
{
    initPixelFormatFields(PF_RGBA);
    generateOpenGLESTexture();
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner, const String& name,
                                 const Sizef& size) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner),
    d_name(name)
{
    initPixelFormatFields(PF_RGBA);
    generateOpenGLESTexture();
    setTextureSize(size);
}

//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner, const String& name,
                                 GLuint tex, const Sizef& size) :
    d_ogltexture(tex),
    d_size(size),
    d_grabBuffer(0),
    d_dataSize(size),
    d_owner(owner),
    d_name(name)
{
    initPixelFormatFields(PF_RGBA);
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
OpenGLESTexture::~OpenGLESTexture()
{
    cleanupOpenGLESTexture();
}

//----------------------------------------------------------------------------//
const String& OpenGLESTexture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& OpenGLESTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& OpenGLESTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& OpenGLESTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::loadFromFile(const String& filename,
    const String& resourceGroup)
{
    // Note from PDT:
    // There is somewhat tight coupling here between OpenGLESTexture and the
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
void OpenGLESTexture::loadFromMemory(const void* buffer,
                                     const Sizef& buffer_size,
                                     PixelFormat pixel_format)
{
    if (!isPixelFormatSupported(pixel_format))
        CEGUI_THROW(InvalidRequestException(
            "Data was supplied in an unsupported pixel format."));
    
    initPixelFormatFields(pixel_format);
    setTextureSize_impl(buffer_size);

    // store size of original data we are loading
    d_dataSize = buffer_size;
    updateCachedScaleValues();

    blitFromMemory(buffer, Rectf(Vector2f(0, 0), buffer_size));
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::loadUncompressedTextureBuffer(const Rectf& dest_area,
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
void OpenGLESTexture::loadCompressedTextureBuffer(const Rectf& dest_area,
                                                  const GLvoid* buffer) const
{
    const GLsizei image_size = getCompressedTextureSize(dest_area.getSize());

    glCompressedTexImage2D(GL_TEXTURE_2D, 0, d_format, 
                           static_cast<GLsizei>(dest_area.getWidth()),
                           static_cast<GLsizei>(dest_area.getHeight()),
                           0, image_size, buffer);
}

//----------------------------------------------------------------------------//
GLsizei OpenGLESTexture::getCompressedTextureSize(const Sizef& pixel_size) const
{
    // Calculate buffer size in bytes
    GLsizei image_space = 
        static_cast<GLsizei>(pixel_size.d_width * pixel_size.d_height);

    GLsizei num_bits =
        (d_format == GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG ? 4 : 2);

    return (image_space * num_bits + 7) / 8;
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::blitFromMemory(const void* sourceData, const Rectf& area)
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
void OpenGLESTexture::blitToMemory(void* targetData)
{
    // TODO:
    CEGUI_THROW(RendererException("unimplemented!"));
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::setTextureSize(const Sizef& sz)
{
    initPixelFormatFields(PF_RGBA);

    setTextureSize_impl(sz);

    d_dataSize = d_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::setTextureSize_impl(const Sizef& sz)
{
    const Sizef size(d_owner.getAdjustedTextureSize(sz));
    d_size = size;

    // make sure size is within boundaries
    GLfloat maxSize = static_cast<GLfloat>(d_owner.getMaxTextureSize());
    if ((size.d_width > maxSize) || (size.d_height > maxSize))
        CEGUI_THROW(RendererException("size too big"));

    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    glBindTexture(GL_TEXTURE_2D, d_ogltexture);

    // set texture to required size
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     static_cast<GLsizei>(size.d_width),
                     static_cast<GLsizei>(size.d_height),
                     0, d_format , d_subpixelFormat, 0);
    }

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::grabTexture()
{
    CEGUI_THROW(RendererException("unimplemented!"));
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::restoreTexture()
{
    CEGUI_THROW(RendererException("unimplemented!"));
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::updateCachedScaleValues()
{
    //
    // calculate what to use for x scale
    //
    const float orgW = d_dataSize.d_width;
    const float texW = d_size.d_width;

    // if texture and original data width are the same, scale is based
    // on the original size.
    // if texture is wider (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_x = 1.0f / ((orgW == texW) ? orgW : texW);

    //
    // calculate what to use for y scale
    //
    const float orgH = d_dataSize.d_height;
    const float texH = d_size.d_height;

    // if texture and original data height are the same, scale is based
    // on the original size.
    // if texture is taller (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_y = 1.0f / ((orgH == texH) ? orgH : texH);
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::generateOpenGLESTexture()
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
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::cleanupOpenGLESTexture()
{
    glDeleteTextures(1, &d_ogltexture);
    d_ogltexture = 0;
}

//----------------------------------------------------------------------------//
GLuint OpenGLESTexture::getOpenGLESTexture() const
{
    return d_ogltexture;
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::setOpenGLESTexture(GLuint tex, const Sizef& size)
{
    if (d_ogltexture != tex)
    {
        // cleanup the current state first.
        cleanupOpenGLESTexture();

        d_ogltexture = tex;
    }

    d_dataSize = d_size = size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
bool OpenGLESTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    switch (fmt)
    {
    case PF_RGBA:
    case PF_RGB:
    case PF_RGBA_4444:
    case PF_RGB_565:
        return true;

    case PF_PVRTC4:
    case PF_PVRTC2:
        return d_owner.isGLExtensionSupported("GL_IMG_texture_compression_pvrtc");

    default:
        return false;
    }
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::initPixelFormatFields(const PixelFormat fmt)
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

    case PF_PVRTC4:
        d_format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not really, but set for completeness.
        d_isCompressed = true;
        break;

    case PF_PVRTC2:
        d_format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
        d_subpixelFormat = GL_UNSIGNED_BYTE; // not really, but set for completeness.
        d_isCompressed = true;
        break;
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
