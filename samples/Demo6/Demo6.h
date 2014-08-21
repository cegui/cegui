/***********************************************************************
    created:    20/8/2005
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _Sample_Demo6_h_
#define _Sample_Demo6_h_

#include "SampleBase.h"

#include "CEGUI/String.h"

namespace CEGUI
{
    class EventArgs;
}

// Sample class
class Demo6Sample : public Sample
{
public:
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);
    // method to perform any required cleanup operations.
    virtual void deinitialise();

protected:
    // helper methods
    void createDemoWindows(CEGUI::Window* root);
    void initDemoEventWiring(CEGUI::Window* root);

    // event handlers
    bool handleQuit(const CEGUI::EventArgs& e);
    bool handleAddColumn(const CEGUI::EventArgs& e);
    bool handleDeleteColumn(const CEGUI::EventArgs& e);
    bool handleAddRow(const CEGUI::EventArgs& e);
    bool handleDeleteRow(const CEGUI::EventArgs& e);
    bool handleSetItem(const CEGUI::EventArgs& e);
    bool handleSelectChanged(const CEGUI::EventArgs& e);
    bool handleSelectModeChanged(const CEGUI::EventArgs& e);
    bool handleContentsChanged(const CEGUI::EventArgs& e);
};

#endif  // end of guard _Sample_Demo6_h_
