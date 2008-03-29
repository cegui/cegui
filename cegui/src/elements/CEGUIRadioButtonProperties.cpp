/***********************************************************************
	filename: 	CEGUIRadioButtonProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for Radio Button class
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
#include "elements/CEGUIRadioButtonProperties.h"
#include "elements/CEGUIRadioButton.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of RadioButtonProperties namespace section
namespace RadioButtonProperties
{
String Selected::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const RadioButton*>(receiver)->isSelected());
}


void Selected::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<RadioButton*>(receiver)->setSelected(PropertyHelper::stringToBool(value));
}


String GroupID::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const RadioButton*>(receiver)->getGroupID());
}


void GroupID::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<RadioButton*>(receiver)->setGroupID(PropertyHelper::stringToUint(value));
}


} // End of  RadioButtonProperties namespace section

} // End of  CEGUI namespace section
