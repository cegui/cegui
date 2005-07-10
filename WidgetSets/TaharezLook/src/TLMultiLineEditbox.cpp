/************************************************************************
	filename: 	TLMultiLineEditbox.cpp
	created:	30/6/2004
	author:		Paul D Turner
	
	purpose:	Implements the Taharez look multi-line edit box widget.
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
#include "TLMultiLineEditbox.h"
#include "CEGUIWindowManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "TLMiniHorzScrollbar.h"
#include "TLMiniVertScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLMultiLineEditbox::WidgetTypeName[]	= "TaharezLook/MultiLineEditbox";

// image / imageset related
const utf8	TLMultiLineEditbox::ImagesetName[]				= "TaharezLook";
const utf8	TLMultiLineEditbox::TopLeftImageName[]			= "MultiLineEditboxTopLeft";
const utf8	TLMultiLineEditbox::TopRightImageName[]			= "MultiLineEditboxTopRight";
const utf8	TLMultiLineEditbox::BottomLeftImageName[]		= "MultiLineEditboxBottomLeft";
const utf8	TLMultiLineEditbox::BottomRightImageName[]		= "MultiLineEditboxBottomRight";
const utf8	TLMultiLineEditbox::LeftEdgeImageName[]			= "MultiLineEditboxLeft";
const utf8	TLMultiLineEditbox::RightEdgeImageName[]		= "MultiLineEditboxRight";
const utf8	TLMultiLineEditbox::TopEdgeImageName[]			= "MultiLineEditboxTop";
const utf8	TLMultiLineEditbox::BottomEdgeImageName[]		= "MultiLineEditboxBottom";
const utf8	TLMultiLineEditbox::BackgroundImageName[]		= "MultiLineEditboxBackdrop";
const utf8	TLMultiLineEditbox::CaratImageName[]			= "EditBoxCarat";
const utf8	TLMultiLineEditbox::SelectionBrushImageName[]	= "MultiLineEditboxSelectionBrush";
const utf8	TLMultiLineEditbox::MouseCursorImageName[]		= "MouseTextBar";

// component widget type names
const utf8*	TLMultiLineEditbox::HorzScrollbarTypeName		= TLMiniHorzScrollbar::WidgetTypeName;
const utf8*	TLMultiLineEditbox::VertScrollbarTypeName		= TLMiniVertScrollbar::WidgetTypeName;


/*************************************************************************
	Constructor for the TLMultiLineEditbox class.
*************************************************************************/
TLMultiLineEditbox::TLMultiLineEditbox(const String& type, const String& name) :
	MultiLineEditbox(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	storeFrameSizes();

	// setup frame images
	d_frame.setImages(
		&iset->getImage(TopLeftImageName),		&iset->getImage(TopRightImageName),
		&iset->getImage(BottomLeftImageName),	&iset->getImage(BottomRightImageName),
		&iset->getImage(LeftEdgeImageName),		&iset->getImage(TopEdgeImageName), 
		&iset->getImage(RightEdgeImageName),	&iset->getImage(BottomEdgeImageName)
		);

	// setup background brush
	d_background.setImage(&iset->getImage(BackgroundImageName));
	d_background.setPosition(Point(d_frameLeftSize, d_frameTopSize));
	d_background.setHorzFormatting(RenderableImage::HorzStretched);
	d_background.setVertFormatting(RenderableImage::VertStretched);

	// set selection brush
	d_selectionBrush = &iset->getImage(SelectionBrushImageName);

	// set carat image
	d_carat	= &iset->getImage(CaratImageName);

	// set cursor for this window.
	setMouseCursor(&iset->getImage(MouseCursorImageName));
}


/*************************************************************************
	Destructor for the TLMultiLineEditbox class.
*************************************************************************/
TLMultiLineEditbox::~TLMultiLineEditbox(void)
{
}


/*************************************************************************
	Return a Rect object describing, in un-clipped pixels, the window
	relative area that the text should be rendered in to.	
*************************************************************************/
Rect TLMultiLineEditbox::getTextRenderArea(void) const
{
	Rect tmp;

	tmp.d_left	= d_frameLeftSize;
	tmp.d_top	= d_frameTopSize;
	tmp.setSize(Size(getAbsoluteWidth() - d_frameLeftSize, getAbsoluteHeight() - d_frameTopSize));

	if (d_vertScrollbar->isVisible())
	{
		tmp.d_right -= d_vertScrollbar->getAbsoluteWidth();
	}
	else
	{
		tmp.d_right -= d_frameRightSize;
	}

	if (d_horzScrollbar->isVisible())
	{
		tmp.d_bottom -= d_horzScrollbar->getAbsoluteHeight();
	}
	else
	{
		tmp.d_bottom -= d_frameBottomSize;
	}

	return tmp;
}


/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as vertical
	scroll bar	
*************************************************************************/
Scrollbar* TLMultiLineEditbox::createVertScrollbar(const String& name) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(VertScrollbarTypeName, name);

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0125f, 0.0f));
	sbar->setMaximumSize(Size(0.0125f, 1.0f));

	return sbar;
}


/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	horizontal scroll bar	
*************************************************************************/
Scrollbar* TLMultiLineEditbox::createHorzScrollbar(const String& name) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(HorzScrollbarTypeName, name);

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0f, 0.016667f));
	sbar->setMaximumSize(Size(1.0f, 0.016667f));

	return sbar;
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	Editbox	
*************************************************************************/
void TLMultiLineEditbox::layoutComponentWidgets()
{
	// set desired size for vertical scroll-bar
	Size v_sz(0.05f, 1.0f);
	d_vertScrollbar->setSize(v_sz);

	// get the actual size used for vertical scroll bar.
	v_sz = absoluteToRelative(d_vertScrollbar->getAbsoluteSize());


	// set desired size for horizontal scroll-bar
	Size h_sz(1.0f, 0.0f);

	if (getAbsoluteHeight() != 0.0f)
	{
		h_sz.d_height = (getAbsoluteWidth() * v_sz.d_width) / getAbsoluteHeight();
	}

	// adjust length to consider width of vertical scroll bar if that is visible
	if (d_vertScrollbar->isVisible())
	{
		h_sz.d_width -= v_sz.d_width;
	}

	d_horzScrollbar->setSize(h_sz);

	// get actual size used
	h_sz = absoluteToRelative(d_horzScrollbar->getAbsoluteSize());


	// position vertical scroll bar
	d_vertScrollbar->setPosition(Point(1.0f - v_sz.d_width, 0.0f));

	// position horizontal scroll bar
	d_horzScrollbar->setPosition(Point(0.0f, 1.0f - h_sz.d_height));
}


/*************************************************************************
	Perform rendering of the widget control frame and other 'static' areas.	
*************************************************************************/
void TLMultiLineEditbox::cacheEditboxBaseImagery()
{
	// draw the box elements
	d_background.draw(d_renderCache);
	d_frame.draw(d_renderCache);
}


/*************************************************************************
	Render the carat.	
*************************************************************************/
void TLMultiLineEditbox::cacheCaratImagery(const Rect& textArea)
{
    const Font* fnt = getFont();

    // require a font so that we can calculate carat position.
    if (fnt)
    {
        // get line that carat is in
        size_t caratLine = getLineNumberFromIndex(d_caratPos);

        // if carat line is valid.
        if (caratLine < d_lines.size())
        {
            // calculate pixel offsets to where carat should be drawn
            size_t caratLineIdx = d_caratPos - d_lines[caratLine].d_startIdx;
            float ypos = caratLine * fnt->getLineSpacing();
            float xpos = fnt->getTextExtent(d_text.substr(d_lines[caratLine].d_startIdx, caratLineIdx));

            // get base offset to target layer for cursor.
            Renderer* renderer = System::getSingleton().getRenderer();
            float baseZ = renderer->getZLayer(7) - renderer->getCurrentZ();

            // calculate finat destination area for carat
            Rect caratArea;
            caratArea.d_left    = textArea.d_left + xpos;
            caratArea.d_top     = textArea.d_top + ypos;
            caratArea.d_right   = caratArea.d_left + d_carat->getWidth();
            caratArea.d_bottom  = caratArea.d_top + fnt->getLineSpacing();

            // cache the carat image for rendering.
            d_renderCache.cacheImage(*d_carat, caratArea, baseZ, ColourRect(colour(1,1,1,getEffectiveAlpha())), &textArea);
        }
    }
}


/*************************************************************************
	Store the sizes for the frame edges	
*************************************************************************/
void TLMultiLineEditbox::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= iset->getImage(LeftEdgeImageName).getWidth();
	d_frameRightSize	= iset->getImage(RightEdgeImageName).getWidth();
	d_frameTopSize		= iset->getImage(TopEdgeImageName).getHeight();
	d_frameBottomSize	= iset->getImage(BottomEdgeImageName).getHeight();
}


/*************************************************************************
	Handler for when the window is sized.
*************************************************************************/
void TLMultiLineEditbox::onSized(WindowEventArgs& e)
{
	// base class processing
	MultiLineEditbox::onSized(e);

	// update size of frame
	Size newsize(getAbsoluteSize());
	d_frame.setSize(newsize);

	// update size of background image
	newsize.d_width		-= (d_frameLeftSize + d_frameRightSize);
	newsize.d_height	-= (d_frameTopSize + d_frameBottomSize);

	d_background.setSize(newsize);
}


/*************************************************************************
	Handler for when the alpha for the window changes.
*************************************************************************/
void TLMultiLineEditbox::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing
	MultiLineEditbox::onAlphaChanged(e);

	// update alpha values for the frame and background brush
	float alpha = getEffectiveAlpha();

	ColourRect cr;
	cr = d_frame.getColours();
	cr.setAlpha(alpha);
	d_frame.setColours(cr);

	cr = d_background.getColours();
	cr.setAlpha(alpha);
	d_background.setColours(cr);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLMultiLineEditbox
*************************************************************************/
Window* TLMultiLineEditboxFactory::createWindow(const String& name)
{
	return new TLMultiLineEditbox(d_type, name);
}

} // End of  CEGUI namespace section
