/************************************************************************
	filename: 	WLTitlebar.cpp
	created:	8/8/2004
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
#include "WLTitlebar.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"
#include "elements/CEGUIFrameWindow.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLTitlebar::WidgetTypeName[]			= "WindowsLook/Titlebar";

const utf8	WLTitlebar::ImagesetName[]				= "WindowsLook";
const utf8	WLTitlebar::TopLeftFrameImageName[]		= "TitlebarTopLeft";
const utf8	WLTitlebar::TopRightFrameImageName[]	= "TitlebarTopRight";
const utf8	WLTitlebar::BottomLeftFrameImageName[]	= "TitlebarBottomLeft";
const utf8	WLTitlebar::BottomRightFrameImageName[]	= "TitlebarBottomRight";
const utf8	WLTitlebar::LeftFrameImageName[]		= "TitlebarLeft";
const utf8	WLTitlebar::RightFrameImageName[]		= "TitlebarRight";
const utf8	WLTitlebar::TopFrameImageName[]			= "TitlebarTop";
const utf8	WLTitlebar::BottomFrameImageName[]		= "TitlebarBottom";
const utf8	WLTitlebar::FillImageName[]				= "TitlebarFill";
const utf8	WLTitlebar::NormalCursorImageName[]		= "MouseMoveCursor";
const utf8	WLTitlebar::NoDragCursorImageName[]		= "MouseArrow";

// colours
const colour WLTitlebar::ActiveColour		= 0xFFA7C7FF;
const colour WLTitlebar::InactiveColour		= 0xFFEFEFEF;
const colour WLTitlebar::CaptionColour		= 0xFF000000;


/*************************************************************************
	Constructor
*************************************************************************/
WLTitlebar::WLTitlebar(const String& type, const String& name) :
	Titlebar(type, name)
{
	// get images
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	
	// store framesizes
	storeFrameSizes();

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
		
	d_fill.setImage(&iset->getImage(FillImageName));
	d_fill.setHorzFormatting(RenderableImage::HorzStretched);
	d_fill.setVertFormatting(RenderableImage::VertStretched);

	// default text colour
	d_captionColour = CaptionColour;

	// set cursor
	setMouseCursor(&iset->getImage(NormalCursorImageName));

	setAlwaysOnTop(false);
}


/*************************************************************************
	Destructor
*************************************************************************/
WLTitlebar::~WLTitlebar(void)
{
}


/*************************************************************************
	return a Rect object describing the appropriately clipped Window
	area in screen space.
*************************************************************************/
Rect WLTitlebar::getPixelRect(void) const
{
    // clip to grand-parent as needed
    if (d_parent && d_parent->getParent() && isClippedByParent())
    {
        return d_parent->getParent()->getInnerRect().getIntersection(getUnclippedPixelRect());
    }
    // clip to screen if no grand-parent, or if clipping has been disabled for us.
    else 
    {
        return System::getSingleton().getRenderer()->getRect().getIntersection(getUnclippedPixelRect());
    }
}


/*************************************************************************
	Perform the actual rendering for this widget.	
*************************************************************************/
void WLTitlebar::drawSelf(float z)
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
	float alpha_comp = getEffectiveAlpha();
	ColourRect colours(((d_parent != NULL) && d_parent->isActive()) ? ActiveColour : InactiveColour);
	colours.setAlpha(alpha_comp);

	//
	// draw the title bar frame
	//
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	d_frame.setSize(absrect.getSize());
	d_frame.setColours(colours);
	d_frame.draw(pos, clipper);

	// calculate size for middle title bar segment
	float midWidth		= absrect.getWidth() - d_frameLeftSize - d_frameRightSize;
	float midHeight		= absrect.getHeight() - d_frameTopSize - d_frameBottomSize;

	//
	// draw title bar frame fill
	//
	d_fill.setSize(Size(midWidth, midHeight));
	d_fill.setColours(colours);
	pos.d_x += d_frameLeftSize;
	pos.d_y += d_frameTopSize;
	d_fill.draw(pos, clipper);

	//
	// Draw the title text
	//
	colours.setColours(d_captionColour);
	colours.setAlpha(alpha_comp);

	Rect textClipper(clipper);
	textClipper.setWidth(midWidth);
	textClipper = clipper.getIntersection(textClipper);

	pos.d_x = absrect.d_left + d_frameLeftSize;
	pos.d_y = absrect.d_top + PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f);
	pos.d_z = System::getSingleton().getRenderer()->getZLayer(1);

	getFont()->drawText(d_parent->getText(), pos, textClipper, colours);
}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void WLTitlebar::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	const Image* img;
	img = &iset->getImage(LeftFrameImageName);
	d_frameLeftSize = img->getWidth();

	img = &iset->getImage(RightFrameImageName);
	d_frameRightSize = img->getWidth();

	img = &iset->getImage(TopFrameImageName);
	d_frameTopSize = img->getHeight();

	img = &iset->getImage(BottomFrameImageName);
	d_frameBottomSize = img->getHeight();
}


/*************************************************************************
	Handler for when drag mode changes
*************************************************************************/
void WLTitlebar::onDraggingModeChanged(WindowEventArgs& e)
{
	// base class processing
	Titlebar::onDraggingModeChanged(e);

	// change cursor depending on new mode.
	if (d_dragEnabled)
	{
		setMouseCursor(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(NormalCursorImageName));
	}
	else
	{
		setMouseCursor(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(NoDragCursorImageName));
	}

	requestRedraw();

	e.handled = true;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLTitlebar
*************************************************************************/
Window* WLTitlebarFactory::createWindow(const String& name)
{
	return new WLTitlebar(d_type, name);
}


} // End of  CEGUI namespace section
