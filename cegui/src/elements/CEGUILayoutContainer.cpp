/***********************************************************************
    filename:   CEGUILayoutContainer.cpp
    created:    29/7/2010
    author:     Martin Preisler

    purpose:    Implements the Layout Container base class
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "elements/CEGUILayoutContainer.h"
#include "CEGUIRenderingSurface.h"

// Start of CEGUI namespace section
namespace CEGUI
{

const String LayoutContainer::EventNamespace("LayoutContainer");

//----------------------------------------------------------------------------//
LayoutContainer::LayoutContainer(const String& type, const String& name):
        Window(type, name),

        d_needsLayouting(false)
{
    // layout should take the whole window by default I think
    setSize(USize(cegui_reldim(1), cegui_reldim(1)));

    subscribeEvent(Window::EventChildAdded,
                   Event::Subscriber(&LayoutContainer::handleChildAdded, this));
    subscribeEvent(Window::EventChildRemoved,
                   Event::Subscriber(&LayoutContainer::handleChildRemoved, this));
}

//----------------------------------------------------------------------------//
LayoutContainer::~LayoutContainer(void)
{}

//----------------------------------------------------------------------------//
void LayoutContainer::markNeedsLayouting()
{
    d_needsLayouting = true;

    //invalidate();
}

//----------------------------------------------------------------------------//
bool LayoutContainer::needsLayouting() const
{
    return d_needsLayouting;
}

//----------------------------------------------------------------------------//
void LayoutContainer::layoutIfNecessary()
{
    if (d_needsLayouting)
    {
        layout();

        d_needsLayouting = false;
    }
}

//----------------------------------------------------------------------------//
void LayoutContainer::update(float elapsed)
{
    Window::update(elapsed);

    layoutIfNecessary();
}

//----------------------------------------------------------------------------//
Rectf LayoutContainer::getUnclippedInnerRect_impl() const
{
    return d_parent ?
           d_parent->getUnclippedInnerRect() :
           Window::getUnclippedInnerRect_impl();
}

//----------------------------------------------------------------------------//
Rectf LayoutContainer::getClientChildWindowContentArea_impl() const
{
    if (!d_parent)
        return Window::getClientChildWindowContentArea_impl();
    else
        return Rectf(getUnclippedOuterRect().getPosition(),
                    d_parent->getUnclippedInnerRect().getSize());
}

//----------------------------------------------------------------------------//
size_t LayoutContainer::getIdxOfChild(Window* wnd) const
{
    for (size_t i = 0; i < getChildCount(); ++i)
    {
        if (getChildAtIdx(i) == wnd)
        {
            return i;
        }
    }

    assert(0);
    return 0;
}

//----------------------------------------------------------------------------//
void LayoutContainer::addChild_impl(Window* wnd)
{
    Window::addChild_impl(wnd);

    // we have to subscribe to the EventSized for layout updates
    d_eventConnections.insert(std::make_pair(wnd,
        wnd->subscribeEvent(Window::EventSized,
            Event::Subscriber(&LayoutContainer::handleChildSized, this))));
    d_eventConnections.insert(std::make_pair(wnd,
        wnd->subscribeEvent(Window::EventMarginChanged,
            Event::Subscriber(&LayoutContainer::handleChildMarginChanged, this))));
}

//----------------------------------------------------------------------------//
void LayoutContainer::removeChild_impl(Window* wnd)
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
bool LayoutContainer::handleChildSized(const EventArgs& e)
{
    markNeedsLayouting();

    return true;
}

//----------------------------------------------------------------------------//
bool LayoutContainer::handleChildMarginChanged(const EventArgs& e)
{
    markNeedsLayouting();

    return true;
}

//----------------------------------------------------------------------------//
bool LayoutContainer::handleChildAdded(const EventArgs& e)
{
    markNeedsLayouting();

    return true;
}

//----------------------------------------------------------------------------//
bool LayoutContainer::handleChildRemoved(const EventArgs& e)
{
    markNeedsLayouting();

    return true;
}

//----------------------------------------------------------------------------//
UVector2 LayoutContainer::getOffsetForWindow(Window* window) const
{
    const UBox& margin = window->getMargin();

    return UVector2(
               margin.d_left,
               margin.d_top
           );
}

//----------------------------------------------------------------------------//
UVector2 LayoutContainer::getBoundingSizeForWindow(Window* window) const
{
    const Sizef& pixelSize = window->getPixelSize();

    // we rely on pixelSize rather than mixed absolute and relative getSize
    // this seems to solve problems when windows overlap because their size
    // is constrained by min size
    const UVector2 size(UDim(0, pixelSize.d_width), UDim(0, pixelSize.d_height));
    // todo: we still do mixed absolute/relative margin, should we convert the
    //       value to absolute?
    const UBox& margin = window->getMargin();

    return UVector2(
               margin.d_left + size.d_x + margin.d_right,
               margin.d_top + size.d_y + margin.d_bottom
           );
}

} // End of  CEGUI namespace section

