/************************************************************************
	filename: 	TLMiniHorzScrollbarThumb.cpp
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of thumb for Taharez mini horizontal
				scroll bar.
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
#include "TLMiniHorzScrollbarThumb.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// Image names
const utf8	TLMiniHorzScrollbarThumb::ImagesetName[]				= "TaharezImagery";
const utf8	TLMiniHorzScrollbarThumb::NormalImageName[]				= "MiniHorzScrollThumbNormal";
const utf8	TLMiniHorzScrollbarThumb::NormalLeftImageName[]			= "MiniHorzScrollThumbLeftNormal";
const utf8	TLMiniHorzScrollbarThumb::NormalMiddleImageName[]		= "MiniHorzScrollThumbMiddleNormal";
const utf8	TLMiniHorzScrollbarThumb::NormalRightImageName[]		= "MiniHorzScrollThumbRightNormal";
const utf8	TLMiniHorzScrollbarThumb::HighlightLeftImageName[]		= "MiniHorzScrollThumbLeftHover";
const utf8	TLMiniHorzScrollbarThumb::HighlightMiddleImageName[]	= "MiniHorzScrollThumbMiddleHover";
const utf8	TLMiniHorzScrollbarThumb::HighlightRightImageName[]		= "MiniHorzScrollThumbRightHover";


/*************************************************************************
	Constructor
*************************************************************************/
TLMiniHorzScrollbarThumb::TLMiniHorzScrollbarThumb(const String& type, const String& name) :
	Thumb(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_normalImage			= &iset->getImage(NormalImageName);
	d_normalLeftImage		= &iset->getImage(NormalLeftImageName);
	d_normalMiddleImage		= &iset->getImage(NormalMiddleImageName);
	d_normalRightImage		= &iset->getImage(NormalRightImageName);
	d_highlightLeftImage	= &iset->getImage(HighlightLeftImageName);
	d_highlightMiddleImage	= &iset->getImage(HighlightMiddleImageName);
	d_highlightRightImage	= &iset->getImage(HighlightRightImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
TLMiniHorzScrollbarThumb::~TLMiniHorzScrollbarThumb(void)
{
}


/*************************************************************************
	render the thumb in the normal state.
*************************************************************************/
void TLMiniHorzScrollbarThumb::drawNormal(float z)
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

	// calculate segment sizes
	using namespace std;
	float minWidth		= absrect.getWidth() * 0.5f;
	float leftWidth		= min(d_normalLeftImage->getWidth(), minWidth);
	float rightWidth	= min(d_normalRightImage->getWidth(), minWidth);
	float middleWidth	= absrect.getWidth() - leftWidth - rightWidth;


	// draw the images
	Vector3	pos(absrect.d_left, absrect.d_top, z);
	Size	sz(leftWidth, absrect.getHeight());
	d_normalLeftImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = middleWidth;
	d_normalMiddleImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = rightWidth;
	d_normalRightImage->draw(pos, sz, clipper, colours);
}


/*************************************************************************
	render the thumb in the hover / highlighted state.
*************************************************************************/
void TLMiniHorzScrollbarThumb::drawHover(float z)
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

	// calculate segment sizes
	using namespace std;
	float minWidth		= absrect.getWidth() * 0.5f;
	float leftWidth		= min(d_highlightLeftImage->getWidth(), minWidth);
	float rightWidth	= min(d_highlightRightImage->getWidth(), minWidth);
	float middleWidth	= absrect.getWidth() - leftWidth - rightWidth;


	// draw the images
	Vector3	pos(absrect.d_left, absrect.d_top, z);
	Size	sz(leftWidth, absrect.getHeight());
	d_highlightLeftImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = middleWidth;
	d_highlightMiddleImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = rightWidth;
	d_highlightRightImage->draw(pos, sz, clipper, colours);
}


/*************************************************************************
	render the thumb in the disabled state
*************************************************************************/
void TLMiniHorzScrollbarThumb::drawDisabled(float z)
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

	// calculate segment sizes
	using namespace std;
	float minWidth		= absrect.getWidth() * 0.5f;
	float leftWidth		= min(d_normalLeftImage->getWidth(), minWidth);
	float rightWidth	= min(d_normalRightImage->getWidth(), minWidth);
	float middleWidth	= absrect.getWidth() - leftWidth - rightWidth;


	// draw the images
	Vector3	pos(absrect.d_left, absrect.d_top, z);
	Size	sz(leftWidth, absrect.getHeight());
	d_normalLeftImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = middleWidth;
	d_normalMiddleImage->draw(pos, sz, clipper, colours);

	pos.d_x += sz.d_width;
	sz.d_width = rightWidth;
	d_normalRightImage->draw(pos, sz, clipper, colours);
}


/*************************************************************************
	Handler for when size changes
*************************************************************************/
void TLMiniHorzScrollbarThumb::onSized(WindowEventArgs& e)
{
	// calculate preferred width from height(which is known).
	float prefWidth = d_normalImage->getWidth() * (getAbsoluteHeight() / d_normalImage->getHeight());

	Window* par = getParent();

	// Only proceed if parent is not NULL.
	if (par != NULL)
	{
		// calculate scaled height.
		float scaledWidth = (par->getAbsoluteWidth() - (2 * par->getAbsoluteHeight())) * 0.575f;

		// use preferred width if there is room, else use the scaled width.
		if (scaledWidth < prefWidth)
		{
			prefWidth = scaledWidth;
		}
	}

	// install new width values.
	d_abs_area.setWidth(prefWidth);
	d_rel_area.setWidth(absoluteToRelativeX_impl(getParent(), prefWidth));

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
	Create, initialise and return a TLMiniHorzScrollbarThumb
*************************************************************************/
Window* TLMiniHorzScrollbarThumbFactory::createWindow(const String& name)
{
	TLMiniHorzScrollbarThumb* wnd = new TLMiniHorzScrollbarThumb(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
