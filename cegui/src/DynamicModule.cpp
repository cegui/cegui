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

#include "CEGUI/DynamicModule.h"
#include "CEGUI/Base.h"
#include "CEGUI/String.h"
#include "CEGUI/Exceptions.h"

#if defined(__WIN32__) || defined(_WIN32)
#   if defined(_MSC_VER)
#       pragma warning(disable : 4552)  // warning: operator has no effect; expected operator with side-effect
#   endif
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
#   include "dlfcn.h"
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
// return whether module name has it's extension in place
bool hasDynamicLibraryExtension(const String& name)
{
    #if defined(__WIN32__) || defined(_WIN32)
        return name.substr(name.length() - 4, 4) == ".dll";
    #elif defined(__APPLE__)
        return name.substr(name.length() - 6, 6) == ".dylib";
    #else
        return name.substr(name.length() - 3, 3) == ".so";
    #endif
}

void appendDynamicLibraryExtension(String& name)
{
    #if defined(__WIN32__) || defined(_WIN32)
        name.append(".dll");
    #elif defined(__APPLE__)
        name.append(".dylib");
    #else
        name.append(".so");
    #endif
}

DynamicModule::DynamicModule(const String& name) :
    d_moduleName(name),
    d_handle(0)
{
	if (name.empty())
		return;

    if (!hasDynamicLibraryExtension(d_moduleName))
    {
        #ifdef CEGUI_HAS_BUILD_SUFFIX
            d_moduleName += CEGUI_BUILD_SUFFIX;
        #endif

        #ifdef CEGUI_HAS_VERSION_SUFFIX
            d_moduleName += "-";
            d_moduleName += CEGUI_VERSION_SUFFIX;
        #endif
        
        appendDynamicLibraryExtension(d_moduleName);
    }

    #ifdef __APPLE__
        String fullpath("@executable_path/../Frameworks/" + d_moduleName);
        d_handle = DYNLIB_LOAD(fullpath.c_str());

        // if that failed, try without the path
        if (!d_handle)
    #endif
        d_handle = DYNLIB_LOAD(d_moduleName.c_str());


#if defined(__linux__) || defined(__APPLE__) || defined(__MINGW32__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
    // see if adding a leading 'lib' helps us to open the library
    if (!d_handle && d_moduleName.substr(0, 3) != "lib")
    {
        d_moduleName.insert(0, "lib");
        #ifdef __APPLE__
            String fullpath("@executable_path/../Frameworks/" + d_moduleName);
            d_handle = DYNLIB_LOAD(fullpath.c_str());

            // if that failed, try without the path
            if (!d_handle)
        #endif
            d_handle = DYNLIB_LOAD(d_moduleName.c_str());
    }
#endif

    // check for library load failure
    if (!d_handle)
        CEGUI_THROW(GenericException("Failed to load module '" +
            d_moduleName + "': " + getFailureString()));
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
#if defined(__linux__) || defined (__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
    retMsg = DYNLIB_ERROR();
#elif defined(__WIN32__) || defined(_WIN32)
    LPVOID msgBuffer;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                      FORMAT_MESSAGE_FROM_SYSTEM | 
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      0,
                      GetLastError(),
                      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
//                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
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
