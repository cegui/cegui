/************************************************************************
	filename: 	TLProgressBar.cpp
	created:	23/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of the Taharez Progress Bar
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
#include "TLProgressBar.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLProgressBar::WidgetTypeName[]	= "TaharezLook/ProgressBar";

// Progress bar image names
const utf8	TLProgressBar::ImagesetName[]				= "TaharezLook";
const utf8	TLProgressBar::ContainerLeftImageName[]		= "ProgressBarLeft";
const utf8	TLProgressBar::ContainerMiddleImageName[]	= "ProgressBarMiddle";
const utf8	TLProgressBar::ContainerRightImageName[]	= "ProgressBarRight";
const utf8	TLProgressBar::DimSegmentImageName[]		= "ProgressBarDimSegment";
const utf8	TLProgressBar::BrightSegmentImageName[]		= "ProgressBarLitSegment";

// some offsets into imagery
const float	TLProgressBar::FirstSegmentOffsetRatioX		= 0.28571f;
const float	TLProgressBar::SegmentOverlapRatio			= 0.25f;


/*************************************************************************
	Constructor for Taharez progress bar objects
*************************************************************************/
TLProgressBar::TLProgressBar(const String& type, const String& name) :
	ProgressBar(type, name)
{
	// cache images to be used
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_left			= &iset->getImage(ContainerLeftImageName);
	d_right			= &iset->getImage(ContainerRightImageName);
	d_middle		= &iset->getImage(ContainerMiddleImageName);
	d_dimSegment	= &iset->getImage(DimSegmentImageName);
	d_litSegment	= &iset->getImage(BrightSegmentImageName);
}


/*************************************************************************
	Destructor for Taharez progress bar objects
*************************************************************************/
TLProgressBar::~TLProgressBar(void)
{
}


/*************************************************************************
	Perform rendering for this widget
*************************************************************************/
void TLProgressBar::drawSelf(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	ColourRect colours(colour(1, 1, 1, getEffectiveAlpha()));

	//
	// Render the container
	//
	float leftWidth	 = d_left->getWidth();
	float rightWidth = d_right->getWidth();
	float midWidth	 = d_middle->getWidth();

	// calculate the number of segments in the mid-section
	int segCount = (int)((absrect.getWidth() - leftWidth - rightWidth) / midWidth);
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	Size sz(leftWidth, absrect.getHeight());

	// left end
	d_left->draw(pos, sz, clipper, colours);
	pos.d_x += leftWidth;

	// middle segments
	sz.d_width = midWidth;
	for (int mid = 0; mid < segCount; ++mid)
	{
		d_middle->draw(pos, sz, clipper, colours);
		pos.d_x += midWidth;
	}

	// right end
	sz.d_width = rightWidth;
	d_right->draw(pos, sz, clipper, colours);


	//
	// Render the 'lit' portion
	//
	// this increment is because the 'ends' of the container form 1 segment in addition to the middle segments
	segCount++;

	float segWidth = d_litSegment->getWidth();
	segWidth -= (segWidth * SegmentOverlapRatio);

	// construct rect for segment area
	Rect segClipper;
	segClipper.setPosition(Point(absrect.d_left + (d_litSegment->getWidth() * FirstSegmentOffsetRatioX), absrect.d_top));
	segClipper.setSize(Size(segCount * segWidth * d_progress, absrect.getHeight()));

	// clip the clipper to 'lit area'
	clipper = segClipper.getIntersection(clipper);

	pos.d_x = absrect.d_left + (d_litSegment->getWidth() * FirstSegmentOffsetRatioX);

	sz.d_width = segWidth;
	for (int seg = 0; seg < segCount; ++seg)
	{
		d_litSegment->draw(pos, sz, clipper, colours);
		pos.d_x += segWidth;
	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLProgressBar
*************************************************************************/
Window* TLProgressBarFactory::createWindow(const String& name)
{
	TLProgressBar* wnd = new TLProgressBar(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
