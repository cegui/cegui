/***********************************************************************
    created:    1/3/2005
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/ScrolledContainer.h"
#include "CEGUI/CoordConverter.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4355)
#endif

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ScrolledContainer::WidgetTypeName("ScrolledContainer");
const String ScrolledContainer::EventNamespace("ScrolledContainer");

//----------------------------------------------------------------------------//
ScrolledContainer::ScrolledContainer(const String& type, const String& name) :
    Window(type, name),
    d_childContentArea(this, static_cast<Element::CachedRectf::DataGenerator>(&ScrolledContainer::getChildContentArea_impl))
{
    setCursorPassThroughEnabled(true); // Improves swipe scrolling experience in a ScrollablePane
    setCursorInputPropagationEnabled(true);
    setRiseOnCursorActivationEnabled(false);

    d_isWidthAdjustedToContent = true;
    d_isHeightAdjustedToContent = true;
    setSize(USize::zero());
}

//----------------------------------------------------------------------------//
void ScrolledContainer::adjustSizeToContent()
{
    if (!isSizeAdjustedToContent())
        return;

    const Rectf extents = getChildExtentsArea();

    USize size = getSize();
    if (isWidthAdjustedToContent())
    {
        d_contentOffset.x = extents.d_min.x;
        size.d_width = cegui_absdim(extents.getWidth());
    }
    if (isHeightAdjustedToContent())
    {
        d_contentOffset.y = extents.d_min.y;
        size.d_height = cegui_absdim(extents.getHeight());
    }

    setSize(size, false);
}

//----------------------------------------------------------------------------//
Rectf ScrolledContainer::getContentPixelRect() const
{
    return Rectf(d_contentOffset, d_pixelSize);
}

//----------------------------------------------------------------------------//
Rectf ScrolledContainer::getChildExtentsArea() const
{
    Rectf extents(0.f, 0.f, 0.f, 0.f);

    const size_t childCount = getChildCount();
    if (childCount == 0)
        return extents;

    Sizef baseSize = d_pixelSize;

    const auto& parentRect = d_parent->getChildContentArea().get();
    if (isWidthAdjustedToContent())
        baseSize.d_width = parentRect.getWidth();
    if (isHeightAdjustedToContent())
        baseSize.d_height = parentRect.getHeight();

    for (size_t i = 0; i < childCount; ++i)
    {
        const Window* const child = getChildAtIndex(i);
        Rectf area(
            CoordConverter::asAbsolute(child->getPosition(), baseSize),
            child->getPixelSize());

        if (child->getHorizontalAlignment() == HorizontalAlignment::Centre)
            area.setPosition(area.getPosition() - glm::vec2(area.getWidth() * 0.5f - baseSize.d_width * 0.5f, 0.0f));
        if (child->getVerticalAlignment() == VerticalAlignment::Centre)
            area.setPosition(area.getPosition() - glm::vec2(0.0f, area.getHeight() * 0.5f - baseSize.d_height * 0.5f));

        if (area.d_min.x < extents.d_min.x)
            extents.d_min.x = area.d_min.x;

        if (area.d_min.y < extents.d_min.y)
            extents.d_min.y = area.d_min.y;

        if (area.d_max.x > extents.d_max.x)
            extents.d_max.x = area.d_max.x;

        if (area.d_max.y > extents.d_max.y)
            extents.d_max.y = area.d_max.y;
    }

    return extents;
}

//----------------------------------------------------------------------------//
bool ScrolledContainer::handleChildAreaChanged(const EventArgs& /*e*/)
{
    adjustSizeToContent();
    return true;
}

//----------------------------------------------------------------------------//
void ScrolledContainer::subscribeOnChildAreaEvents(Window* child)
{
    d_childAreaChangeConnections.emplace(child,
        child->subscribeEvent(Window::EventSized,
            Event::Subscriber(&ScrolledContainer::handleChildAreaChanged, this)));
    d_childAreaChangeConnections.emplace(child,
        child->subscribeEvent(Window::EventMoved,
            Event::Subscriber(&ScrolledContainer::handleChildAreaChanged, this)));
}

//----------------------------------------------------------------------------//
void ScrolledContainer::onIsSizeAdjustedToContentChanged(ElementEventArgs& e)
{
    // Listen to child area changes only when auto-sizing is required
    const bool autoSize = isSizeAdjustedToContent();
    if (autoSize && d_childAreaChangeConnections.empty())
    {
        for (auto* child : d_children)
            subscribeOnChildAreaEvents(static_cast<Window*>(child));
    }
    else if (!autoSize && !d_childAreaChangeConnections.empty())
    {
        for (auto& windowToConnection : d_childAreaChangeConnections)
            windowToConnection.second->disconnect();
        d_childAreaChangeConnections.clear();
    }

    Window::onIsSizeAdjustedToContentChanged(e);
}

//----------------------------------------------------------------------------//
Rectf ScrolledContainer::getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const
{
    // When container size doesn't depend on child extents, child areas are relative
    // to the container itself. This allows for example to fill the whole container.
    // Autosized dimensions are calculated relative to container's parent instead.
    // This allows child items to scale with a viewport and make a single scrollbar pane.

    if (!d_parent || !isSizeAdjustedToContent())
        return Window::getUnclippedInnerRect_impl(skipAllPixelAlignment);

    const auto& parentRect =
        (skipAllPixelAlignment ? d_parent->getUnclippedInnerRect().getFresh(true) : d_parent->getUnclippedInnerRect().get());

    if (isWidthAdjustedToContent() && isHeightAdjustedToContent())
        return parentRect;

    Rectf result = Window::getUnclippedInnerRect_impl(skipAllPixelAlignment);
    if (isWidthAdjustedToContent())
    {
        result.d_min.x = parentRect.d_min.x;
        result.d_max.x = parentRect.d_max.x;
    }
    else if (isHeightAdjustedToContent())
    {
        result.d_min.y = parentRect.d_min.y;
        result.d_max.y = parentRect.d_max.y;
    }
    return result;
}

//----------------------------------------------------------------------------//
Rectf ScrolledContainer::getInnerRectClipper_impl() const
{
    return d_parent ? getParent()->getInnerRectClipper() : Window::getInnerRectClipper_impl();
}

//----------------------------------------------------------------------------//
Rectf ScrolledContainer::getHitTestRect_impl() const
{
    return d_parent ? getParent()->getHitTestRect() : Window::getHitTestRect_impl();
}

//----------------------------------------------------------------------------//
Rectf ScrolledContainer::getChildContentArea_impl(bool skipAllPixelAlignment) const
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
void ScrolledContainer::onChildAdded(ElementEventArgs& e)
{
    Window::onChildAdded(e);

    if (isSizeAdjustedToContent())
    {
        subscribeOnChildAreaEvents(static_cast<Window*>(e.element));
        adjustSizeToContent();
    }
}

//----------------------------------------------------------------------------//
void ScrolledContainer::onChildRemoved(ElementEventArgs& e)
{
    Window::onChildRemoved(e);

    // take a special care of children only if we're not being destroyed
    if (!d_destructionStarted)
    {
        // disconnect from events of this window
        auto range = d_childAreaChangeConnections.equal_range(static_cast<Window*>(e.element));
        for (auto it = range.first; it != range.second; ++it)
            it->second->disconnect();
        d_childAreaChangeConnections.erase(range.first, range.second);

        // recalculate pane size if auto-sized
        adjustSizeToContent();
    }
}

//----------------------------------------------------------------------------//
void ScrolledContainer::cleanupChildren(void)
{
    for (auto& windowToConnection : d_childAreaChangeConnections)
        windowToConnection.second->disconnect();
    d_childAreaChangeConnections.clear();

    Window::cleanupChildren();
}

//----------------------------------------------------------------------------//
uint8_t ScrolledContainer::handleAreaChanges(bool movedOnScreen, bool movedInParent, bool sized)
{
    // NB: inner rect is invalidated here because it depends on a parent and may
    // change even if our area didn't change. Window::handleAreaChanges doesn't
    // invalidate it in this case which may lead to inactual inner rect cache.
    d_childContentArea.invalidateCache();
    d_unclippedInnerRect.invalidateCache();
    return Window::handleAreaChanges(movedOnScreen, movedInParent, sized);
}

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
