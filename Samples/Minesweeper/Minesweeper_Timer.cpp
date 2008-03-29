/***********************************************************************
	filename: 	CEGUITimer.cpp
	created:	08/08/2006
	author:		Olivier Delannoy 
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
#include "Minesweeper_Timer.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace TimerProperties
{
CEGUI::String Delay::get(const CEGUI::PropertyReceiver* receiver) const
{
    return CEGUI::PropertyHelper::floatToString(static_cast<const Timer*>(receiver)->getDelay());
}
void Delay::set(CEGUI::PropertyReceiver* receiver, const CEGUI::String& value)
{
    static_cast<Timer*>(receiver)->setDelay(CEGUI::PropertyHelper::stringToFloat(value));
}

}
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const CEGUI::String Timer::WidgetTypeName("Timer");
const CEGUI::String Timer::EventNamespace("Timer"); 
const CEGUI::String Timer::EventTimerAlarm("EventTimerAlarm");
/*************************************************************************
 * Property definition 
*************************************************************************/
TimerProperties::Delay Timer::d_delayProperty;
/*************************************************************************
    Constructor
*************************************************************************/
Timer::Timer(const CEGUI::String& type, const CEGUI::String& name) :
    Window(type, name),
    d_delay(0), 
    d_started(false), 
    d_currentValue(0)
{
    CEGUI::UVector2 sz(CEGUI::UDim(1.0, 0.0), CEGUI::UDim(1.0, 0.0));
    setMaxSize(sz);
    setSize(sz);
    addTimerProperties();
}

void Timer::start()
{
    d_started = true;
}
void Timer::stop()
{
    d_started = false;
}
bool Timer::isStarted() const
{
    return d_started;
}
void Timer::setDelay(float delay)
{
    d_delay = delay;
}

float Timer::getDelay() const
{
    return d_delay;
}

void Timer::updateSelf(float elapsed)
{
    if (d_delay > 0 && d_started)
    {
        d_currentValue += elapsed;
        if (d_currentValue >= d_delay)
        {
            d_currentValue -= d_delay;
            CEGUI::WindowEventArgs args(this);
            fireEvent(EventTimerAlarm, args, EventNamespace);
        }
    }
}

void Timer::addTimerProperties(void)
{
    addProperty(&d_delayProperty);
}

TimerFactory& getTimerFactory()
{
    static TimerFactory s_factory;
    return s_factory;
}
