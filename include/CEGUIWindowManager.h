/************************************************************************
	filename: 	CEGUIWindowManager.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines the interface for the WindowManager object
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
#ifndef _CEGUIWindowManager_h_
#define _CEGUIWindowManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUILogger.h"
#include <map>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	The WindowManager class describes an object that manages creation and lifetime of Window objects.

	The WindowManager is the means by which Window objects are created and destroyed.  For each sub-class
	of Window that is to be created, there must exist a WindowFactory object which is registered with the
	WindowFactoryManager.  Additionally, the WindowManager tracks every Window object created, and can be
	used to access those Window objects by name.
*/
class CEGUIBASE_API WindowManager : public Singleton <WindowManager>
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructs a new WindowManager object.

		NB: Client code should not create WindowManager objects - they are of limited use to you!  The
		intended pattern of access is to get a pointer to the GUI system's WindowManager via the System
		object, and use that.
	*/
	WindowManager(void) { Logger::getSingleton().logEvent((utf8*)"CEGUI::WindowManager singleton created"); }


	/*!
	\brief
		Destructor for WindowManager objects

		This will properly destry all remaining Window objects.  Note that WindowFactory objects will not
		be destroyed (since they are owned by whoever created them).
	*/
	~WindowManager(void);


	/*!
	\brief
		Return singleton WindowManager object

	\return
		Singleton WindowManager object
	*/
	static	WindowManager&	getSingleton(void)			{return Singleton<WindowManager>::getSingleton();}


	/*!
	\brief
		Return pointer to singleton WindowManager object

	\return
		Pointer to singleton WindowManager object
	*/
	static	WindowManager*	getSingletonPtr(void)		{return Singleton<WindowManager>::getSingletonPtr();}


	/*************************************************************************
		Window Related Methods
	*************************************************************************/
	/*!
	\brief
		Creates a new Window object of the specified type, and gives it the specified unique name.

	\param type
		String that describes the type of Window to be created.  A valid WindowFactory for the specified type must be registered.

	\param name
		String that holds a unique name that is to be given to the new window.

	\return
		Pointer to the newly created Window object.

	\exception	AlreadyExistsException		A Window object with the name \a name already exists.
	\exception	UnknownObjectException		No WindowFactory is registered for \a type Window objects.
	\exception	GenericException			Some other error occurred (Exception message has details).
	*/
	Window*	createWindow(const String& type, const String& name);


	/*!
	\brief
		Destroy the specified Window object.

	\param window
		Pointer to the Window object to be destroyed.  If the \a window is null, or is not recognised, nothing happens.

	\return
		Nothing

	\exception	InvalidRequestException		Can be thrown if the WindowFactory for \a window's object type was removed.
	*/
	void	destroyWindow(Window* window);


	/*!
	\brief
		Destroy the specified Window object.

	\param
		window	String containing the name of the Window object to be destroyed.  If \a window is not recognised, nothing happens.

	\return
		Nothing.

	\exception	InvalidRequestException		Can be thrown if the WindowFactory for \a window's object type was removed.
	*/
	void	destroyWindow(const String& window);


	/*!
	\brief
		Return a pointer to the specified Window object.

	\param name
		String holding the name of the Window object to be returned.

	\return
		Pointer to the Window object with the name \a name.

	\exception UnknownObjectException	No Window object with a name matching \a name was found.
	*/
	Window*	getWindow(const String& name) const;


	/*!
	\brief
		Examines the list of Window objects to see if one exists with the given name

	\param name
		String holding the name of the Window object to look for.

	\return
		true if a Window object was found with a name matching \a name.  false if no matching Window object was found.
	*/
	bool	isWindowPresent(const String& name) const;


	/*!
	\brief
		Destroys all Window objects within the system

	\return
		Nothing.

	\exception	InvalidRequestException		Thrown if the WindowFactory for any Window object type has been removed.
	*/
	void	destroyAllWindows(void);


private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef std::map<String, Window*>			WindowRegistry;				//!< Type used to implement registry of Window objects

	WindowRegistry			d_windowRegistry;			//!< The container that forms the Window registry
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIWindowManager_h_
