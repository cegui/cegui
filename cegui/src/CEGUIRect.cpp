/***********************************************************************
	filename: 	CEGUIRect.cpp
	created:	8/3/2004
	author:		Paul D Turner
	
	purpose:	Implements 'Rect' class
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
#include "CEGUIRect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor
*************************************************************************/
Rect::Rect(float left, float top, float right, float bottom) :
	d_top(top),
	d_bottom(bottom),
	d_left(left),
	d_right(right)
{
}

Rect::Rect(Point pos, Size sz) :
    d_top(pos.d_y),
    d_bottom(pos.d_y + sz.d_height),
    d_left(pos.d_x),
    d_right(pos.d_x + sz.d_width)
{
}


/*************************************************************************
	Return a Rect object that is the intersection of 'this' with 'rect'
*************************************************************************/
Rect Rect::getIntersection(const Rect& rect) const
{
	// check for total exclusion
	if ((d_right > rect.d_left) &&
		(d_left < rect.d_right) &&
		(d_bottom > rect.d_top) &&
		(d_top < rect.d_bottom))
	{
		Rect temp;

		// fill in temp with the intersection
		temp.d_left = (d_left > rect.d_left) ? d_left : rect.d_left;
		temp.d_right = (d_right < rect.d_right) ? d_right : rect.d_right;
		temp.d_top = (d_top > rect.d_top) ? d_top : rect.d_top;
		temp.d_bottom = (d_bottom < rect.d_bottom) ? d_bottom : rect.d_bottom;

		return temp;
	}
	else
	{
		return Rect(0.0f, 0.0f, 0.0f, 0.0f);
	}

}

/*************************************************************************
	Apply an offset the the Rect
*************************************************************************/
Rect& Rect::offset(const Point& pt)
{
	d_left		+= pt.d_x;
	d_right		+= pt.d_x;
	d_top		+= pt.d_y;
	d_bottom	+= pt.d_y;
	return *this;
}


/*************************************************************************
	Check if a given point is within the Rect
*************************************************************************/
bool Rect::isPointInRect(const Point& pt) const
{
	if ((d_left > pt.d_x) ||
		(d_right <= pt.d_x) ||
		(d_top > pt.d_y) ||
		(d_bottom <= pt.d_y))
	{
		return false;
	}

	return true;
}

/*************************************************************************
	Set location of rect retaining current size.
*************************************************************************/
void Rect::setPosition(const Point& pt)
{
	Size sz(getSize());

	d_left = pt.d_x;
	d_top  = pt.d_y;
	setSize(sz);
}


/*************************************************************************
	check the size of the Rect object and if it is bigger than 'sz', 
	resize it so it isn't.	
*************************************************************************/
Rect& Rect::constrainSizeMax(const Size& sz)
{
	if (getWidth() > sz.d_width)
	{
		setWidth(sz.d_width);
	}

	if (getHeight() > sz.d_height)
	{
		setHeight(sz.d_height);
	}

	return *this;
}


/*************************************************************************
	check the size of the Rect object and if it is smaller than 'sz',
	resize it so it isn't.
*************************************************************************/
Rect& Rect::constrainSizeMin(const Size& sz)
{
	if (getWidth() < sz.d_width)
	{
		setWidth(sz.d_width);
	}

	if (getHeight() < sz.d_height)
	{
		setHeight(sz.d_height);
	}

	return *this;
}


/*************************************************************************
	check the size of the Rect object and if it is bigger than 'max_sz'
	or smaller than 'min_sz', resize it so it isn't.
*************************************************************************/
Rect& Rect::constrainSize(const Size& max_sz, const Size& min_sz)
{
	Size curr_sz(getSize());

	if (curr_sz.d_width > max_sz.d_width)
	{
		setWidth(max_sz.d_width);
	}
	else if (curr_sz.d_width < min_sz.d_width)
	{
		setWidth(min_sz.d_width);
	}

	if (curr_sz.d_height > max_sz.d_height)
	{
		setHeight(max_sz.d_height);
	}
	else if (curr_sz.d_height < min_sz.d_height)
	{
		setHeight(min_sz.d_height);
	}

	return *this;
}

Rect& Rect::operator=(const Rect& rhs)
{
	d_left = rhs.d_left;
	d_top = rhs.d_top;
	d_right = rhs.d_right;
	d_bottom = rhs.d_bottom;

	return *this;
}

} // End of  CEGUI namespace section
