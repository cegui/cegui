/************************************************************************
	filename: 	CEGUIWindowFactoryManager.cpp
	created:	22/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the WindowFactoryManager
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
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIExceptions.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> WindowFactoryManager* Singleton<WindowFactoryManager>::ms_Singleton	= NULL;


/*************************************************************************
	Adds a WindowFactory object to the registry
*************************************************************************/
void WindowFactoryManager::addFactory(WindowFactory* factory)
{
	// throw exception if passed factory is null.
	if (factory == NULL)
	{
		throw NullObjectException((utf8*)"WindowFactoryManager::addFactory - The provided WindowFactory pointer was NULL");
	}

	// throw exception if type name for factory is already in use
	if (d_factoryRegistry.find(factory->getTypeName()) != d_factoryRegistry.end())
	{
		throw AlreadyExistsException((utf8*)"WindowFactoryManager::addFactory - A WindowFactory for type '" + factory->getTypeName() + (utf8*)"' is already registered.");
	}

	// add the factory to the registry
	d_factoryRegistry[factory->getTypeName()] = factory;

	Logger::getSingleton().logEvent((utf8*)"WindowFactory for '" + factory->getTypeName() +"' windows added.", Informative);
}


/*************************************************************************
	removes a WindowFactory from the registry (by name)
*************************************************************************/
void WindowFactoryManager::removeFactory(const String& name)
{
	d_factoryRegistry.erase(name);

	Logger::getSingleton().logEvent((utf8*)"WindowFactory for '" + name +"' windows removed.", Informative);
}


/*************************************************************************
	removes a WindowFactory from the registry (by pointer)
*************************************************************************/
void WindowFactoryManager::removeFactory(WindowFactory* factory)
{
	if (factory != NULL)
	{
		removeFactory(factory->getTypeName());
	}

}


/*************************************************************************
	returns a pointer to the requested WindowFactory object
*************************************************************************/
WindowFactory* WindowFactoryManager::getFactory(const String& type) const
{
	WindowFactoryRegistry::const_iterator pos = d_factoryRegistry.find(type);

	if (pos == d_factoryRegistry.end())
	{
		throw UnknownObjectException((utf8*)"WindowFactoryManager::getFactory - A WindowFactory object for '" + type + (utf8*)"' Window objects is not registered with the system.");
	}

	return pos->second;
}


/*************************************************************************
	Returns true if a WindowFactory for a specified type is present
*************************************************************************/
bool WindowFactoryManager::isFactoryPresent(const String& name) const
{
	return (d_factoryRegistry.find(name) != d_factoryRegistry.end());
}


} // End of  CEGUI namespace section
