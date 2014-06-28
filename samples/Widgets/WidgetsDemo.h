/***********************************************************************
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


#include "CEGUI/ForwardRefs.h"

class EventHandlerObject;
class MyListItem;
struct WidgetPropertiesObject;

typedef std::vector<MyListItem*> WidgetListType;

// Sample class
class WidgetDemo : public Sample
{
public:
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);

    void handleWidgetEventFired(const CEGUI::String& eventName, CEGUI::String logMessage);

    // method to perform any required cleanup operations.
    virtual void deinitialise();
protected:
    // initialisation helpers
    void initialiseAvailableWidgetsMap();

    void addItemToWidgetList(const CEGUI::String& widgetName, WidgetListType &widgetList);
    void createLayout();

    void initialiseWidgetInspector(CEGUI::Window* container);

    void initialiseWidgetPropertiesDisplayWindow(CEGUI::Window* widgetPropertiesInspectionContainer);
    void initialiseWidgetSelector(CEGUI::Window* container);
    void initialiseWidgetDisplayWindow();
    void initialiseEventLights(CEGUI::Window* container);
    void initialiseWidgetSelectorContainer(CEGUI::Window* widgetSelectorContainer);
    void initialiseWidgetSelectorListbox();
    void initialiseBackgroundWindow(CEGUI::Window* background);
    void initialiseSkinCombobox(CEGUI::Window* container);
    void initialiseWidgetsEventsLog();

    void initialiseEventHandlerObjects();

    void createListContent(CEGUI::Window* root);

    bool handleSkinSelectionAccepted(const CEGUI::EventArgs& args);
    bool handleWidgetSelectionChanged(const CEGUI::EventArgs& args);

    void fillWidgetPropertiesDisplayWindow( CEGUI::Window* widgetWindowRoot );
    void handleSpecialWindowCases( CEGUI::Window* widgetWindowRoot, CEGUI::String widgetTypeString );
    CEGUI::Window* retrieveOrCreateWidgetWindow(const CEGUI::String& widgetTypeString, const CEGUI::String& widgetName);
    bool getWidgetType(CEGUI::String &widgetName, CEGUI::String &widgetTypeString);
    CEGUI::Window* createWidget(const CEGUI::String &widgetMapping, const CEGUI::String &widgetType);

    CEGUI::Window* initialiseSpecialWidgets(CEGUI::Window* widgetWindow, const CEGUI::String &widgetType);

    void initMenubar(CEGUI::Menubar* menuBar);
    void initRadioButtons(CEGUI::RadioButton* radioButton, CEGUI::Window*& widgetWindow);
    void initListbox(CEGUI::Listbox* listbox);
    void initItemListbox(CEGUI::ItemListbox* itemListbox);
    void initCombobox(CEGUI::Combobox* combobox);
    void initMultiColumnList(CEGUI::MultiColumnList* multilineColumnList);
    void subscribeToAllEvents(CEGUI::Window* widgetWindow);
    void addEventHandlerObjectToMap(CEGUI::String eventName);

    
    void logFiredEvent(const CEGUI::String& logMessage);

    void destroyWidgetWindows();
    void deinitWidgetListItems();

    bool handleRenderingEnded(const CEGUI::EventArgs& args);
    bool handleRootWindowUpdate(const CEGUI::EventArgs& args);

    void saveWidgetPropertiesToMap(const CEGUI::Window* widgetRoot, const CEGUI::Window* widgetWindow);

    static const CEGUI::String s_widgetDemoWindowPrefix;

    CEGUI::GUIContext* d_guiContext;

    CEGUI::Listbox* d_widgetSelectorListbox;
    CEGUI::Combobox* d_skinSelectionCombobox;
    CEGUI::Window* d_widgetDisplayWindow;
    CEGUI::Window* d_widgetDisplayWindowInnerWindow;
    CEGUI::Window* d_widgetsEventsLog;

    CEGUI::Window* d_currentlyDisplayedWidgetRoot;

    CEGUI::Window* d_windowLightMouseMoveEvent;
    CEGUI::Window* d_windowLightUpdatedEvent;

    CEGUI::MultiColumnList* d_widgetPropertiesDisplayWindow;

    std::map<CEGUI::String, WidgetListType> d_skinListItemsMap;
    std::map<CEGUI::String, CEGUI::Window*> d_widgetsMap;
    std::map<CEGUI::String, EventHandlerObject*> d_eventHandlerObjectsMap;
    std::map<const CEGUI::Window*, WidgetPropertiesObject> d_widgetPropertiesMap;
};

#endif
