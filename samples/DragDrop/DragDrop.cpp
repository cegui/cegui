/***********************************************************************
    created:    20/5/2008
    author:     Cegui Team
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "DragDrop.h"
#include "CEGUI/CEGUI.h"

//----------------------------------------------------------------------------//
DragDropSample::DragDropSample() :
    Sample(96)
{
    Sample::d_name = "DragDropSample";
    Sample::d_credits = "The CEGUI team";
    Sample::d_summary = "Drag and drop Sample";
    Sample::d_description = "Drag and drop Sample";
}

//----------------------------------------------------------------------------//
bool DragDropSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_guiContext = guiContext;
    d_usedFiles = CEGUI::String(__FILE__);

    // load windows look
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");

    // load font and setup default if not loaded via scheme
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();
    // Set default font for the gui context
    guiContext->setDefaultFont(defaultFont);

    // set up defaults
    guiContext->getCursor().setDefaultImage("WindowsLook/MouseArrow");

    // load the drive icons imageset
    ImageManager::getSingleton().loadImageset("DriveIcons.imageset");

    // load the initial layout
    guiContext->setRootWindow(
        WindowManager::getSingleton().loadLayoutFromFile("DragDropSample.layout"));

    // setup events
    subscribeEvents();

    // success!
    return true;
}

//----------------------------------------------------------------------------//
void DragDropSample::deinitialise()
{
    // nothing doing in here!
}

//----------------------------------------------------------------------------//
void DragDropSample::subscribeEvents()
{
    using namespace CEGUI;

    Window* root = d_guiContext->getRootWindow();

    /*
     * Subscribe handler to deal with user closing the frame window
     */
    try
    {
        Window* main_wnd = root->getChild("MainWindow");
        main_wnd->subscribeEvent(
            FrameWindow::EventCloseClicked,
            Event::Subscriber(&DragDropSample::handle_CloseButton, this));
    }
    // if something goes wrong, log the issue but do not bomb!
    catch (CEGUI::Exception&)
    {}

    /*
     * Subscribe the same handler to each of the twelve slots
     */
    String base_name = "MainWindow/Slot";

    for (int i = 1; i <= 12; ++i)
    {
        try
        {
            // get the window pointer for this slot
            Window* wnd =
                root->getChild(base_name + PropertyHelper<std::int32_t>::toString(i));

            // subscribe the handler.
            wnd->subscribeEvent(
                Window::EventDragDropItemDropped,
                Event::Subscriber(&DragDropSample::handle_ItemDropped, this));
        }
        // if something goes wrong, log the issue but do not bomb!
        catch (CEGUI::Exception&)
        {}
    }
}

//----------------------------------------------------------------------------//
bool DragDropSample::handle_ItemDropped(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // cast the args to the 'real' type so we can get access to extra fields
    const DragDropEventArgs& dd_args =
        static_cast<const DragDropEventArgs&>(args);

    if (!dd_args.window->getChildCount())
    {
        // add dragdrop item as child of target if target has no item already
        dd_args.window->addChild(dd_args.dragDropItem);
        // Now we must reset the item position from it's 'dropped' location,
        // since we're now a child of an entirely different window
        dd_args.dragDropItem->setPosition(
            UVector2(UDim(0.05f, 0),UDim(0.05f, 0)));
    }

    return true;
}

//----------------------------------------------------------------------------//
bool DragDropSample::handle_CloseButton(const CEGUI::EventArgs&)
{
    return true;
}