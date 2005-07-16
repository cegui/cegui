/************************************************************************
	filename: 	WLFrameWindow.cpp
	created:	9/8/2004
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
#include "WLFrameWindow.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "CEGUIFont.h"
#include "WLTitlebar.h"
#include "WLCloseButton.h"

#include <math.h>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLFrameWindow::WidgetTypeName[]					= "WindowsLook/FrameWindow";

// image stuff
const utf8	WLFrameWindow::ImagesetName[]					= "WindowsLook";
const utf8	WLFrameWindow::TopLeftFrameImageName[]			= "WindowFrameTopLeft";
const utf8	WLFrameWindow::TopRightFrameImageName[]			= "WindowFrameTopRight";
const utf8	WLFrameWindow::BottomLeftFrameImageName[]		= "WindowFrameBottomLeft";
const utf8	WLFrameWindow::BottomRightFrameImageName[]		= "WindowFrameBottomRight";
const utf8	WLFrameWindow::LeftFrameImageName[]				= "WindowFrameLeft";
const utf8	WLFrameWindow::RightFrameImageName[]			= "WindowFrameRight";
const utf8	WLFrameWindow::TopFrameImageName[]				= "WindowFrameTop";
const utf8	WLFrameWindow::BottomFrameImageName[]			= "WindowFrameBottom";
const utf8	WLFrameWindow::ClientBrushImageName[]			= "Background";

const utf8	WLFrameWindow::CloseButtonNormalImageName[]		= "CloseButtonNormal";
const utf8	WLFrameWindow::CloseButtonHoverImageName[]		= "CloseButtonHover";
const utf8	WLFrameWindow::CloseButtonPushedImageName[]		= "CloseButtonPushed";

// cursor images
const utf8	WLFrameWindow::NormalCursorImageName[]			= "MouseArrow";
const utf8	WLFrameWindow::NorthSouthCursorImageName[]		= "MouseNoSoCursor";
const utf8	WLFrameWindow::EastWestCursorImageName[]		= "MouseEsWeCursor";
const utf8	WLFrameWindow::NWestSEastCursorImageName[]		= "MouseNwSeCursor";
const utf8	WLFrameWindow::NEastSWestCursorImageName[]		= "MouseNeSwCursor";

// window type stuff
const utf8*	WLFrameWindow::TitlebarType			= WLTitlebar::WidgetTypeName;
const utf8*	WLFrameWindow::CloseButtonType		= WLCloseButton::WidgetTypeName;

// layout constants
const float	WLFrameWindow::TitlebarXOffset			= 0;
const float	WLFrameWindow::TitlebarYOffset			= 0;
const float	WLFrameWindow::TitlebarTextPadding		= 12;

// colours
const colour WLFrameWindow::ActiveColour			= 0xA7C7FF;
const colour WLFrameWindow::InactiveColour			= 0xEFEFEF;
const colour WLFrameWindow::ClientTopLeftColour		= 0xDFDFF5;
const colour WLFrameWindow::ClientTopRightColour	= 0xDFEFF5;
const colour WLFrameWindow::ClientBottomLeftColour	= 0xF4F3F5;
const colour WLFrameWindow::ClientBottomRightColour	= 0xF0F0F5;


/*************************************************************************
	Constructor
*************************************************************************/
WLFrameWindow::WLFrameWindow(const String& type, const String& name) :
	FrameWindow(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	storeFrameSizes();

	d_frame.setImages(
		NULL, //&iset->getImage(TopLeftFrameImageName),
		NULL, //&iset->getImage(TopRightFrameImageName),
		&iset->getImage(BottomLeftFrameImageName),
		&iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName),
		NULL, //&iset->getImage(TopFrameImageName),
		&iset->getImage(RightFrameImageName),
		&iset->getImage(BottomFrameImageName)
	);

	updateFrameColours();

	// setup client area clearing brush
	d_clientbrush.setImage(&iset->getImage(ClientBrushImageName));
	d_clientbrush.setPosition(Point(d_frameLeftSize, d_frameTopSize));
	d_clientbrush.setHorzFormatting(RenderableImage::HorzStretched);
	d_clientbrush.setVertFormatting(RenderableImage::VertStretched);

	// initialise client area colours
	d_clientbrush.setColours(
		0xFF000000 | ClientTopLeftColour, 
		0xFF000000 | ClientTopRightColour, 
		0xFF000000 | ClientBottomLeftColour, 
		0xFF000000 | ClientBottomRightColour
	);

	// setup cursor images for this window.
	setMouseCursor(&iset->getImage(NormalCursorImageName));
	d_nsSizingCursor = &iset->getImage(NorthSouthCursorImageName);
	d_ewSizingCursor = &iset->getImage(EastWestCursorImageName);
	d_nwseSizingCursor = &iset->getImage(NWestSEastCursorImageName);
	d_neswSizingCursor = &iset->getImage(NEastSWestCursorImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
WLFrameWindow::~WLFrameWindow(void)
{
}


/*************************************************************************
	Return a Rect object that describes, unclipped, the inner rectangle
	for this window.	
*************************************************************************/
Rect WLFrameWindow::getUnclippedInnerRect(void) const
{
    if (d_rolledup)
        return Rect(0,0,0,0);

	Rect tmp(getUnclippedPixelRect());

	if (isFrameEnabled())
	{
		Point pos(d_frame.getPosition());

		tmp.d_left		+= d_frameLeftSize;
		tmp.d_right		-= d_frameRightSize;
		tmp.d_top		+= pos.d_y;

		if (!isTitleBarEnabled())
		{
			tmp.d_top	+= d_frameTopSize;
		}

		tmp.d_bottom	-= d_frameBottomSize;
	}

	return tmp;
}


/*************************************************************************
	Create a control based upon the Titlebar base class to be used as
	the title bar for this window.
*************************************************************************/
Titlebar* WLFrameWindow::createTitlebar(const String& name) const
{
	Titlebar* tbar = (Titlebar*)WindowManager::getSingleton().createWindow(TitlebarType, name);
	tbar->setMetricsMode(Absolute);
	tbar->setPosition(Point(TitlebarXOffset, TitlebarYOffset));

	return tbar;
}


/*************************************************************************
	Create a control based upon the PushButton base class, to be used as
	the close button for the window.
*************************************************************************/
PushButton* WLFrameWindow::createCloseButton(const String& name) const
{
	WLButton* btn = (WLButton*)WindowManager::getSingleton().createWindow(CloseButtonType, name);

	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);
	
	// setup close button imagery
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(CloseButtonNormalImageName));
	img.setColours(ColourRect(0xFFFFFFFF));
	btn->setNormalImage(&img);
	img.setColours(ColourRect(0x7F3FAFAF));
	btn->setDisabledImage(&img);
	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(CloseButtonHoverImageName));
	img.setColours(ColourRect(0xFFFFFFFF));
	btn->setHoverImage(&img);
	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(CloseButtonPushedImageName));
	img.setColours(ColourRect(0xFFFFFFFF));
	btn->setPushedImage(&img);

	btn->setAlpha(0.5f);
	btn->setMetricsMode(Absolute);
	btn->setAlwaysOnTop(true);
	
	return btn;
}


/*************************************************************************
	Setup size and position for the title bar and close button widgets
	attached to this window
*************************************************************************/
void WLFrameWindow::performChildWindowLayout()
{
    FrameWindow::performChildWindowLayout();

	ImagesetManager& ismgr = ImagesetManager::getSingleton();

	// calculate and set size of title bar
	Size titleSz;
	titleSz.d_height = d_titlebar->getFont()->getLineSpacing() + TitlebarTextPadding;
	titleSz.d_width	 = getAbsoluteWidth();
	d_titlebar->setSize(titleSz);

	// set size of close button to be the same as the size of the imagery used to render it.
	float closeSize = ismgr.getImageset(ImagesetName)->getImage(CloseButtonNormalImageName).getWidth();
	d_closeButton->setSize(Size(closeSize, closeSize));

	// calculate position for close button (somewhere over the end of the titlbar)
	float closeX = titleSz.d_width - closeSize - 
		ismgr.getImageset(WLTitlebar::ImagesetName)->getImage(WLTitlebar::RightFrameImageName).getWidth();

	float closeY = TitlebarYOffset + ((titleSz.d_height - closeSize) / 2);

	d_closeButton->setPosition(Point(closeX, closeY));
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void WLFrameWindow::drawSelf(float z)
{
    // do nothing when rolled up.
    if (d_rolledup)
        return;
    
	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	Vector3 pos(absrect.d_left, absrect.d_top, z);

	d_clientbrush.draw(pos, getInnerRect());

	if (isFrameEnabled())
	{
		d_frame.draw(pos, getPixelRect());
	}

}

/*************************************************************************
	Handler called when window is sized
*************************************************************************/
void WLFrameWindow::onSized(WindowEventArgs& e)
{
	// MUST call base class handler no matter what.  This is now required 100%
	FrameWindow::onSized(e);

	Rect area(getUnclippedPixelRect());
	Size newsz(area.getWidth(), area.getHeight());

	//
	// adjust frame and client area rendering objects so that the title bar is outside the frame area.
	//
	float frame_offset = 0;

	// if title bar is active frame is offset by the height of the title bar
	if (isTitleBarEnabled())
	{
		frame_offset = d_titlebar->getUnclippedPixelRect().getHeight();
	}
	// if no title bar, measure the close button instead (which will look crap, actually).
	else if (isCloseButtonEnabled())
	{
		frame_offset = d_closeButton->getUnclippedPixelRect().getHeight();
	}

	// move frame into position
	Point pos(0, frame_offset);
	d_frame.setPosition(pos);

	// adjust size of frame
	newsz.d_height -= frame_offset;
	d_frame.setSize(newsz);

	// adjust position and size of client so it is within the frame	if (isFrameEnabled())
	{
		pos.d_x += d_frameLeftSize;
		newsz.d_width	-= (d_frameLeftSize + d_frameRightSize);
		newsz.d_height	-= d_frameBottomSize;

		if (!isTitleBarEnabled())
		{
			pos.d_y += d_frameTopSize;
			newsz.d_height -= d_frameTopSize;
		}
	}

	d_clientbrush.setSize(newsz);
	d_clientbrush.setPosition(pos);
}


/*************************************************************************
	Handler for alpha value changes
*************************************************************************/
void WLFrameWindow::onAlphaChanged(WindowEventArgs& e)
{
	FrameWindow::onAlphaChanged(e);

	// update alpha values for the frame and client brush
	float alpha = getEffectiveAlpha();

	ColourRect cr;
	cr = d_frame.getColours();
	cr.setAlpha(alpha);
	d_frame.setColours(cr);

	cr = d_clientbrush.getColours();
	cr.setAlpha(alpha);
	d_clientbrush.setColours(cr);
}


/*************************************************************************
    Handler for when clip mode changes
*************************************************************************/
void WLFrameWindow::onClippingChanged(WindowEventArgs& e)
{
    FrameWindow::onClippingChanged(e);

    // set same mode on the titlebar and close button
    if (d_titlebar)
    {
        d_titlebar->setClippedByParent(d_clippedByParent);
    }
    if (d_closeButton)
    {
        d_closeButton->setClippedByParent(d_clippedByParent);
    }
}


/*************************************************************************
	Handler for when window is activated
*************************************************************************/
void WLFrameWindow::onActivated(ActivationEventArgs& e)
{
	Window::onActivated(e);
	updateFrameColours();
	d_closeButton->setAlpha(1.0f);
}


/*************************************************************************
	Handler for when window is deactivated
*************************************************************************/
void WLFrameWindow::onDeactivated(ActivationEventArgs& e)
{
	Window::onDeactivated(e);
	updateFrameColours();
	d_closeButton->setAlpha(0.5f);
}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void WLFrameWindow::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	const Image* img;
	img = &iset->getImage(LeftFrameImageName);
	d_frameLeftSize = img->getWidth() + fabs(img->getOffsetX());

	img = &iset->getImage(RightFrameImageName);
	d_frameRightSize = img->getWidth() + fabs(img->getOffsetX());

	img = &iset->getImage(TopFrameImageName);
	d_frameTopSize = img->getHeight() + fabs(img->getOffsetY());

	img = &iset->getImage(BottomFrameImageName);
	d_frameBottomSize = img->getHeight() + fabs(img->getOffsetY());
}


/*************************************************************************
	Return a Rect that describes, in window relative pixel co-ordinates,
	the outer edge of the sizing area for this window.
*************************************************************************/
Rect WLFrameWindow::getSizingRect(void) const
{
	return d_frame.getRect();
}


/*************************************************************************
	Initialises the Window based object ready for use.
*************************************************************************/
void WLFrameWindow::initialise(void)
{
	// Call base class to create component widgets and "do whatever"
	FrameWindow::initialise();

	// subscribe to enable/disable events on title bar since we need something a little more than that.
	d_titlebar->subscribeEvent(Window::EventDisabled, Event::Subscriber(&CEGUI::WLFrameWindow::componentDisabledHandler, this));
	d_titlebar->subscribeEvent(Window::EventEnabled, Event::Subscriber(&CEGUI::WLFrameWindow::componentEnabledHandler, this));

	// subscribe to enable/disable events on close button since we need something a little more than that.
	d_closeButton->subscribeEvent(Window::EventDisabled, Event::Subscriber(&CEGUI::WLFrameWindow::componentDisabledHandler, this));
	d_closeButton->subscribeEvent(Window::EventEnabled, Event::Subscriber(&CEGUI::WLFrameWindow::componentEnabledHandler, this));
}


/*************************************************************************
	handler used for when the title bar or close button are disabled.	
*************************************************************************/
bool WLFrameWindow::componentDisabledHandler(const EventArgs& e)
{
	((WindowEventArgs&)e).window->hide();

	// update frame images if the title bar has been removed
	if (((WindowEventArgs&)e).window == d_titlebar)
	{
		Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

		d_frame.setImages(&iset->getImage(TopLeftFrameImageName), &iset->getImage(TopRightFrameImageName),
			&iset->getImage(BottomLeftFrameImageName), &iset->getImage(BottomRightFrameImageName),
			&iset->getImage(LeftFrameImageName), &iset->getImage(TopFrameImageName), 
			&iset->getImage(RightFrameImageName), &iset->getImage(BottomFrameImageName));
	}

	// update for possible changed frame size and layout
	WindowEventArgs args(this);
	onSized(args);

	return true;
}


/*************************************************************************
	handler used for when the title bar or close button are enabled.
*************************************************************************/
bool WLFrameWindow::componentEnabledHandler(const EventArgs& e)
{
	((WindowEventArgs&)e).window->show();

	// update frame images if the title bar has been displayed
	if (((WindowEventArgs&)e).window == d_titlebar)
	{
		Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

		d_frame.setImages(NULL, NULL,
			&iset->getImage(BottomLeftFrameImageName), &iset->getImage(BottomRightFrameImageName),
			&iset->getImage(LeftFrameImageName), NULL, 
			&iset->getImage(RightFrameImageName), &iset->getImage(BottomFrameImageName));
	}

	// update for possible changed frame size and layout
	WindowEventArgs args(this);
	onSized(args);

	return true;
}


/*************************************************************************
	Set frame colours for current active / inactive state	
*************************************************************************/
void WLFrameWindow::updateFrameColours(void)
{
	// set active colours
	colour colval(isActive() ? ActiveColour : InactiveColour);
	colval.setAlpha(getEffectiveAlpha());

	d_frame.setColours(colval, colval, colval, colval);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLFrameWindow	
*************************************************************************/
Window* WLFrameWindowFactory::createWindow(const String& name)
{
	return new WLFrameWindow(d_type, name);
}

} // End of  CEGUI namespace section
