/************************************************************************
	filename: 	TLVertScrollbar.cpp
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Vertical Scrollbar widget
				(Large version of scrollbar)
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
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "TLVertScrollbar.h"
#include "TLVertScrollbarThumb.h"
#include "TLButton.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// Progress bar image names
const utf8	TLVertScrollbar::ImagesetName[]					= "TaharezImagery";
const utf8	TLVertScrollbar::ContainerTopImageName[]		= "VertScrollTop";
const utf8	TLVertScrollbar::ContainerMiddleImageName[]		= "VertScrollMiddle";
const utf8	TLVertScrollbar::ContainerBottomImageName[]		= "VertScrollBottom";
const utf8	TLVertScrollbar::ThumbTrackSegmentImageName[]	= "VertScrollBarSegment";
const utf8	TLVertScrollbar::UpButtonNormalImageName[]		= "VertScrollUpNormal";
const utf8	TLVertScrollbar::UpButtonHighlightImageName[]	= "VertScrollUpHover";
const utf8	TLVertScrollbar::DownButtonNormalImageName[]	= "VertScrollDownNormal";
const utf8	TLVertScrollbar::DownButtonHighlightImageName[]	= "VertScrollDownHover";

// some layout stuff
const float	TLVertScrollbar::ThumbWidth			= 0.4f;
const float	TLVertScrollbar::ThumbPositionX		= 0.325f;
const float	TLVertScrollbar::TrackWidthRatio	= 0.2f;
const float	TLVertScrollbar::TrackOffsetXRatio	= 0.45f;
const float	TLVertScrollbar::ButtonWidth		= 0.6f;
const float	TLVertScrollbar::ButtonPositionX	= 0.25f;
const float	TLVertScrollbar::ButtonOffsetYRatio	= 0.5f;

// type names for the component widgets
const utf8	TLVertScrollbar::ThumbWidgetType[]			= "Taharez VertScrollbarThumb";
const utf8	TLVertScrollbar::IncreaseButtonWidgetType[]	= "Taharez Button";
const utf8	TLVertScrollbar::DecreaseButtonWidgetType[]	= "Taharez Button";


/*************************************************************************
	Constructor for Taharez vertical scroll bar widgets
*************************************************************************/
TLVertScrollbar::TLVertScrollbar(const String& type, const String& name) :
	Scrollbar(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup cache of image pointers
	d_containerTop		= &iset->getImage(ContainerTopImageName);
	d_containerMiddle	= &iset->getImage(ContainerMiddleImageName);
	d_containerBottom	= &iset->getImage(ContainerBottomImageName);
	d_thumbTrack		= &iset->getImage(ThumbTrackSegmentImageName);
	d_upNormal			= &iset->getImage(UpButtonNormalImageName);
}


/*************************************************************************
	Destructor for Taharez vertical scroll bar widgets
*************************************************************************/
TLVertScrollbar::~TLVertScrollbar(void)
{
}


/*************************************************************************
	create a PushButton based widget to use as the increase button for
	this scroll bar.
*************************************************************************/
PushButton* TLVertScrollbar::createIncreaseButton(void) const
{
	// create the widget
	TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(IncreaseButtonWidgetType, getName() + "__auto_incbtn__");

	// perform some initialisation
	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
	img.setImage(&iset->getImage(DownButtonNormalImageName));
	btn->setNormalImage(&img);
	btn->setDisabledImage(&img);

	img.setImage(&iset->getImage(DownButtonHighlightImageName));
	btn->setHoverImage(&img);
	btn->setPushedImage(&img);

	return btn;
}


/*************************************************************************
	create a PushButton based widget to use as the decrease button for
	this scroll bar.
*************************************************************************/
PushButton* TLVertScrollbar::createDecreaseButton(void) const
{
	// create the widget
	TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(DecreaseButtonWidgetType, getName() + "__auto_decbtn__");

	// perform some initialisation
	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
	img.setImage(d_upNormal);
	btn->setNormalImage(&img);
	btn->setDisabledImage(&img);

	img.setImage(&iset->getImage(UpButtonHighlightImageName));
	btn->setHoverImage(&img);
	btn->setPushedImage(&img);

	return btn;
}


/*************************************************************************
	create a Thumb based widget to use as the thumb for this scroll bar.
*************************************************************************/
Thumb* TLVertScrollbar::createThumb(void) const
{
	// create the widget
	TLVertScrollbarThumb* thumb = (TLVertScrollbarThumb*)WindowManager::getSingleton().createWindow(ThumbWidgetType, getName() + "__auto_thumb__");

	// perform some initialisation
	thumb->setVertFree(true);
	thumb->setXPosition(ThumbPositionX);
	thumb->setWidth(ThumbWidth);

	return thumb;
}


/*************************************************************************
	layout the scroll bar component widgets
*************************************************************************/
void TLVertScrollbar::layoutComponentWidgets(void)
{
	// calculate button sizes
	Size bsz;
	bsz.d_width = d_abs_area.getWidth() * ButtonWidth;

	float ratio = bsz.d_width / d_upNormal->getWidth();

	bsz.d_height = d_upNormal->getHeight() * ratio;

	// install button sizes
	d_increase->setSize(absoluteToRelative(bsz));
	d_decrease->setSize(absoluteToRelative(bsz));

	// position buttons
	float ySpacing = d_containerTop->getHeight() * ButtonOffsetYRatio;
	d_decrease->setPosition(Point(ButtonPositionX, absoluteToRelativeY(ySpacing)));
	d_increase->setPosition(Point(ButtonPositionX, absoluteToRelativeY(d_abs_area.getHeight() - bsz.d_height - ySpacing)));

	// this will configure thumb widget appropriately
	updateThumb();
}


/*************************************************************************
	update the size and location of the thumb to properly represent the
	current state of the scroll bar
*************************************************************************/
void TLVertScrollbar::updateThumb(void)
{
	using namespace std;

	// calculate actual padding values to use.
	float slideTrackYPadding = d_decrease->getAbsoluteHeight() + (d_containerTop->getHeight() * 0.5f);

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= max(0.0f, d_abs_area.getHeight() - (2 * slideTrackYPadding) - d_thumb->getAbsoluteHeight());

	// Thumb does not change size with document length, we just need to update position and range
	d_thumb->setVertRange(absoluteToRelativeY(slideTrackYPadding), absoluteToRelativeY(slideTrackYPadding + slideExtent));
	d_thumb->setYPosition(absoluteToRelativeY(slideTrackYPadding + (d_position * (slideExtent / posExtent))));
}


/*************************************************************************
	return value that best represents current scroll bar position given
	the current location of the thumb.
*************************************************************************/
float TLVertScrollbar::getValueFromThumb(void) const
{
	// calculate actual padding values to use.
	float slideTrackYPadding = d_decrease->getAbsoluteHeight() + (d_containerTop->getHeight() * 0.5f);

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= d_abs_area.getHeight() - (2 * slideTrackYPadding) - d_thumb->getAbsoluteHeight();

	return	(d_thumb->getAbsoluteYPosition() - slideTrackYPadding) / (slideExtent / posExtent);
}


/*************************************************************************
	Given window location \a pt, return a value indicating what change
	should be made to the scroll bar.
*************************************************************************/
float TLVertScrollbar::getAdjustDirectionFromPoint(const Point& pt) const
{
	Rect absrect(d_thumb->getUnclippedPixelRect());

	if (pt.d_y < absrect.d_top)
	{
		return -1.0f;
	}
	else if (pt.d_y > absrect.d_bottom)
	{
		return 1.0f;
	}
	else
	{
		return 0.0f;
	}

}


/*************************************************************************
	Perform rendering for this widget
*************************************************************************/
void TLVertScrollbar::drawSelf(float z)
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
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	float mid_height = absrect.getHeight() - d_containerTop->getHeight() - d_containerBottom->getHeight();

	//
	// Render scroll bar container
	//
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	Size	sz(absrect.getWidth(), d_containerTop->getHeight());
	d_containerTop->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = mid_height;
	d_containerMiddle->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = d_containerBottom->getHeight();
	d_containerBottom->draw(pos, sz, clipper, colours);

	//
	// render slide-track
	//
	float slideTrackYPadding = d_decrease->getAbsoluteHeight() + (d_containerTop->getHeight() * 0.5f);

	// calculate a new clipper for the slide track area
	absrect.d_top		+= slideTrackYPadding;
	absrect.d_bottom	-= slideTrackYPadding;
	clipper = absrect.getIntersection(clipper);

	pos.d_x += absrect.getWidth() * TrackOffsetXRatio;
	pos.d_y = absrect.d_top;
	pos.d_z = System::getSingleton().getRenderer()->getZLayer(1);

	sz.d_height	= d_thumbTrack->getHeight();
	sz.d_width	= absrect.getWidth() * TrackWidthRatio;

	int segments = (int)((absrect.getHeight() / d_thumbTrack->getHeight()) + 0.99f);

	for (int i = 0; i < segments; ++i)
	{
		d_thumbTrack->draw(pos, sz, clipper, colours);
		pos.d_y += sz.d_height;
	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLVertScrollbar
*************************************************************************/
Window* TLVertScrollbarFactory::createWindow(const String& name)
{
	TLVertScrollbar* wnd = new TLVertScrollbar(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
