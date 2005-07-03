/************************************************************************
	filename: 	TLButton.cpp
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez look Button widget.
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
#include "TLButton.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLButton::WidgetTypeName[]				= "TaharezLook/Button";

const utf8	TLButton::ImagesetName[]				= "TaharezLook";
const utf8	TLButton::LeftNormalImageName[]			= "ButtonLeftNormal";
const utf8	TLButton::MiddleNormalImageName[]		= "ButtonMiddleNormal";
const utf8	TLButton::RightNormalImageName[]		= "ButtonRightNormal";
const utf8	TLButton::LeftHighlightImageName[]		= "ButtonLeftHighlight";
const utf8	TLButton::MiddleHighlightImageName[]	= "ButtonMiddleHighlight";
const utf8	TLButton::RightHighlightImageName[]		= "ButtonRightHighlight";
const utf8	TLButton::LeftPushedImageName[]			= "ButtonLeftPushed";
const utf8	TLButton::MiddlePushedImageName[]		= "ButtonMiddlePushed";
const utf8	TLButton::RightPushedImageName[]		= "ButtonRightPushed";
const utf8  TLButton::MouseCursorImageName[]		= "MouseArrow";

/*************************************************************************
	Constructor
*************************************************************************/
TLButton::TLButton(const String& type, const String& name) :
	PushButton(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup cache of image pointers
	d_leftSectionNormal		= &iset->getImage(LeftNormalImageName);
	d_middleSectionNormal	= &iset->getImage(MiddleNormalImageName);
	d_rightSectionNormal	= &iset->getImage(RightNormalImageName);

	d_leftSectionHover		= &iset->getImage(LeftHighlightImageName);
	d_middleSectionHover	= &iset->getImage(MiddleHighlightImageName);
	d_rightSectionHover		= &iset->getImage(RightHighlightImageName);

	d_leftSectionPushed		= &iset->getImage(LeftPushedImageName);
	d_middleSectionPushed	= &iset->getImage(MiddlePushedImageName);
	d_rightSectionPushed	= &iset->getImage(RightPushedImageName);

	setMouseCursor(&iset->getImage(MouseCursorImageName));
}


/*************************************************************************
	Destructor
*************************************************************************/
TLButton::~TLButton(void)
{
}


/*************************************************************************
	render Widget in normal state	
*************************************************************************/
void TLButton::drawNormal(float z)
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

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// calculate widths for the button segments
		float leftWidth		= d_leftSectionNormal->getWidth();
		float rightWidth	= d_rightSectionNormal->getWidth();
		float midWidth		= absrect.getWidth() - leftWidth - rightWidth;

		//
		// draw the images
		//
		Vector3 pos(absrect.d_left, absrect.d_top, z);
		Size	sz(leftWidth, absrect.getHeight());
		d_leftSectionNormal->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_right - rightWidth;
		sz.d_width = rightWidth;
		d_rightSectionNormal->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_left + leftWidth;
		sz.d_width = midWidth;
		d_middleSectionNormal->draw(pos, sz, clipper, colours);
	}

	// render clients custom image if that is required.
	if (d_useNormalImage)
	{
		colours = d_normalImage.getColours();
		colours.setAlpha(alpha_comp);
		d_normalImage.setColours(colours);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_normalImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_normalColour);
	colours.modulateAlpha(alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in hover / highlight state	
*************************************************************************/
void TLButton::drawHover(float z)
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

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// calculate widths for the button segments
		float leftWidth		= d_leftSectionHover->getWidth();
		float rightWidth	= d_rightSectionHover->getWidth();
		float midWidth		= absrect.getWidth() - leftWidth - rightWidth;

		//
		// draw the images
		//
		Vector3 pos(absrect.d_left, absrect.d_top, z);
		Size	sz(leftWidth, absrect.getHeight());
		d_leftSectionHover->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_right - rightWidth;
		sz.d_width = rightWidth;
		d_rightSectionHover->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_left + leftWidth;
		sz.d_width = midWidth;
		d_middleSectionHover->draw(pos, sz, clipper, colours);
	}

	// render clients custom image if that is required.
	if (d_useHoverImage)
	{
		colours = d_hoverImage.getColours();
		colours.setAlpha(alpha_comp);
		d_hoverImage.setColours(colours);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_hoverImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_hoverColour);
	colours.modulateAlpha(alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in Pushed state	
*************************************************************************/
void TLButton::drawPushed(float z)
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

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// calculate widths for the button segments
		float leftWidth		= d_leftSectionPushed->getWidth();
		float rightWidth	= d_rightSectionPushed->getWidth();
		float midWidth		= absrect.getWidth() - leftWidth - rightWidth;

		//
		// draw the images
		//
		Vector3 pos(absrect.d_left, absrect.d_top, z);
		Size	sz(leftWidth, absrect.getHeight());
		d_leftSectionPushed->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_right - rightWidth;
		sz.d_width = rightWidth;
		d_rightSectionPushed->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_left + leftWidth;
		sz.d_width = midWidth;
		d_middleSectionPushed->draw(pos, sz, clipper, colours);
	}

	// render clients custom image if that is required.
	if (d_usePushedImage)
	{
		colours = d_pushedImage.getColours();
		colours.setAlpha(alpha_comp);
		d_pushedImage.setColours(colours);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_pushedImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_pushedColour);
	colours.modulateAlpha(alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in disabled state	
*************************************************************************/
void TLButton::drawDisabled(float z)
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
	ColourRect colours(colour(0.5f, 0.5f, 0.5f, alpha_comp));

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// calculate widths for the button segments
		float leftWidth		= d_leftSectionNormal->getWidth();
		float rightWidth	= d_rightSectionNormal->getWidth();
		float midWidth		= absrect.getWidth() - leftWidth - rightWidth;

		//
		// draw the images
		//
		Vector3 pos(absrect.d_left, absrect.d_top, z);
		Size	sz(leftWidth, absrect.getHeight());
		d_leftSectionNormal->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_right - rightWidth;
		sz.d_width = rightWidth;
		d_rightSectionNormal->draw(pos, sz, clipper, colours);

		pos.d_x = absrect.d_left + leftWidth;
		sz.d_width = midWidth;
		d_middleSectionNormal->draw(pos, sz, clipper, colours);
	}

	// render clients custom image if that is required.
	if (d_useDisabledImage)
	{
		colours = d_disabledImage.getColours();
		colours.setAlpha(alpha_comp);
		d_disabledImage.setColours(colours);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_disabledImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_disabledColour);
	colours.modulateAlpha(alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLButton
*************************************************************************/
Window* TLButtonFactory::createWindow(const String& name)
{
	return new TLButton(d_type, name);
}

} // End of  CEGUI namespace section
