/************************************************************************
	filename: 	TLSlider.cpp
	created:	22/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Slider widget
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
#include "TLSlider.h"
#include "TLSliderThumb.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// Image names
const utf8	TLSlider::ImagesetName[]		= "TaharezImagery";
const utf8	TLSlider::ContainerImageName[]	= "VertSliderBody";

// window type stuff
const utf8	TLSlider::ThumbType[]			= "Taharez Slider Thumb";

// layout constants
const float	TLSlider::ContainerPaddingX		= 3;


/*************************************************************************
	Constructor for Taharez slider widgets
*************************************************************************/
TLSlider::TLSlider(const String& type, const String& name) :
	Slider(type, name)
{
	d_container = &ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(ContainerImageName);
}


/*************************************************************************
	Destructor for Taharez slider widgets
*************************************************************************/
TLSlider::~TLSlider(void)
{
}


/*************************************************************************
	create a Thumb based widget to use as the thumb for this slider.	
*************************************************************************/
Thumb* TLSlider::createThumb(void) const
{
	Thumb* tmb = (Thumb*)WindowManager::getSingleton().createWindow(ThumbType, getName() + "__auto_thumb__");
	tmb->setVertFree(true);

	// set size for thumb
	float height = ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(TLSliderThumb::NormalImageName).getHeight();
	height /=  d_container->getHeight();
	tmb->setSize(Size(1.0f, height));

	return tmb;
}


/*************************************************************************
	layout the slider component widgets
*************************************************************************/
void TLSlider::layoutComponentWidgets(void)
{
	updateThumb();
}


/*************************************************************************
	update the size and location of the thumb to properly represent the
	current state of the slider	
*************************************************************************/
void TLSlider::updateThumb(void)
{
	float fltVal		= (float)d_value;
	float posExtent		= (float)(d_maxValue - 1);
	float slideExtent	= getAbsoluteHeight()- d_thumb->getAbsoluteHeight();
	
	d_thumb->setVertRange(0, absoluteToRelativeY_impl(this, slideExtent));
	d_thumb->setPosition(Point(0, absoluteToRelativeY_impl(this, slideExtent - (fltVal * (slideExtent / posExtent)))));
}


/*************************************************************************
	return value that best represents current slider value given the
	current location of the thumb.	
*************************************************************************/
ulong TLSlider::getValueFromThumb(void) const
{
	float posExtent		= (float)d_maxValue;
	float slideExtent	= getAbsoluteHeight()- d_thumb->getAbsoluteHeight();

	return getMaxValue() - (ulong)(d_thumb->getAbsoluteYPosition() / (slideExtent / posExtent));
}


/*************************************************************************
	Given window location 'pt', return a value indicating what change
	should be made to the slider.
*************************************************************************/
int TLSlider::getAdjustDirectionFromPoint(const Point& pt) const
{
	Rect absrect(d_thumb->getUnclippedPixelRect());

	if (pt.d_y < absrect.d_top)
	{
		return 1;
	}
	else if (pt.d_y > absrect.d_bottom)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


/*************************************************************************
	Perform the actual rendering for this Window.	
*************************************************************************/
void TLSlider::drawSelf(float z)
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

	// adjust rect so thumb will protrude a little at the sides
	absrect.d_left	+= ContainerPaddingX;
	absrect.d_right	-= ContainerPaddingX;

	// draw the image
	d_container->draw(absrect, z, clipper, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLSlider
*************************************************************************/
Window* TLSliderFactory::createWindow(const String& name)
{
	TLSlider* wnd = new TLSlider(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
