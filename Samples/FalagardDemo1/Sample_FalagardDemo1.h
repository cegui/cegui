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

#include "SampleBase.h"
#include "CEGUI/CEGUI.h"


/*!
\brief
    Extremely simple example console class
*/
class DemoConsole
{
public:
    DemoConsole(CEGUI::Window* parent = 0);
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


class FalagardDemo1Sample : public Sample
{
public:
    FalagardDemo1Sample();
    virtual ~FalagardDemo1Sample() {}

    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);

    // method to perform any required cleanup operations.
    virtual void deinitialise();

    virtual CEGUI::Window* getGUIRoot();
    virtual const CEGUI::String& getUsedFilesString();

protected:
    bool handleRootKeyDown(const CEGUI::EventArgs& args);

    DemoConsole* d_console;

    CEGUI::Window* d_root;

    CEGUI::String d_usedFiles;
};


#endif  // end of guard _Sample_FalagardDemo1_h_
