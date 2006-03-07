/************************************************************************
    filename:   CEGUIDynamicModule.h
    created:    Tue Mar 7 2006
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)

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
#ifndef _CEGUIDynamicModule_h_
#define _CEGUIDynamicModule_h_

#include "CEGUIString.h"

/*************************************************************************
    The following is basically taken from DynLib.h, which is part of
    the Ogre project (http://www.ogre3d.org/)
*************************************************************************/
#if defined(__WIN32__) || defined(_WIN32)
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibrary( a )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

    struct HINSTANCE__;
    typedef struct HINSTANCE__* hInstance;

#elif defined(__linux__)
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#    define DYNLIB_ERROR( ) dlerror( )

#elif defined(__APPLE_CC__)
#    define DYNLIB_HANDLE CFBundleRef
#    define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#    define DYNLIB_ERROR( ) mac_errorBundle()
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class that wraps and gives access to a dynamically linked module
    (.dll, .so, etc...)
*/
class DynamicModule
{
public:
    /*!
    \brief
        Construct the DynamicModule object by loading the dynamic loadable
        module specified.

    \param name
        String object holding the name of a loadable module.

    \return
        Nothing
    */
    DynamicModule(const String& name);

    /*!
    \brief
        Destroys the DynamicModule object and unloads the associated loadable
        module.

    \return
        Nothing
    */
    ~DynamicModule();

    /*!
    \brief
        Return a String containing the name of the dynamic module.
    */
    const String& getModuleName() const;

    /*!
    \brief
        Return the address of the specified symbol.

    \param symbol
        String holding the symbol to look up in the module.

    \return
        Pointer to the requested symbol.

    \exception
        InvalidRequestException thrown if the symbol does not exist.
    */
    void* getSymbolAddress(const String& symbol) const;

private:
    /*!
    \brief
        Return a String containing the last failure message from the platforms
        dynamic loading system.
    */
    String getFailureString() const;

    //! Holds the name of the loaded module.
    String d_moduleName;
    //! Handle for the loaded module
    DYNLIB_HANDLE d_handle;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIDynamicModule_h_
