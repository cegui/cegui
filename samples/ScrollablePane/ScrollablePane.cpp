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

#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/widgets/ScrollablePane.h"
#include "CEGUI/widgets/ScrolledContainer.h"
#include "CEGUI/widgets/FrameWindow.h"

 //----------------------------------------------------------------------------//
ScrollablePaneSample::ScrollablePaneSample()
{
    Sample::d_name = "ScrollablePaneDemo";
    Sample::d_credits = "Tomas Lindquist Olsen, Vladimir 'Niello' Orlov";
    Sample::d_description =
        "The ScrollbarPane sample shows different scenarios of ScrollablePane usage. "
        "It uses the WindowsLook, which gives it a look similar to old Windows apps. "
        "The background consists of several ScrollablePanes to which windows "
        "can be added, using the menu bar. The items on each pane can be "
        "moved freely and the pane can be scrolled with the scrollbars.";
    Sample::d_summary =
        "Some scenarios of ScrollablePane usage.";
}

//----------------------------------------------------------------------------//
bool ScrollablePaneSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = String(__FILE__);
    d_guiContext = guiContext;
    d_inputAggregator = new SampleInputAggregator(guiContext);
    d_inputAggregator->initialise();

    // this sample will use WindowsLook
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");

    // load the default font
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    auto font = loadedFonts.empty() ? 0 : loadedFonts.front();
    guiContext->setDefaultFont(font);

    // set the cursor
    guiContext->getCursor().setDefaultImage("WindowsLook/MouseArrow");

    // set the default tooltip type
    guiContext->setDefaultTooltipType("WindowsLook/Tooltip");

    auto wm = WindowManager::getSingletonPtr();

    // create a root window
    auto root = wm->createWindow("DefaultWindow");
    root->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));

    guiContext->setRootWindow(root);

    // create a menubar.
    // this will fit in the top of the screen and have options for the Sample
    UDim bar_bottom(0, font->getLineSpacing(1.5f));

    Window* bar = wm->createWindow("WindowsLook/Menubar");
    bar->setArea(UDim(0,0),UDim(0,0),UDim(1,0),bar_bottom);
    root->addChild(bar);

    // fill out the menubar
    createMenu(bar);

    // Create the first, fixed-size scrollable pane. This scrollable pane will be
    // a kind of virtual desktop in the sense that it's bigger than the screen.

    auto panelFixed = wm->createWindow("WindowsLook/Static");
    panelFixed->setArea(URect(UDim(0,0),bar_bottom,UDim(0.75f,-1),UDim(0.5f,-1)));
    panelFixed->setProperty("BackgroundColours", "tl:FFBFBFBF tr:FFBFBFBF bl:FFBFBFBF br:FFBFBFBF");
    panelFixed->subscribeEvent(Window::EventSemanticEvent,
        Event::Subscriber(&ScrollablePaneSample::semanticEventHandler, this));
    root->addChild(panelFixed);

    d_pane = static_cast<ScrollablePane*>(wm->createWindow("WindowsLook/ScrollablePane"));
    d_pane->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    d_pane->setSwipeScrollingEnabled(true);
   
    // Disable autosizing. Windows will be sized relative to the content pane area.
    d_pane->setAdjustWidthToContent(false);
    d_pane->setAdjustHeightToContent(false);
    d_pane->setContentSize(USize(UDim(0,5000), UDim(0,5000)));
    d_pane->setHorizontalScrollPosition(0.015f);
    panelFixed->addChild(d_pane);

    // add a dialog to the first pane so we have something to drag around :)
    Window* dlg = wm->createWindow("WindowsLook/FrameWindow");
    dlg->setSize(USize(UDim(0.04f,0),UDim(0.02f,0)));
    dlg->setText("Drag me around");
    dlg->subscribeEvent(FrameWindow::EventCloseClicked, [dlg]()
    {
        WindowManager::getSingletonPtr()->destroyWindow(dlg);
    });
    d_pane->addChild(dlg);

    // Create next, auto-sized scrollable pane.

    auto panelAuto = wm->createWindow("WindowsLook/Static");
    panelAuto->setArea(URect(UDim(0,0),UDim(0.5f,0),UDim(0.75f,-1),UDim(1,0)));
    panelAuto->setProperty("BackgroundColours", "tl:FFBFBFBF tr:FFBFBFBF bl:FFBFBFBF br:FFBFBFBF");
    panelAuto->subscribeEvent(Window::EventSemanticEvent,
        Event::Subscriber(&ScrollablePaneSample::semanticEventHandler, this));
    root->addChild(panelAuto);

    auto pane = static_cast<ScrollablePane*>(wm->createWindow("WindowsLook/ScrollablePane"));
    pane->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    pane->setSwipeScrollingEnabled(true);

    // Enable autosizing. Windows will be sized relative to the viewable area.
    pane->setAdjustWidthToContent(true);
    pane->setAdjustHeightToContent(true);
    panelAuto->addChild(pane);

    // add a dialog to the first pane so we have something to drag around :)
    dlg = wm->createWindow("WindowsLook/FrameWindow");
    dlg->setSize(USize(UDim(0.2f,0),UDim(0.4f,0)));
    dlg->setText("Drag me around");
    dlg->subscribeEvent(FrameWindow::EventCloseClicked, [dlg]()
    {
        WindowManager::getSingletonPtr()->destroyWindow(dlg);
    });
    pane->addChild(dlg);

    // Create another scrollable pane. It will autosize vertically, but will have fixed
    // width. Width of the content area is set to 100% of the ScrollablePane viewport,
    // so horizontal scrollbar will never appear. ScrollablePane therefore becomes a
    // vertical-only scrolling container.

    auto panelVertical = wm->createWindow("WindowsLook/Static");
    panelVertical->setArea(URect(UDim(0.75,0),bar_bottom,UDim(1,0),UDim(1,0)));
    panelVertical->setProperty("BackgroundColours", "tl:FFBFBFBF tr:FFBFBFBF bl:FFBFBFBF br:FFBFBFBF");
    panelVertical->subscribeEvent(Window::EventSemanticEvent,
        Event::Subscriber(&ScrollablePaneSample::semanticEventHandler, this));
    root->addChild(panelVertical);

    pane = static_cast<ScrollablePane*>(wm->createWindow("WindowsLook/ScrollablePane"));
    pane->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    pane->setSwipeScrollingEnabled(true);

    // Enable height adjustment only. Since the height of the content area now depends
    // on areas of children, that areas' relative part will be evaluated against a
    // ScrollablePane viewport and not against the whole content area height.
    pane->setAdjustWidthToContent(false);
    pane->setAdjustHeightToContent(true);
    pane->setContentSize(USize(UDim(1,0), UDim(0,0)));
    panelVertical->addChild(pane);

    // Add some windows to the pane one below other to demonstrate relative positioning.
    // In a real scenario you probably should use a VerticalLayoutContainer instead.
    Window* verticalListParent = pane;

    constexpr bool useLayout = false;
    if (useLayout)
    {
        verticalListParent = wm->createWindow("VerticalLayoutContainer");
        verticalListParent->setCursorInputPropagationEnabled(true);
        pane->addChild(verticalListParent);
    }

    for (int i = 0; i < 5; ++i)
    {
        dlg = wm->createWindow("WindowsLook/FrameWindow");

        if (!useLayout)
            dlg->setPosition(UVector2(UDim(0, 0), UDim(0.25f * i, 0)));

        // Each window width is 100% of the content area, height is 25% of the viewport
        dlg->setSize(USize(UDim(1.0f, 0), UDim(0.25f, 0)));
        dlg->setText("Drag me around");
        dlg->setCursorInputPropagationEnabled(true); // FIXME: frame of the FrameWindow must inherit it!
        dlg->subscribeEvent(FrameWindow::EventCloseClicked, [dlg]()
        {
            WindowManager::getSingletonPtr()->destroyWindow(dlg);
        });
        verticalListParent->addChild(dlg);
    }

    return true;
}

//----------------------------------------------------------------------------//
void ScrollablePaneSample::createMenu(CEGUI::Window* bar)
{
    using namespace CEGUI;

    auto d_wm = WindowManager::getSingletonPtr();

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
    item->subscribeEvent("Clicked", Event::Subscriber(&ScrollablePaneSample::addNewChild, this));
    popup->addChild(item);
}

//----------------------------------------------------------------------------//
void ScrollablePaneSample::deinitialise()
{
    // everything we did is cleaned up by CEGUI
    delete d_inputAggregator;
}

//----------------------------------------------------------------------------//
bool ScrollablePaneSample::addNewChild(const CEGUI::EventArgs&)
{
    using namespace CEGUI;

    // Add a dialog to this pane so we have some more stuff to drag around :)
    Window* dlg = WindowManager::getSingletonPtr()->createWindow("WindowsLook/FrameWindow");
    dlg->setMinSize(USize(UDim(0,200),UDim(0,100)));
    dlg->setSize(USize(UDim(0,200),UDim(0,100)));
    dlg->setText("Drag me around too!");

    // Move the new dialog to the center of the viewable area
    const glm::vec2 offset = d_pane->getContentPane()->getPixelPosition();
    const Sizef viewportSize = d_pane->getViewableArea().getSize();
    const UVector2 pos(
        UDim(0, viewportSize.d_width / 2.f - offset.x - 100.f),
        UDim(0, viewportSize.d_height / 2.f - offset.y - 50.f));
    dlg->setPosition(pos);

    dlg->subscribeEvent(FrameWindow::EventCloseClicked, [dlg]()
    {
        WindowManager::getSingletonPtr()->destroyWindow(dlg);
    });

    d_pane->addChild(dlg);

    return true;
}

//----------------------------------------------------------------------------//
bool ScrollablePaneSample::semanticEventHandler(const CEGUI::EventArgs& e)
{
    const CEGUI::SemanticEventArgs& args = static_cast<const CEGUI::SemanticEventArgs&>(e);
    if (args.d_semanticValue == CEGUI::SemanticValue::SpawnNewDialog)
    {
        addNewChild(CEGUI::EventArgs());
        return true;
    }

    return false;
}
