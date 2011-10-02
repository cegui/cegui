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
#include "CEGUI/WindowRendererSets/Falagard/StaticTextProperties.h"
#include "CEGUI/WindowRendererSets/Falagard/StaticText.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/Logger.h"

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
    return PropertyHelper<ColourRect>::toString(wr->getTextColours());
}


void	TextColours::set(PropertyReceiver* receiver, const String& value)
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setTextColours(PropertyHelper<ColourRect>::fromString(value));
}


String	HorzFormatting::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());

	switch(wr->getHorizontalFormatting())
	{
	case HTF_RIGHT_ALIGNED:
		return String("RightAligned");
		break;

	case HTF_CENTRE_ALIGNED:
		return String("HorzCentred");
		break;

	case HTF_JUSTIFIED:
		return String("HorzJustified");
		break;

	case HTF_WORDWRAP_LEFT_ALIGNED:
		return String("WordWrapLeftAligned");
		break;

	case HTF_WORDWRAP_RIGHT_ALIGNED:
		return String("WordWrapRightAligned");
		break;

	case HTF_WORDWRAP_CENTRE_ALIGNED:
		return String("WordWrapCentred");
		break;

	case HTF_WORDWRAP_JUSTIFIED:
		return String("WordWrapJustified");
		break;

	default:
		return String("LeftAligned");
		break;
	}
}


void	HorzFormatting::set(PropertyReceiver* receiver, const String& value)
{
	HorizontalTextFormatting fmt;

	if (value == "RightAligned")
	{
		fmt = HTF_RIGHT_ALIGNED;
	}
	else if (value == "HorzCentred")
	{
		fmt = HTF_CENTRE_ALIGNED;
	}
	else if (value == "HorzJustified")
	{
		fmt = HTF_JUSTIFIED;
	}
	else if (value == "WordWrapLeftAligned")
	{
		fmt = HTF_WORDWRAP_LEFT_ALIGNED;
	}
	else if (value == "WordWrapRightAligned")
	{
		fmt = HTF_WORDWRAP_RIGHT_ALIGNED;
	}
	else if (value == "WordWrapCentred")
	{
		fmt = HTF_WORDWRAP_CENTRE_ALIGNED;
	}
	else if (value == "WordWrapJustified")
	{
		fmt = HTF_WORDWRAP_JUSTIFIED;
	}
	else
	{
		fmt = HTF_LEFT_ALIGNED;
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
	case VTF_BOTTOM_ALIGNED:
		return String("BottomAligned");
		break;

	case VTF_CENTRE_ALIGNED:
		return String("VertCentred");
		break;

	default:
		return String("TopAligned");
		break;
	}
}


void	VertFormatting::set(PropertyReceiver* receiver, const String& value)
{
	VerticalTextFormatting fmt;

	if (value == "BottomAligned")
	{
		fmt = VTF_BOTTOM_ALIGNED;
	}
	else if (value == "VertCentred")
	{
		fmt = VTF_CENTRE_ALIGNED;
	}
	else
	{
		fmt = VTF_TOP_ALIGNED;
	}

	FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setVerticalFormatting(fmt);
}


String	VertScrollbar::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());
    return PropertyHelper<bool>::toString(wr->isVerticalScrollbarEnabled());
}


void	VertScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setVerticalScrollbarEnabled(PropertyHelper<bool>::fromString(value));
}


String	HorzScrollbar::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());
    return PropertyHelper<bool>::toString(wr->isHorizontalScrollbarEnabled());
}


void	HorzScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<Window*>(receiver)->getWindowRenderer());
    wr->setHorizontalScrollbarEnabled(PropertyHelper<bool>::fromString(value));
}

//----------------------------------------------------------------------------//
String HorzExtent::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());
    return PropertyHelper<float>::toString(wr->getHorizontalTextExtent());
}

//----------------------------------------------------------------------------//
void HorzExtent::set(PropertyReceiver* receiver, const String& value)
{
    Logger::getSingleton().logEvent("Attempt to set value of '" + value + "' "
        " to read only property 'HorzExtent' on window: " +
        static_cast<Window*>(receiver)->getName(),
        Errors);
}

//----------------------------------------------------------------------------//
String VertExtent::get(const PropertyReceiver* receiver) const
{
    FalagardStaticText* wr = static_cast<FalagardStaticText*>(
    static_cast<const Window*>(receiver)->getWindowRenderer());
    return PropertyHelper<float>::toString(wr->getVerticalTextExtent());
}

//----------------------------------------------------------------------------//
void VertExtent::set(PropertyReceiver* receiver, const String& value)
{
    Logger::getSingleton().logEvent("Attempt to set value of '" + value + "' "
        " to read only property 'VertExtent' on window: " +
        static_cast<Window*>(receiver)->getName(),
        Errors);
}

//----------------------------------------------------------------------------//

} // End of  FalagardStaticTextProperties namespace section

} // End of  CEGUI namespace section
