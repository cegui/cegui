/************************************************************************
	filename: 	CEGUIMultiColumnListProperties.h
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to multi-column list properties.
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
#ifndef _CEGUIMultiColumnListProperties_h_
#define _CEGUIMultiColumnListProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of MultiColumnListProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the MultiColumnList class
*/
namespace MultiColumnListProperties
{
/*!
\brief
	Property to access the setting for user sizing of the column headers.

	\par Usage:
		- Name: ColumnsSizable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the column headers can be sized by the user.
		- "False" to indicate the column headers can not be sized by the user.
*/
class ColumnsSizable : public Property
{
public:
	ColumnsSizable() : Property((utf8*)"ColumnsSizable", (utf8*)"Property to get/set the setting for user sizing of the column headers.  Value is either \"True\" or \"False\".") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for user moving of the column headers.

	\par Usage:
		- Name: ColumnsMovable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the column headers can be moved by the user.
		- "False" to indicate the column headers can not be moved by the user.
*/
class ColumnsMovable : public Property
{
public:
	ColumnsMovable() : Property((utf8*)"ColumnsMovable", (utf8*)"Property to get/set the setting for user moving of the column headers.  Value is either \"True\" or \"False\".") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for user modification of the sort column & direction.

	\par Usage:
		- Name: SortSettingEnabled
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the user may modify the sort column and direction by clicking the header segments.
		- "False" to indicate the user may not modify the sort column or direction.
*/
class SortSettingEnabled : public Property
{
public:
	SortSettingEnabled() : Property((utf8*)"SortSettingEnabled", (utf8*)"Property to get/set the setting for for user modification of the sort column & direction.  Value is either \"True\" or \"False\".") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the sort direction setting of the list.

	\par Usage:
		- Name: SortDirection
		- Format: "[text]"

	\par Where [Text] is one of:
		- "Ascending"
		- "Descending"
		- "None"
*/
class SortDirection : public Property
{
public:
	SortDirection() : Property((utf8*)"SortDirection", (utf8*)"Property to get/set the sort direction setting of the list.  Value is the text of one of the SortDirection enumerated value names.") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the current sort column (via ID code).

	\par Usage:
		- Name: SortColumnID
		- Format: "[uint]".

	\par Where:
		- [uint] is any unsigned integer value.
*/
class SortColumnID : public Property
{
public:
	SortColumnID() : Property((utf8*)"SortColumnID", (utf8*)"Property to get/set the current sort column (via ID code).  Value is an unsigned integer number.") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the nominated selection column (via ID).

	\par Usage:
		- Name: NominatedSelectionColumnID
		- Format: "[uint]".

	\par Where:
		- [uint] is any unsigned integer value representing the ID code of the column to be used.
*/
class NominatedSelectionColumnID : public Property
{
public:
	NominatedSelectionColumnID() : Property((utf8*)"NominatedSelectionColumnID", (utf8*)"Property to get/set the nominated selection column (via ID).  Value is an unsigned integer number.") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the nominated selection row.

	\par Usage:
		- Name: NominatedSelectionRow
		- Format: "[uint]".

	\par Where:
		- [uint] is any unsigned integer value representing the index of the row to be used.
*/
class NominatedSelectionRow : public Property
{
public:
	NominatedSelectionRow() : Property((utf8*)"NominatedSelectionRow", (utf8*)"Property to get/set the nominated selection row.  Value is an unsigned integer number.") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the 'always show' setting for the vertical scroll bar of the list box.

	\par Usage:
		- Name: ForceVertScrollbar
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that the vertical scroll bar will always be shown.
		- "False" to indicate that the vertical scroll bar will only be shown when it is needed.
*/
class ForceVertScrollbar : public Property
{
public:
	ForceVertScrollbar() : Property((utf8*)"ForceVertScrollbar", (utf8*)"Property to get/set the 'always show' setting for the vertical scroll bar of the list box.  Value is either \"True\" or \"False\".") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the 'always show' setting for the horizontal scroll bar of the list box.

	\par Usage:
		- Name: ForceHorzScrollbar
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that the horizontal scroll bar will always be shown.
		- "False" to indicate that the horizontal scroll bar will only be shown when it is needed.
*/
class ForceHorzScrollbar : public Property
{
public:
	ForceHorzScrollbar() : Property((utf8*)"ForceHorzScrollbar", (utf8*)"Property to get/set the 'always show' setting for the horizontal scroll bar of the list box.  Value is either \"True\" or \"False\".") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the selection mode setting of the list.

	\par Usage:
		- Name: SelectionMode
		- Format: "[text]"

	\par Where [Text] is one of:
		- "RowSingle"
		- "RowMultiple"
		- "CellSingle"
		- "CellMultiple"
		- "NominatedColumnSingle"
		- "NominatedColumnMultiple"
		- "ColumnSingle"
		- "ColumnMultiple"
		- "NominatedRowSingle"
		- "NominatedRowMultiple"
*/
class SelectionMode : public Property
{
public:
	SelectionMode() : Property((utf8*)"SelectionMode", (utf8*)"Property to get/set the selection mode setting of the list.  Value is the text of one of the SelectionMode enumerated value names.") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};





} // End of  MultiColumnListProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIMultiColumnListProperties_h_
