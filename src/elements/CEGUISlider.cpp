/************************************************************************
	filename: 	CEGUISlider.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Slider widget base class
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
#include "elements/CEGUISlider.h"
#include "elements/CEGUIThumb.h"
#include <boost/bind.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Event name constants
*************************************************************************/
const utf8	Slider::ValueChanged[]		= "ValueChanged";


/*************************************************************************
	Slider base class constructor
*************************************************************************/
Slider::Slider(const String& type, const String& name) :
	Window(type, name),
	d_value(0),
	d_maxValue(100),
	d_thumb(NULL)
{
	addSliderEvents();
}


/*************************************************************************
	Slider base class destructor
*************************************************************************/
Slider::~Slider(void)
{
}


/*************************************************************************
	Initialises the Window based object ready for use.	
*************************************************************************/
void Slider::initialise(void)
{
	// create and attach thumb
	d_thumb = createThumb();
	addChildWindow(d_thumb);

	// bind handler to thumb event triggered when thumb moves
	d_thumb->subscribeEvent(Thumb::ThumbPositionChanged, boost::bind(&CEGUI::Slider::handleThumbMoved, this, _1));

	layoutComponentWidgets();
}


/*************************************************************************
	set the maximum value for the slider.
	Note that the minimum value is fixed at 0.	
*************************************************************************/
void Slider::setMaxValue(ulong maxVal)
{
	d_maxValue = maxVal;

	ulong oldval = d_value;

	// limit current value to be within new max
	if (d_value > d_maxValue) {
		d_value = d_maxValue;
	}

	updateThumb();

	// send notification if slider value changed.
	if (d_value != oldval)
	{
		onValueChanged(WindowEventArgs(this));
	}

}


/*************************************************************************
	set the current slider value.
*************************************************************************/
void Slider::setCurrentValue(ulong value)
{
	ulong oldval = d_value;

	// range for value: 0 <= value <= maxValue
	d_value = (value <= d_maxValue) ? value : d_maxValue;

	updateThumb();

	// send notification if slider value changed.
	if (d_value != oldval)
	{
		onValueChanged(WindowEventArgs(this));
	}

}


/*************************************************************************
	Add slider specific events	
*************************************************************************/
void Slider::addSliderEvents(void)
{
	addEvent(ValueChanged);
}


/*************************************************************************
	Handler triggered when the slider value changes
*************************************************************************/
void Slider::onValueChanged(WindowEventArgs& e)
{
	fireEvent(ValueChanged, e);
}


/*************************************************************************
	Handler for when a mouse button is pressed
*************************************************************************/
void Slider::onMouseButtonDown(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		int adj = getAdjustDirectionFromPoint(e.position);

		// adjust slider position in whichever direction as required.
		if (adj != 0)
		{
			setCurrentValue(getCurrentValue() + adj);
		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for when the size of the slider widget changes.
*************************************************************************/
void Slider::onSized(EventArgs& e)
{
	// base class processing
	Window::onSized(e);

	layoutComponentWidgets();

	e.handled = true;
}


/*************************************************************************
	handler function for when thumb moves.	
*************************************************************************/
void Slider::handleThumbMoved(const EventArgs& e)
{
	setCurrentValue(getValueFromThumb());
}


} // End of  CEGUI namespace section
