/***********************************************************************
	filename: 	CEGUIComboDropList.cpp
	created:	13/6/2004
	author:		Paul D Turner
	
	purpose:	Implements the Combobox Drop-List widget base class
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
#include "elements/CEGUIComboDropList.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUICoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String ComboDropList::EventNamespace("ComboDropList");
const String ComboDropList::WidgetTypeName("CEGUI/ComboDropList");


/*************************************************************************
	Constants
*************************************************************************/
// Event names
const String ComboDropList::EventListSelectionAccepted( "ListSelectionAccepted" );


/*************************************************************************
	Constructor for ComboDropList base class
*************************************************************************/
ComboDropList::ComboDropList(const String& type, const String& name) :
	Listbox(type, name)
{
	d_autoArm = false;
	d_armed = false;

	hide();

    // pass captured inputs to children to enable scrollbars
    setDistributesCapturedInputs(true);
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
void ComboDropList::initialiseComponents(void)
{
	Listbox::initialiseComponents();

	// set-up scroll bars so they return capture to us.
	getVertScrollbar()->setRestoreCapture(true);
	getHorzScrollbar()->setRestoreCapture(true);
}


/*************************************************************************
	Handler for when list selection is confirmed.
*************************************************************************/
void ComboDropList::onListSelectionAccepted(WindowEventArgs& e)
{
	fireEvent(EventListSelectionAccepted, e, EventNamespace);
}


/*************************************************************************
	Handler for mouse movement events
*************************************************************************/
void ComboDropList::onMouseMove(MouseEventArgs& e)
{
	Listbox::onMouseMove(e);

	// if mouse is within our area (but not our children)
	if (isHit(e.position))
	{
		if (!getChildAtPosition(e.position))
		{
			// handle auto-arm
			if (d_autoArm)
			{
				d_armed = true;
			}

			if (d_armed)
			{
				//
				// Convert mouse position to absolute window pixels
				//
				Point localPos(CoordConverter::screenToWindow(*this, e.position));

				// check for an item under the mouse
				ListboxItem* selItem = getItemAtPoint(localPos);

				// if an item is under mouse, select it
				if (selItem)
				{
					setItemSelectState(selItem, true);
				}
				else
				{
					clearAllSelections();
				}

			}
		}

		e.handled = true;
	}
	// not within the list area
	else
	{
		// if left mouse button is down, clear any selection
		if (e.sysKeys & LeftMouse)
		{
			clearAllSelections();
		}

	}

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
			clearAllSelections();
			releaseInput();
		}
		else
		{
			d_armed = true;
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
		if (d_armed && (getChildAtPosition(e.position) == 0))
		{
			releaseInput();

			// if something was selected, confirm that selection.
			if (getSelectedCount() > 0)
			{
				WindowEventArgs args(this);
				onListSelectionAccepted(args);
			}

		}
		// if we are not already armed, in response to a left button up event, we auto-arm.
		else
		{
			d_armed = true;
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
	d_armed = false;
	hide();
	e.handled = true;
}


/*************************************************************************
	Handler for when window is activated
*************************************************************************/
void ComboDropList::onActivated(ActivationEventArgs& e)
{
	Listbox::onActivated(e);
}

} // End of  CEGUI namespace section
