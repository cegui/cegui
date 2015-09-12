/***********************************************************************
    created:    11/8/2012
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
#include "HelloWorld.h"
#include "CEGUI/CEGUI.h"

#include <iostream>


/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool HelloWorldDemo::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

     d_usedFiles = CEGUI::String(__FILE__);

    // CEGUI relies on various systems being set-up, so this is what we do
    // here first.
    //
    // The first thing to do is load a CEGUI 'scheme' this is basically a file
    // that groups all the required resources and definitions for a particular
    // skin so they can be loaded / initialised easily
    //
    // So, we use the SchemeManager singleton to load in a scheme that loads the
    // imagery and registers widgets for the TaharezLook skin.  This scheme also
    // loads in a font that gets used as the system default.
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    // The next thing we do is to set a default mouse cursor image.  This is
    // not strictly essential, although it is nice to always have a visible
    // cursor if a window or widget does not explicitly set one of its own.
    //
    // The TaharezLook Imageset contains an Image named "MouseArrow" which is
    // the ideal thing to have as a defult mouse cursor image.
    guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // Now the system is initialised, we can actually create some UI elements, for
    // this first example, a full-screen 'root' window is set as the active GUI
    // sheet, and then a simple frame window will be created and attached to it.

    // All windows and widgets are created via the WindowManager singleton.
    WindowManager& winMgr = WindowManager::getSingleton();

    // Here we create a "DefaultWindow".  This is a native type, that is, it does
    // not have to be loaded via a scheme, it is always available.  One common use
    // for the DefaultWindow is as a generic container for other windows.  Its
    // size defaults to 1.0f x 1.0f using the Relative metrics mode, which means
    // when it is set as the root GUI sheet window, it will cover the entire display.
    // The DefaultWindow does not perform any rendering of its own, so is invisible.
    //
    // Create a DefaultWindow called 'Root'.
    d_root = (DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root");

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // Set the root window as root of our GUI Context
    guiContext->setRootWindow(d_root);

    // A FrameWindow is a window with a frame and a titlebar which may be moved around
    // and resized.
    //
    // Create a FrameWindow in the TaharezLook style, and name it 'Demo Window'
    FrameWindow* wnd = (FrameWindow*)winMgr.createWindow("TaharezLook/FrameWindow", "Demo Window");

    // Here we attach the newly created FrameWindow to the previously created
    // DefaultWindow which we will be using as the root of the displayed gui.
    d_root->addChild(wnd);

    // Windows are in Relative metrics mode by default.  This means that we can
    // specify sizes and positions without having to know the exact pixel size
    // of the elements in advance.  The relative metrics mode co-ordinates are
    // relative to the parent of the window where the co-ordinates are being set.
    // This means that if 0.5f is specified as the width for a window, that window
    // will be half as its parent window.
    //
    // Here we set the FrameWindow so that it is half the size of the display,
    // and centered within the display.
    wnd->setPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f)));
    wnd->setSize(USize(cegui_reldim(0.5f), cegui_reldim( 0.5f)));

    // now we set the maximum and minum sizes for the new window.  These are
    // specified using relative co-ordinates, but the important thing to note
    // is that these settings are aways relative to the display rather than the
    // parent window.
    //
    // here we set a maximum size for the FrameWindow which is equal to the size
    // of the display, and a minimum size of one tenth of the display.
    wnd->setMaxSize(USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    wnd->setMinSize(USize(cegui_reldim(0.1f), cegui_reldim( 0.1f)));

    // As a final step in the initialisation of our sample window, we set the window's
    // text to "Hello World!", so that this text will appear as the caption in the
    // FrameWindow's titlebar.
    wnd->setText("Hello World!");

    wnd->subscribeEvent(CEGUI::Window::EventMouseClick,  Event::Subscriber(&HelloWorldDemo::handleHelloWorldClicked, this));

    // return true so that the samples framework knows that initialisation was a
    // success, and that it should now run the sample.
    return true;
}


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void HelloWorldDemo::deinitialise()
{
}

bool HelloWorldDemo::handleHelloWorldClicked(const CEGUI::EventArgs&)
{
    std::cout << "Hello World!" << std::endl;

    return false;
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static HelloWorldDemo sample;
    return sample;
}
