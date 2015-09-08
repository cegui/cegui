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
#ifndef _Sample_Text_h_
#define _Sample_Text_h_

#include "SampleBase.h"

namespace CEGUI
{
    class SubscriberSlot;
}

// Sample class
class TextDemo : public Sample
{
public:
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);

    // method to perform any required cleanup operations.
    virtual void deinitialise();

    /*************************************************************************
        member functions used as event handlers
    *************************************************************************/
    bool handleQuit(const CEGUI::EventArgs& e);
private:
    /** Sub routines for each edit panel.*/
    void initStaticText();
    void initSingleLineEdit();
    void initMultiLineEdit();
    /** Sets group- and selected for given radio name.*/
    void initRadio(const CEGUI::String& radio, int group, bool selected);
    /** Generic subscribe helper.*/
    void subscribeEvent(const CEGUI::String& widgetName, const CEGUI::String& eventName, const CEGUI::SubscriberSlot& method);
    /** Returns whether the given radio is selected.*/
    bool isRadioSelected(const CEGUI::String& radio);
    /** Returns whether the given checkbox is selected.*/
    bool isCheckboxSelected(const CEGUI::String& checkbox);
    /** Called when user changes the formatting option of the static text.*/
    bool formatChangedHandler(const CEGUI::EventArgs& e);
    /** Called when user changes the scrollbar option of the multiline edit.*/
    bool vertScrollChangedHandler(const CEGUI::EventArgs& e);
    /** Called when the user clicks the Quit button.*/
    bool quit(const CEGUI::EventArgs& e);

    CEGUI::GUIContext* d_guiContext;
};

#endif  // end of guard _Sample_Demo4_h_
