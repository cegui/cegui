/***********************************************************************
	filename: 	CEGUIMultiColumnListProperties.h
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to multi-column list properties.
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
	ColumnsSizable() : Property(
		"ColumnsSizable",
		"Property to get/set the setting for user sizing of the column headers.  Value is either \"True\" or \"False\".",
		"True")
	{}

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
	ColumnsMovable() : Property(
		"ColumnsMovable",
		"Property to get/set the setting for user moving of the column headers.  Value is either \"True\" or \"False\".",
		"True")
	{}

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
	SortSettingEnabled() : Property(
		"SortSettingEnabled",
		"Property to get/set the setting for for user modification of the sort column & direction.  Value is either \"True\" or \"False\".",
		"True")
	{}

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
	SortDirection() : Property(
		"SortDirection",
		"Property to get/set the sort direction setting of the list.  Value is the text of one of the SortDirection enumerated value names.",
		"None")
	{}

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
	SortColumnID() : Property(
		"SortColumnID",
		"Property to get/set the current sort column (via ID code).  Value is an unsigned integer number.",
		"0", false)
	{}

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
	NominatedSelectionColumnID() : Property(
		"NominatedSelectionColumnID",
		"Property to get/set the nominated selection column (via ID).  Value is an unsigned integer number.",
		"0")
	{}

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
	NominatedSelectionRow() : Property(
		"NominatedSelectionRow",
		"Property to get/set the nominated selection row.  Value is an unsigned integer number.",
		"0")
	{}

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
	ForceVertScrollbar() : Property(
		"ForceVertScrollbar",
		"Property to get/set the 'always show' setting for the vertical scroll bar of the list box.  Value is either \"True\" or \"False\".",
		"False")
	{}

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
	ForceHorzScrollbar() : Property(
		"ForceHorzScrollbar",
		"Property to get/set the 'always show' setting for the horizontal scroll bar of the list box.  Value is either \"True\" or \"False\".",
		"False")
	{}

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
	SelectionMode() : Property(
		"SelectionMode",
		"Property to get/set the selection mode setting of the list.  Value is the text of one of the SelectionMode enumerated value names.",
		"RowSingle")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access a column.

	\par Usage:
		- Name: ColumnHeader
		- Format: "text:[caption] width:{s,o} id:[uint]"

	\par where:
		- [caption] is the column header caption text.
		- [{s,o}] is a UDim specification.
		- [uint] is the unique ID for the column.
*/
class ColumnHeader : public Property
{
public:
	ColumnHeader() : Property(
		"ColumnHeader",
		"Property to set up a column (there is no getter for this property)",
		"", false)
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
	Property to access the number of rows in the list (read-only)

	\par Usage:
		- Name: RowCount
		- Format: "" (property is read-only).
*/
class RowCount : public Property
{
public:
	RowCount() : Property(
		"RowCount",
		"Property to access the number of rows in the list (read only)",
		"", false)
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


} // End of  MultiColumnListProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIMultiColumnListProperties_h_
