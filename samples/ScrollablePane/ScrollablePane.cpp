/***********************************************************************
    created:    Aug 22 2014
    author:     Luca Ebach <lucaebach@gmail.com>
                (based on original code by Tomas Lindquist Olsen)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#include "ScrollablePane.h"

#include "CEGUI/System.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/InputAggregator.h"
#include "CEGUI/InputEventReceiver.h"
#include "CEGUI/Font.h"
#include "CEGUI/Window.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/GUIContext.h"

#include "CEGUI/widgets/ScrollablePane.h"
#include "CEGUI/widgets/ScrolledContainer.h"

#include "Sample.h"

ScrollablePaneSample::ScrollablePaneSample()
{
    Sample::d_name = "ScrollablePaneDemo";
    Sample::d_credits = "Tomas Lindquist Olsen";
    Sample::d_description =
        "The ScrollbarPane sample uses the WindowsLook, which gives it a look similar "
        "to old Windows applications. The background consists of a ScrollablePane to "
        "which windows can be added, using the menu bar. The items on the pane can be "
        "moved freely and the pane can be scrolled with the scrollbars.";
    Sample::d_summary =
        "The background window is of target type \"CEGUI/ScrollablePane\". "
        "The WindowsLook skin is used for all the windows.";
}

bool ScrollablePaneSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_guiContext = guiContext;
    d_usedFiles = CEGUI::String(__FILE__);

    d_inputAggregator = new SampleInputAggregator(d_guiContext);

    // this sample will use WindowsLook
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");

    // load the default font
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    d_font = loadedFonts.empty() ? 0 : loadedFonts.front();
    d_guiContext->setDefaultFont(d_font);

    // set the cursor
    d_system = System::getSingletonPtr();
    d_guiContext->getCursor().setDefaultImage("WindowsLook/MouseArrow");

    // set the default tooltip type
    d_guiContext->setDefaultTooltipType("WindowsLook/Tooltip");

    // We need the window manager to set up the test interface :)
    d_wm = WindowManager::getSingletonPtr();

    // create a root window
    // this will be a static, to give a nice app'ish background
    d_root = d_wm->createWindow("WindowsLook/Static");
    d_root->setProperty("FrameEnabled", "false");
    d_root->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    d_root->setProperty("BackgroundColours", "tl:FFBFBFBF tr:FFBFBFBF bl:FFBFBFBF br:FFBFBFBF");
    d_root->subscribeEvent(Window::EventSemanticEvent,
        Event::Subscriber(&ScrollablePaneSample::semanticEventHandler, this));

    d_guiContext->setRootWindow(d_root);

    // create a menubar.
    // this will fit in the top of the screen and have options for the Sample
    UDim bar_bottom(0,d_font->getLineSpacing(1.5f));

    Window* bar = d_wm->createWindow("WindowsLook/Menubar");
    bar->setArea(UDim(0,0),UDim(0,0),UDim(1,0),bar_bottom);
    d_root->addChild(bar);

    // fill out the menubar
    createMenu(bar);

    // create a scrollable pane for our Sample content
    d_pane = static_cast<ScrollablePane*>(d_wm->createWindow("WindowsLook/ScrollablePane"));
    d_pane->setArea(URect(UDim(0,0),bar_bottom,UDim(1,0),UDim(1,0)));
    // this scrollable pane will be a kind of virtual desktop in the sense that it's bigger than
    // the screen. 3000 x 3000 pixels
    d_pane->setContentPaneAutoSized(false);
    d_pane->setContentPaneArea(CEGUI::Rectf(0, 0, 5000, 5000));
    d_root->addChild(d_pane);

    // add a dialog to this pane so we have something to drag around :)
    Window* dlg = d_wm->createWindow("WindowsLook/FrameWindow");
    dlg->setMinSize(USize(UDim(0,250),UDim(0,100)));
    dlg->setSize(USize(UDim(0,250),UDim(0,100)));
    dlg->setText("Drag me around");
    d_pane->addChild(dlg);

    return true;
}

/*************************************************************************/
void ScrollablePaneSample::createMenu(CEGUI::Window* bar)
{
    using namespace CEGUI;

    // file menu item
    Window* file = d_wm->createWindow("WindowsLook/MenuItem");
    file->setText("File");
    bar->addChild(file);

    // file popup
    Window* popup = d_wm->createWindow("WindowsLook/PopupMenu");
    file->addChild(popup);

    // quit item in file menu
    Window* item = d_wm->createWindow("WindowsLook/MenuItem");
    item->setText("Quit");
    item->subscribeEvent("Clicked", Event::Subscriber(&ScrollablePaneSample::fileQuit, this));
    popup->addChild(item);

    // Sample menu item
    Window* Sample = d_wm->createWindow("WindowsLook/MenuItem");
    Sample->setText("Sample");
    bar->addChild(Sample);

    // Sample popup
    popup = d_wm->createWindow("WindowsLook/PopupMenu");
    Sample->addChild(popup);

    // Sample -> new window
    item = d_wm->createWindow("WindowsLook/MenuItem");
    item->setText("New dialog");
    item->setTooltipText("Hotkey: Space");
    item->subscribeEvent("Clicked", Event::Subscriber(&ScrollablePaneSample::SampleNewDialog, this));
    popup->addChild(item);
}

/************************************************************************/
void ScrollablePaneSample::deinitialise()
{
    // everything we did is cleaned up by CEGUI
    delete d_inputAggregator;
}

/************************************************************************/
bool ScrollablePaneSample::SampleNewDialog(const CEGUI::EventArgs&)
{
    using namespace CEGUI;

    // add a dialog to this pane so we have some more stuff to drag around :)
    Window* dlg = d_wm->createWindow("WindowsLook/FrameWindow");
    dlg->setMinSize(USize(UDim(0,200),UDim(0,100)));
    dlg->setSize(USize(UDim(0,200),UDim(0,100)));
    dlg->setText("Drag me around too!");

    // URGENT FIXME!
    //Vector2f center = CoordConverter::windowToScreen(*d_root, uni_center);
    //Vector2f target = CoordConverter::screenToWindow(*d_pane->getContentPane(), center);
    //dlg->setPosition(UVector2(UDim(0,target.d_x-100), UDim(0,target.d_y-50)));

    d_pane->addChild(dlg);

    return true;
}

bool ScrollablePaneSample::semanticEventHandler(const CEGUI::EventArgs& e)
{
    const CEGUI::SemanticEventArgs& args = static_cast<const CEGUI::SemanticEventArgs&>(e);
    if (args.d_semanticValue == CEGUI::SemanticValue::SpawnNewDialog)
    {
        // this handler does not use the event args at all so this is fine :)
        // though maybe a bit hack-ish...
        SampleNewDialog(e);
        return true;
    }

    return false;
}
