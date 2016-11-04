/***********************************************************************
	created:	28/8/2004
	author:		Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/DefaultWindow.h"
#include <limits>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const String DefaultWindow::WidgetTypeName("DefaultWindow");

/*************************************************************************
    Constructor
*************************************************************************/
DefaultWindow::DefaultWindow(const String& type, const String& name) :
    Window(type, name)
{
    setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
}

//----------------------------------------------------------------------------//
void DefaultWindow::onCursorMove(CursorInputEventArgs& e)
{
    // always call the base class handler
    Window::onCursorMove(e);
    updatePointerEventHandled(e);
}

//----------------------------------------------------------------------------//
void DefaultWindow::onScroll(CursorInputEventArgs& e)
{
    // always call the base class handler
    Window::onScroll(e);
    updatePointerEventHandled(e);
}

//----------------------------------------------------------------------------//
void DefaultWindow::onCursorPressHold(CursorInputEventArgs& e)
{
    // always call the base class handler
    Window::onCursorPressHold(e);
    updatePointerEventHandled(e);
}

//----------------------------------------------------------------------------//
void DefaultWindow::onCursorActivate(CursorInputEventArgs& e)
{
    // always call the base class handler
    Window::onCursorActivate(e);
    updatePointerEventHandled(e);
}

//----------------------------------------------------------------------------//
void DefaultWindow::updatePointerEventHandled(CursorInputEventArgs& e) const
{
    // by default, if we are a root window (no parent) with pass-though enabled
    // we do /not/ mark cursor events as handled.
    if (!d_parent && e.handled && d_cursorPassThroughEnabled)
        --e.handled;
}

//----------------------------------------------------------------------------//
bool DefaultWindow::moveToFront_impl(bool wasClicked)
{
    const bool took_action = Window::moveToFront_impl(wasClicked);

    if (!d_parent && d_cursorPassThroughEnabled)
        return false;
    else
        return took_action;
}

//----------------------------------------------------------------------------//
void DefaultWindow::onSemanticInputEvent(SemanticEventArgs& e)
{
    // always call the base class handler
    Window::onSemanticInputEvent(e);
}

//----------------------------------------------------------------------------//
bool DefaultWindow::canFocus()
{
    // cannot focus something that doesn't have an explicit visual appearance
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
