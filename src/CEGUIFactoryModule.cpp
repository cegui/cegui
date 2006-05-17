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

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const char	FactoryModule::RegisterFactoryFunctionName[] = "registerFactory";
const char  FactoryModule::RegisterAllFunctionName[]     = "registerAllFactories";


/*************************************************************************
	Construct the FactoryModule object by loading the dynamic loadable
	module specified.
*************************************************************************/
FactoryModule::FactoryModule(const String& filename) :
	d_module(filename)
{
    // functions are now optional, and only throw upon the first attempt to use a missing function.
    d_regFunc = (FactoryRegisterFunction)d_module.getSymbolAddress(RegisterFactoryFunctionName);
    d_regAllFunc = (RegisterAllFunction)d_module.getSymbolAddress(RegisterAllFunctionName);
}


/*************************************************************************
	Destroys the FactoryModule object and unloads any loadable module.
*************************************************************************/
FactoryModule::~FactoryModule(void)
{
}


/*************************************************************************
	Register a WindowFactory for 'type' Windows.
*************************************************************************/
void FactoryModule::registerFactory(const String& type) const
{
    // are we attempting to use a missing function export
    if (!d_regFunc)
    {
        throw InvalidRequestException("FactoryModule::registerFactory - Required function export 'void registerFactory(const String& type)' was not found in module '" +
            d_module.getModuleName() + "'.");
    }

	d_regFunc(type);
}

uint FactoryModule::registerAllFactories() const
{
    // are we attempting to use a missing function export
    if (!d_regAllFunc)
    {
        throw InvalidRequestException("FactoryModule::registerAllFactories - Required function export 'uint registerAllFactories(void)' was not found in module '" +
            d_module.getModuleName() + "'.");
    }

    return d_regAllFunc();
}

} // End of  CEGUI namespace section
