/************************************************************************
	filename: 	CEGUIEvent.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for Event class
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
#ifndef _CEGUIEvent_h_
#define _CEGUIEvent_h_

// setup auto-link for boost.signals (works only with _MSC_EXTENSIONS on and with MSCV or Borland compilers, so they say...)
// if this can't be used obviously you need to link with the appropriate version of the library manually.
#define BOOST_LIB_NAME	boost_signals
#include "boost/config/auto_link.hpp"

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#	if !defined (_MSC_EXTENSIONS)
#		pragma warning (disable : 4224)
#	endif
#endif


#if defined(_MSC_VER)
#endif


#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIEventArgs.h"
#include "boost/signals.hpp"



// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Defines an 'event' which can be subscribed to by interested parties.

	An Event can be subscribed by a function, a static member function, or a function object.  Whichever option
	is taken, the function signature needs to be as follows
	\par
	<em>void function_name(const EventArgs& args);</em>
	\note
		Currently, this class is effectively a thin wrapper around boost::signals.  You need to have boost::signals
		available to be able to compile and use this class.  Note also that this may not always be the case, so use
		the interface defined here unless you want something to break in a later update.
	\note
		An Event object may not be copied.
*/
class CEGUIBASE_API Event
{
public:
	typedef	boost::signal1<void, const EventArgs&>	Signal;			//!< Represents the internal signal object
	typedef const Signal::slot_type					Subscriber;		//!< Function / object that can subscribe to an Event
	typedef Signal::group_type						Group;			//!< Group for subscription (groups are called in ascending sequence)
	typedef boost::signals::connection				Connection;		//!< An object that is returned from subscribe function that can be used to control the connection / subscription.

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructs a new Event object with the specified name
	*/
	Event(const String& name) : d_name(name) {}

	/*!
	\brief
		Destructor for Event objects
	*/
	virtual ~Event(void) {}


	/*!
	\brief
		Return the name given to this Event object when it was created.

	\return
		String object containing the name of the Event object.
	*/
	const String& getName(void) const	{return d_name;}


	/*!
	\brief
		Subscribes some function / object to the Event

	\param subscriber
		A function, static member function, or function object, with the signature void function_name(const EventArgs& args)

	\return
		A Connection which can be used to disconnect (unsubscribe) from the Event, and to check the connection state.
	*/
	Connection	subscribe(Subscriber& subscriber)	{return d_sig.connect(subscriber);}


	/*!
	\brief
		Subscribes some function / object to the Event

	\param group
		The Event group to subscribe to, subscription groups are called in ascending order, followed by subscriptions with no group.
		connections to the same group may be called in any order.

	\param subscriber
		A function, static member function, or function object, with the signature void function_name(const EventArgs& args)

	\return
		A Connection which can be used to disconnect (unsubscribe) from the event, and to check the connection state.
	*/
	Connection	subscribe(Group group, Subscriber subscriber)	{return d_sig.connect(group, subscriber);}


	/*
	\brief
		Fires the event.  All event subscribers get called in the appropriate sequence.

	\param args
		An object derived from EventArgs to be passed to each event subscriber.

	\return
		Nothing.
	*/
	void	operator()(const EventArgs& args)		{d_sig(args);}

private:
	/*************************************************************************
		Copy constructor and assignment are not allowed for events
	*************************************************************************/
	Event(const Event& evt)	{}
	Event& operator=(const Event& evt)	{return *this;}

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Signal	d_sig;				//!< boost::signal that implements the signal/slot functionality
	const String	d_name;		//!< Name of this event
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIEvent_h_
