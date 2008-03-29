/***********************************************************************
	filename: 	CEGUIScrollbar.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Scrollbar widget base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIThumb.h"
#include "CEGUIWindowManager.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String Scrollbar::EventNamespace("Scrollbar");
const String Scrollbar::WidgetTypeName("CEGUI/Scrollbar");

/*************************************************************************
    ScrollbarWindowRenderer
*************************************************************************/
ScrollbarWindowRenderer::ScrollbarWindowRenderer(const String& name) :
    WindowRenderer(name, Scrollbar::EventNamespace)
{
}

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
const String Scrollbar::EventScrollPositionChanged( "ScrollPosChanged" );
const String Scrollbar::EventThumbTrackStarted( "ThumbTrackStarted" );
const String Scrollbar::EventThumbTrackEnded( "ThumbTrackEnded" );
const String Scrollbar::EventScrollConfigChanged( "ScrollConfigChanged" );

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String Scrollbar::ThumbNameSuffix( "__auto_thumb__" );
const String Scrollbar::IncreaseButtonNameSuffix( "__auto_incbtn__" );
const String Scrollbar::DecreaseButtonNameSuffix( "__auto_decbtn__" );


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
void Scrollbar::initialiseComponents(void)
{
	// Set up thumb
	Thumb* thumb = getThumb();
	thumb->subscribeEvent(Thumb::EventThumbPositionChanged, Event::Subscriber(&CEGUI::Scrollbar::handleThumbMoved, this));
	thumb->subscribeEvent(Thumb::EventThumbTrackStarted, Event::Subscriber(&CEGUI::Scrollbar::handleThumbTrackStarted, this));
	thumb->subscribeEvent(Thumb::EventThumbTrackEnded, Event::Subscriber(&CEGUI::Scrollbar::handleThumbTrackEnded, this));

	// set up Increase button
	PushButton* increase = getIncreaseButton();
	increase->subscribeEvent(PushButton::EventMouseButtonDown, Event::Subscriber(&CEGUI::Scrollbar::handleIncreaseClicked, this));

	// set up Decrease button
	PushButton* decrease = getDecreaseButton();
	decrease->subscribeEvent(PushButton::EventMouseButtonDown, Event::Subscriber(&CEGUI::Scrollbar::handleDecreaseClicked, this));

	// do initial layout
	performChildWindowLayout();
}


/*************************************************************************
	Set the size of the document or data.
*************************************************************************/
void Scrollbar::setDocumentSize(float document_size)
{
	if (d_documentSize != document_size)
	{
		d_documentSize = document_size;
		updateThumb();

		WindowEventArgs args(this);
		onScrollConfigChanged(args);
	}

}


/*************************************************************************
	Set the page size for this scroll bar.
*************************************************************************/
void Scrollbar::setPageSize(float page_size)
{
	if (d_pageSize != page_size)
	{
		d_pageSize = page_size;
		updateThumb();

		WindowEventArgs args(this);
		onScrollConfigChanged(args);
	}

}


/*************************************************************************
	Set the step size for this scroll bar.
*************************************************************************/
void Scrollbar::setStepSize(float step_size)
{
	if (d_stepSize != step_size)
	{
		d_stepSize = step_size;

		WindowEventArgs args(this);
		onScrollConfigChanged(args);
	}

}


/*************************************************************************
	Set the overlap size for this scroll bar.
*************************************************************************/
void Scrollbar::setOverlapSize(float overlap_size)
{
	if (d_overlapSize != overlap_size)
	{
		d_overlapSize = overlap_size;

		WindowEventArgs args(this);
		onScrollConfigChanged(args);
	}

}


/*************************************************************************
	Set the current position of scroll bar within the document.
*************************************************************************/
void Scrollbar::setScrollPosition(float position)
{
	float old_pos = d_position;

	// max position is (docSize - pageSize), but must be at least 0 (in case doc size is very small)
	float max_pos = ceguimax((d_documentSize - d_pageSize), 0.0f);

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
	Handler triggered when the scroll position changes
*************************************************************************/
void Scrollbar::onScrollPositionChanged(WindowEventArgs& e)
{
	fireEvent(EventScrollPositionChanged, e, EventNamespace);
}


/*************************************************************************
	Handler triggered when the user begins to drag the scroll bar thumb. 	
*************************************************************************/
void Scrollbar::onThumbTrackStarted(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackStarted, e, EventNamespace);
}


/*************************************************************************
	Handler triggered when the scroll bar thumb is released
*************************************************************************/
void Scrollbar::onThumbTrackEnded(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackEnded, e, EventNamespace);
}


/*************************************************************************
	Handler triggered when the scroll bar data configuration changes
*************************************************************************/
void Scrollbar::onScrollConfigChanged(WindowEventArgs& e)
{
	performChildWindowLayout();
	fireEvent(EventScrollConfigChanged, e, EventNamespace);
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
	Handler for scroll wheel changes
*************************************************************************/
void Scrollbar::onMouseWheel(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseWheel(e);

	// scroll by e.wheelChange * stepSize
	setScrollPosition(d_position + d_stepSize * -e.wheelChange);

	// ensure the message does not go to our parent.
	e.handled = true;
}


/*************************************************************************
	handler function for when thumb moves.
*************************************************************************/
bool Scrollbar::handleThumbMoved(const EventArgs& e)
{
	// adjust scroll bar position as required.
	setScrollPosition(getValueFromThumb());

	return true;
}


/*************************************************************************
	handler function for when the increase button is clicked.
*************************************************************************/
bool Scrollbar::handleIncreaseClicked(const EventArgs& e)
{
	if (((const MouseEventArgs&)e).button == LeftButton)
	{
		// adjust scroll bar position as required.
		setScrollPosition(d_position + d_stepSize);

		return true;
	}
	else
	{
		return false;
	}

}


/*************************************************************************
	handler function for when the decrease button is clicked.
*************************************************************************/
bool Scrollbar::handleDecreaseClicked(const EventArgs& e)
{
	if (((const MouseEventArgs&)e).button == LeftButton)
	{
		// adjust scroll bar position as required.
		setScrollPosition(d_position - d_stepSize);

		return true;
	}
	else
	{
		return false;
	}
}


/*************************************************************************
	handler function for when thumb tracking begins
*************************************************************************/
bool Scrollbar::handleThumbTrackStarted(const EventArgs& e)
{
	// simply trigger our own version of this event
	WindowEventArgs args(this);
	onThumbTrackStarted(args);

	return true;
}


/*************************************************************************
	handler function for when thumb tracking begins
*************************************************************************/
bool Scrollbar::handleThumbTrackEnded(const EventArgs& e)
{
	// simply trigger our own version of this event
	WindowEventArgs args(this);
	onThumbTrackEnded(args);

	return true;
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

    // we ban all these properties from xml for auto windows
    if (isAutoWindow())
    {
        banPropertyFromXML(&d_documentSizeProperty);
        banPropertyFromXML(&d_pageSizeProperty);
        banPropertyFromXML(&d_stepSizeProperty);
        banPropertyFromXML(&d_overlapSizeProperty);
        banPropertyFromXML(&d_scrollPositionProperty);

        // scrollbars tend to have their visibility toggled alot, so we ban that as well
        banPropertyFromXML(&d_visibleProperty);
    }
}


/*************************************************************************
    Return a pointer to the 'increase' PushButtoncomponent widget for this
    Scrollbar.
*************************************************************************/
PushButton* Scrollbar::getIncreaseButton() const
{
    return static_cast<PushButton*>(WindowManager::getSingleton().getWindow(
                                    getName() + IncreaseButtonNameSuffix));
}


/*************************************************************************
    Return a pointer to the 'decrease' PushButton component widget for this
    Scrollbar.
*************************************************************************/
PushButton* Scrollbar::getDecreaseButton() const
{
    return static_cast<PushButton*>(WindowManager::getSingleton().getWindow(
                                    getName() + DecreaseButtonNameSuffix));
}


/*************************************************************************
    Return a pointer to the Thumb component widget for this Scrollbar.
*************************************************************************/
Thumb* Scrollbar::getThumb() const
{
    return static_cast<Thumb*>(WindowManager::getSingleton().getWindow(
                               getName() + ThumbNameSuffix));
}

/*************************************************************************
    update the size and location of the thumb to properly represent the
    current state of the scroll bar
*************************************************************************/
void Scrollbar::updateThumb(void)
{
    if (d_windowRenderer != 0)
    {
        ScrollbarWindowRenderer* wr = (ScrollbarWindowRenderer*)d_windowRenderer;
        wr->updateThumb();
    }
    else
    {
        //updateThumb_impl();
        throw InvalidRequestException("Scrollbar::updateThumb - This function must be implemented by the window renderer module");
    }
}

/*************************************************************************
    return value that best represents current scroll bar position given
    the current location of the thumb.
*************************************************************************/
float Scrollbar::getValueFromThumb(void) const
{
    if (d_windowRenderer != 0)
    {
        ScrollbarWindowRenderer* wr = (ScrollbarWindowRenderer*)d_windowRenderer;
        return wr->getValueFromThumb();
    }
    else
    {
        //return getValueFromThumb_impl();
        throw InvalidRequestException("Scrollbar::getValueFromThumb - This function must be implemented by the window renderer module");
    }
}

/*************************************************************************
    Given window location 'pt', return a value indicating what change
    should be made to the scroll bar.
*************************************************************************/
float Scrollbar::getAdjustDirectionFromPoint(const Point& pt) const
{
    if (d_windowRenderer != 0)
    {
        ScrollbarWindowRenderer* wr = (ScrollbarWindowRenderer*)d_windowRenderer;
        return wr->getAdjustDirectionFromPoint(pt);
    }
    else
    {
        //return getAdjustDirectionFromPoint_impl(pt);
        throw InvalidRequestException("Scrollbar::getAdjustDirectionFromPoint - This function must be implemented by the window renderer module");
    }
}

} // End of  CEGUI namespace section
