/************************************************************************
	filename: 	TLTitlebar.cpp
	created:	9/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Look Titlebar widget
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
#include "TLTitlebar.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	TLTitlebar::ImagesetName[]				= "TaharezImagery";
const utf8	TLTitlebar::LeftEndSectionImageName[]	= "TitlebarLeft";
const utf8	TLTitlebar::MiddleSectionImageName[]	= "TitlebarMiddle";
const utf8	TLTitlebar::RightEndSectionImageName[]	= "TitlebarRight";
const utf8	TLTitlebar::NormalCursorImageName[]		= "MouseMoveCursor";


/*************************************************************************
	Constructor
*************************************************************************/
TLTitlebar::TLTitlebar(const String& type, const String& name) :
	Titlebar(type, name)
{
	// get images
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_leftImage		= &iset->getImage(LeftEndSectionImageName);
	d_middleImage	= &iset->getImage(MiddleSectionImageName);
	d_rightImage	= &iset->getImage(RightEndSectionImageName);

	// set cursor
	setMouseCursor(&iset->getImage(NormalCursorImageName));
}


/*************************************************************************
	Destructor
*************************************************************************/
TLTitlebar::~TLTitlebar(void)
{
}


/*************************************************************************
	return a Rect object describing the appropriately clipped Window
	area in screen space.
*************************************************************************/
Rect TLTitlebar::getPixelRect(void) const
{
	// clip to screen if we have no grand-parent
	if ((d_parent == NULL) || (d_parent->getParent() == NULL))
	{
		return System::getSingleton().getRenderer()->getRect().getIntersection(getUnclippedPixelRect());
	}
	// else clip to grand-parent
	else 
	{
		return d_parent->getParent()->getInnerRect().getIntersection(getUnclippedPixelRect());
	}

}


/*************************************************************************
	Perform the actual rendering for this widget.	
*************************************************************************/
void TLTitlebar::drawSelf(float z)
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
	colour colval = ((colour)(getEffectiveAlpha() * 255.0f) << 24) | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	// calculate widths for the title bar segments
	float leftWidth		= d_leftImage->getWidth();
	float rightWidth	= d_rightImage->getWidth();
	float midWidth		= absrect.getWidth() - leftWidth - rightWidth;

	//
	// draw the title bar images
	//
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	Size	sz(leftWidth, absrect.getHeight());
	d_leftImage->draw(pos, sz, clipper, colours);

	pos.d_x = absrect.d_right - rightWidth;
	sz.d_width = rightWidth;
	d_rightImage->draw(pos, sz, clipper, colours);

	pos.d_x = absrect.d_left + leftWidth;
	sz.d_width = midWidth;
	d_middleImage->draw(pos, sz, clipper, colours);

	//
	// Draw the title text
	//
	pos.d_x = absrect.d_left + 10.0f;
	pos.d_y = absrect.d_top + ((absrect.getHeight() - getFont()->getLineSpacing()) / 2);
	pos.d_z = System::getSingleton().getRenderer()->getZLayer(1);
	getFont()->drawText(d_parent->getText(), pos, clipper, colours);
}


/*************************************************************************
	Handler for mouse move events.
*************************************************************************/
void TLTitlebar::onMouseMove(MouseEventArgs& e)
{
	Titlebar::onMouseMove(e);

	// simply mark as handled to stop parent getting the message
	e.handled = true;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLTitlebar
*************************************************************************/
Window* TLTitlebarFactory::createWindow(const String& name)
{
	TLTitlebar* wnd = new TLTitlebar(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
