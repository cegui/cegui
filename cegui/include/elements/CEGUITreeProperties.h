/***********************************************************************
    filename:   CEGUITreeProperties.h
    created:	5-13-07
    author:		Jonathan Welch (Based on Code by David Durant)
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUITreeProperties_h_
#define _CEGUITreeProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of TreeProperties namespace section
/*!
\brief
	Namespace containing classes that make up the properties interface specific
    to the Tree class.
*/
namespace TreeProperties
{
/*!
\brief
	Property to access the sort setting of the Tree.

	\par Usage:
		- Name: Sort
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the tree items should be sorted.
		- "False" to indicate the tree items should not be sorted.
*/
class Sort : public Property
{
public:
	Sort() : Property(
		"Sort",
		"Property to get/set the sort setting of the tree.  "
        "Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the multi-select setting of the tree.

	\par Usage:
		- Name: MultiSelect
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that multiple items may be selected.
		- "False" to indicate that only a single item may be selected.
*/
class MultiSelect : public Property
{
public:
	MultiSelect() : Property(
		"MultiSelect",
		"Property to get/set the multi-select setting of the tree.  "
        "Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the 'always show' setting for the vertical scroll bar of
    the tree.

	\par Usage:
		- Name: ForceVertScrollbar
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that the vertical scroll bar will always be shown.
		- "False" to indicate that the vertical scroll bar will only be shown
          when it is needed.
*/
class ForceVertScrollbar : public Property
{
public:
	ForceVertScrollbar() : Property(
		"ForceVertScrollbar",
		"Property to get/set the 'always show' setting for the vertical scroll "
        "bar of the tree.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the 'always show' setting for the horizontal scroll bar
    of the tree.

	\par Usage:
		- Name: ForceHorzScrollbar
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that the horizontal scroll bar will always be
          shown.
		- "False" to indicate that the horizontal scroll bar will only be shown
          when it is needed.
*/
class ForceHorzScrollbar : public Property
{
public:
	ForceHorzScrollbar() : Property(
		"ForceHorzScrollbar",
		"Property to get/set the 'always show' setting for the horizontal "
        "scroll bar of the tree.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the show item tooltips setting of the tree.

    \par Usage:
        - Name: ItemTooltips
        - Format: "[text]"

    \par Where [Text] is:
        - "True" to indicate that the tooltip of the tree will be set by the
          item below the mouse pointer
        - "False" to indicate that the tree has a static tooltip.
*/
class ItemTooltips : public Property
{
public:
	ItemTooltips() : Property(
		"ItemTooltips",
		"Property to access the show item tooltips setting of the tree.  "
        "Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  TreeProperties namespace section

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUITreeProperties_h_
