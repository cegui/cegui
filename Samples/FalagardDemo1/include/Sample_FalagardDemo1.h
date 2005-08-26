/************************************************************************
    filename:   Sample_FalagardDemo1.h
    created:    19/6/2005
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
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
