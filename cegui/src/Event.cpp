/************************************************************************
    created:    Tue Feb 28 2006
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/Event.h"
#include "CEGUI/EventArgs.h"

#include <algorithm>

namespace CEGUI
{

//----------------------------------------------------------------------------//
Event::Event(const String& name) :
    d_name(name)
{
}

//----------------------------------------------------------------------------//
Event::~Event()
{
    unsubscribeAll();
}

//----------------------------------------------------------------------------//
Event::Connection Event::subscribe(const Event::Subscriber& slot)
{
    return subscribe(static_cast<Group>(-1), slot);
}

//----------------------------------------------------------------------------//
Event::Connection Event::subscribe(Event::Group group, const Event::Subscriber& slot)
{
    Event::Connection c(new BoundSlot(group, slot, *this));
    d_slots.insert(std::pair<Group, Connection>(group, c));
    return c;
}

//----------------------------------------------------------------------------//
void Event::unsubscribeAll()
{
    for (auto& groupAndSlot : d_slots)
    {
        // Make all subscribed slots invalid because we can't control their lifetime
        groupAndSlot.second->d_event = nullptr;
        groupAndSlot.second->d_subscriber->cleanup();

        if (d_isBeingInvoked)
            groupAndSlot.second = nullptr;
    }

    if (!d_isBeingInvoked)
        d_slots.clear();
}

//----------------------------------------------------------------------------//
void Event::operator()(EventArgs& args)
{
    d_isBeingInvoked = true;

    // Execute all subscribers, updating the 'handled' state as we go.
    // Erase unsubscribed slots.
    const auto itLast = d_slots.cend();
    for (auto it = d_slots.begin(); it != itLast; )
    {
        // Hold a strong reference to prevent self-destruction
        Connection curr = it->second;

        // Call the handler
        if (curr && (*curr->d_subscriber)(args))
            ++args.handled;

        // Erase if unsubscribed
        if (!it->second)
            it = d_slots.erase(it);
        else
            ++it;
    }

    d_isBeingInvoked = false;
}

//----------------------------------------------------------------------------//
void Event::unsubscribe(const BoundSlot& slot)
{
    // Try to find the slot in our collection
    auto it = std::find_if(d_slots.begin(), d_slots.end(), [&slot](const std::pair<const Group, Connection>& groupAndSlot)
    {
        return (*groupAndSlot.second) == slot;
    });

    // Erase our reference to the slot, if we had one.
    // Delay erasing if we are in the middle of the invocation loop.
    if (it != d_slots.end())
    {
        if (d_isBeingInvoked)
            it->second = nullptr;
        else
            d_slots.erase(it);
    }
}

//----------------------------------------------------------------------------//

}
