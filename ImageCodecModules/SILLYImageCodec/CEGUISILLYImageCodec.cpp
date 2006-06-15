/***********************************************************************
	filename: 	CEGUISillyImageCodec.cpp
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
#include "CEGUIExceptions.h"
#include "CEGUISILLYImageCodec.h" 
#include "SILLY.h"

// Start of CEGUI namespace section
namespace CEGUI
{
SILLYImageCodec::SILLYImageCodec()
    : ImageCodec("SILLYImageCodec - Official SILLY based image codec")
{
    d_supportedFormat = "tga jpg png";
}

SILLYImageCodec::~SILLYImageCodec()
{
    
}

Texture* SILLYImageCodec::load(const RawDataContainer& data, Texture* result)
{
    SILLY::MemoryDataSource md(data.getDataPtr(), data.getSize());
    SILLY::Image img(md);
    if (!img.isValid()
    {
        Exception("SILLYImageCodec::load - Invalid image source");
        return 0;
    }
    if (!img.loadImageHeader())
    {
        Exception("SILLYImageCodec::load - Invalid image header");
        return 0;
    }
    if (!img.loadImageData())
    {
        Exception("SILLYImageCodec::load - Invalid image data");
        return 0;
    }
    if (!img.isValid())
    {
        Exception("SILLYImageCodec::load - Invalid image");
        return 0;
    }
    result->loadFromMemory(img.getPixelsDataPtr(), img.getWidth(), img.getHeight());
}

} // End of CEGUI namespace section 
