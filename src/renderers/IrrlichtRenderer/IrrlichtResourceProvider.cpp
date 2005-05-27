/************************************************************************
filename: 	IrrlichtResourceProvider.cpp
created:	12/22/2004
author:		Thomas Suter

purpose:	Implements the Resource Provider common functionality
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
#include "renderers/IrrlichtRenderer/IrrlichtResourceProvider.h"

#include "CEGUIExceptions.h"

//#include <xercesc/framework/MemBufInputSource.hpp>

#include <IReadFile.h>

// Start of CEGUI namespace section
namespace CEGUI
{

	IrrlichtResourceProvider::IrrlichtResourceProvider(irr::io::IFileSystem* fsys_) 
		: ResourceProvider(), fsys(fsys_) 
	{
		if(fsys!=0)fsys->grab();
	}

	IrrlichtResourceProvider::~IrrlichtResourceProvider(void) 
	{
		if(fsys!=0)fsys->drop();
	}

	void IrrlichtResourceProvider::loadRawDataContainer(const String& filename, RawDataContainer& output, const String& resourceGroup)
	{
		if(!fsys->existFile(filename.c_str()))
		{
			String sMsg=(utf8*)"IrrlichtResourceProvider::loadRawDataContainer - Filename supplied for loading must be valid";
			sMsg+=(utf8*)" ["+filename+(utf8*)"]";
			throw InvalidRequestException(sMsg);
		}
		else
		{
			irr::u8* input;
			irr::u32 input_size;
			irr::io::IReadFile* f=fsys->createAndOpenFile(filename.c_str());
			input_size=f->getSize();
			input= new irr::u8[input_size];
			f->read(input,input_size);

			output.setData(input);
			output.setSize(input_size);
		}
	}

    void IrrlichtResourceProvider::unloadRawDataContainer(RawDataContainer& data)
    {
        if (data.getDataPtr())
        {
            delete[] data.getDataPtr();
            data.setData(0);
            data.setSize(0);
        }
    }
} // End of  CEGUI namespace section
