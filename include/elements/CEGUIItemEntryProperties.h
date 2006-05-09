/************************************************************************
	filename: 	CEGUIItemEntryProperties.h
	created:	Tue Mar 21 2006
	author:		Tomas Lindquist Olsen
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)

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
#ifndef _CEGUIItemEntryProperties_h_
#define _CEGUIItemEntryProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ItemEntryProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface for the ItemEntry class
*/
namespace ItemEntryProperties
{
    /*!
    \brief
	    Property to access the state of the selectable setting.

	    \par Usage:
		    - Name: Selectable
		    - Format: "[text]".

	    \par Where [text] is:
		    - "True" to indicate that the item is selectable.
		    - "False" to indicate that the item is not selectable.
    */
    class Selectable : public Property
    {
    public:
	    Selectable() : Property(
		    "Selectable",
		    "Property to get/set the state of the selectable setting for the ItemEntry.  Value is either \"True\" or \"False\".",
		    "False")
	    {}

	    String	get(const PropertyReceiver* receiver) const;
	    void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the state of the selected setting.

	    \par Usage:
		    - Name: Selected
		    - Format: "[text]".

	    \par Where [text] is:
		    - "True" to indicate that the item is selectable.
		    - "False" to indicate that the item is not selectable.
    */
    class Selected : public Property
    {
    public:
	    Selected() : Property(
		    "Selected",
		    "Property to get/set the state of the selected setting for the ItemEntry.  Value is either \"True\" or \"False\".",
		    "False")
	    {}

	    String	get(const PropertyReceiver* receiver) const;
	    void	set(PropertyReceiver* receiver, const String& value);
    };

} // End of  ItemEntryProperties namespace section
} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIItemEntryProperties_h_
