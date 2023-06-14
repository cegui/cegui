/***********************************************************************
	created:	31/3/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)

	purpose:	Implementation of ItemEntry widget base class
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
#include "CEGUI/widgets/ItemListBase.h"
#include "CEGUI/Exceptions.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ItemEntry::WidgetTypeName("CEGUI/ItemEntry");
const String ItemEntry::EventSelectionChanged("SelectionChanged");

//----------------------------------------------------------------------------//
ItemEntryWindowRenderer::ItemEntryWindowRenderer(const String& name) :
    WindowRenderer(name, "ItemEntry")
{
}

//----------------------------------------------------------------------------//
ItemEntry::ItemEntry(const String& type, const String& name)
	: Window(type, name)
{
    addItemEntryProperties();
}

//----------------------------------------------------------------------------//
Sizef ItemEntry::getItemPixelSize() const
{
    if (d_windowRenderer)
        return static_cast<ItemEntryWindowRenderer*>(d_windowRenderer)->getItemPixelSize();

    throw InvalidRequestException("This function must be implemented by the window renderer module");
}

//----------------------------------------------------------------------------//
void ItemEntry::setSelected_impl(bool setting, bool notify)
{
    if (d_selectable && setting != d_selected)
    {
        d_selected = setting;

        // notify the ItemListBase if there is one that we just got selected
        // to ensure selection scheme is not broken when setting selection from code
        if (d_ownerList && notify)
        {
            d_ownerList->notifyItemSelectState(this, setting);
        }

        WindowEventArgs args(this);
        onSelectionChanged(args);
    }
}

//----------------------------------------------------------------------------//
void ItemEntry::setSelectable(bool setting)
{
    if (d_selectable != setting)
    {
        setSelected(false);
        d_selectable = setting;
    }
}

//----------------------------------------------------------------------------//
void ItemEntry::onSelectionChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventSelectionChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool ItemEntry::validateWindowRenderer(const WindowRenderer* renderer) const
{
	return dynamic_cast<const ItemEntryWindowRenderer*>(renderer) != nullptr;
}

//----------------------------------------------------------------------------//
void ItemEntry::onClick(MouseButtonEventArgs& e)
{
    if (d_selectable && e.d_button == MouseButton::Left)
    {
        if (d_ownerList)
            d_ownerList->notifyItemActivated(this, e.d_modifiers.hasCtrl(), e.d_modifiers.hasShift());
        else
            setSelected(!isSelected());
        ++e.handled;
    }

    Window::onClick(e);
}

//----------------------------------------------------------------------------//
void ItemEntry::addItemEntryProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(ItemEntry, bool,
        "Selectable", "Property to get/set the state of the selectable setting for the ItemEntry.  Value is either \"true\" or \"false\".",
        &ItemEntry::setSelectable, &ItemEntry::isSelectable, false
    );

    CEGUI_DEFINE_PROPERTY(ItemEntry, bool,
        "Selected", "Property to get/set the state of the selected setting for the ItemEntry.  Value is either \"true\" or \"false\".",
        &ItemEntry::setSelected, &ItemEntry::isSelected, false
    );
}

}
