/***********************************************************************
    filename:   CEGUILayoutCell.cpp
    created:    22/2/2011
    author:     Martin Preisler

    purpose:    Implements the LayoutCell class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "elements/CEGUILayoutCell.h"

// Start of CEGUI namespace section
namespace CEGUI
{

const String LayoutCell::EventNamespace("LayoutCell");
const String LayoutCell::WidgetTypeName("LayoutCell");

//----------------------------------------------------------------------------//
LayoutCell::LayoutCell(const String& type, const String& name):
        Window(type, name)
{
    // cell should take the whole window by default I think
    setSize(UVector2(cegui_reldim(1), cegui_reldim(1)));

    subscribeEvent(Window::EventChildAdded,
                   Event::Subscriber(&LayoutCell::handleChildAdded, this));
    subscribeEvent(Window::EventChildRemoved,
                   Event::Subscriber(&LayoutCell::handleChildRemoved, this));
}

//----------------------------------------------------------------------------//
LayoutCell::~LayoutCell(void)
{}

//----------------------------------------------------------------------------//
Rectf LayoutCell::getUnclippedInnerRect_impl(void) const
{
    return d_parent ?
           d_parent->getUnclippedInnerRect() :
           Window::getUnclippedInnerRect_impl();
}

//----------------------------------------------------------------------------//
Rectf LayoutCell::getClientChildWindowContentArea_impl() const
{
    if (!d_parent)
        return Window::getClientChildWindowContentArea_impl();
    else
        return Rectf(getUnclippedOuterRect().getPosition(),
                    d_parent->getUnclippedInnerRect().getSize());
}

//----------------------------------------------------------------------------//
void LayoutCell::addChild_impl(Window* wnd)
{
    Window::addChild_impl(wnd);

    // we have to subscribe to the EventSized for layout updates
    d_eventConnections.insert(std::make_pair(wnd,
        wnd->subscribeEvent(Window::EventSized,
            Event::Subscriber(&LayoutCell::handleChildSized, this))));
}

//----------------------------------------------------------------------------//
void LayoutCell::removeChild_impl(Window* wnd)
{
    // we want to get rid of the subscription, because the child window could
    // get removed and added somewhere else, we would be wastefully updating
    // layouts if it was sized inside other Window
    ConnectionTracker::iterator conn;

    while ((conn = d_eventConnections.find(wnd)) != d_eventConnections.end())
    {
        conn->second->disconnect();
        d_eventConnections.erase(conn);
    }

    Window::removeChild_impl(wnd);
}

//----------------------------------------------------------------------------//
bool LayoutCell::handleChildSized(const EventArgs& e)
{
    //markNeedsLayouting();

    return true;
}

//----------------------------------------------------------------------------//
bool LayoutCell::handleChildAdded(const EventArgs& e)
{
    if (getChildCount() > 0)
    {
        throw InvalidRequestException("You can't add more than one widget to a layout cell!");
    }

    //markNeedsLayouting();

    return true;
}

//----------------------------------------------------------------------------//
bool LayoutCell::handleChildRemoved(const EventArgs& e)
{
    //markNeedsLayouting();

    return true;
}

} // End of  CEGUI namespace section
