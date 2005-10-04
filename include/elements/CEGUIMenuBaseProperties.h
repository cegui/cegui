/************************************************************************
	filename: 	CEGUIMenuBaseProperties.h
	created:	5/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _CEGUIMenuBaseProperties_h_
#define _CEGUIMenuBaseProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ItemListBaseProperties namespace section
namespace MenuBaseProperties
{

/*!
\brief
	Property to access the item spacing of the menu.

	\par Usage:
		- Name: ItemSpacing
		- Format: "[float]".

	\par Where:
		- [float] represents the item spacing of the menu.
*/
class ItemSpacing : public Property
{
public:
	ItemSpacing() : Property(
		"ItemSpacing",
		"Property to get/set the item spacing of the menu.  Value is a float.",
		"10.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to access the state of the allow multiple popups setting.

    \par Usage:
	    - Name: AllowMultiplePopups
	    - Format: "[text]".

    \par Where [Text] is:
	    - "True" to indicate that auto resizing is enabled.
	    - "False" to indicate that auto resizing is disabled.
*/
class AllowMultiplePopups : public Property
{
public:
    AllowMultiplePopups() : Property(
	    "AllowMultiplePopups",
	    "Property to get/set the state of the allow multiple popups setting for the menu.  Value is either \"True\" or \"False\".",
	    "False")
    {}

    String	get(const PropertyReceiver* receiver) const;
    void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  MenuBaseProperties namespace section
} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIMenuBaseProperties_h_
