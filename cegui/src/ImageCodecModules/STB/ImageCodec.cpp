/***********************************************************************
	created:	Fri Apr 30 2010
	author:		Tobias Schlegel

	purpose:	This codec provides stb_image.c based image loading
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodecModules/STB/ImageCodec.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Size.h"
#define STBI_HEADER_FILE_ONLY
#include "stb_image.cpp"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
STBImageCodec::STBImageCodec()
    : ImageCodec("STBImageCodec - stb_image.c based image codec")
{
    d_supportedFormat = "tga jpg png psd bmp hdr";
}

//----------------------------------------------------------------------------//
STBImageCodec::~STBImageCodec()
{
}

//----------------------------------------------------------------------------//
Texture* STBImageCodec::load(const RawDataContainer& data, Texture* result)
{
    int width, height, comp;

    // load image
    unsigned char* image = stbi_load_from_memory(data.getDataPtr(),
                                                 data.getSize(),
                                                 &width, &height, &comp, 0);

    if (!image) 
    {
        Logger::getSingletonPtr()->logEvent(
            "STBImageCodec::load - Invalid image data", Errors);

        return 0;
    }

    Texture::PixelFormat format;
    switch (comp) 
    {
    case 4:
        format = Texture::PF_RGBA;
        break;
    case 3:
        format = Texture::PF_RGB;
        break;
    default:
        Logger::getSingletonPtr()->logEvent(
            "STBImageCodec::load - Invalid image format. "
            "Only RGB and RGBA images are supported", Errors);

        stbi_image_free(image);
        return 0;
    }

    result->loadFromMemory(image,
                           Sizef(static_cast<float>(width),
                                  static_cast<float>(height)),
                           format);

    // delete temporary image data
    stbi_image_free(image);

    return result;
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace section

