/************************************************************************
    created:	 10/17/2004
    author:		 David Durant (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _Sample_TreeSample_h_
#define _Sample_TreeSample_h_

#include "Sample.h"
#include "CEGUI/CEGUI.h"


class TreeSample : public Sample
{
public:
    TreeSample();
    virtual ~TreeSample() {}

   // method to initialise the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext) override;

   // method to perform any required cleanup operations.
    void deinitialise() override;

protected:
   CEGUI::Window* d_rootWindow;
   static const unsigned int TreeID;
   static const unsigned int EditBoxID;

   bool handleEventSelectionChanged(const CEGUI::EventArgs& args);
   bool handleEventSubtreeExpanded(const CEGUI::EventArgs& args);
   bool handleEventSubtreeCollapsed(const CEGUI::EventArgs& args);
};


#endif  // end of guard _Sample_TreeSample_h_
