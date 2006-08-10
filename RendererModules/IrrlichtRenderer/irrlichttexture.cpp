/***********************************************************************
	filename: 	irrlichttexture.cpp
	created:	20/7/2004
	author:		Thomas Suter
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
#include "irrlichttexture.h"
#include "IrrlichtMemoryFile.h"
#include "CEGUISystem.h"

namespace CEGUI
{
/************************************************************************/
	int IrrlichtTexture::iTextureNumber=0;
/************************************************************************/
	irr::core::stringc IrrlichtTexture::getUniqueName(void)
	{
		char buffer[32];
		sprintf(buffer,"irr_tex_%d",iTextureNumber);
		irr::core::stringc str(buffer);
		++iTextureNumber;
		return str;
	}
/************************************************************************/
	IrrlichtTexture::IrrlichtTexture(Renderer* r, irr::IrrlichtDevice* dr)
		:Texture(r), device(dr),tex(0)
	{
		driver=device->getVideoDriver();
	}
/************************************************************************/
    IrrlichtTexture::IrrlichtTexture(Renderer* r, irr::IrrlichtDevice* dr, float size)
        :Texture(r), device(dr),tex(0)
    {
        driver=device->getVideoDriver();
        irr::core::dimension2d<irr::s32> texSz(size, size);
        tex = driver->addTexture(texSz, getUniqueName().c_str(), irr::video::ECF_A8R8G8B8);
    }
/************************************************************************/
	IrrlichtTexture::~IrrlichtTexture(){
		freeTexture();
	}
/************************************************************************/
	irr::video::ITexture* IrrlichtTexture::getTexture()
	{
		return tex;
	}
/************************************************************************/
	void IrrlichtTexture::setTexture(irr::video::ITexture* texture)
	{
		this->tex=texture;
	}
/************************************************************************/
	void IrrlichtTexture::freeTexture()
	{
		if(tex!=0) 
		{
			tex->drop();
			driver->removeTexture(tex);
		}
		
		tex=0;
	}
/************************************************************************/
	ushort IrrlichtTexture::getWidth(void) const
	{
		if(tex) return tex->getSize().Width;
		return 0;
	}
/************************************************************************/
	ushort IrrlichtTexture::getHeight(void) const
	{
		if(tex) return tex->getSize().Height;
		return 0;
	}
/************************************************************************/
    ushort IrrlichtTexture::getOriginalWidth(void) const
    {
        if(tex) return tex->getOriginalSize().Width;
        return 0;
    }
/************************************************************************/
    ushort IrrlichtTexture::getOriginalHeight(void) const
    {
        if(tex) return tex->getOriginalSize().Height;
        return 0;
    }
/************************************************************************/
    float IrrlichtTexture::getXScale(void) const
    {
        return xScale;
    }
/************************************************************************/
    float IrrlichtTexture::getYScale(void) const
    {
        return yScale;
    }
/************************************************************************/
	void IrrlichtTexture::loadFromFile(const String& filename, const String& resourceGroup)
	{
		freeTexture();

        RawDataContainer texFile;
        System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, texFile, resourceGroup);

        IrrlichtMemoryFile imf(filename, texFile.getDataPtr(), texFile.getSize());

		driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS,true);
		tex=driver->getTexture(&imf);
		tex->grab();

        // unload file data buffer
        System::getSingleton().getResourceProvider()->unloadRawDataContainer(texFile);
        updateCachedScaleValues();
	}
/************************************************************************/
	void IrrlichtTexture::loadFromMemory(const void* buffPtr, 
		uint buffWidth, uint buffHeight, PixelFormat pixelFormat)
	{
		freeTexture();
		
		irr::core::dimension2d<irr::s32> dim(buffWidth,buffHeight);
		irr::core::stringc name=getUniqueName();

		driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS,true);

        unsigned int pixelSize;
        irr::video::ECOLOR_FORMAT texFormat;

        switch(pixelFormat)
        {
            case PF_RGB:
                pixelSize = 3;
                texFormat = irr::video::ECF_R8G8B8;
                break;
            case PF_RGBA:
                pixelSize = 4;
                texFormat = irr::video::ECF_A8R8G8B8;
                break;
        }

		tex=driver->addTexture(dim,name.c_str(), texFormat);
		
		if(texFormat == tex->getColorFormat()) // paranoid!
		{
			irr::u32* tt=(irr::u32*)tex->lock(); 
			irr::core::dimension2d<irr::s32> d=tex->getSize();
			memcpy(tt,buffPtr,d.Width*d.Height*pixelSize);
			tex->unlock();
		}
		tex->grab();
        updateCachedScaleValues();
	}
/************************************************************************/
    void IrrlichtTexture::updateCachedScaleValues()
    {
        //
        // calculate what to use for x scale
        //
        ushort orgW = getOriginalWidth();
        ushort texW = getWidth();

        // if texture and original data width are the same, scale is based
        // on the original size.
        // if texture is wider (and source data was not stretched), scale
        // is based on the size of the resulting texture.
        xScale = 1.0f / ((orgW == texW) ?
            static_cast<float>(orgW) :
            static_cast<float>(texW));

        //
        // calculate what to use for y scale
        //
        ushort orgH = getOriginalHeight();
        ushort texH = getHeight();

        // if texture and original data height are the same, scale is based
        // on the original size.
        // if texture is taller (and source data was not stretched), scale
        // is based on the size of the resulting texture.
        yScale = 1.0f / ((orgH == texH) ?
            static_cast<float>(orgH) :
            static_cast<float>(texH));
    }


}
