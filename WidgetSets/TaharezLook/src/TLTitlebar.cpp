/************************************************************************
	filename: 	TLTitlebar.cpp
	created:	9/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Look Titlebar widget
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
#include "TLTitlebar.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"
#include "elements/CEGUIFrameWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLTitlebar::WidgetTypeName[]	= "TaharezLook/Titlebar";

const utf8	TLTitlebar::ImagesetName[]				= "TaharezLook";
const utf8	TLTitlebar::LeftEndSectionImageName[]	= "NewTitlebarLeft";
const utf8	TLTitlebar::MiddleSectionImageName[]	= "NewTitlebarMiddle";
const utf8	TLTitlebar::RightEndSectionImageName[]	= "NewTitlebarRight";
const utf8	TLTitlebar::SysAreaMiddleImageName[]	= "SysAreaMiddle";
const utf8	TLTitlebar::SysAreaRightImageName[]		= "SysAreaRight";
const utf8	TLTitlebar::NormalCursorImageName[]		= "MouseMoveCursor";
const utf8	TLTitlebar::NoDragCursorImageName[]		= "MouseTarget";

const colour TLTitlebar::CaptionColour		= 0xFFFFFF;


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
	d_sysMidImage	= &iset->getImage(SysAreaMiddleImageName);
	d_sysRightImage = &iset->getImage(SysAreaRightImageName);

	// default text colour
	d_captionColour = CaptionColour;

	// set cursor
	setMouseCursor(&iset->getImage(NormalCursorImageName));

	setAlwaysOnTop(false);
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
	float alpha = getEffectiveAlpha();
	ColourRect colours(colour(1, 1, 1, alpha));

	// calculate widths for the title bar segments
	float leftWidth		= d_leftImage->getWidth();
	float rightWidth	= d_rightImage->getWidth();

	float sysRightWidth	= d_sysRightImage->getWidth();
	float sysMidWidth;

	FrameWindow* parent = (FrameWindow*)getParent();

	sysMidWidth = ((parent != NULL) && parent->isCloseButtonEnabled()) ? d_sysMidImage->getWidth() : 0.0f;

	float midWidth		= absrect.getWidth() - leftWidth - rightWidth - sysRightWidth - sysMidWidth;

	//
	// draw the title bar images
	//
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	Size	sz(leftWidth, absrect.getHeight());
	d_leftImage->draw(pos, sz, clipper, colours);
	pos.d_x += sz.d_width;

	sz.d_width = midWidth;
	d_middleImage->draw(pos, sz, clipper, colours);
	pos.d_x += sz.d_width;

	sz.d_width = rightWidth;
	d_rightImage->draw(pos, sz, clipper, colours);
	pos.d_x += sz.d_width;

	sz.d_width = sysMidWidth;
	d_sysMidImage->draw(pos, sz, clipper, colours);
	pos.d_x += sz.d_width;

	sz.d_width = sysRightWidth;
	d_sysRightImage->draw(pos, sz, clipper, colours);


	//
	// Draw the title text
	//
	// calculate colours to use for caption text
	colours.setColours(d_captionColour);
	colours.setAlpha(alpha);

	Rect textClipper(clipper);
	textClipper.setWidth(midWidth);
	textClipper = clipper.getIntersection(textClipper);
	pos.d_x = absrect.d_left + leftWidth;
	pos.d_y = absrect.d_top + PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f);
	pos.d_z = System::getSingleton().getRenderer()->getZLayer(1);
	getFont()->drawText(d_parent->getText(), pos, textClipper, colours);
}


/*************************************************************************
	Handler for when drag mode changes
*************************************************************************/
void TLTitlebar::onDraggingModeChanged(WindowEventArgs& e)
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
	Create, initialise and return a TLTitlebar
*************************************************************************/
Window* TLTitlebarFactory::createWindow(const String& name)
{
	TLTitlebar* wnd = new TLTitlebar(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
