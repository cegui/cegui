/************************************************************************
	filename: 	CEGUIItemEntry.cpp
	created:	31/3/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
	
	purpose:	Implementation of ItemEntry widget base class
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
#include "elements/CEGUIItemListBase.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
    ItemEntryWindowRenderer constructor
*************************************************************************/
ItemEntryWindowRenderer::ItemEntryWindowRenderer(const String& name) :
    WindowRenderer(name, "ItemEntry")
{
}

/*************************************************************************
    Constants
*************************************************************************/
const String ItemEntry::WidgetTypeName("CEGUI/ItemEntry");
const String ItemEntry::EventSelectionChanged("SelectionChanged");

/*************************************************************************
	Definition of Properties for this class
*************************************************************************/
ItemEntryProperties::Selectable ItemEntry::d_selectableProperty;
ItemEntryProperties::Selected   ItemEntry::d_selectedProperty;

/*************************************************************************
	Constructor for ItemEntry base class.
*************************************************************************/
ItemEntry::ItemEntry(const String& type, const String& name)
	: Window(type, name),
	d_ownerList(0),
    d_selected(false),
    d_selectable(false)
{
    // add the new properties
    addItemEntryProperties();
}

/*************************************************************************
    Get item pixel size
*************************************************************************/
Size ItemEntry::getItemPixelSize(void) const
{
    if (d_windowRenderer != 0)
    {
        return static_cast<ItemEntryWindowRenderer*>(d_windowRenderer)->getItemPixelSize();
    }
    else
    {
        //return getItemPixelSize_impl();
        throw InvalidRequestException("ItemEntry::getItemPixelSize - This function must be implemented by the window renderer module");
    }
}

/*************************************************************************
    Set selection state. Internal
*************************************************************************/
void ItemEntry::setSelected_impl(bool setting, bool notify)
{
    if (d_selectable && setting != d_selected)
    {
        d_selected = setting;

        // notify the ItemListbox if there is one that we just got selected
        // to ensure selection scheme is not broken when setting selection from code
        if (d_ownerList && notify)
        {
            d_ownerList->notifyItemSelectState(this, setting);
        }

        WindowEventArgs args(this);
        onSelectionChanged(args);
    }
}

/*************************************************************************
    Set selectable "mode"
*************************************************************************/
void ItemEntry::setSelectable(bool setting)
{
    if (d_selectable != setting)
    {
        setSelected(false);
        d_selectable = setting;
    }
}

/*************************************************************************
    Handle selection state change
*************************************************************************/
void ItemEntry::onSelectionChanged(WindowEventArgs& e)
{
    requestRedraw();
    fireEvent(EventSelectionChanged, e, EventNamespace);
}

/*************************************************************************
    Handle 'MouseClicked' event
*************************************************************************/
void ItemEntry::onMouseClicked(MouseEventArgs& e)
{
    Window::onMouseClicked(e);

    if (d_selectable && e.button == LeftButton)
    {
        if (d_ownerList)
            d_ownerList->notifyItemClicked(this);
        else
            setSelected(!isSelected());
        e.handled = true;
    }
}

/*************************************************************************
    Add ItemEntry specific properties
*************************************************************************/
void ItemEntry::addItemEntryProperties(void)
{
    addProperty(&d_selectableProperty);
    addProperty(&d_selectedProperty);
}

} // End of  CEGUI namespace section
