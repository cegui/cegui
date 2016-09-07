/***********************************************************************
    created:    July 1 2012
    author:     Paul D Turner
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
#include "EditboxValidation.h"
#include "CEGUI/CEGUI.h"

//----------------------------------------------------------------------------//
bool EditboxValidation::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
    guiContext->getMouseCursor().setDefaultImage("AlfiskoSkin/MouseArrow");
    WindowManager& winMgr = WindowManager::getSingleton();

    Window* root = winMgr.createWindow("DefaultWindow");
    guiContext->setRootWindow(root);

    Window* wnd = root->createChild("AlfiskoSkin/FrameWindow");
    wnd->setPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f)));
    wnd->setSize(USize(cegui_reldim(0.5f), cegui_reldim( 0.5f)));
    wnd->setText("Editbox Validation Demo");

    Window* label = wnd->createChild("AlfiskoSkin/Label");
    label->setProperty("HorzFormatting", "WordWrapCentreAligned");
    label->setSize(USize(cegui_reldim(1.0f), cegui_reldim(0.2f)));
    label->setText("Enter 4 digits into the Editbox. A valid entry will be "
        "[colour='FF00FF00']green, [colour='FFFFFFFF']an invalid entry will be "
        "[colour='FFFF0000']red [colour='FFFFFFFF']and a partially valid entry "
        "will be [colour='FFFFBB11']orange");

    Editbox* eb = static_cast<Editbox*>(wnd->createChild("AlfiskoSkin/Editbox"));
    eb->setPosition(UVector2(cegui_reldim(0.1f), cegui_reldim(0.25f)));
    eb->setSize(USize(cegui_reldim(0.8f), cegui_reldim(0.15f)));
    eb->subscribeEvent(
        Editbox::EventTextValidityChanged,
        Event::Subscriber(&EditboxValidation::validationChangeHandler, this));

    eb->setValidationString("[0-9]{4}");
    eb->activate();

    return true;
}

//----------------------------------------------------------------------------//
bool EditboxValidation::validationChangeHandler(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;
    const RegexMatchStateEventArgs& ra(
        static_cast<const RegexMatchStateEventArgs&>(args));
    Editbox* eb = static_cast<Editbox*>(ra.window);

    switch(ra.matchState)
    {
    case RegexMatcher::MS_INVALID:
        eb->setProperty("NormalTextColour", "FFFF0000");
        break;

    case RegexMatcher::MS_PARTIAL:
        eb->setProperty("NormalTextColour", "FFFFBB11");
        break;

    case RegexMatcher::MS_VALID:
        eb->setProperty("NormalTextColour", "FF00FF00");
        break;
    }

    return true;
}

//----------------------------------------------------------------------------//
bool EditboxValidation::quitButtonHandler(const CEGUI::EventArgs&)
{
    return true;
}

//----------------------------------------------------------------------------//
void EditboxValidation::deinitialise()
{
    // nothing to do here!
}

//----------------------------------------------------------------------------//

/*************************************************************************
Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static EditboxValidation sample;
    return sample;
}
