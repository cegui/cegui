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
#include "SampleBase.h"
#include "CEGUI/CEGUI.h"

using namespace CEGUI;

#define SKIN "TaharezLook"
// for this to work you'll have to change the .layout files
//#define SKIN "WindowsLook"

static const char* PageText [] =
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

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public ListboxTextItem
{
public:
    MyListItem(const String& text) : ListboxTextItem(text)
    {
        setSelectionBrushImage(SKIN "/MultiListSelectionBrush");
    }
};

// Sample class
class TabControlDemo : public Sample
{
public:
    // method to initialse the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext)
    {
        d_guiContext = guiContext;
        d_usedFiles = CEGUI::String(__FILE__);

        // load font and setup default if not loaded via scheme
        Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
        // Set default font for the gui context
        guiContext->setDefaultFont(&defaultFont);

        // we will use of the WindowManager.
        WindowManager& winMgr = WindowManager::getSingleton();

        // load scheme and set up defaults
        SchemeManager::getSingleton().createFromFile(SKIN ".scheme");
        d_guiContext->getMouseCursor().setDefaultImage(SKIN "/MouseArrow");

        // load an image to use as a background
        if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
            ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

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
        background->addChild(winMgr.loadLayoutFromFile("TabControlDemo.layout"));

        TabControl* tc = static_cast<TabControl*>(background->getChild("Frame/TabControl"));

        // Add some pages to tab control
        tc->addTab(winMgr.loadLayoutFromFile("TabPage1.layout"));
        tc->addTab(winMgr.loadLayoutFromFile("TabPage2.layout"));

        WindowManager::getSingleton().DEBUG_dumpWindowNames("asd");

        static_cast<PushButton*>(
            background->getChild("Frame/TabControl/Page1/AddTab"))->subscribeEvent(
                PushButton::EventClicked,
                Event::Subscriber(&TabControlDemo::handleAddTab, this));

        // Click to visit this tab
        static_cast<PushButton*>(
            background->getChild("Frame/TabControl/Page1/Go"))->subscribeEvent(
                PushButton::EventClicked,
                Event::Subscriber(&TabControlDemo::handleGoto, this));

        // Click to make this tab button visible (when scrolling is required)
        static_cast<PushButton*>(
            background->getChild("Frame/TabControl/Page1/Show"))->subscribeEvent(
                PushButton::EventClicked,
                Event::Subscriber(&TabControlDemo::handleShow, this));

        static_cast<PushButton*>(
            background->getChild("Frame/TabControl/Page1/Del"))->subscribeEvent(
                PushButton::EventClicked,
                Event::Subscriber(&TabControlDemo::handleDel, this));

        RadioButton* rb = static_cast<RadioButton*>(
                              background->getChild("Frame/TabControl/Page1/TabPaneTop"));
        rb->setSelected(tc->getTabPanePosition() == TabControl::Top);
        rb->subscribeEvent(
            RadioButton::EventSelectStateChanged,
            Event::Subscriber(&TabControlDemo::handleTabPanePos, this));

        rb = static_cast<RadioButton*>(
                 background->getChild("Frame/TabControl/Page1/TabPaneBottom"));
        rb->setSelected(tc->getTabPanePosition() == TabControl::Bottom);
        rb->subscribeEvent(
            RadioButton::EventSelectStateChanged,
            Event::Subscriber(&TabControlDemo::handleTabPanePos, this));

        Scrollbar* sb = static_cast<Scrollbar*>(
                            background->getChild("Frame/TabControl/Page1/TabHeight"));
        sb->setScrollPosition(tc->getTabHeight().d_offset);
        sb->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&TabControlDemo::handleTabHeight, this));

        sb = static_cast<Scrollbar*>(
                 background->getChild("Frame/TabControl/Page1/TabPadding"));
        sb->setScrollPosition(tc->getTabTextPadding().d_offset);
        sb->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&TabControlDemo::handleTabPadding, this));

        refreshPageList();

        // From now on, we don't rely on the exceptions anymore, but perform nice (and recommended) checks
        // ourselves.

        return true;
    }

    // method to perform any required cleanup operations.
    void deinitialise()
    {
    }

    void refreshPageList()
    {
        Window* root = d_guiContext->getRootWindow();
        // Check if the windows exists
        Listbox* lbox = 0;
        TabControl* tc = 0;

        if (root->isChild("Frame/TabControl/Page1/PageList"))
        {
            lbox = static_cast<Listbox*>(root->getChild(
                                             "Frame/TabControl/Page1/PageList"));
        }

        if (root->isChild("Frame/TabControl"))
        {
            tc = static_cast<TabControl*>(root->getChild(
                                              "Frame/TabControl"));
        }

        if (lbox && tc)
        {
            lbox->resetList();

            for (size_t i = 0; i < tc->getTabCount(); i++)
            {
                lbox->addItem(new MyListItem(
                                  tc->getTabContentsAtIndex(i)->getName()));
            }
        }
    }

    bool handleTabPanePos(const EventArgs& e)
    {
        TabControl::TabPanePosition tpp;

        switch (static_cast<const WindowEventArgs&>(e).window->getID())
        {
        case 0:
            tpp = TabControl::Top;
            break;
        case 1:
            tpp = TabControl::Bottom;
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

    bool handleTabHeight(const EventArgs& e)
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

        // The return value mainly sais that we handled it, not if something failed.
        return true;
    }

    bool handleTabPadding(const EventArgs& e)
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

    bool handleAddTab(const EventArgs&)
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
                std::stringstream pgname;
                pgname << "Page" << num;

                if (root->isChild(String("Frame/TabControl/") + pgname.str().c_str()))
                    // Next
                    continue;

                Window* pg = 0;

                pg = WindowManager::getSingleton().loadLayoutFromFile("TabPage.layout");
                CEGUI_TRY
                {
                    pg = WindowManager::getSingleton().loadLayoutFromFile("TabPage.layout");
                    pg->setName(String(pgname.str().c_str()));
                }
                CEGUI_CATCH(CEGUI::Exception&)
                {
                    Logger::getSingleton().logEvent("Some error occured while adding a tabpage. Please see the logfile.");
                    break;
                }

                // This window has just been created while loading the layout
                if (pg->isChild("Text"))
                {
                    Window* txt = pg->getChild("Text");
                    txt->setText(PageText [num - 3]);

                    pg->setText(pgname.str().c_str());
                    tc->addTab(pg);

                    refreshPageList();
                    break;
                }
            }
        }

        return true;
    }

    bool handleGoto(const EventArgs&)
    {
        Window* root = d_guiContext->getRootWindow();
        // Check if the windows exists
        Listbox* lbox = 0;
        TabControl* tc = 0;

        if (root->isChild("Frame/TabControl/Page1/PageList"))
        {
            lbox = static_cast<Listbox*>(root->getChild(
                                             "Frame/TabControl/Page1/PageList"));
        }

        if (root->isChild("Frame/TabControl"))
        {
            tc = static_cast<TabControl*>(root->getChild(
                                              "Frame/TabControl"));
        }

        if (lbox && tc)
        {
            ListboxItem* lbi = lbox->getFirstSelectedItem();

            if (lbi)
            {
                tc->setSelectedTab(lbi->getText());
            }
        }

        return true;
    }

    bool handleShow(const EventArgs&)
    {
        Window* root = d_guiContext->getRootWindow();
        // Check if the windows exists
        Listbox* lbox = 0;
        TabControl* tc = 0;

        if (root->isChild("Frame/TabControl/Page1/PageList"))
        {
            lbox = static_cast<Listbox*>(root->getChild(
                                             "Frame/TabControl/Page1/PageList"));
        }

        if (root->isChild("Frame/TabControl"))
        {
            tc = static_cast<TabControl*>(root->getChild(
                                              "Frame/TabControl"));
        }

        if (lbox && tc)
        {
            ListboxItem* lbi = lbox->getFirstSelectedItem();

            if (lbi)
            {
                tc->makeTabVisible(lbi->getText());
            }
        }

        return true;
    }

    bool handleDel(const EventArgs&)
    {
        Window* root = d_guiContext->getRootWindow();
        // Check if the windows exists
        Listbox* lbox = 0;
        TabControl* tc = 0;

        if (root->isChild("Frame/TabControl/Page1/PageList"))
        {
            lbox = static_cast<Listbox*>(root->getChild(
                                             "Frame/TabControl/Page1/PageList"));
        }

        if (root->isChild("Frame/TabControl"))
        {
            tc = static_cast<TabControl*>(root->getChild(
                                              "Frame/TabControl"));
        }

        if (lbox && tc)
        {
            ListboxItem* lbi = lbox->getFirstSelectedItem();

            if (lbi)
            {
                Window* content = tc->getTabContents(lbi->getText());
                tc->removeTab(lbi->getText());
                // Remove the actual window from Cegui
                WindowManager::getSingleton().destroyWindow(content);

                refreshPageList();
            }
        }

        return true;
    }


    protected:
        CEGUI::GUIContext* d_guiContext;
};

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static TabControlDemo sample;
    return sample;
}
