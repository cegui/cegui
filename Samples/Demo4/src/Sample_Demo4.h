/************************************************************************
    filename:   Sample_Demo4.h
    created:    20/8/2005
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
#ifndef _Sample_Demo4_h_
#define _Sample_Demo4_h_

#include "CEGuiSample.h"
#include "CEGUI.h"

// Sample class
class Demo4Sample : public CEGuiSample
{
public:
    // method to initialse the samples windows and events.
    bool initialiseSample();

    // method to perform any required cleanup operations.
    void cleanupSample(void);

    /*************************************************************************
        member functions used as event handlers
    *************************************************************************/
    bool handleQuit(const CEGUI::EventArgs& e);
    bool horzMoveHandler(const CEGUI::EventArgs& e);
    bool vertMoveHandler(const CEGUI::EventArgs& e);
    bool vscrollHandler(const CEGUI::EventArgs& e);
};

// some free functions used as event handlers
bool sliderHandler(const CEGUI::EventArgs& e);
bool formatChangedHandler(const CEGUI::EventArgs& e);
bool textChangedHandler(const CEGUI::EventArgs& e);

#endif  // end of guard _Sample_Demo4_h_
