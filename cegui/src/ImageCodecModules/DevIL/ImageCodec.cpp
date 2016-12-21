/***********************************************************************
	created:	07/06/2006
	author:		Olivier Delannoy 
	
	purpose:	This codec provide DevIL based image loading 
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/ImageCodecModules/DevIL/ImageCodec.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/Exceptions.h"
#include <IL/il.h>
#include <string.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// prototypes for internal helper functions
std::uint8_t* getCompressedPixelData(ILenum dxtc_fmt);

//----------------------------------------------------------------------------//
DevILImageCodec::DevILImageCodec()
    : ImageCodec("DevILImageCodec - Official DevIL based image codec")
{
    ilInit();
}

//----------------------------------------------------------------------------//
DevILImageCodec::~DevILImageCodec()
{    
}

//----------------------------------------------------------------------------//
Texture* DevILImageCodec::load(const RawDataContainer& data, Texture* result)
{
    ilPushAttrib(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
    ilEnable(IL_ORIGIN_SET);

    ILuint imgName;
    ilGenImages(1, &imgName);
    ilBindImage(imgName);

    ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);

    if (IL_FALSE != ilLoadL(IL_TYPE_UNKNOWN,
                            static_cast<const void*>(data.getDataPtr()),
                            data.getSize()))
    {
        const size_t width = ilGetInteger(IL_IMAGE_WIDTH);
        const size_t height = ilGetInteger(IL_IMAGE_HEIGHT);

        Texture::PixelFormat cefmt;
        std::uint8_t* pixel_data;

        switch (const ILenum dxtc_fmt = ilGetInteger(IL_DXTC_DATA_FORMAT))
        {
        case IL_DXT1:
            pixel_data = getCompressedPixelData(dxtc_fmt);
            cefmt = Texture::PixelFormat::RgbaDxt1;
            break;

        case IL_DXT3:
            pixel_data = getCompressedPixelData(dxtc_fmt);
            cefmt = Texture::PixelFormat::RgbaDxt3;
            break;

        case IL_DXT5:
            pixel_data = getCompressedPixelData(dxtc_fmt);
            cefmt = Texture::PixelFormat::RgbaDxt5;
            break;

        case IL_DXT_NO_COMP:
            // get image data in required format
            ILenum ilfmt;
            switch (ilGetInteger(IL_IMAGE_FORMAT))
            {
            case IL_RGBA:
            case IL_BGRA:
                ilfmt = IL_RGBA;
                cefmt = Texture::PixelFormat::Rgba;
                break;

            default:
                ilfmt = IL_RGB;
                cefmt = Texture::PixelFormat::Rgb;
                break;
            };

            // allocate temp buffer to receive image data
            pixel_data = new std::uint8_t[width * height * 4];
            ilCopyPixels(0, 0, 0, width, height, 1, ilfmt, IL_UNSIGNED_BYTE,
                         static_cast<void*>(pixel_data));
            break;

        default:
            throw InvalidRequestException(
                "Unsupported DXTC data format returned.");
        }

        // delete DevIL image
        ilDeleteImages(1, &imgName);
        ilPopAttrib();

        // create cegui texture
        try
        {
            result->loadFromMemory(pixel_data, Sizef(width, height), cefmt);
        }
        catch (...)
        {
            delete[] pixel_data;
            throw;
        }

        // free temp buffer
        delete[] pixel_data;

        return result;
    }
	// failed to load image properly.
	else
	{
		// delete DevIL image
		ilDeleteImages(1, &imgName);
		ilPopAttrib();
        return 0;
    }
}

//----------------------------------------------------------------------------//
std::uint8_t* getCompressedPixelData(ILenum dxtc_fmt)
{
    ILuint data_size = ilGetDXTCData(0, 0, dxtc_fmt);
    std::uint8_t* pixel_data = new std::uint8_t[data_size];
    ilGetDXTCData(pixel_data, data_size, dxtc_fmt);

    return pixel_data;
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace section 
  
