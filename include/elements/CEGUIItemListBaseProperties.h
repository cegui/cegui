/************************************************************************
	filename: 	CEGUIItemListBaseProperties.h
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
#ifndef _CEGUIItemListBaseProperties_h_
#define _CEGUIItemListBaseProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ItemListBaseProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface for the ItemListBase class
*/
namespace ItemListBaseProperties
{
    /*!
    \brief
	    Property to access the state of the auto resize enabled setting.

	    \par Usage:
		    - Name: AutoResizeEnabled
		    - Format: "[text]".

	    \par Where [Text] is:
		    - "True" to indicate that auto resizing is enabled.
		    - "False" to indicate that auto resizing is disabled.
    */
    class AutoResizeEnabled : public Property
    {
    public:
	    AutoResizeEnabled() : Property(
		    "AutoResizeEnabled",
		    "Property to get/set the state of the auto resizing enabled setting for the ItemListBase.  Value is either \"True\" or \"False\".",
		    "False")
	    {}

	    String	get(const PropertyReceiver* receiver) const;
	    void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the state of the sorting enabled setting.

        \par Usage:
            - Name: SortEnabled
            - Format: "[text]".

        \par Where [Text] is:
            - "True" to indicate that sorting is enabled.
            - "False" to indicate that sorting is disabled.
    */
    class SortEnabled : public Property
    {
    public:
        SortEnabled() : Property(
            "SortEnabled",
            "Property to get/set the state of the sorting enabled setting for the ItemListBase.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the sorting mode.

        \par Usage:
            - Name: SortMode
            - Format: "[text]".

        \par Where [Text] is:
            - "Ascending" to use ascending sorting.
            - "Descending" to use descending sorting.
            - "UserSort" to use a user specifed callback as sorting function,
              defaults to ascending sorting if no user callback has been specified
              by the application.
    */
    class SortMode : public Property
    {
    public:
        SortMode() : Property(
            "SortMode",
            "Property to get/set the sorting mode for the ItemListBase.  Value is either \"Ascending\", \"Descending\" or \"UserSort\".",
            "Ascending")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

} // End of  ItemListBaseProperties namespace section
} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIItemListBaseProperties_h_
