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
#include "CEGUI/GUIContext.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
ButtonBase::ButtonBase(const String& type, const String& name)
    : Window(type, name)
{
}

//----------------------------------------------------------------------------//
void ButtonBase::updateInternalState(const glm::vec2& cursor_pos)
{
	const bool oldState = d_hovering;
    d_hovering = calculateCurrentHoverState(cursor_pos);
	if (oldState != d_hovering)
		invalidate();
}

//----------------------------------------------------------------------------//
bool ButtonBase::calculateCurrentHoverState(const glm::vec2& cursor_pos)
{
    if (!d_guiContext)
        return false;

    if (auto captureWnd = d_guiContext->getInputCaptureWindow())
    {
        return
            ((captureWnd == this) || (captureWnd->distributesCapturedInputs() && isDescendantOf(captureWnd))) &&
            isHit(cursor_pos);
    }

	return d_guiContext->getWindowContainingCursor() == this;
}

//----------------------------------------------------------------------------//
void ButtonBase::onCursorMove(CursorMoveEventArgs& e)
{
    // this is needed to discover whether cursor is in the widget area or not.
	// The same thing used to be done each frame in the rendering method,
	// but in this version the rendering method may not be called every frame
	// so we must discover the internal widget state here - which is actually
	// more efficient anyway.

	Window::onCursorMove(e);

	updateInternalState(e.d_surfacePos);
	++e.handled;
}

//----------------------------------------------------------------------------//
void ButtonBase::onMouseButtonDown(MouseButtonEventArgs& e)
{
    Window::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left)
	{
        if (captureInput())
        {
			d_pushed = true;
			updateInternalState(e.d_surfacePos);
			invalidate();
        }

		++e.handled;
	}
}

//----------------------------------------------------------------------------//
void ButtonBase::onMouseButtonUp(MouseButtonEventArgs& e)
{
    Window::onMouseButtonUp(e);

    if (e.d_button == MouseButton::Left)
    {
        releaseInput();
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void ButtonBase::setPushedState(bool pushed)
{
    d_pushed = pushed;

    if (!pushed)
	    updateInternalState(getUnprojectedPosition(d_guiContext->getCursorPosition()));
    else
        d_hovering = true;

    invalidate();
}

//----------------------------------------------------------------------------//
void ButtonBase::onCaptureLost(WindowEventArgs& e)
{
	Window::onCaptureLost(e);

	d_pushed = false;
    d_guiContext->updateWindowContainingCursor();
	invalidate();
	++e.handled;
}

//----------------------------------------------------------------------------//
void ButtonBase::onCursorLeaves(CursorInputEventArgs& e)
{
    Window::onCursorLeaves(e);

	d_hovering = false;
	invalidate();
	++e.handled;
}

}
