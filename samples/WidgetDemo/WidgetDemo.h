/***********************************************************************
filename:   WidgetDemo.h
created:    12/7/2012
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _Widget_Demo_
#define _Widget_Demo_

#include "SampleBase.h"

#include <vector>
#include <map>


namespace CEGUI
{
    class Window;
    class Combobox;
    class MultiLineEditbox;
}

class EventHandlerObject;
class MyListItem;

typedef std::vector<MyListItem*> WidgetListType;

// Sample class
class WidgetDemo : public Sample
{
public:
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);

    void handleWidgetEventFired(CEGUI::String eventName);

    // method to perform any required cleanup operations.
    virtual void deinitialise();
protected:
    // initialisation helpers
    void initialiseAvailableWidgetsMap();

    void addItemToWidgetList(const CEGUI::String& widgetName, WidgetListType &widgetList);
    void createLayout();

    void initialiseEventLights(CEGUI::Window* background);
    void initialiseWidgetSelectorContainer(CEGUI::Window* widgetSelectorContainer);
    void initialiseWidgetSelectorListbox();
    void initialiseBackgroundWindow(CEGUI::Window* background);
    void initialiseSkinCombobox();
    void initialiseWidgetsEventsLog();

    void initialiseEventHandlerObjects();

    void createListContent(CEGUI::Window* root);

    bool handleSkinSelectionAccepted(const CEGUI::EventArgs& args);
    bool handleWidgetSelectionChanged(const CEGUI::EventArgs& args);

    CEGUI::Window* createWidget(CEGUI::String &widgetTypeString);

    void addEventHandlerObjectToMap(CEGUI::String eventName);

    
    void logFiredEvent(const CEGUI::String& eventName);

    void destroyWidgetWindows();
    void deinitWidgetListItems();

    bool handleRenderingEnded(const CEGUI::EventArgs& args);


    static const CEGUI::String s_widgetDemoWindowPrefix;

    CEGUI::GUIContext* d_guiContext;

    CEGUI::Listbox* d_widgetSelectorListbox;
    CEGUI::Combobox* d_skinSelectionCombobox;
    CEGUI::Window* d_widgetDisplayWindow;
    CEGUI::MultiLineEditbox* d_widgetsEventsLog;

    CEGUI::Window* d_currentlyDisplayedWidgetRoot;

    CEGUI::Window* d_windowLightMouseMoveEvent;
    CEGUI::Window* d_windowLightUpdatedEvent;

    std::map<CEGUI::String, WidgetListType> d_skinListItemsMap;
    std::map<CEGUI::String, CEGUI::Window*> d_widgetsMap;
    std::map<CEGUI::String, EventHandlerObject*> d_eventHandlerObjectsMap;
};

#endif
