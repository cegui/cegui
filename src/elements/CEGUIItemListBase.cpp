/************************************************************************
	filename: 	CEGUIItemListBase.cpp
	created:	31/3/2005
	author:		Tomas Lindquist Olsen (based on original Listbox code by Paul D Turner)
	
	purpose:	Implementation of ItemListBase widget base class
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
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIItemListBase.h"
#include "elements/CEGUIItemEntry.h"

#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Definition of Properties for this class
*************************************************************************/
ItemListBaseProperties::AutoResizeEnabled	ItemListBase::d_autoResizeEnabledProperty;


/*************************************************************************
	Constants
*************************************************************************/
// event names
const String ItemListBase::EventNamespace("ItemListBase");
const String ItemListBase::EventListContentsChanged("ListItemsChanged");

	
/*************************************************************************
	Constructor for ItemListBase base class.
*************************************************************************/
ItemListBase::ItemListBase(const String& type, const String& name)
	: Window(type, name),
	d_autoResize(false)
{
	// add properties for ItemListBase class
	addItemListBaseProperties();
}


/*************************************************************************
	Destructor for ItemListBase base class.
*************************************************************************/
ItemListBase::~ItemListBase(void)
{
	resetList_impl();
}


/*************************************************************************
	Return the item at index position 'index'.
*************************************************************************/
ItemEntry* ItemListBase::getItemFromIndex(size_t index) const
{
	if (index < d_listItems.size())
	{
		return d_listItems[index];
	}
	else
	{
		throw InvalidRequestException("ItemListBase::getItemFromIndex - the specified index is out of range for this ItemListBase.");
	}
}


/*************************************************************************
	Return the index of ItemEntry \a item
*************************************************************************/
size_t ItemListBase::getItemIndex(const ItemEntry* item) const
{
	ItemEntryList::const_iterator pos = std::find(d_listItems.begin(), d_listItems.end(), item);

	if (pos != d_listItems.end())
	{
		return std::distance(d_listItems.begin(), pos);
	}
	else
	{
		throw InvalidRequestException("ItemListBase::getItemIndex - the specified ItemEntry is not attached to this ItemListBase.");
	}

}


/*************************************************************************
	Search the list for an item with the specified text
*************************************************************************/
ItemEntry* ItemListBase::findItemWithText(const String& text, const ItemEntry* start_item)
{
	// if start_item is NULL begin search at begining, else start at item after start_item
	size_t index = (!start_item) ? 0 : (getItemIndex(start_item) + 1);

	while (index < d_listItems.size())
	{
		// return pointer to this item if it's text matches
		if (d_listItems[index]->getText() == text)
		{
			return d_listItems[index];
		}
		// no matching text, advance to next item
		else
		{
			index++;
		}

	}

	// no items matched.
	return 0;
}


/*************************************************************************
	Return whether the specified ItemEntry is in the List
*************************************************************************/
bool ItemListBase::isItemInList(const ItemEntry* item) const
{
	return std::find(d_listItems.begin(), d_listItems.end(), item) != d_listItems.end();
}


/*************************************************************************
	Remove all items from the list.
*************************************************************************/
void ItemListBase::resetList(void)
{
	if (resetList_impl())
	{
		handleUpdatedItemData();
	}

}


/*************************************************************************
	Add the given ItemEntry to the list.
*************************************************************************/
void ItemListBase::addItem(ItemEntry* item)
{
	if (item)
	{
		// just stick it on the end.
		d_listItems.push_back(item);
		addChildWindow(item);
		handleUpdatedItemData();
	}

}


/*************************************************************************
	Insert an item into the list box after a specified item already in
	the list.
*************************************************************************/
void ItemListBase::insertItem(ItemEntry* item, const ItemEntry* position)
{
	if (item)
	{
		// if position is NULL begin insert at begining, else insert after item 'position'
		ItemEntryList::iterator ins_pos;

		if (!position)
		{
			ins_pos = d_listItems.begin();
		}
		else
		{
			ins_pos = std::find(d_listItems.begin(), d_listItems.end(), position);

			// throw if item 'position' is not in the list
			if (ins_pos == d_listItems.end())
			{
				throw InvalidRequestException("ItemListBase::insertItem - the specified ItemEntry for parameter 'position' is not attached to this ItemListBase.");
			}

		}
		
		d_listItems.insert(ins_pos, item);
		addChildWindow(item);

		handleUpdatedItemData();
	}

}


/*************************************************************************
	Removes the given item from the list box.
*************************************************************************/
void ItemListBase::removeItem(ItemEntry* item)
{
	if (item && isItemInList(item))
	{
	    removeChildWindow(item);
	    if (item->isDestroyedByParent())
	    {
	        WindowManager::getSingleton().destroyWindow(item);
	    }
	}
}


/*************************************************************************
	Set wheter or not this ItemListBase widget should automatically
	resize to fit its content.
*************************************************************************/
void ItemListBase::setAutoResizeEnabled(bool setting)
{
	bool old = d_autoResize;
	d_autoResize = setting;

	// if not already enabled, trigger a resize - only if not currently initialising
	if ( d_autoResize && !old && !d_initialising)
	{
		sizeToContent();
	}

}


/*************************************************************************
	Causes the list box to update it's internal state after changes have
	been made to one or more attached ItemEntry objects.	
*************************************************************************/
void ItemListBase::handleUpdatedItemData(void)
{
	WindowEventArgs args(this);
	onListContentsChanged(args);
}


/*************************************************************************
	Handler called internally when the list contents are changed	
*************************************************************************/
void ItemListBase::onListContentsChanged(WindowEventArgs& e)
{	
    // if we are not currently initialising we might have things todo
	if (!d_initialising)
	{
	    requestRedraw();
	    
	    // if auto resize is enabled - do it
	    if (d_autoResize)
	    {
		    sizeToContent();
	    }
	    // redo the item layout and fire our event
	    layoutItemWidgets();
	    fireEvent(EventListContentsChanged, e, EventNamespace);
	}
}


/************************************************************************
    Handler for when a child is removed
*************************************************************************/
void ItemListBase::onChildRemoved(WindowEventArgs& e)
{
    // make sure it is removed from the itemlist if we have an ItemEntry
    if (e.window->testClassName("ItemEntry"))
    {
        ItemEntryList::iterator pos = std::find(d_listItems.begin(), d_listItems.end(), e.window);

        // if item is in the list
        if (pos != d_listItems.end())
        {
            // remove item
            d_listItems.erase(pos);
            // trigger list update
            handleUpdatedItemData();
		}
    }
}


/*************************************************************************
	Remove all items from the list.
*************************************************************************/
bool ItemListBase::resetList_impl(void)
{
	// just return false if the list is already empty
	if (getItemCount() == 0)
	{
		return false;
	}
	// we have items to be removed and possible deleted
	else
	{
		// delete any items we are supposed to
		for (size_t i = 0; i < getItemCount(); ++i)
		{
			removeChildWindow(d_listItems[i]);
			if (d_listItems[i]->isDestroyedByParent())
			{
			    WindowManager::getSingleton().destroyWindow(d_listItems[i]);
			}
		}

		// clear out the list.
		d_listItems.clear();

		return true;
	}

}


/*************************************************************************
	Add ItemListBase specific properties
*************************************************************************/
void ItemListBase::addItemListBaseProperties(void)
{
    addProperty(&d_autoResizeEnabledProperty);
}


/*************************************************************************
	Internal version of adding a child window.
*************************************************************************/
void ItemListBase::addChild_impl(Window* wnd)
{
	Window::addChild_impl(wnd);

	// if this is an ItemEntry we add it like one. only if it is not already in the list!
	if (wnd->testClassName("ItemEntry")&&!isItemInList((ItemEntry*)wnd))
	{
		// perform normal addItem - please no more infinite recursion !#?¤
		d_listItems.push_back((ItemEntry*)wnd);
		handleUpdatedItemData();
	}

}


/************************************************************************
    Initialisation done
*************************************************************************/
void ItemListBase::endInitialisation(void)
{
    Window::endInitialisation();
    handleUpdatedItemData();
}


/************************************************************************
	Perform child window layout
************************************************************************/
void ItemListBase::performChildWindowLayout(void)
{
	Window::performChildWindowLayout();
	// if we are not currently initialising
	if (!d_initialising)
	{
	    // Redo the item layout.
	    // We don't just call handleUpdateItemData, as that could trigger a resize,
	    // which is not what is being requested.
	    // It would also cause infinite recursion... so lets just avoid that :)
	    layoutItemWidgets();
	}
}

} // End of  CEGUI namespace section
