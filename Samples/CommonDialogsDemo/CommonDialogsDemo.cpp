/***********************************************************************
    filename:   CommonDialogsDemo.cpp
    created:    Sun Oct 23 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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

//----------------------------------------------------------------------------//
class CommonDialogsDemo : public Sample
{
public:
    bool initialiseSample();
    void cleanupSample(void) {}
};

//----------------------------------------------------------------------------//
bool CommonDialogsDemo::initialiseSample()
{
    using namespace CEGUI;

    // load resources and set up system defaults
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    SchemeManager::getSingleton().createFromFile("VanillaCommonDialogs.scheme");
    System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

    // set up the root window / gui sheet
    WindowManager& winMgr = WindowManager::getSingleton();
    Window* root = winMgr.createWindow("DefaultWindow", "Root");
    System::getSingleton().getDefaultGUIContext().setRootWindow(root);

    // create container window for the demo
    FrameWindow* wnd = static_cast<FrameWindow*>(
        winMgr.createWindow("Vanilla/FrameWindow"));
    root->addChild(wnd);

    wnd->setPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f)));
    wnd->setSize(USize(cegui_reldim(0.5f), cegui_reldim( 0.5f)));
    wnd->setText("Common Dialogs Demo - Main Window");

    // Add a colour picker & label
    Window* colourPickerLabel = winMgr.createWindow("Vanilla/StaticText");
    wnd->addChild(colourPickerLabel);
    colourPickerLabel->setSize(USize(UDim(0, 110), UDim(0, 30)));
    colourPickerLabel->setProperty("FrameEnabled", "False");
    colourPickerLabel->setProperty("BackgroundEnabled", "False");
    colourPickerLabel->setText("Colour (click it!):");
    Window* colourPicker = winMgr.createWindow("Vanilla/ColourPicker");
    wnd->addChild(colourPicker);
    colourPicker->setPosition(UVector2(UDim(0,110), UDim(0, 0)));
    colourPicker->setSize(USize(UDim(0, 100), UDim(0, 30)));

    return true;
}

//----------------------------------------------------------------------------//

