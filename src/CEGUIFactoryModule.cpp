/***********************************************************************
	filename: 	CEGUIFactoryModule.cpp
	created:	12/4/2004
	author:		Paul D Turner
	
	purpose:	Implements FactoryModule for Win32 systems
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
#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIExceptions.h"
#include "CEGUIFactoryModule.h"

#include "config.h"

#if defined(CEGUI_STATIC)
#	if defined(CEGUI_FALAGARD_RENDERER)
#		include "../WindowRendererSets/Falagard/include/FalModule.h"
#	endif
#endif



// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const char	FactoryModule::RegisterFactoryFunctionName[] = "registerFactoryFunction";
const char  FactoryModule::RegisterAllFunctionName[]     = "registerAllFactoriesFunction";


/*************************************************************************
	Construct the FactoryModule object by loading the dynamic loadable
	module specified.
*************************************************************************/
FactoryModule::FactoryModule(const String& filename):d_module(NULL)
{
#if !defined(CEGUI_STATIC)
	d_module = new DynamicModule(filename);
#endif

    // functions are now optional, and only throw upon the first attempt to use a missing function.
    if(d_module)
	{
		d_regFunc = (FactoryRegisterFunction)d_module->getSymbolAddress(RegisterFactoryFunctionName);
		d_regAllFunc = (RegisterAllFunction)d_module->getSymbolAddress(RegisterAllFunctionName);
	}
}


/*************************************************************************
	Destroys the FactoryModule object and unloads any loadable module.
*************************************************************************/
FactoryModule::~FactoryModule(void)
{
	if(d_module)
	{
		delete(d_module);
		d_module = NULL;
	}
}


/*************************************************************************
	Register a WindowFactory for 'type' Windows.
*************************************************************************/
void FactoryModule::registerFactory(const String& type) const
{
	//Make sure we are using a dynamic factory and not the static one.
	if(d_module)
	{
		// are we attempting to use a missing function export
		if (!d_regFunc)
		{
			throw InvalidRequestException("FactoryModule::registerFactory - Required function export 'void registerFactory(const String& type)' was not found in module '" +
				d_module->getModuleName() + "'.");
		}

		d_regFunc(type);
	} // if(d_module)
	else
	{
#if defined(CEGUI_STATIC)
		//Call the register function for the current static library
		registerFactoryFunction(type);
#endif
	}
}

uint FactoryModule::registerAllFactories() const
{
	//Make sure we are using a dynamic factory and not a static one
	if(d_module)
	{
		// are we attempting to use a missing function export
		if (!d_regAllFunc)
		{
			throw InvalidRequestException("FactoryModule::registerAllFactories - Required function export 'uint registerAllFactories(void)' was not found in module '" +
				d_module->getModuleName() + "'.");
		}

		return d_regAllFunc();
	} // if(d_module)
	else
	{
#if defined(CEGUI_STATIC)
		return registerAllFactoriesFunction();
#endif
	}

	return 0;
}

} // End of  CEGUI namespace section
