/***********************************************************************
    filename:   CEGUIDynamicModule.cpp
    created:    Tue Mar 7 2006
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUIDynamicModule.h"
#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIExceptions.h"

#if defined(__WIN32__) || defined(_WIN32)
#   if defined(_MSC_VER)
#       pragma warning(disable : 4552)  // warning: operator has no effect; expected operator with side-effect
#   endif
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#if defined(__APPLE_CC__)
#   include "macPlugins.h"
#endif

#if defined(__linux__)
#   include "dlfcn.h"
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
DynamicModule::DynamicModule(const String& name) :
    d_moduleName(name)
{
	//If nothing is passed, don't load anything...
	if(name.empty())
	{
		d_handle = NULL;
		return;
	} // if(name.empty())

#if defined(__linux__)
    // dlopen() does not add .so to the filename, like windows does for .dll
    if (d_moduleName.substr(d_moduleName.length() - 3, 3) != ".so")
        d_moduleName += ".so";

    // see if we need to add the leading 'lib'
    if (d_moduleName.substr(0, 3) != "lib")
        d_moduleName.insert(0, "lib");
#endif

    // Optionally add a _d to the module name for the debug config on Win32
#if (defined(__WIN32__) || defined(_WIN32))
#   if defined (_DEBUG) && defined (CEGUI_LOAD_MODULE_APPEND_SUFFIX_FOR_DEBUG)
    // if name has .dll extension, assume it's complete and do not touch it.
    if (d_moduleName.substr(d_moduleName.length() - 4, 4) != ".dll")
        d_moduleName += CEGUI_LOAD_MODULE_DEBUG_SUFFIX;
#   endif
#endif

    d_handle = DYNLIB_LOAD(d_moduleName.c_str());

    // check for library load failure
    if (!d_handle)
        throw GenericException(
            "DynamicModule::DynamicModule - Failed to load module '" +
            d_moduleName + "': " + getFailureString());
}


DynamicModule::~DynamicModule()
{
    DYNLIB_UNLOAD(d_handle);
}


const String& DynamicModule::getModuleName() const
{
    return d_moduleName;
}


void* DynamicModule::getSymbolAddress(const String& symbol) const
{
    return (void*)DYNLIB_GETSYM(d_handle, symbol.c_str());
}


String DynamicModule::getFailureString() const
{
    String retMsg;
#if defined(__linux__) || defined (__APPLE_CC__)
    retMsg = DYNLIB_ERROR();
#elif defined(__WIN32__) || defined(_WIN32)
    LPVOID msgBuffer;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                      FORMAT_MESSAGE_FROM_SYSTEM | 
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      0,
                      GetLastError(),
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      reinterpret_cast<LPTSTR>(&msgBuffer),
                      0,
                      0))
    {
        retMsg = reinterpret_cast<LPTSTR>(msgBuffer);
        LocalFree(msgBuffer);
    }
    else
    {
        retMsg = "Unknown Error";
    }
#else
    retMsg = "Unknown Error";
#endif
    return retMsg;
}

} // End of  CEGUI namespace section
