/************************************************************************
	filename: 	CEGUIFactoryModule.cpp
	created:	12/4/2004
	author:		Paul D Turner
	
	purpose:	Implements FactoryModule for Win32 systems
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIExceptions.h"
#include "CEGUIFactoryModule.h"

#include <Windows.h>

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	ImplDat based class we use to store some data in
*************************************************************************/
class ModuleDat : public FactoryModuleImplDat
{
public:
	typedef void (*FactoryRegFunc)(const String&); 

	HMODULE	handle;
	FactoryRegFunc	regFunc;

};

/*************************************************************************
	Construct the FactoryModule object by loading the dynamic loadable
	module specified.
*************************************************************************/
FactoryModule::FactoryModule(const String& filename)
{
	ModuleDat* data = new ModuleDat;

	data->handle = LoadLibrary(filename.c_str());

	// check for library load failure
	if (data->handle == NULL)
	{
		delete data;
		throw	GenericException((utf8*)"FactoryModule::FactoryModule - Failed to load module '" + filename + "'.");
	}

	data->regFunc = (ModuleDat::FactoryRegFunc)GetProcAddress(data->handle, "registerFactory");

	// check for failure to find required function export
	if (data->regFunc == NULL)
	{
		FreeLibrary(data->handle);
		delete data;

		throw	GenericException((utf8*)"FactoryModule::FactoryModule - Required function export 'registerFactory' was not found in module '" + filename + "'.");
	}

	d_data = data;
}


/*************************************************************************
	Destroys the FactoryModule object and unloads any loadable module.
*************************************************************************/
FactoryModule::~FactoryModule(void)
{
	FreeLibrary(((ModuleDat*)d_data)->handle);
	delete (ModuleDat*)d_data;
}


/*************************************************************************
	Register a WindowFactory for 'type' Windows.
*************************************************************************/
void FactoryModule::registerFactory(const String& type) const
{
	((ModuleDat*)d_data)->regFunc(type);
}

} // End of  CEGUI namespace section
