/************************************************************************
	filename: 	CEGUIEventSet.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the EventSet class
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
#include "CEGUIEventSet.h"
#include "CEGUIExceptions.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor
*************************************************************************/
EventSet::EventSet(void)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
EventSet::~EventSet(void)
{
	removeAllEvents();
}


/*************************************************************************
	Add a new event to the EventSet
*************************************************************************/
void EventSet::addEvent(const String& name)
{
	if (isEventPresent(name))
	{
		throw AlreadyExistsException("An event named '" + name + "' already exists in the EventSet.");
	}

	d_events[name] = new Event(name);
}


/*************************************************************************
	Remove an event from the EventSet
*************************************************************************/
void EventSet::removeEvent(const String& name)
{
	EventMap::iterator pos = d_events.find(name);

	if (pos != d_events.end())
	{
		delete pos->second;
		d_events.erase(pos);
	}

}


/*************************************************************************
	Remove all events from the EventSet
*************************************************************************/
void EventSet::removeAllEvents(void)
{
	EventMap::iterator pos = d_events.begin();
	EventMap::iterator end = d_events.end()	;

	for (; pos != end; ++pos)
	{
		delete pos->second;
	}

	d_events.clear();
}


/*************************************************************************
	Check to see if an event is available
*************************************************************************/
bool EventSet::isEventPresent(const String& name)
{
	return (d_events.find(name) != d_events.end());
}


/*************************************************************************
	Subscribe to an event (no group)
*************************************************************************/
Event::Connection EventSet::subscribeEvent(const String& name, Event::Subscriber subscriber)
{
	EventMap::iterator pos = d_events.find(name);

	if (pos == d_events.end())
	{
		throw UnknownObjectException("No event named '" + name + "' is defined for this EventSet");	
	}

	return pos->second->subscribe(subscriber);
}


/*************************************************************************
	Subscribe to an event group
*************************************************************************/
Event::Connection EventSet::subscribeEvent(const String& name, Event::Group group, Event::Subscriber subscriber)
{
	EventMap::iterator pos = d_events.find(name);

	if (pos == d_events.end())
	{
		throw UnknownObjectException("No event named '" + name + "' is defined for this EventSet");	
	}

	return pos->second->subscribe(group, subscriber);
}

/*************************************************************************
	Fire / Trigger an event
*************************************************************************/
void EventSet::fireEvent(const String& name, const EventArgs& args)
{
	EventMap::iterator pos = d_events.find(name);

	if (pos == d_events.end())
	{
		throw UnknownObjectException("No event named '" + name + "' is defined for this EventSet");	
	}

	// fire the event
	(*pos->second)(args);
}

} // End of  CEGUI namespace section
