/***********************************************************************
    filename:   CEGUIGUILayout_xmlHandler.cpp
    created:    5/7/2004
    author:     Paul D Turner

    purpose:    Implements XML parser for GUILayout files
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
#include "CEGUIGUILayout_xmlHandler.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIScriptModule.h"
#include "CEGUIXMLAttributes.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Implementation Constants
*************************************************************************/
const String GUILayout_xmlHandler::GUILayoutElement( "GUILayout" );
const String GUILayout_xmlHandler::WindowElement( "Window" );
const String GUILayout_xmlHandler::AutoWindowElement( "AutoWindow" );
const String GUILayout_xmlHandler::PropertyElement( "Property" );
const String GUILayout_xmlHandler::LayoutImportElement( "LayoutImport" );
const String GUILayout_xmlHandler::EventElement( "Event" );
const String GUILayout_xmlHandler::WindowTypeAttribute( "Type" );
const String GUILayout_xmlHandler::WindowNameAttribute( "Name" );
const String GUILayout_xmlHandler::AutoWindowNameSuffixAttribute( "NameSuffix" );
const String GUILayout_xmlHandler::PropertyNameAttribute( "Name" );
const String GUILayout_xmlHandler::PropertyValueAttribute( "Value" );
const String GUILayout_xmlHandler::LayoutParentAttribute( "Parent" );
const String GUILayout_xmlHandler::LayoutImportFilenameAttribute( "Filename" );
const String GUILayout_xmlHandler::LayoutImportPrefixAttribute( "Prefix" );
const String GUILayout_xmlHandler::LayoutImportResourceGroupAttribute( "ResourceGroup" );
const String GUILayout_xmlHandler::EventNameAttribute( "Name" );
const String GUILayout_xmlHandler::EventFunctionAttribute( "Function" );

void GUILayout_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
{
    // handle root GUILayoutElement element
    if (element == GUILayoutElement)
    {
        elementGUILayoutStart(attributes);
    }
    // handle Window element (create window and make an entry on our "window stack")
    else if (element == WindowElement)
    {
        elementWindowStart(attributes);
    }
    // handle AutoWindow element (get an auto child from the window on top of the "window stack")
    else if (element == AutoWindowElement)
    {
        elementAutoWindowStart(attributes);
    }
    // handle Property element (set property for window at top of stack)
    else if (element == PropertyElement)
    {
        elementPropertyStart(attributes);
    }
    // handle layout import element (attach a layout to the window at the top of the stack)
    else if (element == LayoutImportElement)
    {
        elementLayoutImportStart(attributes);
    }
    // handle event subscription element
    else if (element == EventElement)
    {
        elementEventStart(attributes);
    }
    // anything else is an error which *should* have already been caught by XML validation
    else
    {
        Logger::getSingleton().logEvent("GUILayout_xmlHandler::startElement - Unexpected data was found while parsing the gui-layout file: '" + element + "' is unknown.", Errors);
    }
}

void GUILayout_xmlHandler::elementEnd(const String& element)
{
    // handle root GUILayoutElement element
    if (element == GUILayoutElement)
    {
        elementGUILayoutEnd();
    }
    // handle Window element
    else if (element == WindowElement)
    {
        elementWindowEnd();
    }
    // handle Window element
    else if (element == AutoWindowElement)
    {
        elementAutoWindowEnd();
    }
    // handle Property element
    else if (element == PropertyElement)
    {
        elementPropertyEnd();
    }
}


/*************************************************************************
    Put free text into the propertvalue buffer
*************************************************************************/
void GUILayout_xmlHandler::text(const String& text)
{
    d_propertyValue += text;
}

/*************************************************************************
    Destroy all windows created so far.
*************************************************************************/
void GUILayout_xmlHandler::cleanupLoadedWindows(void)
{
    // Notes: We could just destroy the root window of the layout, which normally would also destroy
    // all attached windows.  Since the client may have specified that certain windows are not auto-destroyed
    // we can't rely on this, so we work backwards detaching and deleting windows instead.
    while (!d_stack.empty())
    {
        // only destroy if not an auto window
        if (d_stack.back().second)
        {
            Window* wnd = d_stack.back().first;

            // detach from parent
            if (wnd->getParent())
            {
                wnd->getParent()->removeChildWindow(wnd);
            }

            // destroy the window
            WindowManager::getSingleton().destroyWindow(wnd);
        }

        // pop window from stack
        d_stack.pop_back();
    }

    d_root = 0;
}


/*************************************************************************
    Return a pointer to the 'root' window created.
*************************************************************************/
Window* GUILayout_xmlHandler::getLayoutRootWindow(void) const
{
    return d_root;
}

/*************************************************************************
    Method that handles the opening GUILayout XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementGUILayoutStart(const XMLAttributes& attributes)
{
    d_layoutParent = attributes.getValueAsString(LayoutParentAttribute);

    // before we go to the trouble of creating the layout, see if this parent exists
    if (!d_layoutParent.empty())
    {
        if (!WindowManager::getSingleton().isWindowPresent(d_layoutParent))
        {
            // signal error - with more info about what we have done.
            throw InvalidRequestException("GUILayout_xmlHandler::startElement - layout loading has been aborted since the specified parent Window ('" + d_layoutParent + "') does not exist.");
        }
    }
}

/*************************************************************************
    Method that handles the opening Window XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementWindowStart(const XMLAttributes& attributes)
{
    // get type of window to create
    String windowType(attributes.getValueAsString(WindowTypeAttribute));
    // get name for new window
    String windowName(attributes.getValueAsString(WindowNameAttribute));

    // attempt to create window
    try
    {
        Window* wnd = WindowManager::getSingleton().createWindow(windowType, windowName, d_namingPrefix);

        // add this window to the current parent (if any)
        if (!d_stack.empty())
            d_stack.back().first->addChildWindow(wnd);
        else
            d_root = wnd;

        // make this window the top of the stack
        d_stack.push_back(WindowStackEntry(wnd,true));

        // tell it that it is being initialised
        wnd->beginInitialisation();
    }
    catch (AlreadyExistsException&)
    {
        // delete all windows created
        cleanupLoadedWindows();

        // signal error - with more info about what we have done.
        throw InvalidRequestException("GUILayout_xmlHandler::startElement - layout loading has been aborted since Window named '" + windowName + "' already exists.");
    }
    catch (UnknownObjectException&)
    {
        // delete all windows created
        cleanupLoadedWindows();

        // signal error - with more info about what we have done.
        throw InvalidRequestException("GUILayout_xmlHandler::startElement - layout loading has been aborted since no WindowFactory is available for '" + windowType + "' objects.");
    }
}

/*************************************************************************
    Method that handles the opening AutoWindow XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementAutoWindowStart(const XMLAttributes& attributes)
{
    // get window name
    String nameSuffix(attributes.getValueAsString(AutoWindowNameSuffixAttribute));
    String windowName;

    // attempt to create window
    try
    {
        // we need a window to fetch children
        if (!d_stack.empty())
        {
            windowName = d_stack.back().first->getName() + nameSuffix;
            Window* wnd = WindowManager::getSingleton().getWindow(windowName);
            // make this window the top of the stack
            d_stack.push_back(WindowStackEntry(wnd,false));
        }
    }
    catch (UnknownObjectException&)
    {
        // delete all windows created
        cleanupLoadedWindows();

        // signal error - with more info about what we have done.
        throw InvalidRequestException("GUILayout_xmlHandler::startElement - layout loading has been aborted since no auto window named '" + windowName + "' exists.");
    }
}

/*************************************************************************
    Method that handles the Property XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementPropertyStart(const XMLAttributes& attributes)
{
    // get property name
    String propertyName(attributes.getValueAsString(PropertyNameAttribute));

    // get property value string
    String propertyValue;
    if (attributes.exists(PropertyValueAttribute))
    {
        propertyValue = attributes.getValueAsString(PropertyValueAttribute);
    }

    // Short property 
    if (!propertyValue.empty())
    {
        d_propertyName.clear();
        try
        {
            // need a window to be able to set properties!
            if (!d_stack.empty())
            {
                // get current window being defined.
                Window* curwindow = d_stack.back().first;

                bool useit = true;

                // if client defined a callback, call it and discover if we should
                // set the property.
                if (d_propertyCallback)
                    useit = (*d_propertyCallback)(curwindow,
                                                  propertyName,
                                                  propertyValue,
                                                  d_userData);
                // set the property as needed
                if (useit)
                    curwindow->setProperty(propertyName, propertyValue);
            }
        }
        catch (Exception&)
        {
            // Don't do anything here, but the error will have been logged.
        }
    }
    // Long property 
    else 
    {
        // Store name for later use 
        d_propertyName = propertyName;
        // reset the property value buffer
        d_propertyValue.clear();
    }
}

/*************************************************************************
    Method that handles the LayoutImport XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementLayoutImportStart(const XMLAttributes& attributes)
{
    // get base prefix
    String prefixName(d_namingPrefix);
    // append the prefix specified in the layout doing the import
    prefixName += attributes.getValueAsString(LayoutImportPrefixAttribute);

    try
    {
        // attempt to load the imported sub-layout
        Window* subLayout = WindowManager::getSingleton().loadWindowLayout(
                attributes.getValueAsString( LayoutImportFilenameAttribute),
                prefixName,
                attributes.getValueAsString(LayoutImportResourceGroupAttribute),
                d_propertyCallback,
                d_userData);

        // attach the imported layout to the window being defined
        if ((subLayout != 0) && (!d_stack.empty()))
            d_stack.back().first->addChildWindow(subLayout);
    }
    // something failed when loading the sub-layout
    catch (Exception&)
    {
        // delete all windows created so far
        cleanupLoadedWindows();

        // signal error - with more info about what we have done.
        throw GenericException("GUILayout_xmlHandler::startElement - layout loading aborted due to imported layout load failure (see error(s) above).");
    }
}

/*************************************************************************
    Method that handles the Event XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementEventStart(const XMLAttributes& attributes)
{
    // get name of event
    String eventName(attributes.getValueAsString(EventNameAttribute));
    // get name of function
    String functionName(attributes.getValueAsString(EventFunctionAttribute));

    // attempt to subscribe property on window
    try
    {
        if (!d_stack.empty())
            d_stack.back().first->subscribeScriptedEvent(eventName, functionName);
    }
    catch (Exception&)
    {
        // Don't do anything here, but the error will have been logged.
    }
}

/*************************************************************************
    Method that handles the closing GUILayout XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementGUILayoutEnd()
{
    // attach to named parent if needed
    if (!d_layoutParent.empty() && (d_root != 0))
    {
        WindowManager::getSingleton().getWindow(d_layoutParent)->addChildWindow(d_root);
    }
}

/*************************************************************************
    Method that handles the closing Window XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementWindowEnd()
{
    // pop a window from the window stack
    if (!d_stack.empty())
    {
        d_stack.back().first->endInitialisation();
        d_stack.pop_back();
    }
}

/*************************************************************************
    Method that handles the closing AutoWindow XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementAutoWindowEnd()
{
    // pop a window from the window stack
    if (!d_stack.empty())
    {
        d_stack.pop_back();
    }
}

/*************************************************************************
    Method that handles the closing Property XML element.
*************************************************************************/
void GUILayout_xmlHandler::elementPropertyEnd()
{
    // only do something if this is a "long" property
    if (d_propertyName.empty())
    {
        return;
    }
    try
    {
        // need a window to be able to set properties!
        if (!d_stack.empty())
        {
            // get current window being defined.
            Window* curwindow = d_stack.back().first;

            bool useit = true;

            // if client defined a callback, call it and discover if we should
            // set the property.
            if (d_propertyCallback)
                useit = (*d_propertyCallback)(curwindow,
                                              d_propertyName,
                                              d_propertyValue,
                                              d_userData);
            // set the property as needed
            if (useit)
                curwindow->setProperty(d_propertyName, d_propertyValue);
        }
    }
    catch (Exception&)
    {
        // Don't do anything here, but the error will have been logged.
    }
}
} // End of  CEGUI namespace section
