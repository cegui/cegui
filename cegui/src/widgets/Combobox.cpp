/***********************************************************************
	created:	13/4/2004
	author:		Paul D Turner

	purpose:	Implementation of Combobox base class
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
#include "CEGUI/widgets/Combobox.h"
#include "CEGUI/widgets/Editbox.h"
#include "CEGUI/widgets/PushButton.h"
#include "CEGUI/widgets/ComboDropList.h"
#include "CEGUI/GUIContext.h"

namespace CEGUI
{
const String Combobox::EventNamespace("Combobox");
const String Combobox::WidgetTypeName("CEGUI/Combobox");
const String Combobox::EditboxName("__auto_editbox__");
const String Combobox::DropListName("__auto_droplist__");
const String Combobox::ButtonName("__auto_button__");

// event names from edit box
const String Combobox::EventReadOnlyModeChanged("ReadOnlyModeChanged");
const String Combobox::EventValidationStringChanged("ValidationStringChanged");
const String Combobox::EventMaximumTextLengthChanged("MaximumTextLengthChanged");
const String Combobox::EventTextValidityChanged("TextValidityChanged");
const String Combobox::EventCaretMoved("CaretMoved");
const String Combobox::EventTextSelectionChanged("TextSelectionChanged");
const String Combobox::EventEditboxFull("EditboxFull");
const String Combobox::EventTextAccepted("TextAccepted");

// event names from list widget
const String Combobox::EventListContentsChanged("ListContentsChanged");
const String Combobox::EventListSelectionChanged("ListSelectionChanged");
const String Combobox::EventSortModeChanged("SortModeChanged");
const String Combobox::EventVertScrollbarModeChanged("VertScrollbarModeChanged");
const String Combobox::EventHorzScrollbarModeChanged("HorzScrollbarModeChanged");

// events we produce / generate ourselves
const String Combobox::EventDropListDisplayed("DropListDisplayed");
const String Combobox::EventDropListRemoved("DropListRemoved");
const String Combobox::EventListSelectionAccepted("ListSelectionAccepted");

//----------------------------------------------------------------------------//
Combobox::Combobox(const String& type, const String& name)
    : Window(type, name)
{
	addComboboxProperties();
}

//----------------------------------------------------------------------------//
void Combobox::initialiseComponents()
{
    Editbox* editbox = getEditbox();
    editbox->setFont(d_font);
    editbox->banPropertyFromXML("MaxTextLength");
    editbox->banPropertyFromXML("ReadOnly");

    PushButton* button = getPushButton();

    ComboDropList* droplist = getDropList();
    droplist->setFont(d_font);
    droplist->hide();
    droplist->banPropertyFromXML(Window::VisiblePropertyName);

	// internal event wiring
	button->subscribeEvent(PushButton::EventMouseButtonDown, Event::Subscriber(&CEGUI::Combobox::button_PressHandler, this));
	droplist->subscribeEvent(ComboDropList::EventListSelectionAccepted, Event::Subscriber(&CEGUI::Combobox::droplist_SelectionAcceptedHandler, this));
	droplist->subscribeEvent(Window::EventHidden, Event::Subscriber(&CEGUI::Combobox::droplist_HiddenHandler, this));
	editbox->subscribeEvent(Window::EventMouseButtonDown, Event::Subscriber(&CEGUI::Combobox::editbox_PointerPressHoldHandler, this));

	// event forwarding setup
	editbox->subscribeEvent(Editbox::EventReadOnlyModeChanged, Event::Subscriber(&CEGUI::Combobox::editbox_ReadOnlyChangedHandler, this));
	editbox->subscribeEvent(Editbox::EventValidationStringChanged, Event::Subscriber(&CEGUI::Combobox::editbox_ValidationStringChangedHandler, this));
	editbox->subscribeEvent(Editbox::EventMaximumTextLengthChanged, Event::Subscriber(&CEGUI::Combobox::editbox_MaximumTextLengthChangedHandler, this));
	editbox->subscribeEvent(Editbox::EventTextValidityChanged, Event::Subscriber(&CEGUI::Combobox::editbox_TextValidityChangedHandler, this));
	editbox->subscribeEvent(Editbox::EventCaretMoved, Event::Subscriber(&CEGUI::Combobox::editbox_CaretMovedHandler, this));
	editbox->subscribeEvent(Editbox::EventTextSelectionChanged, Event::Subscriber(&CEGUI::Combobox::editbox_TextSelectionChangedHandler, this));
	editbox->subscribeEvent(Editbox::EventEditboxFull, Event::Subscriber(&CEGUI::Combobox::editbox_EditboxFullEventHandler, this));
	editbox->subscribeEvent(Editbox::EventTextAccepted, Event::Subscriber(&CEGUI::Combobox::editbox_TextAcceptedEventHandler, this));
	editbox->subscribeEvent(Editbox::EventTextChanged, Event::Subscriber(&CEGUI::Combobox::editbox_TextChangedEventHandler, this));

    droplist->subscribeEvent(ListWidget::EventViewContentsChanged, Event::Subscriber(&Combobox::listwidget_ListContentsChangedHandler, this));
    droplist->subscribeEvent(ListWidget::EventSelectionChanged, Event::Subscriber(&Combobox::listwidget_ListSelectionChangedHandler, this));
    droplist->subscribeEvent(ListWidget::EventSortModeChanged, Event::Subscriber(&Combobox::listwidget_SortModeChangedHandler, this));
    droplist->subscribeEvent(ListWidget::EventVertScrollbarDisplayModeChanged, Event::Subscriber(&Combobox::listwidget_VertScrollModeChangedHandler, this));
    droplist->subscribeEvent(ListWidget::EventHorzScrollbarDisplayModeChanged, Event::Subscriber(&Combobox::listwidget_HorzScrollModeChangedHandler, this));

    Window::initialiseComponents();
}

//----------------------------------------------------------------------------//
void Combobox::showDropList()
{
    updateAutoSizedDropList();

	// Display the box
    ComboDropList* droplist = getDropList();
	droplist->show();
	droplist->activate();
	droplist->captureInput();

	// Fire off event
	WindowEventArgs args(this);
	onDropListDisplayed(args);
}

//----------------------------------------------------------------------------//
void Combobox::hideDropList()
{
	// the natural order of things when this happens will ensure the list is
	// hidden and events are fired.
	getDropList()->releaseInput();
}

//----------------------------------------------------------------------------//
bool Combobox::hasInputFocus() const
{
	return getEditbox()->hasInputFocus();
}

//----------------------------------------------------------------------------//
bool Combobox::isReadOnly() const
{
	return getEditbox()->isReadOnly();
}

//----------------------------------------------------------------------------//
RegexMatchState Combobox::getTextMatchState() const
{
	return getEditbox()->getTextMatchState();
}

//----------------------------------------------------------------------------//
const String& Combobox::getValidationString() const
{
	return getEditbox()->getValidationString();
}

//----------------------------------------------------------------------------//
size_t Combobox::getCaretIndex() const
{
	return getEditbox()->getCaretIndex();
}

//----------------------------------------------------------------------------//
size_t Combobox::getTextSelectionStart() const
{
	return getEditbox()->getSelectionStart();
}

//----------------------------------------------------------------------------//
size_t Combobox::getTextSelectionEnd() const
{
	return getEditbox()->getSelectionEnd();
}

//----------------------------------------------------------------------------//
size_t Combobox::getTextSelectionLength() const
{
	return getEditbox()->getSelectionLength();
}

//----------------------------------------------------------------------------//
size_t Combobox::getMaxTextLength() const
{
	return getEditbox()->getMaxTextLength();
}

//----------------------------------------------------------------------------//
void Combobox::setReadOnly(bool setting)
{
	getEditbox()->setReadOnly(setting);
}

//----------------------------------------------------------------------------//
void Combobox::setValidationString(const String& validation_string)
{
	getEditbox()->setValidationString(validation_string);
}

//----------------------------------------------------------------------------//
void Combobox::setCaretIndex(size_t caret_pos)
{
	getEditbox()->setCaretIndex(caret_pos);
}

//----------------------------------------------------------------------------//
void Combobox::setTextSelection(size_t start_pos, size_t end_pos)
{
	getEditbox()->setSelection(start_pos, end_pos);
}

//----------------------------------------------------------------------------//
void Combobox::setTextSelectionStart(size_t start_pos)
{
	getEditbox()->setSelectionStart(start_pos);
}

//----------------------------------------------------------------------------//
void Combobox::setTextSelectionLength(size_t length)
{
	getEditbox()->setSelectionLength(length);
}

//----------------------------------------------------------------------------//
void Combobox::setMaxTextLength(size_t max_len)
{
	getEditbox()->setMaxTextLength(max_len);
}

//----------------------------------------------------------------------------//
size_t Combobox::getItemCount() const
{
	return getDropList()->getItemCount();
}

//----------------------------------------------------------------------------//
StandardItem* Combobox::getSelectedItem() const
{
	return getDropList()->getFirstSelectedItem();
}

//----------------------------------------------------------------------------//
StandardItem* Combobox::getItemFromIndex(size_t index) const
{
    return getDropList()->getItemAtIndex(index);
}

//----------------------------------------------------------------------------//
size_t Combobox::getItemIndex(const StandardItem* item) const
{
    return getDropList()->getModel()->getChildId(item);
}

//----------------------------------------------------------------------------//
bool Combobox::isSortEnabled() const
{
    return getDropList()->getSortMode() != ViewSortMode::NoSorting;
}

//----------------------------------------------------------------------------//
bool Combobox::isItemSelected(size_t index) const
{
    return getDropList()->isIndexSelected(index);
}

//----------------------------------------------------------------------------//
StandardItem* Combobox::findItemWithText(const String& text, const StandardItem* start_item)
{
    return getDropList()->findItemWithText(text, start_item);
}

//----------------------------------------------------------------------------//
bool Combobox::isItemInList(const StandardItem* item) const
{
    return getDropList()->isItemInList(item);
}

//----------------------------------------------------------------------------//
void Combobox::resetList()
{
    getDropList()->clearList();
}

//----------------------------------------------------------------------------//
void Combobox::addItem(StandardItem* item)
{
    getDropList()->addItem(item);
}

//----------------------------------------------------------------------------//
void Combobox::insertItem(StandardItem* item, const StandardItem* position)
{
    getDropList()->insertItem(item, position);
}

//----------------------------------------------------------------------------//
void Combobox::removeItem(const StandardItem* item)
{
    getDropList()->removeItem(item);
}

//----------------------------------------------------------------------------//
void Combobox::clearAllSelections()
{
    getDropList()->clearSelections();
}

//----------------------------------------------------------------------------//
void Combobox::setSortingEnabled(bool setting)
{
    //TODO: migrate the sorting.
    getDropList()->setSortMode(setting ? ViewSortMode::Ascending : ViewSortMode::NoSorting);
}

//----------------------------------------------------------------------------//
void Combobox::setShowVertScrollbar(bool setting)
{
    //TODO: migrate the scrollbar display mode
    getDropList()->setVertScrollbarDisplayMode(setting ? ScrollbarDisplayMode::Shown : ScrollbarDisplayMode::WhenNeeded);
}

//----------------------------------------------------------------------------//
void Combobox::setShowHorzScrollbar(bool setting)
{
    //TODO: migrate the scrollbar display mode
    getDropList()->setHorzScrollbarDisplayMode(setting ? ScrollbarDisplayMode::Shown : ScrollbarDisplayMode::WhenNeeded);
}

//----------------------------------------------------------------------------//
void Combobox::handleUpdatedListItemData()
{
    getDropList()->invalidateView(false);
}

//----------------------------------------------------------------------------//
bool Combobox::isDropDownListVisible() const
{
    return getDropList()->isEffectiveVisible();
}

//----------------------------------------------------------------------------//
void Combobox::setItemSelectState(StandardItem* item, bool state)
{
    const bool wasSelected = (item && getDropList()->isItemSelected(item));
    getDropList()->setIndexSelectionState(item, state);
    itemSelectChangeTextUpdate(item, state, wasSelected);
}

//----------------------------------------------------------------------------//
void Combobox::setItemSelectState(size_t item_index, bool state)
{
    ComboDropList* droplist = getDropList();
    StandardItem* item = (droplist->getItemCount() > item_index) ?
                            droplist->getItemAtIndex(item_index) :
                            nullptr;
    setItemSelectState(item, state);
}

//----------------------------------------------------------------------------//
void Combobox::onReadOnlyChanged(WindowEventArgs& e)
{
	fireEvent(EventReadOnlyModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onValidationStringChanged(WindowEventArgs& e)
{
	fireEvent(EventValidationStringChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onMaximumTextLengthChanged(WindowEventArgs& e)
{
	fireEvent(EventMaximumTextLengthChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onTextValidityChanged(RegexMatchStateEventArgs& e)
{
	fireEvent(EventTextValidityChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onCaretMoved(WindowEventArgs& e)
{
	fireEvent(EventCaretMoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onTextSelectionChanged(WindowEventArgs& e)
{
	fireEvent(EventTextSelectionChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onEditboxFullEvent(WindowEventArgs& e)
{
	fireEvent(EventEditboxFull, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onTextAcceptedEvent(WindowEventArgs& e)
{
    selectListItemWithEditboxText();
	fireEvent(EventTextAccepted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onListContentsChanged(WindowEventArgs& e)
{
	fireEvent(EventListContentsChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onListSelectionChanged(WindowEventArgs& e)
{
	fireEvent(EventListSelectionChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onSortModeChanged(WindowEventArgs& e)
{
	fireEvent(EventSortModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onVertScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onDropListDisplayed(WindowEventArgs& e)
{
    getGUIContext().updateWindowContainingCursor();
    getPushButton()->setPushedState(true);
	fireEvent(EventDropListDisplayed, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onDroplistRemoved(WindowEventArgs& e)
{
    GUIContext* pContext = getGUIContextPtr();
    if (pContext)
        pContext->updateWindowContainingCursor();

    getPushButton()->setPushedState(false);
	fireEvent(EventDropListRemoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onListSelectionAccepted(WindowEventArgs& e)
{
	fireEvent(EventListSelectionAccepted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Combobox::onFontChanged(WindowEventArgs& e)
{
    // Propagate to children
    getEditbox()->setFont(d_font);
    getDropList()->setFont(d_font);

    // Call base class handler
    Window::onFontChanged(e);
}

//----------------------------------------------------------------------------//
void Combobox::onTextChanged(WindowEventArgs& e)
{
    Editbox* editbox = getEditbox();

	// update ourselves only if needed (prevents perpetual event loop & stack overflow)
    if (editbox->getText() != getText())
	{
		// done before doing base class processing so event subscribers see 'updated' version of this.
        editbox->setText(getText());
		++e.handled;

        selectListItemWithEditboxText();

		Window::onTextChanged(e);
	}

}

//----------------------------------------------------------------------------//
bool Combobox::button_PressHandler(const EventArgs&)
{
    selectListItemWithEditboxText();
    showDropList();
	return true;
}

//----------------------------------------------------------------------------//
void Combobox::selectListItemWithEditboxText()
{
    ComboDropList* const droplist = getDropList();
    if (StandardItem* item = droplist->findItemWithText(getEditbox()->getText(), nullptr))
    {
        droplist->setIndexSelectionState(item, true);
        droplist->ensureIndexIsVisible(item);

        // Fire off a selection event to inform subscribers
        WindowEventArgs args(this);
        onListSelectionAccepted(args);
    }
    else
    {
        droplist->clearSelections();
    }
}

//----------------------------------------------------------------------------//
void Combobox::setAutoSizeListHeightToContent(bool auto_size)
{
    d_autoSizeHeight = auto_size;

    if (d_autoSizeHeight && isDropDownListVisible())
        updateAutoSizedDropList();
}

//----------------------------------------------------------------------------//
void Combobox::setAutoSizeListWidthToContent(bool auto_size)
{
    d_autoSizeWidth = auto_size;

    if (d_autoSizeWidth && isDropDownListVisible())
        updateAutoSizedDropList();
}

//----------------------------------------------------------------------------//
void Combobox::updateAutoSizedDropList()
{
    getDropList()->resizeToContent(d_autoSizeWidth, d_autoSizeHeight);
}

//----------------------------------------------------------------------------//
bool Combobox::droplist_SelectionAcceptedHandler(const EventArgs& e)
{
	// copy the text from the selected item into the edit box
    StandardItem* item = static_cast<ComboDropList*>(
        static_cast<const WindowEventArgs&>(e).window)->getFirstSelectedItem();

	if (item)
	{
        Editbox* editbox = getEditbox();
		// Put the text from the list item into the edit box
		editbox->setText(item->getText());

		// select text if it's editable, and move caret to end
		if (!isReadOnly())
		{
			editbox->setSelection(0, item->getText().length());
			editbox->setCaretIndex(item->getText().length());
		}

		editbox->setCaretIndex(0);
		editbox->activate();

		// fire off an event of our own
		WindowEventArgs args(this);
		onListSelectionAccepted(args);
	}

	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::droplist_HiddenHandler(const EventArgs&)
{
	WindowEventArgs args(this);
	onDroplistRemoved(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_PointerPressHoldHandler(const EventArgs& e)
{
    // only interested in left source
    if (static_cast<const MouseButtonEventArgs&>(e).d_button != MouseButton::Left)
        return false;

    Editbox* editbox = getEditbox();
    if (!editbox->isReadOnly())
        return false;

    ComboDropList* droplist = getDropList();

	// if there is an item with the same text as the edit box, pre-select it
	if (auto item = droplist->findItemWithText(editbox->getText(), nullptr))
	{
        droplist->setIndexSelectionState(item, true);
		droplist->ensureIndexIsVisible(item);
	}
	else
	{
        droplist->clearSelections();
	}

    showDropList();

	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::isVertScrollbarAlwaysShown() const
{
    return getDropList()->getVertScrollbarDisplayMode() == ScrollbarDisplayMode::Shown;
}

//----------------------------------------------------------------------------//
bool Combobox::isHorzScrollbarAlwaysShown() const
{
    return getDropList()->getHorzScrollbarDisplayMode() == ScrollbarDisplayMode::Shown;
}

//----------------------------------------------------------------------------//
void Combobox::onActivated(ActivationEventArgs& e)
{
	Window::onActivated(e);

    // When receiving input focus, forward it to the editbox
    if (isFocused())
        getEditbox()->activate();
}

//----------------------------------------------------------------------------//
void Combobox::onSized(ElementEventArgs& e)
{
    if (isDropDownListVisible())
        updateAutoSizedDropList();

    Window::onSized(e);
}

//----------------------------------------------------------------------------//
void Combobox::setSingleCursorActivationEnabled(bool setting)
{
	d_singleClickOperation = setting;
	getDropList()->setAutoArmEnabled(setting);
}

//----------------------------------------------------------------------------//
Editbox* Combobox::getEditbox() const
{
    return static_cast<Editbox*>(getChild(EditboxName));
}

//----------------------------------------------------------------------------//
PushButton* Combobox::getPushButton() const
{
    return static_cast<PushButton*>(getChild(ButtonName));
}

//----------------------------------------------------------------------------//
ComboDropList* Combobox::getDropList() const
{
    return static_cast<ComboDropList*>(getChild(DropListName));
}

//----------------------------------------------------------------------------//
void Combobox::itemSelectChangeTextUpdate(const StandardItem* const item,
    bool new_state, bool old_state)
{
    if (!new_state)
    {
        if (getText() == item->getText())
            setText("");
    }
    else
    {
        if (!old_state)
            setText(item->getText());
    }
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_ReadOnlyChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onReadOnlyChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_ValidationStringChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onValidationStringChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_MaximumTextLengthChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onMaximumTextLengthChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_TextValidityChangedHandler(const EventArgs& e)
{
	RegexMatchStateEventArgs args(this, static_cast<const RegexMatchStateEventArgs&>(e).matchState);
	onTextValidityChanged(args);
	return args.handled > 0;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_CaretMovedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onCaretMoved(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_TextSelectionChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onTextSelectionChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_EditboxFullEventHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onEditboxFullEvent(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_TextAcceptedEventHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onTextAcceptedEvent(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::editbox_TextChangedEventHandler(const EventArgs& e)
{
	// set this windows text to match
	setText(static_cast<const WindowEventArgs&>(e).window->getText());
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::listwidget_ListContentsChangedHandler(const EventArgs&)
{
    if (isDropDownListVisible())
        updateAutoSizedDropList();

    WindowEventArgs args(this);
	onListContentsChanged(args);

	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::listwidget_ListSelectionChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onListSelectionChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::listwidget_SortModeChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onSortModeChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::listwidget_VertScrollModeChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onVertScrollbarModeChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Combobox::listwidget_HorzScrollModeChangedHandler(const EventArgs&)
{
	WindowEventArgs	args(this);
	onHorzScrollbarModeChanged(args);
	return true;
}

//----------------------------------------------------------------------------//
void Combobox::addComboboxProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Combobox, bool,
        "ReadOnly", "Property to get/set the read-only setting for the Editbox.  Value is either \"true\" or \"false\".",
        &Combobox::setReadOnly, &Combobox::isReadOnly, false
    );
    CEGUI_DEFINE_PROPERTY(Combobox, String,
        "ValidationString", "Property to get/set the validation string Editbox.  Value is a text string.",
        &Combobox::setValidationString, &Combobox::getValidationString, ".*"
    );
    CEGUI_DEFINE_PROPERTY(Combobox, size_t,
        "CaretIndex", "Property to get/set the current caret index.  Value is \"[uint]\".",
        &Combobox::setCaretIndex, &Combobox::getCaretIndex, 0
    );
    CEGUI_DEFINE_PROPERTY(Combobox, size_t,
        "TextSelectionStart", "Property to get/set the zero based index of the selection start position within the text.  Value is \"[uint]\".",
        &Combobox::setTextSelectionStart, &Combobox::getTextSelectionStart, 0
    );
    CEGUI_DEFINE_PROPERTY(Combobox, size_t,
        "TextSelectionLength", "Property to get/set the length of the selection (as a count of the number of code points selected).  Value is \"[uint]\".",
        &Combobox::setTextSelectionLength, &Combobox::getTextSelectionLength, 0
    );
    CEGUI_DEFINE_PROPERTY(Combobox, size_t,
        "MaxTextLength", "Property to get/set the the maximum allowed text length (as a count of code points).  Value is \"[uint]\".",
        &Combobox::setMaxTextLength, &Combobox::getMaxTextLength, String().max_size()
    );
    CEGUI_DEFINE_PROPERTY(Combobox, bool,
        "SortList", "Property to get/set the sort setting of the list widget.  Value is either \"true\" or \"false\".",
        &Combobox::setSortingEnabled, &Combobox::isSortEnabled, false /* TODO: Inconsistency between setter, getter and property name */
    );
    CEGUI_DEFINE_PROPERTY(Combobox, bool,
        "ForceVertScrollbar", "Property to get/set the 'always show' setting for the vertical scroll bar of the list widget.  Value is either \"true\" or \"false\".",
        &Combobox::setShowVertScrollbar, &Combobox::isVertScrollbarAlwaysShown, false /* TODO: Inconsistency between setter, getter and property name */
    );
    CEGUI_DEFINE_PROPERTY(Combobox, bool,
        "ForceHorzScrollbar", "Property to get/set the 'always show' setting for the horizontal scroll bar of the list widget.  Value is either \"true\" or \"false\".",
        &Combobox::setShowHorzScrollbar, &Combobox::isHorzScrollbarAlwaysShown, false /* TODO: Inconsistency between setter, getter and property name */
    );
    CEGUI_DEFINE_PROPERTY(Combobox, bool,
        "SingleCursorActivationMode", "Property to get/set the 'single activation mode' setting for the combo box.  Value is either \"true\" or \"false\".",
        &Combobox::setSingleCursorActivationEnabled, &Combobox::getSingleCursorActivationEnabled, false /* TODO: Inconsistency between setter, getter and property name */
    );
    CEGUI_DEFINE_PROPERTY(Combobox, bool,
        "AutoSizeListHeight",
        "Property to get/set whether the drop down list will vertically "
        "auto-size itself to fit it's content. "
        "Value is either \"true\" or \"false\".",
        &Combobox::setAutoSizeListHeightToContent,
        &Combobox::getAutoSizeListHeightToContent, false
    );
    CEGUI_DEFINE_PROPERTY(Combobox, bool,
        "AutoSizeListWidth",
        "Property to get/set whether the drop down list will horizontally "
        "auto-size itself to fit it's content. "
        "Value is either \"true\" or \"false\".",
        &Combobox::setAutoSizeListWidthToContent,
        &Combobox::getAutoSizeListWidthToContent, false
    );
}

}
