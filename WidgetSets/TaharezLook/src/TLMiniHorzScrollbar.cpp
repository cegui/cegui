/************************************************************************
	filename: 	TLMiniHorzScrollbar.cpp
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez mini horizontal scroll bar.
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
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "TLMiniHorzScrollbar.h"
#include "TLMiniHorzScrollbarThumb.h"
#include "TLButton.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLMiniHorzScrollbar::WidgetTypeName[]	= "TaharezLook/HorizontalScrollbar";

// Progress bar image names
const utf8	TLMiniHorzScrollbar::ImagesetName[]						= "TaharezLook";
const utf8	TLMiniHorzScrollbar::ScrollbarBodyImageName[]			= "MiniHorzScrollBarSegment";
const utf8	TLMiniHorzScrollbar::LeftButtonNormalImageName[]		= "MiniHorzScrollLeftNormal";
const utf8	TLMiniHorzScrollbar::LeftButtonHighlightImageName[]		= "MiniHorzScrollLeftHover";
const utf8	TLMiniHorzScrollbar::RightButtonNormalImageName[]		= "MiniHorzScrollRightNormal";
const utf8	TLMiniHorzScrollbar::RightButtonHighlightImageName[]	= "MiniHorzScrollRightHover";

// some layout stuff
const float	TLMiniHorzScrollbar::ThumbPositionY	= 0.15f;
const float	TLMiniHorzScrollbar::ThumbHeight	= 0.7f;				
const float	TLMiniHorzScrollbar::BodyPositionY	= 0.3f;			
const float	TLMiniHorzScrollbar::BodyHeight		= 0.4f;

// type names for the component widgets
const utf8*	TLMiniHorzScrollbar::ThumbWidgetType			= TLMiniHorzScrollbarThumb::WidgetTypeName;
const utf8*	TLMiniHorzScrollbar::IncreaseButtonWidgetType	= TLButton::WidgetTypeName;
const utf8*	TLMiniHorzScrollbar::DecreaseButtonWidgetType	= TLButton::WidgetTypeName;


/*************************************************************************
	Constructor for Taharez mini horizontal scroll bar widgets
*************************************************************************/
TLMiniHorzScrollbar::TLMiniHorzScrollbar(const String& type, const String& name) :
	Scrollbar(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup cache of image pointers
	d_body = &iset->getImage(ScrollbarBodyImageName);
}


/*************************************************************************
	Destructor for Taharez mini horizontal scroll bar widgets
*************************************************************************/
TLMiniHorzScrollbar::~TLMiniHorzScrollbar(void)
{
}


/*************************************************************************
	create a PushButton based widget to use as the increase button for
	this scroll bar.
*************************************************************************/
PushButton* TLMiniHorzScrollbar::createIncreaseButton(const String& name) const
{
	// create the widget
	TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(IncreaseButtonWidgetType, name);

	// perform some initialisation
	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);
	btn->setAlwaysOnTop(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
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
PushButton* TLMiniHorzScrollbar::createDecreaseButton(const String& name) const
{
	// create the widget
	TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(DecreaseButtonWidgetType, name);

	// perform some initialisation
	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);
	btn->setAlwaysOnTop(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
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
Thumb* TLMiniHorzScrollbar::createThumb(const String& name) const
{
	// create the widget
	TLMiniHorzScrollbarThumb* thumb = (TLMiniHorzScrollbarThumb*)WindowManager::getSingleton().createWindow(ThumbWidgetType, name);

	// perform some initialisation
	thumb->setHorzFree(true);
	thumb->setYPosition(ThumbPositionY);
	thumb->setHeight(ThumbHeight);

	return thumb;
}


/*************************************************************************
	layout the scroll bar component widgets
*************************************************************************/
void TLMiniHorzScrollbar::layoutComponentWidgets(void)
{
	Size bsz;
	bsz.d_width = bsz.d_height = getAbsoluteHeight();

	// install button sizes
	d_increase->setSize(absoluteToRelative(bsz));
	d_decrease->setSize(absoluteToRelative(bsz));

	// position buttons
	d_decrease->setPosition(Point(0.0f, 0.0f));
	d_increase->setPosition(Point(absoluteToRelativeX(getAbsoluteWidth() - bsz.d_width), 0.0f));

	// this will configure thumb widget appropriately
	updateThumb();
}


/*************************************************************************
	update the size and location of the thumb to properly represent the
	current state of the scroll bar
*************************************************************************/
void TLMiniHorzScrollbar::updateThumb(void)
{
	// calculate actual padding values to use.
	float slideTrackXPadding = d_decrease->getAbsoluteWidth();

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= ceguimax(0.0f, getAbsoluteWidth() - (2 * slideTrackXPadding) - d_thumb->getAbsoluteWidth());

	// Thumb does not change size with document length, we just need to update position and range
	d_thumb->setHorzRange(absoluteToRelativeX(slideTrackXPadding), absoluteToRelativeX(slideTrackXPadding + slideExtent));
	d_thumb->setXPosition(absoluteToRelativeX(slideTrackXPadding + (d_position * (slideExtent / posExtent))));
}


/*************************************************************************
	return value that best represents current scroll bar position given
	the current location of the thumb.
*************************************************************************/
float TLMiniHorzScrollbar::getValueFromThumb(void) const
{
	// calculate actual padding values to use.
	float slideTrackXPadding = d_decrease->getAbsoluteWidth();

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= getAbsoluteWidth() - (2 * slideTrackXPadding) - d_thumb->getAbsoluteWidth();

	return	(d_thumb->getAbsoluteXPosition() - slideTrackXPadding) / (slideExtent / posExtent);
}


/*************************************************************************
	Given window location 'pt', return a value indicating what change
	should be made to the scroll bar.
*************************************************************************/
float TLMiniHorzScrollbar::getAdjustDirectionFromPoint(const Point& pt) const
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
void TLMiniHorzScrollbar::drawSelf(float z)
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
	// Render bar body
	//
	float button_width = d_decrease->getAbsoluteWidth();

	Vector3 pos(absrect.d_left + button_width, absrect.d_top + PixelAligned(absrect.getHeight() * BodyPositionY), z);
	Size	sz(absrect.getWidth() - PixelAligned(button_width * 0.5f), PixelAligned(absrect.getHeight() * BodyHeight));

	d_body->draw(pos, sz, clipper, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLMiniHorzScrollbar
*************************************************************************/
Window* TLMiniHorzScrollbarFactory::createWindow(const String& name)
{
	return new TLMiniHorzScrollbar(d_type, name);
}

} // End of  CEGUI namespace section
