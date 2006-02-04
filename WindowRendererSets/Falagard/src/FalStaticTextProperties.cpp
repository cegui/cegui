/************************************************************************
	filename: 	FalagardStaticTextProperties.cpp
	created:	17/9/2005
	author:		Tomas L Olsen (based on code by Paul D Turner)
	
	purpose:	Implements properties for the FalagardStaticText class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
