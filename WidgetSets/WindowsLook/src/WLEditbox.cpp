/************************************************************************
	filename: 	WLEditbox.cpp
	created:	9/8/2004
	author:		Paul D Turner
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
#include "WLEditbox.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLEditbox::WidgetTypeName[]				= "WindowsLook/Editbox";

// image name constants
const utf8	WLEditbox::ImagesetName[]				= "WindowsLook";
const utf8	WLEditbox::TopLeftFrameImageName[]		= "EditFrameTopLeft";
const utf8	WLEditbox::TopRightFrameImageName[]		= "EditFrameTopRight";
const utf8	WLEditbox::BottomLeftFrameImageName[]	= "EditFrameBottomLeft";
const utf8	WLEditbox::BottomRightFrameImageName[]	= "EditFrameBottomRight";
const utf8	WLEditbox::LeftFrameImageName[]			= "EditFrameLeft";
const utf8	WLEditbox::RightFrameImageName[]		= "EditFrameRight";
const utf8	WLEditbox::TopFrameImageName[]			= "EditFrameTop";
const utf8	WLEditbox::BottomFrameImageName[]		= "EditFrameBottom";
const utf8	WLEditbox::BackgroundImageName[]		= "Background";
const utf8	WLEditbox::CaratImageName[]				= "EditBoxCarat";
const utf8	WLEditbox::SelectionBrushImageName[]	= "Background";
const utf8	WLEditbox::MouseCursorImageName[]		= "MouseTextBar";

// layout values
const float	WLEditbox::TextPaddingRatio		= 1.0f;

// colours
const colour WLEditbox::ReadWriteBackgroundColour	= 0xFFFFFF;
const colour WLEditbox::ReadOnlyBackgroundColour	= 0xDFDFDF;
const colour WLEditbox::NormalTextColour			= 0x000000;
const colour WLEditbox::SelectedTextColour			= 0xFFFFFF;
const colour WLEditbox::NormalSelectionColour		= 0x607FFF;
const colour WLEditbox::InactiveSelectionColour		= 0x808080;

// implementation constantss
const uint	WLEditbox::ContainerLayer	= 1;
const uint	WLEditbox::SelectionLayer	= 2;
const uint	WLEditbox::TextLayer		= 3;
const uint	WLEditbox::CaratLayer		= 4;


/*************************************************************************
	Constructor for WindowsLook edit box widgets	
*************************************************************************/
WLEditbox::WLEditbox(const String& type, const String& name) :
	Editbox(type, name),
	d_lastTextOffset(0)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	
	d_frame.setImages(
		&iset->getImage(TopLeftFrameImageName),
		&iset->getImage(TopRightFrameImageName),
		&iset->getImage(BottomLeftFrameImageName),
		&iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName),
		&iset->getImage(TopFrameImageName),
		&iset->getImage(RightFrameImageName),
		&iset->getImage(BottomFrameImageName)
		);

	d_background = &iset->getImage(BackgroundImageName);
	d_carat		 = &iset->getImage(CaratImageName);
	d_selection	 = &iset->getImage(SelectionBrushImageName);

	setMouseCursor(&iset->getImage(MouseCursorImageName));

	storeFrameSizes();

	// set up colours
	d_normalTextColour	= NormalTextColour;
	d_selectTextColour	= SelectedTextColour;
	d_selectBrushColour	= NormalSelectionColour;
	d_inactiveSelectBrushColour	= InactiveSelectionColour;
}


/*************************************************************************
	Destructor for WindowsLook edit box widgets	
*************************************************************************/
WLEditbox::~WLEditbox(void)
{
}


/*************************************************************************
	Return the text code point index that is rendered closest to screen
	position 'pt'.	
*************************************************************************/
ulong WLEditbox::getTextIndexFromPosition(const Point& pt) const
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
		return getFont()->getCharAtPixel(String(d_text.length(), getMaskCodePoint()), wndx);
	}
	else
	{
		return getFont()->getCharAtPixel(d_text, wndx);
	}
	
}


/*************************************************************************
	return text padding value to use in pixels	
*************************************************************************/
float WLEditbox::getTextPaddingPixels(void) const
{
	return PixelAligned(d_frameLeftSize * TextPaddingRatio);
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void WLEditbox::drawSelf(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	const Font* fnt = getFont();
	bool hasFocus = hasInputFocus();
	Renderer*	renderer = System::getSingleton().getRenderer();

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	float	alpha_comp = getEffectiveAlpha();
	ColourRect	colours(isReadOnly() ? ReadOnlyBackgroundColour : ReadWriteBackgroundColour);
	colours.setAlpha(alpha_comp);

	//
	// render container
	//
	Vector3 pos(absrect.d_left, absrect.d_top, renderer->getZLayer(ContainerLayer));
	d_frame.draw(pos, clipper);

	// calculate inner area rect considering frame
	absrect.d_left		+= d_frameLeftSize;
	absrect.d_top		+= d_frameTopSize;
	absrect.d_right		-= d_frameRightSize;
	absrect.d_bottom	-= d_frameBottomSize;

	// draw background image
	d_background->draw(absrect, pos.d_z, clipper, colours);

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
	text_rect.d_top  += PixelAligned((text_rect.getHeight() - getFont()->getBaseline()) * 0.5f);
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


/*************************************************************************
	Store sizes of frame edges
*************************************************************************/
void WLEditbox::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= iset->getImageWidth(LeftFrameImageName);
	d_frameRightSize	= iset->getImageWidth(RightFrameImageName);
	d_frameTopSize		= iset->getImageHeight(TopFrameImageName);
	d_frameBottomSize	= iset->getImageHeight(BottomFrameImageName);
}


/*************************************************************************
	Handler for when button size is changed
*************************************************************************/
void WLEditbox::onSized(WindowEventArgs& e)
{
	// default processing
	Editbox::onSized(e);

	// update frame size.
	d_frame.setSize(getAbsoluteSize());

	e.handled = true;
}


/*************************************************************************
	Handler for when alpha changes.
*************************************************************************/
void WLEditbox::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing.
	Editbox::onAlphaChanged(e);

	ColourRect cols(d_frame.getColours());
	cols.setAlpha(getEffectiveAlpha());
	d_frame.setColours(cols);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLEditbox
*************************************************************************/
Window* WLEditboxFactory::createWindow(const String& name)
{
	WLEditbox* wnd = new WLEditbox(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
