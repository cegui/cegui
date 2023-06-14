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
#pragma once
#include "ItemEntry.h"

namespace CEGUI
{

//! Base class for menu items.
class CEGUIEXPORT MenuItem : public ItemEntry
{
public:
    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name

    /** Event fired when the menu item is clicked.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MenuItem that was clicked.
     */
    static const String EventClicked;

    MenuItem(const String& type, const String& name);

    /*!
    \brief
        return true if user is hovering over this widget (or it's pushed and
        user is not over it for highlight)

    \return
        true if the user is hovering or if the button is pushed and the cursor
        is not over the button. Otherwise return false.
    */
    bool isHovering() const { return d_hovering; }

    /*!
    \brief
        Return true if the button widget is in the pushed state.

    \return
        true if the button-type widget is pushed, false if the widget is not
        pushed.
    */
    bool isPushed() const { return d_pushed; }

    //! Returns true if the popup menu attached to the menu item is open.
    bool isOpened() const { return d_opened; }
    //! Returns true if the menu item popup is closing or not.
    bool isPopupClosing() const { return d_popupClosing; }
    //! Returns true if the menu item popup is closed or opened automatically if hovering with the cursor.
    bool hasAutoPopup() const { return d_autoPopupTimeout >= 0.0f; }
    //! Returns the time, which has to elapse before the popup window is opened/closed if the hovering state changes.
    float getAutoPopupTimeout() const { return d_autoPopupTimeout; }
    //! Sets the time, which has to elapse before the popup window is opened/closed if the hovering state changes.
    void setAutoPopupTimeout(float time) { d_autoPopupTimeout = time; }

    /*!
    \brief
        Get the PopupMenu that is currently attached to this MenuItem.

    \return
        A pointer to the currently attached PopupMenu.
        Null is there is no PopupMenu attached.
    */
    PopupMenu* getPopupMenu() const { return d_popup; }

    /*!
    \brief
        Set the popup menu for this item.

    \param popup
        popupmenu window to attach to this item
    */
    void setPopupMenu(PopupMenu* popup);

    /*!
    \brief
        Opens the PopupMenu.

    \param notify
        true if the parent menu bar or menu popup (if any) is to handle the open.
    */
    void openPopupMenu(bool notify = true);

    /*!
    \brief
        Closes the PopupMenu.

    \param notify
        true if the parent menubar (if any) is to handle the close.
    */
    void closePopupMenu(bool notify = true);

    /*!
    \brief
        Toggles the PopupMenu.

    \return
        true if the popup was opened. false if it was closed.
    */
    bool togglePopupMenu();

    //! Starts the closing timer for the popup, which will close it if the timer is enabled.
    void startPopupClosing();

    //! Starts the opening timer for the popup, which will open it if the timer is enabled.
    void startPopupOpening();

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

protected:

    //! Handler invoked internally when the MenuItem is clicked.
    virtual void onClicked(WindowEventArgs& e);

    void onCursorMove(CursorMoveEventArgs& e) override;
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onMouseButtonUp(MouseButtonEventArgs& e) override;
    void onCursorLeaves(CursorInputEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;
    void onDeactivated(ActivationEventArgs& e) override;
    void updateSelf(float elapsed) override;

    void setHovered(bool hovered);
    bool calcHoveredState(const glm::vec2& cursorPos) const;

    //! Recursive function that closes all popups down the hierarchy starting with this one.
    void closeAllMenuItemPopups();

    /*!
    \brief
        Set the popup menu for this item.

    \param popup
        popupmenu window to attach to this item
    */
    void setPopupMenu_impl(PopupMenu* popup, bool add_as_child = true);

    PopupMenu* d_popup = nullptr;       //!< PopupMenu that this item displays when activated.
    float d_autoPopupTimeout = -1.f;    //!< the time in seconds, to wait before opening / closing the popup if the cursor is over the item / over another item in our container
    float d_autoPopupTimeElapsed = 0.f; //!< the current time, which is already elapsed if the timer is running (d_popupClosing or d_popupOpening is true)
    bool d_pushed = false;              //!< true when widget is pushed
    bool d_hovering = false;            //!< true when the button is in 'hover' state and requires the hover rendering.
    bool d_opened = false;              //!< true when the menu item's popup menu is in its opened state.
    bool d_popupClosing = false;        //!< true when the d_popupTimerTimeElapsed timer is running to close the popup (another menu item of our container is hovered)
    bool d_popupOpening = false;        //!< true when the d_popupTimerTimeElapsed timer is running to open the popup (the menu item is hovered)
    bool d_popupWasClosed = false;      //!< Used internally to determine if a popup was just closed on a Clicked event

private:

    void addMenuItemProperties();

    void addChild_impl(Element* element) override;
};

}
