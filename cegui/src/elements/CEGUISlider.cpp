/***********************************************************************
	filename: 	CEGUISlider.cpp
	created:	13/4/2004
	author:		Paul D Turner

	purpose:	Implementation of Slider widget base class
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
#include "elements/CEGUISlider.h"
#include "elements/CEGUIThumb.h"
#include "CEGUIWindowManager.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String Slider::EventNamespace("Slider");
const String Slider::WidgetTypeName("CEGUI/Slider");

/*************************************************************************
    SliderWindowRenderer
*************************************************************************/
SliderWindowRenderer::SliderWindowRenderer(const String& name) :
    WindowRenderer(name, Slider::EventNamespace)
{
}

/*************************************************************************
	Definition of Properties for this class
*************************************************************************/
SliderProperties::CurrentValue	Slider::d_currentValueProperty;
SliderProperties::MaximumValue	Slider::d_maximumValueProperty;
SliderProperties::ClickStepSize	Slider::d_clickStepSizeProperty;


/*************************************************************************
	Event name constants
*************************************************************************/
const String Slider::EventValueChanged( "ValueChanged" );
const String Slider::EventThumbTrackStarted( "ThumbTrackStarted" );
const String Slider::EventThumbTrackEnded( "ThumbTrackEnded" );

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String Slider::ThumbNameSuffix( "__auto_thumb__" );

/*************************************************************************
	Slider base class constructor
*************************************************************************/
Slider::Slider(const String& type, const String& name) :
	Window(type, name),
	d_value(0.0f),
	d_maxValue(1.0f),
	d_step(0.01f)
{
	addSliderProperties();
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
void Slider::initialiseComponents(void)
{
	// get thumb
	Thumb* thumb = getThumb();

	// bind handler to thumb events
	thumb->subscribeEvent(Thumb::EventThumbPositionChanged, Event::Subscriber(&CEGUI::Slider::handleThumbMoved, this));
	thumb->subscribeEvent(Thumb::EventThumbTrackStarted, Event::Subscriber(&CEGUI::Slider::handleThumbTrackStarted, this));
	thumb->subscribeEvent(Thumb::EventThumbTrackEnded, Event::Subscriber(&CEGUI::Slider::handleThumbTrackEnded, this));

	performChildWindowLayout();
}


/*************************************************************************
	set the maximum value for the slider.
	Note that the minimum value is fixed at 0.
*************************************************************************/
void Slider::setMaxValue(float maxVal)
{
	d_maxValue = maxVal;

	float oldval = d_value;

	// limit current value to be within new max
	if (d_value > d_maxValue) {
		d_value = d_maxValue;
	}

	updateThumb();

	// send notification if slider value changed.
	if (d_value != oldval)
	{
		WindowEventArgs args(this);
		onValueChanged(args);
	}

}


/*************************************************************************
	set the current slider value.
*************************************************************************/
void Slider::setCurrentValue(float value)
{
	float oldval = d_value;

	// range for value: 0 <= value <= maxValue
	d_value = (value >= 0.0f) ? ((value <= d_maxValue) ? value : d_maxValue) : 0.0f;

	updateThumb();

	// send notification if slider value changed.
	if (d_value != oldval)
	{
		WindowEventArgs args(this);
		onValueChanged(args);
	}

}


/*************************************************************************
	Handler triggered when the slider value changes
*************************************************************************/
void Slider::onValueChanged(WindowEventArgs& e)
{
	fireEvent(EventValueChanged, e, EventNamespace);
}


/*************************************************************************
	Handler triggered when the user begins to drag the slider thumb.
*************************************************************************/
void Slider::onThumbTrackStarted(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackStarted, e, EventNamespace);
}


/*************************************************************************
	Handler triggered when the slider thumb is released
*************************************************************************/
void Slider::onThumbTrackEnded(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackEnded, e, EventNamespace);
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
		float adj = getAdjustDirectionFromPoint(e.position);

		// adjust slider position in whichever direction as required.
		if (adj != 0)
		{
			setCurrentValue(d_value + (adj * d_step));
		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for scroll wheel changes
*************************************************************************/
void Slider::onMouseWheel(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseWheel(e);

	// scroll by e.wheelChange * stepSize
	setCurrentValue(d_value + d_step * e.wheelChange);

	// ensure the message does not go to our parent.
	e.handled = true;
}


/*************************************************************************
	handler function for when thumb moves.
*************************************************************************/
bool Slider::handleThumbMoved(const EventArgs& e)
{
	setCurrentValue(getValueFromThumb());

	return true;
}


/*************************************************************************
	handler function for when thumb tracking begins
*************************************************************************/
bool Slider::handleThumbTrackStarted(const EventArgs& e)
{
	// simply trigger our own version of this event
	WindowEventArgs args(this);
	onThumbTrackStarted(args);

	return true;
}


/*************************************************************************
	handler function for when thumb tracking begins
*************************************************************************/
bool Slider::handleThumbTrackEnded(const EventArgs& e)
{
	// simply trigger our own version of this event
	WindowEventArgs args(this);
	onThumbTrackEnded(args);

	return true;
}


/*************************************************************************
	Add properties for the slider
*************************************************************************/
void Slider::addSliderProperties(void)
{
	addProperty(&d_currentValueProperty);
	addProperty(&d_clickStepSizeProperty);
	addProperty(&d_maximumValueProperty);
}

/*************************************************************************
    Return a pointer to the Thumb component widget..
*************************************************************************/
Thumb* Slider::getThumb() const
{
    return static_cast<Thumb*>(WindowManager::getSingleton().getWindow(
                               getName() + ThumbNameSuffix));
}

/*************************************************************************
    update the size and location of the thumb to properly represent the
    current state of the scroll bar
*************************************************************************/
void Slider::updateThumb(void)
{
    if (d_windowRenderer != 0)
    {
        SliderWindowRenderer* wr = (SliderWindowRenderer*)d_windowRenderer;
        wr->updateThumb();
    }
    else
    {
        //updateThumb_impl();
        throw InvalidRequestException("Slider::updateThumb - This function must be implemented by the window renderer module");
    }
}

/*************************************************************************
    return value that best represents current scroll bar position given
    the current location of the thumb.
*************************************************************************/
float Slider::getValueFromThumb(void) const
{
    if (d_windowRenderer != 0)
    {
        SliderWindowRenderer* wr = (SliderWindowRenderer*)d_windowRenderer;
        return wr->getValueFromThumb();
    }
    else
    {
        //return getValueFromThumb_impl();
        throw InvalidRequestException("Slider::getValueFromThumb - This function must be implemented by the window renderer module");
    }
}

/*************************************************************************
    Given window location 'pt', return a value indicating what change
    should be made to the scroll bar.
*************************************************************************/
float Slider::getAdjustDirectionFromPoint(const Point& pt) const
{
    if (d_windowRenderer != 0)
    {
        SliderWindowRenderer* wr = (SliderWindowRenderer*)d_windowRenderer;
        return wr->getAdjustDirectionFromPoint(pt);
    }
    else
    {
        //return getAdjustDirectionFromPoint_impl(pt);
        throw InvalidRequestException("Slider::getAdjustDirectionFromPoint - This function must be implemented by the window renderer module");
    }
}

} // End of  CEGUI namespace section
