/***********************************************************************
	filename: 	CEGUIWindowManager.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines the interface for the WindowManager object
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
#ifndef _CEGUIWindowManager_h_
#define _CEGUIWindowManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUILogger.h"
#include "CEGUIIteratorBase.h"
#include "CEGUIEventSet.h"
#include <map>
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif


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
class CEGUIEXPORT WindowManager : public Singleton <WindowManager>,
                                  public EventSet
{
public:
    /*************************************************************************
        Public static data
    *************************************************************************/
    static const String GeneratedWindowNameBase;      //!< Base name to use for generated window names.
    //! Namespace for global events.
    static const String EventNamespace;
    //! Event fired when a new Window object is created.
    static const String EventWindowCreated;
    //! Event fired when a Window object is destroyed.
    static const String EventWindowDestroyed;

	/*!
	\brief
		Function type that is used as a callback when loading layouts from XML; the function is called
		for each Property element encountered.

	\param window
		Window object that the property is to be applied to.

	\param propname
		String holding the name of the property that is being set.

	\param propvalue
		String holding the new value that will be applied to the property specified by /a propname.

	\param userdata
		Some client code supplied data.

	\return
		- true if the property should be set.
		- false if the property should not be set,
	*/
	typedef bool PropertyCallback(Window* window, String& propname, String& propvalue, void* userdata);
	
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
	WindowManager(void);


	/*!
	\brief
		Destructor for WindowManager objects

		This will properly destry all remaining Window objects.  Note that WindowFactory objects will not
		be destroyed (since they are owned by whoever created them).
	*/
	~WindowManager(void);


	/*************************************************************************
		Window Related Methods
	*************************************************************************/
	/*!
	\brief
		Creates a new Window object of the specified type, and gives it the specified unique name.

	\param type
		String that describes the type of Window to be created.  A valid WindowFactory for the specified type must be registered.

	\param name
		String that holds a unique name that is to be given to the new window.  If this string is empty (""), a name
		will be generated for the window.

	\return
		Pointer to the newly created Window object.

    \exception  InvalidRequestException WindowManager is locked and no Windows
                                        may be created.
	\exception	AlreadyExistsException		A Window object with the name \a name already exists.
	\exception	UnknownObjectException		No WindowFactory is registered for \a type Window objects.
	\exception	GenericException			Some other error occurred (Exception message has details).
	*/
	Window* createWindow(const String& type, const String& name = "");


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


	/*!
	\brief
		Creates a set of windows (a Gui layout) from the information in the specified XML file.	

	\param filename
		String object holding the filename of the XML file to be processed.

	\param name_prefix
		String object holding the prefix that is to be used when creating the windows in the layout file, this
		function allows a layout to be loaded multiple times without having name clashes.  Note that if you use
		this facility, then all windows defined within the layout must have names assigned; you currently can not
		use this feature in combination with automatically generated window names.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when loading the layout file.

	\param callback
		PropertyCallback function to be called for each Property element loaded from the layout.  This is
		called prior to the property value being applied to the window enabling client code manipulation of
		properties.

	\param userdata
		Client code data pointer passed to the PropertyCallback function.

	\return
		Pointer to the root Window object defined in the layout.

	\exception FileIOException			thrown if something goes wrong while processing the file \a filename.
	\exception InvalidRequestException	thrown if \a filename appears to be invalid.
	*/
	Window*	loadWindowLayout(const String& filename, const String& name_prefix = "", const String& resourceGroup = "", PropertyCallback* callback = 0, void* userdata = 0);

    /*!
    \brief
        Return whether the window dead pool is empty.

    \return
        - true if there are no windows in the dead pool.
        - false if the dead pool contains >=1 window awaiting destruction.
    */
    bool isDeadPoolEmpty(void) const;

    /*!
    \brief
        Permanently destroys any windows placed in the dead pool.

    \note
        It is probably not a good idea to call this from a Window based event handler
        if the specific window has been or is being destroyed.

    \return
        Nothing.
    */
    void cleanDeadPool(void);

    /*!
    \brief
        Writes a full XML window layout, starting at the given Window to the given OutStream.

    \param window
        Window object to become the root of the layout.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \param writeParent
        If the starting window has a parent window, specifies whether to write the parent name into
        the Parent attribute of the GUILayout XML element.

    \return
        Nothing.
    */
    void writeWindowLayoutToStream(const Window& window, OutStream& out_stream, bool writeParent = false) const;

    /*!
    \brief
        Writes a full XML window layout, starting at the given Window to the given OutStream.

    \param window
        String holding the name of the Window object to become the root of the layout.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \param writeParent
        If the starting window has a parent window, specifies whether to write the parent name into
        the Parent attribute of the GUILayout XML element.

    \return
        Nothing.
    */
    void writeWindowLayoutToStream(const String& window, OutStream& out_stream, bool writeParent = false) const;

    /*!
    \brief
        Rename a window.

    \param window
        String holding the current name of the window to be renamed.

    \param new_name
        String holding the new name for the window

    \exception UnknownObjectException
        thrown if \a window is not known in the system.

    \exception AlreadyExistsException
        thrown if a Window named \a new_name already exists.
    */
    void renameWindow(const String& window, const String& new_name);

    /*!
    \brief
        Rename a window.

    \param window
        Pointer to the window to be renamed.

    \param new_name
        String holding the new name for the window

    \exception AlreadyExistsException
        thrown if a Window named \a new_name already exists.
    */
    void renameWindow(Window* window, const String& new_name);

    /*!
    \brief
        Returns the default resource group currently set for layouts.

    \return
        String describing the default resource group identifier that will be
        used when loading layouts.
    */
    static const String& getDefaultResourceGroup()
        { return d_defaultResourceGroup; }

    /*!
    \brief
        Sets the default resource group to be used when loading layouts

    \param resourceGroup
        String describing the default resource group identifier to be used.

    \return
        Nothing.
    */
    static void setDefaultResourceGroup(const String& resourceGroup)
        { d_defaultResourceGroup = resourceGroup; }

    /*!
    \brief
        Put WindowManager into the locked state.

        While WindowManager is in the locked state all attempts to create a
        Window of any type will fail with an InvalidRequestException being
        thrown.  Calls to lock/unlock are recursive; if multiple calls to lock
        are made, WindowManager is only unlocked after a matching number of
        calls to unlock.

    \note
        This is primarily intended for internal use within the system.
    */
    void lock();

    /*!
    \brief
        Put WindowManager into the unlocked state.

        While WindowManager is in the locked state all attempts to create a
        Window of any type will fail with an InvalidRequestException being
        thrown.  Calls to lock/unlock are recursive; if multiple calls to lock
        are made, WindowManager is only unlocked after a matching number of
        calls to unlock.

    \note
        This is primarily intended for internal use within the system.
    */
    void unlock();

    /*!
    \brief
        Returns whether WindowManager is currently in the locked state.

        While WindowManager is in the locked state all attempts to create a
        Window of any type will fail with an InvalidRequestException being
        thrown.  Calls to lock/unlock are recursive; if multiple calls to lock
        are made, WindowManager is only unlocked after a matching number of
        calls to unlock.

    \return
        - true to indicate WindowManager is locked and that any attempt to
        create Window objects will fail.
        - false to indicate WindowManager is unlocked and that Window objects
        may be created as normal.
    */
    bool isLocked() const;

private:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    /*!
    \brief
        Implementation method to generate a unique name to use for a window.
    */
    String generateUniqueWindowName();

	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const char	GUILayoutSchemaName[];			//!< Filename of the XML schema used for validating GUILayout files.


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef std::map<String, Window*, String::FastLessCompare>			WindowRegistry;				//!< Type used to implement registry of Window objects
    typedef std::vector<Window*>    WindowVector;   //!< Type to use for a collection of Window pointers.

	WindowRegistry			d_windowRegistry;			//!< The container that forms the Window registry
    WindowVector    d_deathrow;     //!< Collection of 'destroyed' windows.

    unsigned long   d_uid_counter;  //!< Counter used to generate unique window names.
    static String d_defaultResourceGroup;   //!< holds default resource group
    //! count of times WM is locked against new window creation.
    uint    d_lockCount;

public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	ConstBaseIterator<WindowRegistry>	WindowIterator;

	/*!
	\brief
		Return a WindowManager::WindowIterator object to iterate over the currently defined Windows.
	*/
	WindowIterator	getIterator(void) const;

    /*!
    \brief
    Outputs the names of ALL existing windows to log (DEBUG function).

    \param zone
    Helper string that can specify where the name dump was made (e.g. "BEFORE FRAME DELETION").

    \return
    Nothing.
    */
    void DEBUG_dumpWindowNames(String zone);    
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIWindowManager_h_
