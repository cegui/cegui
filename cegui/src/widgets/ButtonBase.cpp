/***********************************************************************
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
#include "CEGUI/widgets/ButtonBase.h"
#include "CEGUI/Cursor.h"

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
void ButtonBase::updateInternalState(const glm::vec2& cursor_pos)
{
	const bool oldstate = d_hovering;

    d_hovering = calculateCurrentHoverState(cursor_pos);

	if (oldstate != d_hovering)
		invalidate();
}

//----------------------------------------------------------------------------//
bool ButtonBase::calculateCurrentHoverState(const glm::vec2& cursor_pos)
{
	if (const Window* capture_wnd = getCaptureWindow())
        return
            (capture_wnd == this ||
            (capture_wnd->distributesCapturedInputs() && isAncestor(capture_wnd))) && isHit(cursor_pos);
    else
	    return getGUIContext().getWindowContainingCursor() == this;
}

/*************************************************************************
	Handler for when the cursor moves
*************************************************************************/
void ButtonBase::onCursorMove(CursorInputEventArgs& e)
{
    // this is needed to discover whether cursor is in the widget area or not.
	// The same thing used to be done each frame in the rendering method,
	// but in this version the rendering method may not be called every frame
	// so we must discover the internal widget state here - which is actually
	// more efficient anyway.

	// base class processing
	Window::onCursorMove(e);

	updateInternalState(e.position);
	++e.handled;
}


/*************************************************************************
	Handler for cursor press hold events
*************************************************************************/
void ButtonBase::onCursorPressHold(CursorInputEventArgs& e)
{
	// default processing
    Window::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
	{
        if (captureInput())
        {
			d_pushed = true;
			updateInternalState(e.position);
			invalidate();
        }

		// event was handled by us.
		++e.handled;
	}
}

//----------------------------------------------------------------------------//
void ButtonBase::setPushedState(const bool pushed)
{
    d_pushed = pushed;

    if (!pushed)
	    updateInternalState(getUnprojectedPosition(
            getGUIContext().getCursor().getPosition()));
    else
        d_hovering = true;

    invalidate();
}

/*************************************************************************
	Handler for cursor activation events
*************************************************************************/
void ButtonBase::onCursorActivate(CursorInputEventArgs& e)
{
	// default processing
    Window::onCursorActivate(e);

    if (e.source == CursorInputSource::Left)
	{
		releaseInput();

		// event was handled by us.
		++e.handled;
	}

}

/*************************************************************************
    Handler for when cursor capture is lost
*************************************************************************/
void ButtonBase::onCaptureLost(WindowEventArgs& e)
{
	// Default processing
	Window::onCaptureLost(e);

	d_pushed = false;
    getGUIContext().updateWindowContainingCursor();
	invalidate();

	// event was handled by us.
	++e.handled;
}


/*************************************************************************
    Handler for when cursor leaves the widget
*************************************************************************/
void ButtonBase::onCursorLeaves(CursorInputEventArgs& e)
{
    // default processing
    Window::onCursorLeaves(e);

	d_hovering = false;
	invalidate();

	++e.handled;
}

} // End of  CEGUI namespace section
