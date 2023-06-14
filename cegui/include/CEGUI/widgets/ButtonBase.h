/***********************************************************************
	created:	13/4/2004
	author:		Paul D Turner

	purpose:	Interface to base class for ButtonBase widget
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
#pragma once
#include "../Window.h"

namespace CEGUI
{

//! \brief Base class for all the 'button' type widgets (push button, radio button, check-box, etc)
class CEGUIEXPORT ButtonBase : public Window
{
public:

    ButtonBase(const String& type, const String& name);

    /*************************************************************************
		Accessor type functions
	*************************************************************************/
	/*!
	\brief
		return true if user is hovering over this widget (or it's pushed and user is not over it for highlight)

    \return
        true if the user is hovering or if the button is pushed and the pointer is
        not over the button. Otherwise return false.
    */
	bool	isHovering() const			{return d_hovering;}


	/*!
	\brief
		Return true if the button widget is in the pushed state.

	\return
		true if the button-type widget is pushed, false if the widget is not pushed.
	*/
	bool	isPushed() const			{return d_pushed;}

    /** Internal function to set button's pushed state.  Normally you would
     * not call this, except perhaps when building compound widgets.
     */
    void setPushedState(bool pushed);

protected:
	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
    void    onCursorMove(CursorMoveEventArgs& e) override;
    void    onCursorLeaves(CursorInputEventArgs& e) override;
    void    onMouseButtonDown(MouseButtonEventArgs& e) override;
    void    onMouseButtonUp(MouseButtonEventArgs& e) override;
    void    onCaptureLost(WindowEventArgs& e) override;


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
    /*!
    \brief
    Update the internal state of the widget with the cursor at the given position.

    \param cursor_pos
        Point object describing, in screen pixel co-ordinates, the location of
        the cursor.

    \return
        Nothing
    */
    void updateInternalState(const glm::vec2& cursor_pos);

    bool calculateCurrentHoverState(const glm::vec2& cursor_pos);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool d_pushed = false;			//!< true when widget is pushed
	bool d_hovering = false;			//!< true when the button is in 'hover' state and requires the hover rendering.
};

} // End of  CEGUI namespace section
