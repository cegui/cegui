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
    SchemeManager::getSingleton().create("TaharezLook.scheme");
    System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
    // We need a font
    FontManager::getSingleton().create("DejaVuSans-10.font");
    // Font defaulting
    if(FontManager::getSingleton().isDefined("DejaVuSans-10"))
    {
		System::getSingleton().setDefaultFont("DejaVuSans-10");
    }

    // load an image to use as a background
    ImagesetManager::getSingleton().createFromImageFile("BackgroundImage", "GPN-2000-001437.tga");

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
    background->addChild (winMgr.loadWindowLayout ("TextDemo.layout"));

    // Init the seperate blocks which make up this sample
    initStaticText();
    initSingleLineEdit();
    initMultiLineEdit();

    // Quit button
    subscribeEvent("TextDemo/Quit", PushButton::EventClicked, Event::Subscriber(&TextDemo::quit, this));

    // Success (so far)
    return true;
}

void TextDemo::initStaticText()
{
    // Name, Group, Selected
    initRadio("TextDemo/HorzLeft", 0, true);
    initRadio("TextDemo/HorzRight", 0, false);
    initRadio("TextDemo/HorzCentered", 0, false);
    // New group!
    initRadio("TextDemo/VertTop", 1, true);
    initRadio("TextDemo/VertBottom", 1, false);
    initRadio("TextDemo/VertCentered", 1, false);
    //
    // Events
    //
    // Word-wrap checkbox (we can't re-use a handler struct for the last argument!!)
    subscribeEvent("TextDemo/Wrap", Checkbox::EventCheckStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("TextDemo/HorzLeft", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("TextDemo/HorzRight", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("TextDemo/HorzCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("TextDemo/VertTop", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("TextDemo/VertBottom", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("TextDemo/VertCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
}

void TextDemo::initSingleLineEdit()
{
    WindowManager& winMgr = WindowManager::getSingleton();
    // Only accepts digits for the age field
    if (winMgr.isWindowPresent("TextDemo/editAge"))
    {
        static_cast<Editbox*>(winMgr.getWindow("TextDemo/editAge"))->setValidationString("[0-9]*");
    }
    // Set password restrictions
    if (winMgr.isWindowPresent("TextDemo/editAge"))
    {
        Editbox* passwd = static_cast<Editbox*>(winMgr.getWindow("TextDemo/editPasswd"));
        passwd->setValidationString("[A-Za-z0-9]*");
        // Render masked
        passwd->setTextMasked(true);
    }
}

void TextDemo::initMultiLineEdit()
{
    // Scrollbar checkbox
    subscribeEvent("TextDemo/forceScroll", Checkbox::EventCheckStateChanged, Event::Subscriber(&TextDemo::vertScrollChangedHandler, this));
}

void TextDemo::initRadio(const CEGUI::String& radio, int group, bool selected)
{
    WindowManager& winMgr = WindowManager::getSingleton();
    if (winMgr.isWindowPresent(radio))
    {
        RadioButton* button = static_cast<RadioButton*>(winMgr.getWindow(radio));
        button->setGroupID(group);
        button->setSelected(selected);
    }
}

void TextDemo::subscribeEvent(const String& widget, const String& event, const Event::Subscriber& method)
{
    WindowManager& winMgr = WindowManager::getSingleton();
    if (winMgr.isWindowPresent(widget))
    {
        Window* window = winMgr.getWindow(widget);
        window->subscribeEvent(event, method);
    }
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

bool TextDemo::formatChangedHandler(const CEGUI::EventArgs&)
{
    // we will use the WindowManager to get access to the widgets
    WindowManager& winMgr = WindowManager::getSingleton();

    if (winMgr.isWindowPresent("TextDemo/StaticText"))
    {
        // and also the static text for which we will set the formatting options
        Window* st = winMgr.getWindow("TextDemo/StaticText");

        // handle vertical formatting settings
        if (isRadioSelected("TextDemo/VertTop"))
            st->setProperty("VertFormatting", "TopAligned");
        else if (isRadioSelected("TextDemo/VertBottom"))
            st->setProperty("VertFormatting", "BottomAligned");
        else if (isRadioSelected("TextDemo/VertCentered"))
            st->setProperty("VertFormatting", "VertCentred");

        // handle horizontal formatting settings
        bool wrap = isCheckboxSelected("TextDemo/Wrap");

        if (isRadioSelected("TextDemo/HorzLeft"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapLeftAligned" : "LeftAligned");
        else if (isRadioSelected("TextDemo/HorzRight"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapRightAligned" : "RightAligned");
        else if (isRadioSelected("TextDemo/HorzCentered"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapCentred" : "HorzCentred");
    }

    // event was handled
    return true;
}

bool TextDemo::vertScrollChangedHandler(const CEGUI::EventArgs&)
{
    WindowManager& winMgr = WindowManager::getSingleton();

    if (winMgr.isWindowPresent("TextDemo/editMulti"))
    {
        MultiLineEditbox* multiEdit = static_cast<MultiLineEditbox*>(winMgr.getWindow("TextDemo/editMulti"));
        // Use setter for a change
        multiEdit->setShowVertScrollbar(isCheckboxSelected("TextDemo/forceScroll"));
    }

    // event was handled
    return true;
}

bool TextDemo::quit(const CEGUI::EventArgs&)
{
    // signal quit
    d_sampleApp->setQuitting();

    // event was handled
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void TextDemo::cleanupSample()
{
    // nothing to do here!
}

// Main app
int main(int /*argc*/, char* /*argv*/[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    TextDemo app;
    return app.run();
}
