/***********************************************************************
    created:    27/6/2006
    author:     Andrew Zabolotny
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
#include "Sample.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/SharedStringStream.h"

#include "TabControl.h"

using namespace CEGUI;

static const char* PageText[] =
{
    "This is page three",
    "And this is page four, it's not too different from page three, isn't it?",
    "Yep, you guessed, this is page five",
    "And this is page six",
    "Seven",
    "Eight",
    "Nine. Quite boring, isn't it?",
    "Ten",
    "Eleven",
    "Twelve",
    "Thirteen",
    "Fourteen",
    "Fifteen",
    "And, finally, sixteen. Congrats, you found the last page!",
};

TabControlSample::TabControlSample()
{
    Sample::d_name = "TabControlSample";
    Sample::d_credits = "Andrew Zabolotny";
    Sample::d_description =
        "The TabControl sample contains a window that has multiple pages, "
        "which can be switching by clicking the tabs. Tabs can be adjusted "
        "and created with the widgets on the 1st page.";
    Sample::d_summary =
        "The tab window is of type \"TaharezLook/TabControl\". "
        "For the seperate pages CEGUI layouts are loaded and attached.";
}

bool TabControlSample::initialise(CEGUI::GUIContext* guiContext)
{
    d_guiContext = guiContext;
    d_usedFiles = CEGUI::String(__FILE__);

    // load font and setup default if not loaded via scheme
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();
    // Set default font for the gui context
    guiContext->setDefaultFont(defaultFont);

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile(SKIN ".scheme");
    d_guiContext->getCursor().setDefaultImage(SKIN "/MouseArrow");

    // load an image to use as a background
    if (!ImageManager::getSingleton().isDefined("SpaceBackgroundImage"))
        ImageManager::getSingleton().addBitmapImageFromFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow(SKIN "/StaticImage");
    // set area rectangle
    background->setArea(URect(cegui_reldim(0), cegui_reldim(0),
        cegui_reldim(1), cegui_reldim(1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "SpaceBackgroundImage");
    // install this as the root GUI sheet
    d_guiContext->setRootWindow(background);

    // set tooltip styles (by default there is none)
    d_guiContext->setDefaultTooltipType(SKIN "/Tooltip");

    // load some demo windows and attach to the background 'root'
    background->addChild(winMgr.loadLayoutFromFile("TabControlSample.layout"));

    TabControl* tc = static_cast<TabControl*>(background->getChild("Frame/TabControl"));

    // Add some pages to tab control
    tc->addTab(winMgr.loadLayoutFromFile("TabPage1.layout"));
    tc->addTab(winMgr.loadLayoutFromFile("TabPage2.layout"));

    WindowManager::getSingleton().DEBUG_dumpWindowNames("asd");

    static_cast<PushButton*>(
        background->getChild("Frame/TabControl/Page1/AddTab"))->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&TabControlSample::handleAddTab, this));

    // Click to visit this tab
    static_cast<PushButton*>(
        background->getChild("Frame/TabControl/Page1/Go"))->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&TabControlSample::handleGoto, this));

    // Click to make this tab button visible (when scrolling is required)
    static_cast<PushButton*>(
        background->getChild("Frame/TabControl/Page1/Show"))->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&TabControlSample::handleShow, this));

    static_cast<PushButton*>(
        background->getChild("Frame/TabControl/Page1/Del"))->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&TabControlSample::handleDel, this));

    RadioButton* rb = static_cast<RadioButton*>(
        background->getChild("Frame/TabControl/Page1/TabPaneTop"));
    rb->setSelected(tc->getTabPanePosition() == TabControl::TabPanePosition::Top);
    rb->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&TabControlSample::handleTabPanePos, this));

    rb = static_cast<RadioButton*>(
        background->getChild("Frame/TabControl/Page1/TabPaneBottom"));
    rb->setSelected(tc->getTabPanePosition() == TabControl::TabPanePosition::Bottom);
    rb->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&TabControlSample::handleTabPanePos, this));

    Scrollbar* sb = static_cast<Scrollbar*>(
        background->getChild("Frame/TabControl/Page1/TabHeight"));
    sb->setScrollPosition(tc->getTabHeight().d_offset);
    sb->subscribeEvent(
        Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&TabControlSample::handleTabHeight, this));

    sb = static_cast<Scrollbar*>(
        background->getChild("Frame/TabControl/Page1/TabPadding"));
    sb->setScrollPosition(tc->getTabTextPadding().d_offset);
    sb->subscribeEvent(
        Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&TabControlSample::handleTabPadding, this));

    refreshPageList();

    // From now on, we don't rely on the exceptions anymore, but perform nice (and recommended) checks
    // ourselves.

    return true;
}

void TabControlSample::deinitialise()
{
}

void TabControlSample::refreshPageList()
{
    Window* root = d_guiContext->getRootWindow();
    // Check if the windows exists
    ListWidget* list_widget = getPageListWidget(root);
    TabControl* tc = getTabControl(root);

    if (list_widget && tc)
    {
        list_widget->clearList();

        for (size_t i = 0; i < tc->getTabCount(); i++)
        {
            list_widget->addItem(new StandardItem(
                tc->getTabContentsAtIndex(i)->getName()));
        }
    }
}

bool TabControlSample::handleTabPanePos(const EventArgs& e)
{
    TabControl::TabPanePosition tpp;

    switch (static_cast<const WindowEventArgs&>(e).window->getID())
    {
    case 0:
        tpp = TabControl::TabPanePosition::Top;
        break;
    case 1:
        tpp = TabControl::TabPanePosition::Bottom;
        break;
    default:
        return false;
    }

    // Check if the window exists
    Window* root = d_guiContext->getRootWindow();

    if (root->isChild("Frame/TabControl"))
    {
        static_cast<TabControl*>(root->getChild(
            "Frame/TabControl"))->setTabPanePosition(tpp);
    }

    return true;
}

bool TabControlSample::handleTabHeight(const EventArgs& e)
{
    Scrollbar* sb = static_cast<Scrollbar*>(
        static_cast<const WindowEventArgs&>(e).window);

    // Check if the window exists
    Window* root = d_guiContext->getRootWindow();

    if (root->isChild("Frame/TabControl"))
    {
        static_cast<TabControl*>(root->getChild(
            "Frame/TabControl"))->setTabHeight(
            UDim(0, sb->getScrollPosition()));
    }

    // The return value mainly says that we handled it, not if something failed.
    return true;
}

bool TabControlSample::handleTabPadding(const EventArgs& e)
{
    Scrollbar* sb = static_cast<Scrollbar*>(
        static_cast<const WindowEventArgs&>(e).window);

    // Check if the window exists
    Window* root = d_guiContext->getRootWindow();

    if (root->isChild("Frame/TabControl"))
    {
        static_cast<TabControl*>(root->getChild(
            "Frame/TabControl"))->setTabTextPadding(
            UDim(0, sb->getScrollPosition()));
    }

    return true;
}

bool TabControlSample::handleAddTab(const EventArgs&)
{
    Window* root = d_guiContext->getRootWindow();

    // Check if the window exists
    if (root->isChild("Frame/TabControl"))
    {
        TabControl* tc = static_cast<TabControl*>(root->getChild(
            "Frame/TabControl"));

        // Add some tab buttons once
        for (int num = 3; num <= 16; num++)
        {
            std::stringstream pgNameStream;
            pgNameStream << "Page" << num;

            if (root->isChild(String("Frame/TabControl/") + pgNameStream.str()))
                // Next
                continue;

            Window* pg = WindowManager::getSingleton().loadLayoutFromFile("TabPage.layout");
            pg->setName(String(pgNameStream.str()));

            // This window has just been created while loading the layout
            if (pg->isChild("Text"))
            {
                Window* txt = pg->getChild("Text");
                txt->setText(PageText[num - 3]);

                pg->setText(pgNameStream.str());
                tc->addTab(pg);

                refreshPageList();
                break;
            }
        }
    }

    return true;
}

bool TabControlSample::handleGoto(const EventArgs&)
{
    Window* root = d_guiContext->getRootWindow();
    // Check if the windows exist
    ListWidget* list_widget = getPageListWidget(root);
    TabControl* tc = getTabControl(root);

    if (list_widget && tc)
    {
        StandardItem* item = list_widget->getFirstSelectedItem();

        if (item)
        {
            tc->setSelectedTab(item->getText());
        }
    }

    return true;
}

bool TabControlSample::handleShow(const EventArgs&)
{
    Window* root = d_guiContext->getRootWindow();
    // Check if the windows exist
    ListWidget* list_widget = getPageListWidget(root);
    TabControl* tc = getTabControl(root);

    if (list_widget && tc)
    {
        StandardItem* item = list_widget->getFirstSelectedItem();

        if (item)
        {
            tc->makeTabVisible(item->getText());
        }
    }

    return true;
}

bool TabControlSample::handleDel(const EventArgs&)
{
    Window* root = d_guiContext->getRootWindow();
    // Check if the windows exist
    ListWidget* list_widget = getPageListWidget(root);
    TabControl* tc = getTabControl(root);

    if (list_widget && tc)
    {
        StandardItem* item = list_widget->getFirstSelectedItem();

        if (item)
        {
            Window* content = tc->getTabContents(item->getText());
            tc->removeTab(item->getText());
            // Remove the actual window from CEGUI
            WindowManager::getSingleton().destroyWindow(content);

            refreshPageList();
        }
    }

    return true;
}

TabControl* TabControlSample::getTabControl(Window* root)
{
    String control_id("Frame/TabControl");
    if (root->isChild(control_id))
    {
        return static_cast<TabControl*>(root->getChild(control_id));
    }
    return nullptr;
}

ListWidget* TabControlSample::getPageListWidget(Window* root)
{
    String page_list_id("Frame/TabControl/Page1/PageList");
    if (root->isChild(page_list_id))
    {
        return static_cast<ListWidget*>(root->getChild(page_list_id));
    }
    return nullptr;
}
