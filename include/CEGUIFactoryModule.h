/************************************************************************
	filename: 	CEGUIFactoryModule.h
	created:	12/4/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for object that controls a loadable
				module (.dll/.so/ whatever) that contains concrete
				window / widget implementations and their factories.
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
#ifndef _CEGUIFactoryModule_h_
#define _CEGUIFactoryModule_h_

/*************************************************************************
	The following is basically taken from DynLib.h, which is part of
	the Ogre project (http://www.ogre3d.org/)
*************************************************************************/
#if defined(__WIN32__) || defined(_WIN32)
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibrary( a )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#    define DYNLIB_ERROR( )  "Unknown Error"

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
	Class that encapsulates access to a dynamic loadable module containing implementations of Windows, Widgets, and their factories.
*/
class FactoryModule
{
public:
	/*!
	\brief
		Construct the FactoryModule object by loading the dynamic loadable module specified.

	\param filename
		String object holding the filename of a loadable module.

	\return
		Nothing
	*/
	FactoryModule(const String& filename);


	/*!
	\brief
		Destroys the FactoryModule object and unloads any loadable module.

	\return
		Nothing
	*/
	virtual ~FactoryModule(void);


	/*!
	\brief
		Register a WindowFactory for \a type Windows.

	\param type
		String object holding the name of the Window type a factory is to be registered for.

	\return
		Nothing.
	*/
	void	registerFactory(const String& type) const;

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	static const char	RegisterFactoryFunctionName[];

	typedef void (*FactoryRegisterFunction)(const String&); 

	FactoryRegisterFunction	d_regFunc;	//!< Pointer to the function called to register factories.
	String			d_moduleName;		//!< Holds the name of the loaded module.
	DYNLIB_HANDLE	d_handle;			//!< Pointer to a ImplDat derived class that can hold any required implementation data
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIFactoryModule_h_
