/***********************************************************************
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

#include "CEGUI/widgets/LayoutContainer.h"

namespace CEGUI
{
const String LayoutContainer::EventNamespace("LayoutContainer");

//----------------------------------------------------------------------------//
LayoutContainer::LayoutContainer(const String& type, const String& name):
    Window(type, name),
    d_needsLayouting(false),
    d_childContentArea(this, static_cast<Element::CachedRectf::DataGenerator>(&LayoutContainer::getChildContentArea_impl))
{
    // layout should take the whole window by default I think
    setSize(USize(cegui_reldim(1), cegui_reldim(1)));

    EventSet::subscribeEvent(Window::EventChildAdded,
                   Event::Subscriber(&LayoutContainer::handleChildAdded, this));
    EventSet::subscribeEvent(Window::EventChildRemoved,
                   Event::Subscriber(&LayoutContainer::handleChildRemoved, this));
}

//----------------------------------------------------------------------------//
void LayoutContainer::layoutIfNecessary()
{
    if (d_needsLayouting)
    {
        // Sometimes layout_impl() triggers subsequent layouting, for example when
        // the final size calculation changes parent ScrollablePane scrollbars.
        // This is why the flag is cleared before layouting, not after it.
        d_needsLayouting = false;
        layout_impl();
    }
}

//----------------------------------------------------------------------------//
void LayoutContainer::update(float elapsed)
{
    Window::update(elapsed);
    layoutIfNecessary();
}

//----------------------------------------------------------------------------//
uint8_t LayoutContainer::handleAreaChanges(bool movedOnScreen, bool movedInParent, bool sized)
{
    // NB: inner rect is invalidated here because it depends on a parent and may
    // change even if our area didn't change. Window::handleAreaChanges doesn't
    // invalidate it in this case which may lead to inactual inner rect cache.
    d_childContentArea.invalidateCache();
    d_unclippedInnerRect.invalidateCache();
    const uint8_t flags = Window::handleAreaChanges(movedOnScreen, movedInParent, sized);

    // If content areas resized, relatively sized children may break layout
    if (flags & (ClientSized | NonClientSized))
        markNeedsLayouting();

    return flags;
}

//----------------------------------------------------------------------------//
Rectf LayoutContainer::getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const
{
    if (!d_parent)
        return Window::getUnclippedInnerRect_impl(skipAllPixelAlignment);

    return skipAllPixelAlignment ?
        d_parent->getUnclippedInnerRect().getFresh(true) :
        d_parent->getUnclippedInnerRect().get();
}

//----------------------------------------------------------------------------//
Rectf LayoutContainer::getChildContentArea_impl(bool skipAllPixelAlignment) const
{
    if (!d_parent)
    {
        return skipAllPixelAlignment ? Window::getChildContentArea(false).getFresh(true) : Window::getChildContentArea(false).get();
    }
    else
    {
        return skipAllPixelAlignment ?
            Rectf(getUnclippedOuterRect().getFresh(true).getPosition(), d_parent->getUnclippedInnerRect().getFresh(true).getSize()) :
            Rectf(getUnclippedOuterRect().get().getPosition(), d_parent->getUnclippedInnerRect().get().getSize());
    }
}

//----------------------------------------------------------------------------//
void LayoutContainer::addChild_impl(Element* element)
{
    Window::addChild_impl(element);

    Window* wnd = static_cast<Window*>(element);

    // we have to subscribe to the EventSized for layout updates
    d_eventConnections.insert(std::make_pair(wnd,
        wnd->subscribeEvent(Window::EventSized,
            Event::Subscriber(&LayoutContainer::handleChildSized, this))));
    d_eventConnections.insert(std::make_pair(wnd,
        wnd->subscribeEvent(Window::EventMarginChanged,
            Event::Subscriber(&LayoutContainer::handleChildMarginChanged, this))));
}

//----------------------------------------------------------------------------//
void LayoutContainer::removeChild_impl(Element* element)
{
    if (!d_destructionStarted)
    {
        // we want to get rid of the subscription, because the child window could
        // get removed and added somewhere else, we would be wastefully updating
        // layouts if it was sized inside other Window
        auto range = d_eventConnections.equal_range(static_cast<Window*>(element));
        for (auto it = range.first; it != range.second; ++it)
            it->second->disconnect();
        d_eventConnections.erase(range.first, range.second);
    }

    Window::removeChild_impl(element);
}

//----------------------------------------------------------------------------//
void LayoutContainer::cleanupChildren(void)
{
    for (auto& windowToConnection : d_eventConnections)
        windowToConnection.second->disconnect();
    d_eventConnections.clear();

    Window::cleanupChildren();
}

//----------------------------------------------------------------------------//
bool LayoutContainer::handleChildSized(const EventArgs&)
{
    markNeedsLayouting();
    return true;
}

//----------------------------------------------------------------------------//
bool LayoutContainer::handleChildMarginChanged(const EventArgs&)
{
    markNeedsLayouting();
    return true;
}

//----------------------------------------------------------------------------//
bool LayoutContainer::handleChildAdded(const EventArgs&)
{
    markNeedsLayouting();
    return true;
}

//----------------------------------------------------------------------------//
bool LayoutContainer::handleChildRemoved(const EventArgs&)
{
    markNeedsLayouting();
    return true;
}

//----------------------------------------------------------------------------//
UVector2 LayoutContainer::getOffsetForWindow(Window* window) const
{
    const UBox& margin = window->getMargin();
    return UVector2(margin.d_left, margin.d_top);
}

//----------------------------------------------------------------------------//
UVector2 LayoutContainer::getBoundingSizeForWindow(Window* window) const
{
    // we rely on pixelSize rather than mixed absolute and relative getSize
    // this seems to solve problems when windows overlap because their size
    // is constrained by min size
    // TODO: we still do mixed absolute/relative margin, should we convert the
    //       value to absolute?
    const Sizef& pixelSize = window->getPixelSize();
    const UBox& margin = window->getMargin();
    return UVector2(
               margin.d_left + UDim(0, pixelSize.d_width) + margin.d_right,
               margin.d_top + UDim(0, pixelSize.d_height) + margin.d_bottom);
}

//----------------------------------------------------------------------------//
void LayoutContainer::onChildOrderChanged(ElementEventArgs& e)
{
    markNeedsLayouting();
    Window::onChildOrderChanged(e);
}

//----------------------------------------------------------------------------//

}
