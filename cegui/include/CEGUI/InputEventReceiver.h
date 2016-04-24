/***********************************************************************
	created:	10/7/2013
	author:		Timotei Dolean <timotei21@gmail.com>

	purpose:	Provides an interface for injecting input events
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
#ifndef _CEGUIInputEventReceiver_h_
#define _CEGUIInputEventReceiver_h_

#include "CEGUI/Base.h"
#include "CEGUI/InputEvents.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//!	Interface to be implemented by classes that handle input events
class CEGUIEXPORT InputEventReceiver
{
public:
    virtual ~InputEventReceiver() {}

    /*!
    \brief 
        Injects a new input event.

    \param event
        The input event to be injected.
    
    \return
        - true if the input was processed by the input receiver.
        - false if the input was not processed by the input receiver.
    */
	virtual bool injectInputEvent(const InputEvent& event) = 0;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIInputEventReceiver_h_
