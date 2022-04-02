/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Implements the EventSet class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/EventSet.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/GlobalEventSet.h"
#include "CEGUI/ScriptModule.h"
#include "CEGUI/System.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
void EventSet::addEvent(const String& name)
{
    if (isEventPresent(name))
        throw AlreadyExistsException(
            "An event named '" + name + "' already exists in the EventSet.");

    d_events.emplace(name, new Event(name));
}

//----------------------------------------------------------------------------//
void EventSet::addEvent(Event& event)
{
    if (isEventPresent(event.getName()))
    {
        delete &event;

        throw AlreadyExistsException(
            "An event named '" + event.getName() + "' already exists in the EventSet.");
    }

    d_events.emplace(event.getName(), &event);
}

//----------------------------------------------------------------------------//
Event::Connection EventSet::subscribeScriptedEvent(const String& name,
                                                   const String& subscriber_name)
{
    return getScriptModule()->subscribeEvent(this, name, subscriber_name);
}

//----------------------------------------------------------------------------//
Event::Connection EventSet::subscribeScriptedEvent(const String& name,
                                                   Event::Group group,
                                                   const String& subscriber_name)
{
    return getScriptModule()->subscribeEvent(this, name, group, subscriber_name);
}

//----------------------------------------------------------------------------//
ScriptModule* EventSet::getScriptModule() const
{
    if (ScriptModule* sm = System::getSingletonPtr()->getScriptingModule())
        return sm;

    throw InvalidRequestException(
        "No scripting module is available.");
}

//----------------------------------------------------------------------------//
Event::Connection EventSet::subscribeEvent(const String& name,
                                           Event::Subscriber subscriber)
{
    return getEventObject(name, true)->subscribe(subscriber);
}

//----------------------------------------------------------------------------//
Event::Connection EventSet::subscribeEvent(const String& name,
                                           Event::Group group,
                                           Event::Subscriber subscriber)
{
    return getEventObject(name, true)->subscribe(group, subscriber);
}

//----------------------------------------------------------------------------//
void EventSet::unsubscribeAll(const String& eventName)
{
    if (Event* ev = getEventObject(eventName))
        ev->unsubscribeAll();
}

//----------------------------------------------------------------------------//
void EventSet::unsubscribeAll()
{
    for (auto& pair : d_events)
        pair.second->unsubscribeAll();
}

//----------------------------------------------------------------------------//
void EventSet::fireEvent(const String& name,
                         EventArgs& args,
                         const String& eventNamespace)
{
    if (GlobalEventSet* ges = GlobalEventSet::getSingletonPtr())
        ges->fireEvent(name, args, eventNamespace);

    fireEvent_impl(name, args);
}

//----------------------------------------------------------------------------//
Event* EventSet::getEventObject(const String& name, bool autoAdd)
{
    auto it = d_events.find(name);
    if (it != d_events.end())
        return it->second.get();

    return autoAdd ? d_events.emplace(name, new Event(name)).first->second.get() : nullptr;
}

//----------------------------------------------------------------------------//
void EventSet::fireEvent_impl(const String& name, EventArgs& args)
{
    if (!d_muted)
        if (Event* ev = getEventObject(name))
            (*ev)(args);
}

}
