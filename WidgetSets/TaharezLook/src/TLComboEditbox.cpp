/************************************************************************
	filename: 	TLComboEditbox.cpp
	created:	13/6/2004
	author:		Paul D Turner
	
	purpose:	Implements Taharez Look Combobox-Editbox widget
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
#include "TLComboEditbox.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLComboEditbox::WidgetTypeName[]	= "TaharezLook/ComboEditbox";

// image name constants
const utf8	TLComboEditbox::ImagesetName[]				= "TaharezLook";
const utf8	TLComboEditbox::ContainerLeftImageName[]	= "ComboboxEditLeft";
const utf8	TLComboEditbox::ContainerMiddleImageName[]	= "ComboboxEditMiddle";
const utf8	TLComboEditbox::CaratImageName[]			= "EditBoxCarat";
const utf8	TLComboEditbox::SelectionBrushImageName[]	= "ComboboxSelectionBrush";
const utf8	TLComboEditbox::MouseCursorImageName[]		= "MouseTextBar";

// layout values
const float	TLComboEditbox::TextPaddingRatio		= 0.5f;

// implementation constantss
const uint	TLComboEditbox::SelectionLayer	= 1;
const uint	TLComboEditbox::TextLayer		= 2;
const uint	TLComboEditbox::CaratLayer		= 3;


/*************************************************************************
	Constructor for Taharez edit box widgets	
*************************************************************************/
TLComboEditbox::TLComboEditbox(const String& type, const String& name) :
	Editbox(type, name),
	d_lastTextOffset(0)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	
	// cache images to be used
	d_left		= &iset->getImage(ContainerLeftImageName);
	d_middle	= &iset->getImage(ContainerMiddleImageName);
	d_carat		= &iset->getImage(CaratImageName);
	d_selection	= &iset->getImage(SelectionBrushImageName);

	setMouseCursor(&iset->getImage(MouseCursorImageName));
}


/*************************************************************************
	Destructor for Taharez edit box widgets	
*************************************************************************/
TLComboEditbox::~TLComboEditbox(void)
{
}


/*************************************************************************
	Return the text code point index that is rendered closest to screen
	position 'pt'.	
*************************************************************************/
ulong TLComboEditbox::getTextIndexFromPosition(const Point& pt) const
{
	//
	// calculate final window position to be checked
	//
	float wndx = screenToWindowX(pt.d_x);

	if (getMetricsMode() == Relative)
	{
		wndx = relativeToAbsoluteX(wndx);
	}

	wndx -= d_lastTextOffset;

	//
	// Return the proper index
	//
	if (isTextMasked())
	{
		return getFont()->getCharAtPixel(String(d_text.length(), getMaskCodePoint()), 0, wndx);
	}
	else
	{
		return getFont()->getCharAtPixel(d_text, 0, wndx);
	}
	
}


/*************************************************************************
	return text padding value to use in pixels	
*************************************************************************/
float TLComboEditbox::getTextPaddingPixels(void) const
{
	return PixelAligned(d_left->getWidth() * TextPaddingRatio);
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void TLComboEditbox::drawSelf(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	const Font* fnt = getFont();
	Renderer*	renderer = System::getSingleton().getRenderer();

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	float alpha_comp = getEffectiveAlpha();
	ColourRect colours(colour(1, 1, 1, alpha_comp));

	bool hasFocus = hasInputFocus();

	//
	// render container
	//
	// calculate widths for container segments
	float leftWidth		= d_left->getWidth();
	float midWidth		= absrect.getWidth() - leftWidth;

	Vector3 pos(absrect.d_left, absrect.d_top, z);
	Size	sz(leftWidth, absrect.getHeight());

	// left end
	d_left->draw(pos, sz, clipper, colours);

	// stretchy middle segment
	pos.d_x += sz.d_width;
	sz.d_width = midWidth;
	d_middle->draw(pos, sz, clipper, colours);


	//
	// Required preliminary work for main rendering operations
	//
	// Create a 'masked' version of the string if needed.
	String editText;

	if (isTextMasked())
	{
		editText.insert(0, d_text.length(), getMaskCodePoint());
	}
	else
	{
		editText = d_text;
	}

	// calculate new area rect considering text padding value.
	float textpadding = getTextPaddingPixels();

	absrect.d_left		+= textpadding;
	absrect.d_top		+= textpadding;
	absrect.d_right		-= textpadding;
	absrect.d_bottom	-= textpadding;

	// calculate best position to render text to ensure carat is always visible
	float textOffset;
	float extentToCarat = fnt->getTextExtent(editText.substr(0, getCaratIndex()));

	// if box is inactive
	if (!hasFocus)
	{
		textOffset = d_lastTextOffset;
	}
	// if carat is to the left of the box
	else if ((d_lastTextOffset + extentToCarat) < 0)
	{
		textOffset = -extentToCarat;
	}
	// if carat is off to the right.
	else if ((d_lastTextOffset + extentToCarat) >= (absrect.getWidth() - d_carat->getWidth()))
	{
		textOffset = absrect.getWidth() - extentToCarat - d_carat->getWidth();
	}
	// else carat is already within the box
	else
	{
		textOffset = d_lastTextOffset;
	}

	// adjust clipper for new target area
	clipper = absrect.getIntersection(clipper);


	//
	// Render carat
	//
	if ((!isReadOnly()) && hasFocus)
	{
		Vector3 pos(absrect.d_left + textOffset + extentToCarat, absrect.d_top, renderer->getZLayer(CaratLayer));
		Size	sz(d_carat->getWidth(), absrect.getHeight());
		d_carat->draw(pos, sz, clipper, colours);
	}

	//
	// Draw label text
	//
	// setup initial rect for text formatting
	Rect text_rect(absrect);
	text_rect.d_top  += PixelAligned((text_rect.getHeight() - getFont()->getLineSpacing()) * 0.5f);
	text_rect.d_left += textOffset;

	// draw pre-highlight text
	String sect = editText.substr(0, getSelectionStartIndex());
	colours.setColours(d_normalTextColour);
	colours.setAlpha(alpha_comp);
	fnt->drawText(sect, text_rect, renderer->getZLayer(TextLayer), clipper, LeftAligned, colours);

	text_rect.d_left += fnt->getTextExtent(sect);

	// draw highlight text
	sect = editText.substr(getSelectionStartIndex(), getSelectionLength());
	colours.setColours(d_selectTextColour);
	colours.setAlpha(alpha_comp);
	fnt->drawText(sect, text_rect, renderer->getZLayer(TextLayer), clipper, LeftAligned, colours);

	text_rect.d_left += fnt->getTextExtent(sect);

	// draw post-highlight text
	sect = editText.substr(getSelectionEndIndex());
	colours.setColours(d_normalTextColour);
	colours.setAlpha(alpha_comp);
	fnt->drawText(sect, text_rect, renderer->getZLayer(TextLayer), clipper, LeftAligned, colours);


	//
	// Render selection brush
	//
	if (getSelectionLength() != 0)
	{
		// calculate required start and end offsets
		float selStartOffset	= fnt->getTextExtent(editText.substr(0, getSelectionStartIndex()));
		float selEndOffset		= fnt->getTextExtent(editText.substr(0, getSelectionEndIndex()));

		// setup colours
		colours.setColours((hasFocus && (!isReadOnly())) ? d_selectBrushColour : d_inactiveSelectBrushColour);
		colours.setAlpha(alpha_comp);

		// calculate highlight area
		Rect hlarea;
		hlarea.d_left	= absrect.d_left + textOffset + selStartOffset;
		hlarea.d_right	= absrect.d_left + textOffset + selEndOffset;
		hlarea.d_top	= text_rect.d_top;
		hlarea.d_bottom = hlarea.d_top + fnt->getLineSpacing();

		// render the highlight
		d_selection->draw(hlarea, renderer->getZLayer(SelectionLayer), clipper, colours);
	}

	d_lastTextOffset = textOffset;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLComboEditbox
*************************************************************************/
Window* TLComboEditboxFactory::createWindow(const String& name)
{
	TLComboEditbox* wnd = new TLComboEditbox(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
