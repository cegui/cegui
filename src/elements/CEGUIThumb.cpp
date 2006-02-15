/************************************************************************
	filename: 	CEGUIThumb.cpp
	created:	25/4/2004
	author:		Paul D Turner
	
	purpose:	Implements common parts of the Thumb base class widget
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
#include "elements/CEGUIThumb.h"
#include "CEGUICoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String Thumb::EventNamespace("Thumb");

/*************************************************************************
	Static Properties for this class
*************************************************************************/
ThumbProperties::HotTracked	Thumb::d_hotTrackedProperty;
ThumbProperties::VertFree	Thumb::d_vertFreeProperty;
ThumbProperties::HorzFree	Thumb::d_horzFreeProperty;
ThumbProperties::VertRange	Thumb::d_vertRangeProperty;
ThumbProperties::HorzRange	Thumb::d_horzRangeProperty;


/*************************************************************************
	Event name constants
*************************************************************************/
// generated internally by Window
const String Thumb::EventThumbPositionChanged( "ThumbPosChanged" );
const String Thumb::EventThumbTrackStarted( "ThumbTrackStarted" );
const String Thumb::EventThumbTrackEnded( "ThumbTrackEnded" );


/*************************************************************************
	Constructor for Thumb objects
*************************************************************************/
Thumb::Thumb(const String& type, const String& name) :
	PushButton(type, name),
	d_hotTrack(true),
	d_vertFree(false),
	d_horzFree(false),
	d_vertMin(0.0f),
	d_vertMax(1.0f),
    d_horzMin(0.0f),
	d_horzMax(1.0f),
    d_beingDragged(false)
{
	addThumbProperties();
}


/*************************************************************************
	Destructor for Thumb objects	
*************************************************************************/
Thumb::~Thumb(void)
{
}


/*************************************************************************
	set the movement range of the thumb for the vertical axis.	
*************************************************************************/
void Thumb::setVertRange(float min, float max)
{
	// ensure min <= max, swap if not.
	if (min > max)
	{
		float tmp = min;
		max = min;
		min = tmp;
	}

	d_vertMax = max;
	d_vertMin = min;

	// validate current position.
	float cp = getWindowYPosition().asRelative(getParentPixelHeight());

	if (cp < min)
	{
		setWindowYPosition(cegui_reldim(min));
	}
	else if (cp > max)
	{
		setWindowYPosition(cegui_reldim(max));
	}

}


/*************************************************************************
	set the movement range of the thumb for the horizontal axis.
*************************************************************************/
void Thumb::setHorzRange(float min, float max)
{
    Size parentSize(getParentPixelSize());

	// ensure min <= max, swap if not.
	if (min > max)
	{
		float tmp = min;
		max = min;
		min = tmp;
	}

	d_horzMax = max;
	d_horzMin = min;

	// validate current position.
	float cp = getWindowXPosition().asAbsolute(parentSize.d_width);

	if (cp < min)
	{
		setWindowXPosition(cegui_absdim(min));
	}
	else if (cp > max)
	{
		setWindowXPosition(cegui_absdim(max));
	}

}


/*************************************************************************
	event triggered internally when the position of the thumb	
*************************************************************************/
void Thumb::onThumbPositionChanged(WindowEventArgs& e)
{
	fireEvent(EventThumbPositionChanged, e, EventNamespace);
}


/*************************************************************************
	Handler triggered when the user begins to drag the thumb. 	
*************************************************************************/
void Thumb::onThumbTrackStarted(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackStarted, e, EventNamespace);
}


/*************************************************************************
	Handler triggered when the thumb is released
*************************************************************************/
void Thumb::onThumbTrackEnded(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackEnded, e, EventNamespace);
}


/*************************************************************************
	Handler for mouse movement events
*************************************************************************/
void Thumb::onMouseMove(MouseEventArgs& e)
{
	// default processing
	PushButton::onMouseMove(e);

	// only react if we are being dragged
	if (d_beingDragged)
	{
        Size parentSize(getParentPixelSize());

		Vector2 delta;
		float hmin, hmax, vmin, vmax;

        delta = CoordConverter::screenToWindow(*this, e.position);

        hmin = d_horzMin;
        hmax = d_horzMax;
        vmin = d_vertMin;
        vmax = d_vertMax;

		// calculate amount of movement      
		delta -= d_dragPoint;
        delta.d_x /= parentSize.d_width;
        delta.d_y /= parentSize.d_height;

		//
		// Calculate new (pixel) position for thumb
		//
		UVector2 newPos(getWindowPosition());

		if (d_horzFree)
		{
			newPos.d_x.d_scale += delta.d_x;

			// limit value to within currently set range
			newPos.d_x.d_scale = (newPos.d_x.d_scale < hmin) ? hmin : (newPos.d_x.d_scale > hmax) ? hmax : newPos.d_x.d_scale;
		}

		if (d_vertFree)
		{
			newPos.d_y.d_scale += delta.d_y;

			// limit new position to within currently set range
			newPos.d_y.d_scale = (newPos.d_y.d_scale < vmin) ? vmin : (newPos.d_y.d_scale > vmax) ? vmax : newPos.d_y.d_scale;
		}

		// update thumb position if needed
		if (newPos != getWindowPosition())
		{
			setWindowPosition(newPos);

			// send notification as required
			if (d_hotTrack)
			{
				WindowEventArgs args(this);
				onThumbPositionChanged(args);
			}

		}

	}

	e.handled = true;
}


/*************************************************************************
	Handler for mouse button down events
*************************************************************************/
void Thumb::onMouseButtonDown(MouseEventArgs& e)
{
	// default processing
	PushButton::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		// initialise the dragging state
		d_beingDragged = true;
		d_dragPoint = CoordConverter::screenToWindow(*this, e.position);

		// trigger tracking started event
		WindowEventArgs args(this);
		onThumbTrackStarted(args);

		e.handled = true;
	}

}


/*************************************************************************
	Handler for event triggered when we lose capture of mouse input
*************************************************************************/
void Thumb::onCaptureLost(WindowEventArgs& e)
{
	// default handling
	PushButton::onCaptureLost(e);

	d_beingDragged = false;

	// trigger tracking ended event
	WindowEventArgs args(this);
	onThumbTrackEnded(args);

	// send notification whenever thumb is released
	onThumbPositionChanged(args);
}


/*************************************************************************
	Return a std::pair that describes the current range set for the
	vertical movement.	
*************************************************************************/
std::pair<float, float>	Thumb::getVertRange(void) const
{
	return std::make_pair(d_vertMin, d_vertMax);
}


/*************************************************************************
	Return a std::pair that describes the current range set for the
	horizontal movement.	
*************************************************************************/
std::pair<float, float>	Thumb::getHorzRange(void) const
{
	return std::make_pair(d_horzMin, d_horzMax);
}


/*************************************************************************
	Add thumb specifiec properties
*************************************************************************/
void Thumb::addThumbProperties(void)
{
	addProperty(&d_hotTrackedProperty);
	addProperty(&d_vertFreeProperty);
	addProperty(&d_horzFreeProperty);
	addProperty(&d_vertRangeProperty);
	addProperty(&d_horzRangeProperty);
}


} // End of  CEGUI namespace section
