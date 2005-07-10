/************************************************************************
	filename: 	WLProgressBar.cpp
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
#include "WLProgressBar.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLProgressBar::WidgetTypeName[]				= "WindowsLook/ProgressBar";

// image name constants
const utf8	WLProgressBar::ImagesetName[]				= "WindowsLook";
const utf8	WLProgressBar::TopLeftFrameImageName[]		= "StaticFrameTopLeft";
const utf8	WLProgressBar::TopRightFrameImageName[]		= "StaticFrameTopRight";
const utf8	WLProgressBar::BottomLeftFrameImageName[]	= "StaticFrameBottomLeft";
const utf8	WLProgressBar::BottomRightFrameImageName[]	= "StaticFrameBottomRight";
const utf8	WLProgressBar::LeftFrameImageName[]			= "StaticFrameLeft";
const utf8	WLProgressBar::RightFrameImageName[]		= "StaticFrameRight";
const utf8	WLProgressBar::TopFrameImageName[]			= "StaticFrameTop";
const utf8	WLProgressBar::BottomFrameImageName[]		= "StaticFrameBottom";
const utf8	WLProgressBar::BackgroundImageName[]		= "Background";
const utf8	WLProgressBar::MouseCursorImageName[]		= "MouseArrow";

// colours
colour WLProgressBar::ContainerBackgroundColour		= 0xDFDFDF;
colour WLProgressBar::DefaultProgressColour			= 0x2222FF;

// rendering layers
const uint	WLProgressBar::ContainerLayer	= 0;
const uint	WLProgressBar::ProgressLayer	= 1;


/*************************************************************************
	Constructor for WindowsLook progress bar objects
*************************************************************************/
WLProgressBar::WLProgressBar(const String& type, const String& name) :
	ProgressBar(type, name)
{
	// cache images to be used
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

	// set up colours
	d_progressColour	= DefaultProgressColour;
}


/*************************************************************************
	Destructor for WindowsLook progress bar objects
*************************************************************************/
WLProgressBar::~WLProgressBar(void)
{
}


/*************************************************************************
	Perform rendering for this widget
*************************************************************************/
void WLProgressBar::drawSelf(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	Renderer*	renderer = System::getSingleton().getRenderer();

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// Render the container
	Vector3 pos(absrect.d_left, absrect.d_top, renderer->getZLayer(ContainerLayer));
	d_frame.draw(pos, clipper);

	// calculate inner area rect considering frame
	absrect.d_left		+= d_frameLeftSize;
	absrect.d_top		+= d_frameTopSize;
	absrect.d_right		-= d_frameRightSize;
	absrect.d_bottom	-= d_frameBottomSize;

	// calculate colours to use.
	float alpha_comp = getEffectiveAlpha();
	ColourRect colours(ContainerBackgroundColour);
	colours.setAlpha(alpha_comp);

	// draw background image
	d_background->draw(absrect, pos.d_z, clipper, colours);

	// adjust rect area according to current progress
	absrect.setWidth(PixelAligned(absrect.getWidth() * getProgress()));

	// calculate colours for progress bar itself
	colours.setColours(d_progressColour);
	colours.setAlpha(alpha_comp);
	d_background->draw(absrect, renderer->getZLayer(ProgressLayer), clipper, colours);
}


/*************************************************************************
	Store sizes of frame edges
*************************************************************************/
void WLProgressBar::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= iset->getImageWidth(LeftFrameImageName);
	d_frameRightSize	= iset->getImageWidth(RightFrameImageName);
	d_frameTopSize		= iset->getImageHeight(TopFrameImageName);
	d_frameBottomSize	= iset->getImageHeight(BottomFrameImageName);
}


/*************************************************************************
	Handler for when widget is sized.
*************************************************************************/
void WLProgressBar::onSized(WindowEventArgs& e)
{
	// default processing
	ProgressBar::onSized(e);

	// update frame size.
	d_frame.setSize(getAbsoluteSize());

	e.handled = true;
}


/*************************************************************************
	Handler for when alpha of widget is changed.
*************************************************************************/
void WLProgressBar::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing.
	ProgressBar::onAlphaChanged(e);

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
	Create, initialise and return a WLProgressBar
*************************************************************************/
Window* WLProgressBarFactory::createWindow(const String& name)
{
	return new WLProgressBar(d_type, name);
}

} // End of  CEGUI namespace section
