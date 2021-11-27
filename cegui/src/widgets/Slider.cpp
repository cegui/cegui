/***********************************************************************
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
#include "CEGUI/widgets/Slider.h"
#include "CEGUI/widgets/Thumb.h"

namespace CEGUI
{
const String Slider::EventNamespace("Slider");
const String Slider::WidgetTypeName("CEGUI/Slider");
/*************************************************************************
    Event name constants
*************************************************************************/
const String Slider::EventValueChanged("ValueChanged");
const String Slider::EventMinimumValueChanged("MinimumValueChanged");
const String Slider::EventMaximumValueChanged("MaximumValueChanged");
const String Slider::EventStepChanged("StepChanged");
const String Slider::EventThumbTrackStarted("ThumbTrackStarted");
const String Slider::EventThumbTrackEnded("ThumbTrackEnded");
/*************************************************************************
    Child Widget name constants
*************************************************************************/
const String Slider::ThumbName("__auto_thumb__");

//----------------------------------------------------------------------------//
SliderWindowRenderer::SliderWindowRenderer(const String& name) :
    WindowRenderer(name, Slider::EventNamespace)
{
}

//----------------------------------------------------------------------------//
Slider::Slider(const String& type, const String& name) :
	Window(type, name)
{
	addSliderProperties();
}

//----------------------------------------------------------------------------//
void Slider::initialiseComponents()
{
	Thumb* thumb = getThumb();
	thumb->subscribeEvent(Thumb::EventThumbPositionChanged, Event::Subscriber(&CEGUI::Slider::handleThumbMoved, this));
	thumb->subscribeEvent(Thumb::EventThumbTrackStarted, Event::Subscriber(&CEGUI::Slider::handleThumbTrackStarted, this));
	thumb->subscribeEvent(Thumb::EventThumbTrackEnded, Event::Subscriber(&CEGUI::Slider::handleThumbTrackEnded, this));

    Window::initialiseComponents();
}

//----------------------------------------------------------------------------//
void Slider::setCurrentValue(value_type value)
{
    value = std::max(std::min(value, d_maxValue), d_minValue);

    if (value != d_currentValue)
    {
        d_currentValue = value;
        updateThumb();

        WindowEventArgs args(this);
        onValueChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Slider::setMinimumValue(value_type minValue)
{
    // FIXME: correctness vs freedom in the order of calls
    //if (minValue > d_maxValue)
    //    minValue = d_maxValue;

    if (minValue != d_minValue)
    {
        d_minValue = minValue;
        updateThumb();

        WindowEventArgs args(this);
        onMinimumValueChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Slider::setMaximumValue(value_type maxValue)
{
    // FIXME: correctness vs freedom in the order of calls
    //if (maxValue < d_minValue)
    //    maxValue = d_minValue;

    if (maxValue != d_maxValue)
    {
        d_maxValue = maxValue;
        updateThumb();

        WindowEventArgs args(this);
        onMaximumValueChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Slider::onValueChanged(WindowEventArgs& e)
{
	fireEvent(EventValueChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Slider::onMinimumValueChanged(WindowEventArgs& e)
{
    fireEvent(EventMinimumValueChanged, e, EventNamespace);

    if (d_currentValue < d_minValue)
        setCurrentValue(d_minValue);
}

//----------------------------------------------------------------------------//
void Slider::onMaximumValueChanged(WindowEventArgs& e)
{
    fireEvent(EventMaximumValueChanged, e, EventNamespace);

    if (d_currentValue > d_maxValue)
        setCurrentValue(d_maxValue);
}

//----------------------------------------------------------------------------//
void Slider::onStepChanged(WindowEventArgs& e)
{
    fireEvent(EventStepChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Slider::onThumbTrackStarted(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackStarted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Slider::onThumbTrackEnded(WindowEventArgs& e)
{
	fireEvent(EventThumbTrackEnded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Slider::onCursorPressHold(CursorInputEventArgs& e)
{
	// base class processing
    Window::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
	{
        // adjust slider position in whichever direction as required.
        const float adj = getAdjustDirectionFromPoint(e.position);
		if (adj != 0.f)
			setCurrentValue(d_currentValue + adj * d_stepSize);

		++e.handled;
	}
}

//----------------------------------------------------------------------------//
void Slider::onScroll(CursorInputEventArgs& e)
{
    Window::onScroll(e);
    value_type prevValue = d_currentValue;
    setCurrentValue(d_currentValue + d_stepSize * e.scroll);
    if (prevValue != d_currentValue)
        ++e.handled;
}

//----------------------------------------------------------------------------//
bool Slider::handleThumbMoved(const EventArgs&)
{
	setCurrentValue(getValueFromThumb());
	return true;
}

//----------------------------------------------------------------------------//
bool Slider::handleThumbTrackStarted(const EventArgs&)
{
	WindowEventArgs args(this);
	onThumbTrackStarted(args);
	return true;
}

//----------------------------------------------------------------------------//
bool Slider::handleThumbTrackEnded(const EventArgs&)
{
	WindowEventArgs args(this);
	onThumbTrackEnded(args);
	return true;
}

//----------------------------------------------------------------------------//
Thumb* Slider::getThumb() const
{
    return static_cast<Thumb*>(getChild(ThumbName));
}

//----------------------------------------------------------------------------//
void Slider::updateThumb()
{
    if (!d_windowRenderer)
        throw InvalidRequestException(
            "This function must be implemented by the window renderer module");

    static_cast<SliderWindowRenderer*>(d_windowRenderer)->updateThumb();
}

//----------------------------------------------------------------------------//
Slider::value_type Slider::getValueFromThumb() const
{
    if (!d_windowRenderer)
        throw InvalidRequestException(
            "This function must be implemented by the window renderer module");

    return static_cast<SliderWindowRenderer*>(d_windowRenderer)->getValueFromThumb();
}

//----------------------------------------------------------------------------//
float Slider::getAdjustDirectionFromPoint(const glm::vec2& pt) const
{
    if (!d_windowRenderer)
        throw InvalidRequestException(
            "This function must be implemented by the window renderer module");

    return static_cast<SliderWindowRenderer*>(d_windowRenderer)->getAdjustDirectionFromPoint(pt);
}

//----------------------------------------------------------------------------//
bool Slider::validateWindowRenderer(const WindowRenderer* renderer) const
{
    return dynamic_cast<const SliderWindowRenderer*>(renderer) != nullptr;
}

//----------------------------------------------------------------------------//
void Slider::addSliderProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Slider, value_type,
        "CurrentValue", "Property to get/set the current value of the slider.  Value is a float.",
        &Slider::setCurrentValue, &Slider::getCurrentValue, 0.0
    );

    CEGUI_DEFINE_PROPERTY(Slider, value_type,
        "MinimumValue", "Property to get/set the minimum value of the slider.  Value is a float.",
        &Slider::setMinimumValue, &Slider::getMinimumValue, 0.0
    );

    CEGUI_DEFINE_PROPERTY(Slider, value_type,
        "MaximumValue", "Property to get/set the maximum value of the slider.  Value is a float.",
        &Slider::setMaximumValue, &Slider::getMaximumValue, 1.0
    );

    CEGUI_DEFINE_PROPERTY(Slider, value_type,
        "ClickStepSize", "Property to get/set the click step size of the slider.  Value is a float.",
        &Slider::setStepSize, &Slider::getStepSize, 0.01
    );
}

} // End of  CEGUI namespace section
