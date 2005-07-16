/************************************************************************
	filename: 	WLSlider.cpp
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
#include "WLSlider.h"
#include "WLSliderThumb.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLSlider::WidgetTypeName[]				= "WindowsLook/Slider";

// Image names
const utf8	WLSlider::ImagesetName[]				= "WindowsLook";
const utf8	WLSlider::TrackLeftImageName[]			= "SliderTrackLeft";
const utf8	WLSlider::TrackMiddleImageName[]		= "SlidertrackMiddle";
const utf8	WLSlider::TrackRightImageName[]			= "SliderTrackRight";
const utf8	WLSlider::CalibrationMarkImageName[]	= "SliderTick";
const utf8	WLSlider::MouseCursorImageName[]		= "MouseArrow";

// window type stuff
const utf8*	WLSlider::ThumbType = WLSliderThumb::WidgetTypeName;

// defaults
const float WLSlider::DefaultTickFrequency = 5;


/*************************************************************************
	Constructor for WindowsLook slider widgets
*************************************************************************/
WLSlider::WLSlider(const String& type, const String& name) :
	Slider(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	d_trackLeftImage	= &iset->getImage(TrackLeftImageName);
	d_trackMiddleImage	= &iset->getImage(TrackMiddleImageName);
	d_trackRightImage	= &iset->getImage(TrackRightImageName);
	d_calibrationTickImage	= &iset->getImage(CalibrationMarkImageName);

	setMouseCursor(&iset->getImage(MouseCursorImageName));

	d_calibrationFreq = DefaultTickFrequency;
}


/*************************************************************************
	Destructor for WindowsLook slider widgets
*************************************************************************/
WLSlider::~WLSlider(void)
{
}


/*************************************************************************
	create a Thumb based widget to use as the thumb for this slider.	
*************************************************************************/
Thumb* WLSlider::createThumb(const String& name) const
{
	Thumb* tmb = (Thumb*)WindowManager::getSingleton().createWindow(ThumbType, name);
	tmb->setHorzFree(true);

	// set size for thumb
	tmb->setMetricsMode(Absolute);
	tmb->setSize(ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(WLSliderThumb::NormalImageName).getSize());

	return tmb;
}


/*************************************************************************
	layout the slider component widgets
*************************************************************************/
void WLSlider::performChildWindowLayout()
{
    Slider::performChildWindowLayout();
	updateThumb();
}


/*************************************************************************
	update the size and location of the thumb to properly represent the
	current state of the slider	
*************************************************************************/
void WLSlider::updateThumb(void)
{
	float slideExtent	= getAbsoluteWidth()- d_thumb->getAbsoluteWidth();
	
	d_thumb->setHorzRange(0, slideExtent);
	d_thumb->setPosition(Point(d_value * (slideExtent / d_maxValue), getAbsoluteHeight() - d_thumb->getAbsoluteHeight()));
}


/*************************************************************************
	return value that best represents current slider value given the
	current location of the thumb.	
*************************************************************************/
float WLSlider::getValueFromThumb(void) const
{
	return d_thumb->getAbsoluteXPosition() / ((getAbsoluteWidth() - d_thumb->getAbsoluteWidth()) / d_maxValue);
}


/*************************************************************************
	Given window location 'pt', return a value indicating what change
	should be made to the slider.
*************************************************************************/
float WLSlider::getAdjustDirectionFromPoint(const Point& pt) const
{
	Rect absrect(d_thumb->getUnclippedPixelRect());

	if (pt.d_x < absrect.d_left)
	{
		return -1;
	}
	else if (pt.d_x > absrect.d_right)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*************************************************************************
	Perform the actual rendering for this Window.	
*************************************************************************/
void WLSlider::drawSelf(float z)
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

	// render track
	float leftWidth		= d_trackLeftImage->getWidth();
	float rightWidth	= d_trackRightImage->getWidth();
	float midWidth		= absrect.getWidth() - leftWidth - rightWidth;
	float trackY		= absrect.d_bottom - PixelAligned(d_thumb->getAbsoluteHeight() * 0.5f);

	Vector3 pos(absrect.d_left, trackY, z);
	Size	sz(leftWidth, d_trackMiddleImage->getHeight());
	d_trackLeftImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = midWidth;
	d_trackMiddleImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = rightWidth;
	d_trackRightImage->draw(pos, sz, clipper, colours);

	// render calibration / tick marks
	if (d_calibrationFreq > 0)
	{
		float spacing	= PixelAligned((((getAbsoluteWidth() - d_thumb->getAbsoluteWidth()) / d_maxValue) * d_step) * d_calibrationFreq);
		uint  tickcount = (uint)(getAbsoluteWidth() / spacing);

		pos.d_x = absrect.d_left + PixelAligned(d_thumb->getAbsoluteWidth() * 0.5f);
		pos.d_y = absrect.d_top + (d_thumb->getAbsoluteYPosition() - d_calibrationTickImage->getHeight());

		for (uint lc = 0; lc <= tickcount; ++lc)
		{
			d_calibrationTickImage->draw(pos, clipper, colours);
			pos.d_x += spacing;
		}

	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLSlider
*************************************************************************/
Window* WLSliderFactory::createWindow(const String& name)
{
	return new WLSlider(d_type, name);
}

} // End of  CEGUI namespace section
