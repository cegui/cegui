/************************************************************************
	filename: 	TLMiniVertScrollbarThumb.cpp
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of thumb for Taharez mini vertical
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
#include "TLMiniVertScrollbarThumb.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// Image names
const utf8	TLMiniVertScrollbarThumb::ImagesetName[]				= "TaharezImagery";
const utf8	TLMiniVertScrollbarThumb::NormalImageName[]				= "MiniVertScrollThumbNormal";
const utf8	TLMiniVertScrollbarThumb::NormalTopImageName[]			= "MiniVertScrollThumbTopNormal";
const utf8	TLMiniVertScrollbarThumb::NormalMiddleImageName[]		= "MiniVertScrollThumbMiddleNormal";
const utf8	TLMiniVertScrollbarThumb::NormalBottomImageName[]		= "MiniVertScrollThumbBottomNormal";
const utf8	TLMiniVertScrollbarThumb::HighlightTopImageName[]		= "MiniVertScrollThumbTopHover";
const utf8	TLMiniVertScrollbarThumb::HighlightMiddleImageName[]	= "MiniVertScrollThumbMiddleHover";
const utf8	TLMiniVertScrollbarThumb::HighlightBottomImageName[]	= "MiniVertScrollThumbBottomHover";


/*************************************************************************
	Constructor
*************************************************************************/
TLMiniVertScrollbarThumb::TLMiniVertScrollbarThumb(const String& type, const String& name) :
	Thumb(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_normalImage			= &iset->getImage(NormalImageName);
	d_normalTopImage		= &iset->getImage(NormalTopImageName);
	d_normalMiddleImage		= &iset->getImage(NormalMiddleImageName);
	d_normalBottomImage		= &iset->getImage(NormalBottomImageName);
	d_highlightTopImage		= &iset->getImage(HighlightTopImageName);
	d_highlightMiddleImage	= &iset->getImage(HighlightMiddleImageName);
	d_highlightBottomImage	= &iset->getImage(HighlightBottomImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
TLMiniVertScrollbarThumb::~TLMiniVertScrollbarThumb(void)
{
}


/*************************************************************************
	render the thumb in the normal state.
*************************************************************************/
void TLMiniVertScrollbarThumb::drawNormal(float z)
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
	float minHeight		= absrect.getHeight() * 0.5f;
	float topHeight		= min(d_normalTopImage->getHeight(), minHeight);
	float bottomHeight	= min(d_normalBottomImage->getHeight(), minHeight);
	float middleHeight	= absrect.getHeight() - topHeight - bottomHeight;


	// draw the images
	Vector3	pos(absrect.d_left, absrect.d_top, z);
	Size	sz(absrect.getWidth(), topHeight);
	d_normalTopImage->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = middleHeight;
	d_normalMiddleImage->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = bottomHeight;
	d_normalBottomImage->draw(pos, sz, clipper, colours);
}


/*************************************************************************
	render the thumb in the hover / highlighted state.
*************************************************************************/
void TLMiniVertScrollbarThumb::drawHover(float z)
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
	float minHeight		= absrect.getHeight() * 0.5f;
	float topHeight		= min(d_highlightTopImage->getHeight(), minHeight);
	float bottomHeight	= min(d_highlightBottomImage->getHeight(), minHeight);
	float middleHeight	= absrect.getHeight() - topHeight - bottomHeight;


	// draw the images
	Vector3	pos(absrect.d_left, absrect.d_top, z);
	Size	sz(absrect.getWidth(), topHeight);
	d_highlightTopImage->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = middleHeight;
	d_highlightMiddleImage->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = bottomHeight;
	d_highlightBottomImage->draw(pos, sz, clipper, colours);
}


/*************************************************************************
	render the thumb in the disabled state
*************************************************************************/
void TLMiniVertScrollbarThumb::drawDisabled(float z)
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
	float minHeight		= absrect.getHeight() * 0.5f;
	float topHeight		= min(d_highlightTopImage->getHeight(), minHeight);
	float bottomHeight	= min(d_highlightBottomImage->getHeight(), minHeight);
	float middleHeight	= absrect.getHeight() - topHeight - bottomHeight;


	// draw the images
	Vector3	pos(absrect.d_left, absrect.d_top, z);
	Size	sz(absrect.getWidth(), topHeight);
	d_highlightTopImage->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = middleHeight;
	d_highlightMiddleImage->draw(pos, sz, clipper, colours);

	pos.d_y += sz.d_height;
	sz.d_height = bottomHeight;
	d_highlightBottomImage->draw(pos, sz, clipper, colours);
}


/*************************************************************************
	Handler for when size changes
*************************************************************************/
void TLMiniVertScrollbarThumb::onSized(WindowEventArgs& e)
{
	// calculate preferred height from width (which is known).
	float prefHeight = d_normalImage->getHeight() * (getAbsoluteWidth() / d_normalImage->getWidth());

	Window* par = getParent();

	// Only proceed if parent is not NULL.
	if (par != NULL)
	{
		// calculate scaled height.
		float scaledHeight = (par->getAbsoluteHeight() - (2 * par->getAbsoluteWidth())) * 0.575f;

		// use preferred height if there is room, else use the scaled height.
		if (scaledHeight < prefHeight)
		{
			prefHeight = scaledHeight;
		}
	}

	// install new height values.
	d_abs_area.setHeight(prefHeight);
	d_rel_area.setHeight(absoluteToRelativeY_impl(getParent(), prefHeight));

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
	Create, initialise and return a TLMiniVertScrollbarThumb
*************************************************************************/
Window* TLMiniVertScrollbarThumbFactory::createWindow(const String& name)
{
	TLMiniVertScrollbarThumb* wnd = new TLMiniVertScrollbarThumb(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
