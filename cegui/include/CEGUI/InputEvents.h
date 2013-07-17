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
    TextInputEventType              = 0x0003,	//!< Text was inputted.
    ScrollInputEventType            = 0x0004,	//!< The scroll operation.
    SemanticInputEventType          = 0x0005,	//!< An event with a certain semantic

    UserDefinedInputEventType       = 0x5000,   //!< This marks the beginning of user-defined events.
};

/*!
\brief
    Represents the value of a semantic input event, generated from a specific
    operation or sequence of operations.
*/
enum SemanticValue
{
    NoValue                         = 0x0000,
    PointerActivate                 = 0x0001,
    PointerDeactivate               = 0x0002,
    SelectMultipleItems             = 0x0003,
    SelectCumulative                = 0x0004,
    SelectWord                      = 0x0005,
    SelectAll                       = 0x0006,
    SelectLeftCharacter             = 0x0007,
    SelectRightCharacter            = 0x0008,
    GoToPreviousCharacter           = 0x0009,
    GoToNextCharacter               = 0x0010,
    GoToPreviousWord                = 0x0011,
    GoToNextWord                    = 0x0012,
    GoToStartOfLine                 = 0x0013,
    GoToEndOfLine                   = 0x0014,
    DeleteLastCharacter             = 0x0015,
    DeletePreviousCharacter         = 0x0016,
    Confirm                         = 0x0017,
    Back                            = 0x0018,
    Undo                            = 0x0019,
    Redo                            = 0x0020,
    Cut                             = 0x0021,
    Copy                            = 0x0022,
    Paste                           = 0x0023,

    UserDefinedSemanticValue        = 0x5000,   //!< This marks the beginning of user-defined semantic values.
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

    int d_eventType;        //!< The type of the input event
};

/*!
\brief
	Represents the movement of the pointer
*/
class CEGUIEXPORT PointerMovementInputEvent : public InputEvent
{
public:
    PointerMovementInputEvent() : 
        InputEvent(MovementInputEventType),
        d_position(0.0f, 0.0f),
        d_delta(0.0f, 0.0f)
    {
    }
    
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
	Represents the scroll operation (e.g.: mouse)
*/
class CEGUIEXPORT ScrollInputEvent : public InputEvent
{
public:
    ScrollInputEvent() : 
        InputEvent(ScrollInputEventType),
        d_delta(0),
        d_scrollDirection(0)
    {
    }
    
    float d_delta;            //!< The amount of scroll since last event
    int d_scrollDirection;    //!< 0 for horizontal and 1 for vertical
};


/*!
\brief
	Represents a semantic input event (e.g.: delete a previous character, confirm)
*/
class CEGUIEXPORT SemanticInputEvent : public InputEvent
{
public:
    SemanticInputEvent(SemanticValue value) : 
        InputEvent(SemanticInputEventType),
        d_value(value)
    {
    }
    
    SemanticValue d_value;            //!< The semantic value of this event
};


} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif	// end of guard _CEGUIInputEvents_h_
