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
#include "CEGUI/CEGUI.h"
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
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    // We need a font
    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    // Font defaulting
    if(FontManager::getSingleton().isDefined("DejaVuSans-10"))
    {
		System::getSingleton().setDefaultFont("DejaVuSans-10");
    }

    // load an image to use as a background
    ImageManager::getSingleton().addFromImageFile("BackgroundImage", "GPN-2000-001437.png");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
    // set position and size
    background->setPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setSize(USize(cegui_reldim(1), cegui_reldim( 1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "BackgroundImage");
    // install this as the root GUI sheet
    System::getSingleton().getDefaultGUIContext().setRootWindow(background);

    // Load our layout as a basic
    background->addChild(winMgr.loadLayoutFromFile("TextDemo.layout"));

    // Init the seperate blocks which make up this sample
    initStaticText();
    initSingleLineEdit();
    initMultiLineEdit();

    // Quit button
    subscribeEvent("Root/TextDemo/Quit", PushButton::EventClicked, Event::Subscriber(&TextDemo::quit, this));

    // Success (so far)
    return true;
}

void TextDemo::initStaticText()
{
    // Name, Group, Selected
    initRadio("Root/TextDemo/HorzLeft", 0, true);
    initRadio("Root/TextDemo/HorzRight", 0, false);
    initRadio("Root/TextDemo/HorzCentered", 0, false);
    // New group!
    initRadio("Root/TextDemo/VertTop", 1, true);
    initRadio("Root/TextDemo/VertBottom", 1, false);
    initRadio("Root/TextDemo/VertCentered", 1, false);
    //
    // Events
    //
    // Word-wrap checkbox (we can't re-use a handler struct for the last argument!!)
    subscribeEvent("Root/TextDemo/Wrap", Checkbox::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/HorzLeft", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/HorzRight", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/HorzCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/VertTop", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/VertBottom", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/VertCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
}

void TextDemo::initSingleLineEdit()
{
    Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();
    // Only accepts digits for the age field
    if (root->isChild("Root/TextDemo/editAge"))
    {
        static_cast<Editbox*>(root->getChild("Root/TextDemo/editAge"))->setValidationString("[0-9]*");
    }
    // Set password restrictions
    if (root->isChild("Root/TextDemo/editPasswd"))
    {
        Editbox* passwd = static_cast<Editbox*>(root->getChild("Root/TextDemo/editPasswd"));
        passwd->setValidationString("[A-Za-z0-9]*");
        // Render masked
        passwd->setTextMasked(true);
    }
}

void TextDemo::initMultiLineEdit()
{
    // Scrollbar checkbox
    subscribeEvent("Root/TextDemo/forceScroll", Checkbox::EventSelectStateChanged, Event::Subscriber(&TextDemo::vertScrollChangedHandler, this));
}

void TextDemo::initRadio(const CEGUI::String& radio, int group, bool selected)
{
    Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();
    if (root->isChild(radio))
    {
        RadioButton* button = static_cast<RadioButton*>(root->getChild(radio));
        button->setGroupID(group);
        button->setSelected(selected);
    }
}

void TextDemo::subscribeEvent(const String& widget, const String& event, const Event::Subscriber& method)
{
    Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();
    if (root->isChild(widget))
    {
        Window* window = root->getChild(widget);
        window->subscribeEvent(event, method);
    }
}

bool TextDemo::isRadioSelected(const CEGUI::String& radio)
{
    Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();
    // Check
    if (root->isChild(radio))
    {
        RadioButton* button = static_cast<RadioButton*>(root->getChild(radio));
        return button->isSelected();
    }
    return false;
}

bool TextDemo::isCheckboxSelected(const CEGUI::String& checkbox)
{
    Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();
    // Check
    if (root->isChild(checkbox))
    {
        Checkbox* button = static_cast<Checkbox*>(root->getChild(checkbox));
        return button->isSelected();
    }
    return false;
}

bool TextDemo::formatChangedHandler(const CEGUI::EventArgs&)
{
    Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();

    if (root->isChild("Root/TextDemo/StaticText"))
    {
        // and also the static text for which we will set the formatting options
        Window* st = root->getChild("Root/TextDemo/StaticText");

        // handle vertical formatting settings
        if (isRadioSelected("Root/TextDemo/VertTop"))
            st->setProperty("VertFormatting", "TopAligned");
        else if (isRadioSelected("Root/TextDemo/VertBottom"))
            st->setProperty("VertFormatting", "BottomAligned");
        else if (isRadioSelected("Root/TextDemo/VertCentered"))
            st->setProperty("VertFormatting", "VertCentred");

        // handle horizontal formatting settings
        bool wrap = isCheckboxSelected("Root/TextDemo/Wrap");

        if (isRadioSelected("Root/TextDemo/HorzLeft"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapLeftAligned" : "LeftAligned");
        else if (isRadioSelected("Root/TextDemo/HorzRight"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapRightAligned" : "RightAligned");
        else if (isRadioSelected("Root/TextDemo/HorzCentered"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapCentred" : "HorzCentred");
    }

    // event was handled
    return true;
}

bool TextDemo::vertScrollChangedHandler(const CEGUI::EventArgs&)
{
    Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();

    if (root->isChild("Root/TextDemo/editMulti"))
    {
        MultiLineEditbox* multiEdit = static_cast<MultiLineEditbox*>(root->getChild("Root/TextDemo/editMulti"));
        // Use setter for a change
        multiEdit->setShowVertScrollbar(isCheckboxSelected("Root/TextDemo/forceScroll"));
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
