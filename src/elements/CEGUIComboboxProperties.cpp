/************************************************************************
	filename: 	CEGUIComboboxProperties.cpp
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Combobox properties classes
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
#include "elements/CEGUIComboboxProperties.h"
#include "elements/CEGUICombobox.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ComboboxProperties namespace section
namespace ComboboxProperties
{
String	ReadOnly::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isReadOnly());
}


void	ReadOnly::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setReadOnly(PropertyHelper::stringToBool(value));
}


String	ValidationString::get(const void* receiver) const
{
	return static_cast<const Combobox*>(receiver)->getValidationString();
}


void	ValidationString::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setValidationString(value);
}


String	CaratIndex::get(const void* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const Combobox*>(receiver)->getCaratIndex());
}


void	CaratIndex::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setCaratIndex(PropertyHelper::stringToUint(value));
}


String	EditSelectionStart::get(const void* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const Combobox*>(receiver)->getSelectionStartIndex());
}


void	EditSelectionStart::set(void* receiver, const String& value)
{
	Combobox* eb = static_cast<Combobox*>(receiver);
	uint selStart = PropertyHelper::stringToUint(value);
	eb->setSelection(selStart, selStart + eb->getSelectionLength());
}


String	EditSelectionLength::get(const void* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const Combobox*>(receiver)->getSelectionLength());
}


void	EditSelectionLength::set(void* receiver, const String& value)
{
	Combobox* eb = static_cast<Combobox*>(receiver);
	uint selLen = PropertyHelper::stringToUint(value);
	eb->setSelection(eb->getSelectionStartIndex(), eb->getSelectionStartIndex() + selLen);
}


String	MaxEditTextLength::get(const void* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const Combobox*>(receiver)->getMaxTextLength());
}


void	MaxEditTextLength::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setMaxTextLength(PropertyHelper::stringToUint(value));
}


String	NormalEditTextColour::get(const void* receiver) const
{
	return PropertyHelper::colourToString(static_cast<const Combobox*>(receiver)->getNormalTextColour());
}


void	NormalEditTextColour::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setNormalTextColour(PropertyHelper::stringToColour(value));
}


String	SelectedEditTextColour::get(const void* receiver) const
{
	return PropertyHelper::colourToString(static_cast<const Combobox*>(receiver)->getSelectedTextColour());
}


void	SelectedEditTextColour::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setSelectedTextColour(PropertyHelper::stringToColour(value));
}


String	ActiveEditSelectionColour::get(const void* receiver) const
{
	return PropertyHelper::colourToString(static_cast<const Combobox*>(receiver)->getNormalSelectBrushColour());
}


void	ActiveEditSelectionColour::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setNormalSelectBrushColour(PropertyHelper::stringToColour(value));
}


String	InactiveEditSelectionColour::get(const void* receiver) const
{
	return PropertyHelper::colourToString(static_cast<const Combobox*>(receiver)->getInactiveSelectBrushColour());
}


void	InactiveEditSelectionColour::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setInactiveSelectBrushColour(PropertyHelper::stringToColour(value));
}


String	SortList::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isSortEnabled());
}


void	SortList::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setSortingEnabled(PropertyHelper::stringToBool(value));
}


String	ForceVertScrollbar::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isVertScrollbarAlwaysShown());
}


void	ForceVertScrollbar::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setShowVertScrollbar(PropertyHelper::stringToBool(value));
}


String	ForceHorzScrollbar::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Combobox*>(receiver)->isHorzScrollbarAlwaysShown());
}


void	ForceHorzScrollbar::set(void* receiver, const String& value)
{
	static_cast<Combobox*>(receiver)->setShowHorzScrollbar(PropertyHelper::stringToBool(value));
}


} // End of  ComboboxProperties namespace section

} // End of  CEGUI namespace section
