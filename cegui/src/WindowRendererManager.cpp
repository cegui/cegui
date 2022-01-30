/*************************************************************************
 *
 *
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
#include "CEGUI/WindowRendererManager.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/SharedStringStream.h"
#include <algorithm>

namespace CEGUI
{

/*************************************************************************
    Static data
*************************************************************************/
template<> WindowRendererManager* Singleton<WindowRendererManager>::ms_Singleton = nullptr;
WindowRendererManager::OwnedFactoryList WindowRendererManager::d_ownedFactories;

//----------------------------------------------------------------------------//
void WindowRendererManager::addFactoryInternal(WindowRendererFactory* factory)
{
    // only do the actual add now if our singleton has already been created
    if (WindowRendererManager::getSingletonPtr())
    {
        Logger::getSingleton().logEvent("Created WindowRendererFactory for '" +
            factory->getName() +
            "' WindowRenderers.");
        // add the factory we just created
        try
        {
            WindowRendererManager::getSingleton().addFactory(factory);
        }
        catch (Exception&)
        {
            Logger::getSingleton().logEvent("Deleted WindowRendererFactory for "
                "'" + factory->getName() +
                "' WindowRenderers.");
            // delete the factory object
            delete factory;
            throw;
        }
    }

    d_ownedFactories.push_back(factory);
}

/*************************************************************************
    Singleton functions
*************************************************************************/
WindowRendererManager& WindowRendererManager::getSingleton(void)
{
    return Singleton<WindowRendererManager>::getSingleton();
}
WindowRendererManager* WindowRendererManager::getSingletonPtr(void)
{
    return Singleton<WindowRendererManager>::getSingletonPtr();
}

/*************************************************************************
    Constructor / Destructor
*************************************************************************/
WindowRendererManager::WindowRendererManager()
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::WindowRendererManager Singleton created. (" + addressStr + ")");

    // complete addition of any pre-added WindowRendererFactory objects
    OwnedFactoryList::iterator i = d_ownedFactories.begin();

    if (d_ownedFactories.end() != i)
    {
        Logger::getSingleton().logEvent(
        "---- Adding pre-registered WindowRendererFactory objects ----");

        for (; d_ownedFactories.end() != i; ++i)
            addFactory(*i);
    }
}

WindowRendererManager::~WindowRendererManager()
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::WindowRendererManager Singleton destroyed (" + addressStr + ")");
}

/*************************************************************************
    Is there a WindowRenderer by this name?
*************************************************************************/
bool WindowRendererManager::isFactoryPresent(const String& name) const
{
    return (d_wrReg.find(name) != d_wrReg.end());
}

/*************************************************************************
    Get the named WindowRenderer
*************************************************************************/
WindowRendererFactory* WindowRendererManager::getFactory(const String& name) const
{
    WR_Registry::const_iterator i = d_wrReg.find(name);
    if (i != d_wrReg.end())
    {
        return (*i).second;
    }
    throw UnknownObjectException("There is no WindowRendererFactory named '"+name+"' available");
}

/*************************************************************************
    Add a new WindowRenderer factory
*************************************************************************/
void WindowRendererManager::addFactory(WindowRendererFactory* wr)
{
    if (wr == nullptr)
    {
        return;
    }
    if (d_wrReg.insert(std::make_pair(wr->getName(), wr)).second == false)
    {
        throw AlreadyExistsException("A WindowRendererFactory named '"+wr->getName()+"' already exist");
    }

    String addressStr = SharedStringstream::GetPointerAddressAsString(wr);
    Logger::getSingleton().logEvent("WindowRendererFactory '"+wr->getName()+
        "' added. " + addressStr);
}

/*************************************************************************
    Remove a factory by name
*************************************************************************/
void WindowRendererManager::removeFactory(const String& name)
{
    WR_Registry::iterator i = d_wrReg.find(name);

	// non-existing or already removed? The latter can happen when more then one Scheme
	// was loaded using the same renderer.
	if (i == d_wrReg.end())
	{
		return;
	}

    // see if we own this factory
    OwnedFactoryList::iterator j = std::find(d_ownedFactories.begin(),
                                             d_ownedFactories.end(),
                                             (*i).second);

    String addressStr = SharedStringstream::GetPointerAddressAsString((*i).second);

    d_wrReg.erase(name);

    Logger::getSingleton().logEvent("WindowRendererFactory for '" + name +
                                    "' WindowRenderers removed. " + addressStr);

    // delete factory object if we created it
    if (j != d_ownedFactories.end())
    {
        Logger::getSingleton().logEvent("Deleted WindowRendererFactory for '" +
                                        (*j)->getName() +
                                        "' WindowRenderers.");

        delete (*j);
        d_ownedFactories.erase(j);
    }
}

/*************************************************************************
    Create a WindowRenderer instance by factory name
*************************************************************************/
WindowRenderer* WindowRendererManager::createWindowRenderer(const String& name)
{
    WindowRendererFactory* factory = getFactory(name);
    return factory->create();
}

/*************************************************************************
    Destroy a WindowRenderer using its factory
*************************************************************************/
void WindowRendererManager::destroyWindowRenderer(WindowRenderer* wr)
{
    WindowRendererFactory* factory = getFactory(wr->getName());
    factory->destroy(wr);
}

} // CEGUI
