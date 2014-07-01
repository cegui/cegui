/***********************************************************************
    created:    Tue Feb 17 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// helper function to return byte size of image of given size in given format
static size_t calculateDataSize(const Sizef size, Texture::PixelFormat fmt)
{
    switch (fmt)
    {
    case Texture::PF_RGBA:
        return size.d_width * size.d_height * 4;

    case Texture::PF_RGB:
        return size.d_width * size.d_height * 3;

    case Texture::PF_RGB_565:
    case Texture::PF_RGBA_4444:
        return size.d_width * size.d_height * 2;

    case Texture::PF_PVRTC2:
        return (static_cast<size_t>(size.d_width * size.d_height) * 2 + 7) / 8;

    case Texture::PF_PVRTC4:
        return (static_cast<size_t>(size.d_width * size.d_height) * 4 + 7) / 8;

    case Texture::PF_RGBA_DXT1:
        return std::ceil(size.d_width / 4) * std::ceil(size.d_height / 4) * 8;

    case Texture::PF_RGBA_DXT3:
    case Texture::PF_RGBA_DXT5:
        return std::ceil(size.d_width / 4) * std::ceil(size.d_height / 4) * 16;

    default:
        return 0;
    }
}

//----------------------------------------------------------------------------//
uint32 OgreTexture::d_textureNumber = 0;

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    createEmptyOgreTexture();
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const String& filename,
                         const String& resourceGroup) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    createEmptyOgreTexture();
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const Sizef& sz) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    createEmptyOgreTexture();

    // throw exception if no texture was able to be created
    if (d_texture.isNull())
        CEGUI_THROW(RendererException(
            "Failed to create Texture object with spcecified size."));
    
    d_size.d_width = d_texture->getWidth();
    d_size.d_height = d_texture->getHeight();
    d_dataSize = sz;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, Ogre::TexturePtr& tex,
                         bool take_ownership) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    setOgreTexture(tex, take_ownership);
}

//----------------------------------------------------------------------------//
OgreTexture::~OgreTexture()
{
    freeOgreTexture();
}

//----------------------------------------------------------------------------//
const String& OgreTexture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& OgreTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& OgreTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& OgreTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void OgreTexture::loadFromFile(const String& filename,
                               const String& resourceGroup)
{
    // get and check existence of CEGUI::System object
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException(
            "CEGUI::System object has not been created!"));

    // load file to memory via resource provider
    RawDataContainer texFile;
    sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
                                                     resourceGroup);

    ImageCodec& ic(sys->getImageCodec());

    // if we're using the integrated Ogre codec, set the file-type hint string
    if (ic.getIdentifierString().substr(0, 14)  == "OgreImageCodec")
    {
        String type;
        String::size_type i = filename.find_last_of(".");
        if (i != String::npos && filename.length() - i > 1)
            type = filename.substr(i+1);
        static_cast<OgreImageCodec&>(ic).setImageFileDataType(type);
    }

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    sys->getResourceProvider()->unloadRawDataContainer(texFile);

    // throw exception if data was load loaded to texture.
    if (!res)
        CEGUI_THROW(RendererException(
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void OgreTexture::loadFromMemory(const void* buffer, const Sizef& buffer_size,
                                 PixelFormat pixel_format)
{
    using namespace Ogre;

    if (!isPixelFormatSupported(pixel_format))
        CEGUI_THROW(InvalidRequestException(
            "Data was supplied in an unsupported pixel format."));

    const size_t byte_size = calculateDataSize(buffer_size, pixel_format);

    char* bufferCopy = new char[byte_size];
    memcpy(bufferCopy, buffer, byte_size);

    const Ogre::PixelBox* pixelBox = new Ogre::PixelBox(buffer_size.d_width, buffer_size.d_height,
                                                        1, toOgrePixelFormat(pixel_format), bufferCopy);
    d_texture->freeInternalResources();
    d_texture->setWidth(buffer_size.d_width);
    d_texture->setHeight(buffer_size.d_height);
    d_texture->setDepth(1);
    d_texture->createInternalResources();
    d_texture->getBuffer(0,0).get()->blitFromMemory(*pixelBox);

    // throw exception if no texture was able to be created
    if (d_texture.isNull())
        CEGUI_THROW(RendererException(
            "Failed to blit to Texture from memory."));

    d_size.d_width = d_texture->getWidth();
    d_size.d_height = d_texture->getHeight();
    d_dataSize = buffer_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void OgreTexture::blitFromMemory(const void* sourceData, const Rectf& area)
{
    if (d_texture.isNull()) // TODO: exception?
        return;

    // NOTE: const_cast because Ogre takes pointer to non-const here. Rather
    // than allow that to dictate poor choices in our own APIs, we choose to
    // address the issue as close to the source of the problem as possible.
    Ogre::PixelBox pb(area.getWidth(), area.getHeight(),
                      1, Ogre::PF_A8R8G8B8, const_cast<void*>(sourceData));
    Ogre::Image::Box box(area.left(), area.top(), area.right(), area.bottom());
    d_texture->getBuffer()->blitFromMemory(pb, box);
}

//----------------------------------------------------------------------------//
void OgreTexture::blitToMemory(void* targetData)
{
    if (d_texture.isNull()) // TODO: exception?
        return;

    Ogre::PixelBox pb(d_size.d_width, d_size.d_height,
                      1, Ogre::PF_A8R8G8B8, targetData);
    d_texture->getBuffer()->blitToMemory(pb);
}

//----------------------------------------------------------------------------//
void OgreTexture::freeOgreTexture()
{
    if (!d_texture.isNull() && !d_isLinked)
        Ogre::TextureManager::getSingleton().remove(d_texture->getHandle());

    d_texture.setNull();
}

//----------------------------------------------------------------------------//
Ogre::String OgreTexture::getUniqueName()
{
    #if OGRE_VERSION < 0x10A00
        Ogre::StringUtil::StrStreamType strstream;
        strstream << "_cegui_ogre_" << d_textureNumber++;

        return strstream.str();
    #endif
    #if OGRE_VERSION >= 0x10A00
        Ogre::StringStream strstream;
        strstream << "_cegui_ogre_" << d_textureNumber++;

        return strstream.str();
    #endif   
}

//----------------------------------------------------------------------------//
void OgreTexture::updateCachedScaleValues()
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
void OgreTexture::setOgreTexture(Ogre::TexturePtr texture, bool take_ownership)
{
    freeOgreTexture();

    d_texture = texture;
    d_isLinked = !take_ownership;

    if (!d_texture.isNull())
    {
        d_size.d_width = d_texture->getWidth();
        d_size.d_height= d_texture->getHeight();
        d_dataSize = d_size;
    }
    else
        d_size = d_dataSize = Sizef(0, 0);

    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
Ogre::TexturePtr OgreTexture::getOgreTexture() const
{
    return d_texture;
}

//----------------------------------------------------------------------------//
bool OgreTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    CEGUI_TRY
    {
        return Ogre::TextureManager::getSingleton().
            isEquivalentFormatSupported(Ogre::TEX_TYPE_2D,
                                        toOgrePixelFormat(fmt),
                                        Ogre::TU_DEFAULT);
    }
    CEGUI_CATCH(InvalidRequestException&)
    {
        return false;
    }
}

//----------------------------------------------------------------------------//
Ogre::PixelFormat OgreTexture::toOgrePixelFormat(const Texture::PixelFormat fmt)
{
    switch (fmt)
    {
        case Texture::PF_RGBA:       return Ogre::PF_A8B8G8R8;
        case Texture::PF_RGB:        return Ogre::PF_B8G8R8;
        case Texture::PF_RGB_565:    return Ogre::PF_R5G6B5;
        case Texture::PF_RGBA_4444:  return Ogre::PF_A4R4G4B4;
        case Texture::PF_PVRTC2:     return Ogre::PF_PVRTC_RGBA2;
        case Texture::PF_PVRTC4:     return Ogre::PF_PVRTC_RGBA4;
        case Texture::PF_RGBA_DXT1:  return Ogre::PF_DXT1;
        case Texture::PF_RGBA_DXT3:  return Ogre::PF_DXT3;
        case Texture::PF_RGBA_DXT5:  return Ogre::PF_DXT5;
        
        default:
            CEGUI_THROW(InvalidRequestException(
                "Invalid pixel format translation."));
    }
}

//----------------------------------------------------------------------------//
Texture::PixelFormat OgreTexture::fromOgrePixelFormat(
                                            const Ogre::PixelFormat fmt)
{
    switch (fmt)
    {
        case Ogre::PF_A8R8G8B8:     return Texture::PF_RGBA;
        case Ogre::PF_A8B8G8R8:     return Texture::PF_RGBA;
        case Ogre::PF_R8G8B8:       return Texture::PF_RGB;
        case Ogre::PF_B8G8R8:       return Texture::PF_RGB;
        case Ogre::PF_R5G6B5:       return Texture::PF_RGB_565;
        case Ogre::PF_A4R4G4B4:     return Texture::PF_RGBA_4444;
        case Ogre::PF_PVRTC_RGBA2:  return Texture::PF_PVRTC2;
        case Ogre::PF_PVRTC_RGBA4:  return Texture::PF_PVRTC4;
        case Ogre::PF_DXT1:         return Texture::PF_RGBA_DXT1;
        case Ogre::PF_DXT3:         return Texture::PF_RGBA_DXT3;
        case Ogre::PF_DXT5:         return Texture::PF_RGBA_DXT5;
        
        default:
            CEGUI_THROW(InvalidRequestException(
                "Invalid pixel format translation."));
    }
}

//----------------------------------------------------------------------------//
void OgreTexture::createEmptyOgreTexture()
{
    // try to create a Ogre::Texture with given dimensions
    d_texture = Ogre::TextureManager::getSingleton().createManual(
        getUniqueName(), "General", Ogre::TEX_TYPE_2D,
        1, 1, 0,
        Ogre::PF_A8B8G8R8);
}


//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
