/************************************************************************
	filename: 	CEGUIImage.cpp
	created:	13/3/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Image class members
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
#include "CEGUIImage.h"
#include "CEGUIExceptions.h"
#include "CEGUITexture.h"
#include "CEGUIImageset.h"
#include "CEGUIRenderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constructor
*************************************************************************/
Image::Image(const Imageset* owner, const Rect& area, const Point& render_offset) : 
	d_owner(owner),
	d_area(area),
	d_offset(render_offset)
{
	if (d_owner == NULL)
	{
		throw NullObjectException((utf8*)"Imageset pointer passed to Image constructor must not be null.");
	}

	// TODO: if we ever store texture co-ordinates, they should be calculated here.
}

/*************************************************************************
	Copy constructor
*************************************************************************/
Image::Image(const Image& image) :
	d_owner(image.d_owner),
	d_area(image.d_area),
	d_offset(image.d_offset)
{
}

/*************************************************************************
	destructor
*************************************************************************/
Image::~Image(void)
{
}

/*************************************************************************
	Clip and then queue the image to be rendered.
*************************************************************************/
void Image::draw(const Rect& dest_rect, float z, const Rect& clip_rect, const ColourRect& colours) const
{
	Rect dest(dest_rect);

	// apply rendering offset to the destination Rect
	dest.offset(d_offset);

	// draw
	d_owner->draw(d_area, dest, z, clip_rect, colours);
}

} // End of  CEGUI namespace section
