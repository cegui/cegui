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
#include "CEGUI/String.h"
#include "CEGUI/Vector.h"
#include "CEGUI/Size.h"

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
};


/*!
\brief
	The base class for all input events.
*/
class CEGUIEXPORT InputEvent
{
public:
    InputEvent(int event_type) : eventType(event_type)  {}

	int eventType;		//!< The type of the input event
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIInputEvents_h_
