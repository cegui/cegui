/***********************************************************************
	filename: 	CEGUIDevILImageCodec.cpp
	created:	07/06/2006
	author:		Olivier Delannoy 
	
	purpose:	This codec provide DevIL based image loading 
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
#include "CEGUIDevILImageCodec.h" 
#include <IL/il.h>
#include <IL/ilu.h> 

// Start of CEGUI namespace section
namespace CEGUI
{
DevILImageCodec::DevILImageCodec()
    : ImageCodec("DevILImageCodec - Official DevIL based image codec")
{
    // init DevIL libs we use
    ilInit();
    iluInit();
}

DevILImageCodec::~DevILImageCodec()
{    

}

Texture* DevILImageCodec::load(const RawDataContainer& data, Texture* result)
{
    ilPushAttrib(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
    ilEnable(IL_ORIGIN_SET);

    ILuint imgName;
    ilGenImages(1, &imgName);
    ilBindImage(imgName);

    if (ilLoadL(IL_TYPE_UNKNOWN, (ILvoid*)data.getDataPtr(), data.getSize()) != IL_FALSE)
    {
        // get details about size of loaded image
        ILinfo imgInfo;
        iluGetImageInfo(&imgInfo);
        // set dimensions of texture
        size_t width = imgInfo.Width;
        size_t height = imgInfo.Height;
        // allocate temp buffer to receive image data
        uchar* tmpBuff = new uchar[width * height * 4];

        // get image data in required format
        Texture::PixelFormat cefmt;
        ILenum ilfmt;
        switch (imgInfo.Format)
        {
        case IL_RGBA:
        case IL_BGRA:
            ilfmt = IL_RGBA;
            cefmt = Texture::PF_RGBA;
            break;
        default:
            ilfmt = IL_RGB;
            cefmt = Texture::PF_RGB;
            break;
        };
        ilCopyPixels(0, 0, 0, width, height, 1, ilfmt, IL_UNSIGNED_BYTE, (ILvoid*)tmpBuff);

        // delete DevIL image
        ilDeleteImages(1, &imgName);
        ilPopAttrib();

        // create cegui texture
        try
        {
            result->loadFromMemory(tmpBuff, width, height, cefmt);
        }
        catch(...)
        {
            delete [] tmpBuff;
            throw;
        }

        // free temp buffer
        delete [] tmpBuff;

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


} // End of CEGUI namespace section 


  
