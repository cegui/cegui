/************************************************************************
	filename: 	TLVertScrollbarThumb.cpp
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of large vertical scrollbar thumb widget.
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
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "TLVertScrollbarThumb.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	TLVertScrollbarThumb::WidgetTypeName[]	= "TaharezLook/LargeVerticalScrollbarThumb";

// Image names
const utf8	TLVertScrollbarThumb::ImagesetName[]		= "TaharezLook";
const utf8	TLVertScrollbarThumb::NormalImageName[]		= "VertScrollThumbNormal";
const utf8	TLVertScrollbarThumb::HighlightImageName[]	= "VertScrollThumbHover";


/*************************************************************************
	Constructor
*************************************************************************/
TLVertScrollbarThumb::TLVertScrollbarThumb(const String& type, const String& name) :
	Thumb(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_normalImage		= &iset->getImage(NormalImageName);
	d_highlightImage	= &iset->getImage(HighlightImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
TLVertScrollbarThumb::~TLVertScrollbarThumb(void)
{
}


/*************************************************************************
	render the thumb in the normal state.
*************************************************************************/
void TLVertScrollbarThumb::drawNormal(float z)
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
void TLVertScrollbarThumb::drawHover(float z)
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
void TLVertScrollbarThumb::drawDisabled(float z)
{
	Rect clipper(getPixelRect());

	// if the widget is not totally clipped.
	if (clipper.getWidth() != 0)
	{
		// draw the image
		d_normalImage->draw(getUnclippedPixelRect(), z, clipper, colour(0.5f, 0.5f, 0.5f, getEffectiveAlpha()));
	}

}


/*************************************************************************
	Handler for when size changes
*************************************************************************/
void TLVertScrollbarThumb::onSized(WindowEventArgs& e)
{
	// We want to keep this thumb's aspect the same, so when the size
	// changes, we modify the height in relation to the width (since that is
	// known, but the height is not).
	float ratio = getAbsoluteWidth() / d_normalImage->getWidth();
	d_abs_area.setHeight(d_normalImage->getHeight() * ratio);
	d_rel_area.setHeight(absoluteToRelativeY_impl(getParent(), d_abs_area.getHeight()));

	// base class processing.
	Thumb::onSized(e);

	e.handled = true;
}



//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLVertScrollbarThumb
*************************************************************************/
Window* TLVertScrollbarThumbFactory::createWindow(const String& name)
{
	TLVertScrollbarThumb* wnd = new TLVertScrollbarThumb(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
