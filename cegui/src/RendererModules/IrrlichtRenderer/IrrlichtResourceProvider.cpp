/***********************************************************************
filename: 	IrrlichtResourceProvider.cpp
created:	12/22/2004
author:		Thomas Suter

purpose:	Implements the Resource Provider common functionality
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
#include "IrrlichtResourceProvider.h"

#include "CEGUIExceptions.h"

//#include <xercesc/framework/MemBufInputSource.hpp>

#include <IReadFile.h>

// Start of CEGUI namespace section
namespace CEGUI
{

	IrrlichtResourceProvider::IrrlichtResourceProvider(irr::io::IFileSystem* fsys_) 
		: DefaultResourceProvider(), fsys(fsys_) 
	{
		if(fsys!=0)fsys->grab();
	}

	IrrlichtResourceProvider::~IrrlichtResourceProvider(void) 
	{
		if(fsys!=0)fsys->drop();
	}

	void IrrlichtResourceProvider::loadRawDataContainer(const String& filename, RawDataContainer& output, const String& resourceGroup)
	{
        String final_filename(getFinalFilename(filename, resourceGroup));

		if(!fsys->existFile(final_filename.c_str()))
		{
			String sMsg("IrrlichtResourceProvider::loadRawDataContainer - Filename supplied for loading must be valid");
			sMsg+=" ["+final_filename+"]";
			throw InvalidRequestException(sMsg);
		}
		else
		{
			irr::u8* input;
			irr::u32 input_size;
			irr::io::IReadFile* f=fsys->createAndOpenFile(final_filename.c_str());
			input_size=f->getSize();
			input= new irr::u8[input_size];
			f->read(input,input_size);
			f->drop();

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
