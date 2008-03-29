/***********************************************************************
	filename: 	CEGUIListHeaderSegmentProperties.cpp
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for the header segment class
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
#include "elements/CEGUIListHeaderSegmentProperties.h"
#include "elements/CEGUIListHeaderSegment.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ListHeaderSegmentProperties namespace section
namespace ListHeaderSegmentProperties
{
String	Sizable::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const ListHeaderSegment*>(receiver)->isSizingEnabled());
}


void	Sizable::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<ListHeaderSegment*>(receiver)->setSizingEnabled(PropertyHelper::stringToBool(value));
}


String	Clickable::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const ListHeaderSegment*>(receiver)->isClickable());
}


void	Clickable::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<ListHeaderSegment*>(receiver)->setClickable(PropertyHelper::stringToBool(value));
}


String	Dragable::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const ListHeaderSegment*>(receiver)->isDragMovingEnabled());
}


void	Dragable::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<ListHeaderSegment*>(receiver)->setDragMovingEnabled(PropertyHelper::stringToBool(value));
}


String	SortDirection::get(const PropertyReceiver* receiver) const
{
	switch(static_cast<const ListHeaderSegment*>(receiver)->getSortDirection())
	{
	case ListHeaderSegment::Ascending:
		return String("Ascending");
		break;

	case ListHeaderSegment::Descending:
		return String("Descending");
		break;

	default:
		return String("None");
		break;
	}

}


void	SortDirection::set(PropertyReceiver* receiver, const String& value)
{
	ListHeaderSegment::SortDirection dir;

	if (value == "Ascending")
	{
		dir = ListHeaderSegment::Ascending;
	}
	else if (value == "Descending")
	{
		dir = ListHeaderSegment::Descending;
	}
	else
	{
		dir = ListHeaderSegment::None;
	}

	static_cast<ListHeaderSegment*>(receiver)->setSortDirection(dir);
}


String SizingCursorImage::get(const PropertyReceiver* receiver) const
{
    const Image* img = static_cast<const ListHeaderSegment*>(receiver)->getSizingCursorImage();
    return img ? PropertyHelper::imageToString(img) : String("");
}

void SizingCursorImage::set(PropertyReceiver* receiver, const String &value)
{
    static_cast<ListHeaderSegment*>(receiver)->setSizingCursorImage(PropertyHelper::stringToImage(value));
}


String MovingCursorImage::get(const PropertyReceiver* receiver) const
{
    const Image* img = static_cast<const ListHeaderSegment*>(receiver)->getMovingCursorImage();
    return img ? PropertyHelper::imageToString(img) : String("");
}

void MovingCursorImage::set(PropertyReceiver* receiver, const String &value)
{
    static_cast<ListHeaderSegment*>(receiver)->setMovingCursorImage(PropertyHelper::stringToImage(value));
}

} // End of  ListHeaderSegmentProperties namespace section

} // End of  CEGUI namespace section
