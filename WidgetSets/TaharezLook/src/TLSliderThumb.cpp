/************************************************************************
	filename: 	TLSliderThumb.cpp
	created:	22/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of thumb used with Taharez Slider widget
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
#include "TLSliderThumb.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLSliderThumb::WidgetTypeName[]	= "TaharezLook/SliderThumb";

// Image names
const utf8	TLSliderThumb::ImagesetName[]			= "TaharezLook";
const utf8	TLSliderThumb::NormalImageName[]		= "VertSliderThumbNormal";
const utf8	TLSliderThumb::HighlightImageName[]		= "VertSliderThumbHover";


/*************************************************************************
	Constructor
*************************************************************************/
TLSliderThumb::TLSliderThumb(const String& type, const String& name) :
	Thumb(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_normalImage		= &iset->getImage(NormalImageName);
	d_highlightImage	= &iset->getImage(HighlightImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
TLSliderThumb::~TLSliderThumb(void)
{
}


/*************************************************************************
	render the thumb in the normal state.	
*************************************************************************/
void TLSliderThumb::drawNormal(float z)
{
	Rect clipper(getPixelRect());

	// if the widget is not totally clipped.
	if (clipper.getWidth() != 0)
	{
		// draw the image
		d_normalImage->draw(getUnclippedPixelRect(), z, clipper, colour(1, 1, 1, getEffectiveAlpha()));
	}

}


/*************************************************************************
	render the thumb in the hover / highlighted state.	
*************************************************************************/
void TLSliderThumb::drawHover(float z)
{
	Rect clipper(getPixelRect());

	// if the widget is not totally clipped.
	if (clipper.getWidth() != 0)
	{
		// draw the image
		d_highlightImage->draw(getUnclippedPixelRect(), z, clipper, colour(1, 1, 1, getEffectiveAlpha()));
	}

}


/*************************************************************************
	render the thumb in the disabled state
*************************************************************************/
void TLSliderThumb::drawDisabled(float z)
{
	Rect clipper(getPixelRect());

	// if the widget is not totally clipped.
	if (clipper.getWidth() != 0)
	{
		// draw the image
		d_normalImage->draw(getUnclippedPixelRect(), z, clipper, colour(0.5f, 0.5f, 0.5f, getEffectiveAlpha()));
	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLSliderThumb
*************************************************************************/
Window* TLSliderThumbFactory::createWindow(const String& name)
{
	TLSliderThumb* wnd = new TLSliderThumb(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
