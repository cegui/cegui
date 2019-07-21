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
#ifndef _CEGUIMenuBase_h_
#define _CEGUIMenuBase_h_

#include "../Base.h"
#include "../Window.h"
#include "./ItemListBase.h"


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Enum for "direction" in which menubars open.
*/

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
        static String type("MenubarDirection");
        return type;
    }

    static return_type fromString(const String& str)
    {

        if (str == "Up")
        {
            return MenubarDirection::Up;
        }        
        else
        {
            return MenubarDirection::Down;
        }
    }

    static string_return_type toString(pass_type val)
    {
        if (val == MenubarDirection::Down)
        {
            return "Down";
        }
        else if (val == MenubarDirection::Up)
        {
            return "Up";
        }
        else
        {
            assert(false && "Invalid Menubar Direction Mode");
            return "Down";
        }
    }

};


/*!
\brief
    Abstract base class for menus.
*/
class CEGUIEXPORT MenuBase : public ItemListBase
{
public:
    static const String EventNamespace;             //!< Namespace for global events


    /*************************************************************************
        Event name constants
    *************************************************************************/
    // generated internally by Window
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


    /*************************************************************************
        Accessor type functions
    *************************************************************************/
    /*!
    \brief
        Get the item spacing for this menu.

    \return
        A float value with the current item spacing for this menu
    */
    float   getItemSpacing(void) const
    {
        return d_itemSpacing;
    }


    /*!
    \brief
        Return whether this menu allows multiple popup menus to open at the same time.

    \return
        true if this menu allows multiple popup menus to be opened simultaneously. false if not
    */
    bool    isMultiplePopupsAllowed(void) const
    {
        return d_allowMultiplePopups;
    }

    /*!
    \brief
        Return whether this menu should close all its open child popups, when it gets hidden

    \return
        true if the menu should close all its open child popups, when it gets hidden
    */
    bool    getAutoCloseNestedPopups(void) const
    {
        return d_autoCloseNestedPopups;
    }

    /*!
    \brief
        Get currently opened MenuItem in this menu. Returns NULL if no menu item is open.

    \return
        Pointer to the MenuItem currently open.
    */
    MenuItem*   getPopupMenuItem(void) const
    {
        return d_popupItem;
    }

    /*!
    \brief
        Get the direction in which menus from the menu should open.
    
    \return
        Enum indicating the direction / rule.
    */
    MenubarDirection getMenubarDirection(void) const
    {
       return d_menubarDirection;
    }


    /*************************************************************************
        Manipulators
    *************************************************************************/
    /*!
    \brief
        Set the item spacing for this menu.
    */
    void    setItemSpacing(float spacing)
    {
        d_itemSpacing = spacing;
        handleUpdatedItemData();
    }


    /*!
    \brief
        Change the currently open MenuItem in this menu.

    \param item
        Pointer to a MenuItem to open or NULL to close any opened.
    */
    void    changePopupMenuItem(MenuItem* item);


    /*!
    \brief
        Set whether this menu allows multiple popup menus to be opened simultaneously.
    */
    void    setAllowMultiplePopups(bool setting);

    /*!
    \brief
        Set whether the menu should close all its open child popups, when it gets hidden
    */
    void    setAutoCloseNestedPopups(bool setting)
    {
        d_autoCloseNestedPopups = setting;
    }
    /*!
    \brief
        tells the current popup that it should start its closing timer.
    */
    void    setPopupMenuItemClosing();

    /*!
    \brief
        Set in what manner popups from this menu should be opened.
    */
    void    setMenubarDirection(MenubarDirection dir)
    {
        d_menubarDirection = dir;
    }


    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for MenuBase objects
    */
    MenuBase(const String& type, const String& name);


    /*!
    \brief
        Destructor for MenuBase objects
    */
    virtual ~MenuBase(void);


protected:
    /*************************************************************************
        New Event Handlers
    *************************************************************************/
    /*!
    \brief
        handler invoked internally when the a MenuItem attached to this menu opens its popup.
    */
    virtual void    onPopupOpened(WindowEventArgs& e);


    /*!
    \brief
        handler invoked internally when the a MenuItem attached to this menu closes its popup.
    */
    virtual void    onPopupClosed(WindowEventArgs& e);

    // overridden from base
    void onChildRemoved(ElementEventArgs& e) override;
    void onHidden(WindowEventArgs& e) override;

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    float d_itemSpacing;        //!< The spacing in pixels between items.

    MenuItem* d_popupItem;      //!< The currently open MenuItem. NULL if no item is open. If multiple popups are allowed, this means nothing.
    bool d_allowMultiplePopups; //!< true if multiple popup menus are allowed simultaneously.  false if not.
    bool d_autoCloseNestedPopups; //!< true if the menu should close all its open child popups, when it gets hidden

    MenubarDirection d_menubarDirection; //!< The preferred way in which menus associated to this menubar should open.

private:
    /*************************************************************************
    Private methods
    *************************************************************************/
    void    addMenuBaseProperties(void);
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIMenuBase_h_
