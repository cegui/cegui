/************************************************************************
	filename: 	WLTabPane.cpp
	created:	03/12/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Windows look Tab Pane widget.
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
#include "WLTabPane.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	WLTabPane::WidgetTypeName[]	= "WindowsLook/TabPane";

const utf8	WLTabPane::ImagesetName[]			= "WindowsLook";
const utf8	WLTabPane::LeftImageName[]		    = "TabPaneLeft";
const utf8	WLTabPane::MiddleImageName[]	    = "TabPaneMiddle";
const utf8	WLTabPane::RightImageName[]		    = "TabPaneRight";
const utf8	WLTabPane::LowerRightImageName[]    = "TabPaneLowerRight";
const utf8	WLTabPane::LowerLeftImageName[]     = "TabPaneLowerLeft";
const utf8	WLTabPane::LowerImageName[]         = "TabPaneLower";


/*************************************************************************
	Constructor
*************************************************************************/
WLTabPane::WLTabPane(const String& type, const String& name) :
	TabPane(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup cache of image pointers
	d_leftSection		    = &iset->getImage(LeftImageName);
	d_middleSection	        = &iset->getImage(MiddleImageName);
	d_rightSection	        = &iset->getImage(RightImageName);
    d_lowerLeftSection      = &iset->getImage(LowerLeftImageName);
    d_lowerRightSection     = &iset->getImage(LowerRightImageName);
    d_lowerSection          = &iset->getImage(LowerImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
WLTabPane::~WLTabPane(void)
{
}


/*************************************************************************
	render Widget in normal state	
*************************************************************************/
void WLTabPane::drawSelf(float z)
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
	ColourRect colours(colour(0.97645f, 0.97255f, 0.9451f, getEffectiveAlpha()));

	// calculate widths for the button segments
	float leftWidth		= d_leftSection->getWidth();
	float rightWidth	= d_rightSection->getWidth();
    float lowerHeight	= d_lowerSection->getWidth();
	float midWidth		= absrect.getWidth() - leftWidth - rightWidth;
    float midHeight		= absrect.getHeight() - lowerHeight;

	//
	// draw the images
	//
	Vector3 pos;
    Size	sz;
    pos.d_z = z;
    pos.d_x = absrect.d_left;
    pos.d_y = absrect.d_top;
    sz.d_width = leftWidth;
    sz.d_height = midHeight;
	d_leftSection->draw(pos, sz, clipper, colours);

    pos.d_y += sz.d_height;
    sz.d_height = lowerHeight;
    d_lowerLeftSection->draw(pos, sz, clipper, colours);

    pos.d_x = absrect.d_left + leftWidth;
    pos.d_y = absrect.d_top;
    sz.d_width = midWidth;
    sz.d_height = midHeight;
    d_middleSection->draw(pos, sz, clipper, colours);

    pos.d_y += sz.d_height;
    sz.d_height = lowerHeight;
    d_lowerSection->draw(pos, sz, clipper, colours);

    pos.d_x = absrect.d_right - rightWidth;
    pos.d_y = absrect.d_top;
    sz.d_width = rightWidth;
    sz.d_height = midHeight;
    d_rightSection->draw(pos, sz, clipper, colours);

    pos.d_y += sz.d_height;
    sz.d_height = lowerHeight;
    d_lowerRightSection->draw(pos, sz, clipper, colours);

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLTabPane
*************************************************************************/
Window* WLTabPaneFactory::createWindow(const String& name)
{
	WLTabPane* wnd = new WLTabPane(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
