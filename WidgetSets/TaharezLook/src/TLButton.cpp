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

TLButtonProperties::NormalImage TLButton::d_normalImageProperty;
TLButtonProperties::PushedImage TLButton::d_pushedImageProperty;
TLButtonProperties::HoverImage  TLButton::d_hoverImageProperty;
TLButtonProperties::UseStandardImagery TLButton::d_useStandardImageryProperty;
TLButtonProperties::TextXOffset TLButton::d_textXOffsetProperty;


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

	d_textXOffset = 0.0f;

	setMouseCursor(&iset->getImage(MouseCursorImageName));

	addTLButtonProperties();
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
		d_normalImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
	}

	requestRedraw();
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
		d_hoverImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
	}

	requestRedraw();
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
		d_pushedImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
	}

	requestRedraw();
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
		d_disabledImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
	}

	requestRedraw();
}

float TLButton::getTextXOffset() const
{
   return d_textXOffset;
}

void TLButton::setTextXOffset(float offset)
{
   d_textXOffset = offset;
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
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getBaseline()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_normalColour);
	colours.setAlpha(alpha_comp);
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
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getBaseline()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_hoverColour);
	colours.setAlpha(alpha_comp);
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
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getBaseline()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_pushedColour);
	colours.setAlpha(alpha_comp);
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
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getBaseline()) * 0.5f);
	absrect.d_left += PixelAligned(d_textXOffset * absrect.getWidth());
	colours.setColours(d_disabledColour);
	colours.setAlpha(alpha_comp);
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
void TLButton::onSized(WindowEventArgs& e)
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

void TLButton::addTLButtonProperties(void)
{
   addProperty(&d_normalImageProperty);
   addProperty(&d_pushedImageProperty);
   addProperty(&d_hoverImageProperty);
   addProperty(&d_useStandardImageryProperty);
   addProperty(&d_textXOffsetProperty);
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
