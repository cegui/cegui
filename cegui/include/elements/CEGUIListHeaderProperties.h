/***********************************************************************
	filename: 	CEGUIListHeaderProperties.h
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to the properties for the ListHeader class.
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
#ifndef _CEGUIListHeaderProperties_h_
#define _CEGUIListHeaderProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ListHeaderProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the ListHeader class
*/
namespace ListHeaderProperties
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
	Property to access the sort direction setting of the list header.

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
		"Property to get/set the sort direction setting of the header.  Value is the text of one of the SortDirection enumerated value names.",
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
		"0")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  ListHeaderProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIListHeaderProperties_h_
