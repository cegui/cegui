/************************************************************************
	filename: 	CEGUIScrollbar.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Scrollbar widget base class
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
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIThumb.h"
#include <boost/bind.hpp>


/*************************************************************************
	TODO:
	Mouse wheel support
*************************************************************************/

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Definition of Properties for this class
*************************************************************************/
ScrollbarProperties::DocumentSize	Scrollbar::d_documentSizeProperty;
ScrollbarProperties::PageSize		Scrollbar::d_pageSizeProperty;
ScrollbarProperties::StepSize		Scrollbar::d_stepSizeProperty;
ScrollbarProperties::OverlapSize	Scrollbar::d_overlapSizeProperty;
ScrollbarProperties::ScrollPosition	Scrollbar::d_scrollPositionProperty;


/*************************************************************************
	Event name constants
*************************************************************************/
const utf8	Scrollbar::ScrollPositionChanged[]		= "ScrollPosChanged";


/*************************************************************************
	Constructor for Scrollbar objects
*************************************************************************/
Scrollbar::Scrollbar(const String& type, const String& name) :
	Window(type, name),
	d_documentSize(1.0f),
	d_pageSize(0.0f),
	d_stepSize(1.0f),
	d_overlapSize(0.0f),
	d_position(0.0f)
{
	addScrollbarEvents();
	addScrollbarProperties();
}


/*************************************************************************
	Destructor for Scrollbar objects
*************************************************************************/
Scrollbar::~Scrollbar(void)
{
}


/*************************************************************************
	Initialises the Scrollbar object ready for use.
*************************************************************************/
void Scrollbar::initialise(void)
{
	// Set up thumb
	d_thumb = createThumb();
	addChildWindow(d_thumb);
	d_thumb->subscribeEvent(Thumb::ThumbPositionChanged, boost::bind(&CEGUI::Scrollbar::handleThumbMoved, this, _1));

	// set up Increase button
	d_increase = createIncreaseButton();
	addChildWindow(d_increase);
	d_increase->subscribeEvent(PushButton::Clicked, boost::bind(&CEGUI::Scrollbar::handleIncreaseClicked, this, _1));

	// set up Decrease button
	d_decrease = createDecreaseButton();
	addChildWindow(d_decrease);
	d_decrease->subscribeEvent(PushButton::Clicked, boost::bind(&CEGUI::Scrollbar::handleDecreaseClicked, this, _1));

	// do initial layout
	layoutComponentWidgets();
}


/*************************************************************************
	Set the size of the document or data.
*************************************************************************/
void Scrollbar::setDocumentSize(float document_size)
{
	d_documentSize = document_size;
	updateThumb();
}


/*************************************************************************
	Set the page size for this scroll bar.
*************************************************************************/
void Scrollbar::setPageSize(float page_size)
{
	d_pageSize = page_size;
	updateThumb();
}


/*************************************************************************
	Set the step size for this scroll bar.
*************************************************************************/
void Scrollbar::setStepSize(float step_size)
{
	d_stepSize = step_size;
}


/*************************************************************************
	Set the overlap size for this scroll bar.
*************************************************************************/
void Scrollbar::setOverlapSize(float overlap_size)
{
	d_overlapSize = overlap_size;
}


/*************************************************************************
	Set the current position of scroll bar within the document.
*************************************************************************/
void Scrollbar::setScrollPosition(float position)
{
	using namespace std;

	float old_pos = d_position;

	// max position is (docSize - pageSize), but must be at least 0 (in case doc size is very small)
	float max_pos = max((d_documentSize - d_pageSize), 0.0f);

	// limit position to valid range:  0 <= position <= max_pos
	d_position = (position >= 0) ? ((position <= max_pos) ? position : max_pos) : 0.0f;

	updateThumb();

	// notification if required
	if (d_position != old_pos)
	{
		WindowEventArgs args(this);
		onScrollPositionChanged(args);
	}

}


/*************************************************************************
	Add scroll bar specific events
*************************************************************************/
void Scrollbar::addScrollbarEvents(void)
{
	addEvent(ScrollPositionChanged);
}


/*************************************************************************
	Handler triggered when the scroll position changes
*************************************************************************/
void Scrollbar::onScrollPositionChanged(WindowEventArgs& e)
{
	fireEvent(ScrollPositionChanged, e);
}


/*************************************************************************
	Handler for when mouse button is clicked within the container
*************************************************************************/
void Scrollbar::onMouseButtonDown(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		float adj = getAdjustDirectionFromPoint(e.position);

		// adjust scroll bar position in whichever direction as required.
		if (adj != 0)
		{
			setScrollPosition(d_position + ((d_pageSize - d_overlapSize) * adj));
		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for when widget is re-sized
*************************************************************************/
void Scrollbar::onSized(WindowEventArgs& e)
{
	// base class processing
	Window::onSized(e);

	layoutComponentWidgets();

	e.handled = true;
}


/*************************************************************************
	handler function for when thumb moves.
*************************************************************************/
void Scrollbar::handleThumbMoved(const EventArgs& e)
{
	// adjust scroll bar position as required.
	setScrollPosition(getValueFromThumb());
}


/*************************************************************************
	handler function for when the increase button is clicked.
*************************************************************************/
void Scrollbar::handleIncreaseClicked(const EventArgs& e)
{
	// adjust scroll bar position as required.
	setScrollPosition(d_position + d_stepSize);
}


/*************************************************************************
	handler function for when the decrease button is clicked.
*************************************************************************/
void Scrollbar::handleDecreaseClicked(const EventArgs& e)
{
	// adjust scroll bar position as required.
	setScrollPosition(d_position - d_stepSize);
}

/*************************************************************************
	Add scroll bar properties
*************************************************************************/
void Scrollbar::addScrollbarProperties(void)
{
	addProperty(&d_documentSizeProperty);
	addProperty(&d_pageSizeProperty);
	addProperty(&d_stepSizeProperty);
	addProperty(&d_overlapSizeProperty);
	addProperty(&d_scrollPositionProperty);
}



} // End of  CEGUI namespace section
