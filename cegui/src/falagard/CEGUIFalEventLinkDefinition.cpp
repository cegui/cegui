/***********************************************************************
    filename:   CEGUIFalEventLinkDefinition.cpp
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
#include "falagard/CEGUIFalEventLinkDefinition.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowManager.h"
#include "CEGUILinkedEvent.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// Static string holding parent link identifier
static const String S_parentIdentifier("__parent__");

//----------------------------------------------------------------------------//
EventLinkDefinition::EventLinkDefinition(const String& event_name) :
    d_eventName(event_name)
{
}

//----------------------------------------------------------------------------//
EventLinkDefinition::~EventLinkDefinition()
{
}

//----------------------------------------------------------------------------//
void EventLinkDefinition::addLinkTarget(const String& widget,
                                        const String& event)
{
    const Target t = { widget, event };
    d_targets.push_back(t);
}

//----------------------------------------------------------------------------//
void EventLinkDefinition::clearLinkTargets()
{
    d_targets.clear();
}

//----------------------------------------------------------------------------//
void EventLinkDefinition::initialiseWidget(Window& window) const
{
    LinkedEvent* e = CEGUI_NEW_AO LinkedEvent(d_eventName, &window);

    LinkTargetCollection::const_iterator i = d_targets.begin();
    for ( ; i != d_targets.end(); ++i)
    {
        Window* target = getTargetWindow(window, (*i).d_widgetName);

        if (target)
            e->addLinkedTarget(*target->getEventObject((*i).d_targetEvent,true));
    }
}

//----------------------------------------------------------------------------//
void EventLinkDefinition::cleanUpWidget(Window& window) const
{
    window.removeEvent(d_eventName);
}

//----------------------------------------------------------------------------//
const String& EventLinkDefinition:: getName() const
{
    return d_eventName;
}

//----------------------------------------------------------------------------//
Window* EventLinkDefinition::getTargetWindow(Window& start_wnd,
                                             const String& name) const
{
    if (name.empty())
        return &start_wnd;

    if (name== S_parentIdentifier)
        return start_wnd.getParent();

    return start_wnd.getChild(name);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

