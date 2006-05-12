/***********************************************************************
	filename: 	CEGUIButtonBase.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of ButtonBase widget
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
#include "elements/CEGUIButtonBase.h"
#include "CEGUIMouseCursor.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constructor
*************************************************************************/
ButtonBase::ButtonBase(const String& type, const String& name) :
	Window(type, name),
	d_pushed(false),
	d_hovering(false)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
ButtonBase::~ButtonBase(void)
{
}


/*************************************************************************
	Update the internal state of the Widget
*************************************************************************/
void ButtonBase::updateInternalState(const Point& mouse_pos)
{
	bool oldstate = d_hovering;

	// assume not hovering 
	d_hovering = false;

	// if input is captured, but not by 'this', then we never hover highlight
	const Window* capture_wnd = getCaptureWindow();

	if ((capture_wnd == 0) || (capture_wnd == this))
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet)
        {
			d_hovering = (this == sheet->getChildAtPosition(mouse_pos));
        }
    }

	// if state has changed, trigger a re-draw
	if (oldstate != d_hovering)
	{
		requestRedraw();
	}

}


/*************************************************************************
	Handler for when the mouse moves
*************************************************************************/
void ButtonBase::onMouseMove(MouseEventArgs& e)
{
	// this is needed to discover whether mouse is in the widget area or not.
	// The same thing used to be done each frame in the rendering method,
	// but in this version the rendering method may not be called every frame
	// so we must discover the internal widget state here - which is actually
	// more efficient anyway.

	// base class processing
	Window::onMouseMove(e);

	updateInternalState(e.position);
	e.handled = true;
}


/*************************************************************************
	Handler for mouse button pressed events
*************************************************************************/
void ButtonBase::onMouseButtonDown(MouseEventArgs& e)
{
	// default processing
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		if (captureInput())
		{
			d_pushed = true;
			updateInternalState(e.position);
			requestRedraw();
		}

		// event was handled by us.
		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void ButtonBase::onMouseButtonUp(MouseEventArgs& e)
{
	// default processing
	Window::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		releaseInput();

		// event was handled by us.
		e.handled = true;
	}

}

/*************************************************************************
	Handler for when mouse capture is lost
*************************************************************************/
void ButtonBase::onCaptureLost(WindowEventArgs& e)
{
	// Default processing
	Window::onCaptureLost(e);

	d_pushed = false;
	updateInternalState(MouseCursor::getSingleton().getPosition());
	requestRedraw();

	// event was handled by us.
	e.handled = true;
}


/*************************************************************************
	Handler for when mouse leaves the widget
*************************************************************************/
void ButtonBase::onMouseLeaves(MouseEventArgs& e)
{
	// deafult processing
	Window::onMouseLeaves(e);

	d_hovering = false;
	requestRedraw();

	e.handled = true;
}

} // End of  CEGUI namespace section
