/***********************************************************************
	filename: 	CEGUICheckbox.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Checkbox base class
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
#include "elements/CEGUICheckbox.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String Checkbox::EventNamespace("Checkbox");
const String Checkbox::WidgetTypeName("CEGUI/Checkbox");


/*************************************************************************
	Definitions for Properties
*************************************************************************/
CheckboxProperties::Selected	Checkbox::d_selectedProperty;


/*************************************************************************
	Event name constants
*************************************************************************/
// generated internally by Window
const String Checkbox::EventCheckStateChanged( "CheckStateChanged" );


/*************************************************************************
	Constructor
*************************************************************************/
Checkbox::Checkbox(const String& type, const String& name) :
	ButtonBase(type, name),
	d_selected(false)
{
	addCheckboxProperties();
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

        WindowEventArgs args(this);
		onSelectStateChange(args);
	}

}


/*************************************************************************
	event triggered internally when state of check-box changes	
*************************************************************************/
void Checkbox::onSelectStateChange(WindowEventArgs& e)
{
	fireEvent(EventCheckStateChanged, e, EventNamespace);
}


/*************************************************************************
	Handler for mouse button up events
*************************************************************************/
void Checkbox::onMouseButtonUp(MouseEventArgs& e)
{
	if ((e.button == LeftButton) && isPushed())
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet)
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

	// default handling
	ButtonBase::onMouseButtonUp(e);
}


/*************************************************************************
	Add properties
*************************************************************************/
void Checkbox::addCheckboxProperties(void)
{
	addProperty(&d_selectedProperty);
}



} // End of  CEGUI namespace section
