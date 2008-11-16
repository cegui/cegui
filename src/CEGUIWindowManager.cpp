/***********************************************************************
    filename:   CEGUIWindowManager.cpp
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Implements the WindowManager class
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
#include "CEGUIWindowManager.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIGUILayout_xmlHandler.h"
#include "CEGUIXMLParser.h"
#include <iostream>
#include <sstream>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> WindowManager* Singleton<WindowManager>::ms_Singleton    = 0;
// default resource group
String WindowManager::d_defaultResourceGroup;

/*************************************************************************
    Definition of constant data for WindowManager
*************************************************************************/
// Declared in WindowManager
const char  WindowManager::GUILayoutSchemaName[]    = "GUILayout.xsd";
const String WindowManager::GeneratedWindowNameBase("__cewin_uid_");


/*************************************************************************
    Constructor
*************************************************************************/
WindowManager::WindowManager(void) :
    d_uid_counter(0),
    d_lockCount(0)
{
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
        "CEGUI::WindowManager singleton created " + String(addr_buff));
}


/*************************************************************************
    Destructor
*************************************************************************/
WindowManager::~WindowManager(void)
{
    destroyAllWindows();
    cleanDeadPool();

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
        "CEGUI::WindowManager singleton destroyed " + String(addr_buff));
}


/*************************************************************************
    Create a new window of the specified type
*************************************************************************/
Window* WindowManager::createWindow( const String& type, const String& name /*= ""*/, const String& prefix /*= ""*/ )
{
    // only allow creation of Window objects if we are in unlocked state
    if (isLocked())
        throw InvalidRequestException("WindowManager::createWindow - "
                                      "WindowManager is in the locked state.");

    // Make sure that a non-empty name gets passed to the factory
    String finalName(prefix + name);
    // Still empty?
    if (finalName.empty())
    {
        finalName = generateUniqueWindowName();
    }

    if (isWindowPresent(finalName))
    {
        throw AlreadyExistsException("WindowManager::createWindow - A Window object with the name '" + finalName +"' already exists within the system.");
    }

    WindowFactoryManager& wfMgr = WindowFactoryManager::getSingleton();
    WindowFactory* factory = wfMgr.getFactory(type);

    Window* newWindow = factory->createWindow(finalName);
    newWindow->setPrefix(prefix);

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(newWindow));
    Logger::getSingleton().logEvent("Window '" + finalName +"' of type '" +
        type + "' has been created. " + addr_buff, Informative);

    // see if we need to assign a look to this window
    if (wfMgr.isFalagardMappedType(type))
    {
        const WindowFactoryManager::FalagardWindowMapping& fwm = wfMgr.getFalagardMappingForType(type);
        // this was a mapped type, so assign a look to the window so it can finalise
        // its initialisation
        newWindow->d_falagardType = type;
        newWindow->setWindowRenderer(fwm.d_rendererType);
        newWindow->setLookNFeel(fwm.d_lookName);
    }

    d_windowRegistry[finalName] = newWindow;

    return newWindow;
}


/*************************************************************************
    Destroy the given window by pointer
*************************************************************************/
void WindowManager::destroyWindow(Window* window)
{
    if (window)
    {
        // this is done because the name is used for the log after the window is destroyed,
        // if we just did getName() we would get a const ref to the Window's internal name
        // string which is destroyed along with the window so wouldn't exist when the log tried
        // to use it (as I soon discovered).
        String name = window->getName();

        destroyWindow(name);
    }

}


/*************************************************************************
    Destroy the given window by name
*************************************************************************/
void WindowManager::destroyWindow(const String& window)
{
    WindowRegistry::iterator wndpos = d_windowRegistry.find(window);

    if (wndpos != d_windowRegistry.end())
    {
        Window* wnd = wndpos->second;

        // remove entry from the WindowRegistry.
        d_windowRegistry.erase(wndpos);

        // do 'safe' part of cleanup
        wnd->destroy();

        // add window to dead pool
        d_deathrow.push_back(wnd);

        // notify system object of the window destruction
        System::getSingleton().notifyWindowDestroyed(wnd);

        char addr_buff[32];
        sprintf(addr_buff, "(%p)", static_cast<void*>(wnd));
        Logger::getSingleton().logEvent("Window '" + window + "' has been "
            "added to dead pool. " + addr_buff, Informative);
    }

}


/*************************************************************************
    Return a pointer to the named window
*************************************************************************/
Window* WindowManager::getWindow(const String& name) const
{
    WindowRegistry::const_iterator pos = d_windowRegistry.find(name);

    if (pos == d_windowRegistry.end())
    {
        throw UnknownObjectException("WindowManager::getWindow - A Window object with the name '" + name +"' does not exist within the system");
    }

    return pos->second;
}


/*************************************************************************
    Return true if a window with the given name is present
*************************************************************************/
bool WindowManager::isWindowPresent(const String& name) const
{
    return (d_windowRegistry.find(name) != d_windowRegistry.end());
}


/*************************************************************************
    Destroy all Window objects
*************************************************************************/
void WindowManager::destroyAllWindows(void)
{
    String window_name;
    while (!d_windowRegistry.empty())
    {
        window_name = d_windowRegistry.begin()->first;
        destroyWindow(window_name);
    }

}


/*************************************************************************
    Creates a set of windows (a Gui layout) from the information in the
    specified XML file.
*************************************************************************/
Window* WindowManager::loadWindowLayout(const String& filename, const String& name_prefix, const String& resourceGroup, PropertyCallback* callback, void* userdata)
{
    if (filename.empty())
    {
        throw InvalidRequestException("WindowManager::loadWindowLayout - Filename supplied for gui-layout loading must be valid.");
    }

    // log the fact we are about to load a layout
    Logger::getSingleton().logEvent("---- Beginning loading of GUI layout from '" + filename + "' ----", Informative);

    // create handler object
    GUILayout_xmlHandler handler(name_prefix, callback, userdata);

    // do parse (which uses handler to create actual data)
    try
    {
        System::getSingleton().getXMLParser()->parseXMLFile(handler,
            filename, GUILayoutSchemaName, resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);
    }
    catch(...)
    {
        Logger::getSingleton().logEvent("WindowManager::loadWindowLayout - loading of layout from file '" + filename +"' failed.", Errors);
        throw;
    }

    // log the completion of loading
    Logger::getSingleton().logEvent("---- Successfully completed loading of GUI layout from '" + filename + "' ----", Standard);

    return handler.getLayoutRootWindow();
}

Window* WindowManager::loadWindowLayout( const String& filename, bool generateRandomPrefix )
{
    //We really just use the bool to get rid of ambiguity with the other loadWindowLayout. There is no difference between
    //calling this loadWindowLayout and setting GRP to false, and calling the other one with no argument
    if(generateRandomPrefix)
        return loadWindowLayout(filename,generateUniqueWindowPrefix());

        return loadWindowLayout(filename);
}
bool WindowManager::isDeadPoolEmpty(void) const
{
    return d_deathrow.empty();
}

void WindowManager::cleanDeadPool(void)
{
    WindowVector::reverse_iterator curr = d_deathrow.rbegin();
    for (; curr != d_deathrow.rend(); ++curr)
    {
// in debug mode, log what gets cleaned from the dead pool (insane level)
#if defined(DEBUG) || defined (_DEBUG)
        CEGUI_LOGINSANE("Window '" + (*curr)->getName() + "' about to be finally destroyed from dead pool.");
#endif

        WindowFactory* factory = WindowFactoryManager::getSingleton().getFactory((*curr)->getType());
        factory->destroyWindow(*curr);
    }

    // all done here, so clear all pointers from dead pool
    d_deathrow.clear();
}

void WindowManager::writeWindowLayoutToStream(const Window& window, OutStream& out_stream, bool writeParent) const
{

    XMLSerializer xml(out_stream);
    // output GUILayout start element
    xml.openTag("GUILayout");
    // see if we need the parent attribute to be written
    if ((window.getParent() != 0) && writeParent)
    {
        xml.attribute("Parent", window.getParent()->getName());
    }
    // write windows
    window.writeXMLToStream(xml);
    // write closing GUILayout element
    xml.closeTag();
}

void WindowManager::writeWindowLayoutToStream(const String& window, OutStream& out_stream, bool writeParent) const
{
    writeWindowLayoutToStream(*getWindow(window), out_stream, writeParent);
}

String WindowManager::generateUniqueWindowName()
{
    // build name
    std::ostringstream uidname;
    uidname << GeneratedWindowNameBase.c_str() << d_uid_counter;

    // update counter for next time
    unsigned long old_uid = d_uid_counter;
    ++d_uid_counter;

    // log if we ever wrap-around (which should be pretty unlikely)
    if (d_uid_counter < old_uid)
        Logger::getSingleton().logEvent("UID counter for generated window names has wrapped around - the fun shall now commence!");

    // return generated name as a CEGUI::String.
    return String(uidname.str());
}

CEGUI::String WindowManager::generateUniqueWindowPrefix()
{
    std::ostringstream prefix;
    prefix << d_uid_counter << "_";

    // update counter for next time
    unsigned long old_uid = d_uid_counter;
    ++d_uid_counter;

    // log if we ever wrap-around (which should be pretty unlikely)
    if (d_uid_counter < old_uid)
        Logger::getSingleton().logEvent("UID counter for generated window names has wrapped around - the fun shall now commence!");

    //return generated prefix
    return String(prefix.str());

}

void WindowManager::renameWindow(const String& window, const String& new_name)
{
    renameWindow(getWindow(window), new_name);
}

void WindowManager::renameWindow(Window* window, const String& new_name)
{
    if (window)
    {
        WindowRegistry::iterator pos = d_windowRegistry.find(window->getName());

        if (pos != d_windowRegistry.end())
        {
            // erase old window name from registry
            d_windowRegistry.erase(pos);

            try
            {
                // attempt to rename the window
                window->rename(new_name);
            }
            // rename fails if target name already exists
            catch (AlreadyExistsException&)
            {
                // re-add window to registry under it's old name
                d_windowRegistry[window->getName()] = window;
                // rethrow exception.
                throw;
            }

            // add window to registry under new name
            d_windowRegistry[new_name] = window;
        }
    }
}

/*************************************************************************
    Return a WindowManager::WindowIterator object to iterate over the
    currently defined Windows.
*************************************************************************/
WindowManager::WindowIterator WindowManager::getIterator(void) const
{
    return WindowIterator(d_windowRegistry.begin(), d_windowRegistry.end());
}

/*************************************************************************
    Outputs the names of ALL existing windows to log (DEBUG function).
*************************************************************************/
void WindowManager::DEBUG_dumpWindowNames(String zone)
{
    Logger::getSingleton().logEvent("WINDOW NAMES DUMP (" + zone + ")");
    Logger::getSingleton().logEvent("-----------------");
    CEGUI::WindowManager::WindowIterator windowIt = getIterator();
    while (!windowIt.isAtEnd())
    {
        Logger::getSingleton().logEvent("Window : " + windowIt.getCurrentValue()->getName());
        ++windowIt;
    }
    Logger::getSingleton().logEvent("-----------------");
}

//----------------------------------------------------------------------------//
void WindowManager::lock()
{
    ++d_lockCount;
}

//----------------------------------------------------------------------------//
void WindowManager::unlock()
{
    if (d_lockCount > 0)
        --d_lockCount;
}

//----------------------------------------------------------------------------//
bool WindowManager::isLocked() const
{
    return d_lockCount != 0;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
