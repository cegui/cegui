/************************************************************************
	filename: 	CEGUIStaticText.cpp
	created:	4/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of the static text widget class
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
#include "elements/CEGUIStaticText.h"
#include "CEGUIFont.h"
#include "elements/CEGUIScrollbar.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
Static Properties for this class
*************************************************************************/
StaticTextProperties::TextColours		StaticText::d_textColoursProperty;
StaticTextProperties::VertFormatting	StaticText::d_vertFormattingProperty;
StaticTextProperties::HorzFormatting	StaticText::d_horzFormattingProperty;
StaticTextProperties::VertScrollbar		StaticText::d_vertScrollbarProperty;
StaticTextProperties::HorzScrollbar		StaticText::d_horzScrollbarProperty;


/*************************************************************************
	Constructor for static text widgets.	
*************************************************************************/
StaticText::StaticText(const String& type, const String& name) :
	Static(type, name),
	d_textCols(0xFFFFFFFF),
	d_horzFormatting(LeftAligned),
	d_vertFormatting(VertCentred),
	d_enableVertScrollbar(false),
	d_enableHorzScrollbar(false)
{
	addStaticTextProperties();
}


/*************************************************************************
	Destructor for static text widgets.
*************************************************************************/
StaticText::~StaticText(void)
{
}


/*************************************************************************
	Sets the colours to be applied when rendering the text.	
*************************************************************************/
void StaticText::setTextColours(const ColourRect& colours)
{
	d_textCols = colours;
	requestRedraw();
}


/*************************************************************************
	Sets the colours to be applied when rendering the text.	
*************************************************************************/
void StaticText::setTextColours(const colour& top_left_colour, const colour& top_right_colour, const colour& bottom_left_colour, const colour& bottom_right_colour)
{
	d_textCols.d_top_left		= top_left_colour;
	d_textCols.d_top_right		= top_right_colour;
	d_textCols.d_bottom_left	= bottom_left_colour;
	d_textCols.d_bottom_right	= bottom_right_colour;
	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the text.
*************************************************************************/
void StaticText::setFormatting(HorzFormatting h_fmt, VertFormatting v_fmt)
{
	d_horzFormatting = h_fmt;
	d_vertFormatting = v_fmt;
	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the text.	
*************************************************************************/
void StaticText::setVerticalFormatting(VertFormatting v_fmt)
{
	d_vertFormatting = v_fmt;
	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the text.	
*************************************************************************/
void StaticText::setHorizontalFormatting(HorzFormatting h_fmt)
{
	d_horzFormatting = h_fmt;
	requestRedraw();
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void StaticText::drawSelf(float z)
{
	// get whatever base class needs to render.
	Static::drawSelf(z);

	// render text.
	Rect absarea(getTextRenderArea());
	Rect clipper(absarea.getIntersection(getPixelRect()));

	const Font* font = getFont();

	float textHeight = font->getFormattedLineCount(d_text, absarea, (TextFormatting)d_horzFormatting) * font->getLineSpacing();

	// see if we may need to adjust horizontal position
	if (d_horzScrollbar->isVisible())
	{
		switch(d_horzFormatting)
		{
		case LeftAligned:
		case WordWrapLeftAligned:
			absarea.offset(Point(-d_horzScrollbar->getScrollPosition(), 0));
			break;

		case Centred:
		case WordWrapCentred:
			absarea.setWidth(d_horzScrollbar->getDocumentSize());
			absarea.offset(Point(-d_horzScrollbar->getScrollPosition(), 0));
			break;

		case RightAligned:
		case WordWrapRightAligned:
			absarea.offset(Point(d_horzScrollbar->getScrollPosition(), 0));
			break;
		}

	}

	// adjust y positioning according to formatting option
	switch(d_vertFormatting)
	{
	case TopAligned:
		absarea.d_top -= d_vertScrollbar->getScrollPosition();
		break;

	case VertCentred:
		// if scroll bar is in use, act like TopAligned
		if (d_vertScrollbar->isVisible())
		{
			absarea.d_top -= d_vertScrollbar->getScrollPosition();
		}
		// no scroll bar, so centre text instead.
		else
		{
			absarea.d_top += PixelAligned((absarea.getHeight() - textHeight) * 0.5f);
		}

		break;

	case BottomAligned:
		absarea.d_top = absarea.d_bottom - textHeight;
		absarea.d_top += d_vertScrollbar->getScrollPosition();
		break;
	}

	float alpha = getEffectiveAlpha();
	ColourRect final_cols(
		calculateModulatedAlphaColour(d_textCols.d_top_left, alpha),
		calculateModulatedAlphaColour(d_textCols.d_top_right, alpha),
		calculateModulatedAlphaColour(d_textCols.d_bottom_left, alpha),
		calculateModulatedAlphaColour(d_textCols.d_bottom_right, alpha)
		);

	getFont()->drawText(getText(), absarea, System::getSingleton().getRenderer()->getZLayer(1), clipper, (TextFormatting)d_horzFormatting, final_cols);
}


/*************************************************************************
	Add properties for static text
*************************************************************************/
void StaticText::addStaticTextProperties(void)
{
	addProperty(&d_textColoursProperty);
	addProperty(&d_vertFormattingProperty);
	addProperty(&d_horzFormattingProperty);
	addProperty(&d_vertScrollbarProperty);
	addProperty(&d_horzScrollbarProperty);
}


/*************************************************************************
	Perform initialisation for the widget.
*************************************************************************/
void StaticText::initialise(void)
{
	Static::initialise();

	// create the component sub-widgets
	d_vertScrollbar = createVertScrollbar();
	d_horzScrollbar = createHorzScrollbar();

	d_vertScrollbar->hide();
	d_horzScrollbar->hide();

	addChildWindow(d_vertScrollbar);
	addChildWindow(d_horzScrollbar);

	layoutComponentWidgets();

	// event subscription
	d_vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&StaticText::handleScrollbarChange, this));
	d_horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&StaticText::handleScrollbarChange, this));
}


/*************************************************************************
	Return a Rect object describing, in un-clipped pixels, the window
	relative area that the text should be rendered in to.
*************************************************************************/
Rect StaticText::getTextRenderArea(void) const
{
	Rect area(getUnclippedInnerRect());

	if (d_horzScrollbar->isVisible())
	{
		area.d_bottom -= d_horzScrollbar->getAbsoluteHeight();
	}
	else if (d_frameEnabled)
	{
		area.d_bottom -= d_bottom_height;
	}

	if (d_vertScrollbar->isVisible())
	{
		area.d_right -= d_vertScrollbar->getAbsoluteWidth();
	}
	else if (d_frameEnabled)
	{
		area.d_right -= d_right_width;
	}

	if (d_frameEnabled)
	{
		area.d_left	+= d_left_width;
		area.d_top	+= d_top_height;
	}

	return area;
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	StaticText
*************************************************************************/
void StaticText::layoutComponentWidgets()
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
	display required integrated scroll bars according to current state
	of the edit box and update their values.
*************************************************************************/
void StaticText::configureScrollbars(void)
{
	const Font* font = getFont();

	Rect initialArea(getTextRenderArea());

	float totalHeight	= font->getFormattedLineCount(d_text, initialArea, (TextFormatting)d_horzFormatting) * font->getLineSpacing();
	float widestItem	= font->getFormattedTextExtent(d_text, initialArea, (TextFormatting)d_horzFormatting);

	//
	// First show or hide the scroll bars as needed (or requested)
	//
	// show or hide vertical scroll bar as required (or as specified by option)
	if ((totalHeight > getTextRenderArea().getHeight()) && d_enableVertScrollbar)
	{
		d_vertScrollbar->show();

		// show or hide horizontal scroll bar as required (or as specified by option)
		if ((widestItem > getTextRenderArea().getWidth()) && d_enableHorzScrollbar)
		{
			d_horzScrollbar->show();
		}
		else
		{
			d_horzScrollbar->hide();
		}

	}
	else
	{
		// show or hide horizontal scroll bar as required (or as specified by option)
		if ((widestItem > getTextRenderArea().getWidth()) && d_enableHorzScrollbar)
		{
			d_horzScrollbar->show();

			// show or hide vertical scroll bar as required (or as specified by option)
			if ((totalHeight > getTextRenderArea().getHeight()) && d_enableVertScrollbar)
			{
				d_vertScrollbar->show();
			}
			else
			{
				d_vertScrollbar->hide();
			}

		}
		else
		{
			d_vertScrollbar->hide();
			d_horzScrollbar->hide();
		}

	}

	//
	// Set up scroll bar values
	//
	Rect renderArea(getTextRenderArea());

	d_vertScrollbar->setDocumentSize(totalHeight);
	d_vertScrollbar->setPageSize(renderArea.getHeight());
	d_vertScrollbar->setStepSize(ceguimax(1.0f, renderArea.getHeight() / 10.0f));
	d_vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition());

	d_horzScrollbar->setDocumentSize(widestItem);
	d_horzScrollbar->setPageSize(renderArea.getWidth());
	d_horzScrollbar->setStepSize(ceguimax(1.0f, renderArea.getWidth() / 10.0f));
	d_horzScrollbar->setScrollPosition(d_horzScrollbar->getScrollPosition());
}


/*************************************************************************
	Return whether the vertical scroll bar is set to be shown if needed.	
*************************************************************************/
bool StaticText::isVerticalScrollbarEnabled(void) const
{
	return d_enableVertScrollbar;
}


/*************************************************************************
	Return whether the horizontal scroll bar is set to be shown if needed.	
*************************************************************************/
bool StaticText::isHorizontalScrollbarEnabled(void) const
{
	return d_enableHorzScrollbar;
}


/*************************************************************************
	Set whether the vertical scroll bar will be shown if needed.	
*************************************************************************/
void StaticText::setVerticalScrollbarEnabled(bool setting)
{
	d_enableVertScrollbar = setting;
	configureScrollbars();
	layoutComponentWidgets();
}


/*************************************************************************
	Set whether the horizontal scroll bar will be shown if needed.	
*************************************************************************/
void StaticText::setHorizontalScrollbarEnabled(bool setting)
{
	d_enableHorzScrollbar = setting;
	configureScrollbars();
	layoutComponentWidgets();
}


/*************************************************************************
	Handler called when text is changed.
*************************************************************************/
void StaticText::onTextChanged(WindowEventArgs& e)
{
	Static::onTextChanged(e);

	configureScrollbars();
	requestRedraw();
}


/*************************************************************************
	Handler called when size is changed
*************************************************************************/
void StaticText::onSized(WindowEventArgs& e)
{
	Static::onSized(e);

	layoutComponentWidgets();
	configureScrollbars();
	requestRedraw();
}


/*************************************************************************
	Handler called when font is changed.
*************************************************************************/
void StaticText::onFontChanged(WindowEventArgs& e)
{
	Static::onFontChanged(e);

	configureScrollbars();
	requestRedraw();
}


/*************************************************************************
	Handler for mouse wheel changes
*************************************************************************/
void StaticText::onMouseWheel(MouseEventArgs& e)
{
	// base class processing.
	Static::onMouseWheel(e);

	if (d_vertScrollbar->isVisible() && (d_vertScrollbar->getDocumentSize() > d_vertScrollbar->getPageSize()))
	{
		d_vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition() + d_vertScrollbar->getStepSize() * -e.wheelChange);
	}
	else if (d_horzScrollbar->isVisible() && (d_horzScrollbar->getDocumentSize() > d_horzScrollbar->getPageSize()))
	{
		d_horzScrollbar->setScrollPosition(d_horzScrollbar->getScrollPosition() + d_horzScrollbar->getStepSize() * -e.wheelChange);
	}

	e.handled = true;
}


/*************************************************************************
	Handler called when the scroll bar positions change
*************************************************************************/
bool StaticText::handleScrollbarChange(const EventArgs& e)
{
	requestRedraw();

	return true;
}

/*************************************************************************
	overridden so scroll bars are not partially clipped when active
*************************************************************************/
Rect StaticText::getUnclippedInnerRect(void) const
{
	// use default area from _Window_
	// (not from immediate base class Static, since that's what we're modifying)
	return Window::getUnclippedInnerRect();
}

} // End of  CEGUI namespace section
