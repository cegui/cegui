/************************************************************************
	filename: 	WLMultiLineEditbox.cpp
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
#include "WLMultiLineEditbox.h"
#include "CEGUIWindowManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "WLHorzScrollbar.h"
#include "WLVertScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLMultiLineEditbox::WidgetTypeName[]			= "WindowsLook/MultiLineEditbox";

// image / imageset related
const utf8	WLMultiLineEditbox::ImagesetName[]				= "WindowsLook";
const utf8	WLMultiLineEditbox::TopLeftImageName[]			= "EditFrameTopLeft";
const utf8	WLMultiLineEditbox::TopRightImageName[]			= "EditFrameTopRight";
const utf8	WLMultiLineEditbox::BottomLeftImageName[]		= "EditFrameBottomLeft";
const utf8	WLMultiLineEditbox::BottomRightImageName[]		= "EditFrameBottomRight";
const utf8	WLMultiLineEditbox::LeftEdgeImageName[]			= "EditFrameLeft";
const utf8	WLMultiLineEditbox::RightEdgeImageName[]		= "EditFrameRight";
const utf8	WLMultiLineEditbox::TopEdgeImageName[]			= "EditFrameTop";
const utf8	WLMultiLineEditbox::BottomEdgeImageName[]		= "EditFrameBottom";
const utf8	WLMultiLineEditbox::BackgroundImageName[]		= "Background";
const utf8	WLMultiLineEditbox::CaratImageName[]			= "EditBoxCarat";
const utf8	WLMultiLineEditbox::SelectionBrushImageName[]	= "Background";
const utf8	WLMultiLineEditbox::MouseCursorImageName[]		= "MouseTextBar";

// component widget type names
const utf8*	WLMultiLineEditbox::HorzScrollbarTypeName		= WLHorzScrollbar::WidgetTypeName;
const utf8*	WLMultiLineEditbox::VertScrollbarTypeName		= WLVertScrollbar::WidgetTypeName;

// colours
const colour WLMultiLineEditbox::ReadWriteBackgroundColour	= 0xFFFFFF;
const colour WLMultiLineEditbox::ReadOnlyBackgroundColour	= 0xDFDFDF;
const colour WLMultiLineEditbox::NormalTextColour			= 0xFF000000;
const colour WLMultiLineEditbox::SelectedTextColour			= 0xFFFFFFFF;
const colour WLMultiLineEditbox::NormalSelectionColour		= 0xFF607FFF;
const colour WLMultiLineEditbox::InactiveSelectionColour	= 0xFF808080;


/*************************************************************************
	Constructor for the WLMultiLineEditbox class.
*************************************************************************/
WLMultiLineEditbox::WLMultiLineEditbox(const String& type, const String& name) :
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
	d_background = &iset->getImage(BackgroundImageName);

	// set selection brush
	d_selectionBrush = &iset->getImage(SelectionBrushImageName);

	// set carat image
	d_carat	= &iset->getImage(CaratImageName);

	// set cursor for this window.
	setMouseCursor(&iset->getImage(MouseCursorImageName));

	// set up colours
	d_normalTextColour	= NormalTextColour;
	d_selectTextColour	= SelectedTextColour;
	d_selectBrushColour	= NormalSelectionColour;
	d_inactiveSelectBrushColour	= InactiveSelectionColour;
}


/*************************************************************************
	Destructor for the WLMultiLineEditbox class.
*************************************************************************/
WLMultiLineEditbox::~WLMultiLineEditbox(void)
{
}


/*************************************************************************
	Return a Rect object describing, in un-clipped pixels, the window
	relative area that the text should be rendered in to.	
*************************************************************************/
Rect WLMultiLineEditbox::getTextRenderArea(void) const
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
Scrollbar* WLMultiLineEditbox::createVertScrollbar(void) const
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
Scrollbar* WLMultiLineEditbox::createHorzScrollbar(void) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(HorzScrollbarTypeName, getName() + "__auto_hscrollbar__");

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0f, 0.016667f));
	sbar->setMaximumSize(Size(1.0f, 0.016667f));

	return sbar;
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	Editbox	
*************************************************************************/
void WLMultiLineEditbox::layoutComponentWidgets()
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
	Perform rendering of the widget control frame and other 'static' areas.	
*************************************************************************/
void WLMultiLineEditbox::renderEditboxBaseImagery(float z)
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
	ColourRect colours;

	if (isReadOnly())
	{
		colours.setColours(ReadOnlyBackgroundColour);
	}
	else
	{
		colours.setColours(ReadWriteBackgroundColour);
	}
	
	colours.setAlpha(getEffectiveAlpha());

	//
	// render container
	//
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	d_frame.draw(pos, clipper);

	// calculate inner area rect considering frame
	absrect.d_left		+= d_frameLeftSize;
	absrect.d_top		+= d_frameTopSize;
	absrect.d_right		-= d_frameRightSize;
	absrect.d_bottom	-= d_frameBottomSize;

	// draw background image
	d_background->draw(absrect, z, clipper, colours);
}


/*************************************************************************
	Render the carat.	
*************************************************************************/
void WLMultiLineEditbox::renderCarat(float baseX, float baseY, const Rect& clipper)
{
	// calculate position of carat
	const Font* fnt = getFont();

	uint indexCount = 0;
	uint caratLine = getLineNumberFromIndex(d_caratPos);

	if (caratLine < (uint)d_lines.size())
	{
		uint caratLineIdx = d_caratPos - d_lines[caratLine].d_startIdx;

		float ypos = caratLine * fnt->getLineSpacing();
		float xpos = fnt->getTextExtent(d_text.substr(d_lines[caratLine].d_startIdx, caratLineIdx));

		colour col(1, 1, 1, getEffectiveAlpha());
		d_carat->draw(Vector3(baseX + xpos, baseY + ypos, System::getSingleton().getRenderer()->getZLayer(7)), Size(d_carat->getWidth(), fnt->getLineSpacing()), clipper, ColourRect(col));
	}

}


/*************************************************************************
	Store the sizes for the frame edges	
*************************************************************************/
void WLMultiLineEditbox::storeFrameSizes(void)
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
void WLMultiLineEditbox::onSized(WindowEventArgs& e)
{
	// base class processing
	MultiLineEditbox::onSized(e);

	// update size of frame
	Size newsize(getAbsoluteSize());
	d_frame.setSize(newsize);
}


/*************************************************************************
	Handler for when the alpha for the window changes.
*************************************************************************/
void WLMultiLineEditbox::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing
	MultiLineEditbox::onAlphaChanged(e);

	// update alpha values for the frame and background brush
	ColourRect cr;

	cr = d_frame.getColours();
	cr.setAlpha(getEffectiveAlpha());
	d_frame.setColours(cr);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLMultiLineEditbox
*************************************************************************/
Window* WLMultiLineEditboxFactory::createWindow(const String& name)
{
	WLMultiLineEditbox* wnd = new WLMultiLineEditbox(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
