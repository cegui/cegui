/***********************************************************************
	filename: 	CEGUICheckboxProperties.h
	created:	9/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for the Checkbox class
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
#ifndef _CEGUICheckboxProperties_h_
#define _CEGUICheckboxProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of CheckboxProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Checkbox class
*/
namespace CheckboxProperties
{
/*!
\brief
	Property to access the selected state of the check box.

	This property offers access to the select state for the Checkbox object.

	\par Usage:
		- Name: Selected
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the check box is selected (has check mark).
		- "False" to indicate the check box is not selected (does not have check mark).
*/
class Selected : public Property
{
public:
	Selected() : Property(
		"Selected",
		"Property to get/set the selected state of the Checkbox.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  CheckboxProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUICheckboxProperties_h_
