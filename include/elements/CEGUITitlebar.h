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
public:
	/*!
	\brief
		Return whether this title bar will respond to dragging.

	\return
		true if the title bar will respond to dragging, false if the title bar will not respond.
	*/
	bool	isDraggingEnabled(void) const;


	/*!
	\brief
		Set whether this title bar widget will respond to dragging.

	\param setting
		true if the title bar should respond to being dragged, false if it should not respond.

	\return
		Nothing.
	*/
	void	setDraggingEnabled(bool setting);


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
		New event handlers for title bar
	*************************************************************************/
	/*!
	\brief
		Event handler called when the 'draggable' state for the title bar is changed.
		
		Note that this is for 'internal' use at the moment and as such does not add or
		fire a public Event that can be subscribed to.
	*/
	virtual void	onDraggingModeChanged(WindowEventArgs& e) {}


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool	d_dragging;			//!< set to true when the window is being dragged.
	Point	d_dragPoint;		//!< Point at which we are being dragged.
	bool	d_dragEnabled;		//!< true when dragging for the widget is enabled.

	Rect	d_oldCursorArea;	//!< Used to backup cursor restraint area.
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUITitlebar_h_
