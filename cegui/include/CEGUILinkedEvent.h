/***********************************************************************
    filename:   CEGUILinkedEvent.h
    created:    Sun Nov 21 2010
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUILinkedEvent_h
#define _CEGUILinkedEvent_h_

#include "CEGUIEvent.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    LinkedEvent is an Event subclass that provides a mechanism to link or chain
    Events together, such that when a specified target event is fired, then
    this event will additionally fire in response.
    \par
    The expected use of this mechanism is for container or compound widgets to
    be able to expose events of component widgets without needing to provide
    repetetive boilerplate event forwarding code, and also for scenarios where
    code is not typically provided (i.e. data based skinning scenarios).
*/
class CEGUIEXPORT LinkedEvent : public Event
{
public:
    LinkedEvent(const String& event_name, Event& source_event,
                EventSet* target_event_set);
    ~LinkedEvent();

protected:
    bool handler(const EventArgs& args);

    LinkedEvent(const LinkedEvent& e) : Event(e) {}

    //! our connection to the source Event.
    ScopedConnection d_con;
    //! reference to the event set that we added ourself to
    const EventSet* d_owner;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUILinkedEvent_h_

