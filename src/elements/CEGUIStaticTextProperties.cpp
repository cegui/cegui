/************************************************************************
	filename: 	CEGUIStaticTextProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for the StaticText class
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
#include "elements/CEGUIStaticTextProperties.h"
#include "elements/CEGUIStaticText.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of StaticTextProperties namespace section
namespace StaticTextProperties
{
String	TextColours::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::colourRectToString(static_cast<const StaticText*>(receiver)->getTextColours());
}


void	TextColours::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<StaticText*>(receiver)->setTextColours(PropertyHelper::stringToColourRect(value));
}


String	HorzFormatting::get(const PropertyReceiver* receiver) const
{
	switch(static_cast<const StaticText*>(receiver)->getHorizontalFormatting())
	{
	case StaticText::RightAligned:
		return String("RightAligned");
		break;

	case StaticText::HorzCentred:
		return String("HorzCentred");
		break;

	case StaticText::HorzJustified:
		return String("HorzJustified");
		break;

	case StaticText::WordWrapLeftAligned:
		return String("WordWrapLeftAligned");
		break;

	case StaticText::WordWrapRightAligned:
		return String("WordWrapRightAligned");
		break;

	case StaticText::WordWrapCentred:
		return String("WordWrapCentred");
		break;

	case StaticText::WordWrapJustified:
		return String("WordWrapJustified");
		break;

	default:
		return String("LeftAligned");
		break;
	}
}


void	HorzFormatting::set(PropertyReceiver* receiver, const String& value)
{
	StaticText::HorzFormatting fmt;

	if (value == "RightAligned")
	{
		fmt = StaticText::RightAligned;
	}
	else if (value == "HorzCentred")
	{
		fmt = StaticText::HorzCentred;
	}
	else if (value == "HorzJustified")
	{
		fmt = StaticText::HorzJustified;
	}
	else if (value == "WordWrapLeftAligned")
	{
		fmt = StaticText::WordWrapLeftAligned;
	}
	else if (value == "WordWrapRightAligned")
	{
		fmt = StaticText::WordWrapRightAligned;
	}
	else if (value == "WordWrapCentred")
	{
		fmt = StaticText::WordWrapCentred;
	}
	else if (value == "WordWrapJustified")
	{
		fmt = StaticText::WordWrapJustified;
	}
	else
	{
		fmt = StaticText::LeftAligned;
	}

	static_cast<StaticText*>(receiver)->setHorizontalFormatting(fmt);
}


String	VertFormatting::get(const PropertyReceiver* receiver) const
{
	switch(static_cast<const StaticText*>(receiver)->getVerticalFormatting())
	{
	case StaticText::BottomAligned:
		return String("BottomAligned");
		break;

	case StaticText::VertCentred:
		return String("VertCentred");
		break;

	default:
		return String("TopAligned");
		break;
	}
}


void	VertFormatting::set(PropertyReceiver* receiver, const String& value)
{
	StaticText::VertFormatting fmt;

	if (value == "BottomAligned")
	{
		fmt = StaticText::BottomAligned;
	}
	else if (value == "VertCentred")
	{
		fmt = StaticText::VertCentred;
	}
	else
	{
		fmt = StaticText::TopAligned;
	}

	static_cast<StaticText*>(receiver)->setVerticalFormatting(fmt);
}


String	VertScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const StaticText*>(receiver)->isVerticalScrollbarEnabled());
}


void	VertScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<StaticText*>(receiver)->setVerticalScrollbarEnabled(PropertyHelper::stringToBool(value));
}

String	HorzScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const StaticText*>(receiver)->isHorizontalScrollbarEnabled());
}


void	HorzScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<StaticText*>(receiver)->setHorizontalScrollbarEnabled(PropertyHelper::stringToBool(value));
}

} // End of  StaticTextProperties namespace section

} // End of  CEGUI namespace section
