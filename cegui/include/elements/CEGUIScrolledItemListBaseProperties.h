/************************************************************************
    filename:   CEGUIScrolledItemListBaseProperties.h
    created:    Tue Nov 1 2005
    author:     Tomas Lindquist Olsen
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIScrolledItemListBaseProperties_h_
#define _CEGUIScrolledItemListBaseProperties_h_

#include "../CEGUIProperty.h"

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
