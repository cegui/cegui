/************************************************************************
	filename: 	WLVertScrollbarThumb.cpp
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
#include "WLVertScrollbarThumb.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLVertScrollbarThumb::WidgetTypeName[]					= "WindowsLook/VerticalScrollbarThumb";

const utf8	WLVertScrollbarThumb::ImagesetName[]					= "WindowsLook";
const utf8	WLVertScrollbarThumb::BackgroundImageName[]				= "Background";
const utf8	WLVertScrollbarThumb::NormalLeftImageName[]				= "ButtonNormalLeft";
const utf8	WLVertScrollbarThumb::NormalRightImageName[]			= "ButtonNormalRight";
const utf8	WLVertScrollbarThumb::NormalTopImageName[]				= "ButtonNormalTop";
const utf8	WLVertScrollbarThumb::NormalBottomImageName[]			= "ButtonNormalBottom";
const utf8	WLVertScrollbarThumb::NormalTopLeftImageName[]			= "ButtonNormalTopLeft";
const utf8	WLVertScrollbarThumb::NormalTopRightImageName[]			= "ButtonNormalTopRight";
const utf8	WLVertScrollbarThumb::NormalBottomLeftImageName[]		= "ButtonNormalBottomLeft";
const utf8	WLVertScrollbarThumb::NormalBottomRightImageName[]		= "ButtonNormalBottomRight";
const utf8	WLVertScrollbarThumb::HoverLeftImageName[]				= "ButtonHoverLeft";
const utf8	WLVertScrollbarThumb::HoverRightImageName[]				= "ButtonHoverRight";
const utf8	WLVertScrollbarThumb::HoverTopImageName[]				= "ButtonHoverTop";
const utf8	WLVertScrollbarThumb::HoverBottomImageName[]			= "ButtonHoverBottom";
const utf8	WLVertScrollbarThumb::HoverTopLeftImageName[]			= "ButtonHoverTopLeft";
const utf8	WLVertScrollbarThumb::HoverTopRightImageName[]			= "ButtonHoverTopRight";
const utf8	WLVertScrollbarThumb::HoverBottomLeftImageName[]		= "ButtonHoverBottomLeft";
const utf8	WLVertScrollbarThumb::HoverBottomRightImageName[]		= "ButtonHoverBottomRight";
const utf8	WLVertScrollbarThumb::PushedLeftImageName[]				= "ButtonPushedLeft";
const utf8	WLVertScrollbarThumb::PushedRightImageName[]			= "ButtonPushedRight";
const utf8	WLVertScrollbarThumb::PushedTopImageName[]				= "ButtonPushedTop";
const utf8	WLVertScrollbarThumb::PushedBottomImageName[]			= "ButtonPushedBottom";
const utf8	WLVertScrollbarThumb::PushedTopLeftImageName[]			= "ButtonPushedTopLeft";
const utf8	WLVertScrollbarThumb::PushedTopRightImageName[]			= "ButtonPushedTopRight";
const utf8	WLVertScrollbarThumb::PushedBottomLeftImageName[]		= "ButtonPushedBottomLeft";
const utf8	WLVertScrollbarThumb::PushedBottomRightImageName[]		= "ButtonPushedBottomRight";
const utf8	WLVertScrollbarThumb::GripperImageName[]				= "VertScrollbarGrip";
const utf8	WLVertScrollbarThumb::MouseCursorImageName[]			= "MouseArrow";

// colours
const colour WLVertScrollbarThumb::NormalPrimaryColour		= 0xFFAFAFAF;
const colour WLVertScrollbarThumb::NormalSecondaryColour	= 0xFFFFFFFF;
const colour WLVertScrollbarThumb::HoverPrimaryColour		= 0xFFCFD9CF;
const colour WLVertScrollbarThumb::HoverSecondaryColour		= 0xFFF2FFF2;
const colour WLVertScrollbarThumb::PushedPrimaryColour		= 0xFFAFAFAF;
const colour WLVertScrollbarThumb::PushedSecondaryColour	= 0xFFFFFFFF;
const colour WLVertScrollbarThumb::DisabledPrimaryColour	= 0xFF999999;
const colour WLVertScrollbarThumb::DisabledSecondaryColour	= 0xFF999999;

// layout related constants
const float	WLVertScrollbarThumb::MinimumHeightWithGripRatio	= 2.0f;


/*************************************************************************
	Constructor
*************************************************************************/
WLVertScrollbarThumb::WLVertScrollbarThumb(const String& type, const String& name) :
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
WLVertScrollbarThumb::~WLVertScrollbarThumb(void)
{
}


/*************************************************************************
	render the thumb in the normal state.
*************************************************************************/
void WLVertScrollbarThumb::drawNormal(float z)
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
	if (absrect.getHeight() >= d_gripperImage->getHeight() * MinimumHeightWithGripRatio)
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
void WLVertScrollbarThumb::drawHover(float z)
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
	if (absrect.getHeight() >= d_gripperImage->getHeight() * MinimumHeightWithGripRatio)
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
void WLVertScrollbarThumb::drawPushed(float z)
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
	if (absrect.getHeight() >= d_gripperImage->getHeight() * MinimumHeightWithGripRatio)
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
void WLVertScrollbarThumb::drawDisabled(float z)
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
	if (absrect.getHeight() >= d_gripperImage->getHeight() * MinimumHeightWithGripRatio)
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
void WLVertScrollbarThumb::storeFrameSizes(void)
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
void WLVertScrollbarThumb::onSized(WindowEventArgs& e)
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
void WLVertScrollbarThumb::onAlphaChanged(WindowEventArgs& e)
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
	Create, initialise and return a WLVertScrollbarThumb
*************************************************************************/
Window* WLVertScrollbarThumbFactory::createWindow(const String& name)
{
	return new WLVertScrollbarThumb(d_type, name);
}

} // End of  CEGUI namespace section
