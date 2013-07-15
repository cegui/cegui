/***********************************************************************
	filename: 	InputEvents.h
	created:	10/7/2013
	author:		Timotei Dolean <timotei21@gmail.com>

	purpose:	Holds the input events which are used by the Input Aggregator
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIInputEvents_h_
#define _CEGUIInputEvents_h_

#include "CEGUI/Base.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/String.h"
#include "CEGUI/Vector.h"
#include "CEGUI/Size.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	The default input events used inside CEGUI
*/
enum InputEventType
{
    MovementInputEventType          = 0x0001,	//!< The movement of a pointer.
    PressedButtonInputEventType     = 0x0002,	//!< A button was pressed.
    ReleasedButtonInputEventType    = 0x0003,	//!< A button was released.
    TextInputEventType              = 0x0004,	//!< Text was inputted.
    ScrollInputEventType            = 0x0005,	//!< The scroll operation.

    UserDefinedInputEventType       = 0x500,   //!< This marks the beginning of user-defined events.
};


/*!
\brief
	The base class for all input events.
*/
class CEGUIEXPORT InputEvent
{
public:
    InputEvent(int event_type) : d_eventType(event_type)  {}
    virtual ~InputEvent() {}

	int d_eventType;		//!< The type of the input event
};

/*!
\brief
	Represents the movement of the pointer
*/
class CEGUIEXPORT PointerMovementInputEvent : public InputEvent
{
public:
    PointerMovementInputEvent() 
        : InputEvent(MovementInputEventType)
        , d_position(0.0f, 0.0f)
        , d_delta(0.0f, 0.0f)
    {}
    
    Vector2f d_position;      //!< The actual position of the pointer
    Vector2f d_delta;         //!< The delta from the last position
};

/*!
\brief
	Represents the input of a character
*/
class CEGUIEXPORT TextInputEvent : public InputEvent
{
public:
    TextInputEvent() : InputEvent(TextInputEventType)  {}
    
    char d_character;         //!< The character inputted
};

/*!
\brief
	Represents the pressing of a button
*/
class CEGUIEXPORT PressedButtonInputEvent : public InputEvent
{
public:
    PressedButtonInputEvent() : InputEvent(PressedButtonInputEventType) {}
    
    // TODO: aggregate the mouse/gamepad/etc buttons too
    Key d_button;             //!< The button pressed
};

/*!
\brief
	Represents the releasing of a pressed button
*/
class CEGUIEXPORT ReleasedButtonInputEvent : public InputEvent
{
public:
    ReleasedButtonInputEvent() : InputEvent(ReleasedButtonInputEventType) {}
    
    // TODO: aggregate the mouse/gamepad/etc buttons too
    Key d_button;            //!< The button released
};

/*!
\brief
	Represents the scroll operation (e.g.: mouse)
*/
class CEGUIEXPORT ScrollInputEvent : public InputEvent
{
public:
    ScrollInputEvent() 
        : InputEvent(ScrollInputEventType)
        , d_delta(0)
        , d_scrollDirection(0)
    {}
    
    float d_delta;            //!< The amount of scroll since last event
    int d_scrollDirection;    //!< 0 for horizontal and 1 for vertical
};

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif	// end of guard _CEGUIInputEvents_h_
