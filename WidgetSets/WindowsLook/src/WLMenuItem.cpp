/************************************************************************
    filename:   WLMenuItem.cpp
    created:    2/4/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)
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
#include "WLMenuItem.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
//#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{

const utf8 WLMenubarItem::WidgetTypeName[] = "WindowsLook/MenubarItem";

const utf8	WLMenubarItem::ImagesetName[]				= "WindowsLook";
const utf8	WLMenubarItem::TopLeftFrameImageName[]		= "MenuTopLeft";
const utf8	WLMenubarItem::TopRightFrameImageName[]		= "MenuTopRight";
const utf8	WLMenubarItem::BottomLeftFrameImageName[]	= "MenuBottomLeft";
const utf8	WLMenubarItem::BottomRightFrameImageName[]	= "MenuBottomRight";
const utf8	WLMenubarItem::LeftFrameImageName[]			= "MenuLeft";
const utf8	WLMenubarItem::RightFrameImageName[]		= "MenuRight";
const utf8	WLMenubarItem::TopFrameImageName[]			= "MenuTop";
const utf8	WLMenubarItem::BottomFrameImageName[]		= "MenuBottom";
const utf8	WLMenubarItem::BackgroundImageName[]		= "Background";


/*************************************************************************
	Constructor for WLMenubarItem widgets.	
*************************************************************************/
WLMenubarItem::WLMenubarItem(const String& type, const String& name) :
	MenuItem(type, name)
{
	setNormalTextColour(0xFF000000);
	setHoverColour(0xFFA7C7FF);
	setPushedColour(0xFF87A7DF);

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

	d_fill.setImage(&iset->getImage(BackgroundImageName));
	d_fill.setHorzFormatting(RenderableImage::HorzStretched);
	d_fill.setVertFormatting(RenderableImage::VertStretched);
	
	// centre text in item
	d_textFormatting = Centred;
}


/*************************************************************************
	Destructor for WLMenubar widgets.
*************************************************************************/
WLMenubarItem::~WLMenubarItem(void)
{
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void WLMenubarItem::drawSelf(float z)
{
	drawBackground(z);

	if (isDisabled())
		d_textColour = d_disabledTextColour;
	else
		d_textColour = d_normalTextColour;

	TextItem::drawSelf(z);
}


/*************************************************************************
	Draw the background.
*************************************************************************/
void WLMenubarItem::drawBackground(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0 || isDisabled())
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	colour col;

	if (d_opened)
		col = d_openedColour;
	else if (d_pushed)
		col = d_pushedColour;
	else if (d_hovering)
		col = d_hoverColour;
	else
		return;

	ColourRect colours(col);
	colours.modulateAlpha(getEffectiveAlpha());

	//
	// draw the frame
	//
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	d_frame.setSize(absrect.getSize());
	d_frame.setColours(colours);
	d_frame.draw(pos, clipper);

	// calculate size for middle title bar segment
	float midWidth		= absrect.getWidth() - d_frameLeftSize - d_frameRightSize;
	float midHeight		= absrect.getHeight() - d_frameTopSize - d_frameBottomSize;

	//
	// draw the centre fill
	//
	d_fill.setSize(Size(midWidth, midHeight));
	d_fill.setColours(colours);
	pos.d_x += d_frameLeftSize;
	pos.d_y += d_frameTopSize;
	d_fill.draw(pos, clipper);
}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void WLMenubarItem::storeFrameSizes(void)
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


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Factory Methods
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

Window* WLMenubarItemFactory::createWindow(const String& name)
{
    return new WLMenubarItem(d_type, name);
}


void WLMenubarItemFactory::destroyWindow(Window* window)
{
    if (window->getType() == d_type)
    {
		delete window;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


const utf8 WLPopupMenuItem::WidgetTypeName[] = "WindowsLook/PopupMenuItem";

const utf8	WLPopupMenuItem::ImagesetName[]					= "WindowsLook";
const utf8	WLPopupMenuItem::TopLeftFrameImageName[]		= "MenuTopLeft";
const utf8	WLPopupMenuItem::TopRightFrameImageName[]		= "MenuTopRight";
const utf8	WLPopupMenuItem::BottomLeftFrameImageName[]		= "MenuBottomLeft";
const utf8	WLPopupMenuItem::BottomRightFrameImageName[]	= "MenuBottomRight";
const utf8	WLPopupMenuItem::LeftFrameImageName[]			= "MenuLeft";
const utf8	WLPopupMenuItem::RightFrameImageName[]			= "MenuRight";
const utf8	WLPopupMenuItem::TopFrameImageName[]			= "MenuTop";
const utf8	WLPopupMenuItem::BottomFrameImageName[]			= "MenuBottom";
const utf8	WLPopupMenuItem::BackgroundImageName[]			= "Background";

const utf8	WLPopupMenuItem::ArrowLeftImageName[]			= "PopupMenuArrowLeft";
const utf8	WLPopupMenuItem::ArrowRightImageName[]			= "PopupMenuArrowRight";


/*************************************************************************
	Constructor for WLPopupMenuItem widgets.	
*************************************************************************/
WLPopupMenuItem::WLPopupMenuItem(const String& type, const String& name) :
	MenuItem(type, name)
{
	setNormalTextColour(0xFF000000);
	setHoverColour(0xFFA7C7FF);
	setPushedColour(0xFF87A7DF);
	
	d_textXOffset = 20;

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

	d_fill.setImage(&iset->getImage(BackgroundImageName));
	d_fill.setHorzFormatting(RenderableImage::HorzStretched);
	d_fill.setVertFormatting(RenderableImage::VertStretched);
	
	d_arrowLeft.setImage(&iset->getImage(ArrowLeftImageName));
	d_arrowLeft.setHorzFormatting(RenderableImage::HorzCentred);
	d_arrowLeft.setVertFormatting(RenderableImage::VertCentred);
	
	d_arrowRight.setImage(&iset->getImage(ArrowRightImageName));
	d_arrowRight.setHorzFormatting(RenderableImage::HorzCentred);
	d_arrowRight.setVertFormatting(RenderableImage::VertCentred);
}


/*************************************************************************
	Destructor for WLPopupMenuItem widgets.
*************************************************************************/
WLPopupMenuItem::~WLPopupMenuItem(void)
{
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void WLPopupMenuItem::drawSelf(float z)
{
	drawBackground(z);

	// text
	if (isDisabled())
		d_textColour = d_disabledTextColour;
	else
		d_textColour = d_normalTextColour;

	TextItem::drawSelf(z);
}


/*************************************************************************
	Draw the background.
*************************************************************************/
void WLPopupMenuItem::drawBackground(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0 || isDisabled())
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	colour col;
	bool draw_bg = true;

	if (d_opened)
		col = d_openedColour;
	else if (d_pushed)
		col = d_pushedColour;
	else if (d_hovering)
		col = d_hoverColour;
	else
		draw_bg = false;

	col.setAlpha(getEffectiveAlpha());
	ColourRect colours(col);

	if (draw_bg)
	{
		//
		// draw the frame
		//
		Vector3 pos(absrect.d_left, absrect.d_top, z);
		d_frame.setSize(absrect.getSize());
		d_frame.setColours(colours);
		d_frame.draw(pos, clipper);

		// calculate size for middle title bar segment
		float midWidth		= absrect.getWidth() - d_frameLeftSize - d_frameRightSize;
		float midHeight		= absrect.getHeight() - d_frameTopSize - d_frameBottomSize;

		//
		// draw the centre fill
		//
		d_fill.setSize(Size(midWidth, midHeight));
		d_fill.setColours(colours);
		pos.d_x += d_frameLeftSize;
		pos.d_y += d_frameTopSize;
		d_fill.draw(pos, clipper);
	}

	// draw a popup indicator arrow for items with a popup menu
	if (d_popup!=NULL)
	{
		RenderableImage& arrow = d_opened?d_arrowLeft:d_arrowRight;

		arrow.setSize(Size(absrect.getHeight(),absrect.getHeight()));
		arrow.setColours(ColourRect(colour(0,0,0,col.getAlpha())));
		arrow.draw(Vector3(absrect.d_right-absrect.getHeight(),absrect.d_top,z),clipper);
	}

}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void WLPopupMenuItem::storeFrameSizes(void)
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
	Get the optimal pixelsize for this PopupMenuItem
*************************************************************************/
Size WLPopupMenuItem::getItemPixelSize()
{
	Size sz = MenuItem::getItemPixelSize();
	sz.d_width += d_arrowRight.getImage()->getWidth();
	return sz;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Factory Methods
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

Window* WLPopupMenuItemFactory::createWindow(const String& name)
{
    return new WLPopupMenuItem(d_type, name);
}


void WLPopupMenuItemFactory::destroyWindow(Window* window)
{
    if (window->getType() == d_type)
    {
		delete window;
	}
}

} // End of  CEGUI namespace section
