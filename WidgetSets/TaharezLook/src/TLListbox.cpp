/************************************************************************
	filename: 	TLListbox.cpp
	created:	7/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Listbox widget
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
#include "TLListbox.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "TLMiniHorzScrollbar.h"
#include "TLMiniVertScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	TLListbox::WidgetTypeName[]		= "TaharezLook/Listbox";

// image / imageset related
const utf8	TLListbox::ImagesetName[]				= "TaharezLook";
const utf8	TLListbox::TopLeftImageName[]			= "ListboxTopLeft";
const utf8	TLListbox::TopRightImageName[]			= "ListboxTopRight";
const utf8	TLListbox::BottomLeftImageName[]		= "ListboxBottomLeft";
const utf8	TLListbox::BottomRightImageName[]		= "ListboxBottomRight";
const utf8	TLListbox::LeftEdgeImageName[]			= "ListboxLeft";
const utf8	TLListbox::RightEdgeImageName[]			= "ListboxRight";
const utf8	TLListbox::TopEdgeImageName[]			= "ListboxTop";
const utf8	TLListbox::BottomEdgeImageName[]		= "ListboxBottom";
const utf8	TLListbox::BackgroundImageName[]		= "ListboxBackdrop";
const utf8	TLListbox::SelectionBrushImageName[]	= "ListboxSelectionBrush";
const utf8	TLListbox::MouseCursorImageName[]		= "MouseTarget";

// component widget type names
const utf8*	TLListbox::HorzScrollbarTypeName		= TLMiniHorzScrollbar::WidgetTypeName;
const utf8*	TLListbox::VertScrollbarTypeName		= TLMiniVertScrollbar::WidgetTypeName;


/*************************************************************************
	Constructor for Taharez Look Listbox widgets	
*************************************************************************/
TLListbox::TLListbox(const String& type, const String& name) :
	Listbox(type, name)
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

	// set cursor for this window.
	setMouseCursor(&iset->getImage(MouseCursorImageName));
}


/*************************************************************************
	Destructor for Taharez Look Listbox widgets.	
*************************************************************************/
TLListbox::~TLListbox(void)
{
}


/*************************************************************************
	Return a Rect object describing, in un-clipped pixels, the window
	relative area that is to be used for rendering list items.
*************************************************************************/
Rect TLListbox::getListRenderArea(void) const
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
Scrollbar* TLListbox::createVertScrollbar(void) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(VertScrollbarTypeName, getName() + "__auto_vscrollbar__");

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0125f, 0.0f));
	sbar->setMaximumSize(Size(0.0125f, 1.0f));

	return sbar;
}


/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	horizontal scroll bar
*************************************************************************/
Scrollbar* TLListbox::createHorzScrollbar(void) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(HorzScrollbarTypeName, getName() + "__auto_hscrollbar__");

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0f, 0.016667f));
	sbar->setMaximumSize(Size(1.0f, 0.016667f));

	return sbar;
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	Listbox	
*************************************************************************/
void TLListbox::layoutComponentWidgets()
{
	// set desired size for vertical scroll-bar
	Size v_sz(0.05f, 1.0f);
	d_vertScrollbar->setSize(v_sz);

	// get the actual size used for vertical scroll bar.
	v_sz = absoluteToRelative(d_vertScrollbar->getAbsoluteSize());


	// set desired size for horizontal scroll-bar
	Size h_sz(1.0f, 0.0f);

	if (d_abs_area.getHeight() != 0.0f)
	{
		h_sz.d_height = (d_abs_area.getWidth() * v_sz.d_width) / d_abs_area.getHeight();
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
	Perform the rendering for everything except the items
*************************************************************************/
void TLListbox::renderListboxBaseImagery(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// draw the box elements
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	d_background.draw(pos, clipper);
	d_frame.draw(pos, clipper);
}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void TLListbox::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= iset->getImage(LeftEdgeImageName).getWidth();
	d_frameRightSize	= iset->getImage(RightEdgeImageName).getWidth();
	d_frameTopSize		= iset->getImage(TopEdgeImageName).getHeight();
	d_frameBottomSize	= iset->getImage(BottomEdgeImageName).getHeight();
}


/*************************************************************************
	Handler for when window is sized
*************************************************************************/
void TLListbox::onSized(WindowEventArgs& e)
{
	// base class processing
	Listbox::onSized(e);

	// update size of frame
	Size newsize(getAbsoluteSize());
	d_frame.setSize(newsize);

	// update size of background image
	newsize.d_width		-= (d_frameLeftSize + d_frameRightSize);
	newsize.d_height	-= (d_frameTopSize + d_frameBottomSize);

	d_background.setSize(newsize);
}


/*************************************************************************
	Handler for alpha value changes
*************************************************************************/
void TLListbox::onAlphaChanged(WindowEventArgs& e)
{
	Listbox::onAlphaChanged(e);

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
	Create, initialise and return a TLListbox
*************************************************************************/
Window* TLListboxFactory::createWindow(const String& name)
{
	TLListbox* wnd = new TLListbox(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
