/***********************************************************************
    created:    5/4/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)

    purpose:    Interface to base class for MenuBase widget
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
#pragma once
#include "ItemListBase.h"

namespace CEGUI
{

//! Enum for "direction" in which menubars open.
enum class MenubarDirection : int {
    Down, Up
};

template<>
class PropertyHelper<MenubarDirection>
{
public:
    typedef MenubarDirection return_type;
    typedef return_type safe_method_return_type;
    typedef MenubarDirection pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static const String type("MenubarDirection");
        return type;
    }

    static return_type fromString(const String& str)
    {
            return (str == "Up") ? MenubarDirection::Up : MenubarDirection::Down;
    }

    static string_return_type toString(pass_type val)
    {
        if (val == MenubarDirection::Down)
            return "Down";
        if (val == MenubarDirection::Up)
            return "Up";

        assert(false && "Invalid Menubar Direction Mode");
        return "Down";
    }
};

//! Abstract base class for menus.
class CEGUIEXPORT MenuBase : public ItemListBase
{
public:
    static const String EventNamespace; //!< Namespace for global events

    /** Event fired when a MenuItem attached to this menu opened a PopupMenu.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the PopupMenu that was opened.
     */
    static const String EventPopupOpened;
    /** Event fired when a MenuItem attached to this menu closed a PopupMenu.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the PopupMenu that was closed.
     */
    static const String EventPopupClosed;

    MenuBase(const String& type, const String& name);

    //! Get the item spacing for this menu.
    float getItemSpacing() const { return d_itemSpacing; }
    //! Set the item spacing for this menu.
    void setItemSpacing(float spacing) { d_itemSpacing = spacing; handleUpdatedItemData(); }
    //! Return whether this menu allows multiple popup menus to open at the same time.
    bool isMultiplePopupsAllowed() const { return d_allowMultiplePopups; }
    //! Set whether this menu allows multiple popup menus to be opened simultaneously.
    void setAllowMultiplePopups(bool setting);
    //! Return whether this menu should close all its open child popups, when it gets hidden
    bool getAutoCloseNestedPopups() const { return d_autoCloseNestedPopups; }
    //! Set whether the menu should close all its open child popups, when it gets hidden
    void setAutoCloseNestedPopups(bool setting) { d_autoCloseNestedPopups = setting; }
    //! Get currently opened MenuItem in this menu. Returns NULL if no menu item is open.
    MenuItem* getPopupMenuItem() const { return d_popupItem; }
    //! Get the direction in which menus from the menu should open.
    MenubarDirection getMenubarDirection() const { return d_menubarDirection; }
    //! Set in what manner popups from this menu should be opened.
    void setMenubarDirection(MenubarDirection dir) { d_menubarDirection = dir; }

    /*!
    \brief
        Change the currently open MenuItem in this menu.

    \param item
        Pointer to a MenuItem to open or nullptr to close any opened.
    */
    void changePopupMenuItem(MenuItem* item);

    //! Tells the current popup that it should start its closing timer.
    void setPopupMenuItemClosing();
    //! Close all opened popup children
    void closeAllChildPopups();

protected:

    //! Handler invoked internally when the a MenuItem attached to this menu opens its popup.
    virtual void onPopupOpened(WindowEventArgs& e);
    //! Handler invoked internally when the a MenuItem attached to this menu closes its popup.
    virtual void onPopupClosed(WindowEventArgs& e);

    void onChildRemoved(ElementEventArgs& e) override;
    void onHidden(WindowEventArgs& e) override;

    MenuItem* d_popupItem = nullptr;                              //!< The currently open MenuItem. nullptr if no item is open. If multiple popups are allowed, this means nothing.
    float d_itemSpacing = 0.f;                                    //!< The spacing in pixels between items.
    MenubarDirection d_menubarDirection = MenubarDirection::Down; //!< The preferred way in which menus associated to this menubar should open.
    bool d_allowMultiplePopups = false;                           //!< true if multiple popup menus are allowed simultaneously.  false if not.
    bool d_autoCloseNestedPopups = false;                         //!< true if the menu should close all its open child popups, when it gets hidden

private:

    void addMenuBaseProperties();
};

}
