/***********************************************************************
	filename: 	CEGUIComboboxProperties.cpp
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Combobox properties classes
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
#include "elements/CEGUIComboboxProperties.h"
#include "elements/CEGUICombobox.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ComboboxProperties namespace section
namespace ComboboxProperties
{
String	ReadOnly::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isReadOnly());
}


void	ReadOnly::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setReadOnly(PropertyHelper::stringToBool(value));
}


String	ValidationString::get(const PropertyReceiver* receiver) const
{
	return static_cast<const Combobox*>(receiver)->getValidationString();
}


void	ValidationString::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setValidationString(value);
}


String	CaratIndex::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Combobox*>(receiver)->getCaratIndex()));
}


void	CaratIndex::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setCaratIndex(PropertyHelper::stringToUint(value));
}


String	EditSelectionStart::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Combobox*>(receiver)->getSelectionStartIndex()));
}


void	EditSelectionStart::set(PropertyReceiver* receiver, const String& value)
{
	Combobox* eb = static_cast<Combobox*>(receiver);
	uint selStart = PropertyHelper::stringToUint(value);
	eb->setSelection(selStart, selStart + eb->getSelectionLength());
}


String	EditSelectionLength::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Combobox*>(receiver)->getSelectionLength()));
}


void	EditSelectionLength::set(PropertyReceiver* receiver, const String& value)
{
	Combobox* eb = static_cast<Combobox*>(receiver);
	uint selLen = PropertyHelper::stringToUint(value);
	eb->setSelection(eb->getSelectionStartIndex(), eb->getSelectionStartIndex() + selLen);
}


String	MaxEditTextLength::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Combobox*>(receiver)->getMaxTextLength()));
}


void	MaxEditTextLength::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setMaxTextLength(PropertyHelper::stringToUint(value));
}


String	SortList::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isSortEnabled());
}


void	SortList::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setSortingEnabled(PropertyHelper::stringToBool(value));
}


String	ForceVertScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isVertScrollbarAlwaysShown());
}


void	ForceVertScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setShowVertScrollbar(PropertyHelper::stringToBool(value));
}


String	ForceHorzScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isHorzScrollbarAlwaysShown());
}


void	ForceHorzScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setShowHorzScrollbar(PropertyHelper::stringToBool(value));
}


String	SingleClickMode::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->getSingleClickEnabled());
}


void	SingleClickMode::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setSingleClickEnabled(PropertyHelper::stringToBool(value));
}


} // End of  ComboboxProperties namespace section

} // End of  CEGUI namespace section
