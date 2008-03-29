/***********************************************************************
filename: 	CEGUIFreeImageImageCodec.cpp
created:	Sun Jun 18th 2006
author:		Andrzej Krzysztof Haczewski (aka guyver6)

purpose:	This codec provide FreeImage based image loading 
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
#include "CEGUIExceptions.h"
#include "CEGUIFreeImageImageCodec.h" 
#include "CEGUILogger.h" 

#include <FreeImage.h>

namespace
{
    void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
    {
        CEGUI::Logger::getSingleton().logEvent(
            CEGUI::String("FreeImage error (") + FreeImage_GetFormatFromFIF(fif) + "): " + message, CEGUI::Errors);
    };

}

// Start of CEGUI namespace section
namespace CEGUI
{
FreeImageImageCodec::FreeImageImageCodec()
    : ImageCodec("FreeImageCodec - FreeImage based image codec")
{
    FreeImage_Initialise(true);
    FreeImage_SetOutputMessage(&FreeImageErrorHandler);

    // Getting extensions
    for (int i = 0; i < FreeImage_GetFIFCount(); ++i)
    {
        String exts(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)i));

        // Replace commas with spaces
        for (size_t i = 0; i < exts.length(); ++i)
            if (exts[i] == ',')
                exts[i] = ' ';

        // Add space after existing extensions
        if (!d_supportedFormat.empty())
            d_supportedFormat += ' ';

        d_supportedFormat += exts;
    }
}

FreeImageImageCodec::~FreeImageImageCodec()
{
    FreeImage_DeInitialise();
}

Texture* FreeImageImageCodec::load(const RawDataContainer& data, Texture* result)
{
    int len = (int)data.getSize();
    FIMEMORY *mem = 0;
    FIBITMAP *img = 0;
    Texture *retval = 0;

    try 
    {
        mem = FreeImage_OpenMemory((BYTE*)data.getDataPtr(), len);
        if (mem == 0)
            throw MemoryException("Unable to open memory stream, FreeImage_OpenMemory failed");

        FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(mem, len);

        if (fif == FIF_UNKNOWN) // it may be that it's TARGA or MNG
        {
            fif = FIF_TARGA;
            img = FreeImage_LoadFromMemory(fif, mem, 0);

            if (img == 0)
            {
                fif = FIF_MNG;
                img = FreeImage_LoadFromMemory(fif, mem, 0);
            }
        }
        else
            img = FreeImage_LoadFromMemory(fif, mem, 0);

        if (img == 0)
            throw GenericException("Unable to load image, FreeImage_LoadFromMemory failed");

        FIBITMAP *newImg = FreeImage_ConvertTo32Bits(img);
        if (newImg == 0)
            throw GenericException("Unable to convert image, FreeImage_ConvertTo32Bits failed");
        FreeImage_Unload(img);
        img = newImg;
        newImg = 0;

        // FreeImage pixel format for little-endian architecture (which CEGUI
        // supports) is like BGRA. We need to convert that to RGBA.
        //
        // It is now:
        // RED_MASK		0x00FF0000
        // GREEN_MASK	0x0000FF00
        // BLUE_MASK	0x000000FF
        // ALPHA_MASK	0xFF000000
        //
        // It should be:
        // RED_MASK		0x000000FF
        // GREEN_MASK	0x0000FF00
        // BLUE_MASK	0x00FF0000
        // ALPHA_MASK	0xFF000000

        uint pitch = FreeImage_GetPitch(img);
        uint height = FreeImage_GetHeight(img);
        uint width = FreeImage_GetWidth(img);
        BYTE *srcBuf = FreeImage_GetBits(img);
        uint8 *dstBuf = new uint8[width * height << 2];

        // We need to convert pixel format a little
        for (uint i = 0; i < height; ++i)
        {
            for (uint j = 0; j < width; ++j)
            {
                uint p = *(((uint*)(srcBuf + i * pitch)) + j);
                uint r = (p >> 16) & 0x000000FF;
                uint b = (p << 16) & 0x00FF0000;
                p &= 0xFF00FF00;
                p |= r | b;
                // At the same time we can flip verticaly the image
                // (since textures are required to be upside down)
                // hence this funny maths
                *(((uint*)dstBuf) + (width * (height-1) - i * width) + j) = p;
            }
        }
        FreeImage_Unload(img);
        img = 0;

        result->loadFromMemory(dstBuf, width, height, Texture::PF_RGBA);
        delete [] dstBuf;
        retval = result;
    }
    catch(Exception&)
    {
    }

    if (img != 0) FreeImage_Unload(img);
    if (mem != 0) FreeImage_CloseMemory(mem);

    return retval;
}


} // End of CEGUI namespace section 

