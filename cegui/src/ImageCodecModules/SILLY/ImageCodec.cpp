/***********************************************************************
	created:	Thu Jun 15 2006
	author:		Tomas Lindquist Olsen

	purpose:	This codec provides SILLY based image loading
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/ImageCodecModules/SILLY/ImageCodec.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include <SILLY.h>

namespace CEGUI
{
SILLYImageCodec::SILLYImageCodec()
    : ImageCodec("SILLYImageCodec - Official SILLY based image codec")
{
    d_supportedFormat = "tga jpg png";
    if (! SILLY::SILLYInit())
        throw GenericException("Unable to initialize SILLY library");

}

SILLYImageCodec::~SILLYImageCodec()
{
    SILLY::SILLYCleanup();
}

Texture* SILLYImageCodec::load(const RawDataContainer& data, Texture* result)
{
    SILLY::MemoryDataSource md(static_cast<const SILLY::byte*>(data.getDataPtr()), data.getSize());
    SILLY::Image img(md);
    if (!img.loadImageHeader())
    {
        Logger::getSingletonPtr()->logEvent("SILLYImageCodec::load - Invalid image header", LoggingLevel::Error);
        return nullptr;
    }

    SILLY::PixelFormat dstfmt;
    Texture::PixelFormat cefmt;
    switch (img.getSourcePixelFormat())
    {
    case SILLY::PF_RGB:
        dstfmt = SILLY::PF_RGB;
        cefmt = Texture::PixelFormat::Rgb;
        break;
    case SILLY::PF_RGBA:
    case SILLY::PF_A1B5G5R5:
        dstfmt = SILLY::PF_RGBA;
        cefmt = Texture::PixelFormat::Rgba;
        break;
    default:
        Logger::getSingletonPtr()->logEvent("SILLYImageCodec::load - Unsupported pixel format", LoggingLevel::Error);
        return nullptr;
    }

    if (!img.loadImageData(dstfmt, SILLY::PO_TOP_LEFT))
    { 
        Logger::getSingletonPtr()->logEvent("SILLYImageCodec::load - Invalid image data", LoggingLevel::Error);
        return nullptr;
    }

    result->loadFromMemory(img.getPixelsDataPtr(),
                           Sizef(static_cast<float>(img.getWidth()), static_cast<float>(img.getHeight())), cefmt);
    return result;
}

} // End of CEGUI namespace section 
