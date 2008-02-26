/***********************************************************************
    filename:   Sample_Text.cpp
    modified:   26/2/2008
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
#include "Sample_Text.h"
#include "CEGUI.h"
#include "CEGuiBaseApplication.h"
#include <cstdlib>

using namespace CEGUI;

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool TextDemo::initialiseSample()
{
    // we will make extensive use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
    System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	if(!FontManager::getSingleton().isFontPresent("Commonwealth-10"))
		FontManager::getSingleton().createFont("Commonwealth-10.font");

    // load an image to use as a background
    ImagesetManager::getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
    // set position and size
    background->setPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setSize(UVector2(cegui_reldim(1), cegui_reldim( 1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "set:BackgroundImage image:full_image");
    // install this as the root GUI sheet
    System::getSingleton().setGUISheet(background);

    // Load our layout as a basic
    background->addChildWindow (winMgr.loadWindowLayout ("TextDemo.layout"));

    // Init the seperate blocks which make up this sample
    initStaticText();
/*
    //
    // widget initialisation
    //
    // quit button
    PushButton* btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "QuitButton"));
    sheet->addChildWindow(btn);
    btn->setText("Quit!");
    // Center
    btn->setPosition(UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
    btn->setHorizontalAlignment(HA_CENTRE);
    btn->setSize(UVector2(cegui_reldim(0.1f), cegui_reldim( 0.042f)));
    btn->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FormattedTextDemo::handleQuit, this));
    btn->setAlwaysOnTop(true);


    // word-wrap checkbox
    Checkbox* cb = static_cast<Checkbox*>(winMgr.createWindow("TaharezLook/Checkbox", "TextWindow/CB1"));
    textwnd->addChildWindow(cb);
    cb->setPosition(UVector2(cegui_reldim(0.65f), cegui_reldim( 0.13f)));
    cb->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.05f)));
    cb->setText("Word Wrap");
    // subscribe a handler to listen for when the check-box button select state changes
    cb->subscribeEvent(Checkbox::EventCheckStateChanged, &formatChangedHandler);

    // horizontal formatting radio group
    RadioButton* rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB1"));
    textwnd->addChildWindow(rb);
    rb->setPosition(UVector2(cegui_reldim(0.65f), cegui_reldim( 0.3f)));
    rb->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.05f)));
    rb->setGroupID(1);
    rb->setText("Left Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB2"));
    textwnd->addChildWindow(rb);
    rb->setPosition(UVector2(cegui_reldim(0.65f), cegui_reldim( 0.35f)));
    rb->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.05f)));
    rb->setGroupID(1);
    rb->setText("Right Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB3"));
    textwnd->addChildWindow(rb);
    rb->setPosition(UVector2(cegui_reldim(0.65f), cegui_reldim( 0.4f)));
    rb->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.05f)));
    rb->setGroupID(1);
    rb->setText("Centred");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    // vertical formatting radio group
    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB4"));
    textwnd->addChildWindow(rb);
    rb->setPosition(UVector2(cegui_reldim(0.65f), cegui_reldim( 0.6f)));
    rb->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.05f)));
    rb->setGroupID(2);
    rb->setText("Top Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB5"));
    textwnd->addChildWindow(rb);
    rb->setPosition(UVector2(cegui_reldim(0.65f), cegui_reldim( 0.65f)));
    rb->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.05f)));
    rb->setGroupID(2);
    rb->setText("Bottom Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB6"));
    textwnd->addChildWindow(rb);
    rb->setPosition(UVector2(cegui_reldim(0.65f), cegui_reldim( 0.7f)));
    rb->setSize(UVector2(cegui_reldim(0.35f), cegui_reldim( 0.05f)));
    rb->setGroupID(2);
    rb->setText("Centred");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    // Edit box for text entry
    Editbox* eb = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "TextWindow/Editbox1"));
    textwnd->addChildWindow(eb);
    eb->setPosition(UVector2(cegui_reldim(0.05f), cegui_reldim( 0.85f)));
    eb->setMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 0.04f)));
    eb->setSize(UVector2(cegui_reldim(0.90f), cegui_reldim( 0.08f)));
    // subscribe a handler to listen for when the text changes
    eb->subscribeEvent(Window::EventTextChanged, &textChangedHandler);

    //
    // Controls are set up.  Install initial settings
    //
    static_cast<Checkbox*>(winMgr.getWindow("TextWindow/CB1"))->setSelected(true);
    static_cast<RadioButton*>(winMgr.getWindow("TextWindow/RB1"))->setSelected(true);
    static_cast<RadioButton*>(winMgr.getWindow("TextWindow/RB4"))->setSelected(true);
    winMgr.getWindow("TextWindow/Editbox1")->setText("Come on then, edit me!");*/

    // success!
    return true;
}

void TextDemo::initStaticText()
{
    // Name, Group, Selected
    initRadio("Root/TextDemo/StaticGroup/HorzLeft", 0, true);
    initRadio("Root/TextDemo/StaticGroup/HorzRight", 0, false);
    initRadio("Root/TextDemo/StaticGroup/HorzCentered", 0, false);
    // New group!
    initRadio("Root/TextDemo/StaticGroup/VertTop", 1, true);
    initRadio("Root/TextDemo/StaticGroup/VertBottom", 1, false);
    initRadio("Root/TextDemo/StaticGroup/VertCentered", 1, false);
    //
    // Events
    //
    // Word-wrap checkbox
    Event::Subscriber handler(&TextDemo::formatChangedHandler, this);
    subscribeEvent("Root/TextDemo/StaticGroup/Wrap", Checkbox::EventCheckStateChanged, handler);
    subscribeEvent("Root/TextDemo/StaticGroup/HorzLeft", RadioButton::EventSelectStateChanged, handler);
    subscribeEvent("Root/TextDemo/StaticGroup/HorzRight", RadioButton::EventSelectStateChanged, handler);
    subscribeEvent("Root/TextDemo/StaticGroup/HorzCentered", RadioButton::EventSelectStateChanged, handler);
    subscribeEvent("Root/TextDemo/StaticGroup/VertTop", RadioButton::EventSelectStateChanged, handler);
    subscribeEvent("Root/TextDemo/StaticGroup/VertBottom", RadioButton::EventSelectStateChanged, handler);
    subscribeEvent("Root/TextDemo/StaticGroup/VertCentered", RadioButton::EventSelectStateChanged, handler);
}

void TextDemo::initRadio(const CEGUI::String& radio, int group, bool selected)
{
    WindowManager& winMgr = WindowManager::getSingleton();
    // Check
    if (winMgr.isWindowPresent(radio))
    {
        RadioButton* button = static_cast<RadioButton*>(winMgr.getWindow(radio));
        button->setGroupID(group);
        button->setSelected(selected);
    }
}

void TextDemo::subscribeEvent(const String& widget, const String& event, Event::Subscriber& method)
{
    WindowManager& winMgr = WindowManager::getSingleton();
    // Check
    if (winMgr.isWindowPresent(widget))
    {
        Window* window = winMgr.getWindow(widget);
        window->subscribeEvent(event, method);
    }
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void TextDemo::cleanupSample()
{
    // nothing to do here!
}

bool TextDemo::isRadioSelected(const CEGUI::String& radio)
{
    WindowManager& winMgr = WindowManager::getSingleton();    
    // Check
    if (winMgr.isWindowPresent(radio))
    {
        RadioButton* button = static_cast<RadioButton*>(winMgr.getWindow(radio));
        return button->isSelected();
    }
    return false;
}

bool TextDemo::isCheckboxSelected(const CEGUI::String& checkbox)
{
    WindowManager& winMgr = WindowManager::getSingleton();    
    // Check
    if (winMgr.isWindowPresent(checkbox))
    {
        Checkbox* button = static_cast<Checkbox*>(winMgr.getWindow(checkbox));
        return button->isSelected();
    }
    return false;
}


/*************************************************************************
    Free function to handle change of format options
*************************************************************************/
bool TextDemo::formatChangedHandler(const CEGUI::EventArgs& e)
{
    // we will use the WindowManager to get access to the widgets
    WindowManager& winMgr = WindowManager::getSingleton();

    if (winMgr.isWindowPresent("Root/TextDemo/StaticGroup/Text"))
    {
        // and also the static text for which we will set the formatting options
        Window* st = winMgr.getWindow("Root/TextDemo/StaticGroup/Text");

        // handle vertical formatting settings
        if (isRadioSelected("Root/TextDemo/StaticGroup/VertTop"))
            st->setProperty("VertFormatting", "TopAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/VertBottom"))
            st->setProperty("VertFormatting", "BottomAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/VertCentered"))
            st->setProperty("VertFormatting", "VertCentred");

        // handle horizontal formatting settings
        bool wrap = isCheckboxSelected("Root/TextDemo/StaticGroup/Wrap");

        if (isRadioSelected("Root/TextDemo/StaticGroup/HorzLeft"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapLeftAligned" : "LeftAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/HorzRight"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapRightAligned" : "RightAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/HorzCentered"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapCentred" : "HorzCentred");
    }

    // event was handled
    return true;
}

/*************************************************************************
    Free function handler called when editbox text changes
*************************************************************************/
/*bool textChangedHandler(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    //find the static box
    Window* st = WindowManager::getSingleton().getWindow("TextWindow/Static");

    // set text from the edit box...
    st->setText(static_cast<const WindowEventArgs&>(e).window->getText());

    return true;
}*/

/*************************************************************************
    Handler method that signals the application to quit
*************************************************************************/
bool TextDemo::handleQuit(const CEGUI::EventArgs& e)
{
    // signal quit
    d_sampleApp->setQuitting();

    // event was handled
    return true;
}

// Main app
int main(int argc, char *argv[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    TextDemo app;
    return app.run();
}
