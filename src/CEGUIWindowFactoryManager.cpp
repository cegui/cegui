/************************************************************************
	filename: 	CEGUIWindowFactoryManager.cpp
	created:	22/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the WindowFactoryManager
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#include <algorithm>

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
		throw NullObjectException("WindowFactoryManager::addFactory - The provided WindowFactory pointer was NULL");
	}

	// throw exception if type name for factory is already in use
	if (d_factoryRegistry.find(factory->getTypeName()) != d_factoryRegistry.end())
	{
		throw AlreadyExistsException("WindowFactoryManager::addFactory - A WindowFactory for type '" + factory->getTypeName() + "' is already registered.");
	}

	// add the factory to the registry
	d_factoryRegistry[factory->getTypeName()] = factory;

	Logger::getSingleton().logEvent("WindowFactory for '" + factory->getTypeName() +"' windows added.");
}


/*************************************************************************
	removes a WindowFactory from the registry (by name)
*************************************************************************/
void WindowFactoryManager::removeFactory(const String& name)
{
	d_factoryRegistry.erase(name);

	Logger::getSingleton().logEvent("WindowFactory for '" + name +"' windows removed.");
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
	// first try for a 'real' type
	WindowFactoryRegistry::const_iterator pos = d_factoryRegistry.find(type);

	// found an actual factory for this type
	if (pos != d_factoryRegistry.end())
	{
		return pos->second;
	}
	// no real type exists with that type so try for an aliased type
	else
	{
		TypeAliasRegistry::const_iterator aliasPos = d_aliasRegistry.find(type);

        // found aliased type
		if (aliasPos != d_aliasRegistry.end())
		{
			// recursively call getFactory, for the alias target type (allows aliasing of aliased names)
			return getFactory(aliasPos->second.getActiveTarget());
		}
		// no aliased type, try for a falagard mapped type
		else
		{
            FalagardMapRegistry::const_iterator falagard = d_falagardRegistry.find(type);

            // found falagard mapping for this type
            if (falagard != d_falagardRegistry.end())
            {
                // recursively call getFactory on the target base type
                return getFactory(falagard->second.d_baseType);
            }
            // type not found anywhere, give up with an exception.
            else
            {
              throw UnknownObjectException("WindowFactoryManager::getFactory - A WindowFactory object (nor an alias, or mapping) for '" + type + "' Window objects is not registered with the system.");
            }
        }
    }
}


/*************************************************************************
	Returns true if a WindowFactory (or an alias) for a specified type
	is present
*************************************************************************/
bool WindowFactoryManager::isFactoryPresent(const String& name) const
{
	// first try for a 'real' type
	if (d_factoryRegistry.find(name) != d_factoryRegistry.end())
	{
		return true;
	}
	// no real type exists with that type, see if we have an appropriate alias instead.
	else
	{
		return (d_aliasRegistry.find(name) != d_aliasRegistry.end());
	}

}


WindowFactoryManager& WindowFactoryManager::getSingleton(void)
{
	return Singleton<WindowFactoryManager>::getSingleton();
}


WindowFactoryManager* WindowFactoryManager::getSingletonPtr(void)
{
	return Singleton<WindowFactoryManager>::getSingletonPtr();
}


/*************************************************************************
	Return a WindowFactoryManager::WindowFactoryIterator object to
	iterate over the available WindowFactory types.
*************************************************************************/
WindowFactoryManager::WindowFactoryIterator	WindowFactoryManager::getIterator(void) const
{
	return WindowFactoryIterator(d_factoryRegistry.begin(), d_factoryRegistry.end());
}


/*************************************************************************
	Return a WindowFactoryManager::TypeAliasIterator object to iterate
	over the defined aliases for window types.
*************************************************************************/
WindowFactoryManager::TypeAliasIterator WindowFactoryManager::getAliasIterator(void) const
{
	return TypeAliasIterator(d_aliasRegistry.begin(), d_aliasRegistry.end());
}


/*************************************************************************
	Add a window type alias mapping
*************************************************************************/
void WindowFactoryManager::addWindowTypeAlias(const String& aliasName, const String& targetType)
{
	// throw if target type does not exist
	if (!isFactoryPresent(targetType))
	{
		throw UnknownObjectException("WindowFactoryManager::addWindowTypeAlias - alias '" + aliasName + "' could not be created because the target type '" + targetType + "' is unknown within the system.");
	}

	TypeAliasRegistry::iterator pos = d_aliasRegistry.find(aliasName);

	if (pos == d_aliasRegistry.end())
	{
		d_aliasRegistry[aliasName].d_targetStack.push_back(targetType);
	}
	// alias already exists, add our new entry to the list already there
	else
	{
		pos->second.d_targetStack.push_back(targetType);
	}

	Logger::getSingleton().logEvent("Window type alias named '" + aliasName + "' added for window type '" + targetType +"'.");
}


/*************************************************************************
	Remove a window type alias mapping
*************************************************************************/
void WindowFactoryManager::removeWindowTypeAlias(const String& aliasName, const String& targetType)
{
	// find alias name
	TypeAliasRegistry::iterator pos = d_aliasRegistry.find(aliasName);

	// if alias name exists
	if (pos != d_aliasRegistry.end())
	{
		// find the specified target for this alias
		std::vector<String>::iterator aliasPos = std::find(pos->second.d_targetStack.begin(), pos->second.d_targetStack.end(), targetType);
		
		// if the target exists for this alias
		if (aliasPos != pos->second.d_targetStack.end())
		{
			// erase the target mapping
			pos->second.d_targetStack.erase(aliasPos);

			Logger::getSingleton().logEvent("Window type alias named '" + aliasName + "' removed for window type '" + targetType +"'.");

			// if the list of targets for this alias is now empty
			if (pos->second.d_targetStack.empty())
			{
				// erase the alias name also
				d_aliasRegistry.erase(aliasName);

				Logger::getSingleton().logEvent("Window type alias named '" + aliasName + "' has no more targets and has been removed.", Informative);
			}

		}

	}

}

void WindowFactoryManager::addFalagardWindowMapping(const String& newType, const String& targetType, const String& lookName)
{
    WindowFactoryManager::FalagardWindowMapping mapping;
    mapping.d_windowType = newType;
    mapping.d_baseType   = targetType;
    mapping.d_lookName   = lookName;

    // see if the type we're creating already exists
    if (d_falagardRegistry.find(newType) != d_falagardRegistry.end())
    {
        // type already exists, log the fact that it's going to be replaced.
        Logger::getSingleton().logEvent("Falagard mapping for type '" + newType + "' already exists - current mapping will be replaced.");
    }

    Logger::getSingleton().logEvent("Creating falagard mapping for type '" + newType + "' using base type '" + targetType + "' and LookN'Feel '" + lookName + "'.");

    d_falagardRegistry[newType] = mapping;
}

void WindowFactoryManager::removeFalagardWindowMapping(const String& type)
{
    FalagardMapRegistry::iterator iter = d_falagardRegistry.find(type);

    if (iter != d_falagardRegistry.end())
    {
        Logger::getSingleton().logEvent("Removing falagard mapping for type '" + type + "'.");
        d_falagardRegistry.erase(iter);
    }
}

WindowFactoryManager::FalagardMappingIterator WindowFactoryManager::getFalagardMappingIterator() const
{
    return FalagardMappingIterator(d_falagardRegistry.begin(), d_falagardRegistry.end());
}

bool WindowFactoryManager::isFalagardMappedType(const String& type) const
{
    return d_falagardRegistry.find(type) != d_falagardRegistry.end();
}

const String& WindowFactoryManager::getMappedLookForType(const String& type) const
{
    FalagardMapRegistry::const_iterator iter = d_falagardRegistry.find(type);

    if (iter != d_falagardRegistry.end())
    {
        return (*iter).second.d_lookName;
    }
    // type does not exist as a mapped type
    else
    {
        throw InvalidRequestException("WindowFactoryManager::getMappedLookForType - Window factory type '" + type + "' is not a falagard mapped type (or does not exist at all).");
    }
}

//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Methods for AliasTargetStack class
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
const String& WindowFactoryManager::AliasTargetStack::getActiveTarget(void) const
{
	return d_targetStack.back();
}


uint WindowFactoryManager::AliasTargetStack::getStackedTargetCount(void) const
{
	return (uint)d_targetStack.size();
}


} // End of  CEGUI namespace section
