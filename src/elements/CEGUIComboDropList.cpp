/************************************************************************
	filename: 	CEGUIComboDropList.cpp
	created:	13/6/2004
	author:		Paul D Turner
	
	purpose:	Implements the Combobox Drop-List widget base class
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
#include "elements/CEGUIComboDropList.h"
#include "elements/CEGUIScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// Event names
const utf8	ComboDropList::ListSelectionAccepted[]		= "ListSelectionAccepted";


/*************************************************************************
	Constructor for ComboDropList base class
*************************************************************************/
ComboDropList::ComboDropList(const String& type, const String& name) :
	Listbox(type, name)
{
	addComboDropListEvents();
	hide();
}


/*************************************************************************
	Destructor for ComboDropList base class
*************************************************************************/
ComboDropList::~ComboDropList(void)
{
}


/*************************************************************************
	Initialise the Window based object ready for use.	
*************************************************************************/
void ComboDropList::initialise(void)
{
	Listbox::initialise();

	// set-up scroll bars so they return capture to us.
	d_vertScrollbar->setRestoreCapture(true);
	d_horzScrollbar->setRestoreCapture(true);
}


/*************************************************************************
	Add drop-list specific events
*************************************************************************/
void ComboDropList::addComboDropListEvents(void)
{
	addEvent(ListSelectionAccepted);
}


/*************************************************************************
	Handler for when list selection is confirmed.
*************************************************************************/
void ComboDropList::onListSelectionAccepted(WindowEventArgs& e)
{
	fireEvent(ListSelectionAccepted, e);
}


/*************************************************************************
	Handler for mouse movement events
*************************************************************************/
void ComboDropList::onMouseMove(MouseEventArgs& e)
{
	Listbox::onMouseMove(e);

	// remove current selection
	clearAllSelections_impl();

	// if mouse is within our area (but not our children)
	if (isHit(e.position) && (getChildAtPosition(e.position) == NULL))
	{
		//
		// Convert mouse position to absolute window pixels
		//
		Point localPos(screenToWindow(e.position));

		if (getMetricsMode() == Relative)
		{
			localPos = relativeToAbsolute(localPos);
		}

		// check for an item under the mouse
		ListboxItem* selItem = getItemAtPoint(localPos);

		// if an item is under mouse, select it
		if (selItem != NULL)
		{
			setItemSelectState(selItem, true);
		}

		e.handled = true;
	}

	e.handled = false;
}


/*************************************************************************
	Handler for mouse button pressed events
*************************************************************************/
void ComboDropList::onMouseButtonDown(MouseEventArgs& e)
{
	Listbox::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		if (!isHit(e.position))
		{
			releaseInput();
		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void ComboDropList::onMouseButtonUp(MouseEventArgs& e)
{
	Listbox::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		releaseInput();

		// if something was selected, confirm that selection.
		if (getSelectedCount() > 0)
		{
			WindowEventArgs args(this);
			onListSelectionAccepted(args);
		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for when input capture is lost
*************************************************************************/
void ComboDropList::onCaptureLost(WindowEventArgs& e)
{
	Listbox::onCaptureLost(e);
	hide();
	e.handled = true;
}


/*************************************************************************
	Handler for when window is activated
*************************************************************************/
void ComboDropList::onActivated(WindowEventArgs& e)
{
	Listbox::onActivated(e);
}

} // End of  CEGUI namespace section
