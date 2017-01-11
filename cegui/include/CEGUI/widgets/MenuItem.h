/***********************************************************************
    created:    2/4/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)

    purpose:    Interface to base class for MenuItem widget
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
#ifndef _CEGUIMenuItem_h_
#define _CEGUIMenuItem_h_

#include "../Base.h"
#include "../Window.h"
#include "./ItemEntry.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Base class for menu items.
*/
class CEGUIEXPORT MenuItem : public ItemEntry
{
public:
    static const String EventNamespace;             //!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

    /*************************************************************************
        Event name constants
    *************************************************************************/
    // generated internally by Window
    /** Event fired when the menu item is clicked.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MenuItem that was clicked.
     */
    static const String EventClicked;


    /*************************************************************************
        Accessor type functions
    *************************************************************************/
    /*!
    \brief
        return true if user is hovering over this widget (or it's pushed and
        user is not over it for highlight)

    \return
        true if the user is hovering or if the button is pushed and the cursor
        is not over the button. Otherwise return false.
    */
    bool    isHovering(void) const
    {
        return d_hovering;
    }


    /*!
    \brief
        Return true if the button widget is in the pushed state.

    \return
        true if the button-type widget is pushed, false if the widget is not
        pushed.
    */
    bool    isPushed(void) const
    {
        return d_pushed;
    }


    /*!
    \brief
        Returns true if the popup menu attached to the menu item is open.
    */
    bool    isOpened(void) const
    {
        return d_opened;
    }

    /*!
    \brief
        Returns true if the menu item popup is closing or not.
    */
    bool    isPopupClosing(void) const
    {
        return d_popupClosing;
    }

    /*!
    \brief
        Returns true if the menu item popup is closed or opened automatically
        if hovering with the cursor.
    */
    bool    hasAutoPopup(void) const
    {
        return d_autoPopupTimeout > 0.0f;
    }

    /*!
    \brief
        Returns the time, which has to elapse before the popup window is
        opened/closed if the hovering state changes.
    */
    float    getAutoPopupTimeout(void) const
    {
        return d_autoPopupTimeout;
    }

    /*!
    \brief
        Sets the time, which has to elapse before the popup window is
        opened/closed if the hovering state changes.
    */
    void    setAutoPopupTimeout(float time)
    {
        d_autoPopupTimeout = time;
    }

    /*!
    \brief
        Get the PopupMenu that is currently attached to this MenuItem.

    \return
        A pointer to the currently attached PopupMenu.
        Null is there is no PopupMenu attached.
    */
    PopupMenu*  getPopupMenu(void) const
    {
        return d_popup;
    }

    /*************************************************************************
        Manipulators
    *************************************************************************/
    /*!
    \brief
        Set the popup menu for this item.

    \param popup
        popupmenu window to attach to this item

    \return
        Nothing.
    */
    void    setPopupMenu(PopupMenu* popup);


    /*!
    \brief
        Opens the PopupMenu.

    \param notify
        true if the parent menu bar or menu popup (if any) is to handle the open.
    */
    void    openPopupMenu(bool notify = true);


    /*!
    \brief
        Closes the PopupMenu.

    \param notify
        true if the parent menubar (if any) is to handle the close.

    \return
        Nothing.
    */
    void    closePopupMenu(bool notify = true);


    /*!
    \brief
        Toggles the PopupMenu.

    \return
        true if the popup was opened. false if it was closed.
    */
    bool    togglePopupMenu(void);

    /*!
    \brief
        starts the closing timer for the popup, which will close it if the timer is enabled.
    */
    void    startPopupClosing(void);

    /*!
    \brief
        starts the opening timer for the popup, which will open it if the timer is enabled.
    */
    void    startPopupOpening(void);

    /*!
    \brief
       Computes the offset at which a popup menu will appear. Returns false if
       the popup menu should not be moved from its current position.
       
       The default impl will try to avoid having the popup menu clipped.

    \param offset
       This is (potentially) the output value of the function.
       
    \return
       true if an offset was computed and stored at "offset", false if not.
    */
    virtual bool computePopupOffset(UVector2 & offset) const;
    
    /*!
     \brief
       Computes the box within which we attempt to place the popup menu.
       This is used in default impl of computePopupOffset.
       It should be in absolute coordinates.

     \return
       In the default impl, it's just the size of the root container.
       In some applications you may wish to confine the popup menu to a smaller
       area, or make MenuItem search its ancestors for a scrollable pane and
       use the bounds of that, etc.
    */
    virtual Rectf popupBoundingBox() const;

    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for MenuItem objects
    */
    MenuItem(const String& type, const String& name);


    /*!
    \brief
        Destructor for MenuItem objects
    */
    virtual ~MenuItem(void);


protected:
    /*************************************************************************
        New Event Handlers
    *************************************************************************/
    /*!
    \brief
        handler invoked internally when the MenuItem is clicked.
    */
    virtual void    onClicked(WindowEventArgs& e);


    /*************************************************************************
        Overridden event handlers
    *************************************************************************/
    void    onCursorMove(CursorInputEventArgs& e) override;
    void    onCursorPressHold(CursorInputEventArgs& e) override;
    void    onCursorActivate(CursorInputEventArgs& e) override;
    void    onCaptureLost(WindowEventArgs& e) override;
    void    onCursorLeaves(CursorInputEventArgs& e) override;
    void    onTextChanged(WindowEventArgs& e) override;
    void    updateSelf(float elapsed) override;


    /*************************************************************************
        Implementation Functions
    *************************************************************************/
    /*!
    \brief
        Update the internal state of the widget with the cursor at the given position.

    \param cursor_pos
        Point object describing, in screen pixel co-ordinates, the location of the cursor.

    \return
        Nothing
    */
    void    updateInternalState(const glm::vec2& cursor_pos);


    /*!
    \brief
        Recursive function that closes all popups down the hierarchy starting with this one.

    \return
        Nothing.
    */
    void    closeAllMenuItemPopups();


    /*!
    \brief
        Set the popup menu for this item.

    \param popup
        popupmenu window to attach to this item

    \return
        Nothing.
    */
    void    setPopupMenu_impl(PopupMenu* popup, bool add_as_child = true);

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    bool d_pushed;          //!< true when widget is pushed
    bool d_hovering;        //!< true when the button is in 'hover' state and requires the hover rendering.
    bool d_opened;          //!< true when the menu item's popup menu is in its opened state.
    bool d_popupClosing;    //!< true when the d_popupTimerTimeElapsed timer is running to close the popup (another menu item of our container is hovered)
    bool d_popupOpening;    //!< true when the d_popupTimerTimeElapsed timer is running to open the popup (the menu item is hovered)
    float d_autoPopupTimeout; //!< the time in seconds, to wait before opening / closing the popup if the cursor is over the item / over another item in our container
    float d_autoPopupTimeElapsed;  //!< the current time, which is already elapsed if the timer is running (d_popupClosing or d_popupOpening is true)

    PopupMenu*  d_popup;    //!< PopupMenu that this item displays when activated.

    bool d_popupWasClosed;  //!< Used internally to determine if a popup was just closed on a Clicked event

private:

    /*************************************************************************
        Private methods
    *************************************************************************/
    void addMenuItemProperties(void);

    /*!
    \copydoc Window::addChild_impl
    */
    void addChild_impl(Element* element) override;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIMenuItem_h_
