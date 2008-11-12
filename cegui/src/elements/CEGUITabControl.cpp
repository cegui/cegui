/***********************************************************************
	filename: 	CEGUITabControl.cpp
	created:	08/08/2004
	author:		Steve Streeting

	purpose:	Implementation of Tab Control widget base class
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
#include "CEGUIExceptions.h"
#include "elements/CEGUITabControl.h"
#include "elements/CEGUITabButton.h"
#include "elements/CEGUIPushButton.h"
#include "elements/CEGUIGUISheet.h"
#include "CEGUIFont.h"
#include "CEGUIWindowManager.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String TabControl::EventNamespace("TabControl");
const String TabControl::WidgetTypeName("CEGUI/TabControl");

/*************************************************************************
    TabControlWindowRenderer
*************************************************************************/
TabControlWindowRenderer::TabControlWindowRenderer(const String& name) :
    WindowRenderer(name, TabControl::EventNamespace)
{
}

/*************************************************************************
	Definition of Properties for this class
*************************************************************************/
TabControlProperties::TabHeight		            TabControl::d_tabHeightProperty;
TabControlProperties::TabTextPadding		    TabControl::d_tabTextPaddingProperty;
TabControlProperties::TabPanePosition		    TabControl::d_tabPanePosition;

/*************************************************************************
	Constants
*************************************************************************/
// event names
const String TabControl::EventSelectionChanged( "TabSelectionChanged" );

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String TabControl::ContentPaneNameSuffix( "__auto_TabPane__" );
const String TabControl::TabButtonNameSuffix( "__auto_btn" );
const String TabControl::TabButtonPaneNameSuffix( "__auto_TabPane__Buttons" );
const String TabControl::ButtonScrollLeftSuffix( "__auto_TabPane__ScrollLeft" );
const String TabControl::ButtonScrollRightSuffix( "__auto_TabPane__ScrollRight" );

/*************************************************************************
    Miscelaneous private strings
*************************************************************************/
static const String EnableTop = "EnableTop";
static const String EnableBottom = "EnableBottom";
static const String n0 = "0";
static const String n1 = "1";

/*************************************************************************
	Constructor for TabControl base class.
*************************************************************************/
TabControl::TabControl(const String& type, const String& name)
    : Window(type, name),
    d_tabHeight(0, -1), // means 'to be initialized later'
    d_tabPadding(0, 5),
    d_firstTabOffset(0),
    d_tabPanePos(Top)
{
	addTabControlProperties();
}


/*************************************************************************
	Destructor for TabControl base class.
*************************************************************************/
TabControl::~TabControl(void)
{
    // Should be handled in superclass (all child windows)
}

/*************************************************************************
	Initialise the Window based object ready for use.
*************************************************************************/
void TabControl::initialiseComponents(void)
{
	performChildWindowLayout();

    String name = getName () + ButtonScrollLeftSuffix;
    if (WindowManager::getSingleton().isWindowPresent (name))
        WindowManager::getSingleton().getWindow (name)->subscribeEvent (
            PushButton::EventClicked, Event::Subscriber(
                &CEGUI::TabControl::handleScrollPane, this));

    name = getName() + ButtonScrollRightSuffix;
    if (WindowManager::getSingleton().isWindowPresent (name))
        WindowManager::getSingleton().getWindow (name)->subscribeEvent (
            PushButton::EventClicked, Event::Subscriber(
                &CEGUI::TabControl::handleScrollPane, this));
}
/*************************************************************************
Get the number of tabs
*************************************************************************/
size_t TabControl::getTabCount(void) const
{
    return getTabPane()->getChildCount();
}
/*************************************************************************
Get the tab with a given name
*************************************************************************/
Window*	TabControl::getTabContents(const String& name) const
{
    return getTabPane()->getChild(name);
}
/*************************************************************************
Get the tab at a given ID
*************************************************************************/
Window*	TabControl::getTabContents(uint ID) const
{
    return getTabPane()->getChild(ID);
}
/*************************************************************************
Get the tab for the given index
*************************************************************************/
Window*	TabControl::getTabContentsAtIndex(size_t index) const
{
    if (index >= d_tabButtonVector.size ())
        return 0;
    return d_tabButtonVector [index]->getTargetWindow();
}

/*************************************************************************
Return whether the tab content window is currently selected.
*************************************************************************/
bool TabControl::isTabContentsSelected(Window* wnd) const
{
	TabButton* button = getButtonForTabContents(wnd);
	return button->isSelected();
}

/*************************************************************************
Return whether the tab content window is currently selected.
*************************************************************************/
size_t TabControl::getSelectedTabIndex() const
{
    for (size_t i = 0; i < d_tabButtonVector.size (); ++i)
        if (d_tabButtonVector [i]->isSelected ())
            return i;

	throw UnknownObjectException("TabControl::getSelectedTabIndex - Current tab not in list?");
}

/*************************************************************************
Set the selected tab by window name
*************************************************************************/
void TabControl::setSelectedTab(const String &name)
{
    selectTab_impl(getTabPane()->getChild(name));
}
/*************************************************************************
Set the selected tab by window ID
*************************************************************************/
void TabControl::setSelectedTab(uint ID)
{
    selectTab_impl(getTabPane()->getChild(ID));
}
/*************************************************************************
Set the selected tab by window name
*************************************************************************/
void TabControl::setSelectedTabAtIndex(size_t index)
{
	selectTab_impl(getTabContentsAtIndex(index));
}

/*************************************************************************
Make the tab by window name visible
*************************************************************************/
void TabControl::makeTabVisible(const String &name)
{
    makeTabVisible_impl(getTabPane()->getChild(name));
}
/*************************************************************************
Make the tab by window ID visible
*************************************************************************/
void TabControl::makeTabVisible(uint ID)
{
    makeTabVisible_impl(getTabPane()->getChild(ID));
}
/*************************************************************************
Make the tab by window name visible
*************************************************************************/
void TabControl::makeTabVisibleAtIndex(size_t index)
{
	makeTabVisible_impl(getTabContentsAtIndex(index));
}

/*************************************************************************
Set the tab height
*************************************************************************/
void TabControl::setTabHeight(const UDim& height)
{
    d_tabHeight = height;

    performChildWindowLayout();
}

/*************************************************************************
Set the tab text padding
*************************************************************************/
void TabControl::setTabTextPadding(const UDim& padding)
{
    d_tabPadding = padding;

    performChildWindowLayout();
}

/*************************************************************************
Add a new tab
*************************************************************************/
void TabControl::addTab(Window* wnd)
{
    // abort attempts to add null window pointers, but log it for tracking.
    if (!wnd)
    {
        Logger::getSingleton().logEvent("Attempt to add null window pointer as "
            "tab to TabControl '" + getName() + "'.  Ignoring!", Informative);

        return;
    }

    // Create a new TabButton
    addButtonForTabContent(wnd);
    // Add the window to the content pane
    getTabPane()->addChildWindow(wnd);
    // Auto-select?
    if (getTabCount() == 1)
        setSelectedTab(wnd->getName());
    else
        // initialise invisible content
        wnd->setVisible(false);

	// when adding the 1st page, autosize tab pane height
    if (d_tabHeight.d_scale == 0 && d_tabHeight.d_offset == -1)
        d_tabHeight.d_offset = 8 + getFont()->getFontHeight ();

    // Just request redraw
    performChildWindowLayout();
    requestRedraw();
    // Subscribe to text changed event so that we can resize as needed
    d_eventConnections[wnd] =
        wnd->subscribeEvent(Window::EventTextChanged,
            Event::Subscriber(&TabControl::handleContentWindowTextChanged,
                              this));
}
/*************************************************************************
Remove a tab
*************************************************************************/
void TabControl::removeTab(const String& name)
{
    // do nothing if given window is not attached as a tab.
    if (getTabPane()->isChild(name))
        removeTab_impl(getTabPane()->getChild(name));
}
/*************************************************************************
Remove a tab by ID
*************************************************************************/
void TabControl::removeTab(uint ID)
{
    // do nothing if given window is not attached as a tab.
    if (getTabPane()->isChild(ID))
        removeTab_impl(getTabPane()->getChild(ID));
}
/*************************************************************************
Add tab button
*************************************************************************/
void TabControl::addButtonForTabContent(Window* wnd)
{
    // Create the button
    TabButton* tb = createTabButton(makeButtonName(wnd));
    // Copy font
    tb->setFont(getFont());
    // Set target window
    tb->setTargetWindow(wnd);
    // Instert into map
    d_tabButtonVector.push_back(tb);
    // add the button
    getTabButtonPane()->addChildWindow(tb);
    // Subscribe to clicked event so that we can change tab
    tb->subscribeEvent(TabButton::EventClicked,
        Event::Subscriber(&TabControl::handleTabButtonClicked, this));
    tb->subscribeEvent(TabButton::EventDragged,
        Event::Subscriber(&TabControl::handleDraggedPane, this));
    tb->subscribeEvent(TabButton::EventScrolled,
        Event::Subscriber(&TabControl::handleWheeledPane, this));
}

/*************************************************************************
	Return the tab button for the given tab content window
*************************************************************************/
TabButton* TabControl::getButtonForTabContents(Window* wnd) const
{
    for (size_t i = 0; i < d_tabButtonVector.size (); ++i)
        if (d_tabButtonVector [i]->getTargetWindow () == wnd)
            return d_tabButtonVector [i];

	throw UnknownObjectException("TabControl::getButtonForTabContents - The Window object is not a tab contents.");
}
/*************************************************************************
Remove tab button
*************************************************************************/
void TabControl::removeButtonForTabContent(Window* wnd)
{
    // get
    TabButton* tb = static_cast<TabButton*>(
        getTabButtonPane()->getChild(makeButtonName(wnd)));
    // remove
    TabButtonVector::iterator i;
    for (i = d_tabButtonVector.begin (); i < d_tabButtonVector.end (); ++i)
        if (*i == tb)
        {
            d_tabButtonVector.erase (i);
            break;
        }
    getTabButtonPane()->removeChildWindow(tb);
	// destroy
	WindowManager::getSingleton().destroyWindow(tb);
}
/*************************************************************************
Remove tab button
*************************************************************************/
String TabControl::makeButtonName(Window* wnd)
{
    // Create the button name as "'auto' parent + 'auto' button + tab name"
    String buttonName = getTabButtonPane()->getName();
    buttonName.append(TabButtonNameSuffix);
    size_t pathEndPos = wnd->getName().find_last_of("/");
    buttonName.append(wnd->getName().substr(pathEndPos == String::npos ? 0 : pathEndPos + 1));
    return buttonName;
}
/*************************************************************************
Select tab implementation
*************************************************************************/
void TabControl::selectTab_impl(Window* wnd)
{
    makeTabVisible_impl(wnd);

    bool modified = false;
    // Iterate in order of tab index
    for (size_t i = 0; i < d_tabButtonVector.size(); ++i)
    {
        // get corresponding tab button and content window
        TabButton* tb = d_tabButtonVector [i];
        Window* child = tb->getTargetWindow();
        // Should we be selecting?
        bool selectThis = (child == wnd);
        // Are we modifying this tab?
        modified = modified || (tb->isSelected() != selectThis);
        // Select tab & set visible if this is the window, not otherwise
        tb->setSelected(selectThis);
        child->setVisible(selectThis);
    }
    // Trigger event?
    if (modified)
    {
        WindowEventArgs args(this);
        onSelectionChanged(args);
    }
}
/*************************************************************************
Make tab visible implementation
*************************************************************************/
void TabControl::makeTabVisible_impl(Window* wnd)
{
    TabButton *tb = 0;

    for (size_t i = 0; i < d_tabButtonVector.size (); ++i)
    {
        // get corresponding tab button and content window
        tb = d_tabButtonVector [i];
        Window* child = tb->getTargetWindow();
        if (child == wnd)
            break;
        tb = 0;
    }

    if (!tb)
        return;

    float ww = getPixelSize ().d_width;
    float x = tb->getXPosition().asAbsolute (ww);
    float w = tb->getPixelSize ().d_width;
    float lx = 0, rx = ww;

    Window *scrollLeftBtn = 0, *scrollRightBtn = 0;
    String name = getName() + ButtonScrollLeftSuffix;
    if (WindowManager::getSingleton().isWindowPresent (name))
    {
        scrollLeftBtn = WindowManager::getSingleton().getWindow (name);
        lx = scrollLeftBtn->getArea ().d_max.d_x.asAbsolute (ww);
        scrollLeftBtn->setWantsMultiClickEvents (false);
    }

    name = getName() + ButtonScrollRightSuffix;
    if (WindowManager::getSingleton().isWindowPresent (name))
    {
        scrollRightBtn = WindowManager::getSingleton().getWindow (name);
        rx = scrollRightBtn->getXPosition ().asAbsolute (ww);
        scrollRightBtn->setWantsMultiClickEvents (false);
    }

    if (x < lx)
        d_firstTabOffset += lx - x;
    else
    {
        if (x + w <= rx)
            return; // nothing to do

        d_firstTabOffset += rx - (x + w);
    }

    performChildWindowLayout ();
}
/*************************************************************************
Add tab control properties
*************************************************************************/
void TabControl::addTabControlProperties(void)
{
    addProperty(&d_tabHeightProperty);
    addProperty(&d_tabTextPaddingProperty);
    addProperty(&d_tabPanePosition);
}
/*************************************************************************
Internal version of adding a child window
*************************************************************************/
void TabControl::addChild_impl(Window* wnd)
{
    // Look for __auto_TabPane__ in the name (hopefully no-one will use this!)
    if (wnd->getName().find(ContentPaneNameSuffix) != String::npos)
    {
        // perform normal addChild
        Window::addChild_impl(wnd);
    }
    else
    {
        // This is another control, therefore add as a tab
        addTab(wnd);
    }
}
/*************************************************************************
Internal version of removing a child window
*************************************************************************/
void TabControl::removeChild_impl(Window* wnd)
{
    // protect against possible null pointers
    if (!wnd) return;

    // Look for __auto_TabPane__ in the name (hopefully no-one will use this!)
    if (wnd->getName().find(ContentPaneNameSuffix) != String::npos)
    {
        // perform normal removeChild
        Window::removeChild_impl(wnd);
    }
    else
    {
        // This is some user window, therefore remove as a tab
        removeTab(wnd->getName());
    }
}
/*************************************************************************
Selection changed event
*************************************************************************/
void TabControl::onSelectionChanged(WindowEventArgs& e)
{
    requestRedraw();
    fireEvent(EventSelectionChanged, e, EventNamespace);
}
/*************************************************************************
Font changed event
*************************************************************************/
void TabControl::onFontChanged(WindowEventArgs& e)
{
    // Propagate font change to buttons
    for (size_t i = 0; i < d_tabButtonVector.size(); ++i)
        d_tabButtonVector [i]->setFont (getFont ());
}
/*************************************************************************
	Calculate size and position for a tab button
*************************************************************************/
void TabControl::calculateTabButtonSizePosition(size_t index)
{
    TabButton* btn = d_tabButtonVector [index];
    // relative height is always 1.0 for buttons since they are embedded in a
    // panel of the correct height already
    btn->setHeight(cegui_reldim(1.0f));
    btn->setYPosition(cegui_absdim(0.0f));
    // x position is based on previous button
    if (!index)
        // First button
        btn->setXPosition(cegui_absdim(d_firstTabOffset));
    else
    {
		Window* prevButton = d_tabButtonVector [index - 1];

		// position is prev pos + width
        btn->setXPosition(prevButton->getArea().d_max.d_x);
    }
    // Width is based on font size (expressed as absolute)
    Font* fnt = btn->getFont();
    btn->setWidth(cegui_absdim(fnt->getTextExtent(btn->getText())) +
                        getTabTextPadding() + getTabTextPadding());

    float left_x = btn->getXPosition ().d_offset;
    btn->setVisible ((left_x < getPixelSize ().d_width) &&
                     (left_x + btn->getPixelSize ().d_width > 0));
    btn->requestRedraw();
}
/*************************************************************************
Layout the widgets
*************************************************************************/
void TabControl::performChildWindowLayout()
{
    Window* tabButtonPane = getTabButtonPane();
    Window* tabContentPane = getTabPane();

    // Enable top/bottom edges of the tabPane control,
    // if supported by looknfeel
    if (tabContentPane->isPropertyPresent (EnableTop))
        tabContentPane->setProperty (EnableTop, (d_tabPanePos == Top) ? n0 : n1);
    if (tabContentPane->isPropertyPresent (EnableBottom))
        tabContentPane->setProperty (EnableBottom, (d_tabPanePos == Top) ? n1 : n0);
    if (tabButtonPane->isPropertyPresent (EnableTop))
        tabButtonPane->setProperty (EnableTop, (d_tabPanePos == Top) ? n0 : n1);
    if (tabButtonPane->isPropertyPresent (EnableBottom))
        tabButtonPane->setProperty (EnableBottom, (d_tabPanePos == Top) ? n1 : n0);

    Window::performChildWindowLayout();

    // Calculate the size & position of the tab scroll buttons
    Window *scrollLeftBtn = 0, *scrollRightBtn = 0;
    String name = getName() + ButtonScrollLeftSuffix;
    if (WindowManager::getSingleton().isWindowPresent (name))
        scrollLeftBtn = WindowManager::getSingleton().getWindow (name);

    name = getName() + ButtonScrollRightSuffix;
    if (WindowManager::getSingleton().isWindowPresent (name))
        scrollRightBtn = WindowManager::getSingleton().getWindow (name);

    // Calculate the positions and sizes of the tab buttons
    if (d_firstTabOffset > 0)
        d_firstTabOffset = 0;

    for (;;)
    {
        size_t i;
        for (i = 0; i < d_tabButtonVector.size (); ++i)
            calculateTabButtonSizePosition (i);

        if (d_tabButtonVector.empty ())
        {
            if (scrollRightBtn)
                scrollRightBtn->setVisible (false);
            if (scrollLeftBtn)
                scrollLeftBtn->setVisible (false);
            break;
        }

        // Now check if tab pane wasn't scrolled too far
        --i;
        float xmax = d_tabButtonVector [i]->getXPosition ().d_offset +
            d_tabButtonVector [i]->getPixelSize ().d_width;
        float width = tabContentPane->getPixelSize ().d_width;

        // If right button margin exceeds right window margin,
        // or leftmost button is at offset 0, finish
        if ((xmax > (width - 0.5)) || (d_firstTabOffset == 0))
        {
            if (scrollLeftBtn)
                scrollLeftBtn->setVisible (d_firstTabOffset < 0);
            if (scrollRightBtn)
                scrollRightBtn->setVisible (xmax > width);
            break;
        }

        // Scroll the tab pane until the rightmost button
        // touches the right margin
        d_firstTabOffset += width - xmax;
        // If we scrolled it too far, set leftmost button offset to 0
        if (d_firstTabOffset > 0)
            d_firstTabOffset = 0;
    }
}
/*************************************************************************
Text changed on a content window
*************************************************************************/
bool TabControl::handleContentWindowTextChanged(const EventArgs& args)
{
    // update text
    const WindowEventArgs& wargs = static_cast<const WindowEventArgs&>(args);
    Window* tabButton = getTabButtonPane()->getChild(
        makeButtonName(wargs.window));
    tabButton->setText(wargs.window->getText());
    // sort out the layout
    performChildWindowLayout();
	requestRedraw();

	return true;
}
/*************************************************************************
Tab button clicked
*************************************************************************/
bool TabControl::handleTabButtonClicked(const EventArgs& args)
{
    const WindowEventArgs& wargs = static_cast<const WindowEventArgs&>(args);
    TabButton* tabButton = static_cast<TabButton*>(wargs.window);
    setSelectedTab(tabButton->getTargetWindow()->getName());

	return true;
}

/*************************************************************************
    Return a pointer to the tab button pane (Window)for
    this TabControl.
*************************************************************************/
Window* TabControl::getTabButtonPane() const
{
    return WindowManager::getSingleton().getWindow(getName() +
                                                   TabButtonPaneNameSuffix);
}

/*************************************************************************
    Return a pointer to the TabPane component widget for
    this TabControl.
*************************************************************************/
Window* TabControl::getTabPane() const
{
    return WindowManager::getSingleton().getWindow(getName() + ContentPaneNameSuffix);
}

int TabControl::writeChildWindowsXML(XMLSerializer& xml_stream) const
{
    int childOutputCount = Window::writeChildWindowsXML(xml_stream);

    // since TabControl content is actually added to the component tab
    // content pane window, this overridden function exists to dump those
    // out as if they were our own children.
    for (size_t i = 0; i < getTabCount(); ++i)
    {
        getTabContentsAtIndex(i)->writeXMLToStream(xml_stream);
        ++childOutputCount;
    }

    return childOutputCount;
}

/*************************************************************************
    create and return a pointer to a TabButton widget for use as a
    clickable tab header
*************************************************************************/
TabButton* TabControl::createTabButton(const String& name) const
{
    if (d_windowRenderer != 0)
    {
        TabControlWindowRenderer* wr = (TabControlWindowRenderer*)d_windowRenderer;
        return wr->createTabButton(name);
    }
    else
    {
        //return createTabButton_impl(name);
        throw InvalidRequestException("TabControl::createTabButton - This function must be implemented by the window renderer module");
    }
}

/*************************************************************************
	change the positioning of the tab pane.
*************************************************************************/
void TabControl::setTabPanePosition(TabPanePosition pos)
{
	d_tabPanePos = pos;
    performChildWindowLayout();
}

/*************************************************************************
    scroll the tab pane to left or right depending of the clicked button
*************************************************************************/
bool TabControl::handleScrollPane(const EventArgs& e)
{
    const WindowEventArgs& wargs = static_cast<const WindowEventArgs&>(e);

    size_t i;
    float delta = 0;
    // Find the leftmost visible button
    for (i = 0; i < d_tabButtonVector.size(); ++i)
    {
        if (d_tabButtonVector [i]->isVisible (true))
            break;
        delta = d_tabButtonVector [i]->getPixelSize ().d_width;
    }

    if (wargs.window->getName () == getName() + ButtonScrollLeftSuffix)
    {
        if (delta == 0.0f && i < d_tabButtonVector.size ())
            delta = d_tabButtonVector [i]->getPixelSize ().d_width;

        // scroll button pane to the right
        d_firstTabOffset += delta;
    }
    else if (i < d_tabButtonVector.size ())
        // scroll button pane to left
        d_firstTabOffset -= d_tabButtonVector [i]->getPixelSize ().d_width;

    performChildWindowLayout();
	return true;
}

bool TabControl::handleDraggedPane(const EventArgs& e)
{
    const MouseEventArgs& me = static_cast<const MouseEventArgs&>(e);

    if (me.button == MiddleButton)
    {
        // This is the middle-mouse-click event, remember initial drag position
        Window *but_pane = getTabButtonPane();
        d_btGrabPos = (me.position.d_x - but_pane->getPixelRect ().d_left) -
            d_firstTabOffset;
    }
    else if (me.button == NoButton)
    {
        // Regular mouse move event
        Window *but_pane = getTabButtonPane();
        float new_to = (me.position.d_x - but_pane->getPixelRect ().d_left) -
            d_btGrabPos;
        if ((new_to < d_firstTabOffset - 0.9) ||
            (new_to > d_firstTabOffset + 0.9))
        {
            d_firstTabOffset = new_to;
            performChildWindowLayout();
        }
    }

    return true;
}

bool TabControl::handleWheeledPane(const EventArgs& e)
{
    const MouseEventArgs& me = static_cast<const MouseEventArgs&>(e);

    Window *but_pane = getTabButtonPane();
    float delta = but_pane->getPixelRect ().getWidth () / 20;

    d_firstTabOffset -= me.wheelChange * delta;
    performChildWindowLayout();

    return true;
}

void TabControl::removeTab_impl(Window* window)
{
    // silently abort if window to be removed is 0.
    if (!window)
        return;

    // delete connection to event we subscribed earlier
    d_eventConnections.erase(window);
    // Was this selected?
    bool reselect = window->isVisible();
    // Tab buttons are the 2nd onward children
    getTabPane()->removeChildWindow(window);

    // remove button too
    removeButtonForTabContent(window);

    if (reselect && (getTabCount() > 0))
        // Select another tab
        setSelectedTab(getTabPane()->getChildAtIdx(0)->getName());

    performChildWindowLayout();

    requestRedraw();
}

} // End of  CEGUI namespace section
