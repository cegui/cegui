/***********************************************************************
	filename: 	CEGUIPopupMenuProperties.h
	created:	5/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
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
#ifndef _CEGUIPopupMenuProperties_h_
#define _CEGUIPopupMenuProperties_h_

#include "../CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ItemListBaseProperties namespace section
namespace PopupMenuProperties
{

/*!
\brief
	Property to access the fade in time in seconds of the popup menu.

	\par Usage:
		- Name: FadeInTime
		- Format: "[float]".

	\par Where:
		- [float] represents the fade in time in seconds of the popup menu.
*/
class FadeInTime : public Property
{
public:
	FadeInTime() : Property(
		"FadeInTime",
		"Property to get/set the fade in time in seconds of the popup menu.  Value is a float.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the fade out time in seconds of the popup menu.

	\par Usage:
		- Name: FadeOutTime
		- Format: "[float]".

	\par Where:
		- [float] represents the fade out time in seconds of the popup menu.
*/
class FadeOutTime : public Property
{
public:
	FadeOutTime() : Property(
		"FadeOutTime",
		"Property to get/set the fade out time in seconds of the popup menu.  Value is a float.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  PopupMenuProperties namespace section
} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIPopupMenuProperties_h_
