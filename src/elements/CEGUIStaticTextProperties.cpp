/************************************************************************
	filename: 	CEGUIStaticTextProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for the StaticText class
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
#include "elements/CEGUIStaticTextProperties.h"
#include "elements/CEGUIStaticText.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of StaticTextProperties namespace section
namespace StaticTextProperties
{
String	TextColours::get(const void* receiver) const
{
	return PropertyHelper::colourRectToString(static_cast<const StaticText*>(receiver)->getTextColours());
}


void	TextColours::set(void* receiver, const String& value)
{
	static_cast<StaticText*>(receiver)->setTextColours(PropertyHelper::stringToColourRect(value));
}


String	HorzFormatting::get(const void* receiver) const
{
	switch(static_cast<const StaticText*>(receiver)->getHorizontalFormatting())
	{
	case StaticText::RightAligned:
		return String((utf8*)"RightAligned");
		break;

	case StaticText::HorzCentred:
		return String((utf8*)"HorzCentred");
		break;

	case StaticText::WordWrapLeftAligned:
		return String((utf8*)"WordWrapLeftAligned");
		break;

	case StaticText::WordWrapRightAligned:
		return String((utf8*)"WordWrapRightAligned");
		break;

	case StaticText::WordWrapCentred:
		return String((utf8*)"WordWrapCentred");
		break;

	default:
		return String((utf8*)"LeftAligned");
		break;
	}
}


void	HorzFormatting::set(void* receiver, const String& value)
{
	StaticText::HorzFormatting fmt;

	if (value == (utf8*)"RightAligned")
	{
		fmt = StaticText::RightAligned;
	}
	else if (value == (utf8*)"HorzCentred")
	{
		fmt = StaticText::HorzCentred;
	}
	else if (value == (utf8*)"WordWrapLeftAligned")
	{
		fmt = StaticText::WordWrapLeftAligned;
	}
	else if (value == (utf8*)"WordWrapRightAligned")
	{
		fmt = StaticText::WordWrapRightAligned;
	}
	else if (value == (utf8*)"WordWrapCentred")
	{
		fmt = StaticText::WordWrapCentred;
	}
	else
	{
		fmt = StaticText::LeftAligned;
	}

	static_cast<StaticText*>(receiver)->setHorizontalFormatting(fmt);
}


String	VertFormatting::get(const void* receiver) const
{
	switch(static_cast<const StaticText*>(receiver)->getVerticalFormatting())
	{
	case StaticText::BottomAligned:
		return String((utf8*)"BottomAligned");
		break;

	case StaticText::VertCentred:
		return String((utf8*)"VertCentred");
		break;

	default:
		return String((utf8*)"TopAligned");
		break;
	}
}


void	VertFormatting::set(void* receiver, const String& value)
{
	StaticText::VertFormatting fmt;

	if (value == (utf8*)"BottomAligned")
	{
		fmt = StaticText::BottomAligned;
	}
	else if (value == (utf8*)"VertCentred")
	{
		fmt = StaticText::VertCentred;
	}
	else
	{
		fmt = StaticText::TopAligned;
	}

	static_cast<StaticText*>(receiver)->setVerticalFormatting(fmt);
}

} // End of  StaticTextProperties namespace section

} // End of  CEGUI namespace section
