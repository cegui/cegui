/***********************************************************************
    filename:   TabControlDemo.cpp
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
#include "CEGuiSample.h"
#include "CEGUI.h"
#include "CEGUIPropertyHelper.h"

using namespace CEGUI;

#define SKIN "TaharezLook"
// for this to work you'll have to change the .layout files
//#define SKIN "WindowsLook"

static char *PageText [] =
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
    MyListItem (const String& text) : ListboxTextItem(text)
    {
        setSelectionBrushImage(SKIN, "MultiListSelectionBrush");
    }
};

// Sample class
class TabControlDemo : public CEGuiSample
{
public:
    // method to initialse the samples windows and events.
    bool initialiseSample ()
	{
        // we will use of the WindowManager.
        WindowManager& winMgr = WindowManager::getSingleton ();

        // load scheme and set up defaults
        SchemeManager::getSingleton().loadScheme (SKIN ".scheme");
        System::getSingleton().setDefaultMouseCursor (SKIN, "MouseArrow");
        FontManager::getSingleton().createFont("DejaVuSans-10.font");

        // load an image to use as a background
        ImagesetManager::getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga");

        // here we will use a StaticImage as the root, then we can use it to place a background image
        Window* background = winMgr.createWindow (SKIN "/StaticImage");
        // set area rectangle
        background->setArea (URect (cegui_reldim (0), cegui_reldim (0),
                                          cegui_reldim (1), cegui_reldim (1)));
        // disable frame and standard background
        background->setProperty ("FrameEnabled", "false");
        background->setProperty ("BackgroundEnabled", "false");
        // set the background image
        background->setProperty ("Image", "set:BackgroundImage image:full_image");
        // install this as the root GUI sheet
        System::getSingleton ().setGUISheet (background);

        // set tooltip styles (by default there is none)
        System::getSingleton ().setDefaultTooltip (SKIN "/Tooltip");

        // load some demo windows and attach to the background 'root'
        background->addChildWindow (winMgr.loadWindowLayout ("TabControlDemo.layout", "TabControlDemo/"));

        TabControl *tc = static_cast<TabControl *>(winMgr.getWindow ("TabControlDemo/TabControl"));

        // Add some pages to tab control
        tc->addTab (winMgr.loadWindowLayout ("TabPage1.layout", "TabControlDemo/"));
        tc->addTab (winMgr.loadWindowLayout ("TabPage2.layout", "TabControlDemo/"));

        static_cast<PushButton *> (
            winMgr.getWindow("TabControlDemo/Page1/AddTab"))->subscribeEvent (
            PushButton::EventClicked,
            Event::Subscriber (&TabControlDemo::handleAddTab, this));

        static_cast<PushButton *> (
            winMgr.getWindow("TabControlDemo/Page1/Go"))->subscribeEvent (
            PushButton::EventClicked,
            Event::Subscriber (&TabControlDemo::handleGo, this));

        static_cast<PushButton *> (
            winMgr.getWindow("TabControlDemo/Page1/Show"))->subscribeEvent (
            PushButton::EventClicked,
            Event::Subscriber (&TabControlDemo::handleShow, this));

        static_cast<PushButton *> (
            winMgr.getWindow("TabControlDemo/Page1/Del"))->subscribeEvent (
            PushButton::EventClicked,
            Event::Subscriber (&TabControlDemo::handleDel, this));

        RadioButton *rb = static_cast<RadioButton *> (
            winMgr.getWindow("TabControlDemo/Page1/TabPaneTop"));
        rb->setSelected (tc->getTabPanePosition () == TabControl::Top);
        rb->subscribeEvent (
            RadioButton::EventSelectStateChanged,
            Event::Subscriber (&TabControlDemo::handleTabPanePos, this));

        rb = static_cast<RadioButton *> (
            winMgr.getWindow("TabControlDemo/Page1/TabPaneBottom"));
        rb->setSelected (tc->getTabPanePosition () == TabControl::Bottom);
        rb->subscribeEvent (
            RadioButton::EventSelectStateChanged,
            Event::Subscriber (&TabControlDemo::handleTabPanePos, this));

        Scrollbar *sb = static_cast<Scrollbar *> (
            winMgr.getWindow("TabControlDemo/Page1/TabHeight"));
        sb->setScrollPosition (tc->getTabHeight ().d_offset);
        sb->subscribeEvent (
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber (&TabControlDemo::handleTabHeight, this));

        sb = static_cast<Scrollbar *> (
            winMgr.getWindow("TabControlDemo/Page1/TabPadding"));
        sb->setScrollPosition (tc->getTabTextPadding ().d_offset);
        sb->subscribeEvent (
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber (&TabControlDemo::handleTabPadding, this));

        refreshPageList ();

        return true;
	}

    // method to perform any required cleanup operations.
    void cleanupSample ()
    {
        // me? cleanup? what?
    }

    void refreshPageList ()
    {
        WindowManager& winMgr = WindowManager::getSingleton ();
        Listbox *lbox = static_cast<Listbox *> (
            winMgr.getWindow("TabControlDemo/Page1/PageList"));
        TabControl *tc = static_cast<TabControl *>(
            winMgr.getWindow ("TabControlDemo/TabControl"));

        lbox->resetList ();
        for (size_t i = 0; i < tc->getTabCount (); i++)
            lbox->addItem (new MyListItem (
                tc->getTabContentsAtIndex (i)->getName ()));
    }

    bool handleTabPanePos (const EventArgs& e)
    {
        TabControl::TabPanePosition tpp;
        switch (static_cast<const WindowEventArgs&> (e).window->getID ())
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

        static_cast<TabControl *> (WindowManager::getSingleton ().getWindow (
            "TabControlDemo/TabControl"))->setTabPanePosition (tpp);

        return true;
    }

    bool handleTabHeight (const EventArgs& e)
    {
        Scrollbar *sb = static_cast<Scrollbar *> (
            static_cast<const WindowEventArgs&> (e).window);

        static_cast<TabControl *> (WindowManager::getSingleton ().getWindow (
            "TabControlDemo/TabControl"))->setTabHeight (
                UDim (0, sb->getScrollPosition ()));

        return true;
    }

    bool handleTabPadding (const EventArgs& e)
    {
        Scrollbar *sb = static_cast<Scrollbar *> (
            static_cast<const WindowEventArgs&> (e).window);

        static_cast<TabControl *> (WindowManager::getSingleton ().getWindow (
            "TabControlDemo/TabControl"))->setTabTextPadding (
                UDim (0, sb->getScrollPosition ()));

        return true;
    }

    bool handleAddTab (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();
        TabControl *tc = static_cast<TabControl *>(winMgr.getWindow ("TabControlDemo/TabControl"));

        for (int num = 3; num <= 16; num++)
        {
            std::stringstream prefix;
            prefix << "TabControlDemo/Page" << num;
            if (winMgr.isWindowPresent (prefix.str ()))
                continue;

            Window *pg = winMgr.loadWindowLayout ("TabPage.layout", prefix.str ());

            prefix << "Text";
            Window *txt = winMgr.getWindow (prefix.str ());

            txt->setText (PageText [num - 3]);

            std::stringstream pgname;
            pgname << "Page " << num;
            pg->setText (pgname.str ());
            tc->addTab (pg);

            refreshPageList ();
            break;
        }

        return true;
    }

    bool handleGo (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();
        Listbox *lbox = static_cast<Listbox *> (
            winMgr.getWindow("TabControlDemo/Page1/PageList"));

        ListboxItem *lbi = lbox->getFirstSelectedItem ();
        if (lbi)
        {
            TabControl *tc = static_cast<TabControl *>(winMgr.getWindow ("TabControlDemo/TabControl"));
            tc->setSelectedTab (lbi->getText ());
        }

        return true;
    }

    bool handleShow (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();
        Listbox *lbox = static_cast<Listbox *> (
            winMgr.getWindow("TabControlDemo/Page1/PageList"));

        ListboxItem *lbi = lbox->getFirstSelectedItem ();
        if (lbi)
        {
            TabControl *tc = static_cast<TabControl *>(winMgr.getWindow ("TabControlDemo/TabControl"));
            tc->makeTabVisible (lbi->getText ());
        }

        return true;
    }

    bool handleDel (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();
        Listbox *lbox = static_cast<Listbox *> (
            winMgr.getWindow("TabControlDemo/Page1/PageList"));

        ListboxItem *lbi = lbox->getFirstSelectedItem ();
        if (lbi)
        {
            TabControl *tc = static_cast<TabControl *>(winMgr.getWindow ("TabControlDemo/TabControl"));
            tc->removeTab (lbi->getText ());
            winMgr.destroyWindow (lbi->getText ());

            refreshPageList ();
        }

        return true;
    }
};

int main(int argc, char *argv[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    TabControlDemo app;
    return app.run ();
}
