/************************************************************************
	filename: 	TLListHeaderSegment.cpp
	created:	15/6/2004
	author:		Paul D Turner
	
	purpose:	Implements Taharez look list header segment widget.
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
#include "TLListHeaderSegment.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// image / imageset related
const utf8	TLListHeaderSegment::ImagesetName[]				= "TaharezImagery";
const utf8	TLListHeaderSegment::BackdropNormalImageName[]	= "HeaderBarBackdropNormal";
const utf8	TLListHeaderSegment::BackdropHoverImageName[]	= "HeaderBarBackdropHover";
const utf8	TLListHeaderSegment::SplitterNormalImageName[]	= "HeaderBarSplitterNormal";
const utf8	TLListHeaderSegment::SplitterHoverImageName[]	= "HeaderBarSplitterHover";
const utf8	TLListHeaderSegment::SortUpImageName[]			= "HeaderBarSortUp";
const utf8	TLListHeaderSegment::SortDownImageName[]		= "HeaderBarSortDown";
const utf8	TLListHeaderSegment::NormalMouseCursor[]		= "MouseArrow";
const utf8	TLListHeaderSegment::SizingMouseCursor[]		= "MouseEsWeCursor";
const utf8	TLListHeaderSegment::MovingMouseCursor[]		= "MouseMoveCursor";


/*************************************************************************
	Constructor	
*************************************************************************/
TLListHeaderSegment::TLListHeaderSegment(const String& type, const String& name) :
	ListHeaderSegment(type, name)
{
	// init the image cache
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_backNormalImage		= &iset->getImage(BackdropNormalImageName);
	d_backHoverImage		= &iset->getImage(BackdropHoverImageName);
	d_splitterNormalImage	= &iset->getImage(SplitterNormalImageName);
	d_splitterHoverImage	= &iset->getImage(SplitterHoverImageName);
	d_sortAscendImage		= &iset->getImage(SortUpImageName);
	d_sortDescendImage		= &iset->getImage(SortDownImageName);

	d_normalMouseCursor		= &iset->getImage(NormalMouseCursor);
	d_sizingMouseCursor		= &iset->getImage(SizingMouseCursor);
	d_movingMouseCursor		= &iset->getImage(MovingMouseCursor);
}


/*************************************************************************
	Destructor
*************************************************************************/
TLListHeaderSegment::~TLListHeaderSegment(void)
{
}


/*************************************************************************
	Render the widget
*************************************************************************/
void TLListHeaderSegment::drawSelf(float z)
{
	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// get clipping Rect for window
	Rect clipper(getPixelRect());

	Vector3 pos(absrect.d_left, absrect.d_top, z);

	// if widget is not totally clipped
	if (clipper.getWidth() != 0)
	{
		renderSegmentImagery(pos, getEffectiveAlpha(), clipper);
	}

	// always draw the ghost if the segment is geing dragged.
	if (d_dragMoving)
	{
		clipper = System::getSingleton().getRenderer()->getRect();
		pos.d_x = absrect.d_left + d_dragPosition.d_x;
		pos.d_y = absrect.d_top + d_dragPosition.d_y;
		pos.d_z = 0.0f;
		renderSegmentImagery(pos, getEffectiveAlpha() * 0.5f, clipper);
	}
}


/*************************************************************************
	Render segment	
*************************************************************************/
void TLListHeaderSegment::renderSegmentImagery(Vector3 pos, float alpha, const Rect& clipper)
{
	Rect absrect(pos.d_x, pos.d_y, pos.d_x + getAbsoluteWidth(), pos.d_y + getAbsoluteHeight());
	Rect destrect(absrect);

	// calculate colours to use.
	colour alpha_comp = ((colour)(alpha * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	//
	// draw the main images
	//
	destrect.d_right -= d_splitterNormalImage->getWidth();

	if ((d_segmentHover != d_segmentPushed) && !d_splitterHover && isClickable())
	{
		d_backHoverImage->draw(destrect, pos.d_z, clipper, colours);
	}
	else
	{
		d_backNormalImage->draw(destrect, pos.d_z, clipper, colours);
	}

	// draw splitter
	destrect.d_left = destrect.d_right;
	destrect.d_right = absrect.d_right;

	if (d_splitterHover)
	{
		d_splitterHoverImage->draw(destrect, pos.d_z, clipper, colours);
	}
	else
	{
		d_splitterNormalImage->draw(destrect, pos.d_z, clipper, colours);
	}

	// 
	// adjust clipper to be inside the splitter bar imagery
	//
	destrect.d_right = destrect.d_left;

	// pad left by half the width of the 'sort' icon.
	destrect.d_left = absrect.d_left + d_sortAscendImage->getWidth() * 0.5f;
	Rect inner_clip(destrect.getIntersection(clipper));

	//
	// Render 'sort' icon as needed
	//
	if (d_sortDir == Ascending)
	{
		d_sortAscendImage->draw(Vector3(destrect.d_left, destrect.d_top + d_sortAscendImage->getHeight() * 0.5f, pos.d_z), inner_clip, colours);
		destrect.d_left += d_sortAscendImage->getWidth() * 1.5f;
	}
	else if (d_sortDir == Descending)
	{
		d_sortDescendImage->draw(Vector3(destrect.d_left, destrect.d_top + d_sortDescendImage->getHeight() * 0.5f, pos.d_z), inner_clip, colours);
		destrect.d_left += d_sortDescendImage->getWidth() * 1.5f;
	}

	//
	// Render the text
	//
	// find a font to be used.
	const Font* fnt;
	if (d_font != NULL)
	{
		fnt = d_font;
	}
	else if (d_parent != NULL)
	{
		fnt = d_parent->getFont();
	}
	else
	{
		fnt = System::getSingleton().getDefaultFont();
	}

	// centre text vertically
	destrect.d_top += ((destrect.getHeight() - fnt->getLineSpacing()) * 0.5f);
	fnt->drawText(getText(), destrect, pos.d_z, inner_clip, LeftAligned, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLListHeaderSegment
*************************************************************************/
Window* TLListHeaderSegmentFactory::createWindow(const String& name)
{
	TLListHeaderSegment* wnd = new TLListHeaderSegment(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
