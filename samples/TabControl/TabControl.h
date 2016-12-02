/***********************************************************************
created:    2014-08-22
author:     Luca Ebach <lucaebach@gmail.com>
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
#ifndef _TabControlSample_h_
#define _TabControlSample_h_

#include "Sample.h"

using namespace CEGUI;

#define SKIN "TaharezLook"
// for this to work you'll have to change the .layout files
//#define SKIN "WindowsLook"

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class TabControlListItem : public ListboxTextItem
{
public:
    TabControlListItem(const String& text) : ListboxTextItem(text)
    {
        setSelectionBrushImage(SKIN "/MultiListSelectionBrush");
    }
};

// Sample class
class TabControlSample : public Sample
{
public:
    TabControlSample();
    virtual ~TabControlSample() {}

    // method to initialse the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext) override;

    // method to perform any required cleanup operations.
    void deinitialise() override;

    void refreshPageList();
    bool handleTabPanePos(const EventArgs& e);
    bool handleTabHeight(const EventArgs& e);
    bool handleTabPadding(const EventArgs& e);
    bool handleAddTab(const EventArgs&);
    bool handleGoto(const EventArgs&);
    bool handleShow(const EventArgs&);
    bool handleDel(const EventArgs&);

protected:
    TabControl* getTabControl(Window* root);
    ListWidget* getPageListWidget(Window* root);

    CEGUI::GUIContext* d_guiContext;
};

#endif // _TabControlSample_h_
