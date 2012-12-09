/***********************************************************************
	filename: 	CEGUITitlebarProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface for title bar property classes
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
#ifndef _CEGUITitlebarProperties_h_
#define _CEGUITitlebarProperties_h_

#include "../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of TitlebarProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Titlebar class
*/
namespace TitlebarProperties
{
/*!
\brief
	Property to access the state of the dragging enabled setting for the Titlebar.

	\par Usage:
		- Name: DraggingEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate that drag moving is enabled.
		- "False" to indicate that drag moving is disabled.
*/
class DraggingEnabled : public Property
{
public:
	DraggingEnabled() : Property(
		"DraggingEnabled",
		"Property to get/set the state of the dragging enabled setting for the Titlebar.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  TitlebarProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUITitlebarProperties_h_
