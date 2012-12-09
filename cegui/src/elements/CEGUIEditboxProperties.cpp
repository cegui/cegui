/***********************************************************************
	filename: 	CEGUIEditboxProperties.cpp
	created:	9/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for the Editbox class
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
#include "elements/CEGUIEditboxProperties.h"
#include "elements/CEGUIEditbox.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIExceptions.h"
#include <cstdlib>
#include <cstdio>


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of EditboxProperties namespace section
namespace EditboxProperties
{

String ReadOnly::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Editbox*>(receiver)->isReadOnly());
}


void ReadOnly::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Editbox*>(receiver)->setReadOnly(PropertyHelper::stringToBool(value));
}


String MaskText::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Editbox*>(receiver)->isTextMasked());
}


void MaskText::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Editbox*>(receiver)->setTextMasked(PropertyHelper::stringToBool(value));
}


String MaskCodepoint::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const Editbox*>(receiver)->getMaskCodePoint());
}


void MaskCodepoint::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Editbox*>(receiver)->setMaskCodePoint(PropertyHelper::stringToUint(value));
}


String ValidationString::get(const PropertyReceiver* receiver) const
{
	return static_cast<const Editbox*>(receiver)->getValidationString();
}


void ValidationString::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Editbox*>(receiver)->setValidationString(value);
}


String CaratIndex::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Editbox*>(receiver)->getCaratIndex()));
}


void CaratIndex::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Editbox*>(receiver)->setCaratIndex(PropertyHelper::stringToUint(value));
}


String SelectionStart::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Editbox*>(receiver)->getSelectionStartIndex()));
}


void SelectionStart::set(PropertyReceiver* receiver, const String& value)
{
	Editbox* eb = static_cast<Editbox*>(receiver);
	uint selStart = PropertyHelper::stringToUint(value);
	eb->setSelection(selStart, selStart + eb->getSelectionLength());
}


String SelectionLength::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Editbox*>(receiver)->getSelectionLength()));
}


void SelectionLength::set(PropertyReceiver* receiver, const String& value)
{
	Editbox* eb = static_cast<Editbox*>(receiver);
	uint selLen = PropertyHelper::stringToUint(value);
	eb->setSelection(eb->getSelectionStartIndex(), eb->getSelectionStartIndex() + selLen);
}


String MaxTextLength::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const Editbox*>(receiver)->getMaxTextLength()));
}


void MaxTextLength::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Editbox*>(receiver)->setMaxTextLength(PropertyHelper::stringToUint(value));
}

} // End of  EditboxProperties namespace section

} // End of  CEGUI namespace section
