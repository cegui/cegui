/***********************************************************************
	created:	13/4/2004
	author:		Paul D Turner

	purpose:	Implementation of MultiColumnList widget base class
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
#include "CEGUI/widgets/MultiColumnList.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/widgets/ListboxItem.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/Logger.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/text/TextUtils.h"
#include <algorithm>

namespace CEGUI
{
const String MultiColumnList::EventNamespace("MultiColumnList");
const String MultiColumnList::WidgetTypeName("CEGUI/MultiColumnList");

/*************************************************************************
    MultiColumnListWindowRenderer
*************************************************************************/
MultiColumnListWindowRenderer::MultiColumnListWindowRenderer(const String& name) :
    WindowRenderer(name, MultiColumnList::EventNamespace)
{
}
/*************************************************************************
	Constants
*************************************************************************/
// Event names
const String MultiColumnList::EventSelectionModeChanged( "SelectionModeChanged" );
const String MultiColumnList::EventNominatedSelectColumnChanged( "NominatedSelectColumnChanged" );
const String MultiColumnList::EventNominatedSelectRowChanged( "NominatedSelectRowChanged" );
const String MultiColumnList::EventVertScrollbarModeChanged( "VertScrollbarModeChanged" );
const String MultiColumnList::EventHorzScrollbarModeChanged( "HorzScrollbarModeChanged" );
const String MultiColumnList::EventSelectionChanged( "SelectionChanged" );
const String MultiColumnList::EventListContentsChanged( "ListContentsChanged" );
const String MultiColumnList::EventSortColumnChanged( "SortColumnChanged" );
const String MultiColumnList::EventSortDirectionChanged( "SortDirectionChanged" );
const String MultiColumnList::EventListColumnSized( "ListColumnSized" );
const String MultiColumnList::EventListColumnMoved( "ListColumnMoved" );

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String MultiColumnList::VertScrollbarName( "__auto_vscrollbar__" );
const String MultiColumnList::HorzScrollbarName( "__auto_hscrollbar__" );
const String MultiColumnList::ListHeaderName( "__auto_listheader__" );


/*************************************************************************
	Constructor for the Multi-column list base class
*************************************************************************/
MultiColumnList::MultiColumnList(const String& type, const String& name) :
	Window(type, name),
	d_forceVertScroll(false),
	d_forceHorzScroll(false),
	d_nominatedSelectCol(0),
	d_nominatedSelectRow(0),
	d_lastSelected(nullptr),
    d_columnCount(0),
    d_autoSizeColumnUsesHeader(false)
{
	// add properties
	addMultiColumnListProperties();

	// set default selection mode
	d_selectMode = SelectionMode::CellSingle;		// hack to ensure call below does what it should.
	setSelectionMode(SelectionMode::RowSingle);
}


/*************************************************************************
	Destructor for the multi-column list base class.
*************************************************************************/
MultiColumnList::~MultiColumnList(void)
{
	// delete any items we are supposed to
	resetList_impl();
}


/*************************************************************************
	Return whether user manipulation of the sort column and direction
	is enabled.
*************************************************************************/
bool MultiColumnList::isUserSortControlEnabled(void) const
{
	return getListHeader()->isSortingEnabled();
}


/*************************************************************************
	Return whether the user may size column segments.
*************************************************************************/
bool MultiColumnList::isUserColumnSizingEnabled(void) const
{
	return getListHeader()->isColumnSizingEnabled();
}


/*************************************************************************
	Return whether the user may modify the order of the columns.
*************************************************************************/
bool MultiColumnList::isUserColumnDraggingEnabled(void) const
{
	return getListHeader()->isColumnDraggingEnabled();
}


/*************************************************************************
	Return the number of columns in the multi-column list
*************************************************************************/
unsigned int MultiColumnList::getColumnCount(void) const
{
	return d_columnCount;
}


/*************************************************************************
	Return the number of rows in the multi-column list.
*************************************************************************/
unsigned int MultiColumnList::getRowCount(void) const
{
	return static_cast<unsigned int>(d_grid.size());
}


/*************************************************************************
	Return the zero based index of the current sort column.  There must
	be at least one column to successfully call this method.
*************************************************************************/
unsigned int MultiColumnList::getSortColumn(void) const
{
	return getListHeader()->getSortColumn();
}

unsigned int MultiColumnList::getSortColumnID(void) const
{
    if (getColumnCount() > 0)
    {
        return getColumnID(getSortColumn());
    }
    return 0;
}

/*************************************************************************
	Return the zero based column index of the column with the specified ID.
*************************************************************************/
unsigned int MultiColumnList::getColumnWithID(unsigned int col_id) const
{
	return getListHeader()->getColumnFromID(col_id);
}


/*************************************************************************
	Return the zero based index of the column whos header text matches
	the specified text.
*************************************************************************/
unsigned int MultiColumnList::getColumnWithHeaderText(const String& text) const
{
	return getListHeader()->getColumnWithText(text);
}


/*************************************************************************
	Return the total width of all column headers.
*************************************************************************/
UDim MultiColumnList::getTotalColumnHeadersWidth(void) const
{
    const ListHeader* header = getListHeader();
    UDim width(0,0);

    for (unsigned int i = 0; i < getColumnCount(); ++i)
        width += header->getColumnWidth(i);

    return width;
}


/*************************************************************************
	Return the width of the specified column header.
*************************************************************************/
UDim MultiColumnList::getColumnHeaderWidth(unsigned int col_idx) const
{
	return getListHeader()->getColumnWidth(col_idx);
}


/*************************************************************************
	Return the currently set sort direction.
*************************************************************************/
ListHeaderSegment::SortDirection MultiColumnList::getSortDirection(void) const
{
	return getListHeader()->getSortDirection();
}


/*************************************************************************
	Return the ListHeaderSegment object for the specified column
*************************************************************************/
ListHeaderSegment& MultiColumnList::getHeaderSegmentForColumn(unsigned int col_idx) const
{
	return getListHeader()->getSegmentFromColumn(col_idx);
}


/*************************************************************************
	return index of row containing the given ListboxItem
*************************************************************************/
unsigned int MultiColumnList::getItemRowIndex(const ListboxItem* item) const
{
	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		if (isListboxItemInRow(item, i))
		{
			return i;
		}

	}

	// item is not attached to the list box, throw...
	throw InvalidRequestException(
        "the given ListboxItem is not attached to this MultiColumnList.");
}


/*************************************************************************
	return the index of the column containing the given ListboxItem
*************************************************************************/
unsigned int MultiColumnList::getItemColumnIndex(const ListboxItem* item) const
{
	for (unsigned int i = 0; i < getColumnCount(); ++i)
	{
		if (isListboxItemInColumn(item, i))
		{
			return i;
		}

	}

	// item is not attached to the list box, throw...
	throw InvalidRequestException(
        "the given ListboxItem is not attached to this MultiColumnList.");
}


/*************************************************************************
	return an MCLGridRef for the given ListboxItem.
*************************************************************************/
MCLGridRef MultiColumnList::getItemGridReference(const ListboxItem* item) const
{
	return MCLGridRef(getItemRowIndex(item), getItemColumnIndex(item));
}


/*************************************************************************
	return a ListboxItem pointer for the given reference.
	May be NULL if position is blank.  Throws on out of range.
*************************************************************************/
ListboxItem* MultiColumnList::getItemAtGridReference(const MCLGridRef& grid_ref) const
{
	// check for invalid grid ref
	if (grid_ref.column >= getColumnCount())
	{
		throw InvalidRequestException(
            "the column given in the grid reference is out of range.");
	}
	
    if (grid_ref.row >= getRowCount())
	{
		throw InvalidRequestException(
            "the row given in the grid reference is out of range.");
	}
	else
	{
		return d_grid[grid_ref.row][grid_ref.column];
	}

}


/*************************************************************************
	return true if the given item is in the given column.
*************************************************************************/
bool MultiColumnList::isListboxItemInColumn(const ListboxItem* item, unsigned int col_idx) const
{
	// check for invalid index
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException(
            "the column index given is out of range.");
	}

	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		if (d_grid[i][col_idx] == item)
		{
			return true;
		}

	}

	// Item was not in the column.
	return false;
}


/*************************************************************************
	return true if the given item is in the given row.
*************************************************************************/
bool MultiColumnList::isListboxItemInRow(const ListboxItem* item, unsigned int row_idx) const
{
	// check for invalid index
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException(
            "the row index given is out of range.");
	}

	for (unsigned int i = 0; i < getColumnCount(); ++i)
	{
		if (d_grid[row_idx][i] == item)
		{
			return true;
		}

	}

	// Item was not in the row.
	return false;
}


/*************************************************************************
	return true if the given item is somewhere in the list box.
*************************************************************************/
bool MultiColumnList::isListboxItemInList(const ListboxItem* item) const
{
	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		for (unsigned int j = 0; j < getColumnCount(); ++j)
		{
			if (d_grid[i][j] == item)
			{
				return true;
			}

		}

	}

	return false;
}


/*************************************************************************
	Search the items in column 'col_idx' and return the first item after
	'start_item' that matches the specified text.  If 'start_item' is NULL
	searching start at (and includes) the first item in the column.
*************************************************************************/
ListboxItem* MultiColumnList::findColumnItemWithText(const String& text, unsigned int col_idx, const ListboxItem* start_item) const
{
	// ensure column is valid
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException(
            "specified column index is out of range.");
	}

	// find start position for search
	unsigned int i = (!start_item) ? 0 : getItemRowIndex(start_item) + 1;

	for ( ; i < getRowCount(); ++i)
	{
		// does this item match?
		if (d_grid[i][col_idx]->getText() == text)
		{
			return d_grid[i][col_idx];
		}

	}

	// no matching item.
	return nullptr;
}


/*************************************************************************
	Search the items in row 'row_idx' and return the first item after
	'start_item' that matches the specified text.  If 'start_item' is NULL
	searching start at (and includes) the first item in the row.
*************************************************************************/
ListboxItem* MultiColumnList::findRowItemWithText(const String& text, unsigned int row_idx, const ListboxItem* start_item) const
{
	// ensure row is valid
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException(
            "specified row index is out of range.");
	}

	// find start position for search
	unsigned int i = (!start_item) ? 0 : getItemColumnIndex(start_item) + 1;

	for ( ; i < getColumnCount(); ++i)
	{
		// does this item match?
		if (d_grid[row_idx][i]->getText() == text)
		{
			return d_grid[row_idx][i];
		}

	}

	// no matching item.
	return nullptr;
}


/*************************************************************************
	Search the list and return the first item after 'start_item' that
	matches the specified text.  If 'start_item' is NULL searching starts
	at (and includes) the first item in the list.

	Searching proceeds across the columns and down the rows.
*************************************************************************/
ListboxItem* MultiColumnList::findListItemWithText(const String& text, const ListboxItem* start_item) const
{
	MCLGridRef startRef(0, 0);

	// get position of start_item if it's not NULL
	if (start_item)
	{
		startRef = getItemGridReference(start_item);
		++startRef.column;
	}

	// perform the search
	for (unsigned int i = startRef.row; i < getRowCount(); ++i)
	{
		for (unsigned int j = startRef.column; j < getColumnCount(); ++j)
		{
			// does this item match?
			if (d_grid[i][j]->getText() == text)
			{
				return d_grid[i][j];
			}

		}

	}

	// No match
	return nullptr;
}


/*************************************************************************
	Scan the list (across columns for each row) and return the first
	selected item.
*************************************************************************/
ListboxItem* MultiColumnList::getFirstSelectedItem(void) const
{
	return getNextSelected(nullptr);
}


/*************************************************************************
	Search the list and return the first item after 'start_item' that
	is selected.  If 'start_item' is NULL searching starts at (and
	includes) the first item in the list.

	Searching proceeds across the columns and down the rows.
*************************************************************************/
ListboxItem* MultiColumnList::getNextSelected(const ListboxItem* start_item) const
{
	MCLGridRef startRef(0, 0);

	// get position of start_item if it's not NULL
	if (start_item)
	{
		startRef = getItemGridReference(start_item);
		if (++startRef.column == getColumnCount())
		{
		    startRef.column = 0;
		    ++startRef.row;
		}
	}

	// perform the search
	for (unsigned int i = startRef.row; i < getRowCount(); ++i)
	{
		for (unsigned int j = startRef.column; j < getColumnCount(); ++j)
		{
			// does this item match?
			ListboxItem* item = d_grid[i][j];

			if ((item != nullptr) && item->isSelected())
			{
				return d_grid[i][j];
			}

		}

	}

	// No match
	return nullptr;
}


/*************************************************************************
	Return the total number of selected items
*************************************************************************/
unsigned int MultiColumnList::getSelectedCount(void) const
{
	unsigned int count = 0;

	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		for (unsigned int j = 0; j < getColumnCount(); ++j)
		{
			ListboxItem* item = d_grid[i][j];

			if ((item != nullptr) && item->isSelected())
			{
				++count;
			}

		}

	}

	return count;
}


/*************************************************************************
	Return whether the item at the given grid position is selected.
*************************************************************************/
bool MultiColumnList::isItemSelected(const MCLGridRef& grid_ref) const
{
	ListboxItem* item = getItemAtGridReference(grid_ref);

	if (item)
	{
		return item->isSelected();
	}

	// if no item exists here, then it can't be selected.
	return false;
}


/*************************************************************************
	Return the ID of the nominated selection column
*************************************************************************/
unsigned int MultiColumnList::getNominatedSelectionColumnID(void) const
{
    if (getColumnCount() > 0)
    {
        return getListHeader()->getSegmentFromColumn(d_nominatedSelectCol).getID();
    }
    return 0;
}


/*************************************************************************
	Return the nominated selection column
*************************************************************************/
unsigned int MultiColumnList::getNominatedSelectionColumn(void) const
{
	return d_nominatedSelectCol;
}


/*************************************************************************
	Return the nominated selection row.
*************************************************************************/
unsigned int MultiColumnList::getNominatedSelectionRow(void) const
{
	return d_nominatedSelectRow;
}


/*************************************************************************
	Return the active selection mode.
*************************************************************************/
MultiColumnList::SelectionMode MultiColumnList::getSelectionMode(void) const
{
	return d_selectMode;
}


/*************************************************************************
	Initialise the Window based object ready for use.
*************************************************************************/
void MultiColumnList::initialiseComponents()
{
	// get the component sub-widgets
	Scrollbar* vertScrollbar = getVertScrollbar();
	Scrollbar* horzScrollbar = getHorzScrollbar();
	ListHeader* header       = getListHeader();

	// subscribe some events
	header->subscribeEvent(ListHeader::EventSegmentRenderOffsetChanged, Event::Subscriber(&CEGUI::MultiColumnList::handleHeaderScroll, this));
	header->subscribeEvent(ListHeader::EventSegmentSequenceChanged, Event::Subscriber(&CEGUI::MultiColumnList::handleHeaderSegMove, this));
	header->subscribeEvent(ListHeader::EventSegmentSized, Event::Subscriber(&CEGUI::MultiColumnList::handleColumnSizeChange, this));
	header->subscribeEvent(ListHeader::EventSortColumnChanged , Event::Subscriber(&CEGUI::MultiColumnList::handleSortColumnChange, this));
	header->subscribeEvent(ListHeader::EventSortDirectionChanged, Event::Subscriber(&CEGUI::MultiColumnList::handleSortDirectionChange, this));
	header->subscribeEvent(ListHeader::EventSplitterDoubleClicked, Event::Subscriber(&CEGUI::MultiColumnList::handleHeaderSegDblClick, this));
	horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&CEGUI::MultiColumnList::handleHorzScrollbar, this));
    vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&CEGUI::MultiColumnList::handleVertScrollbar, this));


	// final initialisation now widget is complete
	setSortDirection(ListHeaderSegment::SortDirection::NoSorting);

	// Perform initial layout
	configureScrollbars();
    Window::initialiseComponents();
}


/*************************************************************************
	Remove all items from the list.
*************************************************************************/
void MultiColumnList::resetList(void)
{
	if (resetList_impl())
	{
		WindowEventArgs args(this);
		onListContentsChanged(args);
	}

}


/*************************************************************************
	Add a column to the table.
*************************************************************************/
void MultiColumnList::addColumn(const String& text, unsigned int col_id, const UDim& width)
{
	insertColumn(text, col_id, width, getColumnCount());
}

/*************************************************************************
	Add a column to the table.
*************************************************************************/
void MultiColumnList::addColumn(const String& value)
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
    addColumn(caption, PropertyHelper<std::uint32_t>::fromString(id), PropertyHelper<UDim>::fromString(width));
}
/*************************************************************************
	Insert a column into the table.
*************************************************************************/
void MultiColumnList::insertColumn(const String& text, unsigned int col_id, const UDim& width, unsigned int position)
{
	// if position is out of range, add item to end of current columns.
	if (position > getColumnCount())
	{
		position = getColumnCount();
	}

	// set-up the header for the new column.
	getListHeader()->insertColumn(text, col_id, width, position);
    ++d_columnCount;

    // Set the font equal to that of our list
    ListHeaderSegment& segment = getHeaderSegmentForColumn(position);
    segment.setFont(d_font);
    // ban properties from segment that we control from here.
    segment.banPropertyFromXML("ID");
    segment.banPropertyFromXML("Text");
    segment.banPropertyFromXML("Font");

	// Insert a blank entry at the appropriate position in each row.
	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
        d_grid[i].d_items.insert(
            d_grid[i].d_items.begin() + position,
            static_cast<ListboxItem*>(nullptr));
	}

	// update stored nominated selection column if that has changed.
	if ((d_nominatedSelectCol >= position) && (getColumnCount() > 1))
	{
		d_nominatedSelectCol++;
	}

	// signal a change to the list contents
	WindowEventArgs args(this);
	onListContentsChanged(args);
}


/*************************************************************************
	Remove a column from the table.
*************************************************************************/
void MultiColumnList::removeColumn(unsigned int col_idx)
{
	// ensure index is valid, and throw if not.
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException(
            "the specified column index is out of range.");
	}
	else
	{
		// update stored column index values
		if (d_nominatedSelectCol == col_idx)
		{
			d_nominatedSelectCol = 0;
		}

		// remove the column from each row
		for (unsigned int i = 0; i < getRowCount(); ++i)
		{
			// extract the item pointer.
			ListboxItem* item = d_grid[i][col_idx];

			// remove the column entry from the row
			d_grid[i].d_items.erase(d_grid[i].d_items.begin() + col_idx);

			// delete the ListboxItem as needed.
			if ((item != nullptr) && item->isAutoDeleted())
			{
				delete item;
			}

		}

		// remove header segment
		getListHeader()->removeColumn(col_idx);
        --d_columnCount;

		// signal a change to the list contents
		WindowEventArgs args(this);
		onListContentsChanged(args);
	}

}


/*************************************************************************
	Remove a column from the table (via ID)
*************************************************************************/
void MultiColumnList::removeColumnWithID(unsigned int col_id)
{
	removeColumn(getColumnWithID(col_id));
}


/*************************************************************************
	Move a column within the table
*************************************************************************/
void MultiColumnList::moveColumn(unsigned int col_idx, unsigned int position)
{
	// move the segment on the header, events will ensure the items get moved also.
	getListHeader()->moveColumn(col_idx, position);
}


/*************************************************************************
	Move a column (with specified ID) within the table.
*************************************************************************/
void MultiColumnList::moveColumnWithID(unsigned int col_id, unsigned int position)
{
	moveColumn(getColumnWithID(col_id), position);
}


/*************************************************************************
	Add a row to the bottom of the table
*************************************************************************/
unsigned int MultiColumnList::addRow(unsigned int row_id)
{
	return addRow(nullptr, 0, row_id);
}


/*************************************************************************
	Add a row to the bottom of the table
*************************************************************************/
unsigned int MultiColumnList::addRow(ListboxItem* item, unsigned int col_id, unsigned int row_id)
{
	unsigned int col_idx;

	// Build the new row
	ListRow row;
	row.d_sortColumn = getSortColumn();
	row.d_items.resize(getColumnCount(), nullptr);
	row.d_rowID = row_id;

	if (item)
	{
		// discover which column to initially set
		col_idx = getColumnWithID(col_id);

		// establish item ownership & enter item into column
		item->setOwnerWindow(this);
		row[col_idx] = item;
	}

	unsigned int pos;

	// if sorting is enabled, insert at an appropriate position
    const ListHeaderSegment::SortDirection dir = getSortDirection();
	if (dir != ListHeaderSegment::SortDirection::NoSorting)
	{
        // calculate where the row should be inserted
        ListItemGrid::iterator ins_pos = dir == ListHeaderSegment::SortDirection::Descending ?
            std::upper_bound(d_grid.begin(), d_grid.end(), row, pred_descend) :
            std::upper_bound(d_grid.begin(), d_grid.end(), row);
        // insert item and get final inserted position.
        ListItemGrid::iterator final_pos = d_grid.insert(ins_pos, row);
		// get final inserted position as an uint.
		pos = static_cast<unsigned int>(std::distance(d_grid.begin(), final_pos));
	}
	// not sorted, just stick it on the end.
	else
	{
		pos = getRowCount();
		d_grid.push_back(row);
	}

	// signal a change to the list contents
	WindowEventArgs args(this);
	onListContentsChanged(args);

	return pos;
}


/*************************************************************************
	Insert a row into the table
*************************************************************************/
unsigned int MultiColumnList::insertRow(unsigned int row_idx, unsigned int row_id)
{
	return insertRow(nullptr, 0, row_idx, row_id);
}


/*************************************************************************
	Insert a row into the table
*************************************************************************/
unsigned int MultiColumnList::insertRow(ListboxItem* item, unsigned int col_id, unsigned int row_idx, unsigned int row_id)
{
	// if sorting is enabled, use add instead of insert
	if (getSortDirection() != ListHeaderSegment::SortDirection::NoSorting)
	{
		return addRow(item, col_id, row_id);
	}
	else
	{
		// Build the new row (empty)
		ListRow row;
		row.d_sortColumn = getSortColumn();
		row.d_items.resize(getColumnCount(), nullptr);
		row.d_rowID = row_id;

		// if row index is too big, just insert at end.
		if (row_idx > getRowCount())
		{
			row_idx = getRowCount();
		}

		d_grid.insert(d_grid.begin() + row_idx, row);

		// set the initial item in the new row
		setItem(item, col_id, row_idx);

		// signal a change to the list contents
		WindowEventArgs args(this);
		onListContentsChanged(args);

		return row_idx;
	}

}


/*************************************************************************
	Remove a row from the list.
*************************************************************************/
void MultiColumnList::removeRow(unsigned int row_idx)
{
	// ensure row exists
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException(
            "The specified row index is out of range.");
	}
	else
	{
		// delete items we are supposed to
		for (unsigned int i = 0; i < getColumnCount(); ++i)
		{
			ListboxItem* item = d_grid[row_idx][i];

			if ((item != nullptr) && item->isAutoDeleted())
			{
				delete item;
			}

		}

		// erase the row from the grid.
		d_grid.erase(d_grid.begin() + row_idx);

		// if we have erased the selection row, reset that to 0
		if (d_nominatedSelectRow == row_idx)
		{
			d_nominatedSelectRow = 0;
		}

		// signal a change to the list contents
		WindowEventArgs args(this);
		onListContentsChanged(args);
	}

}


/*************************************************************************
	Replace the item at grid-ref 'position' with 'item'.
	The old item is deleted according to the items auto-delete setting
*************************************************************************/
void MultiColumnList::setItem(ListboxItem* item, const MCLGridRef& position)
{
	// validate grid ref
	if (position.column >= getColumnCount())
	{
		throw InvalidRequestException(
            "the specified column index is invalid.");
	}
	else if (position.row >= getRowCount())
	{
		throw InvalidRequestException(
            "the specified row index is invalid.");
	}

	// delete old item as required
	ListboxItem* oldItem = d_grid[position.row][position.column];

	if ((oldItem != nullptr) && oldItem->isAutoDeleted())
	{
		delete oldItem;
	}

	// set new item.
	if (item)
		item->setOwnerWindow(this);

	d_grid[position.row][position.column] = item;


	// signal a change to the list contents
	WindowEventArgs args(this);
	onListContentsChanged(args);
}


/*************************************************************************
	Replace the item in row 'row_idx', in the column with ID 'col_id'
	with 'item'.  The old item is deleted as required.
*************************************************************************/
void MultiColumnList::setItem(ListboxItem* item, unsigned int col_id, unsigned int row_idx)
{
	setItem(item, MCLGridRef(row_idx, getColumnWithID(col_id)));
}


/*************************************************************************
	Set the current selection mode.
*************************************************************************/
void MultiColumnList::setSelectionMode(MultiColumnList::SelectionMode sel_mode)
{
	if (d_selectMode != sel_mode)
	{
		d_selectMode = sel_mode;

		clearAllSelections();

		switch(d_selectMode)
		{
		case SelectionMode::RowSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= true;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

        case SelectionMode::RowMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= true;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case SelectionMode::CellSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case SelectionMode::CellMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case SelectionMode::NominatedColumnSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= true;
			d_useNominatedRow	= false;
			break;

		case SelectionMode::NominatedColumnMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= true;
			d_useNominatedRow	= false;
			break;

		case SelectionMode::ColumnSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= true;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case SelectionMode::ColumnMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= true;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case SelectionMode::NominatedRowSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= true;
			break;

		case SelectionMode::NominatedRowMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= true;
			break;

		default:
			throw InvalidRequestException(
                "invalid or unknown SelectionMode value supplied.");
		}

		// Fire event.
		WindowEventArgs args(this);
		onSelectionModeChanged(args);
	}

}


/*************************************************************************
	Set the nominated column for 'NominatedColumn*' selections (by ID)
*************************************************************************/
void MultiColumnList::setNominatedSelectionColumnID(unsigned int col_id)
{
	setNominatedSelectionColumn(getColumnWithID(col_id));
}


/*************************************************************************
	Set the nominated column for 'NominatedColumn*' selections (by Index)
*************************************************************************/
void MultiColumnList::setNominatedSelectionColumn(unsigned int col_idx)
{
	if (d_nominatedSelectCol != col_idx)
	{
		clearAllSelections();

		d_nominatedSelectCol = col_idx;

		// Fire event.
		WindowEventArgs args(this);
		onNominatedSelectColumnChanged(args);
	}

}


/*************************************************************************
	Set the nominated row for 'NominatedRow*' selections
*************************************************************************/
void MultiColumnList::setNominatedSelectionRow(unsigned int row_idx)
{
	if (d_nominatedSelectRow != row_idx)
	{
		clearAllSelections();

		d_nominatedSelectRow = row_idx;

		// Fire event.
		WindowEventArgs args(this);
		onNominatedSelectRowChanged(args);
	}

}


/*************************************************************************
	Set the current sort direction.
*************************************************************************/
void MultiColumnList::setSortDirection(ListHeaderSegment::SortDirection direction)
{
	if (getSortDirection() != direction)
	{
		// set the sort direction on the header, events will make sure everything else is updated.
		getListHeader()->setSortDirection(direction);
	}

}


/*************************************************************************
	Set the current sort column (by index)
*************************************************************************/
void MultiColumnList::setSortColumn(unsigned int col_idx)
{
	if (getSortColumn() != col_idx)
	{
		// set the sort column on the header, events will make sure everything else is updated.
		getListHeader()->setSortColumn(col_idx);
	}

}


/*************************************************************************
	Set the current sort column (by ID)
*************************************************************************/
void MultiColumnList::setSortColumnByID(unsigned int col_id)
{
    ListHeader* header = getListHeader();

	if (header->getSegmentFromColumn(getSortColumn()).getID() != col_id)
	{
		// set the sort column on the header, events will make sure everything else is updated.
		header->setSortColumnFromID(col_id);
	}

}


/*************************************************************************
	Enable / Disable forced display of the vertical scroll bar
*************************************************************************/
void MultiColumnList::setShowVertScrollbar(bool setting)
{
	if (d_forceVertScroll != setting)
	{
		d_forceVertScroll = setting;

		configureScrollbars();

		// Event firing.
		WindowEventArgs args(this);
		onVertScrollbarModeChanged(args);
	}

}


/*************************************************************************
	Enable / Disable forced display of the horizontal scroll bar
*************************************************************************/
void MultiColumnList::setShowHorzScrollbar(bool setting)
{
	if (d_forceHorzScroll != setting)
	{
		d_forceHorzScroll = setting;

		configureScrollbars();

		// Event firing.
		WindowEventArgs args(this);
		onHorzScrollbarModeChanged(args);
	}

}


/*************************************************************************
	Clear the selected state from all currently selected items.
*************************************************************************/
void MultiColumnList::clearAllSelections(void)
{
	// only fire events and update if we actually made any changes
	if (clearAllSelections_impl())
	{
		// Fire event.
		WindowEventArgs args(this);
		onSelectionChanged(args);
	}


}


/*************************************************************************
	Set the selected state of 'item' (must be attached to the list box)
*************************************************************************/
void MultiColumnList::setItemSelectState(ListboxItem* item, bool state)
{
	setItemSelectState(getItemGridReference(item), state);
}


/*************************************************************************
	Set the selected state of the item at 'grid_ref'.
*************************************************************************/
void MultiColumnList::setItemSelectState(const MCLGridRef& grid_ref, bool state)
{
	if (setItemSelectState_impl(grid_ref, state))
	{
		// Fire event.
		WindowEventArgs args(this);
		onSelectionChanged(args);
	}

}


/*************************************************************************
	Update the list box internal state after attached items have been
	externally modified.
*************************************************************************/
void MultiColumnList::handleUpdatedItemData(void)
{
    resortList();
	configureScrollbars();
	invalidate();
}


/*************************************************************************
	Set the width of the specified column header (and therefore the
	column itself).
*************************************************************************/
void MultiColumnList::setColumnHeaderWidth(unsigned int col_idx, const UDim& width)
{
    getListHeader()->setColumnWidth(col_idx, width);
}


/*************************************************************************
	display required integrated scroll bars according to current state
	of the list box and update their values.
*************************************************************************/
void MultiColumnList::configureScrollbars(void)
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();
	float totalHeight	= getTotalRowsHeight();
	float fullWidth		= getListHeader()->getTotalSegmentsPixelExtent();

	//
	// First show or hide the scroll bars as needed (or requested)
	//
	// show or hide vertical scroll bar as required (or as specified by option)
	if ((totalHeight > getListRenderArea().getHeight()) || d_forceVertScroll)
	{
		vertScrollbar->show();

		// show or hide horizontal scroll bar as required (or as specified by option)
		if ((fullWidth > getListRenderArea().getWidth()) || d_forceHorzScroll)
		{
			horzScrollbar->show();
		}
		else
		{
			horzScrollbar->hide();
		}

	}
	else
	{
		// show or hide horizontal scroll bar as required (or as specified by option)
		if ((fullWidth > getListRenderArea().getWidth()) || d_forceHorzScroll)
		{
			horzScrollbar->show();

			// show or hide vertical scroll bar as required (or as specified by option)
			if ((totalHeight > getListRenderArea().getHeight()) || d_forceVertScroll)
			{
				vertScrollbar->show();
			}
			else
			{
				vertScrollbar->hide();
			}

		}
		else
		{
			vertScrollbar->hide();
			horzScrollbar->hide();
		}

	}

	//
	// Set up scroll bar values
	//
	Rectf renderArea(getListRenderArea());

	vertScrollbar->setDocumentSize(totalHeight);
	vertScrollbar->setPageSize(renderArea.getHeight());
	vertScrollbar->setStepSize(std::max(1.0f, renderArea.getHeight() / 10.0f));
	vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition());

	horzScrollbar->setDocumentSize(fullWidth);
	horzScrollbar->setPageSize(renderArea.getWidth());
	horzScrollbar->setStepSize(std::max(1.0f, renderArea.getWidth() / 10.0f));
	horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition());
}


/*************************************************************************
	select all strings between positions 'start' and 'end'.  (inclusive)
*************************************************************************/
bool MultiColumnList::selectRange(const MCLGridRef& start, const MCLGridRef& end)
{
	MCLGridRef tmpStart(start);
	MCLGridRef tmpEnd(end);

	// ensure start is before end
	if (tmpStart.column > tmpEnd.column)
	{
		tmpStart.column = tmpEnd.column;
		tmpEnd.column = start.column;
	}

	if (tmpStart.row > tmpEnd.row)
	{
		tmpStart.row = tmpEnd.row;
		tmpEnd.row = start.row;
	}

	bool modified = false;

	// loop through all items selecting them.
	for (unsigned int i = tmpStart.row; i <= tmpEnd.row; ++i)
	{
		for (unsigned int j = tmpStart.column; j <= tmpEnd.column; ++j)
		{
			ListboxItem* item = d_grid[i][j];

			if (item)
			{
				modified |= setItemSelectState_impl(getItemGridReference(item), true);
			}

		}

	}

	return modified;
}


/*************************************************************************
	Return the sum of all row heights
*************************************************************************/
float MultiColumnList::getTotalRowsHeight(void) const
{
	float height = 0.0f;

	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		height += getHighestRowItemHeight(i);
	}

	return height;
}


/*************************************************************************
	Return the width of the widest item in the given column
*************************************************************************/
float MultiColumnList::getWidestColumnItemWidth(unsigned int col_idx) const
{
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException(
            "specified column is out of range.");
	}
	else
	{
		float width = 0.0f;

		// check each item in the column
		for (unsigned int i = 0; i < getRowCount(); ++i)
		{
			ListboxItem* item = d_grid[i][col_idx];

			// if the slot has an item in it
			if (item)
			{
				Sizef sz(item->getPixelSize());

				// see if this item is wider than the previous widest
				if (sz.d_width > width)
				{
					// update current widest
					width = sz.d_width;
				}

			}

		}

		// return the widest item.
		return width;
	}

}


/*************************************************************************
	Return the height of the highest item in the given row.
*************************************************************************/
float MultiColumnList::getHighestRowItemHeight(unsigned int row_idx) const
{
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException(
            "specified row is out of range.");
	}
	else
	{
		float height = 0.0f;

		// check each item in the column
		for (unsigned int i = 0; i < getColumnCount(); ++i)
		{
			ListboxItem* item = d_grid[row_idx][i];

			// if the slot has an item in it
			if (item)
			{
				Sizef sz(item->getPixelSize());

				// see if this item is higher than the previous highest
				if (sz.d_height > height)
				{
					// update current highest
					height = sz.d_height;
				}

			}

		}

		// return the hightest item.
		return height;
	}

}


/*************************************************************************
	Clear the selected state for all items (implementation)
*************************************************************************/
bool MultiColumnList::clearAllSelections_impl(void)
{
	// flag used so we can track if we did anything.
	bool modified = false;

	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		for (unsigned int j = 0; j < getColumnCount(); ++j)
		{
			ListboxItem* item = d_grid[i][j];

			// if slot has an item, and item is selected
			if (item != nullptr && item->isSelected())
			{
				// clear selection state and set modified flag
				item->setSelected(false);
				modified = true;
			}

		}

	}

	// signal whether or not we did anything.
	return modified;
}


/*************************************************************************
	Return the ListboxItem under the given window local pixel co-ordinate.
*************************************************************************/
ListboxItem* MultiColumnList::getItemAtPoint(const glm::vec2& localPos) const
{
    const ListHeader* header = getListHeader();
    const Rectf listArea(getListRenderArea());

    float y = listArea.d_min.y - getVertScrollbar()->getScrollPosition();
    float x = listArea.d_min.x - getHorzScrollbar()->getScrollPosition();
    
    if(y > localPos.y)
        return nullptr;

    for (unsigned int i = 0; i < getRowCount(); ++i)
    {
        y += getHighestRowItemHeight(i);

        // have we located the row?
        if (localPos.y < y)
        {
            // scan across to find column that was clicked
            for (unsigned int j = 0; j < getColumnCount(); ++j)
            {
                const ListHeaderSegment& seg = header->getSegmentFromColumn(j);
                x += CoordConverter::asAbsolute(seg.getWidth(), header->getPixelSize().d_width);

                // was this the column?
                if (localPos.x < x)
                {
                    // return contents of grid element that was clicked.
                    return d_grid[i][j];
                }
            }
        }
    }

    return nullptr;
}


/*************************************************************************
	Set select state for the given item.  This appropriately selects other
	items depending upon the select mode.  Returns true if something is
	changed, else false.
*************************************************************************/
bool MultiColumnList::setItemSelectState_impl(const MCLGridRef grid_ref, bool state)
{
	// validate grid ref
	if (grid_ref.column >= getColumnCount())
	{
		throw InvalidRequestException(
            "the specified column index is invalid.");
	}
	else if (grid_ref.row >= getRowCount())
	{
		throw InvalidRequestException(
            "the specified row index is invalid.");
	}

	// only do this if the setting is changing
	if (d_grid[grid_ref.row][grid_ref.column]->isSelected() != state)
	{
		// if using nominated selection row and/ or column, check that they match.
		if ((!d_useNominatedCol || (d_nominatedSelectCol == grid_ref.column)) &&
			(!d_useNominatedRow || (d_nominatedSelectRow == grid_ref.row)))
		{
			// clear current selection if not multi-select box
			if (state && (!d_multiSelect))
			{
				clearAllSelections_impl();
			}

			// full row?
			if (d_fullRowSelect)
			{
				// clear selection on all items in the row
				setSelectForItemsInRow(grid_ref.row, state);
			}
			// full column?
			else if (d_fullColSelect)
			{
				// clear selection on all items in the column
				setSelectForItemsInColumn(grid_ref.column, state);

			}
			// single item to be affected
			else
			{
				d_grid[grid_ref.row][grid_ref.column]->setSelected(state);
			}

			return true;
		}

	}

	return false;
}


/*************************************************************************
	Select all items in the given row
*************************************************************************/
void MultiColumnList::setSelectForItemsInRow(unsigned int row_idx, bool state)
{
	for (unsigned int i = 0; i < getColumnCount(); ++i)
	{
		ListboxItem* item = d_grid[row_idx][i];

		if (item)
		{
			item->setSelected(state);
		}

	}

}


/*************************************************************************
	Select all items in the given column
*************************************************************************/
void MultiColumnList::setSelectForItemsInColumn(unsigned int col_idx, bool state)
{
	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		ListboxItem* item = d_grid[i][col_idx];

		if (item)
		{
			item->setSelected(state);
		}

	}

}

/*************************************************************************
	Move the column at index 'col_idx' so it is at index 'position'.

	Implementation version which does not move the header segment
	(since that may have already happned).
*************************************************************************/
void MultiColumnList::moveColumn_impl(unsigned int col_idx, unsigned int position)
{
	// ensure index is valid, and throw if not.
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException(
            "the specified source column index is out of range.");
	}
	else
	{
		// if position is too big, insert at end.
		if (position > getColumnCount())
		{
			position = getColumnCount();
		}

		// update select column index value if needed
		if (d_nominatedSelectCol == col_idx)
		{
			d_nominatedSelectCol = position;
		}
		else if ((col_idx < d_nominatedSelectCol) && (position >= d_nominatedSelectCol))
		{
			d_nominatedSelectCol--;
		}
		else if ((col_idx > d_nominatedSelectCol) && (position <= d_nominatedSelectCol))
		{
			d_nominatedSelectCol++;
		}

		// move column entry in each row.
		for (unsigned int i = 0; i < getRowCount(); ++i)
		{
			// store entry.
			ListboxItem* item = d_grid[i][col_idx];

			// remove the original column for this row.
			d_grid[i].d_items.erase(d_grid[i].d_items.begin() + col_idx);

			// insert entry at its new position
			d_grid[i].d_items.insert(d_grid[i].d_items.begin() + position, item);
		}

	}

}


/*************************************************************************
	Handler called when selection mode changes
*************************************************************************/
void MultiColumnList::onSelectionModeChanged(WindowEventArgs& e)
{
	fireEvent(EventSelectionModeChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the nominated selection column changes
*************************************************************************/
void MultiColumnList::onNominatedSelectColumnChanged(WindowEventArgs& e)
{
	fireEvent(EventNominatedSelectColumnChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when nominated selection row changes
*************************************************************************/
void MultiColumnList::onNominatedSelectRowChanged(WindowEventArgs& e)
{
	fireEvent(EventNominatedSelectRowChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when vertcial scroll bar 'force' mode changes
*************************************************************************/
void MultiColumnList::onVertScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when horizontal scroll bar 'force' mode changes
*************************************************************************/
void MultiColumnList::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the current selection in the list changes.
*************************************************************************/
void MultiColumnList::onSelectionChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventSelectionChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when list contents changes
*************************************************************************/
void MultiColumnList::onListContentsChanged(WindowEventArgs& e)
{
	configureScrollbars();
	invalidate();
	fireEvent(EventListContentsChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the sort column changes
*************************************************************************/
void MultiColumnList::onSortColumnChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventSortColumnChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the sort direction changes
*************************************************************************/
void MultiColumnList::onSortDirectionChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventSortDirectionChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when a column is sized
*************************************************************************/
void MultiColumnList::onListColumnSized(WindowEventArgs& e)
{
	configureScrollbars();
	invalidate();
	fireEvent(EventListColumnSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void MultiColumnList::onListColumnMoved(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventListColumnMoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool MultiColumnList::handleFontRenderSizeChange(const Font& font)
{
    bool handled = false;

    for (unsigned int i = 0; i < getRowCount(); ++i)
        for (unsigned int j = 0; j < getColumnCount(); ++j)
            if (auto item = d_grid[i][j])
                handled |= item->handleFontRenderSizeChange(&font);

    for (unsigned int col = 0; col < getColumnCount(); ++col)
    {
        if (getHeaderSegmentForColumn(col).getFont() == &font)
        {
            invalidate();
            break;
        }
    }

    // Always call just in case
    handled |= Window::handleFontRenderSizeChange(font);

    return handled;
}

//----------------------------------------------------------------------------//
void MultiColumnList::onFontChanged(WindowEventArgs& e)
{
    // Propagate to children
    // Set the font equal to that of our list
    for (unsigned int col = 0; col < getColumnCount(); ++col)
        getHeaderSegmentForColumn(col).setFont(d_font);

    // Call base class handler
    Window::onFontChanged(e);
}

//----------------------------------------------------------------------------//
void MultiColumnList::onSized(ElementEventArgs& e)
{
	Window::onSized(e);
	configureScrollbars();
	++e.handled;
}

//----------------------------------------------------------------------------//
void MultiColumnList::onMouseButtonDown(MouseButtonEventArgs& e)
{
    Window::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left)
    {
        handleSelection(e.d_localPos, e.d_modifiers.hasCtrl(), e.d_modifiers.hasShift());
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void MultiColumnList::onScroll(ScrollEventArgs& e)
{
	Window::onScroll(e);

    if (Scrollbar::standardProcessing(getVertScrollbar(), getHorzScrollbar(), -e.d_delta, e.d_modifiers.hasAlt()))
        ++e.handled;
}

//----------------------------------------------------------------------------//
void MultiColumnList::handleSelection(const glm::vec2& localPos, bool cumulative, bool range)
{
    bool modified = false;

    ListboxItem* item = getItemAtPoint(localPos);

    if (item)
    {
        // TODO: wtf this is overwritten right after
        // clear old selections if not a cumulative selection or if multi-select is off
        if (!cumulative || !d_multiSelect)
        {
            modified = clearAllSelections_impl();
        }

        modified = true;

        // select range or item, depending upon state and last selected item
        if (range && (d_lastSelected != nullptr) && d_multiSelect)
        {
            modified |= selectRange(getItemGridReference(item), getItemGridReference(d_lastSelected));
        }
        else
        {
            modified |= setItemSelectState_impl(getItemGridReference(item), item->isSelected() ^ true);
        }

        // update last selected item
        d_lastSelected = item->isSelected() ? item : 0;
    }

    // fire event if needed
    if (modified)
    {
        WindowEventArgs args(this);
        onSelectionChanged(args);
    }
}

/*************************************************************************
	Event handler for header offset changes (scrolling)
*************************************************************************/
bool MultiColumnList::handleHeaderScroll(const EventArgs&)
{
	// grab the header scroll value, convert to pixels, and set the scroll bar to match.
	getHorzScrollbar()->setScrollPosition(getListHeader()->getSegmentOffset());

	return true;
}


/*************************************************************************
	Event handler for drag & drop of header segments
*************************************************************************/
bool MultiColumnList::handleHeaderSegMove(const EventArgs& e)
{
    const HeaderSequenceEventArgs& headerSeqEventArgs = static_cast<const HeaderSequenceEventArgs&>(e);
	moveColumn_impl(headerSeqEventArgs.d_oldIdx, headerSeqEventArgs.d_newIdx);

	// signal change to our clients
	WindowEventArgs args(this);
	onListColumnMoved(args);

	return true;
}



/*************************************************************************
	Event handler for when header segment size (column width) changes
*************************************************************************/
bool MultiColumnList::handleColumnSizeChange(const EventArgs&)
{
	configureScrollbars();

	// signal change to our clients
	WindowEventArgs args(this);
	onListColumnSized(args);

	return true;
}


/*************************************************************************
	Event handler for when horizontal scroll bar is moved.
*************************************************************************/
bool MultiColumnList::handleHorzScrollbar(const EventArgs&)
{
	// set header offset to match scroll position
	getListHeader()->setSegmentOffset(getHorzScrollbar()->getScrollPosition());
    invalidate();
	return true;
}

/*************************************************************************
	Event handler for when vertical scroll bar is moved.
*************************************************************************/
bool MultiColumnList::handleVertScrollbar(const EventArgs&)
{
    invalidate();
	return true;
}


/*************************************************************************
	Handler for when sort column in header is changed
*************************************************************************/
bool MultiColumnList::handleSortColumnChange(const EventArgs&)
{
	unsigned int col = getSortColumn();

	// set new sort column on all rows
	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		d_grid[i].d_sortColumn = col;
	}

    resortList();

	// signal change to our clients
	WindowEventArgs args(this);
	onSortColumnChanged(args);

	return true;
}


/*************************************************************************
	Handler for when sort direction in header is changed
*************************************************************************/
bool MultiColumnList::handleSortDirectionChange(const EventArgs&)
{
    resortList();
	// signal change to our clients
	WindowEventArgs args(this);
	onSortDirectionChanged(args);

	return true;
}


/*************************************************************************
	Handler for when user double-clicks on header segment splitter
*************************************************************************/
bool MultiColumnList::handleHeaderSegDblClick(const EventArgs& e)
{
    const WindowEventArgs& winEventArgs = static_cast<const WindowEventArgs&>(e);

	// get the column index for the segment that was double-clicked
	unsigned int col = getListHeader()->getColumnFromSegment(
        static_cast<ListHeaderSegment&>(*winEventArgs.window));

	autoSizeColumnHeader(col);

	return true;
}


/*************************************************************************
	Set whether user manipulation of the sort column and direction are
	enabled.
*************************************************************************/
void MultiColumnList::setUserSortControlEnabled(bool setting)
{
	getListHeader()->setSortingEnabled(setting);
}


/*************************************************************************
	Set whether the user may size column segments.
*************************************************************************/
void MultiColumnList::setUserColumnSizingEnabled(bool setting)
{
	getListHeader()->setColumnSizingEnabled(setting);
}


/*************************************************************************
	Set whether the user may modify the order of the columns.
*************************************************************************/
void MultiColumnList::setUserColumnDraggingEnabled(bool setting)
{
	getListHeader()->setColumnDraggingEnabled(setting);
}


/*************************************************************************
	Return the ID code assigned to the requested column.
*************************************************************************/
unsigned int MultiColumnList::getColumnID(unsigned int col_idx) const
{
	return getListHeader()->getSegmentFromColumn(col_idx).getID();
}


/*************************************************************************
	Return the ID code assigned to the requested row.
*************************************************************************/
unsigned int MultiColumnList::getRowID(unsigned int row_idx) const
{
	// check for invalid index
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException(
            "the row index given is out of range.");
	}
	else
	{
		return d_grid[row_idx].d_rowID;
	}
}


/*************************************************************************
	Return the zero based row index of the row with the specified ID.
*************************************************************************/
unsigned int MultiColumnList::getRowWithID(unsigned int row_id) const
{
	for (unsigned int i = 0; i < getRowCount(); ++i)
	{
		if (d_grid[i].d_rowID == row_id)
		{
			return i;
		}
	}

	// No such row found, throw exception
	throw InvalidRequestException(
        "no row with the requested ID is present.");
}


/*************************************************************************
	std algorithm predicate used for sorting in descending order (static)
*************************************************************************/
bool MultiColumnList::pred_descend(const ListRow& a, const ListRow& b)
{
	return a > b;
}


/*************************************************************************
	Return whether the vertical scroll bar is always shown.
*************************************************************************/
bool MultiColumnList::isVertScrollbarAlwaysShown(void) const
{
	return d_forceVertScroll;
}


/*************************************************************************
	Return whether the horizontal scroll bar is always shown.
*************************************************************************/
bool MultiColumnList::isHorzScrollbarAlwaysShown(void) const
{
	return d_forceHorzScroll;
}


/*************************************************************************
	Adds properties for MCL
*************************************************************************/
void MultiColumnList::addMultiColumnListProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(MultiColumnList, bool,
        "ColumnsSizable", "Property to get/set the setting for user sizing of the column headers.  Value is either \"true\" or \"false\".",
        &MultiColumnList::setUserColumnSizingEnabled, &MultiColumnList::isUserColumnSizingEnabled, true /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, bool,
        "ColumnsMovable", "Property to get/set the setting for user moving of the column headers.  Value is either \"true\" or \"false\".",
        &MultiColumnList::setUserColumnDraggingEnabled, &MultiColumnList::isUserColumnDraggingEnabled, true /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, bool,
        "SortSettingEnabled", "Property to get/set the setting for for user modification of the sort column & direction."
        "  Value is either \"true\" or \"false\".",
        &MultiColumnList::setUserSortControlEnabled, &MultiColumnList::isUserSortControlEnabled, true /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, ListHeaderSegment::SortDirection,
        "SortDirection", "Property to get/set the sort direction setting of the list."
        "  Value is the text of one of the SortDirection enumerated value names.",
        &MultiColumnList::setSortDirection, &MultiColumnList::getSortDirection, ListHeaderSegment::SortDirection::NoSorting
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, bool,
        "ForceVertScrollbar", "Property to get/set the 'always show' setting for the vertical scroll bar of the list box."
        "  Value is either \"true\" or \"false\".",
        &MultiColumnList::setShowVertScrollbar, &MultiColumnList::isVertScrollbarAlwaysShown, false /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, bool,
        "ForceHorzScrollbar", "Property to get/set the 'always show' setting for the horizontal scroll bar of the list box."
        "  Value is either \"true\" or \"false\".",
        &MultiColumnList::setShowHorzScrollbar, &MultiColumnList::isHorzScrollbarAlwaysShown, false /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, unsigned int,
        "NominatedSelectionColumnID", "Property to get/set the nominated selection column (via ID).  Value is an unsigned integer number.",
        &MultiColumnList::setNominatedSelectionColumn, &MultiColumnList::getNominatedSelectionColumnID, 0 /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, unsigned int,
        "NominatedSelectionRow", "Property to get/set the nominated selection row.  Value is an unsigned integer number.",
        &MultiColumnList::setNominatedSelectionRow, &MultiColumnList::getNominatedSelectionRow, 0
    );

    CEGUI_DEFINE_PROPERTY_NO_XML(MultiColumnList, unsigned int,
        "RowCount", "Property to access the number of rows in the list (read only)",
        nullptr, &MultiColumnList::getRowCount, 0
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, MultiColumnList::SelectionMode,
        "SelectionMode", "Property to get/set the selection mode setting of the list."
        "  Value is the text of one of the SelectionMode enumerated value names.",
        &MultiColumnList::setSelectionMode, &MultiColumnList::getSelectionMode, MultiColumnList::SelectionMode::RowSingle
    );

    CEGUI_DEFINE_PROPERTY(MultiColumnList, bool,
        "AutoSizeColumnUsesHeader", "Property to get/set the 'use header size' flag when auto-sizing a column."
        " Value is either \"true\" or \"false\".",
        &MultiColumnList::setAutoSizeColumnUsesHeader, &MultiColumnList::getAutoSizeColumnUsesHeader, false
    );

    CEGUI_DEFINE_PROPERTY_NO_XML(MultiColumnList, String,
        "ColumnHeader", "Property to set up a column (there is no getter for this property)",
        &MultiColumnList::addColumn, 0, "" /* TODO: This is quite a hack, isn't it? */
    );
}


/*************************************************************************
	Remove all items from the list.
*************************************************************************/
bool MultiColumnList::resetList_impl(void)
{
	// just return false if the list is already empty (no rows == empty)
	if (getRowCount() == 0)
	{
		return false;
	}
	// we have items to be removed and possible deleted
	else
	{
		for (unsigned int i = 0; i < getRowCount(); ++i)
		{
			for (unsigned int j = 0; j < getColumnCount(); ++j)
			{
				ListboxItem* item = d_grid[i][j];

				// delete item as needed.
				if ((item != nullptr) && item->isAutoDeleted())
				{
					delete item;
				}

			}

		}

		// clear all items from the grid.
		d_grid.clear();

		// reset other affected fields
		d_nominatedSelectRow = 0;
		d_lastSelected = nullptr;

		return true;
	}

}


/*************************************************************************
	Automatically determines the "best fit" size for the specified column
	and sets the column width to the same.
*************************************************************************/
void MultiColumnList::autoSizeColumnHeader(unsigned int col_idx)
{
	// check for invalid index
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException(
            "the column index given is out of range.");
	}
	else
	{
		// get the width of the widest item in the column.
		float width = std::max(getWidestColumnItemWidth(col_idx), ListHeader::MinimumSegmentPixelWidth);

		// set new column width
		setColumnHeaderWidth(col_idx, cegui_absdim(width));
	}

}


/*************************************************************************
	Set the ID code assigned to a given row.
*************************************************************************/
void MultiColumnList::setRowID(unsigned int row_idx, unsigned int row_id)
{
	// check for invalid index
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException(
            "the row index given is out of range.");
	}
	else
	{
		d_grid[row_idx].d_rowID = row_id;
	}
}


/*************************************************************************
    Return a pointer to the vertical scrollbar component widget for this
    MultiColumnList.
*************************************************************************/
Scrollbar* MultiColumnList::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(VertScrollbarName));
}


/*************************************************************************
    Return a pointer to the horizontal scrollbar component widget for this
    MultiColumnList.
*************************************************************************/
Scrollbar* MultiColumnList::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(HorzScrollbarName));
}


/*************************************************************************
    Return a pointer to the list header component widget for this
    MultiColumnList.
*************************************************************************/
ListHeader* MultiColumnList::getListHeader() const
{
    return static_cast<ListHeader*>(getChild(ListHeaderName));
}

bool MultiColumnList::validateWindowRenderer(const WindowRenderer* renderer) const
{
	return dynamic_cast<const MultiColumnListWindowRenderer*>(renderer) != nullptr;
}

/*************************************************************************
    Write xml properties for this MultiColumnList to a stream.
*************************************************************************/
int MultiColumnList::writePropertiesXML(XMLSerializer& xml_stream) const
{
    // basically this is here to translate the columns in the list into
    // instances of the <ColumnHeader> element.  Because the SortColumnID
    // property requires the column to exist, we also write that out manually.

    // Dump all other properties first
    int propCnt = Window::writePropertiesXML(xml_stream);

    // create an dump <ColumnHeader> elements
    unsigned int columnCount = getColumnCount();
    for (unsigned int i = 0; i < columnCount; ++i)
    {
        ListHeaderSegment& seg = getHeaderSegmentForColumn(i);

        // column text
        String propString = "text:";
        propString += seg.getText();
        // column width
        propString += " width:";
        propString += PropertyHelper<UDim>::toString(seg.getWidth());
        // column id
        propString += " id:";
        propString += PropertyHelper<std::uint32_t>::toString(seg.getID());
        // create the tag
        xml_stream.openTag(Property::XMLElementName)
            .attribute(Property::NameXMLAttributeName, "ColumnHeader")
            .attribute(Property::ValueXMLAttributeName, propString)
            .closeTag();
        ++propCnt;
    }

    // write out SortColumnID property, if any(!)
    if(columnCount != 0)
    {
		try
		{
			std::uint32_t sortColumnID = getColumnWithID(getSortColumn());
			if (sortColumnID != 0)
			{
                xml_stream.openTag(Property::XMLElementName)
                    .attribute(Property::NameXMLAttributeName, "SortColumnID")
                    .attribute(Property::ValueXMLAttributeName,
                               PropertyHelper<std::uint32_t>::toString(sortColumnID))
                    .closeTag();
			    ++propCnt;
			}
		}
		catch (InvalidRequestException&)
		{
			// This catches error(s) from the MultiLineColumnList for example
			Logger::getSingleton().logEvent("MultiColumnList::writePropertiesXML - invalid sort column requested. Continuing...", LoggingLevel::Error);
		}
    }
	
    return propCnt;
}


/*************************************************************************
    Return a Rect object describing, in un-clipped pixels, the window
    relative area that is to be used for rendering list items.
*************************************************************************/
Rectf MultiColumnList::getListRenderArea() const
{
    if (d_windowRenderer != nullptr)
    {
        MultiColumnListWindowRenderer* wr = static_cast<MultiColumnListWindowRenderer*>(d_windowRenderer);
        return wr->getListRenderArea();
    }
    else
    {
        //return getListRenderArea_impl();
        throw InvalidRequestException(
            "This function must be implemented by the window renderer module");
    }
}

//----------------------------------------------------------------------------//
void MultiColumnList::ensureItemIsVisible(const ListboxItem* item)
{
    // NB: throws InvalidRequestException if non-existant item
    ensureItemIsVisible(getItemGridReference(item));
}

//----------------------------------------------------------------------------//
void MultiColumnList::ensureItemIsVisible(const MCLGridRef& grid_ref)
{
    ensureRowIsVisible(grid_ref.row);
    ensureColumnIsVisible(grid_ref.column);
}

//----------------------------------------------------------------------------//
void MultiColumnList::ensureItemRowIsVisible(const ListboxItem* item)
{
    // NB: throws InvalidRequestException if non-existant item
    ensureRowIsVisible(getItemGridReference(item).row);
}

//----------------------------------------------------------------------------//
void MultiColumnList::ensureItemColumnIsVisible(const ListboxItem* item)
{
    // NB: throws InvalidRequestException if non-existant item
    ensureColumnIsVisible(getItemGridReference(item).column);
}

//----------------------------------------------------------------------------//
void MultiColumnList::ensureRowIsVisible(unsigned int row_idx)
{
    unsigned int rows = getRowCount();

    Scrollbar* vertScrollbar = getVertScrollbar();

    // handle horizontal scrolling
    // handle simple "scroll to the bottom" case
    if (row_idx >= rows)
    {
        vertScrollbar->setScrollPosition(
            vertScrollbar->getDocumentSize() - vertScrollbar->getPageSize());
    }
    else
    {
        float bottom;
        float top = 0.0f;
        float listHeight = getListRenderArea().getHeight();

        // get distance to top of item
        unsigned int row;
        for (row = 0; row < row_idx; ++row)
            top += getHighestRowItemHeight(row);

        // calculate distance to bottom of item
        bottom = top + getHighestRowItemHeight(row);

        // account for current scrollbar value
        float currPos = vertScrollbar->getScrollPosition();
        top      -= currPos;
        bottom   -= currPos;

        // if top edge is above the view area, or if the item is too big to fit
        if ((top < 0.0f) || ((bottom - top) > listHeight))
        {
            // scroll top of item to top of box.
            vertScrollbar->setScrollPosition(currPos + top);
        }
        // if the bottom edge is below the view area
        else if (bottom >= listHeight)
        {
            // position bottom of item at the bottom of the list
            vertScrollbar->setScrollPosition(currPos + bottom - listHeight);
        }
    }
}

//----------------------------------------------------------------------------//
void MultiColumnList::ensureColumnIsVisible(unsigned int col_idx)
{
    unsigned int cols = getColumnCount();
    Scrollbar* horzScrollbar = getHorzScrollbar();

    // handle horizontal scrolling
    // first the simple "scroll to the right edge" case
    if (col_idx >= cols)
    {
        horzScrollbar->setScrollPosition(
            horzScrollbar->getDocumentSize() - horzScrollbar->getPageSize());
    }
    else
    {
        float right;
        float left = 0.0f;
        float listWidth = getListRenderArea().getWidth();

        // get distance to left edge of item
        unsigned int col;
        for (col = 0; col < col_idx; ++col)
            left += CoordConverter::asAbsolute(getColumnHeaderWidth(col),
                                               getParentPixelSize().d_width);

        // get the distance to the right edge of the item
        right = left + CoordConverter::asAbsolute(getColumnHeaderWidth(col),
                                                  getParentPixelSize().d_width);

        // account for current scrollbar value
        float currPos = horzScrollbar->getScrollPosition();
        left    -= currPos;
        right   -= currPos;

        // if the left edge is to the left of the view area, or if the item is
        // too big to fit
        if ((left < 0.0f) || ((right - left) > listWidth))
        {
            // scroll left edge of item to left edge of box.
            horzScrollbar->setScrollPosition(currPos + left);
        }
        // if right edge is to the right of the view area
        else if (right >= listWidth)
        {
            // position the right edge of the item at the right edge of the list
            horzScrollbar->setScrollPosition(currPos + right - listWidth);
        }
    }
}

//----------------------------------------------------------------------------//
void MultiColumnList::setAutoSizeColumnUsesHeader(bool include_header)
{
    d_autoSizeColumnUsesHeader = include_header;
}

//----------------------------------------------------------------------------//
bool MultiColumnList::getAutoSizeColumnUsesHeader() const
{
    return d_autoSizeColumnUsesHeader;
}

//----------------------------------------------------------------------------//

//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Operators for ListRow
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Less-than operator
*************************************************************************/
bool MultiColumnList::ListRow::operator<(const ListRow& rhs) const
{
	ListboxItem* a = d_items[d_sortColumn];
	ListboxItem* b = rhs.d_items[d_sortColumn];

	// handle cases with empty slots
	if (!b)
	{
		return false;
	}
	else if (!a)
	{
		return true;
	}
	else
	{
		return *a < *b;
	}

}


/*************************************************************************
	Greater-than operator
*************************************************************************/
bool MultiColumnList::ListRow::operator>(const ListRow& rhs) const
{
	ListboxItem* a = d_items[d_sortColumn];
	ListboxItem* b = rhs.d_items[d_sortColumn];

	// handle cases with empty slots
	if (!a)
	{
		return false;
	}
	else if (!b)
	{
		return true;
	}
	else
	{
		return *a > *b;
	}

}

/*************************************************************************
    Cause list content to be (re)sorted
*************************************************************************/
void MultiColumnList::resortList()
{
    // re-sort list according to direction
    ListHeaderSegment::SortDirection dir = getSortDirection();

    if (dir == ListHeaderSegment::SortDirection::Descending)
    {
        std::sort(d_grid.begin(), d_grid.end(), pred_descend);
    }
    else if (dir == ListHeaderSegment::SortDirection::Ascending)
    {
        std::sort(d_grid.begin(), d_grid.end());
    }
    // else no (or invalid) direction, so do not sort.
}

//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Operators for MCLGridRef
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	return true if this is less than (appears before) 'rhs' in grid.
*************************************************************************/
bool MCLGridRef::operator<(const MCLGridRef& rhs) const
{
	if ((row < rhs.row) ||
		((row == rhs.row) && (column < rhs.column)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*************************************************************************
	return true if this is less than or equal to 'rhs'
*************************************************************************/
bool MCLGridRef::operator<=(const MCLGridRef& rhs) const
{
	return !operator>(rhs);
}


/*************************************************************************
	return true if this is greater than (appears after) 'rhs' in grid.
*************************************************************************/
bool MCLGridRef::operator>(const MCLGridRef& rhs) const
{
	return (operator<(rhs) || operator==(rhs)) ? false : true;
}


/*************************************************************************
	return true if this is greater than or equal to 'rhs'
*************************************************************************/
bool MCLGridRef::operator>=(const MCLGridRef& rhs) const
{
	return !operator<(rhs);
}


/*************************************************************************
	return true if this grid ref is equal to that in 'rhs'
*************************************************************************/
bool MCLGridRef::operator==(const MCLGridRef& rhs) const
{
	return ((column == rhs.column) && (row == rhs.row)) ? true : false;
}


/*************************************************************************
	return true if this grid ref is different to that in 'rhs'
*************************************************************************/
bool MCLGridRef::operator!=(const MCLGridRef& rhs) const
{
	return !operator==(rhs);
}

} // End of  CEGUI namespace section
