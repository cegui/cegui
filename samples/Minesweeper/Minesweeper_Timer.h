/***********************************************************************
    created:    08/08/2006
    author:     Olivier Delannoy

    purpose:    Interface to timer window 
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
#ifndef _Minesweeper_Timer_h_
#define _Minesweeper_Timer_h_

#include "CEGUI/Window.h"
#include "CEGUI/WindowFactory.h"
#include "CEGUI/Property.h" 

namespace TimerProperties 
{
/*!
    \brief 
    Property to access the delay between two alarm 

    \par Usage:
        - Name: Delay
        - Format: "[float]".

    \par Where:
        - [float] represents the current delay of the timer
*/   
class Delay : public CEGUI::Property
{
public:    
    Delay() : Property("Delay", "Property to get/set the current delay used by the timer. Value is a float.", "0.000000") {}
    CEGUI::String get(const CEGUI::PropertyReceiver* receiver) const;
    void set(CEGUI::PropertyReceiver* receiver, const CEGUI::String& value);
    CEGUI::Property* clone() const;
};
  
}

/*!
\brief
    Window class intended to be used as a timer. 
    
    
    An application can contains several timer to generate events each count milliseconds. 
    The accuracy of the timer is not accurate enough to do any time counting and is 
    not guaranty either.
    
    This class does no rendering and so appears totally transparent.  This window defaults
    to position 0.0f, 0.0f with a size of 1.0f x 1.0f.

*/
class Timer : public CEGUI::Window
{
public:
    /*************************************************************************
        Constants
    *************************************************************************/
    // type name for this widget
    static const CEGUI::String WidgetTypeName;             //!< The unique typename of this widget
    static const CEGUI::String EventNamespace;             //!< Store the event namespace related to the timer 
    static const CEGUI::String EventTimerAlarm;            //!< The name of the event generated  by this widget 



    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for Timer windows.
    */
    Timer(const CEGUI::String& type, const CEGUI::String& name);
    /*!
    \brief
        Destructor for Timer windows.
    */
    virtual ~Timer(void) {}

    /*!
    \brief start the timer in order to generate alarm event 
    */
    void start();
    /*!
    \brief stop generating alarm event  
    */
    void stop();
    /*!
    \brief Check wether the timer is started or not 
    */
    bool isStarted() const;
    /*! 
    \brief 
        Set the delay between to event generation in seconds 
     */
    void setDelay(float delay);

    float getDelay() const;

protected:
    virtual void updateSelf(float elapsed);
    
private:
    static TimerProperties::Delay d_delayProperty;
    float d_delay; //!< Store the current delay between two alarm 
    float d_currentValue; //!< Set the current value 
    bool  d_started; //!< Store wether the timer should produce event or not 
    void addTimerProperties(void);
};

class TimerFactory : public CEGUI::WindowFactory
{
public:
    TimerFactory() : CEGUI::WindowFactory(Timer::WidgetTypeName) {}
    CEGUI::Window* createWindow(const CEGUI::String& name)
    { return new Timer(d_type, name); }
    void destroyWindow(CEGUI::Window* window)
    { delete window;}
};

TimerFactory& getTimerFactory();

#endif  // end of guard _CEGUIGUISheet_h_
