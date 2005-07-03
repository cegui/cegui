/************************************************************************
	filename: 	WLHorzScrollbarThumb.cpp
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
#include "WLHorzScrollbarThumb.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLHorzScrollbarThumb::WidgetTypeName[]					= "WindowsLook/HorizontalScrollbarThumb";

const utf8	WLHorzScrollbarThumb::ImagesetName[]					= "WindowsLook";
const utf8	WLHorzScrollbarThumb::BackgroundImageName[]				= "Background";
const utf8	WLHorzScrollbarThumb::NormalLeftImageName[]				= "ButtonNormalLeft";
const utf8	WLHorzScrollbarThumb::NormalRightImageName[]			= "ButtonNormalRight";
const utf8	WLHorzScrollbarThumb::NormalTopImageName[]				= "ButtonNormalTop";
const utf8	WLHorzScrollbarThumb::NormalBottomImageName[]			= "ButtonNormalBottom";
const utf8	WLHorzScrollbarThumb::NormalTopLeftImageName[]			= "ButtonNormalTopLeft";
const utf8	WLHorzScrollbarThumb::NormalTopRightImageName[]			= "ButtonNormalTopRight";
const utf8	WLHorzScrollbarThumb::NormalBottomLeftImageName[]		= "ButtonNormalBottomLeft";
const utf8	WLHorzScrollbarThumb::NormalBottomRightImageName[]		= "ButtonNormalBottomRight";
const utf8	WLHorzScrollbarThumb::HoverLeftImageName[]				= "ButtonHoverLeft";
const utf8	WLHorzScrollbarThumb::HoverRightImageName[]				= "ButtonHoverRight";
const utf8	WLHorzScrollbarThumb::HoverTopImageName[]				= "ButtonHoverTop";
const utf8	WLHorzScrollbarThumb::HoverBottomImageName[]			= "ButtonHoverBottom";
const utf8	WLHorzScrollbarThumb::HoverTopLeftImageName[]			= "ButtonHoverTopLeft";
const utf8	WLHorzScrollbarThumb::HoverTopRightImageName[]			= "ButtonHoverTopRight";
const utf8	WLHorzScrollbarThumb::HoverBottomLeftImageName[]		= "ButtonHoverBottomLeft";
const utf8	WLHorzScrollbarThumb::HoverBottomRightImageName[]		= "ButtonHoverBottomRight";
const utf8	WLHorzScrollbarThumb::PushedLeftImageName[]				= "ButtonPushedLeft";
const utf8	WLHorzScrollbarThumb::PushedRightImageName[]			= "ButtonPushedRight";
const utf8	WLHorzScrollbarThumb::PushedTopImageName[]				= "ButtonPushedTop";
const utf8	WLHorzScrollbarThumb::PushedBottomImageName[]			= "ButtonPushedBottom";
const utf8	WLHorzScrollbarThumb::PushedTopLeftImageName[]			= "ButtonPushedTopLeft";
const utf8	WLHorzScrollbarThumb::PushedTopRightImageName[]			= "ButtonPushedTopRight";
const utf8	WLHorzScrollbarThumb::PushedBottomLeftImageName[]		= "ButtonPushedBottomLeft";
const utf8	WLHorzScrollbarThumb::PushedBottomRightImageName[]		= "ButtonPushedBottomRight";
const utf8	WLHorzScrollbarThumb::GripperImageName[]				= "HorzScrollbarGrip";
const utf8	WLHorzScrollbarThumb::MouseCursorImageName[]			= "MouseArrow";

// colours
const colour WLHorzScrollbarThumb::NormalPrimaryColour		= 0xAFAFAF;
const colour WLHorzScrollbarThumb::NormalSecondaryColour	= 0xFFFFFF;
const colour WLHorzScrollbarThumb::HoverPrimaryColour		= 0xCFD9CF;
const colour WLHorzScrollbarThumb::HoverSecondaryColour		= 0xF2FFF2;
const colour WLHorzScrollbarThumb::PushedPrimaryColour		= 0xAFAFAF;
const colour WLHorzScrollbarThumb::PushedSecondaryColour	= 0xFFFFFF;
const colour WLHorzScrollbarThumb::DisabledPrimaryColour	= 0x999999;
const colour WLHorzScrollbarThumb::DisabledSecondaryColour	= 0x999999;

// layout related constants
const float	WLHorzScrollbarThumb::MinimumWidthWithGripRatio	= 2.0f;


/*************************************************************************
	Constructor
*************************************************************************/
WLHorzScrollbarThumb::WLHorzScrollbarThumb(const String& type, const String& name) :
	Thumb(type, name)
{
	storeFrameSizes();

	// setup frames & image pointers
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_normalFrame.setImages(
		&iset->getImage(NormalTopLeftImageName),
		&iset->getImage(NormalTopRightImageName),
		&iset->getImage(NormalBottomLeftImageName),
		&iset->getImage(NormalBottomRightImageName),
		&iset->getImage(NormalLeftImageName),
		&iset->getImage(NormalTopImageName),
		&iset->getImage(NormalRightImageName),
		&iset->getImage(NormalBottomImageName)
		);

	d_hoverFrame.setImages(
		&iset->getImage(HoverTopLeftImageName),
		&iset->getImage(HoverTopRightImageName),
		&iset->getImage(HoverBottomLeftImageName),
		&iset->getImage(HoverBottomRightImageName),
		&iset->getImage(HoverLeftImageName),
		&iset->getImage(HoverTopImageName),
		&iset->getImage(HoverRightImageName),
		&iset->getImage(HoverBottomImageName)
		);

	d_pushedFrame.setImages(
		&iset->getImage(PushedTopLeftImageName),
		&iset->getImage(PushedTopRightImageName),
		&iset->getImage(PushedBottomLeftImageName),
		&iset->getImage(PushedBottomRightImageName),
		&iset->getImage(PushedLeftImageName),
		&iset->getImage(PushedTopImageName),
		&iset->getImage(PushedRightImageName),
		&iset->getImage(PushedBottomImageName)
		);

	d_background	= &iset->getImage(BackgroundImageName);
	d_gripperImage	= &iset->getImage(GripperImageName);

	// set mouse for this widget.
	setMouseCursor(&iset->getImage(MouseCursorImageName));
}


/*************************************************************************
	Destructor
*************************************************************************/
WLHorzScrollbarThumb::~WLHorzScrollbarThumb(void)
{
}


/*************************************************************************
	render the thumb in the normal state.
*************************************************************************/
void WLHorzScrollbarThumb::drawNormal(float z)
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
	ColourRect colours(NormalPrimaryColour, NormalSecondaryColour, NormalSecondaryColour, NormalPrimaryColour);
	colours.setAlpha(getEffectiveAlpha());

	// draw background image
	Rect bkRect(absrect);
	bkRect.d_left	+= d_frameLeftSize;
	bkRect.d_right	-= d_frameRightSize;
	bkRect.d_top	+= d_frameTopSize;
	bkRect.d_bottom	-= d_frameBottomSize;
	d_background->draw(bkRect, z, clipper, colours);

	// draw frame
	d_normalFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);

	// draw gripper if needed
	if (absrect.getWidth() >= d_gripperImage->getWidth() * MinimumWidthWithGripRatio)
	{
		Vector3 gripPos(
			absrect.d_left + ((absrect.getWidth() - d_gripperImage->getWidth()) * 0.5f),
			absrect.d_top + ((absrect.getHeight() - d_gripperImage->getHeight()) * 0.5f),
			z
		);

		d_gripperImage->draw(gripPos, clipper, colours);
	}

}


/*************************************************************************
	render the thumb in the hover / highlighted state.
*************************************************************************/
void WLHorzScrollbarThumb::drawHover(float z)
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
	ColourRect colours(HoverPrimaryColour, HoverSecondaryColour, HoverSecondaryColour, HoverPrimaryColour);
	colours.setAlpha(getEffectiveAlpha());

	// draw background image
	Rect bkRect(absrect);
	bkRect.d_left	+= d_frameLeftSize;
	bkRect.d_right	-= d_frameRightSize;
	bkRect.d_top	+= d_frameTopSize;
	bkRect.d_bottom	-= d_frameBottomSize;
	d_background->draw(bkRect, z, clipper, colours);

	// draw frame
	d_hoverFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);

	// draw gripper if needed
	if (absrect.getWidth() >= d_gripperImage->getWidth() * MinimumWidthWithGripRatio)
	{
		Vector3 gripPos(
			absrect.d_left + ((absrect.getWidth() - d_gripperImage->getWidth()) * 0.5f),
			absrect.d_top + ((absrect.getHeight() - d_gripperImage->getHeight()) * 0.5f),
			z
		);

		d_gripperImage->draw(gripPos, clipper, colours);
	}

}


/*************************************************************************
	render Widget in Pushed state	
*************************************************************************/
void WLHorzScrollbarThumb::drawPushed(float z)
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
	ColourRect colours(PushedPrimaryColour, PushedSecondaryColour, PushedSecondaryColour, PushedPrimaryColour);
	colours.setAlpha(getEffectiveAlpha());

	// draw background image
	Rect bkRect(absrect);
	bkRect.d_left	+= d_frameLeftSize;
	bkRect.d_right	-= d_frameRightSize;
	bkRect.d_top	+= d_frameTopSize;
	bkRect.d_bottom	-= d_frameBottomSize;
	d_background->draw(bkRect, z, clipper, colours);

	// draw frame
	d_pushedFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);

	// draw gripper if needed
	if (absrect.getWidth() >= d_gripperImage->getWidth() * MinimumWidthWithGripRatio)
	{
		Vector3 gripPos(
			absrect.d_left + ((absrect.getWidth() - d_gripperImage->getWidth()) * 0.5f),
			absrect.d_top + ((absrect.getHeight() - d_gripperImage->getHeight()) * 0.5f),
			z
		);

		d_gripperImage->draw(gripPos, clipper, colours);
	}

}


/*************************************************************************
	render the thumb in the disabled state
*************************************************************************/
void WLHorzScrollbarThumb::drawDisabled(float z)
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
	ColourRect colours(DisabledPrimaryColour, DisabledSecondaryColour, DisabledSecondaryColour, DisabledPrimaryColour);
	colours.setAlpha(getEffectiveAlpha());

	// draw background image
	Rect bkRect(absrect);
	bkRect.d_left	+= d_frameLeftSize;
	bkRect.d_right	-= d_frameRightSize;
	bkRect.d_top	+= d_frameTopSize;
	bkRect.d_bottom	-= d_frameBottomSize;
	d_background->draw(bkRect, z, clipper, colours);

	// draw frame
	d_normalFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);

	// draw gripper if needed
	if (absrect.getWidth() >= d_gripperImage->getWidth() * MinimumWidthWithGripRatio)
	{
		Vector3 gripPos(
			absrect.d_left + ((absrect.getWidth() - d_gripperImage->getWidth()) * 0.5f),
			absrect.d_top + ((absrect.getHeight() - d_gripperImage->getHeight()) * 0.5f),
			z
		);

		d_gripperImage->draw(gripPos, clipper, colours);
	}

}


/*************************************************************************
	Store sizes of frame edges
*************************************************************************/
void WLHorzScrollbarThumb::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= iset->getImageWidth(NormalLeftImageName);
	d_frameRightSize	= iset->getImageWidth(NormalRightImageName);
	d_frameTopSize		= iset->getImageHeight(NormalTopImageName);
	d_frameBottomSize	= iset->getImageHeight(NormalBottomImageName);
}


/*************************************************************************
	Handler for when button size is changed
*************************************************************************/
void WLHorzScrollbarThumb::onSized(WindowEventArgs& e)
{
	// default processing
	Thumb::onSized(e);

	Size absSize(getAbsoluteSize());

	// update frame size.
	d_normalFrame.setSize(absSize);
	d_hoverFrame.setSize(absSize);
	d_pushedFrame.setSize(absSize);

	e.handled = true;
}


/*************************************************************************
	Handler for when alpha changes.
*************************************************************************/
void WLHorzScrollbarThumb::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing.
	Thumb::onAlphaChanged(e);

	ColourRect cols;
	float alpha = getEffectiveAlpha();

	cols = d_normalFrame.getColours();
	cols.setAlpha(alpha);
	d_normalFrame.setColours(cols);

	cols = d_hoverFrame.getColours();
	cols.setAlpha(alpha);
	d_hoverFrame.setColours(cols);

	cols = d_pushedFrame.getColours();
	cols.setAlpha(alpha);
	d_pushedFrame.setColours(cols);
}



//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLHorzScrollbarThumb
*************************************************************************/
Window* WLHorzScrollbarThumbFactory::createWindow(const String& name)
{
	return new WLHorzScrollbarThumb(d_type, name);
}

} // End of  CEGUI namespace section
