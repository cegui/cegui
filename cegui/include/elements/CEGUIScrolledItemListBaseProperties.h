/************************************************************************
    filename:   CEGUIScrolledItemListBaseProperties.h
    created:    Tue Nov 1 2005
    author:     Tomas Lindquist Olsen
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
#ifndef _CEGUIScrolledItemListBaseProperties_h_
#define _CEGUIScrolledItemListBaseProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ItemListboxProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface for the ScrolledItemListBase class
*/
namespace ScrolledItemListBaseProperties
{
    /*!
    \brief
        Property to access the state of the force vertical scrollbar setting.

        \par Usage:
            - Name: ForceVertScrollbar
            - Format: "[text]".

        \par Where [Text] is:
            - "True" to indicate that the vertical scrollbar should always be shown.
            - "False" to indicate that the vertical scrollbar should only be shown when needed.
    */
    class ForceVertScrollbar : public Property
    {
    public:
        ForceVertScrollbar() : Property(
            "ForceVertScrollbar",
            "Property to get/set the state of the force vertical scrollbar setting for the ScrolledItemListBase.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the state of the force horizontal scrollbar setting.

        \par Usage:
            - Name: ForceHorzScrollbar
            - Format: "[text]".

        \par Where [Text] is:
            - "True" to indicate that the horizontal scrollbar should always be shown.
            - "False" to indicate that the horizontal scrollbar should only be shown when needed.
    */
    class ForceHorzScrollbar : public Property
    {
    public:
        ForceHorzScrollbar() : Property(
            "ForceHorzScrollbar",
            "Property to get/set the state of the force horizontal scrollbar setting for the ScrolledItemListBase.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

} // End of  ScrolledItemListBaseProperties namespace section

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUScrolledItemListBaseProperties_h_
