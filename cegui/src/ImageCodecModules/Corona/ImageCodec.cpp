/***********************************************************************
	created:	07/06/2006
	author:		Olivier Delannoy 
	
	purpose:	This codec provide Corona based image loading 
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
#include "CEGUI/ImageCodecModules/Corona/ImageCodec.h" 
#include "CEGUI/Logger.h" 
#include "CEGUI/Size.h"

#include <corona.h> 

// Start of CEGUI namespace section
namespace CEGUI
{
CoronaImageCodec::CoronaImageCodec()
    : ImageCodec("CoronaImageCodec - Official Corona based image codec")
{
    corona::FileFormatDesc** formats = corona::GetSupportedReadFormats();
    for (size_t i = 0 ; formats[i] ; ++i)
    {
        for (size_t j = 0 ; j < formats[i]->getExtensionCount() ; ++j)
        {
            d_supportedFormat += formats[i]->getExtension(j);
        }
    }
}

CoronaImageCodec::~CoronaImageCodec()
{
    
}

Texture* CoronaImageCodec::load(const RawDataContainer& data, Texture* result)
{
    corona::File* texFile = corona::CreateMemoryFile(data.getDataPtr(), (int)data.getSize());
    if (texFile == 0)
    {
        Logger::getSingleton().logEvent("Unable to create corona::File object", Errors);
        return 0;
    }
    corona::Image* texImg = corona::OpenImage(texFile);
    delete texFile;
    if (texImg == 0)
    {
        Logger::getSingleton().logEvent("Unable to load image, corona::OpenImage failed", Errors);
        return 0;
    }
    Texture::PixelFormat cefmt;
    switch(texImg->getFormat())
    {
        case corona::PF_R8G8B8:
            texImg = corona::ConvertImage(texImg, corona::PF_R8G8B8);
            cefmt = Texture::PF_RGB;
            break;
        default:
            texImg = corona::ConvertImage(texImg, corona::PF_R8G8B8A8);
            cefmt = Texture::PF_RGBA;
            break;
    }
    texImg = corona::ConvertImage(texImg, corona::PF_R8G8B8A8);
    if (texImg == 0)
    {
        Logger::getSingleton().logEvent("Unable to convert image to RGBA", Errors);
        return 0; 
    }
    result->loadFromMemory(texImg->getPixels(),
                           Sizef(texImg->getWidth(),
                                  texImg->getHeight()),
                           cefmt);
    delete texImg;
    return result;    
}


} // End of CEGUI namespace section 
