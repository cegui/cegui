/***********************************************************************
	filename: 	CEGUIFrameWindowProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for FrameWIndow class
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
#ifndef _CEGUIFrameWindowProperties_h_
#define _CEGUIFrameWindowProperties_h_

#include "../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of FrameWindowProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the FrameWindow class
*/
namespace FrameWindowProperties
{

/*!
\brief
	Property to access the roll-up / shade state of the window.

	\par Usage:
		- Name: RollUpState
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the window is / should be rolled-up.
		- "False" to indicate the window is not / should not be rolled up
*/
class RollUpState : public Property
{
public:
	RollUpState() : Property(
		"RollUpState",
		"Property to get/set the roll-up / shade state of the window.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  FrameWindowProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIFrameWindowProperties_h_
