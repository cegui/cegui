/***********************************************************************
    filename:   CEGUIDynamicModule.h
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
#ifndef _CEGUIDynamicModule_h_
#define _CEGUIDynamicModule_h_

#include "CEGUIString.h"

/*************************************************************************
    The following is basically taken from DynLib.h, which is part of
    the Ogre project (http://www.ogre3d.org/)
*************************************************************************/
#if defined(__WIN32__) || defined(_WIN32)
#   define DYNLIB_HANDLE hInstance
#   define DYNLIB_LOAD( a ) LoadLibrary( a )
#   define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#   define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

    struct HINSTANCE__;
    typedef struct HINSTANCE__* hInstance;

#elif defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
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
class CEGUIEXPORT DynamicModule
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
