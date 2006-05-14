/************************************************************************
    filename:   CEGUIItemListboxProperties.h
    created:    Mon Oct 31 2005
    author:     Tomas Lindquist Olsen
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
#ifndef _CEGUIItemListboxProperties_h_
#define _CEGUIItemListboxProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ItemListboxProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface for the ItemListbox class
*/
namespace ItemListboxProperties
{
    /*!
    \brief
        Property to access the state of the multiselect enabled setting.

        \par Usage:
            - Name: MultiSelect
            - Format: "[text]".

        \par Where [Text] is:
            - "True" to indicate that multiselect is enabled.
            - "False" to indicate that multiselect is disabled.
    */
    class MultiSelect : public Property
    {
    public:
        MultiSelect() : Property(
            "MultiSelect",
            "Property to get/set the state of the multiselect enabled setting for the ItemListbox.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

} // End of  ItemListboxProperties namespace section

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIItemListboxProperties_h_
