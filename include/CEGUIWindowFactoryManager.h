/************************************************************************
	filename: 	CEGUIWindowFactoryManager.h
	created:	22/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for WindowFactoryManager class
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
#ifndef _CEGUIWindowFactoryManager_h_
#define _CEGUIWindowFactoryManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUILogger.h"
#include "CEGUIIteratorBase.h"
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
	Class that manages WindowFactory objects
*/
class CEGUIBASE_API WindowFactoryManager : public Singleton<WindowFactoryManager>
{
public:
	/*************************************************************************
		Class used to track active alias targets
	*************************************************************************/
	class CEGUIBASE_API AliasTargetStack
	{
	public:
		/*!
		\brief
			Constructor for WindowAliasTargetStack objects
		*/
		AliasTargetStack(void) {}


		/*!
		\brief
			Destructor for WindowAliasTargetStack objects
		*/
		~AliasTargetStack(void) {}


		/*!
		\brief
			Return a String holding the current target type for this stack

		\return
			reference to a String object holding the currently active target type name for this stack.
		*/
		const String&	getActiveTarget(void) const;

		/*!
		\brief
			Return the number of stacked target types in the stack

		\return
			number of target types stacked for this alias.
		*/
		uint	getStackedTargetCount(void) const;


	private:
		friend class WindowFactoryManager;
		typedef std::vector<String>	TargetTypeStack;		//!< Type used to implement stack of target type names.

		TargetTypeStack	d_targetStack;		//!< Container holding the target types.
	};


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructs a new WindowFactoryManager object.
	*/
	WindowFactoryManager(void)
	{
		Logger::getSingleton().logEvent((utf8*)"CEGUI::WindowFactoryManager singleton created");
	}


	/*!
	\brief
		Destructor for WindowFactoryManager objects
	*/
	~WindowFactoryManager(void)
	{
		Logger::getSingleton().logEvent((utf8*)"CEGUI::WindowFactoryManager singleton destroyed");
	}


	/*************************************************************************
		Public Interface
	*************************************************************************/
	/*!
	\brief
		Return singleton WindowFactoryManager object

	\return
		Singleton WindowFactoryManager object
	*/
	static	WindowFactoryManager&	getSingleton(void);


	/*!
	\brief
		Return pointer to singleton WindowFactoryManager object

	\return
		Pointer to singleton WindowFactoryManager object
	*/
	static	WindowFactoryManager*	getSingletonPtr(void);


	/*!
	\brief
		Adds a new WindowFactory to the list of registered factories.

	\param factory
		Pointer to the WindowFactory to be added to the WindowManager.

	\return
		Nothing
	
	\exception NullObjectException		\a factory was null.
	\exception AlreadyExistsException	\a factory provided a Window type name which is in use by another registered WindowFactory.
	*/
	void	addFactory(WindowFactory* factory);


	/*!
	\brief
		Removes a WindowFactory from the list of registered factories.
		
	\note
		The WindowFactory object is not destroyed (since it was created externally), instead it is just removed from the list.

	\param name
		String which holds the name (technically, Window type name) of the WindowFactory to be removed.  If \a name is not
		in the list, no error occurs (nothing happens).

	\return
		Nothing
	*/
	void	removeFactory(const String& name);


	/*!
	\brief
		Removes a WindowFactory from the list of registered factories.

	\note
		The WindowFactory object is not destroyed (since it was created externally), instead it is just removed from the list.

	\param factory
		Pointer to the factory object to be removed.  If \a factory is null, or if no such WindowFactory is in the list, no
		error occurs (nothing happens).

	\return
		Nothing
	*/
	void	removeFactory(WindowFactory* factory);


	/*!
	\brief
		Remove all WindowFactory objects from the list.

	\return
		Nothing
	*/
	void	removeAllFactories(void)		{d_factoryRegistry.clear();}


	/*!
	\brief
		Return a pointer to the specified WindowFactory object.

	\param type
		String holding the Window object type to return the WindowFactory for.

	\return
		Pointer to the WindowFactory object that creates Windows of the type \a type.

	\exception UnknownObjectException	No WindowFactory object for Window objects of type \a type was found.
	*/
	WindowFactory*	getFactory(const String& type) const;


	/*!
	\brief
		Checks the list of registered WindowFactory objects for one which creates Window objects of the specified type.

	\param name
		String containing the name (technically, Window type name) of the WindowFactory to check for.

	\return
		true if a WindowFactory that creates Window objects of type \a name is registered.  Else, false.
	*/
	bool	isFactoryPresent(const String& name) const;


	/*!
	\brief
		Adds an alias for a current window type.

		This method allows you to create an alias for a specified window type.  This means that you can then use
		either name as the type parameter when creating a window.

	\note
		You need to be careful using this system.  Creating an alias using a name that already exists will replace the previous
		mapping for that alias.  Each alias name maintains a stack, which means that it is possible to remove an alias and have the
		previous alias restored.  The windows created via an alias use the real type, so removing an alias after window creation is always
		safe (i.e. it is not the same as removing a real factory, which would cause an exception when trying to destroy a window with a missing
		factory).

	\param aliasName
		String object holding the alias name.  That is the name that \a targetType will also be known as from no on.

	\param targetType
		String object holding the type window type name that is to be aliased.  This type must already exist.

	\return
		Nothing.

	\exception UnknownObjectException	thrown if \a targetType is not known within the system.
	*/
	void	addWindowTypeAlias(const String& aliasName, const String& targetType);


	/*!
	\brief
		Remove the specified alias mapping.  If the alias mapping does not exist, nothing happens.

	\note
		You are required to supply both the alias and target names because there may exist more than one entry for a given
		alias - therefore you are required to be explicit about which alias is to be removed.

	\param aliasName
		String object holding the alias name.

	\param targetType
		String object holding the type window type name that was aliased.

	\return
		Nothing.
	*/
	void	removeWindowTypeAlias(const String& aliasName, const String& targetType);


private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef	std::map<String, WindowFactory*>	WindowFactoryRegistry;		//!< Type used to implement registry of WindowFactory objects
	typedef std::map<String, AliasTargetStack>	TypeAliasRegistry;		//!< Type used to implement registry of window type aliases.

	WindowFactoryRegistry	d_factoryRegistry;			//!< The container that forms the WindowFactory registry
	TypeAliasRegistry		d_aliasRegistry;			//!< The container that forms the window type alias registry.

public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	ConstBaseIterator<WindowFactoryRegistry>	WindowFactoryIterator;
	typedef ConstBaseIterator<TypeAliasRegistry>		TypeAliasIterator;

	/*!
	\brief
		Return a WindowFactoryManager::WindowFactoryIterator object to iterate over the available WindowFactory types.
	*/
	WindowFactoryIterator	getIterator(void) const;


	/*!
	\brief
		Return a WindowFactoryManager::TypeAliasIterator object to iterate over the defined aliases for window types.
	*/
	TypeAliasIterator	getAliasIterator(void) const;
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIWindowFactoryManager_h_
