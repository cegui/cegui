/************************************************************************
	filename: 	WLSliderThumb.cpp
	created:	10/8/2004
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
#include "WLSliderThumb.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLSliderThumb::WidgetTypeName[]			= "WindowsLook/SliderThumb";

// Image names
const utf8	WLSliderThumb::ImagesetName[]			= "WindowsLook";
const utf8	WLSliderThumb::NormalImageName[]		= "SliderThumbNormal";
const utf8	WLSliderThumb::HighlightImageName[]		= "SliderThumbHover";
const utf8	WLSliderThumb::MouseCursorImageName[]	= "MouseEsWeCursor";


/*************************************************************************
	Constructor
*************************************************************************/
WLSliderThumb::WLSliderThumb(const String& type, const String& name) :
	Thumb(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_normalImage		= &iset->getImage(NormalImageName);
	d_highlightImage	= &iset->getImage(HighlightImageName);

	setMouseCursor(&iset->getImage(MouseCursorImageName));
}


/*************************************************************************
	Destructor
*************************************************************************/
WLSliderThumb::~WLSliderThumb(void)
{
}


/*************************************************************************
	render the thumb in the normal state.	
*************************************************************************/
void WLSliderThumb::drawNormal(float z)
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

	// draw the image
	d_normalImage->draw(absrect, z, clipper, colours);
}


/*************************************************************************
	render the thumb in the hover / highlighted state.	
*************************************************************************/
void WLSliderThumb::drawHover(float z)
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

	// draw the image
	d_highlightImage->draw(absrect, z, clipper, colours);
}


/*************************************************************************
	render the thumb in the disabled state
*************************************************************************/
void WLSliderThumb::drawDisabled(float z)
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

	// draw the image
	d_normalImage->draw(absrect, z, clipper, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLSliderThumb
*************************************************************************/
Window* WLSliderThumbFactory::createWindow(const String& name)
{
	WLSliderThumb* wnd = new WLSliderThumb(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
