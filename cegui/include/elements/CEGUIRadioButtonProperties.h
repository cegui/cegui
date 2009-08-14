/***********************************************************************
	filename: 	CEGUIRadioButtonProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for RadioButton class
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
#ifndef _CEGUIRadioButtonProperties_h_
#define _CEGUIRadioButtonProperties_h_

#include "../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of RadioButtonProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the RadioButton class
*/
namespace RadioButtonProperties
{
/*!
\brief
	Property to access the selected state of the RadioButton.

	\par Usage:
		- Name: Selected
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the radio button is selected.
		- "False" to indicate the radio button is not selected.
*/
class Selected : public Property
{
public:
	Selected() : Property(
		"Selected",
		"Property to get/set the selected state of the RadioButton.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the radio button group ID.

	\par Usage:
		- Name: GroupID
		- Format: "[uint]".

	\par Where:
		- [uint] is any unsigned integer value.
*/
class GroupID : public Property
{
public:
	GroupID() : Property(
		"GroupID",
		"Property to get/set the radio button group ID.  Value is an unsigned integer number.",
		"0")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


} // End of  RadioButtonProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIRadioButtonProperties_h_
