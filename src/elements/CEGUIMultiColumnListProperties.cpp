/***********************************************************************
	filename: 	CEGUIMultiColumnListProperties.cpp
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Implements multi-column list properties.
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
#include "elements/CEGUIMultiColumnListProperties.h"
#include "elements/CEGUIMultiColumnList.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUILogger.h"
#include "CEGUITextUtils.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of MultiColumnListProperties namespace section
namespace MultiColumnListProperties
{
String	ColumnsSizable::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiColumnList*>(receiver)->isUserColumnSizingEnabled());
}


void	ColumnsSizable::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setUserColumnSizingEnabled(PropertyHelper::stringToBool(value));
}


String	ColumnsMovable::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiColumnList*>(receiver)->isUserColumnDraggingEnabled());
}


void	ColumnsMovable::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setUserColumnDraggingEnabled(PropertyHelper::stringToBool(value));
}


String	SortSettingEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiColumnList*>(receiver)->isUserSortControlEnabled());
}


void	SortSettingEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setUserSortControlEnabled(PropertyHelper::stringToBool(value));
}


String	SortColumnID::get(const PropertyReceiver* receiver) const
{
	const MultiColumnList* mcl = static_cast<const MultiColumnList*>(receiver);
	// scriptkid: this check was added to fix mantis ticket #219 (editor), but could also get called
	// from another client app. Also, the API docs state that getSortColumn requires at least one column,
	// so this check doesn't hurt.
	if (mcl->getColumnCount() > 0)
	{
		return PropertyHelper::uintToString(mcl->getColumnID(mcl->getSortColumn()));
	}
	return "0"; // Return default value. For the Setting part, responsibility lies with the caller again.
}


void	SortColumnID::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setSortColumnByID(PropertyHelper::stringToUint(value));
}


String	SortDirection::get(const PropertyReceiver* receiver) const
{
	switch(static_cast<const MultiColumnList*>(receiver)->getSortDirection())
	{
	case ListHeaderSegment::Ascending:
		return String("Ascending");
		break;

	case ListHeaderSegment::Descending:
		return String("Descending");
		break;

	default:
		return String("None");
		break;
	}

}


void	SortDirection::set(PropertyReceiver* receiver, const String& value)
{
	ListHeaderSegment::SortDirection dir;

	if (value == "Ascending")
	{
		dir = ListHeaderSegment::Ascending;
	}
	else if (value == "Descending")
	{
		dir = ListHeaderSegment::Descending;
	}
	else
	{
		dir = ListHeaderSegment::None;
	}

	static_cast<MultiColumnList*>(receiver)->setSortDirection(dir);
}


String	NominatedSelectionColumnID::get(const PropertyReceiver* receiver) const
{
	const MultiColumnList* mcl = static_cast<const MultiColumnList*>(receiver);
	if (mcl->getColumnCount() > 0)
	{
		return PropertyHelper::uintToString(mcl->getNominatedSelectionColumnID());
	}
	return "0"; // Return default value. For the Setting part, responsibility lies with the caller again.
}


void	NominatedSelectionColumnID::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setNominatedSelectionColumn(PropertyHelper::stringToUint(value));
}


String	NominatedSelectionRow::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const MultiColumnList*>(receiver)->getNominatedSelectionRow());
}


void	NominatedSelectionRow::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setNominatedSelectionRow(PropertyHelper::stringToUint(value));
}


String	ForceVertScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiColumnList*>(receiver)->isVertScrollbarAlwaysShown());
}


void	ForceVertScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setShowVertScrollbar(PropertyHelper::stringToBool(value));
}


String	ForceHorzScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiColumnList*>(receiver)->isHorzScrollbarAlwaysShown());
}


void	ForceHorzScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiColumnList*>(receiver)->setShowHorzScrollbar(PropertyHelper::stringToBool(value));
}


String	SelectionMode::get(const PropertyReceiver* receiver) const
{
	switch(static_cast<const MultiColumnList*>(receiver)->getSelectionMode())
	{
	case MultiColumnList::RowMultiple:
		return String("RowMultiple");
		break;

	case MultiColumnList::ColumnSingle:
		return String("ColumnSingle");
		break;

	case MultiColumnList::ColumnMultiple:
		return String("ColumnMultiple");
		break;

	case MultiColumnList::CellSingle:
		return String("CellSingle");
		break;

	case MultiColumnList::CellMultiple:
		return String("CellMultiple");
		break;

	case MultiColumnList::NominatedColumnSingle:
		return String("NominatedColumnSingle");
		break;

	case MultiColumnList::NominatedColumnMultiple:
		return String("NominatedColumnMultiple");
		break;

	case MultiColumnList::NominatedRowSingle:
		return String("NominatedRowSingle");
		break;

	case MultiColumnList::NominatedRowMultiple:
		return String("NominatedRowMultiple");
		break;

	default:
		return String("RowSingle");
		break;
	}
}


void	SelectionMode::set(PropertyReceiver* receiver, const String& value)
{
	MultiColumnList::SelectionMode mode;

	if (value == "RowMultiple")
	{
		mode = MultiColumnList::RowMultiple;
	}
	else if (value == "ColumnSingle")
	{
		mode = MultiColumnList::ColumnSingle;
	}
	else if (value == "ColumnMultiple")
	{
		mode = MultiColumnList::ColumnMultiple;
	}
	else if (value == "CellSingle")
	{
		mode = MultiColumnList::CellSingle;
	}
	else if (value == "CellMultiple")
	{
		mode = MultiColumnList::CellMultiple;
	}
	else if (value == "NominatedColumnSingle")
	{
		mode = MultiColumnList::NominatedColumnSingle;
	}
	else if (value == "NominatedColumnMultiple")
	{
		mode = MultiColumnList::NominatedColumnMultiple;
	}
	else if (value == "NominatedRowSingle")
	{
		mode = MultiColumnList::NominatedRowSingle;
	}
	else if (value == "NominatedRowMultiple")
	{
		mode = MultiColumnList::NominatedRowMultiple;
	}
	else
	{
		mode = MultiColumnList::RowSingle;
	}

	static_cast<MultiColumnList*>(receiver)->setSelectionMode(mode);
}


String ColumnHeader::get(const PropertyReceiver* receiver) const
{
	return String("");
}


void ColumnHeader::set(PropertyReceiver* receiver, const String& value)
{
    size_t idstart = value.rfind("id:");
    size_t wstart = value.rfind("width:");
    size_t capstart = value.find("text:");

    // some defaults in case of missing data
    String caption, id("0"), width("{0.33,0}");

    // extract the caption field
    if (capstart != String::npos)
    {
        capstart = value.find_first_of(":") + 1;

        if (wstart == String::npos)
            if (idstart == String::npos)
                caption = value.substr(capstart);
            else
                caption = value.substr(capstart, idstart - capstart);
        else
            caption = value.substr(capstart, wstart - capstart);

        // trim trailing whitespace
        TextUtils::trimTrailingChars(caption, TextUtils::DefaultWhitespace);
    }

    // extract the width field
    if (wstart != String::npos)
    {
        width = value.substr(wstart);
        width = width.substr(width.find_first_of("{"));
        width = width.substr(0, width.find_last_of("}") + 1);
    }

    // extract the id field.
    if (idstart != String::npos)
    {
        id = value.substr(idstart);
        id = id.substr(id.find_first_of(":") + 1);
    }

    // add the column accordingly
    static_cast<MultiColumnList*>(receiver)->addColumn(
        caption, PropertyHelper::stringToUint(id), PropertyHelper::stringToUDim(width));
}


String RowCount::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const MultiColumnList*>(receiver)->getRowCount());
}


void RowCount::set(PropertyReceiver* receiver, const String& value)
{
	// property is read only.
	Logger::getSingleton().logEvent(
		"Attempt to set read only property 'RowCount' on MultiColumnListbox '" + 
		static_cast<const MultiColumnList*>(receiver)->getName() + "'.", Errors);
}


} // End of  MultiColumnListProperties namespace section

} // End of  CEGUI namespace section
