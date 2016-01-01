/***********************************************************************
created:    12/7/2012
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & Thce CEGUI Development Team
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
#include "WidgetDemo.h"

#include "CEGUI/CEGUI.h"

#include <cmath>


using namespace CEGUI;

//----------------------------------------------------------------------------//
// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
//----------------------------------------------------------------------------//

class MyListItem : public ListboxTextItem
{
public:
    MyListItem(const String& text, CEGUI::uint item_id = 0) :
      ListboxTextItem(text, item_id)
      {
          setSelectionBrushImage("Vanilla-Images/GenericBrush");
      }
};

//----------------------------------------------------------------------------//
// Helper struct to handle widget properties
//----------------------------------------------------------------------------//
struct WidgetPropertiesObject
{
    std::vector<const CEGUI::Property*> d_propertyList;
    const CEGUI::Window* d_widget;
}; 

//----------------------------------------------------------------------------//
// Helper class to deal with the different event names, used to output the name
// of the event for generic events
//----------------------------------------------------------------------------//
class EventHandlerObject
{
public: 
    EventHandlerObject(CEGUI::String eventName, WidgetDemo* owner);
    bool handleEvent(const CEGUI::EventArgs& args);
private:
    CEGUI::String d_eventName;
    WidgetDemo* d_owner;
};

EventHandlerObject::EventHandlerObject(CEGUI::String eventName, WidgetDemo* owner)
    : d_eventName(eventName),
    d_owner(owner)
{
}

bool EventHandlerObject::handleEvent(const CEGUI::EventArgs& args)
{
    CEGUI::String logMessage = "[colour='FFFFBBBB']" + d_eventName + "[colour='FFFFFFFF']";
    logMessage += CEGUI::String(" (");

    if(dynamic_cast<const CEGUI::MouseEventArgs*>(&args))
    {
        logMessage += "MouseEvent";
    }
    else if(dynamic_cast<const CEGUI::MouseCursorEventArgs*>(&args))
    {
        logMessage += "MouseCursorEvent";
    }
    else if(const CEGUI::KeyEventArgs* keyArgs = dynamic_cast<const CEGUI::KeyEventArgs*>(&args))
    {
        logMessage += "KeyEvent: '" + CEGUI::String(1, keyArgs->codepoint) + "'";
    }
    else if(dynamic_cast<const CEGUI::WindowEventArgs*>(&args))
    {
        logMessage += "WindowEvent";
    }
    else if(dynamic_cast<const CEGUI::ActivationEventArgs*>(&args))
    {
        logMessage += "ActivationEvent";
    }
    else if(dynamic_cast<const CEGUI::DragDropEventArgs*>(&args))
    {
        logMessage += "DragDropEvent";
    }


    logMessage += CEGUI::String(")");

    logMessage += "\n";
    d_owner->handleWidgetEventFired(d_eventName, logMessage);

    return false;
}


//----------------------------------------------------------------------------//
// The following are for the main WidgetDemo class.
//----------------------------------------------------------------------------//

/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
const CEGUI::String WidgetDemo::s_widgetDemoWindowPrefix = "WidgetDemoWindow_";

bool WidgetDemo::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);
    d_guiContext = guiContext;

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
    d_guiContext->getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // load an image to use as a background
    if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
        ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // Retrieve the available widget types and save them inside a map
    initialiseAvailableWidgetsMap();
    initialiseEventHandlerObjects();

    d_currentlyDisplayedWidgetRoot = 0;
    //Create windows and initialise them 
    createLayout();

    d_guiContext->subscribeEvent(CEGUI::GUIContext::EventRenderQueueEnded, Event::Subscriber(&WidgetDemo::handleRenderingEnded, this));
    d_guiContext->getRootWindow()->subscribeEvent(CEGUI::Window::EventUpdated, Event::Subscriber(&WidgetDemo::handleRootWindowUpdate, this));

    if(CEGUI::ListboxItem* skinItem = d_skinSelectionCombobox->getListboxItemFromIndex(0))
    {
        d_skinSelectionCombobox->setItemSelectState(skinItem, true);
        handleSkinSelectionAccepted(CEGUI::WindowEventArgs(d_skinSelectionCombobox));
    }
    if(CEGUI::ListboxItem* widgetItem = d_widgetSelectorListbox->getListboxItemFromIndex(0))
    {
        d_widgetSelectorListbox->setItemSelectState(widgetItem, true);
    }

    // success!
    return true;
}

/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void WidgetDemo::deinitialise()
{
    if(d_currentlyDisplayedWidgetRoot != 0)
        d_widgetDisplayWindowInnerWindow->removeChild(d_currentlyDisplayedWidgetRoot);

    destroyWidgetWindows();

    deinitWidgetListItems();
}

bool WidgetDemo::handleSkinSelectionAccepted(const CEGUI::EventArgs& args)
{
    const WindowEventArgs& winArgs = static_cast<const CEGUI::WindowEventArgs&>(args);

    CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(winArgs.window);
    CEGUI::String schemeName = combobox->getSelectedItem()->getText();

    WidgetListType& widgetsList = d_skinListItemsMap[schemeName];

    d_widgetSelectorListbox->resetList();

    for(unsigned int i = 0; i < widgetsList.size(); ++i)
    {
        MyListItem* item = widgetsList[i];
        d_widgetSelectorListbox->addItem(item);
    }

    // event was handled
    return true;
}

bool WidgetDemo::handleRenderingEnded(const CEGUI::EventArgs&)
{
    d_windowLightMouseMoveEvent->disable();
    d_windowLightUpdatedEvent->disable();

    return true;
}

bool WidgetDemo::handleRootWindowUpdate(const CEGUI::EventArgs& args)
{
    const CEGUI::UpdateEventArgs& updateArgs = static_cast<const CEGUI::UpdateEventArgs&>(args);
    float passedTime = updateArgs.d_timeSinceLastFrame;

    if(d_currentlyDisplayedWidgetRoot == 0)
        return true;

    CEGUI::ProgressBar* progressBar = dynamic_cast<CEGUI::ProgressBar*>(d_currentlyDisplayedWidgetRoot);
    if(progressBar != 0)
    {
        float newProgress = progressBar->getProgress() + passedTime * 0.2f;
        if(newProgress < 1.0f)
            progressBar->setProgress(newProgress);
    }

    return true;
}

bool WidgetDemo::handleWidgetSelectionChanged(const CEGUI::EventArgs&)
{
    //Get widget name and mapping type
    CEGUI::String widgetName;
    CEGUI::String widgetTypeString;

    bool typesFound = getWidgetType(widgetName, widgetTypeString);
    if(!typesFound)
        return true;

    //Clear events log
    d_widgetsEventsLog->setText("");

    //Remove previous children window from the widget-display window
    if(d_currentlyDisplayedWidgetRoot)
        d_widgetDisplayWindowInnerWindow->removeChild(d_currentlyDisplayedWidgetRoot);

    //Get the widget root window
    CEGUI::Window* widgetWindowRoot = retrieveOrCreateWidgetWindow(widgetTypeString, widgetName);

    d_widgetDisplayWindowInnerWindow->addChild(widgetWindowRoot);
    d_currentlyDisplayedWidgetRoot = widgetWindowRoot;
    d_widgetDisplayWindow->setText("Demo of widget: \"" + widgetTypeString + "\"");

    //Special initialisations for certain Windows
    handleSpecialWindowCases(widgetWindowRoot, widgetTypeString);

    //Set the property items for the property inspector
    fillWidgetPropertiesDisplayWindow(widgetWindowRoot);


    // event was handled
    return true;
}


void WidgetDemo::initialiseAvailableWidgetsMap()
{
    //Retrieve the widget look types and add a Listboxitem for each widget, to the right scheme in the map
    CEGUI::WindowFactoryManager& windowFactorymanager = CEGUI::WindowFactoryManager::getSingleton();
    CEGUI::WindowFactoryManager::FalagardMappingIterator falMappingIter = windowFactorymanager.getFalagardMappingIterator();

    while(!falMappingIter.isAtEnd())
    {
        CEGUI::String falagardBaseType = falMappingIter.getCurrentValue().d_windowType;

        int slashPos = falagardBaseType.find_first_of('/');
        CEGUI::String group = falagardBaseType.substr(0, slashPos);
        CEGUI::String name = falagardBaseType.substr(slashPos + 1, falagardBaseType.size() - 1);

        if(group.compare("SampleBrowserSkin") != 0)
        {

            std::map<CEGUI::String, WidgetListType>::iterator iter = d_skinListItemsMap.find(group);
            if(iter == d_skinListItemsMap.end())
            {
                //Create new list
                d_skinListItemsMap[group];
            }

            WidgetListType& widgetList = d_skinListItemsMap.find(group)->second;
            addItemToWidgetList(name, widgetList);
        }

        ++falMappingIter;
    }

    //Add the default types as well
    d_skinListItemsMap["No Skin"];
    WidgetListType& defaultWidgetsList = d_skinListItemsMap["No Skin"];

    addItemToWidgetList("DefaultWindow", defaultWidgetsList);
    addItemToWidgetList("DragContainer", defaultWidgetsList);
    addItemToWidgetList("VerticalLayoutContainer", defaultWidgetsList);
    addItemToWidgetList("HorizontalLayoutContainer", defaultWidgetsList);
    addItemToWidgetList("GridLayoutContainer", defaultWidgetsList);
}


void WidgetDemo::createLayout()
{
    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "BackgroundWindow");
    initialiseBackgroundWindow(background);
    // install this as the root GUI sheet
    d_guiContext->setRootWindow(background);

    initialiseWidgetSelector(background);

    initialiseWidgetInspector(background);
}

void WidgetDemo::initialiseSkinCombobox(CEGUI::Window* container)
{
    WindowManager& winMgr = WindowManager::getSingleton();

    CEGUI::Window* skinSelectionComboboxLabel = winMgr.createWindow("Vanilla/Label", "SkinSelectionComboboxLabel");
    skinSelectionComboboxLabel->setText("Select a Skin and a Widget");
    skinSelectionComboboxLabel->setPosition(CEGUI::UVector2(cegui_reldim(0.65f), cegui_reldim(0.12f)));
    skinSelectionComboboxLabel->setSize(CEGUI::USize(cegui_reldim(0.24f), cegui_reldim(0.07f)));

    d_skinSelectionCombobox = static_cast<CEGUI::Combobox*>(winMgr.createWindow("Vanilla/Combobox", "SkinSelectionCombobox"));
    d_skinSelectionCombobox->setPosition(CEGUI::UVector2(cegui_reldim(0.65f), cegui_reldim(0.2f)));
    d_skinSelectionCombobox->setSize(CEGUI::USize(cegui_reldim(0.24f), cegui_reldim(0.3f)));
    d_skinSelectionCombobox->setReadOnly(true);
    d_skinSelectionCombobox->setSortingEnabled(false);

    d_skinSelectionCombobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, Event::Subscriber(&WidgetDemo::handleSkinSelectionAccepted, this));

    std::map<CEGUI::String, WidgetListType>::iterator iter = d_skinListItemsMap.begin();
    while(iter != d_skinListItemsMap.end())
    {
        d_skinSelectionCombobox->addItem(new MyListItem(iter->first));

        ++iter;
    }

    container->addChild(d_skinSelectionCombobox);
    container->addChild(skinSelectionComboboxLabel);
}

void WidgetDemo::initialiseBackgroundWindow(CEGUI::Window* background)
{
    background->setPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setSize(USize(cegui_reldim(1), cegui_reldim( 1)));
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    background->setProperty("Image", "SpaceBackgroundImage");
}

void WidgetDemo::initialiseWidgetSelectorListbox()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    d_widgetSelectorListbox = static_cast<CEGUI::Listbox*>(winMgr.createWindow("Vanilla/Listbox", "WidgetSelectorListbox"));
    d_widgetSelectorListbox->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.075f)));
    d_widgetSelectorListbox->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(0.925f)));
    d_widgetSelectorListbox->setShowVertScrollbar(false);
    d_widgetSelectorListbox->setSortingEnabled(true);

    d_widgetSelectorListbox->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, Event::Subscriber(&WidgetDemo::handleWidgetSelectionChanged, this));   
}

void WidgetDemo::initialiseWidgetSelectorContainer(CEGUI::Window* widgetSelectorContainer)
{
    widgetSelectorContainer->setPosition(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim(0.25f)));
    widgetSelectorContainer->setSize(CEGUI::USize(cegui_reldim(0.325f), cegui_reldim(0.56f)));
    widgetSelectorContainer->setText("Widget Selector");
    widgetSelectorContainer->setProperty("VertFormatting", "TopAligned");
    widgetSelectorContainer->setProperty("HorzFormatting", "CentreAligned");
}

void WidgetDemo::initialiseWidgetsEventsLog()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    d_widgetsEventsLog = winMgr.createWindow("Vanilla/StaticText", "WidgetEventsLog");
    d_widgetsEventsLog->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.65f)));
    d_widgetsEventsLog->setSize(CEGUI::USize(cegui_reldim(0.9f), cegui_reldim(0.25f)));
    d_widgetsEventsLog->setFont("DejaVuSans-12");

    d_widgetsEventsLog->setProperty("VertScrollbar", "true"); 

    d_widgetsEventsLog->setProperty("HorzFormatting", "WordWrapLeftAligned");

    d_widgetsEventsLog->setProperty("VertFormatting", "TopAligned");
}

/*************************************************************************
Helper function to add MyListItem's to the widget list
*************************************************************************/
void WidgetDemo::addItemToWidgetList(const CEGUI::String& widgetName, WidgetListType &widgetList)
{
    MyListItem* widgetListItem = new MyListItem(widgetName);
    widgetListItem->setAutoDeleted(false);
    widgetList.push_back(widgetListItem);
}

void WidgetDemo::initialiseEventHandlerObjects()
{
#include "AllEvents.inc"

    std::set<String>::iterator iter = allEvents.begin();
    while(iter != allEvents.end())
    {
        addEventHandlerObjectToMap(*iter);

        ++iter;
    }
}

CEGUI::Window* WidgetDemo::createWidget(const CEGUI::String &widgetMapping, const CEGUI::String &widgetType)
{
    //Create default widget of the selected type
    CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();

    CEGUI::Window* widgetWindow = windowManager.createWindow(widgetMapping, s_widgetDemoWindowPrefix + widgetMapping);
    //Subscribe to all possible events the window could fire, the handler will output them to the log
    subscribeToAllEvents(widgetWindow);

    //Set a default text - for Spinners we set no text so it won't cause an exception
    CEGUI::Spinner* spinner = dynamic_cast<CEGUI::Spinner*>(widgetWindow); 
    if(!spinner)
        widgetWindow->setText(widgetType);

    //Create extra widgets and special setups for certain widget types for better demonstration
    CEGUI::Window* widgetWindowRoot = initialiseSpecialWidgets(widgetWindow, widgetType);

    //Get all properties and save them in a map for the properties display window
    saveWidgetPropertiesToMap(widgetWindowRoot, widgetWindow);

    return widgetWindowRoot;
}


void WidgetDemo::handleWidgetEventFired(const CEGUI::String& eventName, CEGUI::String logMessage)
{
    if(eventName == CEGUI::Window::EventMouseMove)
    {
        d_windowLightMouseMoveEvent->enable();
    }
    else if(eventName == CEGUI::Window::EventUpdated)
    {
        d_windowLightUpdatedEvent->enable();
    }
    else
    {
        logFiredEvent(logMessage);
    }
}

void WidgetDemo::addEventHandlerObjectToMap(CEGUI::String eventName)
{
    d_eventHandlerObjectsMap[eventName] = new EventHandlerObject(eventName, this);
}

void WidgetDemo::deinitWidgetListItems()
{
    std::map<CEGUI::String, WidgetListType>::iterator iter = d_skinListItemsMap.begin();
    while(iter != d_skinListItemsMap.end())
    {
        WidgetListType& widgetsList = iter->second;

        while(widgetsList.size() > 0)
        {
            MyListItem* item = widgetsList.back();

            d_widgetSelectorListbox->removeItem(item);
            delete item;
            widgetsList.pop_back();
        }

        ++iter;
    }
}

void WidgetDemo::destroyWidgetWindows()
{
    CEGUI::WindowManager& winMan = CEGUI::WindowManager::getSingleton();
    std::map<CEGUI::String, CEGUI::Window*>::iterator iter = d_widgetsMap.begin();
    while(iter != d_widgetsMap.end())
    {
        winMan.destroyWindow(iter->second);

        ++iter;
    }
}

void WidgetDemo::initialiseEventLights(CEGUI::Window* container)
{
    CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

    CEGUI::Window* horizontalLayout = winMgr.createWindow("HorizontalLayoutContainer", "EventLightsContainer");
    horizontalLayout->setPosition(CEGUI::UVector2(cegui_reldim(0.085f), cegui_reldim(0.93f)));
    container->addChild(horizontalLayout);


    d_windowLightUpdatedEvent = winMgr.createWindow("SampleBrowserSkin/Light");
    horizontalLayout->addChild(d_windowLightUpdatedEvent);
    d_windowLightUpdatedEvent->setSize(CEGUI::USize(cegui_reldim(0.0f), cegui_reldim(0.04f)));
    d_windowLightUpdatedEvent->setAspectMode(CEGUI::AM_EXPAND);
    d_windowLightUpdatedEvent->setProperty("LightColour", "FF66FF66");

    CEGUI::Window* updateEventLabel = winMgr.createWindow("Vanilla/Label");
    horizontalLayout->addChild(updateEventLabel);
    updateEventLabel->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.04f)));
    updateEventLabel->setText("EventUpdated");
    updateEventLabel->setFont("DejaVuSans-12-NoScale");
    updateEventLabel->setProperty("HorzFormatting", "LeftAligned");

    d_windowLightMouseMoveEvent = winMgr.createWindow("SampleBrowserSkin/Light");
    horizontalLayout->addChild(d_windowLightMouseMoveEvent);
    d_windowLightMouseMoveEvent->setSize(CEGUI::USize(cegui_reldim(0.0f), cegui_reldim(0.04f)));
    d_windowLightMouseMoveEvent->setAspectMode(CEGUI::AM_EXPAND);
    d_windowLightMouseMoveEvent->setProperty("LightColour", "FF77BBFF");

    CEGUI::Window* mouseMoveEventLabel = winMgr.createWindow("Vanilla/Label");
    horizontalLayout->addChild(mouseMoveEventLabel);
    mouseMoveEventLabel->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.04f)));
    mouseMoveEventLabel->setText("EventMouseMove");
    mouseMoveEventLabel->setFont("DejaVuSans-12-NoScale");
    mouseMoveEventLabel->setProperty("HorzFormatting", "LeftAligned");
}

void WidgetDemo::logFiredEvent(const CEGUI::String& logMessage)
{
    ListboxItem* item = d_widgetSelectorListbox->getFirstSelectedItem();
    if(!item)
        return;

    CEGUI::String eventsLog = d_widgetsEventsLog->getText();
    eventsLog += logMessage;

    //Remove line
    int pos = std::max<int>(static_cast<int>(eventsLog.length() - 2056), 0);
    int len = std::min<int>(static_cast<int>(eventsLog.length()), 2056);
    eventsLog = eventsLog.substr(pos, len);
    if(len == 2056)
    {
        String::size_type newlinePos = eventsLog.find_first_of("\n");
        if(newlinePos != String::npos)
            eventsLog = eventsLog.substr(newlinePos, String::npos);
    }
    d_widgetsEventsLog->setText(eventsLog);

    //Scroll to end
    CEGUI::Scrollbar* scrollbar = static_cast<CEGUI::Scrollbar*>(d_widgetsEventsLog->getChild("__auto_vscrollbar__"));
    scrollbar->setScrollPosition(scrollbar->getDocumentSize() - scrollbar->getPageSize());
}

void WidgetDemo::subscribeToAllEvents(CEGUI::Window* widgetWindow)
{
    //Register all events for the widget window
    std::map<CEGUI::String, EventHandlerObject*>::iterator iter = d_eventHandlerObjectsMap.begin();
    while(iter != d_eventHandlerObjectsMap.end())
    {
        widgetWindow->subscribeEvent(iter->first, Event::Subscriber(&EventHandlerObject::handleEvent, iter->second));

        ++iter;
    }
}

CEGUI::Window* WidgetDemo::initialiseSpecialWidgets(CEGUI::Window* widgetWindow, const CEGUI::String &widgetType)
{
    CEGUI::RadioButton* radioButton = dynamic_cast<CEGUI::RadioButton*>(widgetWindow);
    if(radioButton)
    {
        initRadioButtons(radioButton, widgetWindow);
    }

    CEGUI::MultiLineEditbox* multilineEditbox = dynamic_cast<CEGUI::MultiLineEditbox*>(widgetWindow);
    if(multilineEditbox || widgetType.compare("StaticText") == 0)
    {
        widgetWindow->setText("Lorem ipsum dolor sit amet, consectetur adipisici elit, sed eiusmod tempor incidunt"
            "ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco"
            "laboris nisi ut aliquid ex ea commodi consequat.Quis aute iure reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\n\n"
            "Excepteur sint obcaecat cupiditat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    }

    if(widgetType.compare("CaptionedStaticText") == 0)
    {
        widgetWindow->setProperty("Text", "Caption");
    }

    if(widgetType.compare("StaticText") == 0)
    {
        if(widgetWindow->isPropertyPresent("VertScrollbar"))
            widgetWindow->setProperty("VertScrollbar", "true"); 

        if(widgetWindow->isPropertyPresent("HorzFormatting"))
            widgetWindow->setProperty("HorzFormatting", "WordWrapLeftAligned");
    }

    if(widgetType.compare("StaticImage") == 0)
    {
        widgetWindow->setProperty("Image", "SpaceBackgroundImage");
    }

    CEGUI::Listbox* listbox = dynamic_cast<CEGUI::Listbox*>(widgetWindow);
    if(listbox)
    {
        initListbox(listbox);
    }

    CEGUI::ComboDropList* combodroplist = dynamic_cast<CEGUI::ComboDropList*>(widgetWindow);
    if(combodroplist)
    {
        initListbox(combodroplist);
    }

    CEGUI::ItemListbox* itemListbox = dynamic_cast<CEGUI::ItemListbox*>(widgetWindow);
    if(itemListbox)
    {
        initItemListbox(itemListbox);
    }

    CEGUI::Combobox* combobox = dynamic_cast<CEGUI::Combobox*>(widgetWindow);
    if(combobox)
    {
        initCombobox(combobox);
    }

    CEGUI::MultiColumnList* multilineColumnList = dynamic_cast<CEGUI::MultiColumnList*>(widgetWindow);
    if(multilineColumnList)
    {
        initMultiColumnList(multilineColumnList);
    }

    CEGUI::Menubar* menuBar = dynamic_cast<CEGUI::Menubar*>(widgetWindow);
    if(menuBar)
    {     
        initMenubar(menuBar);

    }

    return widgetWindow;
}

void WidgetDemo::initMultiColumnList(CEGUI::MultiColumnList* multilineColumnList)
{
    multilineColumnList->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(0.4f)));

    multilineColumnList->addColumn("Server Name", 0, cegui_reldim(0.38f));
    multilineColumnList->addColumn("Address ", 1, cegui_reldim(0.44f));
    multilineColumnList->addColumn("Ping", 2, cegui_reldim(0.15f));

    // Add some empty rows to the MCL
    multilineColumnList->addRow();
    multilineColumnList->addRow();
    multilineColumnList->addRow();
    multilineColumnList->addRow();
    multilineColumnList->addRow();

    // Set first row item texts for the MCL
    multilineColumnList->setItem(new MyListItem("Laggers World"), 0, 0);
    multilineColumnList->setItem(new MyListItem("yourgame.some-server.com"), 1, 0);
    multilineColumnList->setItem(new MyListItem("[colour='FFFF0000']1000ms"), 2, 0);

    // Set second row item texts for the MCL
    multilineColumnList->setItem(new MyListItem("Super-Server"), 0, 1);
    multilineColumnList->setItem(new MyListItem("whizzy.fakenames.net"), 1, 1);
    multilineColumnList->setItem(new MyListItem("[colour='FF00FF00']8ms"), 2, 1);

    // Set third row item texts for the MCL
    multilineColumnList->setItem(new MyListItem("Cray-Z-Eds"), 0, 2);
    multilineColumnList->setItem(new MyListItem("crayzeds.notarealserver.co.uk"), 1, 2);
    multilineColumnList->setItem(new MyListItem("[colour='FF00FF00']43ms"), 2, 2);

    // Set fourth row item texts for the MCL
    multilineColumnList->setItem(new MyListItem("Fake IPs"), 0, 3);
    multilineColumnList->setItem(new MyListItem("123.320.42.242"), 1, 3);
    multilineColumnList->setItem(new MyListItem("[colour='FFFFFF00']63ms"), 2, 3);

    // Set fifth row item texts for the MCL
    multilineColumnList->setItem(new MyListItem("Yet Another Game Server"), 0, 4);
    multilineColumnList->setItem(new MyListItem("abc.abcdefghijklmn.org"), 1, 4);
    multilineColumnList->setItem(new MyListItem("[colour='FFFF6600']284ms"), 2, 4);
}

void WidgetDemo::initCombobox(CEGUI::Combobox* combobox)
{
    MyListItem* item1 = new MyListItem("Combobox Item 1");
    combobox->addItem(item1);
    MyListItem* item2 = new MyListItem("Combobox Item 2");
    combobox->addItem(item2);

    MyListItem* item3 = new MyListItem("Combobox Item 3");
    item3->setSelectionColours(CEGUI::Colour(0.3f, 0.7f, 1.0f, 1.0f));
    combobox->addItem(item3);

    MyListItem* item4 = new MyListItem("Combobox Item 4");
    item4->setSelectionColours(CEGUI::Colour(0.3f, 1.0f, 0.7f, 1.0f));
    combobox->addItem(item4);

    if(combobox->getType().compare("WindowsLook/Combobox") == 0)
    {
        item1->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
        item2->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
        item3->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
        item4->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
    }
}


void WidgetDemo::saveWidgetPropertiesToMap(const CEGUI::Window* widgetRoot, const CEGUI::Window* widgetWindow)
{
    CEGUI::PropertySet::PropertyIterator propertyIter = widgetWindow->getPropertyIterator();

    std::vector<const CEGUI::Property*>& propertyList = d_widgetPropertiesMap[widgetRoot].d_propertyList;
    d_widgetPropertiesMap[widgetRoot].d_widget = widgetWindow;

    while(!propertyIter.isAtEnd())
    {
        propertyList.push_back(*propertyIter);

        ++propertyIter;
    }
}

void WidgetDemo::initListbox(CEGUI::Listbox* listbox)
{
    MyListItem* item1 = new MyListItem("Listbox Item 1");
    listbox->addItem(item1);
    MyListItem* item2 = new MyListItem("Listbox Item 2");
    listbox->addItem(item2);

    MyListItem* item3 = new MyListItem("Listbox Item 3");
    item3->setSelectionColours(CEGUI::Colour(0.3f, 0.7f, 1.0f, 1.0f));
    listbox->addItem(item3);

    MyListItem* item4 = new MyListItem("Listbox Item 4");
    item4->setSelectionColours(CEGUI::Colour(0.3f, 1.0f, 0.7f, 1.0f));
    listbox->addItem(item4);

    if(listbox->getType().compare("WindowsLook/Listbox") == 0)
    {
        item1->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
        item2->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
        item3->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
        item4->setTextColours(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f));
    }
}

void WidgetDemo::initItemListbox(CEGUI::ItemListbox* itemListbox)
{
    CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();

    CEGUI::ItemEntry* itemListboxItem;

    itemListboxItem = static_cast<CEGUI::ItemEntry*>(windowManager.createWindow("TaharezLook/ListboxItem", "ItemListboxTestItem1"));
    itemListbox->addItem(itemListboxItem);
    itemListbox->setText("Item 1");

    itemListboxItem = static_cast<CEGUI::ItemEntry*>(windowManager.createWindow("TaharezLook/ListboxItem", "ItemListboxTestItem2"));
    itemListbox->addItem(itemListboxItem);
    itemListbox->setText("Item 2");

    itemListboxItem = static_cast<CEGUI::ItemEntry*>(windowManager.createWindow("TaharezLook/ListboxItem", "ItemListboxTestItem3"));
    itemListbox->addItem(itemListboxItem);
    itemListbox->setText("Item 3");

    itemListboxItem = static_cast<CEGUI::ItemEntry*>(windowManager.createWindow("TaharezLook/ListboxItem", "ItemListboxTestItem4"));
    itemListbox->addItem(itemListboxItem);
    itemListbox->setText("Item 4");
}

void WidgetDemo::initRadioButtons(CEGUI::RadioButton* radioButton, CEGUI::Window*& widgetWindow)
{
    CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();

    CEGUI::RadioButton* radioButton1 = radioButton;
    widgetWindow = windowManager.createWindow("DefaultWindow", "RadioButtonWidgetDemoRoot");
    widgetWindow->addChild(radioButton1);

    CEGUI::Window* radioButton2 = windowManager.createWindow(radioButton1->getType(), "WidgetDemoRadiobutton1");
    widgetWindow->addChild(radioButton2);
    radioButton2->setText("Additional Radiobutton1");
    radioButton2->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.17f)));

    CEGUI::Window* radioButton3 = windowManager.createWindow(radioButton1->getType(), "WidgetDemoRadiobutton2");
    widgetWindow->addChild(radioButton3);
    radioButton3->setText("Additional Radiobutton2");
    radioButton3->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.27f)));
}

void WidgetDemo::initialiseWidgetDisplayWindow()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    d_widgetDisplayWindow = winMgr.createWindow("Vanilla/FrameWindow", "WidgetDisplayWindow");
    d_widgetDisplayWindow->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.05f)));
    d_widgetDisplayWindow->setSize(CEGUI::USize(cegui_reldim(0.9f), cegui_reldim(0.6f)));
    d_widgetDisplayWindow->setText("Widget Demo");

    d_widgetDisplayWindowInnerWindow =  winMgr.createWindow("DefaultWindow", "WidgetDisplayWindowInnerContainer");
    d_widgetDisplayWindowInnerWindow->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    d_widgetDisplayWindow->addChild(d_widgetDisplayWindowInnerWindow);
}

void WidgetDemo::initialiseWidgetSelector(CEGUI::Window* container)
{
    WindowManager& winMgr = WindowManager::getSingleton();

    initialiseSkinCombobox(container);

    CEGUI::Window* widgetSelectorContainer = winMgr.createWindow("Vanilla/StaticText", "WidgetSelectorContainer");
    initialiseWidgetSelectorContainer(widgetSelectorContainer);
    container->addChild(widgetSelectorContainer);

    initialiseWidgetSelectorListbox();
    widgetSelectorContainer->addChild(d_widgetSelectorListbox);
}

void WidgetDemo::initialiseWidgetInspector(CEGUI::Window* container)
{
    WindowManager& winMgr = WindowManager::getSingleton();

    //Add a tabcontrol serving as WidgetInspector, allowing to switch between events+widgets and the properties display
    TabControl* tabControl = static_cast<TabControl*>(winMgr.createWindow("TaharezLook/TabControl", "WidgetDemoWidgetInspector"));
    container->addChild(tabControl);
    tabControl->setSize(CEGUI::USize(cegui_reldim(0.55f), cegui_reldim(0.96f)));
    tabControl->setPosition(CEGUI::UVector2(cegui_reldim(0.02f), cegui_reldim(0.02f)));

    //Create the respective windows containing the displays
    CEGUI::Window* widgetMainInspectionContainer = winMgr.createWindow("DefaultWindow", "WidgetInspectionContainer");
    CEGUI::Window* widgetPropertiesInspectionContainer = winMgr.createWindow("DefaultWindow", "WidgetPropertiesInspectionContainer");


    //Add the pages to the tab control
    widgetMainInspectionContainer->setText("Widget Inspector");
    tabControl->addTab(widgetMainInspectionContainer);
    widgetPropertiesInspectionContainer->setText("Widget Properties");
    tabControl->addTab(widgetPropertiesInspectionContainer);

    //Create properties window
    initialiseWidgetPropertiesDisplayWindow(widgetPropertiesInspectionContainer);

    //Create the widget display windows
    initialiseWidgetDisplayWindow();
    widgetMainInspectionContainer->addChild(d_widgetDisplayWindow);

    initialiseWidgetsEventsLog();
    widgetMainInspectionContainer->addChild(d_widgetsEventsLog);

    initialiseEventLights(widgetMainInspectionContainer);
}

bool WidgetDemo::getWidgetType(CEGUI::String &widgetName, CEGUI::String &widgetTypeString)
{
    //Retrieving the Strings for the selections
    CEGUI::ListboxItem* widgetListboxItem = d_widgetSelectorListbox->getFirstSelectedItem();
    CEGUI::ListboxItem* skinListboxItem = d_skinSelectionCombobox->getSelectedItem();

    if(!skinListboxItem || !widgetListboxItem)
        return false;

    //Recreate the widget's type as String
    widgetName = widgetListboxItem->getText();

    if(skinListboxItem->getText().compare("No Skin") != 0)
        widgetTypeString= skinListboxItem->getText() + "/";

    widgetTypeString += widgetName;

    return true;
}

CEGUI::Window* WidgetDemo::retrieveOrCreateWidgetWindow(const CEGUI::String& widgetTypeString, const CEGUI::String& widgetName)
{
    //Choose the existing widget if available, otherwise create it and save it to the list
    std::map<CEGUI::String, CEGUI::Window*>::iterator iter = d_widgetsMap.find(widgetTypeString);
    if(iter != d_widgetsMap.end())
    {
        return iter->second;
    }
    else
    {
        d_widgetsMap[widgetTypeString] = createWidget(widgetTypeString, widgetName);
        return d_widgetsMap[widgetTypeString];
    }
}

void WidgetDemo::handleSpecialWindowCases(CEGUI::Window* widgetWindowRoot, CEGUI::String widgetTypeString)
{
    //Reset to 0 progress in case of a progressbar
    CEGUI::ProgressBar* progressBar = dynamic_cast<CEGUI::ProgressBar*>(d_currentlyDisplayedWidgetRoot);
    if(progressBar != 0)
        progressBar->setProgress(0.0f);

    //Apply the tooltip to the widget display window in case of a tooltip
    CEGUI::Tooltip* tooltip = dynamic_cast<CEGUI::Tooltip*>(d_currentlyDisplayedWidgetRoot);
    if(tooltip)
    {
        d_widgetDisplayWindowInnerWindow->setTooltip(tooltip);
        d_widgetDisplayWindowInnerWindow->removeChild(widgetWindowRoot);
        d_widgetDisplayWindowInnerWindow->setTooltipText(widgetTypeString);
        d_currentlyDisplayedWidgetRoot = 0;
    }
    else
        d_widgetDisplayWindowInnerWindow->setTooltip(0);
}

void WidgetDemo::fillWidgetPropertiesDisplayWindow(CEGUI::Window* widgetWindowRoot)
{
    d_widgetPropertiesDisplayWindow->resetList();

    std::vector<const CEGUI::Property*> propertyList = d_widgetPropertiesMap[widgetWindowRoot].d_propertyList;
    const CEGUI::Window* widget = d_widgetPropertiesMap[widgetWindowRoot].d_widget;

    std::vector<const CEGUI::Property*>::iterator iter = propertyList.begin();
    unsigned int i = 0;
    while(iter != propertyList.end())
    {
        const CEGUI::Property* curProperty = *iter;

        // We have to call this function to update the MCL because the items have changed their properties meanwhile and thus are eventually not sorted anymore
        // When the order in the vector is not correct anymore this will result in an assert when adding a row. The following call will sort the list again and thus
        // it will be ensured everything will be sorted before adding a new row.
        d_widgetPropertiesDisplayWindow->handleUpdatedItemData();

        // Add an empty row to the MultiColumnList
        if(i >= d_widgetPropertiesDisplayWindow->getRowCount())
            d_widgetPropertiesDisplayWindow->addRow();

        unsigned int rowID = d_widgetPropertiesDisplayWindow->getRowID(i);

        // Set the first row item (name) for the property
        d_widgetPropertiesDisplayWindow->setItem(new MyListItem(curProperty->getName()), 0, rowID);

        // Set the third row item (type) for the property
        d_widgetPropertiesDisplayWindow->setItem(new MyListItem(curProperty->getDataType()), 1, rowID);

        CEGUI_TRY
        {
            // Set the second row item (value) for the property if it is gettable
            if(widget->isPropertyPresent(curProperty->getName()))
                d_widgetPropertiesDisplayWindow->setItem(new MyListItem(widget->getProperty(curProperty->getName())), 2, rowID);
        }
        CEGUI_CATCH(CEGUI::InvalidRequestException exception)
        {
        }

        ++iter;
        ++i;
    }

    d_widgetPropertiesDisplayWindow->handleUpdatedItemData();
}

void WidgetDemo::initialiseWidgetPropertiesDisplayWindow(CEGUI::Window* widgetPropertiesInspectionContainer)
{
    WindowManager& winMgr = WindowManager::getSingleton();
    d_widgetPropertiesDisplayWindow = static_cast<CEGUI::MultiColumnList*>(
        winMgr.createWindow("TaharezLook/MultiColumnList", "WidgetPropertiesDisplay")
        );

        //Create the properties display window
    d_widgetPropertiesDisplayWindow->setSize(CEGUI::USize(cegui_reldim(0.9f), cegui_reldim(0.9f)));
    d_widgetPropertiesDisplayWindow->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.05f)));

    widgetPropertiesInspectionContainer->addChild(d_widgetPropertiesDisplayWindow);

    d_widgetPropertiesDisplayWindow->addColumn("Name", 0, cegui_reldim(0.45f));
    d_widgetPropertiesDisplayWindow->addColumn("Type ", 1, cegui_reldim(0.25f));
    d_widgetPropertiesDisplayWindow->addColumn("Value", 2, cegui_reldim(0.8f));
   
    d_widgetPropertiesDisplayWindow->setShowHorzScrollbar(false);
    d_widgetPropertiesDisplayWindow->setUserColumnDraggingEnabled(false);
    d_widgetPropertiesDisplayWindow->setUserColumnSizingEnabled(true);

    d_widgetPropertiesDisplayWindow->setSortColumnByID(0);
    d_widgetPropertiesDisplayWindow->setSortDirection(CEGUI::ListHeaderSegment::Ascending);
}

void WidgetDemo::initMenubar(CEGUI::Menubar* menuBar)
{
    CEGUI::String skin = menuBar->getType();
    skin = skin.substr(0, skin.find_first_of('/'));
    CEGUI::String menuItemMapping = skin + "/MenuItem";
    CEGUI::String popupMenuMapping = skin + "/PopupMenu";

    CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton(); 
    CEGUI::MenuItem* fileMenuItem = static_cast<CEGUI::MenuItem*>(windowManager.createWindow(menuItemMapping, "FileMenuItem"));
    fileMenuItem->setText("File");
    menuBar->addChild(fileMenuItem);
 
    CEGUI::PopupMenu* filePopupMenu = static_cast<CEGUI::PopupMenu*>(windowManager.createWindow(popupMenuMapping, "FilePopupMenu"));
    fileMenuItem->addChild(filePopupMenu);

    CEGUI::MenuItem* menuItem;
    menuItem = static_cast<CEGUI::MenuItem*>(windowManager.createWindow(menuItemMapping, "FileTestMenuItem1"));
    menuItem->setText("Open");
    filePopupMenu->addItem(menuItem);

    menuItem = static_cast<CEGUI::MenuItem*>(windowManager.createWindow(menuItemMapping, "FileTestMenuItem2"));
    menuItem->setText("Save");
    filePopupMenu->addItem(menuItem);

    menuItem = static_cast<CEGUI::MenuItem*>(windowManager.createWindow(menuItemMapping, "FileTestMenuItem3"));
    menuItem->setText("Exit");
    filePopupMenu->addItem(menuItem);


    CEGUI::MenuItem* viewMenuItem = static_cast<CEGUI::MenuItem*>(windowManager.createWindow(menuItemMapping, "ViewMenuItem"));
    fileMenuItem->setText("View");
    menuBar->addChild(viewMenuItem);

    CEGUI::PopupMenu* viewPopupMenu = static_cast<CEGUI::PopupMenu*>(windowManager.createWindow(popupMenuMapping, "ViewPopupMenu"));
    viewMenuItem->addChild(viewPopupMenu);

    menuItem = static_cast<CEGUI::MenuItem*>(windowManager.createWindow(menuItemMapping, "ViewTestMenuItem1"));
    menuItem->setText("Midgets");
    viewPopupMenu->addItem(menuItem);
}
/*************************************************************************
Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static WidgetDemo sample;
    return sample;
}
