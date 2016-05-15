/***********************************************************************
    created:    20/5/2008
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
#ifndef _DragDropSample_h_
#define _DragDropSample_h_

#include "Sample.h"

namespace CEGUI
{
    class GUIContext;
    class EventArgs;
}

// Sample class
class DragDropSample : public Sample
{
public:
    DragDropSample();

    virtual ~DragDropSample() {}

    // override member to initialise the sample.
    bool initialise(CEGUI::GUIContext* guiContext) override;
    // override member to perform cleanup.
    void deinitialise() override;

private:
    /// member to subscribe handler to each 'slot' in the layout.
    void subscribeEvents();
    /// member that handles what happens when an item is dropped in a slot.
    bool handle_ItemDropped(const CEGUI::EventArgs& args);
    /// member that handles the frame window close button and quits
    bool handle_CloseButton(const CEGUI::EventArgs& args);

    CEGUI::GUIContext* d_guiContext;
};

#endif  // end of guard _DragDropSample_h_

