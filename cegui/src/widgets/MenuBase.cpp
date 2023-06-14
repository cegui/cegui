/***********************************************************************
    created:    5/4/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)

    purpose:    Implementation of MenuBase widget base class
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
#include "CEGUI/widgets/MenuBase.h"
#include "CEGUI/widgets/PopupMenu.h"
#include "CEGUI/widgets/MenuItem.h"

namespace CEGUI
{
const String MenuBase::EventNamespace("MenuBase");
const String MenuBase::EventPopupOpened("PopupOpened");
const String MenuBase::EventPopupClosed("PopupClosed");

//----------------------------------------------------------------------------//
MenuBase::MenuBase(const String& type, const String& name)
    : ItemListBase(type, name)
{
    addMenuBaseProperties();
}

//----------------------------------------------------------------------------//
void MenuBase::changePopupMenuItem(MenuItem* item)
{
    if (!d_allowMultiplePopups && d_popupItem == item)
        return;

    if (!d_allowMultiplePopups && d_popupItem)
    {
        WindowEventArgs we(d_popupItem->getPopupMenu());
        d_popupItem->closePopupMenu(false);
        d_popupItem = nullptr;
        onPopupClosed(we);
    }

    if (item)
    {
        d_popupItem = item;
        d_popupItem->openPopupMenu(false);
        WindowEventArgs we(d_popupItem->getPopupMenu());
        onPopupOpened(we);
    }
}

//----------------------------------------------------------------------------//
void MenuBase::onPopupOpened(WindowEventArgs& e)
{
    fireEvent(EventPopupOpened, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void MenuBase::onPopupClosed(WindowEventArgs& e)
{
    fireEvent(EventPopupClosed, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void MenuBase::setAllowMultiplePopups(bool setting)
{
    if (d_allowMultiplePopups != setting)
    {
        if (!setting)
            closeAllChildPopups();
        d_allowMultiplePopups = setting;
    }
}

//----------------------------------------------------------------------------//
void MenuBase::setPopupMenuItemClosing()
{
    if (d_popupItem)
        d_popupItem->startPopupClosing();
}

//----------------------------------------------------------------------------//
void MenuBase::closeAllChildPopups()
{
    for (auto item : d_listItems)
    {
        if (auto menuItem = dynamic_cast<MenuItem*>(item))
        {
            if (!menuItem->getPopupMenu())
                continue;

            WindowEventArgs we(menuItem->getPopupMenu());
            menuItem->closePopupMenu(false);
            onPopupClosed(we);
        }
    }
}

//----------------------------------------------------------------------------//
void MenuBase::onChildRemoved(ElementEventArgs& e)
{
    // If the removed window was our tracked popup item, zero ptr to it.
    if (e.element == d_popupItem)
        d_popupItem = nullptr;

    ItemListBase::onChildRemoved(e);
}

//----------------------------------------------------------------------------//
void MenuBase::onHidden(WindowEventArgs&)
{
    if (!getAutoCloseNestedPopups())
        return;

    changePopupMenuItem(nullptr);

    if (d_allowMultiplePopups)
        closeAllChildPopups();
}

//----------------------------------------------------------------------------//
void MenuBase::addMenuBaseProperties()
{
    const String propertyOrigin = "CEGUI/MenuBase";

    CEGUI_DEFINE_PROPERTY(MenuBase, float,
        "ItemSpacing", "Property to get/set the item spacing of the menu.  Value is a float.",
        &MenuBase::setItemSpacing, &MenuBase::getItemSpacing, 10.0f
    );

    CEGUI_DEFINE_PROPERTY(MenuBase, bool,
        "AllowMultiplePopups", "Property to get/set the state of the allow multiple popups setting for the menu.  Value is either \"true\" or \"false\".",
        &MenuBase::setAllowMultiplePopups, &MenuBase::isMultiplePopupsAllowed, false /* TODO: Inconsistency and awful English */
    );

    CEGUI_DEFINE_PROPERTY(MenuBase, bool,
        "AutoCloseNestedPopups", "Property to set if the menu should close all its open child popups, when it gets hidden. Value is either \"true\" or \"false\".",
        &MenuBase::setAutoCloseNestedPopups, &MenuBase::getAutoCloseNestedPopups, false
    );

    CEGUI_DEFINE_PROPERTY(MenuBase, MenubarDirection,
        "MenubarDirection", "Property to get/set the direction in which child popups should open, or the manner in which this direction is determined. Value is either \"Down\", or \"Up\".",
        &MenuBase::setMenubarDirection, &MenuBase::getMenubarDirection, MenubarDirection::Down
    );
}

}
