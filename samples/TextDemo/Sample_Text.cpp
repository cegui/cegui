/***********************************************************************
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
#include <cstdlib>

using namespace CEGUI;

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool TextDemo::initialise(CEGUI::GUIContext* guiContext)
{
    d_guiContext = guiContext;
    d_usedFiles = CEGUI::String(__FILE__);

    // we will make extensive use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // load an image to use as a background
    if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
        ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
    // set position and size
    background->setPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setSize(USize(cegui_reldim(1), cegui_reldim( 1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "SpaceBackgroundImage");
    // install this as the root GUI sheet
    d_guiContext->setRootWindow(background);

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
    // Word-wrap checkbox (we can't re-use a handler struct for the last argument!!)
    subscribeEvent("Root/TextDemo/StaticGroup/Wrap", ToggleButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/StaticGroup/HorzLeft", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/StaticGroup/HorzRight", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/StaticGroup/HorzCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/StaticGroup/VertTop", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/StaticGroup/VertBottom", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
    subscribeEvent("Root/TextDemo/StaticGroup/VertCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::formatChangedHandler, this));
}

void TextDemo::initSingleLineEdit()
{
    Window* root = d_guiContext->getRootWindow();
    // Only accepts digits for the age field
    if (root->isChild("Root/TextDemo/SingleLineGroup/editAge"))
    {
        static_cast<Editbox*>(root->getChild("Root/TextDemo/SingleLineGroup/editAge"))->setValidationString("[0-9]*");
    }
    // Set password restrictions
    if (root->isChild("Root/TextDemo/SingleLineGroup/editPasswd"))
    {
        Editbox* passwd = static_cast<Editbox*>(root->getChild("Root/TextDemo/SingleLineGroup/editPasswd"));
        passwd->setValidationString("[A-Za-z0-9]*");
        // Render masked
        passwd->setTextMasked(true);
    }
}

void TextDemo::initMultiLineEdit()
{
    // Scrollbar checkbox
    subscribeEvent("Root/TextDemo/MultiLineGroup/forceScroll", ToggleButton::EventSelectStateChanged, Event::Subscriber(&TextDemo::vertScrollChangedHandler, this));
}

void TextDemo::initRadio(const CEGUI::String& radio, int group, bool selected)
{
    Window* root = d_guiContext->getRootWindow();
    if (root->isChild(radio))
    {
        RadioButton* button = static_cast<RadioButton*>(root->getChild(radio));
        button->setGroupID(group);
        button->setSelected(selected);
    }
}

void TextDemo::subscribeEvent(const String& widget, const String& event, const Event::Subscriber& method)
{
    Window* root = d_guiContext->getRootWindow();
    if (root->isChild(widget))
    {
        Window* window = root->getChild(widget);
        window->subscribeEvent(event, method);
    }
}

bool TextDemo::isRadioSelected(const CEGUI::String& radio)
{
    Window* root = d_guiContext->getRootWindow();
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
    Window* root = d_guiContext->getRootWindow();
    // Check
    if (root->isChild(checkbox))
    {
        ToggleButton* button = static_cast<ToggleButton*>(root->getChild(checkbox));
        return button->isSelected();
    }
    return false;
}

bool TextDemo::formatChangedHandler(const CEGUI::EventArgs&)
{
    Window* root = d_guiContext->getRootWindow();

    if (root->isChild("Root/TextDemo/StaticGroup/StaticText"))
    {
        // and also the static text for which we will set the formatting options
        Window* st = root->getChild("Root/TextDemo/StaticGroup/StaticText");

        // handle vertical formatting settings
        if (isRadioSelected("Root/TextDemo/StaticGroup/VertTop"))
            st->setProperty("VertFormatting", "TopAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/VertBottom"))
            st->setProperty("VertFormatting", "BottomAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/VertCentered"))
            st->setProperty("VertFormatting", "CentreAligned");

        // handle horizontal formatting settings
        bool wrap = isCheckboxSelected("Root/TextDemo/StaticGroup/Wrap");

        if (isRadioSelected("Root/TextDemo/StaticGroup/HorzLeft"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapLeftAligned" : "LeftAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/HorzRight"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapRightAligned" : "RightAligned");
        else if (isRadioSelected("Root/TextDemo/StaticGroup/HorzCentered"))
            st->setProperty("HorzFormatting", wrap ? "WordWrapCentreAligned" : "CentreAligned");
    }

    // event was handled
    return true;
}

bool TextDemo::vertScrollChangedHandler(const CEGUI::EventArgs&)
{
    Window* root = d_guiContext->getRootWindow();

    if (root->isChild("Root/TextDemo/MultiLineGroup/editMulti"))
    {
        MultiLineEditbox* multiEdit = static_cast<MultiLineEditbox*>(root->getChild("Root/TextDemo/MultiLineGroup/editMulti"));
        // Use setter for a change
        multiEdit->setShowVertScrollbar(isCheckboxSelected("Root/TextDemo/MultiLineGroup/forceScroll"));
    }

    // event was handled
    return true;
}

bool TextDemo::quit(const CEGUI::EventArgs&)
{

    // event was handled
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void TextDemo::deinitialise()
{
    // nothing to do here!
}


/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static TextDemo sample;
    return sample;
}