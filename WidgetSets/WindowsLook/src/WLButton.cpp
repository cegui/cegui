/************************************************************************
	filename: 	WLButton.cpp
	created:	8/8/2004
	author:		Paul D Turner
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
#include "WLButton.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLButton::WidgetTypeName[]					= "WindowsLook/Button";
const utf8	WLButton::ImagesetName[]					= "WindowsLook";
const utf8	WLButton::BackgroundImageName[]				= "Background";
const utf8	WLButton::NormalLeftImageName[]				= "ButtonNormalLeft";
const utf8	WLButton::NormalRightImageName[]			= "ButtonNormalRight";
const utf8	WLButton::NormalTopImageName[]				= "ButtonNormalTop";
const utf8	WLButton::NormalBottomImageName[]			= "ButtonNormalBottom";
const utf8	WLButton::NormalTopLeftImageName[]			= "ButtonNormalTopLeft";
const utf8	WLButton::NormalTopRightImageName[]			= "ButtonNormalTopRight";
const utf8	WLButton::NormalBottomLeftImageName[]		= "ButtonNormalBottomLeft";
const utf8	WLButton::NormalBottomRightImageName[]		= "ButtonNormalBottomRight";
const utf8	WLButton::HoverLeftImageName[]				= "ButtonHoverLeft";
const utf8	WLButton::HoverRightImageName[]				= "ButtonHoverRight";
const utf8	WLButton::HoverTopImageName[]				= "ButtonHoverTop";
const utf8	WLButton::HoverBottomImageName[]			= "ButtonHoverBottom";
const utf8	WLButton::HoverTopLeftImageName[]			= "ButtonHoverTopLeft";
const utf8	WLButton::HoverTopRightImageName[]			= "ButtonHoverTopRight";
const utf8	WLButton::HoverBottomLeftImageName[]		= "ButtonHoverBottomLeft";
const utf8	WLButton::HoverBottomRightImageName[]		= "ButtonHoverBottomRight";
const utf8	WLButton::PushedLeftImageName[]				= "ButtonPushedLeft";
const utf8	WLButton::PushedRightImageName[]			= "ButtonPushedRight";
const utf8	WLButton::PushedTopImageName[]				= "ButtonPushedTop";
const utf8	WLButton::PushedBottomImageName[]			= "ButtonPushedBottom";
const utf8	WLButton::PushedTopLeftImageName[]			= "ButtonPushedTopLeft";
const utf8	WLButton::PushedTopRightImageName[]			= "ButtonPushedTopRight";
const utf8	WLButton::PushedBottomLeftImageName[]		= "ButtonPushedBottomLeft";
const utf8	WLButton::PushedBottomRightImageName[]		= "ButtonPushedBottomRight";
const utf8	WLButton::MouseCursorImageName[]			= "MouseArrow";

// colours
const colour	WLButton::NormalPrimaryColour		= 0xAFAFAF;
const colour	WLButton::NormalSecondaryColour		= 0xFFFFFF;
const colour	WLButton::HoverPrimaryColour		= 0xCFD9CF;
const colour	WLButton::HoverSecondaryColour		= 0xF2FFF2;
const colour	WLButton::PushedPrimaryColour		= 0xAFAFAF;
const colour	WLButton::PushedSecondaryColour		= 0xFFFFFF;
const colour	WLButton::DisabledPrimaryColour		= 0x999999;
const colour	WLButton::DisabledSecondaryColour	= 0x999999;
const colour	WLButton::EnabledTextLabelColour	= 0x000000;
const colour	WLButton::DisabledTextLabelColour	= 0x888888;

	
/*************************************************************************
	Constructor
*************************************************************************/
WLButton::WLButton(const String& type, const String& name) :
	PushButton(type, name)
{
	// default options
	d_autoscaleImages		= true;
	d_useStandardImagery	= true;
	d_useNormalImage		= false;
	d_useHoverImage			= false;
	d_usePushedImage		= false;
	d_useDisabledImage		= false;

	storeFrameSizes();

	// setup frames & image pointers
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_normalFrame.setImages(
		&iset->getImage(NormalTopLeftImageName),
		&iset->getImage(NormalTopRightImageName),
		&iset->getImage(NormalBottomLeftImageName),
		&iset->getImage(NormalBottomRightImageName),
		&iset->getImage(NormalLeftImageName),
		&iset->getImage(NormalTopImageName),
		&iset->getImage(NormalRightImageName),
		&iset->getImage(NormalBottomImageName)
	);

	d_hoverFrame.setImages(
		&iset->getImage(HoverTopLeftImageName),
		&iset->getImage(HoverTopRightImageName),
		&iset->getImage(HoverBottomLeftImageName),
		&iset->getImage(HoverBottomRightImageName),
		&iset->getImage(HoverLeftImageName),
		&iset->getImage(HoverTopImageName),
		&iset->getImage(HoverRightImageName),
		&iset->getImage(HoverBottomImageName)
	);

	d_pushedFrame.setImages(
		&iset->getImage(PushedTopLeftImageName),
		&iset->getImage(PushedTopRightImageName),
		&iset->getImage(PushedBottomLeftImageName),
		&iset->getImage(PushedBottomRightImageName),
		&iset->getImage(PushedLeftImageName),
		&iset->getImage(PushedTopImageName),
		&iset->getImage(PushedRightImageName),
		&iset->getImage(PushedBottomImageName)
	);

	d_background	= &iset->getImage(BackgroundImageName);

	// set mouse for this widget.
	setMouseCursor(&iset->getImage(MouseCursorImageName));

	// set default colours for text
	d_normalColour	 = EnabledTextLabelColour;
	d_hoverColour	 = EnabledTextLabelColour;
	d_pushedColour	 = EnabledTextLabelColour;
	d_disabledColour = DisabledTextLabelColour;
}


/*************************************************************************
	Destructor
*************************************************************************/
WLButton::~WLButton(void)
{
}



/*************************************************************************
	set whether or not to render the standard imagery for the button	
*************************************************************************/
void WLButton::setStandardImageryEnabled(bool setting)
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
void WLButton::setNormalImage(const RenderableImage* image)
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
void WLButton::setHoverImage(const RenderableImage* image)
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
void WLButton::setPushedImage(const RenderableImage* image)
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
void WLButton::setDisabledImage(const RenderableImage* image)
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


/*************************************************************************
	render Widget in normal state	
*************************************************************************/
void WLButton::drawNormal(float z)
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
	ColourRect colours(NormalPrimaryColour, NormalSecondaryColour, NormalSecondaryColour, NormalPrimaryColour);
	colours.setAlpha(alpha_comp);

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// draw background image
		Rect bkRect(absrect);
		bkRect.d_left	+= d_frameLeftSize;
		bkRect.d_right	-= d_frameRightSize;
		bkRect.d_top	+= d_frameTopSize;
		bkRect.d_bottom	-= d_frameBottomSize;
		d_background->draw(bkRect, z, clipper, colours);

		// draw frame
		d_normalFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);
	}

	// render clients custom image if that is required.
	if (d_useNormalImage)
	{
		colours = d_normalImage.getColours();
		colours.setAlpha(getEffectiveAlpha());
		d_normalImage.setColours(colours);
		Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
		d_normalImage.draw(imgpos, clipper);
	}

	//
	// Draw label text
	//
	colours.setColours(d_normalColour);
	colours.setAlpha(alpha_comp);
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) / 2);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in hover / highlight state	
*************************************************************************/
void WLButton::drawHover(float z)
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
	ColourRect colours(HoverPrimaryColour, HoverSecondaryColour, HoverSecondaryColour, HoverPrimaryColour);
	colours.setAlpha(alpha_comp);

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// draw background image
		Rect bkRect(absrect);
		bkRect.d_left	+= d_frameLeftSize;
		bkRect.d_right	-= d_frameRightSize;
		bkRect.d_top	+= d_frameTopSize;
		bkRect.d_bottom	-= d_frameBottomSize;
		d_background->draw(bkRect, z, clipper, colours);

		// draw frame
		d_hoverFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);
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
	colours.setColours(d_hoverColour);
	colours.setAlpha(alpha_comp);
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) / 2);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in Pushed state	
*************************************************************************/
void WLButton::drawPushed(float z)
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
	ColourRect colours(PushedPrimaryColour, PushedSecondaryColour, PushedSecondaryColour, PushedPrimaryColour);
	colours.setAlpha(alpha_comp);

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// draw background image
		Rect bkRect(absrect);
		bkRect.d_left	+= d_frameLeftSize;
		bkRect.d_right	-= d_frameRightSize;
		bkRect.d_top	+= d_frameTopSize;
		bkRect.d_bottom	-= d_frameBottomSize;
		d_background->draw(bkRect, z, clipper, colours);

		// draw frame
		d_pushedFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);
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
	colours.setColours(d_pushedColour);
	colours.setAlpha(alpha_comp);
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) / 2);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	render Widget in disabled state	
*************************************************************************/
void WLButton::drawDisabled(float z)
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
	ColourRect colours(DisabledPrimaryColour, DisabledSecondaryColour, DisabledSecondaryColour, DisabledPrimaryColour);
	colours.setAlpha(alpha_comp);

	// render standard button imagery if required.
	if (d_useStandardImagery)
	{
		// draw background image
		Rect bkRect(absrect);
		bkRect.d_left	+= d_frameLeftSize;
		bkRect.d_right	-= d_frameRightSize;
		bkRect.d_top	+= d_frameTopSize;
		bkRect.d_bottom	-= d_frameBottomSize;
		d_background->draw(bkRect, z, clipper, colours);

		// draw frame
		d_normalFrame.draw(Vector3(absrect.d_left,absrect.d_top,z), clipper);
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
	colours.setColours(d_disabledColour);
	colours.setAlpha(alpha_comp);
	absrect.d_top += PixelAligned((absrect.getHeight() - getFont()->getLineSpacing()) / 2);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
	Set whether to auto re-size custom image areas when the button is
	sized.	
*************************************************************************/
void WLButton::setCustomImageryAutoSized(bool setting)
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
	Store sizes of frame edges
*************************************************************************/
void WLButton::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= iset->getImageWidth(NormalLeftImageName);
	d_frameRightSize	= iset->getImageWidth(NormalRightImageName);
	d_frameTopSize		= iset->getImageHeight(NormalTopImageName);
	d_frameBottomSize	= iset->getImageHeight(NormalBottomImageName);
}


/*************************************************************************
	Handler for when button size is changed
*************************************************************************/
void WLButton::onSized(WindowEventArgs& e)
{
	// default processing
	PushButton::onSized(e);

	Size absSize(getAbsoluteSize());

	// update frame size.
	d_normalFrame.setSize(absSize);
	d_hoverFrame.setSize(absSize);
	d_pushedFrame.setSize(absSize);

	// scale user images if required.
	if (d_autoscaleImages)
	{
		Rect area(0, 0, absSize.d_width, absSize.d_height);

		d_normalImage.setRect(area);
		d_hoverImage.setRect(area);
		d_pushedImage.setRect(area);
		d_disabledImage.setRect(area);
	}

	e.handled = true;
}


/*************************************************************************
	Handler for when alpha changes.
*************************************************************************/
void WLButton::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing.
	PushButton::onAlphaChanged(e);

	ColourRect cols;
	float alpha = getEffectiveAlpha();

	cols = d_normalFrame.getColours();
	cols.setAlpha(alpha);
	d_normalFrame.setColours(cols);

	cols = d_hoverFrame.getColours();
	cols.setAlpha(alpha);
	d_hoverFrame.setColours(cols);

	cols = d_pushedFrame.getColours();
	cols.setAlpha(alpha);
	d_pushedFrame.setColours(cols);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLButton
*************************************************************************/
Window* WLButtonFactory::createWindow(const String& name)
{
	WLButton* wnd = new WLButton(d_type, name);
	wnd->initialise();

	return wnd;
}


} // End of  CEGUI namespace section
