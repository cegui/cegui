/***********************************************************************
	filename: 	FalagardStaticTextProperties.cpp
	created:	17/9/2005
	author:		Tomas L Olsen (based on code by Paul D Turner)
	
	purpose:	Implements properties for the FalagardStaticText class
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
#include "FalStaticTextProperties.h"
#include "FalStaticText.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of FalagardStaticTextProperties namespace section
namespace FalagardStaticTextProperties
{
String	TextColours::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());
    return PropertyHelper::colourRectToString(wr->getTextColours());
}


void	TextColours::set(PropertyReceiver* receiver, const String& value)
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setTextColours(PropertyHelper::stringToColourRect(value));
}


String	HorzFormatting::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());

	switch(wr->getHorizontalFormatting())
	{
	case FalagardStaticText::RightAligned:
		return String("RightAligned");
		break;

	case FalagardStaticText::HorzCentred:
		return String("HorzCentred");
		break;

	case FalagardStaticText::HorzJustified:
		return String("HorzJustified");
		break;

	case FalagardStaticText::WordWrapLeftAligned:
		return String("WordWrapLeftAligned");
		break;

	case FalagardStaticText::WordWrapRightAligned:
		return String("WordWrapRightAligned");
		break;

	case FalagardStaticText::WordWrapCentred:
		return String("WordWrapCentred");
		break;

	case FalagardStaticText::WordWrapJustified:
		return String("WordWrapJustified");
		break;

	default:
		return String("LeftAligned");
		break;
	}
}


void	HorzFormatting::set(PropertyReceiver* receiver, const String& value)
{
	FalagardStaticText::HorzFormatting fmt;

	if (value == "RightAligned")
	{
		fmt = FalagardStaticText::RightAligned;
	}
	else if (value == "HorzCentred")
	{
		fmt = FalagardStaticText::HorzCentred;
	}
	else if (value == "HorzJustified")
	{
		fmt = FalagardStaticText::HorzJustified;
	}
	else if (value == "WordWrapLeftAligned")
	{
		fmt = FalagardStaticText::WordWrapLeftAligned;
	}
	else if (value == "WordWrapRightAligned")
	{
		fmt = FalagardStaticText::WordWrapRightAligned;
	}
	else if (value == "WordWrapCentred")
	{
		fmt = FalagardStaticText::WordWrapCentred;
	}
	else if (value == "WordWrapJustified")
	{
		fmt = FalagardStaticText::WordWrapJustified;
	}
	else
	{
		fmt = FalagardStaticText::LeftAligned;
	}

	FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setHorizontalFormatting(fmt);
}


String	VertFormatting::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());

	switch(wr->getVerticalFormatting())
	{
	case FalagardStaticText::BottomAligned:
		return String("BottomAligned");
		break;

	case FalagardStaticText::VertCentred:
		return String("VertCentred");
		break;

	default:
		return String("TopAligned");
		break;
	}
}


void	VertFormatting::set(PropertyReceiver* receiver, const String& value)
{
	FalagardStaticText::VertFormatting fmt;

	if (value == "BottomAligned")
	{
		fmt = FalagardStaticText::BottomAligned;
	}
	else if (value == "VertCentred")
	{
		fmt = FalagardStaticText::VertCentred;
	}
	else
	{
		fmt = FalagardStaticText::TopAligned;
	}

	FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setVerticalFormatting(fmt);
}


String	VertScrollbar::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());
    return PropertyHelper::boolToString(wr->isVerticalScrollbarEnabled());
}


void	VertScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setVerticalScrollbarEnabled(PropertyHelper::stringToBool(value));
}


String	HorzScrollbar::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());
    return PropertyHelper::boolToString(wr->isHorizontalScrollbarEnabled());
}


void	HorzScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setHorizontalScrollbarEnabled(PropertyHelper::stringToBool(value));
}

} // End of  FalagardStaticTextProperties namespace section

} // End of  CEGUI namespace section
