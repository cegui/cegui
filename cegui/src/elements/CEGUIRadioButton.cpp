/***********************************************************************
	filename: 	CEGUIRadioButton.cpp
	created:	13/4/2004
	author:		Paul D Turner

	purpose:	Implementation of RadioButton widget base class
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
#include "elements/CEGUIRadioButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String RadioButton::EventNamespace("RadioButton");
const String RadioButton::WidgetTypeName("CEGUI/RadioButton");


/*************************************************************************
	Definitions of Properties for this class
*************************************************************************/
RadioButtonProperties::Selected	RadioButton::d_selectedProperty;
RadioButtonProperties::GroupID	RadioButton::d_groupIDProperty;


/*************************************************************************
	Event name constants
*************************************************************************/
// generated internally by Window
const String RadioButton::EventSelectStateChanged( "SelectStateChanged" );


/*************************************************************************
	Constructor
*************************************************************************/
RadioButton::RadioButton(const String& type, const String& name) :
	ButtonBase(type, name),
	d_selected(false),
	d_groupID(0)
{
	addRadioButtonProperties();
}


/*************************************************************************
	Destructor
*************************************************************************/
RadioButton::~RadioButton(void)
{
}


/*************************************************************************
	set whether the radio button is selected or not
*************************************************************************/
void RadioButton::setSelected(bool select)
{
	if (select != d_selected)
	{
		d_selected = select;
		requestRedraw();

		// if new state is 'selected', we must de-select any selected radio buttons within our group.
		if (d_selected)
		{
			deselectOtherButtonsInGroup();
		}

		WindowEventArgs args(this);
		onSelectStateChanged(args);
	}

}


/*************************************************************************
	set the groupID for this radio button
*************************************************************************/
void RadioButton::setGroupID(ulong group)
{
	d_groupID = group;

	if (d_selected)
	{
		deselectOtherButtonsInGroup();
	}

}


/*************************************************************************
	Deselect any selected radio buttons attached to the same parent
	within the same group (but not do not deselect 'this').
*************************************************************************/
void RadioButton::deselectOtherButtonsInGroup(void) const
{
	// nothing to do unless we are attached to another window.
	if (d_parent)
	{
		size_t child_count = d_parent->getChildCount();

		// scan all children
		for (size_t child = 0; child < child_count; ++child)
		{
			// is this child same type as we are?
			if (d_parent->getChildAtIdx(child)->getType() == getType())
			{
				RadioButton* rb = (RadioButton*)d_parent->getChildAtIdx(child);

				// is child same group, selected, but not 'this'?
				if (rb->isSelected() && (rb != this) && (rb->getGroupID() == d_groupID))
				{
					// deselect the radio button.
					rb->setSelected(false);
				}

			}

		}

	}

}


/*************************************************************************
	event triggered internally when the select state of the button changes.
*************************************************************************/
void RadioButton::onSelectStateChanged(WindowEventArgs& e)
{
	fireEvent(EventSelectStateChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when mouse button gets released
*************************************************************************/
void RadioButton::onMouseButtonUp(MouseEventArgs& e)
{
	if ((e.button == LeftButton) && isPushed())
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet)
		{
			// if mouse was released over this widget
			if (this == sheet->getTargetChildAtPosition(e.position))
			{
				// select this button & deselect all others in the same group.
				setSelected(true);
			}

		}

		e.handled = true;
	}

	// default handling
	ButtonBase::onMouseButtonUp(e);
}


/*************************************************************************
	Return a pointer to the RadioButton object within the same group as
	this RadioButton, that is currently selected.
*************************************************************************/
RadioButton* RadioButton::getSelectedButtonInGroup(void) const
{
	// Only search we we are a child window
	if (d_parent)
	{
		size_t child_count = d_parent->getChildCount();

		// scan all children
		for (size_t child = 0; child < child_count; ++child)
		{
			// is this child same type as we are?
			if (d_parent->getChildAtIdx(child)->getType() == getType())
			{
				RadioButton* rb = (RadioButton*)d_parent->getChildAtIdx(child);

				// is child same group and selected?
				if (rb->isSelected() && (rb->getGroupID() == d_groupID))
				{
					// return the matching RadioButton pointer (may even be 'this').
					return rb;
				}

			}

		}

	}

	// no selected button attached to this window is in same group
	return 0;
}

/*************************************************************************
	Add properties for radio button
*************************************************************************/
void RadioButton::addRadioButtonProperties(void)
{
	addProperty(&d_selectedProperty);
	addProperty(&d_groupIDProperty);
}


} // End of  CEGUI namespace section
