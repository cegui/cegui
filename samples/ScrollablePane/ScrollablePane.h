/***********************************************************************
created:    Aug 22 2014
author:     Luca Ebach <lucaebach@gmail.com> 
            (based on origingal code by Tomas Lindquist Olsen)
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _ScrollablePaneSample_h_
#define _ScrollablePaneSample_h_

#include "CEGUI/System.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/InputAggregator.h"
#include "CEGUI/InputEventReceiver.h"
#include "CEGUI/Font.h"
#include "CEGUI/Window.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/GUIContext.h"

#include "CEGUI/widgets/ScrollablePane.h"
#include "CEGUI/widgets/ScrolledContainer.h"

#include "Sample.h"


/*************************************************************************
Custom implementation of InputAggregator
*************************************************************************/
class SampleInputAggregator : public CEGUI::InputAggregator
{
public:
    SampleInputAggregator(CEGUI::InputEventReceiver* input_receiver) :
        CEGUI::InputAggregator(input_receiver)
    {
    }

    void initialise()
    {
        d_keyValuesMappings[static_cast<unsigned char>(CEGUI::Key::Scan::Space)] =
            CEGUI::SemanticValue::SpawnNewDialog;
    }
};

// ScrollablePane Sample sample class
class ScrollablePaneSample : public Sample
{
public:
    ScrollablePaneSample();
    virtual ~ScrollablePaneSample() {}

    // method to initialse the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext) override;
    // method to perform any required cleanup operations.
    void deinitialise() override;

private:
    // creates the menubar with content
    void createMenu(CEGUI::Window* bar);

    // quit menu item handler
    bool fileQuit(const CEGUI::EventArgs&) { return true; }

    // new dialog menu item handler
    bool SampleNewDialog(const CEGUI::EventArgs& e);
    bool semanticEventHandler(const CEGUI::EventArgs& e);

    // member data
    CEGUI::WindowManager* d_wm; // we will use the window manager alot
    CEGUI::System* d_system;    // the gui system
    CEGUI::Window* d_root;      // the gui sheet
    CEGUI::Font* d_font;        // the font we use
    CEGUI::ScrollablePane* d_pane; // the scrollable pane. center piece of the Sample

    CEGUI::GUIContext* d_guiContext;
};

#endif // _ScrollablePaneSample