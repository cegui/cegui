/************************************************************************
	filename: 	WLMultiColumnList.cpp
	created:	16/8/2004
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
#include "WLMultiColumnList.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "CEGUIFont.h"
#include "elements/CEGUIScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// image / imageset related
const utf8	WLMultiColumnList::ImagesetName[]				= "WindowsLook";
const utf8	WLMultiColumnList::TopLeftImageName[]			= "StaticFrameTopLeft";
const utf8	WLMultiColumnList::TopRightImageName[]			= "StaticFrameTopRight";
const utf8	WLMultiColumnList::BottomLeftImageName[]		= "StaticFrameBottomLeft";
const utf8	WLMultiColumnList::BottomRightImageName[]		= "StaticFrameBottomRight";
const utf8	WLMultiColumnList::LeftEdgeImageName[]			= "StaticFrameLeft";
const utf8	WLMultiColumnList::RightEdgeImageName[]			= "StaticFrameRight";
const utf8	WLMultiColumnList::TopEdgeImageName[]			= "StaticFrameTop";
const utf8	WLMultiColumnList::BottomEdgeImageName[]		= "StaticFrameBottom";
const utf8	WLMultiColumnList::BackgroundImageName[]		= "Background";
const utf8	WLMultiColumnList::SelectionBrushImageName[]	= "Background";
const utf8	WLMultiColumnList::MouseCursorImageName[]		= "MouseArrow";

// component widget type names
const utf8	WLMultiColumnList::HorzScrollbarTypeName[]		= "WindowsLook/HorizontalScrollbar";
const utf8	WLMultiColumnList::VertScrollbarTypeName[]		= "WindowsLook/VerticalScrollbar";
const utf8	WLMultiColumnList::ListHeaderTypeName[]			= "WindowsLook/ListHeader";

	
/*************************************************************************
	Constructor for WindowsLook multi-column list objects.
*************************************************************************/
WLMultiColumnList::WLMultiColumnList(const String& type, const String& name) :
	MultiColumnList(type, name)
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
	Destructor for WLMultiColumnList objects.
*************************************************************************/
WLMultiColumnList::~WLMultiColumnList(void)
{
}


/*************************************************************************
	Return a Rect object describing, in un-clipped pixels, the window
	relative area that is to be used for rendering list items.
*************************************************************************/
Rect WLMultiColumnList::getListRenderArea(void) const
{
	Rect tmp;

	tmp.d_left	= d_frameLeftSize;
	tmp.d_top	= d_frameTopSize + d_header->getAbsoluteHeight();
	tmp.setSize(Size(getAbsoluteWidth() - d_frameLeftSize, getAbsoluteHeight() - d_frameTopSize - d_header->getAbsoluteHeight()));

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
	create and return a pointer to a ListHeaer widget for use as the
	column headers.	
*************************************************************************/
ListHeader*	WLMultiColumnList::createListHeader(void) const
{
	ListHeader* hdr = (ListHeader*)WindowManager::getSingleton().createWindow(ListHeaderTypeName, getName() + "__auto_listheader__");

	// set min/max sizes
	hdr->setMinimumSize(Size(0.0f, 0.014f));
	hdr->setMaximumSize(Size(1.0f, 1.0f));

	return hdr;
}


/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	vertical scroll bar	
*************************************************************************/
Scrollbar* WLMultiColumnList::createVertScrollbar(void) const
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
Scrollbar* WLMultiColumnList::createHorzScrollbar(void) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(HorzScrollbarTypeName, getName() + "__auto_hscrollbar__");

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0f, 0.016667f));
	sbar->setMaximumSize(Size(1.0f, 0.016667f));

	return sbar;
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	MultiColumnList
*************************************************************************/
void WLMultiColumnList::layoutComponentWidgets()
{
	Rect listArea(getListRenderArea());

	// set position for header (fixed)
	Point hdr_pos(absoluteToRelative(Point(d_frameLeftSize, d_frameTopSize)));
	d_header->setPosition(hdr_pos);

	// set size for header
	Size hdr_sz(absoluteToRelative(Size(getAbsoluteWidth() - d_frameLeftSize - d_frameRightSize, getFont()->getLineSpacing() * 1.5f)));
	d_header->setSize(hdr_sz);

	// get actual size used for header
	hdr_sz = absoluteToRelative(d_header->getAbsoluteSize());

	// set desired size for vertical scroll-bar
	Size v_sz(0.05f, 1.0f - hdr_sz.d_height - hdr_pos.d_y);
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
	d_vertScrollbar->setPosition(Point(1.0f - v_sz.d_width, hdr_sz.d_height + hdr_pos.d_y));

	// position horizontal scroll bar
	d_horzScrollbar->setPosition(Point(0.0f, 1.0f - h_sz.d_height));
}


/*************************************************************************
	Render static imagery 
*************************************************************************/
void WLMultiColumnList::renderListboxBaseImagery(float z)
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
void WLMultiColumnList::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_frameLeftSize		= 2;//iset->getImage(LeftEdgeImageName).getWidth();
	d_frameRightSize	= 2;//iset->getImage(RightEdgeImageName).getWidth();
	d_frameTopSize		= 2;//iset->getImage(TopEdgeImageName).getHeight();
	d_frameBottomSize	= 2;//iset->getImage(BottomEdgeImageName).getHeight();
}


/*************************************************************************
	Handler for when window is sized
*************************************************************************/
void WLMultiColumnList::onSized(WindowEventArgs& e)
{
	// base class processing
	MultiColumnList::onSized(e);

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
void WLMultiColumnList::onAlphaChanged(WindowEventArgs& e)
{
	MultiColumnList::onAlphaChanged(e);

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
	Create, initialise and return a WLMultiColumnList
*************************************************************************/
Window* WLMultiColumnListFactory::createWindow(const String& name)
{
	WLMultiColumnList* wnd = new WLMultiColumnList(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
