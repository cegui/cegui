/************************************************************************
	filename: 	CEGUICombobox.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Combobox base class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "elements/CEGUICombobox.h"
#include "elements/CEGUIEditbox.h"
#include "elements/CEGUIPushButton.h"
#include "elements/CEGUIComboDropList.h"
#include "elements/CEGUIListboxItem.h"

#include <boost/bind.hpp>


/*************************************************************************
	General information.

	The Combobox class is, for the most part, a huge proxy to the
	component Editbox and ComboDropList (Listbox) widgets.  The Combobox
	widget itself actually does very little.
*************************************************************************/

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Definitions for Properties for this class
*************************************************************************/
ComboboxProperties::ReadOnly					Combobox::d_readOnlyProperty;
ComboboxProperties::ValidationString			Combobox::d_validationStringProperty;
ComboboxProperties::CaratIndex					Combobox::d_caratIndexProperty;
ComboboxProperties::EditSelectionStart			Combobox::d_selStartProperty;
ComboboxProperties::EditSelectionLength			Combobox::d_selLengthProperty;
ComboboxProperties::MaxEditTextLength			Combobox::d_maxTextLengthProperty;
ComboboxProperties::NormalEditTextColour		Combobox::d_normalTextColourProperty;
ComboboxProperties::SelectedEditTextColour		Combobox::d_selectedTextColourProperty;
ComboboxProperties::ActiveEditSelectionColour	Combobox::d_activeSelectionColourProperty;
ComboboxProperties::InactiveEditSelectionColour	Combobox::d_inactiveSelectionColourProperty;
ComboboxProperties::SortList					Combobox::d_sortProperty;
ComboboxProperties::ForceVertScrollbar			Combobox::d_forceVertProperty;
ComboboxProperties::ForceHorzScrollbar			Combobox::d_forceHorzProperty;


/*************************************************************************
	Constants
*************************************************************************/
// event names from edit box
const utf8	Combobox::ReadOnlyChanged[]				= "ReadOnlyChanged";
const utf8	Combobox::ValidationStringChanged[]		= "ValidationStringChanged";
const utf8	Combobox::MaximumTextLengthChanged[]	= "MaximumTextLengthChanged";
const utf8	Combobox::TextInvalidatedEvent[]		= "TextInvalidatedEvent";
const utf8	Combobox::InvalidEntryAttempted[]		= "InvalidEntryAttempted";
const utf8	Combobox::CaratMoved[]					= "CaratMoved";
const utf8	Combobox::TextSelectionChanged[]		= "TextSelectionChanged";
const utf8	Combobox::EditboxFullEvent[]			= "EditboxFullEvent";
const utf8	Combobox::TextAcceptedEvent[]			= "TextAcceptedEvent";

// event names from list box
const utf8	Combobox::ListContentsChanged[]			= "ListContentsChanged";
const utf8	Combobox::ListSelectionChanged[]		= "ListSelectionChanged";
const utf8	Combobox::SortModeChanged[]				= "SortModeChanged";
const utf8	Combobox::VertScrollbarModeChanged[]	= "VertScrollbarModeChanged";
const utf8	Combobox::HorzScrollbarModeChanged[]	= "HorzScrollbarModeChanged";

// events we produce / generate ourselves
const utf8	Combobox::DropListDisplayed[]			= "DropListDisplayed";
const utf8	Combobox::DropListRemoved[]				= "DropListRemoved";
const utf8	Combobox::ListSelectionAccepted[]		= "ListSelectionAccepted";

	
/*************************************************************************
	Constructor for Combobox base class
*************************************************************************/
Combobox::Combobox(const String& type, const String& name) :
	Window(type, name)
{
	addComboboxEvents();
	addComboboxProperties();
}


/*************************************************************************
	Destructor for Combobox base class
*************************************************************************/
Combobox::~Combobox(void)
{
}


/*************************************************************************
	Initialise the Window based object ready for use.
*************************************************************************/
void Combobox::initialise(void)
{
	d_editbox	= createEditbox();
	d_droplist	= createDropList();
	d_button	= createPushButton();
    d_droplist->setFont(getFont());
    d_editbox->setFont(getFont());

	addChildWindow(d_editbox);
	addChildWindow(d_droplist);
	addChildWindow(d_button);

	// internal event wiring
	d_button->subscribeEvent(PushButton::Clicked, boost::bind(&CEGUI::Combobox::button_ClickHandler, this, _1));
	d_droplist->subscribeEvent(ComboDropList::ListSelectionAccepted, boost::bind(&CEGUI::Combobox::droplist_SelectionAcceptedHandler, this, _1));
	d_droplist->subscribeEvent(Window::HiddenEvent, boost::bind(&CEGUI::Combobox::droplist_HiddenHandler, this, _1));

	// event forwarding setup
	d_editbox->subscribeEvent(Editbox::ReadOnlyChanged, boost::bind(&CEGUI::Combobox::editbox_ReadOnlyChangedHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::ValidationStringChanged, boost::bind(&CEGUI::Combobox::editbox_ValidationStringChangedHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::MaximumTextLengthChanged, boost::bind(&CEGUI::Combobox::editbox_MaximumTextLengthChangedHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::TextInvalidatedEvent, boost::bind(&CEGUI::Combobox::editbox_TextInvalidatedEventHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::InvalidEntryAttempted, boost::bind(&CEGUI::Combobox::editbox_InvalidEntryAttemptedHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::CaratMoved, boost::bind(&CEGUI::Combobox::editbox_CaratMovedHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::TextSelectionChanged, boost::bind(&CEGUI::Combobox::editbox_TextSelectionChangedHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::EditboxFullEvent, boost::bind(&CEGUI::Combobox::editbox_EditboxFullEventHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::TextAcceptedEvent, boost::bind(&CEGUI::Combobox::editbox_TextAcceptedEventHandler, this, _1));
	d_editbox->subscribeEvent(Editbox::TextChangedEvent, boost::bind(&CEGUI::Combobox::editbox_TextChangedEventHandler, this, _1));
	d_droplist->subscribeEvent(Listbox::ListContentsChanged, boost::bind(&CEGUI::Combobox::listbox_ListContentsChangedHandler, this, _1));
	d_droplist->subscribeEvent(Listbox::SelectionChanged, boost::bind(&CEGUI::Combobox::listbox_ListSelectionChangedHandler, this, _1));
	d_droplist->subscribeEvent(Listbox::SortModeChanged, boost::bind(&CEGUI::Combobox::listbox_SortModeChangedHandler, this, _1));
	d_droplist->subscribeEvent(Listbox::VertScrollbarModeChanged, boost::bind(&CEGUI::Combobox::listbox_VertScrollModeChangedHandler, this, _1));
	d_droplist->subscribeEvent(Listbox::HorzScrollbarModeChanged, boost::bind(&CEGUI::Combobox::listbox_HorzScrollModeChangedHandler, this, _1));

	// put components in their initial positions
	layoutComponentWidgets();
}


/*************************************************************************
	Show the drop-down list
*************************************************************************/
void Combobox::showDropList(void)
{
	// Display the box
	d_droplist->show();
	d_droplist->activate();
	d_droplist->captureInput();

	// Fire off event
	WindowEventArgs args(this);
	onDropListDisplayed(args);
}


/*************************************************************************
	Hide the drop-down list
*************************************************************************/
void Combobox::hideDropList(void)
{
	// the natural order of things when this happens will ensure the list is
	// hidden and events are fired.
	d_droplist->releaseInput();
}


/*************************************************************************
	return true if the Editbox has input focus.
*************************************************************************/
bool Combobox::hasInputFocus(void) const
{
	return d_editbox->hasInputFocus();
}


/*************************************************************************
	return true if the Editbox is read-only.
*************************************************************************/
bool Combobox::isReadOnly(void) const
{
	return d_editbox->isReadOnly();
}


/*************************************************************************
	return true if the Editbox text is valid given the currently set
	validation string.
*************************************************************************/
bool Combobox::isTextValid(void) const
{
	return d_editbox->isTextValid();
}


/*************************************************************************
	return the currently set validation string
*************************************************************************/
const String& Combobox::getValidationString(void) const
{
	return d_editbox->getValidationString();
}


/*************************************************************************
	return the current position of the carat.
*************************************************************************/
ulong Combobox::getCaratIndex(void) const
{
	return d_editbox->getCaratIndex();
}


/*************************************************************************
	return the current selection start point.
*************************************************************************/
ulong Combobox::getSelectionStartIndex(void) const
{
	return d_editbox->getSelectionStartIndex();
}


/*************************************************************************
	return the current selection end point.
*************************************************************************/
ulong Combobox::getSelectionEndIndex(void) const
{
	return d_editbox->getSelectionEndIndex();
}


/*************************************************************************
	return the length of the current selection (in code points / characters).
*************************************************************************/
ulong Combobox::getSelectionLength(void) const
{
	return d_editbox->getSelectionLength();
}


/*************************************************************************
	return the maximum text length set for this Editbox.
*************************************************************************/
ulong Combobox::getMaxTextLength(void) const
{
	return d_editbox->getMaxTextLength();
}


/*************************************************************************
	return the currently set colour to be used for rendering Editbox text
	in the normal, unselected state.
*************************************************************************/
colour Combobox::getNormalTextColour(void) const	
{
	return d_editbox->getNormalTextColour();
}


/*************************************************************************
	return the currently set colour to be used for rendering the Editbox
	text when within the selected region.
*************************************************************************/
colour Combobox::getSelectedTextColour(void) const
{
	return d_editbox->getSelectedTextColour();
}


/*************************************************************************
	return the currently set colour to be used for rendering the Editbox
	selection highlight when the Editbox is active.
*************************************************************************/
colour Combobox::getNormalSelectBrushColour(void) const
{
	return d_editbox->getNormalSelectBrushColour();
}


/*************************************************************************
	return the currently set colour to be used for rendering the Editbox
	selection highlight when the Editbox is inactive.
*************************************************************************/
colour Combobox::getInactiveSelectBrushColour(void) const
{
	return d_editbox->getInactiveSelectBrushColour();
}


/*************************************************************************
	Specify whether the Editbox is read-only.
*************************************************************************/
void Combobox::setReadOnly(bool setting)
{
	d_editbox->setReadOnly(setting);
}


/*************************************************************************
	Set the text validation string.
*************************************************************************/
void Combobox::setValidationString(const String& validation_string)
{
	d_editbox->setValidationString(validation_string);
}


/*************************************************************************
	Set the current position of the carat.
*************************************************************************/
void Combobox::setCaratIndex(ulong carat_pos)
{
	d_editbox->setCaratIndex(carat_pos);
}


/*************************************************************************
	Define the current selection for the Editbox
*************************************************************************/
void Combobox::setSelection(ulong start_pos, ulong end_pos)
{
	d_editbox->setSelection(start_pos, end_pos);
}


/*************************************************************************
	set the maximum text length for this Editbox.
*************************************************************************/
void Combobox::setMaxTextLength(ulong max_len)
{
	d_editbox->setMaxTextLength(max_len);
}


/*************************************************************************
	Set the colour to be used for rendering Editbox text in the normal,
	unselected state.
*************************************************************************/
void Combobox::setNormalTextColour(colour col)
{
	d_editbox->setNormalTextColour(col);
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox text when within
	the selected region.	
*************************************************************************/
void Combobox::setSelectedTextColour(colour col)
{
	d_editbox->setSelectedTextColour(col);
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox selection
	highlight when the Editbox is active.
*************************************************************************/
void Combobox::setNormalSelectBrushColour(colour col)
{
	d_editbox->setNormalSelectBrushColour(col);
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox selection
	highlight when the Editbox is inactive.
*************************************************************************/
void Combobox::setInactiveSelectBrushColour(colour col)
{
	d_editbox->setInactiveSelectBrushColour(col);
}


/*************************************************************************
	Return number of items attached to the list box	
*************************************************************************/
uint Combobox::getItemCount(void) const
{
	return d_droplist->getItemCount();
}


/*************************************************************************
	Return a pointer to the currently selected item.	
*************************************************************************/
ListboxItem* Combobox::getSelectedItem(void) const
{
	return d_droplist->getFirstSelectedItem();
}


/*************************************************************************
	Return the item at index position \a index.
*************************************************************************/
ListboxItem* Combobox::getListboxItemFromIndex(uint index) const
{
	return d_droplist->getListboxItemFromIndex(index);
}


/*************************************************************************
	Return the index of ListboxItem 'item'
*************************************************************************/
uint Combobox::getItemIndex(const ListboxItem* item) const
{
	return d_droplist->getItemIndex(item);
}


/*************************************************************************
	return whether list sorting is enabled	
*************************************************************************/
bool Combobox::isSortEnabled(void) const
{
	return d_droplist->isSortEnabled();
}


/*************************************************************************
	return whether the string at index position 'index' is selected	
*************************************************************************/
bool Combobox::isItemSelected(uint index) const
{
	return d_droplist->isItemSelected(index);
}


/*************************************************************************
	Search the list for an item with the specified text	
*************************************************************************/
ListboxItem* Combobox::findItemWithText(const String& text, const ListboxItem* start_item)
{
	return d_droplist->findItemWithText(text, start_item);
}


/*************************************************************************
	Return whether the specified ListboxItem is in the List	
*************************************************************************/
bool Combobox::isListboxItemInList(const ListboxItem* item) const
{
	return d_droplist->isListboxItemInList(item);
}


/*************************************************************************
	Remove all items from the list.	
*************************************************************************/
void Combobox::resetList(void)
{
	d_droplist->resetList();
}


/*************************************************************************
	Add the given ListboxItem to the list.	
*************************************************************************/
void Combobox::addItem(ListboxItem* item)
{
	d_droplist->addItem(item);
}


/*************************************************************************
	Insert an item into the list box after a specified item already in
	the list.	
*************************************************************************/
void Combobox::insertItem(ListboxItem* item, const ListboxItem* position)
{
	d_droplist->insertItem(item, position);
}


/*************************************************************************
	Removes the given item from the list box.	
*************************************************************************/
void Combobox::removeItem(const ListboxItem* item)
{
	d_droplist->removeItem(item);
}


/*************************************************************************
	Clear the selected state for all items.	
*************************************************************************/
void Combobox::clearAllSelections(void)
{
	d_droplist->clearAllSelections();
}


/*************************************************************************
	Set whether the list should be sorted.	
*************************************************************************/
void Combobox::setSortingEnabled(bool setting)
{
	d_droplist->setSortingEnabled(setting);
}


/*************************************************************************
	Set whether the vertical scroll bar should always be shown.	
*************************************************************************/
void Combobox::setShowVertScrollbar(bool setting)
{
	d_droplist->setShowVertScrollbar(setting);
}


/*************************************************************************
	Set whether the horizontal scroll bar should always be shown.	
*************************************************************************/
void Combobox::setShowHorzScrollbar(bool setting)
{
	d_droplist->setShowHorzScrollbar(setting);
}


/*************************************************************************
	Set the select state of an attached ListboxItem.	
*************************************************************************/
void Combobox::setItemSelectState(ListboxItem* item, bool state)
{
	d_droplist->setItemSelectState(item, state);
}


/*************************************************************************
	Set the select state of an attached ListboxItem.	
*************************************************************************/
void Combobox::setItemSelectState(uint item_index, bool state)
{
	d_droplist->setItemSelectState(item_index, state);
}


/*************************************************************************
	Causes the list box to update it's internal state after changes have
	been made to one or more attached ListboxItem objects.
*************************************************************************/
void Combobox::handleUpdatedListItemData(void)
{
	d_droplist->handleUpdatedItemData();
}


/*************************************************************************
	Add combo box specific events
*************************************************************************/
void Combobox::addComboboxEvents(void)
{
	addEvent(ReadOnlyChanged);				addEvent(ValidationStringChanged);
	addEvent(MaximumTextLengthChanged);		addEvent(TextInvalidatedEvent);
	addEvent(InvalidEntryAttempted);		addEvent(CaratMoved);
	addEvent(TextSelectionChanged);			addEvent(EditboxFullEvent);
	addEvent(TextAcceptedEvent);			addEvent(ListContentsChanged);
	addEvent(ListSelectionChanged);			addEvent(SortModeChanged);
	addEvent(VertScrollbarModeChanged);		addEvent(HorzScrollbarModeChanged);
	addEvent(DropListDisplayed);			addEvent(DropListRemoved);
	addEvent(ListSelectionAccepted);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onReadOnlyChanged(WindowEventArgs& e)
{
	fireEvent(ReadOnlyChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onValidationStringChanged(WindowEventArgs& e)
{
	fireEvent(ValidationStringChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onMaximumTextLengthChanged(WindowEventArgs& e)
{
	fireEvent(MaximumTextLengthChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onTextInvalidatedEvent(WindowEventArgs& e)
{
	fireEvent(TextInvalidatedEvent, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onInvalidEntryAttempted(WindowEventArgs& e)
{
	fireEvent(InvalidEntryAttempted, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onCaratMoved(WindowEventArgs& e)
{
	fireEvent(CaratMoved, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onTextSelectionChanged(WindowEventArgs& e)
{
	fireEvent(TextSelectionChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onEditboxFullEvent(WindowEventArgs& e)
{
	fireEvent(EditboxFullEvent, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onTextAcceptedEvent(WindowEventArgs& e)
{
	fireEvent(TextAcceptedEvent, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onListContentsChanged(WindowEventArgs& e)
{
	fireEvent(ListContentsChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onListSelectionChanged(WindowEventArgs& e)
{
	fireEvent(ListSelectionChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onSortModeChanged(WindowEventArgs& e)
{
	fireEvent(SortModeChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onVertScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(VertScrollbarModeChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(HorzScrollbarModeChanged, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onDropListDisplayed(WindowEventArgs& e)
{
	fireEvent(DropListDisplayed, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onDroplistRemoved(WindowEventArgs& e)
{
	fireEvent(DropListRemoved, e);
}


/*************************************************************************
	Handler for when 
*************************************************************************/
void Combobox::onListSelectionAccepted(WindowEventArgs& e)
{
	fireEvent(ListSelectionAccepted, e);
}


/*************************************************************************
	Handler for when widget is sized
*************************************************************************/
void Combobox::onSized(WindowEventArgs& e)
{
	Window::onSized(e);

	layoutComponentWidgets();

	e.handled = true;
}
/*************************************************************************
Handler for when widget font is changed
*************************************************************************/
void Combobox::onFontChanged(WindowEventArgs& e)
{
    // Propagate to children
    d_editbox->setFont(getFont());
    d_droplist->setFont(getFont());
}

/*************************************************************************
	Handler for when text changes
*************************************************************************/
void Combobox::onTextChanged(WindowEventArgs& e)
{
	// update ourselves only if needed (prevents perpetual event loop & stack overflow)
	if (d_editbox->getText() != d_text)
	{
		// done before doing base class processing so event subscribers see 'updated' version of this.
		d_editbox->setText(d_text);
		e.handled = true;

		Window::onTextChanged(e);
	}

}



/*************************************************************************
	Handler function for button clicks.
*************************************************************************/
void Combobox::button_ClickHandler(const EventArgs& e)
{
	showDropList();

	// if there is an item with the same text as the edit box, pre-select it
	ListboxItem* item = d_droplist->findItemWithText(d_editbox->getText(), NULL);

	if (item != NULL)
	{
		d_droplist->setItemSelectState(item, true);
		d_droplist->ensureItemIsVisible(item);
	}

}


/*************************************************************************
	Handler for selections made in the drop-list
*************************************************************************/
void Combobox::droplist_SelectionAcceptedHandler(const EventArgs& e)
{
	// copy the text from the selected item into the edit box
	ListboxItem* item = ((ComboDropList*)((WindowEventArgs&)e).window)->getFirstSelectedItem();

	if (item != NULL)
	{
		// Put the text from the list item into the edit box
		d_editbox->setText(item->getText());

		// select text if it's editable, and move carat to end
		if (!isReadOnly())
		{
			d_editbox->setSelection(0, item->getText().length());
			d_editbox->setCaratIndex(item->getText().length());
		}

		d_editbox->setCaratIndex(0);

		// fire off an event of our own
		WindowEventArgs args(this);
		onListSelectionAccepted(args);

		// finally, activate the edit box
		d_editbox->activate();
	}

}


/*************************************************************************
	Handler for when drop-list hides itself
*************************************************************************/
void Combobox::droplist_HiddenHandler(const EventArgs& e)
{
	WindowEventArgs args(this);
	onDroplistRemoved(args);
}


/*************************************************************************
	Return whether the vertical scroll bar is always shown.	
*************************************************************************/
bool Combobox::isVertScrollbarAlwaysShown(void) const
{
	return d_droplist->isVertScrollbarAlwaysShown();
}


/*************************************************************************
	Return whether the horizontal scroll bar is always shown.	
*************************************************************************/
bool Combobox::isHorzScrollbarAlwaysShown(void) const
{
	return d_droplist->isHorzScrollbarAlwaysShown();
}


/*************************************************************************
	Add properties for this class
*************************************************************************/
void Combobox::addComboboxProperties(void)
{
	addProperty(&d_sortProperty);
	addProperty(&d_forceHorzProperty);
	addProperty(&d_forceVertProperty);
	addProperty(&d_readOnlyProperty);
	addProperty(&d_validationStringProperty);
	addProperty(&d_maxTextLengthProperty);
	addProperty(&d_selStartProperty);
	addProperty(&d_selLengthProperty);
	addProperty(&d_normalTextColourProperty);
	addProperty(&d_selectedTextColourProperty);
	addProperty(&d_activeSelectionColourProperty);
	addProperty(&d_inactiveSelectionColourProperty);
	addProperty(&d_caratIndexProperty);
}


/*************************************************************************
	Activate the edit box component of the Combobox.
*************************************************************************/
void Combobox::activateEditbox(void)
{
	if (!d_editbox->isActive())
	{
		d_editbox->activate();
	}
}



/*************************************************************************
	Widget activation handler
*************************************************************************/
void Combobox::onActivated(WindowEventArgs& e)
{
	if (!isActive())
	{
		Window::onActivated(e);
		activateEditbox();
	}

}



//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Handlers to relay child widget events so they appear to come from us
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
void Combobox::editbox_ReadOnlyChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onReadOnlyChanged(args);
}


void Combobox::editbox_ValidationStringChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onValidationStringChanged(args);
}


void Combobox::editbox_MaximumTextLengthChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onMaximumTextLengthChanged(args);
}


void Combobox::editbox_TextInvalidatedEventHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onTextInvalidatedEvent(args);
}


void Combobox::editbox_InvalidEntryAttemptedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onInvalidEntryAttempted(args);
}


void Combobox::editbox_CaratMovedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onCaratMoved(args);
}


void Combobox::editbox_TextSelectionChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onTextSelectionChanged(args);
}


void Combobox::editbox_EditboxFullEventHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onEditboxFullEvent(args);
}


void Combobox::editbox_TextAcceptedEventHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onTextAcceptedEvent(args);
}


void Combobox::editbox_TextChangedEventHandler(const EventArgs& e)
{
	// set this windows text to match
	setText(((const WindowEventArgs&)e).window->getText());
}


void Combobox::listbox_ListContentsChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onListContentsChanged(args);
}


void Combobox::listbox_ListSelectionChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onListSelectionChanged(args);
}


void Combobox::listbox_SortModeChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onSortModeChanged(args);
}


void Combobox::listbox_VertScrollModeChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onVertScrollbarModeChanged(args);
}


void Combobox::listbox_HorzScrollModeChangedHandler(const EventArgs& e)
{
	WindowEventArgs	args(this);
	onHorzScrollbarModeChanged(args);
}

} // End of  CEGUI namespace section
