/************************************************************************
	filename: 	TLFrameWindow.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Look Frame Window class.
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
#include "TLFrameWindow.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "TLTitlebar.h"
#include "TLButton.h"
#include "CEGUIFont.h"

#include <boost/bind.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// image stuff
const utf8	TLFrameWindow::ImagesetName[]					= "TaharezImagery";
const utf8	TLFrameWindow::TopLeftFrameImageName[]			= "WindowTopLeft";
const utf8	TLFrameWindow::TopRightFrameImageName[]			= "WindowTopRight";
const utf8	TLFrameWindow::BottomLeftFrameImageName[]		= "WindowBottomLeft";
const utf8	TLFrameWindow::BottomRightFrameImageName[]		= "WindowBottomRight";
const utf8	TLFrameWindow::LeftFrameImageName[]				= "WindowLeftEdge";
const utf8	TLFrameWindow::RightFrameImageName[]			= "WindowRightEdge";
const utf8	TLFrameWindow::TopFrameImageName[]				= "WindowTopEdge";
const utf8	TLFrameWindow::BottomFrameImageName[]			= "WindowBottomEdge";
const utf8	TLFrameWindow::ClientBrushImageName[]			= "ClientBrush";

const utf8	TLFrameWindow::CloseButtonNormalImageName[]		= "NewCloseButtonNormal";
const utf8	TLFrameWindow::CloseButtonHoverImageName[]		= "NewCloseButtonHover";
const utf8	TLFrameWindow::CloseButtonPushedImageName[]		= "NewCloseButtonPressed";

// cursor images
const utf8	TLFrameWindow::NormalCursorImageName[]			= "MouseTarget";
const utf8	TLFrameWindow::NorthSouthCursorImageName[]		= "MouseNoSoCursor";
const utf8	TLFrameWindow::EastWestCursorImageName[]		= "MouseEsWeCursor";
const utf8	TLFrameWindow::NWestSEastCursorImageName[]		= "MouseNwSeCursor";
const utf8	TLFrameWindow::NEastSWestCursorImageName[]		= "MouseNeSwCursor";


// window type stuff
const utf8	TLFrameWindow::TitlebarType[]		= "Taharez Titlebar";
const utf8	TLFrameWindow::CloseButtonType[]	= "Taharez Close Button";

// layout constants
const float	TLFrameWindow::TitlebarXOffset			= 0;
const float	TLFrameWindow::TitlebarYOffset			= 0;
const float	TLFrameWindow::TitlebarTextPadding		= 8;
const float	TLFrameWindow::TitlebarWidthPercentage	= 66;


/*************************************************************************
	Constructor
*************************************************************************/
TLFrameWindow::TLFrameWindow(const String& type, const String& name) :
	FrameWindow(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frame.setImages(NULL, NULL,
		&iset->getImage(BottomLeftFrameImageName), &iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName), NULL, 
		&iset->getImage(RightFrameImageName), &iset->getImage(BottomFrameImageName));

	storeFrameSizes();

	// setup client area clearing brush
	d_clientbrush.setImage(&iset->getImage(ClientBrushImageName));
	d_clientbrush.setPosition(Point(d_frameLeftSize, d_frameTopSize));
	d_clientbrush.setHorzFormatting(RenderableImage::HorzTiled);
	d_clientbrush.setVertFormatting(RenderableImage::VertTiled);

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
TLFrameWindow::~TLFrameWindow(void)
{
}


/*************************************************************************
	Return a Rect object that describes, unclipped, the inner rectangle
	for this window.	
*************************************************************************/
Rect TLFrameWindow::getUnclippedInnerRect(void) const
{
	Rect tmp(getUnclippedPixelRect());

	if (isFrameEnabled())
	{
		Point pos(d_frame.getPosition());

		tmp.d_left		+= pos.d_x + d_frameLeftSize;
		tmp.d_right		-= d_frameRightSize;
		tmp.d_top		+= pos.d_y + d_frameTopSize;
		tmp.d_bottom	-= d_frameBottomSize;
	}

	return tmp;
}


/*************************************************************************
	Create a control based upon the Titlebar base class to be used as
	the title bar for this window.
*************************************************************************/
Titlebar* TLFrameWindow::createTitlebar(void) const
{
	TLTitlebar* tbar = (TLTitlebar*)WindowManager::getSingleton().createWindow(TitlebarType, getName() + "__auto_titlebar__");
	tbar->setMetricsMode(Absolute);
	tbar->setPosition(Point(TitlebarXOffset, TitlebarYOffset));

	return tbar;
}


/*************************************************************************
	Create a control based upon the PushButton base class, to be used as
	the close button for the window.
*************************************************************************/
PushButton* TLFrameWindow::createCloseButton(void) const
{
	TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(CloseButtonType, getName() + "__auto_closebutton__");

	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);
	
	// setup close button imagery
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(CloseButtonNormalImageName));
	img.setColours(ColourRect(0xFFBBBBBB));
	btn->setNormalImage(&img);
	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(CloseButtonHoverImageName));
	img.setColours(ColourRect(0xFFFFFFFF));
	btn->setHoverImage(&img);
	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(CloseButtonPushedImageName));
	img.setColours(ColourRect(0xFF999999));
	btn->setPushedImage(&img);

	btn->setMetricsMode(Absolute);
	btn->setAlwaysOnTop(true);
	
	return btn;
}


/*************************************************************************
	Setup size and position for the title bar and close button widgets
	attached to this window
*************************************************************************/
void TLFrameWindow::layoutComponentWidgets()
{
	ImagesetManager& ismgr = ImagesetManager::getSingleton();

	// calculate and set size of title bar
	Size titleSz;
	titleSz.d_height = getFont()->getLineSpacing() + TitlebarTextPadding;
	titleSz.d_width	 = isRolledup() ? d_abs_openSize.d_width : d_abs_area.getWidth();
	d_titlebar->setSize(titleSz);

	// set size of close button to be the same as the height for the title bar.
	float closeSize = ismgr.getImageset(ImagesetName)->getImage(CloseButtonNormalImageName).getWidth();
	d_closeButton->setSize(Size(closeSize, closeSize));

	// calculate position for close button (somewhere over the end of the titlbar)
	float closeX = titleSz.d_width - closeSize - 
		ismgr.getImageset(TLTitlebar::ImagesetName)->getImage(TLTitlebar::SysAreaRightImageName).getWidth();

	float closeY = TitlebarYOffset + ((titleSz.d_height - closeSize) / 2);

	d_closeButton->setPosition(Point(closeX, closeY));
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void TLFrameWindow::drawSelf(float z)
{
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
void TLFrameWindow::onSized(WindowEventArgs& e)
{
	// MUST call base class handler no matter what.  This is now required 100%
	FrameWindow::onSized(e);

	Rect area(getUnclippedPixelRect());
	Size newsz(area.getWidth(), area.getHeight());

	//
	// adjust frame and client area rendering objects so that the title bar is outside the frame area.
	//
	float frame_offset = 0;

	// if title bar is active, close button is the same height.
	if (isTitleBarEnabled())
	{
		frame_offset = d_titlebar->getUnclippedPixelRect().getHeight();
	}
	// if no title bar, measure the close button instead.
	else if (isCloseButtonEnabled())
	{
		frame_offset = d_closeButton->getUnclippedPixelRect().getHeight();
	}

	// move frame into position
	Point pos(0, frame_offset);
	d_frame.setPosition(pos);

	// adjust position for client brush
	pos.d_y += d_frameTopSize;
	pos.d_x += d_frameLeftSize;
	d_clientbrush.setPosition(pos);

	// adjust size of frame
	newsz.d_height -= frame_offset;
	d_frame.setSize(newsz);

	// modify size of client so it is within the frame
	if (isFrameEnabled())
	{
		newsz.d_width	-= (d_frameLeftSize + d_frameRightSize);
		newsz.d_height	-= (d_frameTopSize + d_frameBottomSize);
	}

	d_clientbrush.setSize(newsz);
}


/*************************************************************************
	Handler for alpha value changes
*************************************************************************/
void TLFrameWindow::onAlphaChanged(WindowEventArgs& e)
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
	Store the sizes for the frame edges
*************************************************************************/
void TLFrameWindow::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	const Image* img;
	img = &iset->getImage(LeftFrameImageName);
	d_frameLeftSize = img->getWidth() + img->getOffsetX();

	img = &iset->getImage(LeftFrameImageName);
	d_frameRightSize = img->getWidth() + img->getOffsetX();

	img = &iset->getImage(TopFrameImageName);
	d_frameTopSize = img->getHeight() + img->getOffsetY();

	img = &iset->getImage(BottomFrameImageName);
	d_frameBottomSize = img->getHeight() + img->getOffsetY();
}


/*************************************************************************
	Return a Rect that describes, in window relative pixel co-ordinates,
	the outer edge of the sizing area for this window.
*************************************************************************/
Rect TLFrameWindow::getSizingRect(void) const
{
	return d_frame.getRect();
}


/*************************************************************************
	Initialises the Window based object ready for use.
*************************************************************************/
void TLFrameWindow::initialise(void)
{
	// Call base class to create component widgets and "do whatever"
	FrameWindow::initialise();

	// subscribe to enable/disable events on title bar since we need something a little more than that.
	d_titlebar->subscribeEvent(Window::DisabledEvent, boost::bind(&CEGUI::TLFrameWindow::componentDisabledHandler, this, _1));
	d_titlebar->subscribeEvent(Window::EnabledEvent, boost::bind(&CEGUI::TLFrameWindow::componentEnabledHandler, this, _1));

	// subscribe to enable/disable events on close button since we need something a little more than that.
	d_closeButton->subscribeEvent(Window::DisabledEvent, boost::bind(&CEGUI::TLFrameWindow::componentDisabledHandler, this, _1));
	d_closeButton->subscribeEvent(Window::EnabledEvent, boost::bind(&CEGUI::TLFrameWindow::componentEnabledHandler, this, _1));
}


/*************************************************************************
	handler used for when the title bar or close button are disabled.	
*************************************************************************/
void TLFrameWindow::componentDisabledHandler(const EventArgs& e)
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
}


/*************************************************************************
	handler used for when the title bar or close button are enabled.
*************************************************************************/
void TLFrameWindow::componentEnabledHandler(const EventArgs& e)
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
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLFrameWindow	
*************************************************************************/
Window* TLFrameWindowFactory::createWindow(const String& name)
{
	TLFrameWindow* wnd = new TLFrameWindow(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
