/***********************************************************************
	filename: 	CEGUIItemListBaseProperties.h
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
