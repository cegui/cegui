/************************************************************************
    filename:   Sample_TreeDemo.h
    created:	 10/17/2004
    author:		 David Durant (based on code by Paul D Turner)
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
#ifndef _Sample_TreeDemo_h_
#define _Sample_TreeDemo_h_

#include "CEGuiSample.h"
#include "CEGUI.h"


class TreeDemoSample : public CEGuiSample
   {
public:
   // method to initialse the samples windows and events.
   bool initialiseSample();

   // method to perform any required cleanup operations.
   void cleanupSample(void);

protected:
   CEGUI::Window* TreeDemoWindow;
   static const unsigned int TreeID;
   static const unsigned int EditBoxID;

   bool handleRootKeyDown(const CEGUI::EventArgs& args);
   bool handleEventSelectionChanged(const CEGUI::EventArgs& args);
   bool handleEventBranchOpened(const CEGUI::EventArgs& args);
   bool handleEventBranchClosed(const CEGUI::EventArgs& args);
   };


#endif  // end of guard _Sample_TreeDemo_h_
