/************************************************************************
	filename: 	irrlichttexture.cpp
	created:	20/7/2004
	author:		Thomas Suter
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "renderers/IrrlichtRenderer/irrlichttexture.h"

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
	void IrrlichtTexture::loadFromFile(const String& filename, const String& resourceGroup)
	{
		freeTexture();
		driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS,true);
		tex=driver->getTexture(filename.c_str());
		
		tex->grab();
	}
/************************************************************************/
	void IrrlichtTexture::loadFromMemory(const void* buffPtr, 
		uint buffWidth, uint buffHeight)
	{
		freeTexture();
		
		irr::core::dimension2d<irr::s32> dim(buffWidth,buffHeight);
		irr::core::stringc name=getUniqueName();

		driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS,true);
		tex=driver->addTexture(dim,name.c_str(),irr::video::ECF_A8R8G8B8);
		
		if(irr::video::ECF_A8R8G8B8==tex->getColorFormat()) // paranoid!
		{
			irr::u32* tt=(irr::u32*)tex->lock(); 
			irr::core::dimension2d<irr::s32> d=tex->getSize();
			memcpy(tt,buffPtr,d.Width*d.Height*sizeof(CEGUI::ulong));
			tex->unlock();
		}
		tex->grab();
	}


}
