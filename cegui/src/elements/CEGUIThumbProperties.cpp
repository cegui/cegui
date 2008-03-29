/***********************************************************************
	filename: 	CEGUIThumbProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements the Thumb class properties.
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
#include "elements/CEGUIThumbProperties.h"
#include "elements/CEGUIThumb.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ThumbProperties namespace section
namespace ThumbProperties
{
String	HotTracked::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Thumb*>(receiver)->isHotTracked());
}


void	HotTracked::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Thumb*>(receiver)->setHotTracked(PropertyHelper::stringToBool(value));
}


String	VertFree::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Thumb*>(receiver)->isVertFree());
}


void	VertFree::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Thumb*>(receiver)->setVertFree(PropertyHelper::stringToBool(value));
}


String	HorzFree::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Thumb*>(receiver)->isHorzFree());
}


void	HorzFree::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Thumb*>(receiver)->setHorzFree(PropertyHelper::stringToBool(value));
}


String	VertRange::get(const PropertyReceiver* receiver) const
{
	using namespace std;
	char buff[64];

	pair<float, float> range = static_cast<const Thumb*>(receiver)->getVertRange();
	sprintf(buff, "min:%f max:%f", range.first, range.second);

	return String(buff);
}


void	VertRange::set(PropertyReceiver* receiver, const String& value)
{
	using namespace std;

	float rangeMin = 0, rangeMax = 0;
	sscanf(value.c_str(), " min:%f max:%f", &rangeMin, &rangeMax);

	static_cast<Thumb*>(receiver)->setVertRange(rangeMin, rangeMax);
}


String	HorzRange::get(const PropertyReceiver* receiver) const
{
	using namespace std;

	char buff[64];
	pair<float, float> range = static_cast<const Thumb*>(receiver)->getHorzRange();
	sprintf(buff, "min:%f max:%f", range.first, range.second);

	return String(buff);
}


void	HorzRange::set(PropertyReceiver* receiver, const String& value)
{
	using namespace std;

	float rangeMin = 0, rangeMax = 0;
	sscanf(value.c_str(), " min:%f max:%f", &rangeMin, &rangeMax);

	static_cast<Thumb*>(receiver)->setHorzRange(rangeMin, rangeMax);
}


} // End of  ThumbProperties namespace section

} // End of  CEGUI namespace section
