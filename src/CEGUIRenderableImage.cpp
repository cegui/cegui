/************************************************************************
	filename: 	CEGUIRenderableImage.cpp
	created:	17/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of RenderableImage UI entity
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
#include "CEGUIRenderableImage.h"
#include "CEGUIImage.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constructor		
*************************************************************************/
RenderableImage::RenderableImage(void) :
	d_image(NULL),
	d_horzFormat(LeftAligned),
	d_vertFormat(TopAligned)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
RenderableImage::~RenderableImage(void)
{
}


/*************************************************************************
	Renders the imagery for a RenderableImage element.
*************************************************************************/
void RenderableImage::draw_impl(const Vector3& position, const Rect& clip_rect) const
{
	// do not draw anything if image is not set.
	if (d_image == NULL)
		return;

	// calculate final clipping rect which is intersection of RenderableImage area and supplied clipping area
	Rect final_clipper(position.d_x, position.d_y, 0, 0);
	final_clipper.setSize(d_area.getSize());
	final_clipper = clip_rect.getIntersection(final_clipper);

	Size imgSize(d_image->getSize());

	// calculate number of times to tile image based of formatting options
	uint horzTiles = (d_horzFormat == HorzTiled) ? (uint)((d_area.getWidth() + (imgSize.d_width - 1)) / imgSize.d_width) : 1;
	uint vertTiles = (d_vertFormat == VertTiled) ? (uint)((d_area.getHeight() + (imgSize.d_height - 1)) / imgSize.d_height) : 1;

	// calculate 'base' X co-ordinate, depending upon formatting
	float baseX;

	switch (d_horzFormat)
	{
		case HorzStretched:
			imgSize.d_width = d_area.getWidth();
			// intentional fall-through

		case HorzTiled:
		case LeftAligned:
			baseX = position.d_x;
			break;

		case HorzCentred:
			baseX = position.d_x + ((d_area.getWidth() - imgSize.d_width) / 2);
			break;

		case RightAligned:
			baseX = position.d_x + d_area.getWidth() - imgSize.d_width;
			break;

		default:
			throw InvalidRequestException((utf8*)"An unknown horizontal formatting value was specified in a RenderableImage object.");
	}

	// calculate 'base' Y co-ordinate, depending upon formatting
	float baseY;

	switch (d_vertFormat)
	{
		case VertStretched:
			imgSize.d_height = d_area.getHeight();
			// intentional fall-through

		case VertTiled:
		case TopAligned:
			baseY = position.d_y;
			break;

		case VertCentred:
			baseY = position.d_y + ((d_area.getHeight() - imgSize.d_height) / 2);
			break;

		case BottomAligned:
			baseY = position.d_y + d_area.getHeight() - imgSize.d_height;
			break;

		default:
			throw InvalidRequestException((utf8*)"An unknown vertical formatting value was specified in a RenderableImage object.");
	}

	Vector3 drawpos(0,baseY, position.d_z);

	// perform actual rendering
	for (uint row = 0; row < vertTiles; ++row)
	{
		drawpos.d_x = baseX;

		for (uint col = 0; col < horzTiles; ++col)
		{
			d_image->draw(drawpos, imgSize, final_clipper, d_colours);
			drawpos.d_x += imgSize.d_width;
		}

		drawpos.d_y += imgSize.d_height;
	}

}

} // End of  CEGUI namespace section
