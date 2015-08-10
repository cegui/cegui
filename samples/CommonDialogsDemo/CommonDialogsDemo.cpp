/***********************************************************************
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

#include "CEGUI/CommonDialogs/ColourPicker/ColourPicker.h"

//----------------------------------------------------------------------------//
class CommonDialogsDemo : public Sample
{
public:
    virtual bool initialise(CEGUI::GUIContext* guiContext);
    void deinitialise(void) {}
};

//----------------------------------------------------------------------------//
bool CommonDialogsDemo::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    // initialise the common dialogs library; this is very important!
    initialiseCEGUICommonDialogs();

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // load resources and set up system defaults
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    SchemeManager::getSingleton().createFromFile("VanillaCommonDialogs.scheme");
    guiContext->getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

    // set up the root window / gui sheet
    WindowManager& winMgr = WindowManager::getSingleton();
    Window* root = winMgr.createWindow("DefaultWindow", "Root");
    guiContext->setRootWindow(root);

    // create container window for the demo
    FrameWindow* wnd = static_cast<FrameWindow*>(
        winMgr.createWindow("Vanilla/FrameWindow"));
    root->addChild(wnd);

    wnd->setAlwaysOnTop(true);

    wnd->setPosition(UVector2(cegui_reldim(0.05f), cegui_reldim( 0.25f)));
    wnd->setSize(USize(cegui_reldim(0.5f), cegui_reldim( 0.5f)));
    wnd->setText("Common Dialogs Demo - Main Window");
    wnd->setCloseButtonEnabled(false);

    // Add a colour picker & label
    Window* colourPickerLabel = winMgr.createWindow("Vanilla/Label");
    wnd->addChild(colourPickerLabel);
    colourPickerLabel->setSize(USize(UDim(1.0f, 0.0f), UDim(0.0f, 30.0f)));
    colourPickerLabel->setText("Open the colour picker by clicking on the respective box:");

    CEGUI::ColourPicker* colourPicker = static_cast<CEGUI::ColourPicker*>(winMgr.createWindow("Vanilla/ColourPicker"));
    wnd->addChild(colourPicker);
    colourPicker->setPosition(UVector2(UDim(0, 20), UDim(0, 40)));
    colourPicker->setSize(USize(UDim(0, 100), UDim(0, 30)));
    colourPicker->setColour(CEGUI::Colour(1.0f, 0.0f, 0.0f, 0.5f));

    colourPicker = static_cast<CEGUI::ColourPicker*>(winMgr.createWindow("Vanilla/ColourPicker"));
    wnd->addChild(colourPicker);
    colourPicker->setPosition(UVector2(UDim(0, 20), UDim(0, 80)));
    colourPicker->setSize(USize(UDim(0, 100), UDim(0, 30)));
    colourPicker->setColour(CEGUI::Colour(0.0f, 1.0f, 1.0f, 0.0f));

    colourPicker = static_cast<CEGUI::ColourPicker*>(winMgr.createWindow("Vanilla/ColourPicker"));
    wnd->addChild(colourPicker);
    colourPicker->setPosition(UVector2(UDim(0, 20), UDim(0, 120)));
    colourPicker->setSize(USize(UDim(0, 100), UDim(0, 30)));
    colourPicker->setColour(CEGUI::Colour(0.4f, 0.4f, 0.0f, 1.0f));

    colourPicker = static_cast<CEGUI::ColourPicker*>(winMgr.createWindow("Vanilla/ColourPicker"));
    wnd->addChild(colourPicker);
    colourPicker->setPosition(UVector2(UDim(0, 20), UDim(0, 160)));
    colourPicker->setSize(USize(UDim(0, 100), UDim(0, 30)));
    colourPicker->setColour(CEGUI::Colour(1.0f, 0.2f, 0.5f, 0.8f));

    return true;
}

//----------------------------------------------------------------------------//


/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static CommonDialogsDemo sample;
    return sample;
}
