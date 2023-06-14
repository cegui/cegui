/***********************************************************************
    created:    13/4/2004
    author:        Paul D Turner

    purpose:    Interface to base class for Combobox widget
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
#include "CEGUI/Window.h"
#include "CEGUI/RegexMatcher.h"

namespace CEGUI
{

//! \brief Base class for the Combobox widget
class CEGUIEXPORT Combobox : public Window
{
public:

    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name
    static const String EditboxName;    //!< Widget name for the editbox component.
    static const String DropListName;   //!< Widget name for the drop list component.
    static const String ButtonName;     //!< Widget suffix for the button component.

    // event names from edit box
    /** Event fired when the read-only mode for the edit box is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose read only mode
     * has been changed.
     */
    static const String EventReadOnlyModeChanged;
    /** Event fired when the edix box validation string is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose validation
     * string was changed.
     */
    static const String EventValidationStringChanged;
    /** Event fired when the maximum string length is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose maximum edit box
     * string length has been changed.
     */
    static const String EventMaximumTextLengthChanged;
    /** Event fired when the validity of the Combobox text (as determined by a
     * RegexMatcher object) has changed.
     * Handlers are passed a const RegexMatchStateEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose text validity has
     * changed and RegexMatchStateEventArgs::matchState set to the new match
     * validity. Handler return is significant, as follows:
     * - true indicates the new state - and therfore text - is to be accepted.
     * - false indicates the new state is not acceptable, and the previous text
     *   should remain in place. NB: This is only possible when the validity
     *   change is due to a change in the text, if the validity change is due to
     *   a change in the validation regular expression string, then returning
     *   false will have no effect.
     */
    static const String EventTextValidityChanged;
    /** Event fired when the edit box text insertion position is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose caret position has
     * been changed.
     */
    static const String EventCaretMoved;
    /** Event fired when the current edit box text selection is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose edit box text selection
     * has been changed.
     */
    static const String EventTextSelectionChanged;
    /** Event fired when the number of characters in the edit box has reached
     * the currently set maximum.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose edit box has become
     * full.
     */
    static const String EventEditboxFull;
    /** Event fired when the user accepts the current edit box text by pressing
     * Return, Enter, or Tab.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose edit box text has been
     * accepted / confirmed by the user.
     */
    static const String EventTextAccepted;

    // event names from list widget
    /** Event fired when the contents of the list is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose list content has
     * changed.
     */
    static const String EventListContentsChanged;
    /** Event fired when there is a change to the currently selected item in the
     * list.
     * @note This change in selection may be temporary (for example, when
     * hovering over an item in the combobox). See also the event
     * CEGUI::Combobox::EventListSelectionAccepted that is fired for a selection
     * that the user has 'confirmed' in some way.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose currently selected list
     * item has changed.
     */
    static const String EventListSelectionChanged;
    /** Event fired when the sort mode setting of the list is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose list sorting mode has
     * been changed.
     */
    static const String EventSortModeChanged;
    /** Event fired when the vertical scroll bar 'force' setting for the list is
     * changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose vertical scroll bar
     * setting is changed.
     */
    static const String EventVertScrollbarModeChanged;
    /** Event fired when the horizontal scroll bar 'force' setting for the list
     * is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose horizontal scroll bar
     * setting has been changed.
     */
    static const String EventHorzScrollbarModeChanged;

    // events we produce / generate ourselves
    /** Event fired when the drop-down list is displayed
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose drop down list has
     * been displayed.
     */
    static const String EventDropListDisplayed;
    /** Event fired when the drop-down list is removed / hidden.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox whose drop down list has
     * been hidden.
     */
    static const String EventDropListRemoved;
    /** Event fired when a selection from the drop-down list is accepted. This occurs
     * whenever an item in the list is selected either manually through input or if
     * on of the selection setting functions are called and successful select an item.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Combobox in which the user has
     * confirmed a selection from the drop down list.
     */
    static const String EventListSelectionAccepted;

    Combobox(const String& type, const String& name);

    void initialiseComponents() override;
    bool isHit(const glm::vec2& /*position*/, bool /*allow_disabled*/) const override { return false; }

    /*!
    \brief
        returns the mode of operation for the combo box.

    \return
        - true if the user can show the list and select an item with a single
        cursor activation.
        - false if the user must activate the cursor to show the list and then
        activate the cursor again to select an item.
    */
    bool getSingleCursorActivationEnabled() const { return d_singleClickOperation; }

    /*!
    \brief
        returns true if the drop down list is visible.

    \return
        true if the drop down list is visible, false otherwise.
    */
    bool isDropDownListVisible() const;

    /*!
    \brief
        Return a pointer to the Editbox component widget for this Combobox.

    \return
        Pointer to an Editbox object.

    \exception UnknownObjectException
        Thrown if the Editbox component does not exist.
    */
    Editbox* getEditbox() const;

    /*!
    \brief
        Return a pointer to the PushButton component widget for this Combobox.

    \return
        Pointer to a PushButton object.

    \exception UnknownObjectException
        Thrown if the PushButton component does not exist.
    */
    PushButton* getPushButton() const;

    /*!
    \brief
        Return a pointer to the ComboDropList component widget for this
        Combobox.

    \return
        Pointer to an ComboDropList object.

    \exception UnknownObjectException
        Thrown if the ComboDropList component does not exist.
    */
    ComboDropList* getDropList() const;

    //! return whether the drop-list will vertically auto size to content.
    bool getAutoSizeListHeightToContent() const { return d_autoSizeHeight; }

    //! return whether the drop-list will horizontally auto size to content.
    bool getAutoSizeListWidthToContent() const { return d_autoSizeWidth; }

    //! return true if the Editbox has input focus.
    bool hasInputFocus() const;

    /*!
    \brief
        return true if the Editbox is read-only.

    \return
        true if the Editbox is read only and can't be edited by the user, false if the Editbox is not
        read only and may be edited by the user.
    */
    bool isReadOnly() const;

    /*!
    \brief
        return the validation RegexMatchState for the current Combobox text, given
        the currently set validation string.

    \note
        Validation is performed by means of a regular expression.  If the text
        matches the regex, the text is said to have passed validation.  If the
        text does not match with the regex then the text fails validation.

    \return
        One of the RegexMatchState enumerated values indicating the current match
        state.
    */
    RegexMatchState getTextMatchState() const;

    /*!
    \brief
        return the currently set validation string

    \note
        Validation is performed by means of a regular expression.  If the text matches the regex, the text is said to have passed
        validation.  If the text does not match with the regex then the text fails validation.

    \return
        String object containing the current validation regex data
    */
    const String& getValidationString() const;

    /*!
    \brief
        return the current position of the caret.

    \return
        Index of the insert caret relative to the start of the text.
    */
    size_t getCaretIndex() const;

    /*!
    \brief
        return the current selection start point.

    \return
        Index of the selection start point relative to the start of the text.  If no selection is defined this function returns
        the position of the caret.
    */
    size_t getTextSelectionStart() const;

    /*!
    \brief
        return the current selection end point.

    \return
        Index of the selection end point relative to the start of the text.  If no selection is defined this function returns
        the position of the caret.
    */
    size_t getTextSelectionEnd() const;

    /*!
    \brief
        return the length of the current selection (in code points / characters).

    \return
        Number of code points (or characters) contained within the currently defined selection.
    */
    size_t getTextSelectionLength() const;

    /*!
    \brief
        return the maximum text length set for this Editbox.

    \return
        The maximum number of code points (characters) that can be entered into this Editbox.

    \note
        Depending on the validation string set, the actual length of text that can be entered may be less than the value
        returned here (it will never be more).
    */
    size_t getMaxTextLength() const;

    //! Return number of items attached to the list widget.
    size_t getItemCount() const;

    //! Return a pointer to the currently selected item, ot nullptr if none is.
    StandardItem* getSelectedItem() const;

    /*!
    \brief
        Return the item at index position \a index.

    \param index
        Zero based index of the item to be returned.

    \return
        Pointer to the StandardItem at index position \a index in the list widget.

    \exception    InvalidRequestException    thrown if \a index is out of range.
    */
    StandardItem* getItemFromIndex(size_t index) const;

    /*!
    \brief
        Return the index of StandardItem \a item

    \param item
        Pointer to a StandardItem who's zero based index is to be returned.

    \return
        Zero based index indicating the position of StandardItem \a item in the list widget.
    */
    size_t getItemIndex(const StandardItem* item) const;

    /*!
    \brief
        return whether list sorting is enabled

    \return
        true if the list is sorted, false if the list is not sorted
    */
    bool isSortEnabled() const;

    /*!
    \brief
        return whether the string at index position \a index is selected

    \param index
        Zero based index of the item to be examined.

    \return
        true if the item at \a index is selected, false if the item at \a index is not selected.

    \exception    InvalidRequestException    thrown if \a index is out of range.
    */
    bool isItemSelected(size_t index) const;

    /*!
    \brief
        Search the list for an item with the specified text

    \param text
        String object containing the text to be searched for.

    \param start_item
        StandardItem where the search is to begin, the search will not
        include \a item.  If \a item is NULL, the search will begin from the
        first item in the list.

    \return
        Pointer to the first StandardItem in the list after \a item that has
        text matching \a text. If no item matches the criteria NULL is returned.
    */
    StandardItem* findItemWithText(const String& text, const StandardItem* start_item);

    /*!
    \brief
        Return whether the specified StandardItem is in the List

    \return
       true if StandardItem \a item is in the list, false ifStandardItem
       \a item is not in the list.
    */
    bool isItemInList(const StandardItem* item) const;

    /*!
    \brief
        Return whether the vertical scroll bar is always shown.

    \return
        - true if the scroll bar will always be shown even if it is not required.
        - false if the scroll bar will only be shown when it is required.
    */
    bool isVertScrollbarAlwaysShown() const;

    /*!
    \brief
        Return whether the horizontal scroll bar is always shown.

    \return
        - true if the scroll bar will always be shown even if it is not required.
        - false if the scroll bar will only be shown when it is required.
    */
    bool isHorzScrollbarAlwaysShown() const;

    //! Show the drop-down list
    void showDropList();

    //! Hide the drop-down list
    void hideDropList();

    /*!
    \brief
        Set the mode of operation for the combo box.

    \param setting
        - true if the user should be able to show the list and select an item
        with a single cursor activation.
        - false if the user must activate the cursor to show the list and then
        activate the cursor again to select an item.
    */
    void setSingleCursorActivationEnabled(bool setting);

    //! Select item in list matching editbox text, clear selection if none match
    void selectListItemWithEditboxText();

    /*!
    \brief
        Sets whether the Combobox drop-down list will automatically resize
        it's height according to the total height of items added to the list.
    */
    void setAutoSizeListHeightToContent(bool auto_size);

    /*!
    \brief
        Sets whether the Combobox drop-down list will automatically resize
        it's width according to the width of the items added to the list.
    */
    void setAutoSizeListWidthToContent(bool auto_size);

    //! update drop list size according to auto-size options.
    void updateAutoSizedDropList();

    /*!
    \brief
        Specify whether the Editbox is read-only.

    \param setting
        true if the Editbox is read only and can't be edited by the user, false if the Editbox is not
        read only and may be edited by the user.
    */
    void setReadOnly(bool setting);


    /*!
    \brief
        Set the text validation string.

    \note
        Validation is performed by means of a regular expression.  If the text matches the regex, the text is said to have passed
        validation.  If the text does not match with the regex then the text fails validation.

    \param validation_string
        String object containing the validation regex data to be used.
    */
    void setValidationString(const String& validation_string);

    /*!
    \brief
        Set the current position of the caret.

    \param caret_pos
        New index for the insert caret relative to the start of the text.  If the value specified is greater than the
        number of characters in the Editbox, the caret is positioned at the end of the text.
    */
    void setCaretIndex(size_t caret_pos);

    /*!
    \brief
        Define the current selection for the Editbox

    \param start_pos
        Index of the starting point for the selection.  If this value is greater than the number of characters in the Editbox, the
        selection start will be set to the end of the text.

    \param end_pos
        Index of the ending point for the selection.  If this value is greater than the number of characters in the Editbox, the
        selection start will be set to the end of the text.
    */

    void setTextSelection(size_t start_pos, size_t end_pos);
    /*!
    \brief
        Define the current selection start for the Editbox

    \param start_pos
        Index of the starting point for the selection.  If this value is greater than the number of characters in the Editbox, the
        selection start will be set to the end of the text.
    */
    void setTextSelectionStart(size_t start_pos);

    /*!
    \brief
        Define the current selection for the Editbox

    \param start_pos
        Length of the selection.
    */
    void setTextSelectionLength(size_t length);

    /*!
    \brief
        set the maximum text length for this Editbox.

    \param max_len
        The maximum number of code points (characters) that can be entered into this Editbox.

    \note
        Depending on the validation string set, the actual length of text that can be entered may be less than the value
        set here (it will never be more).
    */
    void setMaxTextLength(size_t max_len);

    /*!
    \brief
        Remove all items from the list.

        Note that this will cause 'AutoDelete' items to be deleted.
    */
    void resetList();

    /*!
    \brief
        Add the given StandardItem to the list.

    \param item
        Pointer to the StandardItem to be added to the list. Note that it is
        the passed object that is added to the list, a copy is not made.
        If this parameter is NULL, nothing happens.
    */
    void addItem(StandardItem* item);

    /*!
    \brief
        Insert an item into the list after a specified item already in the list.

        Note that if the list is sorted, the item may not end up in the requested position.

    \param item
        Pointer to the StandardItem to be inserted. Note that it is the passed
        object that is added to the list, a copy is not made.
        If this parameter is NULL, nothing happens.

    \param position
        Pointer to a StandardItem that \a item is to be inserted after. If this
        parameter is NULL, the item is inserted at the start of the list.
    */
   void insertItem(StandardItem* item, const StandardItem* position);

    /*!
    \brief
        Removes the given item from the list widget.

    \param item
        Pointer to the StandardItem that is to be removed.
        If \a item is not attached to this list widget then nothing will happen.
    */
    void removeItem(const StandardItem* item);

    //! Clear the selected state for all items.
    void clearAllSelections();

    /*!
    \brief
        Set whether the list should be sorted.

    \param setting
        true if the list should be sorted, false if the list should not be sorted.
    */
    void setSortingEnabled(bool setting);

    /*!
    \brief
        Set whether the vertical scroll bar should always be shown.

    \param setting
        true if the vertical scroll bar should be shown even when it is not required.  false if the vertical
        scroll bar should only be shown when it is required.
    */
    void setShowVertScrollbar(bool setting);

    /*!
    \brief
        Set whether the horizontal scroll bar should always be shown.

    \param setting
        true if the horizontal scroll bar should be shown even when it is not required.  false if the horizontal
        scroll bar should only be shown when it is required.
    */
    void setShowHorzScrollbar(bool setting);

    /*!
    \brief
        Set the select state of an attached StandardItem.

    \param item
        The StandardItem to be affected.

    \param state
        true to select the item, false to de-select the item.
    */
    void setItemSelectState(StandardItem* item, bool state);

    /*!
    \brief
        Set the select state of an attached StandardItem.

    \param item_index
        The zero based index of the StandardItem to be affected.
        This must be a valid index (0 <= index < getItemCount())

    \param state
        true to select the item, false to de-select the item.

    \exception  InvalidRequestException thrown if \a item_index is out of range for the list widget
    */
    void setItemSelectState(size_t item_index, bool state);

    /*!
    \brief
        Causes the list widget to update it's internal state after changes have
        been made to one or more attached StandardItem objects.

        Client code must call this whenever it has made any changes to
        StandardItem objects already attached to the list widget. If you are
        just adding items, or removed items to update them prior to re-adding
        them, there is no need to call this method.
    */
    void handleUpdatedListItemData();

protected:

    //! Update the Combobox text to reflect programmatically made changes to selected list item.
    void itemSelectChangeTextUpdate(const StandardItem* const item, bool new_state, bool old_state);

    bool button_PressHandler(const EventArgs& e);
    bool droplist_SelectionAcceptedHandler(const EventArgs& e);
    bool droplist_HiddenHandler(const EventArgs& e);
    bool editbox_PointerPressHoldHandler(const EventArgs& e);
    bool editbox_ReadOnlyChangedHandler(const EventArgs& e);
    bool editbox_ValidationStringChangedHandler(const EventArgs& e);
    bool editbox_MaximumTextLengthChangedHandler(const EventArgs& e);
    bool editbox_TextValidityChangedHandler(const EventArgs& e);
    bool editbox_CaretMovedHandler(const EventArgs& e);
    bool editbox_TextSelectionChangedHandler(const EventArgs& e);
    bool editbox_EditboxFullEventHandler(const EventArgs& e);
    bool editbox_TextAcceptedEventHandler(const EventArgs& e);
    bool editbox_TextChangedEventHandler(const EventArgs& e);
    bool listwidget_ListContentsChangedHandler(const EventArgs& e);
    bool listwidget_ListSelectionChangedHandler(const EventArgs& e);
    bool listwidget_SortModeChangedHandler(const EventArgs& e);
    bool listwidget_VertScrollModeChangedHandler(const EventArgs& e);
    bool listwidget_HorzScrollModeChangedHandler(const EventArgs& e);

    //! Handler called internally when the read only state of the Combobox's Editbox has been changed.
    virtual void onReadOnlyChanged(WindowEventArgs& e);
    //! Handler called internally when the Combobox's Editbox validation string has been changed.
    virtual void onValidationStringChanged(WindowEventArgs& e);
    //! Handler called internally when the Combobox's Editbox maximum text length is changed.
    virtual void onMaximumTextLengthChanged(WindowEventArgs& e);
    //! Handler called when something has caused the validity state of the current text to change.
    virtual void onTextValidityChanged(RegexMatchStateEventArgs& e);
    //! Handler called internally when the caret in the Comboxbox's Editbox moves.
    virtual void onCaretMoved(WindowEventArgs& e);
    //! Handler called internally when the selection within the Combobox's Editbox changes.
    virtual void onTextSelectionChanged(WindowEventArgs& e);
    //! Handler called internally when the maximum length is reached for text in the Combobox's Editbox.
    virtual void onEditboxFullEvent(WindowEventArgs& e);
    //! Handler called internally when the text in the Combobox's Editbox is accepted (by various means).
    virtual void onTextAcceptedEvent(WindowEventArgs& e);
    //! Handler called internally when the Combobox's Drop-down list contents are changed.
    virtual void onListContentsChanged(WindowEventArgs& e);
    /*!
    \brief
        Handler called internally when the selection within the Combobox's drop-down list changes
        (this is not the 'final' accepted selection, just the currently highlighted item).
    */
    virtual void onListSelectionChanged(WindowEventArgs& e);
    //! Handler called  fired internally when the sort mode for the Combobox's drop-down list is changed.
    virtual void onSortModeChanged(WindowEventArgs& e);
    /*!
    \brief
        Handler called internally when the 'force' setting for the vertical scrollbar within the Combobox's
        drop-down list is changed.
    */
    virtual void onVertScrollbarModeChanged(WindowEventArgs& e);
    /*!
    \brief
        Handler called internally when the 'force' setting for the horizontal scrollbar within the Combobox's
        drop-down list is changed.
    */
    virtual void onHorzScrollbarModeChanged(WindowEventArgs& e);
    //! Handler called internally when the Combobox's drop-down list has been displayed.
    virtual void onDropListDisplayed(WindowEventArgs& e);
    //! Handler called internally when the Combobox's drop-down list has been hidden.
    virtual void onDroplistRemoved(WindowEventArgs& e);
    //! Handler called internally when the user has confirmed a selection within the Combobox's drop-down list.
    virtual void onListSelectionAccepted(WindowEventArgs& e);

    void onFontChanged(WindowEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;
    void onActivated(ActivationEventArgs& e) override;
    void onSized(ElementEventArgs& e) override;

    bool d_singleClickOperation = false;  //!< true if user can show and select from list in a single click.
    bool d_autoSizeHeight = false;
    bool d_autoSizeWidth = false;

private:

    void addComboboxProperties();
};

}
