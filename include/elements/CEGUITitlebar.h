/************************************************************************
	filename: 	CEGUITitlebar.h
	created:	25/4/2004
	author:		Paul D Turner
	
	purpose:	Interface for a Titlebar Widget
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#ifndef _CEGUITitlebar_h_
#define _CEGUITitlebar_h_

#include "CEGUIWindow.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Class representing the title bar for Frame Windows.

*/
class CEGUIBASE_API Titlebar : public Window
{
protected:
	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Titlebar base class.
	*/
	Titlebar(const String& type, const String& name);


	/*!
	\brief
		Destructor for Titlebar base class.
	*/
	virtual ~Titlebar(void);


	/*************************************************************************
		Overridden event handler functions
	*************************************************************************/
	virtual void	onMouseMove(MouseEventArgs& e);
	virtual void	onMouseButtonDown(MouseEventArgs& e);
	virtual void	onMouseButtonUp(MouseEventArgs& e);
	virtual void	onMouseDoubleClicked(MouseEventArgs& e);
	virtual void	onCaptureLost(WindowEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool	d_dragging;			//!< set to true when the window is being dragged.
	Point	d_dragPoint;		//!< Point at which we are being dragged.

	Rect	d_oldCursorArea;	//!< Used to backup cursor restraint area.
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUITitlebar_h_
