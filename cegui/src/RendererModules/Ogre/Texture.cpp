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
#include <sstream>
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include "CEGUI/RendererModules/Ogre/OgreMacros.h"
#include "CEGUI/RendererModules/Ogre/ResourceProvider.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/Rectf.h"
#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
#include <OgreRoot.h>
#include <OgreTextureGpuManager.h>
#include <OgrePixelFormatGpuUtils.h>
#include <OgreStagingTexture.h>
#else
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#endif // CEGUI_USE_OGRE_TEXTURE_GPU

#include <cstdint>
#include <sstream>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// helper function to return byte size of image of given size in given format
static size_t calculateDataSize(const Sizef size, Texture::PixelFormat fmt)
{
    switch (fmt)
    {
    case Texture::PixelFormat::Rgba:
        return static_cast<size_t>(size.d_width * size.d_height * 4);

    case Texture::PixelFormat::Rgb:
        return static_cast<size_t>(size.d_width * size.d_height * 3);

    case Texture::PixelFormat::Rgb565:
    case Texture::PixelFormat::Rgba4444:
        return static_cast<size_t>(size.d_width * size.d_height * 2);

    case Texture::PixelFormat::Pvrtc2:
        return (static_cast<size_t>(size.d_width * size.d_height) * 2 + 7) / 8;

    case Texture::PixelFormat::Pvrtc4:
        return (static_cast<size_t>(size.d_width * size.d_height) * 4 + 7) / 8;

    case Texture::PixelFormat::RgbaDxt1:
        return static_cast<size_t>( std::ceil(size.d_width / 4) * std::ceil(size.d_height / 4) * 8 );

    case Texture::PixelFormat::RgbaDxt3:
    case Texture::PixelFormat::RgbaDxt5:
        return static_cast<size_t>( std::ceil(size.d_width / 4) * std::ceil(size.d_height / 4) * 16 );

    default:
        return 0;
    }
}

//----------------------------------------------------------------------------//
std::uint32_t OgreTexture::d_textureNumber = 0;

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, bool notNullTexture) :
    #ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    d_texture(0),
    #endif
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    #ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    if (notNullTexture)
        createOgreTexture();
    #else
    createEmptyOgreTexture();
    #endif
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const String& filename,
                         const String& resourceGroup) :
    #ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    d_texture(0),
    #endif
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const Sizef& sz) :
    #ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    d_texture(0),
    #endif
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    const Ogre::uint32 width = static_cast<Ogre::uint32>(sz.d_width);
    const Ogre::uint32 height = static_cast<Ogre::uint32>(sz.d_height);

    createOgreTexture(Ogre::PFG_RGBA8_UNORM, width, height);

    #if 0
    size_t bytesPerRow = Ogre::PixelFormatGpuUtils::getBytesPerPixel( d_texture->getPixelFormat() ) * width;
    void* data = OGRE_MALLOC_SIMD( bytesPerRow * height, Ogre::MEMCATEGORY_RENDERSYS );
    memset( data, 0, bytesPerRow * height );

    Ogre::TextureGpuManager *textureMgr = Ogre::Root::getSingletonPtr()->getRenderSystem()->getTextureGpuManager();

    Ogre::StagingTexture *stagingTexture = textureMgr->getStagingTexture(
            width, height, 1u, 1u, d_texture->getPixelFormat() );

    stagingTexture->startMapRegion();
    Ogre::TextureBox box = stagingTexture->mapRegion( width, height, 1u, 1u, d_texture->getPixelFormat() );
    box.copyFrom( data, width, height, bytesPerRow );
    stagingTexture->stopMapRegion();

    stagingTexture->upload( box, d_texture, 0, nullptr, nullptr, true );

    textureMgr->removeStagingTexture( stagingTexture );
    OGRE_FREE_SIMD(data, Ogre::MEMCATEGORY_RENDERSYS);
    #if OGRE_VERSION_MINOR == 2
    d_texture->notifyDataIsReady();
    #endif
    #endif
#else
    d_texture = Ogre::TextureManager::getSingleton().createManual(
        getUniqueName(), "General", Ogre::TEX_TYPE_2D,
        sz.d_width, sz.d_height, 0,
        Ogre::PF_A8B8G8R8);
#endif // CEGUI_USE_OGRE_TEXTURE_GPU

    // throw exception if no texture was able to be created
    if (OGRE_ISNULL(d_texture))
        throw RendererException(
            "Failed to create Texture object with spcecified size.");

    d_size.d_width = static_cast<float>(d_texture->getWidth());
    d_size.d_height = static_cast<float>(d_texture->getHeight());
    d_dataSize = sz;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
OgreTexture::OgreTexture(const String& name, Ogre::TextureGpu* tex,
                         bool take_ownership) :
    d_texture(0),
#else
OgreTexture::OgreTexture(const String& name, Ogre::TexturePtr& tex,
                         bool take_ownership) :
#endif // CEGUI_USE_OGRE_TEXTURE_GPU
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
const glm::vec2& OgreTexture::getTexelScaling() const
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
        throw RendererException(
            "CEGUI::System object has not been created!");

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
        throw RendererException(
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'.");
}

//----------------------------------------------------------------------------//
void OgreTexture::loadFromMemory(const void* buffer, const Sizef& buffer_size,
                                 PixelFormat pixel_format)
{
    using namespace Ogre;

    if (!isPixelFormatSupported(pixel_format))
        throw InvalidRequestException(
            "Data was supplied in an unsupported pixel format.");

#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    Ogre::PixelFormatGpu ogrePixelFormat = toOgrePixelFormat(pixel_format);

    const std::uint32_t width = static_cast<std::uint32_t>(buffer_size.d_width);
    const std::uint32_t height = static_cast<std::uint32_t>(buffer_size.d_height);

    size_t bytesPerPixel = Ogre::PixelFormatGpuUtils::getBytesPerPixel(ogrePixelFormat);
    size_t bytesPerRow = bytesPerPixel * width;
    Ogre::TextureBox srcBox( width, height, 1u, 1u, bytesPerPixel, bytesPerRow, bytesPerRow * height );
    srcBox.data = const_cast<void*>( buffer );
    Ogre::TextureBox dstBox( width, height, 1u, 1u, 0, 0, 0 );

    if ( pixel_format == CEGUI::Texture::PixelFormat::Rgb )
    {
        // We need to expand the 24bpp RGB texture to 32bpp RGBA.
        // This is because OGRE 2.2 no longer supports RGB
        PixelFormatGpu ogrePixelFormatDst = toOgrePixelFormat(CEGUI::Texture::PixelFormat::Rgba);

        dstBox.bytesPerPixel = Ogre::PixelFormatGpuUtils::getBytesPerPixel(ogrePixelFormatDst);
        dstBox.bytesPerRow   = dstBox.bytesPerPixel * width;
        dstBox.bytesPerImage = dstBox.bytesPerRow * height;
        dstBox.data = new char[dstBox.bytesPerImage];

        Ogre::PixelFormatGpuUtils::bulkPixelConversion(
            srcBox, ogrePixelFormat,
            dstBox, ogrePixelFormatDst
        );
        ogrePixelFormat = ogrePixelFormatDst;
    }

    createOgreTexture(ogrePixelFormat, width, height);

    // build a staging texture that we'll use to upload into.
    Ogre::TextureGpuManager *textureMgr = Ogre::Root::getSingletonPtr()->getRenderSystem()->getTextureGpuManager();
    StagingTexture* stagingTexture = textureMgr->getStagingTexture(width, height, 1u, 1u, ogrePixelFormat);

    // map a region and copy data into it
    stagingTexture->startMapRegion();
    TextureBox box = stagingTexture->mapRegion(width, height, 1u, 1u, ogrePixelFormat);
    if ( pixel_format == CEGUI::Texture::PixelFormat::Rgb )
        box.copyFrom(dstBox);
    else
        box.copyFrom(srcBox);
    stagingTexture->stopMapRegion();

    // trigger upload to the gpu in the defined texture.
    stagingTexture->upload(box, d_texture, 0, nullptr, nullptr, true);

    if (dstBox.data)
        delete static_cast<char*>(dstBox.data);

    // remove the staging texture and notify that the texture is finished
    textureMgr->removeStagingTexture(stagingTexture);

    #if OGRE_VERSION_MINOR == 2
    d_texture->notifyDataIsReady();
    #endif
#else
    const size_t byte_size = calculateDataSize(buffer_size, pixel_format);

    char* bufferCopy = new char[byte_size];
    memcpy(bufferCopy, buffer, byte_size);

    const Ogre::PixelBox* pixelBox = new Ogre::PixelBox(static_cast<std::uint32_t>(buffer_size.d_width), static_cast<std::uint32_t>(buffer_size.d_height),
                                                        1, toOgrePixelFormat(pixel_format), bufferCopy);
    createEmptyOgreTexture(pixel_format);
    d_texture->freeInternalResources();
    d_texture->setWidth(static_cast<std::uint32_t>(buffer_size.d_width));
    d_texture->setHeight(static_cast<std::uint32_t>(buffer_size.d_height));
    d_texture->setDepth(1);
    d_texture->createInternalResources();
    d_texture->getBuffer(0,0).get()->blitFromMemory(*pixelBox);
#endif // CEGUI_USE_OGRE_TEXTURE_GPU

    // throw exception if no texture was able to be created
    if (OGRE_ISNULL(d_texture))
        throw RendererException(
            "Failed to blit to Texture from memory.");

    d_size.d_width = static_cast<float>(d_texture->getWidth());
    d_size.d_height = static_cast<float>(d_texture->getHeight());
    d_dataSize = buffer_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void OgreTexture::blitFromMemory(const void* sourceData, const Rectf& area)
{
    if (OGRE_ISNULL(d_texture)) // TODO: exception?
        return;

    // Ogre doesn't like null data, so skip if the sourceData is null and
    // area is zero size
    if (sourceData == nullptr)
    {
        if (static_cast<int>(area.getWidth()) == 0 &&
            static_cast<int>(area.getHeight()) == 0)
        {
            return;
        }

        // Here we are trying to write to a non-zero size area with null
        // ptr for data
        throw RendererException("blitFromMemory source is null");
    }


#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    Ogre::uint32 width = static_cast<Ogre::uint32>(area.getWidth());
    Ogre::uint32 height = static_cast<Ogre::uint32>(area.getHeight());

    size_t bytesPerPixel = Ogre::PixelFormatGpuUtils::getBytesPerPixel(d_texture->getPixelFormat());
    size_t bytesPerRow = bytesPerPixel * width;

    // acquire a staging texture
    Ogre::TextureGpuManager *textureMgr = Ogre::Root::getSingletonPtr()->getRenderSystem()->getTextureGpuManager();
    Ogre::StagingTexture *stagingTexture = textureMgr->getStagingTexture(
            width, height, 1u, 1u, d_texture->getPixelFormat());

    // setup the map region that we'll copy to
    stagingTexture->startMapRegion();
    Ogre::TextureBox box = stagingTexture->mapRegion( width, height, 1u, 1u, d_texture->getPixelFormat());
    box.copyFrom(const_cast<void*>( sourceData ), width, height, bytesPerRow);
    stagingTexture->stopMapRegion();

    // upload the data to the gpu
    // We specify a destination area based on the area rect provided as we may
    // only want to upload a portion to the GPU texture area and not the entire thing.

    Ogre::TextureBox dst;
    dst.x = static_cast<Ogre::uint32>( area.left() );
    dst.y = static_cast<Ogre::uint32>( area.top() );
    dst.width = width;
    dst.height = height;
    dst.depth = 1u;
    dst.numSlices = 1u;
    stagingTexture->upload( box, d_texture, 0, nullptr, &dst );

    // remove the staging texture and notify texture is prepared
    textureMgr->removeStagingTexture( stagingTexture );
#else
    // NOTE: const_cast because Ogre takes pointer to non-const here. Rather
    // than allow that to dictate poor choices in our own APIs, we choose to
    // address the issue as close to the source of the problem as possible.
    Ogre::PixelBox pb(static_cast<Ogre::uint32>(area.getWidth()),
                      static_cast<Ogre::uint32>(area.getHeight()),
                      1,
                      d_texture->getFormat(), const_cast<void*>(sourceData));

    Ogre::Box box(static_cast<Ogre::uint32>(area.left()),
			      static_cast<Ogre::uint32>(area.top()),
			      static_cast<Ogre::uint32>(area.right()),
			      static_cast<Ogre::uint32>(area.bottom()) );
    d_texture->getBuffer()->blitFromMemory(pb, box);
#endif // CEGUI_USE_OGRE_TEXTURE_GPU
}

//----------------------------------------------------------------------------//
void OgreTexture::blitToMemory(void* targetData)
{
    if (OGRE_ISNULL(d_texture)) // TODO: exception?
        return;

#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    throw RendererException("Unimplemented: blitToMemory for Ogre >= 2.2");
#else
    Ogre::PixelBox pb(static_cast<std::uint32_t>(d_size.d_width), static_cast<std::uint32_t>(d_size.d_height),
                      1, d_texture->getFormat(), targetData);
    d_texture->getBuffer()->blitToMemory(pb);
#endif // CEGUI_USE_OGRE_TEXTURE_GPU
}

//----------------------------------------------------------------------------//
void OgreTexture::freeOgreTexture()
{
    if (!OGRE_ISNULL(d_texture) && !d_isLinked) {
        #ifdef CEGUI_USE_OGRE_TEXTURE_GPU
        Ogre::Root::getSingletonPtr()->getRenderSystem()->getTextureGpuManager()->destroyTexture(d_texture);
        #else
        Ogre::TextureManager::getSingleton().remove(d_texture->getHandle());
        #endif // CEGUI_USE_OGRE_TEXTURE_GPU
    }

    #ifdef CEGUI_USE_OGRE_TEXTURE_GPU
    d_texture = 0;
    #else
    OGRE_RESET(d_texture);
    #endif // CEGUI_USE_OGRE_TEXTURE_GPU
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
    d_texelScaling.x = 1.0f / ((orgW == texW) ? orgW : texW);

    //
    // calculate what to use for y scale
    //
    const float orgH = d_dataSize.d_height;
    const float texH = d_size.d_height;

    // if texture and original data height are the same, scale is based
    // on the original size.
    // if texture is taller (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.y = 1.0f / ((orgH == texH) ? orgH : texH);
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
void OgreTexture::setOgreTexture(Ogre::TextureGpu* texture, bool take_ownership)
#else
void OgreTexture::setOgreTexture(Ogre::TexturePtr texture, bool take_ownership)
#endif // CEGUI_USE_OGRE_TEXTURE_GPU
{

    freeOgreTexture();

    d_texture = texture;
    d_isLinked = !take_ownership;

    if (!OGRE_ISNULL(d_texture))
    {
        d_size.d_width = static_cast<float>(d_texture->getWidth());
        d_size.d_height= static_cast<float>(d_texture->getHeight());
        d_dataSize = d_size;
    }
    else
        d_size = d_dataSize = Sizef(0, 0);

    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
Ogre::TextureGpu* OgreTexture::getOgreTexture() const
{
    return d_texture;
}
#else
Ogre::TexturePtr OgreTexture::getOgreTexture() const
{
    return d_texture;
}
#endif // CEGUI_USE_OGRE_TEXTURE_GPU

//----------------------------------------------------------------------------//
bool OgreTexture::textureIsLinked() const
{
    return d_isLinked;
}

//----------------------------------------------------------------------------//
bool OgreTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    try
    {
    #ifdef CEGUI_USE_OGRE_TEXTURE_GPU
        OgreTexture::toOgrePixelFormat(fmt);
        return true;
    #else
        return Ogre::TextureManager::getSingleton().
            isEquivalentFormatSupported(Ogre::TEX_TYPE_2D,
                                        toOgrePixelFormat(fmt),
                                        Ogre::TU_DEFAULT);
    #endif // CEGUI_USE_OGRE_TEXTURE_GPU
    }
    catch (InvalidRequestException&)
    {
        return false;
    }
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
Ogre::PixelFormatGpu OgreTexture::toOgrePixelFormat(const Texture::PixelFormat fmt)
{
    switch (fmt)
    {
        case Texture::PixelFormat::Rgba:      return Ogre::PFG_RGBA8_UNORM;
        case Texture::PixelFormat::Rgb:       return Ogre::PFG_RGB8_UNORM;
        case Texture::PixelFormat::Rgb565:    return Ogre::PFG_B5G6R5_UNORM;
        case Texture::PixelFormat::Rgba4444:  return Ogre::PFG_B4G4R4A4_UNORM;
        case Texture::PixelFormat::Pvrtc2:    return Ogre::PFG_PVRTC_RGBA2;
        case Texture::PixelFormat::Pvrtc4:    return Ogre::PFG_PVRTC_RGBA4;
        case Texture::PixelFormat::RgbaDxt1:  return Ogre::PFG_BC1_UNORM;
        case Texture::PixelFormat::RgbaDxt3:  return Ogre::PFG_BC2_UNORM;
        case Texture::PixelFormat::RgbaDxt5:  return Ogre::PFG_BC3_UNORM;

        default:
            throw InvalidRequestException(
                "Invalid pixel format translation.");
    }
}

//----------------------------------------------------------------------------//
Texture::PixelFormat OgreTexture::fromOgrePixelFormat(
                                            const Ogre::PixelFormatGpu fmt)
{
    switch (fmt)
    {
        case Ogre::PFG_RGBA8_UNORM:       return Texture::PixelFormat::Rgba;
        case Ogre::PFG_RGBA8_UNORM_SRGB:  return Texture::PixelFormat::Rgba;
        case Ogre::PFG_BGRA8_UNORM:       return Texture::PixelFormat::Rgba;
        case Ogre::PFG_BGRA8_UNORM_SRGB:  return Texture::PixelFormat::Rgba;
        case Ogre::PFG_RGB8_UNORM:        return Texture::PixelFormat::Rgb;
        case Ogre::PFG_RGB8_UNORM_SRGB:   return Texture::PixelFormat::Rgb;
        case Ogre::PFG_BGR8_UNORM:        return Texture::PixelFormat::Rgb;
        case Ogre::PFG_BGR8_UNORM_SRGB:   return Texture::PixelFormat::Rgb;
        case Ogre::PFG_B5G6R5_UNORM:      return Texture::PixelFormat::Rgb565;
        case Ogre::PFG_B4G4R4A4_UNORM:    return Texture::PixelFormat::Rgba4444;
        case Ogre::PFG_PVRTC_RGBA2:       return Texture::PixelFormat::Pvrtc2;
        case Ogre::PFG_PVRTC_RGBA4:       return Texture::PixelFormat::Pvrtc4;
        case Ogre::PFG_BC1_UNORM:         return Texture::PixelFormat::RgbaDxt1;
        case Ogre::PFG_BC1_UNORM_SRGB:    return Texture::PixelFormat::RgbaDxt1;
        case Ogre::PFG_BC2_UNORM:         return Texture::PixelFormat::RgbaDxt3;
        case Ogre::PFG_BC2_UNORM_SRGB:    return Texture::PixelFormat::RgbaDxt3;
        case Ogre::PFG_BC3_UNORM:         return Texture::PixelFormat::RgbaDxt5;
        case Ogre::PFG_BC3_UNORM_SRGB:    return Texture::PixelFormat::RgbaDxt5;

        default:
            throw InvalidRequestException(
                "Invalid pixel format translation.");
    }
}
#else
Ogre::PixelFormat OgreTexture::toOgrePixelFormat(const Texture::PixelFormat fmt)
{
    switch (fmt)
    {
        case Texture::PixelFormat::Rgba:       return Ogre::PF_A8B8G8R8;
        case Texture::PixelFormat::Rgb:        return Ogre::PF_B8G8R8;
        case Texture::PixelFormat::Rgb565:    return Ogre::PF_R5G6B5;
        case Texture::PixelFormat::Rgba4444:  return Ogre::PF_A4R4G4B4;
        case Texture::PixelFormat::Pvrtc2:     return Ogre::PF_PVRTC_RGBA2;
        case Texture::PixelFormat::Pvrtc4:     return Ogre::PF_PVRTC_RGBA4;
        case Texture::PixelFormat::RgbaDxt1:  return Ogre::PF_DXT1;
        case Texture::PixelFormat::RgbaDxt3:  return Ogre::PF_DXT3;
        case Texture::PixelFormat::RgbaDxt5:  return Ogre::PF_DXT5;

        default:
            throw InvalidRequestException(
                "Invalid pixel format translation.");
    }
}

//----------------------------------------------------------------------------//
Texture::PixelFormat OgreTexture::fromOgrePixelFormat(
                                            const Ogre::PixelFormat fmt)
{
    switch (fmt)
    {
        case Ogre::PF_A8R8G8B8:     return Texture::PixelFormat::Rgba;
        case Ogre::PF_A8B8G8R8:     return Texture::PixelFormat::Rgba;
        case Ogre::PF_R8G8B8:       return Texture::PixelFormat::Rgb;
        case Ogre::PF_B8G8R8:       return Texture::PixelFormat::Rgb;
        case Ogre::PF_R5G6B5:       return Texture::PixelFormat::Rgb565;
        case Ogre::PF_A4R4G4B4:     return Texture::PixelFormat::Rgba4444;
        case Ogre::PF_PVRTC_RGBA2:  return Texture::PixelFormat::Pvrtc2;
        case Ogre::PF_PVRTC_RGBA4:  return Texture::PixelFormat::Pvrtc4;
        case Ogre::PF_DXT1:         return Texture::PixelFormat::RgbaDxt1;
        case Ogre::PF_DXT3:         return Texture::PixelFormat::RgbaDxt3;
        case Ogre::PF_DXT5:         return Texture::PixelFormat::RgbaDxt5;

        default:
            throw InvalidRequestException(
                "Invalid pixel format translation.");
    }
}
#endif // CEGUI_USE_OGRE_TEXTURE_GPU

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_TEXTURE_GPU
void OgreTexture::createOgreTexture(
    Ogre::PixelFormatGpu pixel_format,
    std::uint32_t width, std::uint32_t height,
    Ogre::GpuPageOutStrategy::GpuPageOutStrategy pageOutStrategy
) {
    Ogre::TextureGpuManager *textureMgr = Ogre::Root::getSingletonPtr()->getRenderSystem()->getTextureGpuManager();
    d_texture = textureMgr->createOrRetrieveTexture(
                                    getUniqueName(),
                                    pageOutStrategy,
                                    Ogre::TextureFlags::ManualTexture,
                                    Ogre::TextureTypes::Type2D,
                                    Ogre::BLANKSTRING,
                                    0u );
    d_texture->setPixelFormat(pixel_format);
    d_texture->setNumMipmaps(1u);
    d_texture->setResolution(width, height);

    // schedule texture to be transitioned to the gpu
    d_texture->_transitionTo( Ogre::GpuResidency::Resident, nullptr );
    d_texture->_setNextResidencyStatus( Ogre::GpuResidency::Resident );
}
#else
void OgreTexture::createEmptyOgreTexture(PixelFormat pixel_format)
{
    // try to create a Ogre::Texture with given dimensions
    d_texture = Ogre::TextureManager::getSingleton().createManual(
        getUniqueName(), "General", Ogre::TEX_TYPE_2D,
        1, 1, 0,
        toOgrePixelFormat(pixel_format));
}
#endif // CEGUI_USE_OGRE_TEXTURE_GPU


//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
