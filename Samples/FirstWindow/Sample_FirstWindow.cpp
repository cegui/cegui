/***********************************************************************
    filename:   Sample_FirstWindow.cpp
    created:    10/3/2005
    author:     Paul D Turner
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
#include "Sample_FirstWindow.h"
#include "CEGUI.h"

int main(int argc, char *argv[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    FirstWindowSample app;
    return app.run();
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool FirstWindowSample::initialiseSample()
{
    using namespace CEGUI;

    // CEGUI relies on various systems being set-up, so this is what we do
    // here first.
    //
    // Note that is is possible, and even usual, for most of these steps to
    // be done automatically via a "scheme" definition, or even from the
    // cegui.conf configuration file, however for completeness, and as an
    // example, virtually everything is being done manually in this example
    // code.

    // Imagesets area a collection of named areas within a texture or image
    // file.  Each area becomes an Image, and has a unique name by which it
    // can be referenced.  This sample is using the TaharezLook widgets, and
    // these rely upon the TaharezLook Imageset; so we must load this in
    // before doing anything else.  Note that the Imageset would normally be
    // specified as part of a scheme file, although as this example is
    // demonstrating, it is not a requirement.
    //
    // Load TaharezLook imageset by making use of the ImagesetManager singleton.
    Imageset* taharezImages = ImagesetManager::getSingleton().createImageset("TaharezLook.imageset");

    // The next thing we do is to set a default mouse cursor image.  This is
    // not strictly essential, although it is nice to always have a visible
    // cursor if a window or widget does not explicitly set one of its own.
    //
    // The TaharezLook Imageset contains an Image named "MouseArrow" which is
    // the ideal thing to have as a defult mouse cursor image.
    System::getSingleton().setDefaultMouseCursor(&taharezImages->getImage("MouseArrow"));

    // Now we have the gui imagery side of thigs set up, we should load in a font.
    // You should always load in at least one font, this is to ensure that there
    // is a default available for any gui element which needs to draw text.
    // The first font you load is automatically set as the initial default font,
    // although you can change the default later on if so desired.  Again, it is
    // possible to list fonts to be automatically loaded as part of a scheme, so
    // this step may not usually be performed explicitly.
    //
    // Fonts are loaded via the FontManager singleton.
    FontManager::getSingleton().createFont("Commonwealth-10.font");

    // The widgets that we will be using for this sample are the TaharezLook widgets,
    // and to enable us to use this 'skin' we must load the xml specification - which
    // within cegui is known as a "looknfeel" file.
    //
    // We load the looknfeel via the WidgetLookManager singleton.
    WidgetLookManager::getSingleton().parseLookNFeelSpecification("TaharezLook.looknfeel");

    // The final step of basic initialisation that is usually peformed is
    // registering some widgets with the system via a scheme file.  The scheme
    // basically states the name of a dynamically loaded module that contains the
    // widget classes that we wish to use.  As stated previously, a scheme can actually
    // conatin much more information, though for the sake of this first example, we
    // load a scheme which only contains what is required to register some widgets.
    //
    // Use the SchemeManager singleton to load in a scheme that registers widgets
    // for TaharezLook.
    SchemeManager::getSingleton().loadScheme("TaharezLookWidgets.scheme");

    // Now the system is initialised, we can actually create some UI elements, for
    // this first example, a full-screen 'root' window is set as the active GUI
    // sheet, and then a simple frame window will be created and attached to it.

    // All windows and widgets are created via the WindowManager singleton.
    WindowManager& winMgr = WindowManager::getSingleton();

    // Here we create a "DeafultWindow".  This is a native type, that is, it does
    // not have to be loaded via a scheme, it is always available.  One common use
    // for the DefaultWindow is as a generic container for other windows.  Its
    // size defaults to 1.0f x 1.0f using the Relative metrics mode, which means
    // when it is set as the root GUI sheet window, it will cover the entire display.
    // The DefaultWindow does not perform any rendering of its own, so is invisible.
    //
    // Create a DefaultWindow called 'Root'.
    DefaultWindow* root = (DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root");

    // set the GUI root window (also known as the GUI "sheet"), so the gui we set up
    // will be visible.
    System::getSingleton().setGUISheet(root);

    // A FrameWindow is a window with a frame and a titlebar which may be moved around
    // and resized.
    //
    // Create a FrameWindow in the TaharezLook style, and name it 'Demo Window'
    FrameWindow* wnd = (FrameWindow*)winMgr.createWindow("TaharezLook/FrameWindow", "Demo Window");

    // Here we attach the newly created FrameWindow to the previously created
    // DefaultWindow which we will be using as the root of the displayed gui.
    root->addChildWindow(wnd);

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
    wnd->setSize(UVector2(cegui_reldim(0.5f), cegui_reldim( 0.5f)));

    // now we set the maximum and minum sizes for the new window.  These are
    // specified using relative co-ordinates, but the important thing to note
    // is that these settings are aways relative to the display rather than the
    // parent window.
    //
    // here we set a maximum size for the FrameWindow which is equal to the size
    // of the display, and a minimum size of one tenth of the display.
    wnd->setMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    wnd->setMinSize(UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));

    // As a final step in the initialisation of our sample window, we set the window's
    // text to "Hello World!", so that this text will appear as the caption in the
    // FrameWindow's titlebar.
    wnd->setText("Hello World!");

    // return true so that the samples framework knows that initialisation was a
    // success, and that it should now run the sample.
    return true;
}


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void FirstWindowSample::cleanupSample()
{
    // nothing to do here!
}
