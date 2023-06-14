/***********************************************************************
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
#include "CEGUI/widgets/TabButton.h"
#include "CEGUI/GUIContext.h"

namespace CEGUI
{
const String TabButton::EventNamespace("TabButton");
const String TabButton::WidgetTypeName("CEGUI/TabButton");
const String TabButton::EventClicked( "Clicked" );
const String TabButton::EventDragged( "Dragged" );
const String TabButton::EventScrolled( "Scrolled" );

//----------------------------------------------------------------------------//
TabButton::TabButton(const String& type, const String& name) :
	ButtonBase(type, name),
    d_selected(false),
    d_dragging(false),
    d_targetWindow(nullptr)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
TabButton::~TabButton()
{
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

/*************************************************************************
	handler invoked internally when the button is clicked.
*************************************************************************/
void TabButton::onClicked(WindowEventArgs& e)
{
	fireEvent(EventClicked, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void TabButton::onMouseButtonDown(MouseButtonEventArgs& e)
{
    if (e.d_button == MouseButton::Middle)
    {
        activate();
        if (captureInput())
        {
            ++e.handled;
            d_dragging = true;
            fireEvent(EventDragged, e, EventNamespace);
        }
    }

    ButtonBase::onMouseButtonDown(e);
}

//----------------------------------------------------------------------------//
void TabButton::onMouseButtonUp(MouseButtonEventArgs& e)
{
    if (e.d_button == MouseButton::Middle)
    {
        d_dragging = false;
        releaseInput();
        ++e.handled;
    }

    ButtonBase::onMouseButtonUp(e);
}

//----------------------------------------------------------------------------//
void TabButton::onClick(MouseButtonEventArgs& e)
{
    if (e.d_button == MouseButton::Left && isPushed())
	{
		WindowEventArgs args(this);
		onClicked(args);
		++e.handled;
    }

    ButtonBase::onClick(e);
}

//----------------------------------------------------------------------------//
void TabButton::onCursorMove(CursorMoveEventArgs& e)
{
    if (d_dragging)
    {
        fireEvent(EventDragged, e, EventNamespace);
        ++e.handled;
    }

	ButtonBase::onCursorMove(e);
}

}
