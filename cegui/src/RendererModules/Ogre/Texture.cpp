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
#ifdef CEGUI_OGRE_NEXT
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include "CEGUI/RendererModules/Ogre/OgreMacros.h"
#include "CEGUI/RendererModules/Ogre/ResourceProvider.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/Rectf.h"

#include <OgreTextureGpuManager.h>
#include <OgreTextureGpu.h>
#include <OgreStagingTexture.h>
#include <OgrePixelFormatGpuUtils.h>
#include <OgreRoot.h>
#include <OgreStringConverter.h>

#include <cstdint>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//

//! For Debug only, to write test colours in textureData
static void writeColorPatternToDataRGBA(Ogre::uint8* imageData, size_t dataSize, size_t bytesPerRow)
{
	memset(imageData, 50, dataSize);

	int numberOfComtonents = 4;  //RGBA
	int totalRows = dataSize / bytesPerRow;
	int component = 0;
	int currentRow = 0;
	int bufferIndexRow = 0;
	for(Ogre::uint32 bufferIndex = 0; bufferIndex < dataSize; ++bufferIndex)
	{
		float colour_red_percentage = (float)currentRow / (float)totalRows;
		float colour_green_percentage = 0.0f;
		float colour_blue_percentage = (float)bufferIndexRow / (float)bytesPerRow;
		Ogre::uint8 colour_red = std::min(255u, (Ogre::uint32)(colour_red_percentage * 255.0f));
		Ogre::uint8 colour_green = std::min(255u, (Ogre::uint32)(colour_green_percentage * 255.0f));
		Ogre::uint8 colour_blue = std::min(255u, (Ogre::uint32)(colour_blue_percentage * 255.0f));

		if(component == 0) //red
			imageData[bufferIndex] = colour_red;
		else if(component == 1) //green
			imageData[bufferIndex] = colour_green;
		else if(component == 2) //blue
			imageData[bufferIndex] = colour_blue;
		else //alpha
			imageData[bufferIndex] = 255;

		component++;
		if(component >= numberOfComtonents)
			component = 0;

		bufferIndexRow++;
		if(bufferIndexRow >= bytesPerRow)
		{
			currentRow++;
			bufferIndexRow = 0;
		}
	}
}

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

// helper function to return byte size of one pixel in given format
static size_t calculateBytesPerPixel(Ogre::PixelFormatGpu pixelFormat)
{
	switch(pixelFormat)
	{
		case Ogre::PFG_RGBA8_UNORM:				return 4u;
		
		//Note: other formats are not supported by this class

		//case Ogre::PFG_BC1_UNORM_SRGB:         return ;
		//case Ogre::PFG_BC2_UNORM_SRGB:         return ;
		//case Ogre::PFG_BC3_UNORM_SRGB:         return ;

	default:
		throw InvalidRequestException(
			"Invalid pixel format (calculateBytesPerPixel)");
	}
}

//----------------------------------------------------------------------------//
std::uint32_t OgreTexture::d_textureNumber = 0;

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name),
	d_nameFile(),
    d_texture(0)
{
    createEmptyOgreTexture(toOgrePixelFormat(CEGUI::Texture::PixelFormat::Rgba));
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const String& filename,
                         const String& resourceGroup) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name),
	d_nameFile(filename),
    d_texture(0)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const Sizef& sz) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name),
	d_nameFile(),
    d_texture(0)
{
 	createOgreTexture(toOgrePixelFormat(CEGUI::Texture::PixelFormat::Rgba), sz.d_width, sz.d_height);
    
	d_size.d_width = static_cast<float>(d_texture->getWidth());
    d_size.d_height = static_cast<float>(d_texture->getHeight());
    d_dataSize = sz;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, Ogre::TextureGpu* textureGpu,
                         bool take_ownership) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name),
	d_nameFile(),
    d_texture(0)
{
    setOgreTexture(textureGpu, take_ownership);
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

    CEGUI::ImageCodec& ic(sys->getImageCodec());

    // if we're using the integrated Ogre codec, set the file-type hint string
    if (ic.getIdentifierString().substr(0, 14)  == "OgreImageCodec")
    {
        String type;
        String::size_type i = filename.find_last_of(".");
        if (i != String::npos && filename.length() - i > 1)
            type = filename.substr(i+1);
        static_cast<OgreImageCodec&>(ic).setImageFileDataType(type);
    }

    CEGUI::Texture* res = sys->getImageCodec().load(texFile, this);

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
    if(!isPixelFormatSupported(pixel_format))
        throw InvalidRequestException(
            "Data was supplied in an unsupported pixel format.");

    Ogre::PixelFormatGpu ogrePixelFormat = toOgrePixelFormat(pixel_format);

    const std::uint32_t width = static_cast<std::uint32_t>(buffer_size.d_width);
    const std::uint32_t height = static_cast<std::uint32_t>(buffer_size.d_height);

    size_t bytesPerPixel = Ogre::PixelFormatGpuUtils::getBytesPerPixel(ogrePixelFormat);
    size_t bytesPerRow = bytesPerPixel * width;
    Ogre::TextureBox srcBox(width, height, 1u, 1u, bytesPerPixel, bytesPerRow, bytesPerRow * height);
    srcBox.data = const_cast<void*>(buffer);

    Ogre::TextureBox textureBoxRgb(width, height, 1u, 1u, 0, 0, 0);
    if(pixel_format == CEGUI::Texture::PixelFormat::Rgb)
    {
        // We need to expand the 24bpp RGB texture to 32bpp RGBA.
        // This is because OGRE 2.2 no longer supports RGB
        Ogre::PixelFormatGpu ogrePixelFormatDst = toOgrePixelFormat(CEGUI::Texture::PixelFormat::Rgba);

        textureBoxRgb.bytesPerPixel = Ogre::PixelFormatGpuUtils::getBytesPerPixel(ogrePixelFormatDst);
        textureBoxRgb.bytesPerRow = textureBoxRgb.bytesPerPixel * width;
        textureBoxRgb.bytesPerImage = textureBoxRgb.bytesPerRow * height;
        textureBoxRgb.data = new char[textureBoxRgb.bytesPerImage];

        Ogre::PixelFormatGpuUtils::bulkPixelConversion(
            srcBox, ogrePixelFormat,
            textureBoxRgb, ogrePixelFormatDst
        );
        ogrePixelFormat = ogrePixelFormatDst;
    }

    createOgreTexture(ogrePixelFormat, width, height);

    // build a staging texture that we'll use to upload into.
    Ogre::TextureGpuManager *textureMgr = Ogre::Root::getSingletonPtr()->getRenderSystem()->getTextureGpuManager();
    Ogre::StagingTexture* stagingTexture = textureMgr->getStagingTexture(width, height, 1u, 1u, ogrePixelFormat);

    // map a region and copy data into it
    stagingTexture->startMapRegion();
    Ogre::TextureBox box = stagingTexture->mapRegion(width, height, 1u, 1u, ogrePixelFormat);
    if(pixel_format == CEGUI::Texture::PixelFormat::Rgb)
        box.copyFrom(textureBoxRgb);
    else
        box.copyFrom(srcBox);
    stagingTexture->stopMapRegion();

    // trigger upload to the gpu in the defined texture.
    stagingTexture->upload(box, d_texture, 0, nullptr, nullptr, true);

    if(textureBoxRgb.data)
        delete static_cast<char*>(textureBoxRgb.data);

    // remove the staging texture and notify that the texture is finished
    textureMgr->removeStagingTexture(stagingTexture);

#if OGRE_VERSION >= 0x020200 && OGRE_VERSION < 0x020300
    texture->notifyDataIsReady(); // Must NOT call in Ogre 2.3 or later
#endif

    // throw exception if no texture was able to be created
    if(OGRE_ISNULL(d_texture))
        throw RendererException(
            "Failed to blit to Texture from memory.");

    d_size.d_width = static_cast<float>(d_texture->getWidth());
    d_size.d_height = static_cast<float>(d_texture->getHeight());
    d_dataSize = buffer_size;
    updateCachedScaleValues();
}


void OgreTexture::blitFromMemory(const void* sourceData, const Rectf& area)
{
    if(d_isLinked) //do not write to linked textures
        return;

    //this is used for text textures

    /*Copies a region from normal memory to a region of this TextureGpu.
        The source image can be in any size.
        @param sourceData MemoryData of source pixels  Format must be like the TextureGpu
        @param area  Box describing the destination region in this TextureGpu
        @remarks The source and destination regions dimensions don't have to match, in which
        case scaling is done.This scaling is generally done using a bilinear filter in hardware.*/

    if(!d_texture) // TODO: exception?
        return;

    // Ogre doesn't like null data, so skip if the sourceData is null and
    // area is zero size
    if(sourceData == nullptr)
    {
        if(static_cast<int>(area.getWidth()) == 0 &&
            static_cast<int>(area.getHeight()) == 0)
        {
            return;
        }

        // Here we are trying to write to a non-zero size area with null
        // ptr for data
        throw RendererException("blitFromMemory source is null");
    }

    //Uploading data to a TextureGpu
    Ogre::TextureGpu* texture = d_texture;
    Ogre::TextureGpuManager* textureManager = Ogre::Root::getSingleton().getRenderSystem()->getTextureGpuManager();


    Ogre::uint32 width = static_cast<Ogre::uint32>(area.getWidth());
    Ogre::uint32 height = static_cast<Ogre::uint32>(area.getHeight());
    size_t bytesPerPixel = Ogre::PixelFormatGpuUtils::getBytesPerPixel(texture->getPixelFormat());
    size_t bytesPerRow = bytesPerPixel * width;
               

    Ogre::uint8* imageData = 0; //imageData is not needed, but left here to match the Ogre Examples
    //-----------------------start upload to TextureGpu----------------------------------------

    //We have to upload the data via a StagingTexture, which acts as an intermediate stash
    //memory that is both visible to CPU and GPU.
    Ogre::StagingTexture *stagingTexture = textureManager->getStagingTexture(
        width,
        height,
        texture->getDepth(),
        texture->getNumSlices(),
        texture->getPixelFormat());

    //Call this function to indicate you're going to start calling mapRegion. startMapRegion
    //must be called from main thread.
    stagingTexture->startMapRegion();
    //Map region of the staging texture. This function can be called from any thread after
    //startMapRegion has already been called.
    Ogre::TextureBox texBox = stagingTexture->mapRegion(width, height,
        texture->getDepth(), texture->getNumSlices(),
        texture->getPixelFormat());
    //Fill data start------------------------------------------------------------------------
    
    texBox.copyFrom(const_cast<void*>(sourceData), width, height, bytesPerRow);

    // upload the data to the gpu
    // We specify a destination area based on the area rect provided as we may
    // only want to upload a portion to the GPU texture area and not the entire thing.
    Ogre::TextureBox dst;
    dst.x = static_cast<Ogre::uint32>(area.left());
    dst.y = static_cast<Ogre::uint32>(area.top());
    dst.width = width;
    dst.height = height;

    //Fill data end------------------------------------------------------------------------	
    //stopMapRegion indicates you're done calling mapRegion. Call this from the main thread.
    //It is your responsability to ensure you're done using all pointers returned from
    //previous mapRegion calls, and that you won't call it again.
    //You cannot upload until you've called this function.
    //Do NOT call startMapRegion again until you're done with upload() calls.
    stagingTexture->stopMapRegion();
    //Upload an area of the staging texture into the texture. Must be done from main thread.
    //The last bool parameter, 'skipSysRamCopy', is only relevant for AlwaysKeepSystemRamCopy
    //textures, and we set it to true because we know it's already up to date. Otherwise
    //it needs to be false.
    stagingTexture->upload(texBox, texture, 0, 0, &dst, false);
    //Tell the TextureGpuManager we're done with this StagingTexture. Otherwise it will leak.
    textureManager->removeStagingTexture(stagingTexture);
    stagingTexture = 0;

    //Do not free the pointer if texture's paging strategy is GpuPageOutStrategy::AlwaysKeepSystemRamCopy
    //OGRE_FREE_SIMD(imageData, Ogre::MEMCATEGORY_RESOURCE);
    //imageData = 0;

#if OGRE_VERSION >= 0x020200 && OGRE_VERSION < 0x020300
    texture->notifyDataIsReady(); // Must NOT call in Ogre 2.3 or later
#endif
}
//----------------------------------------------------------------------------//
void OgreTexture::blitToMemory(void* targetData)
{
	//Not implemented
}

//----------------------------------------------------------------------------//
void OgreTexture::freeOgreTexture()
{
   if(d_texture && !d_isLinked)
	{
		Ogre::TextureGpuManager* textureGpuManager = Ogre::Root::getSingleton().getRenderSystem()->getTextureGpuManager();
		textureGpuManager->destroyTexture(d_texture);
		d_texture = 0;		
	}
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
        Ogre::String name("_cegui_ogre_" + Ogre::StringConverter::toString(d_textureNumber++));
        return name;
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
void OgreTexture::setOgreTexture(Ogre::TextureGpu* textureGpu, bool take_ownership)
{
    freeOgreTexture();

    d_texture = textureGpu;
    d_isLinked = !take_ownership;

    if (d_texture)
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
Ogre::TextureGpu* OgreTexture::getOgreTexture() const
{
    return d_texture;
}

//----------------------------------------------------------------------------//
bool OgreTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    try
    {
		return Ogre::Root::getSingleton().getRenderSystem()->getTextureGpuManager()->checkSupport(toOgrePixelFormat(fmt),
			Ogre::TextureTypes::Type2D, Ogre::TextureFlags::ManualTexture);
    }
    catch (InvalidRequestException&)
    {
        return false;
    }
}

//----------------------------------------------------------------------------//
Ogre::PixelFormatGpu OgreTexture::toOgrePixelFormat(const Texture::PixelFormat fmt)
{
    switch (fmt)
    {
        case Texture::PixelFormat::Rgba:        return Ogre::PFG_RGBA8_UNORM;
        case Texture::PixelFormat::Rgb:       return Ogre::PFG_RGB8_UNORM;
        case Texture::PixelFormat::Rgb565:      return Ogre::PFG_B5G6R5_UNORM;
        case Texture::PixelFormat::Rgba4444:    return Ogre::PFG_B4G4R4A4_UNORM;
        case Texture::PixelFormat::Pvrtc2:      return Ogre::PFG_PVRTC_RGBA2;
        case Texture::PixelFormat::Pvrtc4:      return Ogre::PFG_PVRTC_RGBA4;
        case Texture::PixelFormat::RgbaDxt1:    return Ogre::PFG_BC1_UNORM;
        case Texture::PixelFormat::RgbaDxt3:    return Ogre::PFG_BC2_UNORM;
        case Texture::PixelFormat::RgbaDxt5:    return Ogre::PFG_BC3_UNORM;

        default:
            throw InvalidRequestException(
                "Invalid pixel format translation.");
    }
}

//----------------------------------------------------------------------------//
Texture::PixelFormat OgreTexture::fromOgrePixelFormat(const Ogre::PixelFormatGpu fmt)
{
    switch (fmt)
    {
		case Ogre::PFG_RGBA8_UNORM:		  return Texture::PixelFormat::Rgba;
        case Ogre::PFG_BGRA8_UNORM:     return Texture::PixelFormat::Rgba;
        case Ogre::PFG_B5G6R5_UNORM:      return Texture::PixelFormat::Rgb565;
        case Ogre::PFG_B4G4R4A4_UNORM:    return Texture::PixelFormat::Rgba4444;
        case Ogre::PFG_PVRTC_RGBA2:       return Texture::PixelFormat::Pvrtc2;
        case Ogre::PFG_PVRTC_RGBA4:       return Texture::PixelFormat::Pvrtc4;
        case Ogre::PFG_BC1_UNORM:         return Texture::PixelFormat::RgbaDxt1;
        case Ogre::PFG_BC2_UNORM:         return Texture::PixelFormat::RgbaDxt3;
        case Ogre::PFG_BC3_UNORM:         return Texture::PixelFormat::RgbaDxt5;

        default:
            throw InvalidRequestException(
                "Invalid pixel format translation.");
    }
}

//----------------------------------------------------------------------------//
void OgreTexture::createEmptyOgreTexture(Ogre::PixelFormatGpu pixel_format)
{
	createOgreTexture(pixel_format, 1, 1);
}

void OgreTexture::createOgreTexture(Ogre::PixelFormatGpu pixel_format, Ogre::uint32 width, Ogre::uint32 height)
{
	freeOgreTexture();
		
	Ogre::uint32 textureFlags = Ogre::TextureFlags::ManualTexture;
	Ogre::GpuPageOutStrategy::GpuPageOutStrategy gpuPageOutStrategy = Ogre::GpuPageOutStrategy::Discard;

	d_texture = Ogre::Root::getSingleton().getRenderSystem()->getTextureGpuManager()->createOrRetrieveTexture(
		getUniqueName(),
		gpuPageOutStrategy,
		textureFlags,
		Ogre::TextureTypes::Type2D,
		"General"
	);

	// throw exception if no texture was able to be created
	if(!d_texture)
		throw RendererException(
			"Failed to create Texture object");

	d_texture->setPixelFormat(pixel_format);
	//d_texture->setTextureType(Ogre::TextureTypes::Type2D);
	d_texture->setNumMipmaps(1u);
	d_texture->setResolution(width, height);

    // schedule texture to be transitioned to the gpu
    if(d_texture->getNextResidencyStatus() != Ogre::GpuResidency::Resident)
    {
        d_texture->_transitionTo(Ogre::GpuResidency::Resident, nullptr);
        d_texture->_setNextResidencyStatus(Ogre::GpuResidency::Resident);
    }
}


//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
#else	//CEGUI_OGRE_NEXT
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include "CEGUI/RendererModules/Ogre/OgreMacros.h"
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>

#include <cstdint>

// Start of CEGUI namespace section
namespace CEGUI
{
	//----------------------------------------------------------------------------//
	// helper function to return byte size of image of given size in given format
	static size_t calculateDataSize(const Sizef size, Texture::PixelFormat fmt)
	{
		switch(fmt)
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
			return static_cast<size_t>(std::ceil(size.d_width / 4) * std::ceil(size.d_height / 4) * 8);

		case Texture::PixelFormat::RgbaDxt3:
		case Texture::PixelFormat::RgbaDxt5:
			return static_cast<size_t>(std::ceil(size.d_width / 4) * std::ceil(size.d_height / 4) * 16);

		default:
			return 0;
		}
	}

	//----------------------------------------------------------------------------//
	std::uint32_t OgreTexture::d_textureNumber = 0;

	//----------------------------------------------------------------------------//
	OgreTexture::OgreTexture(const String& name) :
		d_isLinked(false),
		d_size(0, 0),
		d_dataSize(0, 0),
		d_texelScaling(0, 0),
		d_name(name)
	{
		createEmptyOgreTexture(Texture::PixelFormat::Rgba);
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
		d_texture = Ogre::TextureManager::getSingleton().createManual(
			getUniqueName(), "General", Ogre::TEX_TYPE_2D,
			sz.d_width, sz.d_height, 0,
			Ogre::PF_A8B8G8R8);

		// throw exception if no texture was able to be created
		if(OGRE_ISNULL(d_texture))
			throw RendererException(
				"Failed to create Texture object with spcecified size.");

		d_size.d_width = static_cast<float>(d_texture->getWidth());
		d_size.d_height = static_cast<float>(d_texture->getHeight());
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
		if(!sys)
			throw RendererException(
				"CEGUI::System object has not been created!");

		// load file to memory via resource provider
		RawDataContainer texFile;
		sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
			resourceGroup);

		ImageCodec& ic(sys->getImageCodec());

		// if we're using the integrated Ogre codec, set the file-type hint string
		if(ic.getIdentifierString().substr(0, 14) == "OgreImageCodec")
		{
			String type;
			String::size_type i = filename.find_last_of(".");
			if(i != String::npos && filename.length() - i > 1)
				type = filename.substr(i + 1);
			static_cast<OgreImageCodec&>(ic).setImageFileDataType(type);
		}

		Texture* res = sys->getImageCodec().load(texFile, this);

		// unload file data buffer
		sys->getResourceProvider()->unloadRawDataContainer(texFile);

		// throw exception if data was load loaded to texture.
		if(!res)
			throw RendererException(
				sys->getImageCodec().getIdentifierString() +
				" failed to load image '" + filename + "'.");
	}

	//----------------------------------------------------------------------------//
	void OgreTexture::loadFromMemory(const void* buffer, const Sizef& buffer_size,
		PixelFormat pixel_format)
	{
		using namespace Ogre;

		if(!isPixelFormatSupported(pixel_format))
			throw InvalidRequestException(
				"Data was supplied in an unsupported pixel format.");

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
		d_texture->getBuffer(0, 0).get()->blitFromMemory(*pixelBox);

		// throw exception if no texture was able to be created
		if(OGRE_ISNULL(d_texture))
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
		if(OGRE_ISNULL(d_texture)) // TODO: exception?
			return;

		// Ogre doesn't like null data, so skip if the sourceData is null and
		// area is zero size
		if(sourceData == nullptr)
		{
			if(static_cast<int>(area.getWidth()) == 0 &&
				static_cast<int>(area.getHeight()) == 0)
			{
				return;
			}

			// Here we are trying to write to a non-zero size area with null
			// ptr for data
			throw RendererException("blitFromMemory source is null");
		}


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
			static_cast<Ogre::uint32>(area.bottom()));
		d_texture->getBuffer()->blitFromMemory(pb, box);
	}

	//----------------------------------------------------------------------------//
	void OgreTexture::blitToMemory(void* targetData)
	{
		if(OGRE_ISNULL(d_texture)) // TODO: exception?
			return;

		Ogre::PixelBox pb(static_cast<std::uint32_t>(d_size.d_width), static_cast<std::uint32_t>(d_size.d_height),
			1, d_texture->getFormat(), targetData);
		d_texture->getBuffer()->blitToMemory(pb);
	}

	//----------------------------------------------------------------------------//
	void OgreTexture::freeOgreTexture()
	{
		if(!OGRE_ISNULL(d_texture) && !d_isLinked)
			Ogre::TextureManager::getSingleton().remove(d_texture->getHandle());

		OGRE_RESET(d_texture);
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
	void OgreTexture::setOgreTexture(Ogre::TexturePtr texture, bool take_ownership)
	{
		freeOgreTexture();

		d_texture = texture;
		d_isLinked = !take_ownership;

		if(!OGRE_ISNULL(d_texture))
		{
			d_size.d_width = static_cast<float>(d_texture->getWidth());
			d_size.d_height = static_cast<float>(d_texture->getHeight());
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
		try
		{
			return Ogre::TextureManager::getSingleton().
				isEquivalentFormatSupported(Ogre::TEX_TYPE_2D,
					toOgrePixelFormat(fmt),
					Ogre::TU_DEFAULT);
		}
		catch(InvalidRequestException&)
		{
			return false;
		}
	}

	//----------------------------------------------------------------------------//
	Ogre::PixelFormat OgreTexture::toOgrePixelFormat(const Texture::PixelFormat fmt)
	{
		switch(fmt)
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
		switch(fmt)
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

	//----------------------------------------------------------------------------//
	void OgreTexture::createEmptyOgreTexture(PixelFormat pixel_format)
	{
		// try to create a Ogre::Texture with given dimensions
		d_texture = Ogre::TextureManager::getSingleton().createManual(
			getUniqueName(), "General", Ogre::TEX_TYPE_2D,
			1, 1, 0,
			toOgrePixelFormat(pixel_format));
	}


	//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

#endif	//CEGUI_OGRE_NEXT
