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
#include "elements/CEGUITitlebar.h"


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

// window type stuff
const utf8	TLFrameWindow::TitlebarType[]		= "Taharez Titlebar";
const utf8	TLFrameWindow::CloseButtonType[]	= "Taharez Button";



/*************************************************************************
	Constructor
*************************************************************************/
TLFrameWindow::TLFrameWindow(const String& type, const String& name) :
	FrameWindow(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup frame images
	d_frame.setImages(&iset->getImage(TopLeftFrameImageName), &iset->getImage(TopRightFrameImageName),
		&iset->getImage(BottomLeftFrameImageName), &iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName), &iset->getImage(TopFrameImageName), 
		&iset->getImage(RightFrameImageName), &iset->getImage(BottomFrameImageName));

	storeFrameSizes();

	// setup client area clearing brush
	d_clientbrush.setImage(&iset->getImage(ClientBrushImageName));
	d_clientbrush.setPosition(Point(d_frameLeftSize, d_frameTopSize));
	d_clientbrush.setHorzFormatting(RenderableImage::HorzTiled);
	d_clientbrush.setVertFormatting(RenderableImage::VertTiled);
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
		tmp.d_left		+= d_frameLeftSize;
		tmp.d_right		-= d_frameRightSize;
		tmp.d_top		+= d_frameTopSize;
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
	return (Titlebar*)WindowManager::getSingleton().createWindow(TitlebarType, getName() + "__auto_titlebar__");
}


/*************************************************************************
	Create a control based upon the PushButton base class, to be used as
	the close button for the window.
*************************************************************************/
PushButton* TLFrameWindow::createCloseButton(void) const
{
	return NULL;
}


/*************************************************************************
	Setup size and position for the title bar and close button widgets
	attached to this window
*************************************************************************/
void TLFrameWindow::layoutComponentWidgets()
{
	if (d_titlebar != NULL)
	{
		d_titlebar->setMetricsMode(Absolute);
		d_titlebar->setPosition(Point(5, -10));
		d_titlebar->setSize(Size(100, 20));
		d_titlebar->setMetricsMode(Inherited);
	}

	if (d_closeButton != NULL)
	{
	}

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
void TLFrameWindow::onSized(EventArgs& e)
{
	// MUST call base class handler no matter what.  This is now required 100%
	FrameWindow::onSized(e);

	Rect area(getUnclippedPixelRect());
	Size newsz(area.getWidth(), area.getHeight());

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
void TLFrameWindow::onAlphaChanged(EventArgs& e)
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

	d_frameLeftSize		= iset->getImage(TopLeftFrameImageName).getWidth();
	d_frameRightSize	= iset->getImage(TopRightFrameImageName).getWidth();
	d_frameTopSize		= iset->getImage(TopFrameImageName).getHeight();
	d_frameBottomSize	= iset->getImage(BottomFrameImageName).getHeight();

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
