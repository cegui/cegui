/***********************************************************************
    filename:   Sample_Demo7.h
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
#ifndef _Sample_Demo7_h_
#define _Sample_Demo7_h_

#include "CEGuiSample.h"
#include "CEGUI.h"

// Sample class
class Demo7Sample : public CEGuiSample
{
public:
    // method to initialse the samples windows and events.
    bool initialiseSample();

    // method to perform any required cleanup operations.
    void cleanupSample(void);

protected:
    // initialisation helpers
    void createListContent(void);
    void initDemoEventWiring(void);

    // methods that process GUI events
    bool handleQuit(const CEGUI::EventArgs& e);
    bool handleSlider(const CEGUI::EventArgs& e);
    bool handleRadio(const CEGUI::EventArgs& e);
    bool handleCheck(const CEGUI::EventArgs& e);
};


// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public CEGUI::ListboxTextItem
{
public:
    MyListItem(const CEGUI::String& text) : ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};


#endif  // end of guard _Sample_Demo7_h_
