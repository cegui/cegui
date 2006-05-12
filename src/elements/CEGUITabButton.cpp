/***********************************************************************
	filename: 	CEGUITabButton.cpp
	created:	8/8/2004
	author:		Steve Streeting
	
	purpose:	Implementation of TabButton widget base class
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
#include "elements/CEGUITabButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String TabButton::EventNamespace("TabButton");
const String TabButton::WidgetTypeName("CEGUI/TabButton");


/*************************************************************************
	Event name constants
*************************************************************************/
const String TabButton::EventClicked( "Clicked" );


/*************************************************************************
	Constructor
*************************************************************************/
TabButton::TabButton(const String& type, const String& name) :
	ButtonBase(type, name),
    d_selected(false), 
    d_rightOfSelected(true)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
TabButton::~TabButton(void)
{
}


/*************************************************************************
	handler invoked internally when the button is clicked.	
*************************************************************************/
void TabButton::onClicked(WindowEventArgs& e)
{
	fireEvent(EventClicked, e, EventNamespace);
}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void TabButton::onMouseButtonUp(MouseEventArgs& e)
{
	if ((e.button == LeftButton) && isPushed())
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet)
		{
			// if mouse was released over this widget
			if (this == sheet->getChildAtPosition(e.position))
			{
				// fire event
				WindowEventArgs args(this);
				onClicked(args);
			}

		}

		e.handled = true;
	}

	// default handling
	ButtonBase::onMouseButtonUp(e);
}


/*************************************************************************
Set target window
*************************************************************************/
void TabButton::setTargetWindow(Window* wnd)
{
    d_targetWindow = wnd;
    // Copy initial text
    setText(wnd->getText());
    // Parent control will keep text up to date, since changes affect layout
}

} // End of  CEGUI namespace section
