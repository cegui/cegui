/************************************************************************
	filename: 	CEGUIRect.cpp
	created:	8/3/2004
	author:		Paul D Turner
	
	purpose:	Implements 'Rect' class
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
#include "CEGUIRect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor
*************************************************************************/
Rect::Rect(float left, float top, float right, float bottom) :
	d_left(left),
	d_top(top),
	d_right(right),
	d_bottom(bottom)
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


} // End of  CEGUI namespace section
