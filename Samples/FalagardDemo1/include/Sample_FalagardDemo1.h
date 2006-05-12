/***********************************************************************
    filename:   Sample_FalagardDemo1.h
    created:    19/6/2005
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
#ifndef _Sample_FalagardDemo1_h_
#define _Sample_FalagardDemo1_h_

#include "CEGuiSample.h"
#include "CEGUI.h"


/*!
\brief
    Extremely simple example console class
*/
class DemoConsole
{
public:
    DemoConsole(const CEGUI::String& id_name, CEGUI::Window* parent = 0);
    ~DemoConsole();

    void toggleVisibility();
    bool isVisible() const;

private:
    // these must match the IDs assigned in the layout
    static const unsigned int SubmitButtonID;
    static const unsigned int EntryBoxID;
    static const unsigned int HistoryID;

    bool handleSubmit(const CEGUI::EventArgs& args);
    bool handleKeyDown(const CEGUI::EventArgs& args);

    CEGUI::Window* d_root;
    int d_historyPos;
    std::vector<CEGUI::String> d_history;
};


class FalagardDemo1Sample : public CEGuiSample
{
public:
    // method to initialse the samples windows and events.
    bool initialiseSample();

    // method to perform any required cleanup operations.
    void cleanupSample(void);

protected:
    bool handleRootKeyDown(const CEGUI::EventArgs& args);

    DemoConsole* d_console;
};


#endif  // end of guard _Sample_FalagardDemo1_h_
