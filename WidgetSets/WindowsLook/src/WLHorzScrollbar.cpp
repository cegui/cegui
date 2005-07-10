/************************************************************************
	filename: 	WLHorzScrollbar.cpp
	created:	10/8/2004
	author:		Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#include "WLHorzScrollbar.h"
#include "WLHorzScrollbarThumb.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "WLButton.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLHorzScrollbar::WidgetTypeName[]				= "WindowsLook/HorizontalScrollbar";

// image name constants
const utf8	WLHorzScrollbar::ImagesetName[]					= "WindowsLook";
const utf8	WLHorzScrollbar::TopLeftFrameImageName[]		= "StaticFrameTopLeft";
const utf8	WLHorzScrollbar::TopRightFrameImageName[]		= "StaticFrameTopRight";
const utf8	WLHorzScrollbar::BottomLeftFrameImageName[]		= "StaticFrameBottomLeft";
const utf8	WLHorzScrollbar::BottomRightFrameImageName[]	= "StaticFrameBottomRight";
const utf8	WLHorzScrollbar::LeftFrameImageName[]			= "StaticFrameLeft";
const utf8	WLHorzScrollbar::RightFrameImageName[]			= "StaticFrameRight";
const utf8	WLHorzScrollbar::TopFrameImageName[]			= "StaticFrameTop";
const utf8	WLHorzScrollbar::BottomFrameImageName[]			= "StaticFrameBottom";
const utf8	WLHorzScrollbar::BackgroundImageName[]			= "Background";
const utf8	WLHorzScrollbar::LeftButtonNormalImageName[]	= "LargeLeftArrow";
const utf8	WLHorzScrollbar::LeftButtonHighlightImageName[]	= "LargeLeftArrow";
const utf8	WLHorzScrollbar::RightButtonNormalImageName[]	= "LargeRightArrow";
const utf8	WLHorzScrollbar::RightButtonHighlightImageName[]= "LargeRightArrow";
const utf8	WLHorzScrollbar::MouseCursorImageName[]			= "MouseArrow";

// Colours
const colour WLHorzScrollbar::BackgroundColour		= 0xDFDFDF;

// some layout stuff
const float	WLHorzScrollbar::MinThumbWidth			= 10.0f;

// type names for the component widgets
const utf8*	WLHorzScrollbar::ThumbWidgetType			= WLHorzScrollbarThumb::WidgetTypeName;
const utf8*	WLHorzScrollbar::IncreaseButtonWidgetType	= WLButton::WidgetTypeName;
const utf8*	WLHorzScrollbar::DecreaseButtonWidgetType	= WLButton::WidgetTypeName;


/*************************************************************************
	Constructor for WindowsLook horizontal scroll bar widgets
*************************************************************************/
WLHorzScrollbar::WLHorzScrollbar(const String& type, const String& name) :
	Scrollbar(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frame.setImages(
		&iset->getImage(TopLeftFrameImageName),
		&iset->getImage(TopRightFrameImageName),
		&iset->getImage(BottomLeftFrameImageName),
		&iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName),
		&iset->getImage(TopFrameImageName),
		&iset->getImage(RightFrameImageName),
		&iset->getImage(BottomFrameImageName)
		);

	d_background = &iset->getImage(BackgroundImageName);

	setMouseCursor(&iset->getImage(MouseCursorImageName));

	storeFrameSizes();
}


/*************************************************************************
	Destructor for WindowsLook horizontal scroll bar widgets
*************************************************************************/
WLHorzScrollbar::~WLHorzScrollbar(void)
{
}


/*************************************************************************
	create a PushButton based widget to use as the increase button for
	this scroll bar.
*************************************************************************/
PushButton* WLHorzScrollbar::createIncreaseButton(const String& name) const
{
	// create the widget
	WLButton* btn = (WLButton*)WindowManager::getSingleton().createWindow(IncreaseButtonWidgetType, name);

	// perform some initialisation
	btn->setStandardImageryEnabled(true);
	btn->setCustomImageryAutoSized(true);
	btn->setAlwaysOnTop(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzCentred);
	img.setVertFormatting(RenderableImage::VertCentred);
	img.setImage(&iset->getImage(RightButtonNormalImageName));
	btn->setNormalImage(&img);
	btn->setDisabledImage(&img);

	img.setImage(&iset->getImage(RightButtonHighlightImageName));
	btn->setHoverImage(&img);
	btn->setPushedImage(&img);

	return btn;
}


/*************************************************************************
	create a PushButton based widget to use as the decrease button for
	this scroll bar.
*************************************************************************/
PushButton* WLHorzScrollbar::createDecreaseButton(const String& name) const
{
	// create the widget
	WLButton* btn = (WLButton*)WindowManager::getSingleton().createWindow(DecreaseButtonWidgetType, name);

	// perform some initialisation
	btn->setStandardImageryEnabled(true);
	btn->setCustomImageryAutoSized(true);
	btn->setAlwaysOnTop(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzCentred);
	img.setVertFormatting(RenderableImage::VertCentred);
	img.setImage(&iset->getImage(LeftButtonNormalImageName));
	btn->setNormalImage(&img);
	btn->setDisabledImage(&img);

	img.setImage(&iset->getImage(LeftButtonHighlightImageName));
	btn->setHoverImage(&img);
	btn->setPushedImage(&img);

	return btn;
}


/*************************************************************************
	create a Thumb based widget to use as the thumb for this scroll bar.
*************************************************************************/
Thumb* WLHorzScrollbar::createThumb(const String& name) const
{
	// create the widget
	Thumb* thumb = (Thumb*)WindowManager::getSingleton().createWindow(ThumbWidgetType, name);

	// perform initialisation
	thumb->setHorzFree(true);

	return thumb;
}


/*************************************************************************
	layout the scroll bar component widgets
*************************************************************************/
void WLHorzScrollbar::layoutComponentWidgets(void)
{
	Size bsz;
	bsz.d_width = bsz.d_height = getAbsoluteHeight() - d_frameLeftSize - d_frameRightSize;

	// install button sizes
	d_increase->setSize(absoluteToRelative(bsz));
	d_decrease->setSize(absoluteToRelative(bsz));

	// position buttons
	d_decrease->setPosition(absoluteToRelative(Point(d_frameLeftSize, d_frameTopSize)));
	d_increase->setPosition(absoluteToRelative(Point(getAbsoluteWidth() - bsz.d_width - d_frameRightSize, d_frameTopSize)));

	// this will configure thumb widget appropriately
	updateThumb();
}


/*************************************************************************
	update the size and location of the thumb to properly represent the
	current state of the scroll bar
*************************************************************************/
void WLHorzScrollbar::updateThumb(void)
{
	// calculate actual padding values to use.
	float slideTrackXPadding = d_decrease->getAbsoluteWidth() + d_frameLeftSize;

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= ceguimax(0.0f, getAbsoluteWidth() - (2 * slideTrackXPadding));
	float thumbWidth	= (d_documentSize <= d_pageSize) ? slideExtent : slideExtent * d_pageSize / d_documentSize;
	slideExtent -= thumbWidth;

	// make sure thumb is not too small
	if (thumbWidth < MinThumbWidth)
	{
		thumbWidth = MinThumbWidth;
	}

	d_thumb->setSize(Absolute, Size(thumbWidth, d_increase->getAbsoluteHeight()));
	d_thumb->setHorzRange(absoluteToRelativeX(slideTrackXPadding), absoluteToRelativeX(slideTrackXPadding + slideExtent));
	d_thumb->setPosition(Absolute, Point(slideTrackXPadding + (d_position * (slideExtent / posExtent)), d_frameTopSize));
}


/*************************************************************************
	return value that best represents current scroll bar position given
	the current location of the thumb.
*************************************************************************/
float WLHorzScrollbar::getValueFromThumb(void) const
{
	// calculate actual padding values to use.
	float slideTrackXPadding = d_decrease->getAbsoluteWidth() + d_frameLeftSize;

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= ceguimax(0.0f, getAbsoluteWidth() - (2 * slideTrackXPadding) - d_thumb->getAbsoluteWidth());

	return	(d_thumb->getAbsoluteXPosition() - slideTrackXPadding) / (slideExtent / posExtent);
}


/*************************************************************************
	Given window location \a pt, return a value indicating what change
	should be made to the scroll bar.
*************************************************************************/
float WLHorzScrollbar::getAdjustDirectionFromPoint(const Point& pt) const
{
	Rect absrect(d_thumb->getUnclippedPixelRect());

	if (pt.d_x < absrect.d_left)
	{
		return -1.0f;
	}
	else if (pt.d_x > absrect.d_right)
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
void WLHorzScrollbar::drawSelf(float z)
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
	ColourRect colours(BackgroundColour);
	colours.setAlpha(getEffectiveAlpha());

	// draw background image
	d_background->draw(absrect, z, clipper, colours);

	// draw container
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	d_frame.draw(pos, clipper);
}


/*************************************************************************
	Store sizes of frame edges
*************************************************************************/
void WLHorzScrollbar::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= iset->getImageWidth(LeftFrameImageName);
	d_frameRightSize	= iset->getImageWidth(RightFrameImageName);
	d_frameTopSize		= iset->getImageHeight(TopFrameImageName);
	d_frameBottomSize	= iset->getImageHeight(BottomFrameImageName);
}


/*************************************************************************
	Handler for when button size is changed
*************************************************************************/
void WLHorzScrollbar::onSized(WindowEventArgs& e)
{
	// default processing
	Scrollbar::onSized(e);

	// update frame size.
	d_frame.setSize(getAbsoluteSize());

	e.handled = true;
}


/*************************************************************************
	Handler for when alpha changes.
*************************************************************************/
void WLHorzScrollbar::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing.
	Scrollbar::onAlphaChanged(e);

	ColourRect cols(d_frame.getColours());
	cols.setAlpha(getEffectiveAlpha());
	d_frame.setColours(cols);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLHorzScrollbar
*************************************************************************/
Window* WLHorzScrollbarFactory::createWindow(const String& name)
{
	return new WLHorzScrollbar(d_type, name);
}

} // End of  CEGUI namespace section
