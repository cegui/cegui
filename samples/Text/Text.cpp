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
#include "Text.h"
#include "CEGUI/CEGUI.h"
#include <stddef.h>

using namespace CEGUI;

TextSample::TextSample()
{
    Sample::d_name = "TextSample";
    Sample::d_credits = "CEGUI team";
    Sample::d_description =
        "The text sample contains text editing widgets and offers possibilities to "
        "change the text formatting or enter text to play around with the options";
    Sample::d_summary = "The text formatting and scrollbar visibility "
        "are adjusted using the properties of the windows";
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool TextSample::initialise(CEGUI::GUIContext* guiContext)
{
    d_guiContext = guiContext;
    d_usedFiles = CEGUI::String(__FILE__);

    // we will make extensive use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load font and setup default if not loaded via scheme
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();
    // Set default font for the gui context
    guiContext->setDefaultFont(defaultFont);

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    guiContext->setDefaultCursorImage("TaharezLook/MouseArrow");

    // load an image to use as a background
    if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
        ImageManager::getSingleton().addBitmapImageFromFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
    // set position and size
    background->setArea(UVector2(cegui_reldim(0), cegui_reldim(0)), USize(cegui_reldim(1), cegui_reldim(1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "SpaceBackgroundImage");
    // install this as the root GUI sheet
    d_guiContext->setRootWindow(background);

    // Load our layout as a basic
    background->addChild(winMgr.loadLayoutFromFile("TextSample.layout"));

    // Init the seperate blocks which make up this sample
    initStaticText();
    initSingleLineEdit();
    initMultiLineEdit();

    // Quit button
    subscribeEvent("Root/TextSample/Quit", PushButton::EventClicked, Event::Subscriber(&TextSample::quit, this));

    // Success (so far)
    return true;
}

void TextSample::initStaticText()
{
    // Name, Group, Selected
    initRadio("Root/TextSample/StaticGroup/HorzLeft", 0, true);
    initRadio("Root/TextSample/StaticGroup/HorzRight", 0, false);
    initRadio("Root/TextSample/StaticGroup/HorzCentered", 0, false);
    // New group!
    initRadio("Root/TextSample/StaticGroup/VertTop", 1, true);
    initRadio("Root/TextSample/StaticGroup/VertBottom", 1, false);
    initRadio("Root/TextSample/StaticGroup/VertCentered", 1, false);
    //
    // Events
    //
    // Word-wrap checkbox (we can't re-use a handler struct for the last argument!!)
    subscribeEvent("Root/TextSample/StaticGroup/Wrap", ToggleButton::EventSelectStateChanged, Event::Subscriber(&TextSample::formatChangedHandler, this));
    subscribeEvent("Root/TextSample/StaticGroup/HorzLeft", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextSample::formatChangedHandler, this));
    subscribeEvent("Root/TextSample/StaticGroup/HorzRight", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextSample::formatChangedHandler, this));
    subscribeEvent("Root/TextSample/StaticGroup/HorzCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextSample::formatChangedHandler, this));
    subscribeEvent("Root/TextSample/StaticGroup/VertTop", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextSample::formatChangedHandler, this));
    subscribeEvent("Root/TextSample/StaticGroup/VertBottom", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextSample::formatChangedHandler, this));
    subscribeEvent("Root/TextSample/StaticGroup/VertCentered", RadioButton::EventSelectStateChanged, Event::Subscriber(&TextSample::formatChangedHandler, this));
}

void TextSample::initSingleLineEdit()
{
    Window* root = d_guiContext->getRootWindow();

    // Only accepts digits for the age field
    if (Window* wnd = root->findChild("Root/TextSample/SingleLineGroup/editAge"))
        static_cast<Editbox*>(wnd)->setValidationString("[0-9]*");

    // Set password restrictions
    if (Window* wnd = root->findChild("Root/TextSample/SingleLineGroup/editPasswd"))
    {
        Editbox* passwd = static_cast<Editbox*>(wnd);
        passwd->setValidationString("[A-Za-z0-9]*");
        // Render masked
        passwd->setTextMaskingEnabled(true);
    }
}

void TextSample::initMultiLineEdit()
{
    // Scrollbar checkbox
    subscribeEvent("Root/TextSample/MultiLineGroup/forceScroll", ToggleButton::EventSelectStateChanged, Event::Subscriber(&TextSample::vertScrollChangedHandler, this));
}

void TextSample::initRadio(const CEGUI::String& radio, int group, bool selected)
{
    if (Window* wnd = d_guiContext->getRootWindow()->findChild(radio))
    {
        RadioButton* button = static_cast<RadioButton*>(wnd);
        button->setGroupID(group);
        button->setSelected(selected);
    }
}

void TextSample::subscribeEvent(const String& widget, const String& event, const Event::Subscriber& method)
{
    if (Window* window = d_guiContext->getRootWindow()->findChild(widget))
        window->subscribeEvent(event, method);
}

bool TextSample::isRadioSelected(const CEGUI::String& radio)
{
    if (Window* wnd = d_guiContext->getRootWindow()->findChild(radio))
        return static_cast<RadioButton*>(wnd)->isSelected();
    return false;
}

bool TextSample::isCheckboxSelected(const CEGUI::String& checkbox)
{
    if (Window* wnd = d_guiContext->getRootWindow()->findChild(checkbox))
        return static_cast<ToggleButton*>(wnd)->isSelected();
    return false;
}

bool TextSample::formatChangedHandler(const CEGUI::EventArgs&)
{
    if (Window* st = d_guiContext->getRootWindow()->findChild("Root/TextSample/StaticGroup/StaticText"))
    {
        // handle vertical formatting settings
        if (isRadioSelected("Root/TextSample/StaticGroup/VertTop"))
            st->setProperty("VertFormatting", "TopAligned");
        else if (isRadioSelected("Root/TextSample/StaticGroup/VertBottom"))
            st->setProperty("VertFormatting", "BottomAligned");
        else if (isRadioSelected("Root/TextSample/StaticGroup/VertCentered"))
            st->setProperty("VertFormatting", "CentreAligned");

        // handle horizontal formatting settings
        if (isRadioSelected("Root/TextSample/StaticGroup/HorzLeft"))
            st->setProperty("HorzFormatting", "LeftAligned");
        else if (isRadioSelected("Root/TextSample/StaticGroup/HorzRight"))
            st->setProperty("HorzFormatting", "RightAligned");
        else if (isRadioSelected("Root/TextSample/StaticGroup/HorzCentered"))
            st->setProperty("HorzFormatting", "CentreAligned");

        const bool wrap = isCheckboxSelected("Root/TextSample/StaticGroup/Wrap");
        st->setProperty("WordWrap", wrap ? "true" : "false");
    }

    // event was handled
    return true;
}

bool TextSample::vertScrollChangedHandler(const CEGUI::EventArgs&)
{
    if (Window* wnd = d_guiContext->getRootWindow()->findChild("Root/TextSample/MultiLineGroup/editMulti"))
        static_cast<MultiLineEditbox*>(wnd)->setShowVertScrollbar(isCheckboxSelected("Root/TextSample/MultiLineGroup/forceScroll"));

    // event was handled
    return true;
}

bool TextSample::quit(const CEGUI::EventArgs&)
{

    // event was handled
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void TextSample::deinitialise()
{
    // nothing to do here!
}
