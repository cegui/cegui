/***********************************************************************
	filename: 	CEGUIScrollbarProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for the scroll bar.
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
#include "elements/CEGUIScrollbarProperties.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ScrollbarProperties namespace section
namespace ScrollbarProperties
{
String	DocumentSize::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const Scrollbar*>(receiver)->getDocumentSize());
}


void	DocumentSize::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Scrollbar*>(receiver)->setDocumentSize(PropertyHelper::stringToFloat(value));
}


String	PageSize::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const Scrollbar*>(receiver)->getPageSize());
}


void	PageSize::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Scrollbar*>(receiver)->setPageSize(PropertyHelper::stringToFloat(value));
}


String	StepSize::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const Scrollbar*>(receiver)->getStepSize());
}


void	StepSize::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Scrollbar*>(receiver)->setStepSize(PropertyHelper::stringToFloat(value));
}


String	OverlapSize::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const Scrollbar*>(receiver)->getOverlapSize());
}


void	OverlapSize::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Scrollbar*>(receiver)->setOverlapSize(PropertyHelper::stringToFloat(value));
}


String	ScrollPosition::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const Scrollbar*>(receiver)->getScrollPosition());
}


void	ScrollPosition::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Scrollbar*>(receiver)->setScrollPosition(PropertyHelper::stringToFloat(value));
}

} // End of  ScrollbarProperties namespace section

} // End of  CEGUI namespace section
