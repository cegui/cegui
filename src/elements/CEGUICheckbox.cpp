/************************************************************************
	filename: 	CEGUICheckbox.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Checkbox base class
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
#include "elements/CEGUICheckbox.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Event name constants
*************************************************************************/
// generated internally by Window
const utf8	Checkbox::CheckStateChanged[]		= "CheckStateChanged";


/*************************************************************************
	Constructor
*************************************************************************/
Checkbox::Checkbox(const String& type, const String& name) :
	ButtonBase(type, name),
	d_selected(false)
{
	// add events for this widget
	addCheckboxEvents();
}


/*************************************************************************
	Destructor
*************************************************************************/
Checkbox::~Checkbox(void)
{
}


/*************************************************************************
	set whether the check-box is selected or not	
*************************************************************************/
void Checkbox::setSelected(bool select)
{
	if (select != d_selected)
	{
		d_selected = select;
		requestRedraw();

		onSelectStateChange(WindowEventArgs(this));
	}

}


/*************************************************************************
	event triggered internally when state of check-box changes	
*************************************************************************/
void Checkbox::onSelectStateChange(WindowEventArgs& e)
{
	fireEvent(CheckStateChanged, e);
}


/*************************************************************************
	Handler for mouse button up events
*************************************************************************/
void Checkbox::onMouseButtonUp(MouseEventArgs& e)
{
	// default handling
	ButtonBase::onMouseButtonUp(e);

	Window* sheet = System::getSingleton().getGUISheet();

	if (sheet != NULL)
	{
		// if mouse was released over this widget
		if (this == sheet->getChildAtPosition(e.position))
		{
			// toggle selected state
			setSelected(d_selected ^ true);
		}

	}

	e.handled = true;
}


/*************************************************************************
	Add check-box specific events
*************************************************************************/
void Checkbox::addCheckboxEvents(void)
{
	addEvent(CheckStateChanged);
}


} // End of  CEGUI namespace section
