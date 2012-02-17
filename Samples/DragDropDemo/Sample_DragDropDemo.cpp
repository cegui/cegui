/***********************************************************************
    filename:   Sample_DragDopDemo.cpp
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
#include "Sample_DragDropDemo.h"
#include "CEGUI/CEGUI.h"
#include "CEGuiBaseApplication.h"

//----------------------------------------------------------------------------//
int main(int /*argc*/, char* /*argv*/[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    DragDropDemo app;
    return app.run();
}

//----------------------------------------------------------------------------//
bool DragDropDemo::initialiseSample()
{
    using namespace CEGUI;

    // load windows look
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");

    // load font and setup default if not loaded via scheme
    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

    // set up defaults
    System::getSingleton().getDefaultGUIRoot().getMouseCursor().setDefaultImage("WindowsLook/MouseArrow");
    System::getSingleton().setDefaultFont("DejaVuSans-10");

    // load the drive icons imageset
    ImageManager::getSingleton().loadImageset("DriveIcons.imageset");

    // load the initial layout
    System::getSingleton().getDefaultGUIRoot().setRootWindow(
        WindowManager::getSingleton().loadLayoutFromFile("DragDropDemo.layout"));

    // setup events
    subscribeEvents();

    // success!
    return true;
}

//----------------------------------------------------------------------------//
void DragDropDemo::cleanupSample()
{
    // nothing doing in here!
}

//----------------------------------------------------------------------------//
void DragDropDemo::subscribeEvents()
{
    using namespace CEGUI;

    Window* root = System::getSingleton().getDefaultGUIRoot().getRootWindow();

    WindowManager& wmgr = WindowManager::getSingleton();

    /*
     * Subscribe handler to deal with user closing the frame window
     */
    CEGUI_TRY
    {
        Window* main_wnd = root->getChild("MainWindow");
        main_wnd->subscribeEvent(
            FrameWindow::EventCloseClicked,
            Event::Subscriber(&DragDropDemo::handle_CloseButton, this));
    }
    // if something goes wrong, log the issue but do not bomb!
    CEGUI_CATCH(CEGUI::Exception&)
    {}

    /*
     * Subscribe the same handler to each of the twelve slots
     */
    String base_name = "MainWindow/Slot";

    for (int i = 1; i <= 12; ++i)
    {
        CEGUI_TRY
        {
            // get the window pointer for this slot
            Window* wnd =
                root->getChild(base_name + PropertyHelper<int>::toString(i));

            // subscribe the handler.
            wnd->subscribeEvent(
                Window::EventDragDropItemDropped,
                Event::Subscriber(&DragDropDemo::handle_ItemDropped, this));
        }
        // if something goes wrong, log the issue but do not bomb!
        CEGUI_CATCH(CEGUI::Exception&)
        {}
    }
}

//----------------------------------------------------------------------------//
bool DragDropDemo::handle_ItemDropped(const CEGUI::EventArgs& args)
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
bool DragDropDemo::handle_CloseButton(const CEGUI::EventArgs&)
{
    d_sampleApp->setQuitting();
    return true;
}

//----------------------------------------------------------------------------//
