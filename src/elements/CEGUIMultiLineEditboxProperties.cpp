/***********************************************************************
	filename: 	CEGUIMultiLineEditboxProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for the MultiLineEditbox class
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
#include "elements/CEGUIMultiLineEditboxProperties.h"
#include "elements/CEGUIMultiLineEditbox.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIExceptions.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of MultiLineEditboxProperties namespace section
namespace MultiLineEditboxProperties
{
String ReadOnly::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiLineEditbox*>(receiver)->isReadOnly());
}


void ReadOnly::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiLineEditbox*>(receiver)->setReadOnly(PropertyHelper::stringToBool(value));
}


String WordWrap::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiLineEditbox*>(receiver)->isWordWrapped());
}


void WordWrap::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiLineEditbox*>(receiver)->setWordWrapping(PropertyHelper::stringToBool(value));
}


String CaratIndex::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const MultiLineEditbox*>(receiver)->getCaratIndex()));
}


void CaratIndex::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiLineEditbox*>(receiver)->setCaratIndex(PropertyHelper::stringToUint(value));
}


String SelectionStart::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const MultiLineEditbox*>(receiver)->getSelectionStartIndex()));
}


void SelectionStart::set(PropertyReceiver* receiver, const String& value)
{
	MultiLineEditbox* eb = static_cast<MultiLineEditbox*>(receiver);
	uint selStart = PropertyHelper::stringToUint(value);
	eb->setSelection(selStart, selStart + eb->getSelectionLength());
}


String SelectionLength::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const MultiLineEditbox*>(receiver)->getSelectionLength()));
}


void SelectionLength::set(PropertyReceiver* receiver, const String& value)
{
	MultiLineEditbox* eb = static_cast<MultiLineEditbox*>(receiver);
	uint selLen = PropertyHelper::stringToUint(value);
	eb->setSelection(eb->getSelectionStartIndex(), eb->getSelectionStartIndex() + selLen);
}


String MaxTextLength::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<uint>(static_cast<const MultiLineEditbox*>(receiver)->getMaxTextLength()));
}


void MaxTextLength::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiLineEditbox*>(receiver)->setMaxTextLength(PropertyHelper::stringToUint(value));
}


String SelectionBrushImage::get(const PropertyReceiver* receiver) const
{
    const Image* img = static_cast<const MultiLineEditbox*>(receiver)->getSelectionBrushImage();
    return img ? PropertyHelper::imageToString(img) : String("");
}

void SelectionBrushImage::set(PropertyReceiver* receiver, const String &value)
{
    static_cast<MultiLineEditbox*>(receiver)->setSelectionBrushImage(PropertyHelper::stringToImage(value));
}

String	ForceVertScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const MultiLineEditbox*>(receiver)->isVertScrollbarAlwaysShown());
}

void	ForceVertScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<MultiLineEditbox*>(receiver)->setShowVertScrollbar(PropertyHelper::stringToBool(value));
}

} // End of  MultiLineEditboxProperties namespace section

} // End of  CEGUI namespace section
