/************************************************************************
	filename: 	TLButton.cpp
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez look Button widget.
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
#include "TLButton.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	TLButton::ImagesetName[]				= "TaharezImagery";
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

	// default options
	d_autoscaleImages		= true;
	d_useStandardImagery	= true;
	d_useNormalImage		= false;
	d_useHoverImage			= false;
	d_usePushedImage		= false;
	d_useDisabledImage		= false;

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
	set whether or not to render the standard imagery for the button	
*************************************************************************/
void TLButton::setStandardImageryEnabled(bool setting)
{
	if (d_useStandardImagery != setting)
	{
		d_useStandardImagery = setting;
		requestRedraw();
	}

}


/*************************************************************************
	set the image to render for the button (normal state) - use NULL to
	disable drawing of image	
*************************************************************************/
void TLButton::setNormalImage(const RenderableImage* image)
{
	if (image == NULL)
	{
		d_useNormalImage = false;
	}
	else
	{
		d_useNormalImage = true;
		d_normalImage = *image;
	}

}


/*************************************************************************
	set the image to render for the button (hover state)  - use NULL to
	disable drawing of image
*************************************************************************/
void TLButton::setHoverImage(const RenderableImage* image)
{
	if (image == NULL)
	{
		d_useHoverImage = false;
	}
	else
	{
		d_useHoverImage = true;
		d_hoverImage = *image;
	}

}


/*************************************************************************
	set the image to render for the button (pushed state)  - use NULL to
	disable drawing of image	
*************************************************************************/
void TLButton::setPushedImage(const RenderableImage* image)
{
	if (image == NULL)
	{
		d_usePushedImage = false;
	}
	else
	{
		d_usePushedImage = true;
		d_pushedImage = *image;
	}

}


/*************************************************************************
	set the image to render for the button (disabled state)  - use NULL
	to disable drawing of image	
*************************************************************************/
void TLButton::setDisabledImage(const RenderableImage* image)
{
	if (image == NULL)
	{
		d_useDisabledImage = false;
	}
	else
	{
		d_useDisabledImage = true;
		d_disabledImage = *image;
	}

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
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

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
		ColourRect imgcols = d_normalImage.getColours();
		imgcols.setAlpha(getEffectiveAlpha());
		d_normalImage.setColours(imgcols);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_normalImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) / 2;
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_normalColour | alpha_comp);
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
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

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
		ColourRect imgcols = d_hoverImage.getColours();
		imgcols.setAlpha(getEffectiveAlpha());
		d_hoverImage.setColours(imgcols);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_hoverImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) / 2;
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_hoverColour | alpha_comp);
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
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

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
		ColourRect imgcols = d_pushedImage.getColours();
		imgcols.setAlpha(getEffectiveAlpha());
		d_pushedImage.setColours(imgcols);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_pushedImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) / 2;
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_pushedColour | alpha_comp);
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
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0x7F7F7F;
	ColourRect colours(colval, colval, colval, colval);

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
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_disabledImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) / 2;
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_disabledColour | alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	Set whether to auto re-size custom image areas when the button is
	sized.	
*************************************************************************/
void TLButton::setCustomImageryAutoSized(bool setting)
{
	// if we are enabling auto-sizing, scale images for current size
	if ((setting == true) && (setting != d_autoscaleImages))
	{
		Rect area(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight());

		d_normalImage.setRect(area);
		d_hoverImage.setRect(area);
		d_pushedImage.setRect(area);
		d_disabledImage.setRect(area);

		requestRedraw();
	}

	d_autoscaleImages = setting;
}


/*************************************************************************
	Handler for when button size is changed
*************************************************************************/
void TLButton::onSized(EventArgs& e)
{
	// default processing
	PushButton::onSized(e);

	// scale images if required.
	if (d_autoscaleImages)
	{
		Rect area(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight());

		d_normalImage.setRect(area);
		d_hoverImage.setRect(area);
		d_pushedImage.setRect(area);
		d_disabledImage.setRect(area);

		e.handled = true;
	}

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
	TLButton* wnd = new TLButton(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
