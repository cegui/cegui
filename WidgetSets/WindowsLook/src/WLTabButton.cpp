/************************************************************************
	filename: 	WLTabButton.cpp
	created:	03/12/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Windows look Tab Button widget.
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
#include "WLTabButton.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	WLTabButton::WidgetTypeName[]	= "WindowsLook/TabButton";

const utf8	WLTabButton::ImagesetName[]				    = "WindowsLook";
const utf8	WLTabButton::LeftNormalImageName[]		    = "TabButtonLeftNormal";
const utf8	WLTabButton::MiddleNormalImageName[]	    = "TabButtonMiddleNormal";
const utf8	WLTabButton::RightNormalImageName[]		    = "TabButtonRightNormal";
const utf8	WLTabButton::UpperRightNormalImageName[]    = "TabButtonUpperRightNormal";
const utf8	WLTabButton::UpperLeftNormalImageName[]     = "TabButtonUpperLeftNormal";
const utf8	WLTabButton::UpperNormalImageName[]         = "TabButtonUpperNormal";
const utf8	WLTabButton::LowerNormalImageName[]         = "TabButtonLowerNormal";
const utf8	WLTabButton::LowerRightNormalImageName[]    = "TabButtonLowerRightNormal";
const utf8	WLTabButton::LowerLeftNormalImageName[]     = "TabButtonLowerLeftNormal";
const utf8	WLTabButton::LeftSelectedImageName[]	    = "TabButtonLeftSelected";
const utf8	WLTabButton::MiddleSelectedImageName[]	    = "TabButtonMiddleSelected";
const utf8	WLTabButton::RightSelectedImageName[]	    = "TabButtonRightSelected";
const utf8	WLTabButton::UpperRightSelectedImageName[]  = "TabButtonUpperRightSelected";
const utf8	WLTabButton::UpperLeftSelectedImageName[]   = "TabButtonUpperLeftSelected";
const utf8	WLTabButton::UpperSelectedImageName[]       = "TabButtonUpperSelected";


/*************************************************************************
	Constructor
*************************************************************************/
WLTabButton::WLTabButton(const String& type, const String& name) :
	TabButton(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup cache of image pointers
	d_leftSectionNormal		    = &iset->getImage(LeftNormalImageName);
	d_middleSectionNormal	    = &iset->getImage(MiddleNormalImageName);
	d_rightSectionNormal	    = &iset->getImage(RightNormalImageName);
    d_upperLeftSectionNormal    = &iset->getImage(UpperLeftNormalImageName);
    d_upperRightSectionNormal   = &iset->getImage(UpperRightNormalImageName);
    d_upperSectionNormal        = &iset->getImage(UpperNormalImageName);
    d_lowerSectionNormal        = &iset->getImage(LowerNormalImageName);
    d_lowerLeftSectionNormal    = &iset->getImage(LowerLeftNormalImageName);
    d_lowerRightSectionNormal   = &iset->getImage(LowerRightNormalImageName);

    d_leftSectionSelected       = &iset->getImage(LeftSelectedImageName);
    d_middleSectionSelected	    = &iset->getImage(MiddleSelectedImageName);
    d_rightSectionSelected	    = &iset->getImage(RightSelectedImageName);
    d_upperLeftSectionSelected  = &iset->getImage(UpperLeftSelectedImageName);
    d_upperRightSectionSelected = &iset->getImage(UpperRightSelectedImageName);
    d_upperSectionSelected      = &iset->getImage(UpperSelectedImageName);

	d_normalColour	= 0xFF000000;
	d_hoverColour	= 0xFF000000;
	d_pushedColour	= 0xFF000000;
}


/*************************************************************************
	Destructor
*************************************************************************/
WLTabButton::~WLTabButton(void)
{
}


/*************************************************************************
	render Widget in normal state	
*************************************************************************/
void WLTabButton::drawNormal(float z)
{
    const uint INDENT = 3;
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
	ColourRect colours(colour(1, 1, 1, alpha_comp));
	ColourRect fillColours(
		colour(1, 1, 1),
		colour(1, 1, 1),
		colour(0.75f, 0.75f, 0.75f),
		colour(0.75f, 0.75f, 0.75f)
		);
	fillColours.setAlpha(alpha_comp);

	// calculate widths for the button segments
	float leftWidth		= d_upperSectionNormal->getWidth();
	float rightWidth	= d_upperSectionNormal->getWidth();
    float topHeight	    = d_upperSectionNormal->getHeight();
    float lowerHeight	= d_lowerSectionNormal->getHeight();
	float midWidth		= absrect.getWidth() - leftWidth - rightWidth;
    float midHeight		= absrect.getHeight() - topHeight - lowerHeight;

	//
	// draw the images
	//
	Vector3 pos;
    Size	sz;
    pos.d_z = z;
    pos.d_x = absrect.d_left;
    pos.d_y = absrect.d_top + INDENT;
	d_upperLeftSectionNormal->draw(pos, clipper, colours);

    pos.d_y += d_upperLeftSectionNormal->getHeight();
	sz.d_width = d_leftSectionNormal->getWidth();
    sz.d_height = midHeight - INDENT;
    d_leftSectionNormal->draw(pos, sz, clipper, colours);

    pos.d_y += sz.d_height;
    d_lowerLeftSectionNormal->draw(pos, clipper, colours);

    pos.d_x = absrect.d_left + d_upperLeftSectionNormal->getWidth();
    pos.d_y = absrect.d_top + INDENT;
    sz.d_width = midWidth;
    sz.d_height = d_upperSectionNormal->getHeight();
    d_upperSectionNormal->draw(pos, sz, clipper, colours);

    pos.d_y += sz.d_height;
    sz.d_height = midHeight - INDENT;
    d_middleSectionNormal->draw(pos, sz, clipper, fillColours);

    pos.d_x = absrect.d_right - d_upperRightSectionNormal->getWidth();
    pos.d_y = absrect.d_top + INDENT;
    d_upperRightSectionNormal->draw(pos, clipper, colours);

	pos.d_x = absrect.d_right - d_rightSectionNormal->getWidth();
    pos.d_y += d_upperRightSectionNormal->getHeight();
	sz.d_width = d_rightSectionNormal->getWidth();
    sz.d_height = midHeight - INDENT;
    d_rightSectionNormal->draw(pos, sz, clipper, colours);

	pos.d_x = absrect.d_right - d_lowerRightSectionNormal->getWidth();
    pos.d_y += sz.d_height;
    d_lowerRightSectionNormal->draw(pos, clipper, colours);

	sz.d_width = midWidth;
	sz.d_height = d_lowerSectionNormal->getHeight();
    pos.d_x = absrect.d_left + d_lowerLeftSectionNormal->getWidth();
    pos.d_y = absrect.d_bottom - sz.d_height;
    d_lowerSectionNormal->draw(pos, sz, clipper, colours);
        

	//
	// Draw label text
	//
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getFontHeight()) * 0.5f);
	colours.setColours(d_normalColour);
	colours.setAlpha(alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in selected state	
*************************************************************************/
void WLTabButton::drawPushed(float z)
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
	ColourRect colours(colour(1, 1, 1, alpha_comp));
	ColourRect fillColours(colour(0.97645f, 0.97255f, 0.9451f, alpha_comp));

	// calculate widths for the button segments
	float midWidth		= absrect.getWidth() - d_upperSectionSelected->getWidth() - d_upperSectionSelected->getWidth();
	float midHeight		= absrect.getHeight() - d_upperSectionSelected->getHeight();

	//
	// draw the images
	//
	Vector3 pos;
	Size	sz;
	pos.d_z = z;
	pos.d_x = absrect.d_left;
	pos.d_y = absrect.d_top;
	d_upperLeftSectionSelected->draw(pos, clipper, colours);

	pos.d_y += d_upperLeftSectionSelected->getHeight();
	sz.d_width = d_leftSectionSelected->getWidth();
	sz.d_height = midHeight;
	d_leftSectionSelected->draw(pos, sz, clipper, colours);

	pos.d_x = absrect.d_left + d_upperLeftSectionSelected->getWidth();
	pos.d_y = absrect.d_top;
	sz.d_width = midWidth;
	sz.d_height = d_upperSectionSelected->getHeight();
	d_upperSectionSelected->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = midHeight;
	d_middleSectionSelected->draw(pos, sz, clipper, fillColours);

	pos.d_x = absrect.d_right - d_upperRightSectionSelected->getWidth();
	pos.d_y = absrect.d_top;
	d_upperRightSectionSelected->draw(pos, clipper, colours);

	pos.d_x = absrect.d_right - d_rightSectionSelected->getWidth();
	pos.d_y += d_upperRightSectionSelected->getHeight();
	sz.d_width = d_rightSectionSelected->getWidth();
	sz.d_height = midHeight;
	d_rightSectionSelected->draw(pos, sz, clipper, colours);

	//
	// Draw label text
	//
	absrect.d_top += (absrect.getHeight() - getFont()->getFontHeight()) * 0.5f;
	colours.setColours(d_pushedColour);
	colours.setAlpha(alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in hover state	
*************************************************************************/
void WLTabButton::drawHover(float z)
{
	// if we're selected, do not render the hover imagery
	if (d_selected)
	{
		drawPushed(z);
		return;
	}

	const uint INDENT = 3;
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
	ColourRect colours(colour(1, 1, 1, alpha_comp));
	ColourRect fillColours(
		colour(1, 1, 1),
		colour(1, 1, 1),
		colour(0.85f, 0.85f, 0.85f),
		colour(0.85f, 0.85f, 0.85f)
		);
	fillColours.setAlpha(alpha_comp);

	// calculate widths for the button segments
	float midWidth		= absrect.getWidth() - d_upperSectionSelected->getWidth() - d_upperSectionSelected->getWidth();
	float midHeight		= absrect.getHeight() - d_upperSectionSelected->getHeight();

	//
	// draw the images
	//
	Vector3 pos;
	Size	sz;
	pos.d_z = z;
	pos.d_x = absrect.d_left;
	pos.d_y = absrect.d_top + INDENT;
	d_upperLeftSectionSelected->draw(pos, clipper, colours);

	pos.d_y += d_upperLeftSectionSelected->getHeight();
	sz.d_width = d_leftSectionSelected->getWidth();
	sz.d_height = midHeight - INDENT;
	d_leftSectionSelected->draw(pos, sz, clipper, colours);

	pos.d_x = absrect.d_left + d_upperLeftSectionSelected->getWidth();
	pos.d_y = absrect.d_top + INDENT;
	sz.d_width = midWidth;
	sz.d_height = d_upperSectionSelected->getHeight();
	d_upperSectionSelected->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = midHeight - INDENT;
	d_middleSectionSelected->draw(pos, sz, clipper, fillColours);

	pos.d_x = absrect.d_right - d_upperRightSectionSelected->getWidth();
	pos.d_y = absrect.d_top + INDENT;
	d_upperRightSectionSelected->draw(pos, clipper, colours);

	pos.d_x = absrect.d_right - d_rightSectionSelected->getWidth();
	pos.d_y += d_upperRightSectionSelected->getHeight();
	sz.d_width = d_rightSectionSelected->getWidth();
	sz.d_height = midHeight - INDENT;
	d_rightSectionSelected->draw(pos, sz, clipper, colours);

	//
	// Draw label text
	//
	absrect.d_top += (absrect.getHeight() - getFont()->getFontHeight()) * 0.5f;
	colours.setColours(d_pushedColour);
	colours.setAlpha(alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLTabButton
*************************************************************************/
Window* WLTabButtonFactory::createWindow(const String& name)
{
	WLTabButton* wnd = new WLTabButton(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
