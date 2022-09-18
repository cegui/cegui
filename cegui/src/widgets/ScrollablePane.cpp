/***********************************************************************
    created:    1/3/2005
    author:     Paul D Turner
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/ScrollablePane.h"
#include "CEGUI/widgets/ScrolledContainer.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/WindowManager.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ScrollablePane::WidgetTypeName("CEGUI/ScrollablePane");
const String ScrollablePane::EventNamespace("ScrollablePane");
const String ScrollablePane::EventContentPaneChanged("ContentPaneChanged");
const String ScrollablePane::EventVertScrollbarModeChanged("VertScrollbarModeChanged");
const String ScrollablePane::EventHorzScrollbarModeChanged("HorzScrollbarModeChanged");
const String ScrollablePane::EventContentPaneScrolled("ContentPaneScrolled");
const String ScrollablePane::VertScrollbarName( "__auto_vscrollbar__" );
const String ScrollablePane::HorzScrollbarName( "__auto_hscrollbar__" );
const String ScrollablePane::ScrolledContainerName( "__auto_container__" );
//----------------------------------------------------------------------------//
ScrollablePaneWindowRenderer::ScrollablePaneWindowRenderer(const String& name) :
WindowRenderer(name, ScrollablePane::EventNamespace)
{
}

//----------------------------------------------------------------------------//
ScrollablePane::ScrollablePane(const String& type, const String& name) :
    Window(type, name),
    d_forceVertScroll(false),
    d_forceHorzScroll(false),
    d_swipeScrollingEnabled(false),
    d_contentRect(0.f, 0.f, 0.f, 0.f),
    d_vertStep(0.1f),
    d_vertOverlap(0.01f),
    d_horzStep(0.1f),
    d_horzOverlap(0.01f)
{
    addScrollablePaneProperties();
    
    // create scrolled container widget
    ScrolledContainer* container = static_cast<ScrolledContainer*>(
        WindowManager::getSingleton().createWindow(
            ScrolledContainer::WidgetTypeName,
            ScrolledContainerName));
    container->setAutoWindow(true);

    // Size adjustment flags are synchronized with ones of the container.
    // ScrollablePane's own values have no meaning, at least for now.
    d_isWidthAdjustedToContent = container->isWidthAdjustedToContent();
    d_isHeightAdjustedToContent = container->isHeightAdjustedToContent();

    // add scrolled container widget as child
    addChild(container);
}

//----------------------------------------------------------------------------//
ScrollablePane::~ScrollablePane(void)
{}

//----------------------------------------------------------------------------//
const ScrolledContainer* ScrollablePane::getContentPane(void) const
{
    return getScrolledContainer();
}

//----------------------------------------------------------------------------//
Rectf ScrollablePane::getContentPixelRect(void) const
{
    return getScrolledContainer()->getContentPixelRect();
}

//----------------------------------------------------------------------------//
USize ScrollablePane::getContentPaneSize(void) const
{
    return getScrolledContainer()->getSize();
}

//----------------------------------------------------------------------------//
void ScrollablePane::setContentPaneSize(const USize& size)
{
    getScrolledContainer()->setSize(size);
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isVerticalScrollbarAlwaysShown() const
{
    return d_forceVertScroll;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setAlwaysShowVerticalScrollbar(bool setting)
{
    if (d_forceVertScroll != setting)
    {
        d_forceVertScroll = setting;
        
        configureScrollbars();
        WindowEventArgs args(this);
        onVerticalScrollbarModeChanged(args);
    }
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isHorizontalScrollbarAlwaysShown(void) const
{
    return d_forceHorzScroll;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setAlwaysShowHorizontalScrollbar(bool setting)
{
    if (d_forceHorzScroll != setting)
    {
        d_forceHorzScroll = setting;
        
        configureScrollbars();
        WindowEventArgs args(this);
        onHorizontalScrollbarModeChanged(args);
    }
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isSwipeScrollingEnabled() const
{
    return d_swipeScrollingEnabled;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setSwipeScrollingEnabled(bool setting)
{
    d_swipeScrollingEnabled = setting;

    if (!d_swipeScrollingEnabled && d_swiping)
        releaseInput();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getHorizontalStepSize(void) const
{
    return d_horzStep;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setHorizontalStepSize(float step)
{
    d_horzStep = step;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getHorizontalOverlapSize(void) const
{
    return d_horzOverlap;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setHorizontalOverlapSize(float overlap)
{
    d_horzOverlap = overlap;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getHorizontalScrollPosition(void) const
{
    return getHorzScrollbar()->getUnitIntervalScrollPosition();
}

//----------------------------------------------------------------------------//
void ScrollablePane::setHorizontalScrollPosition(float position)
{
    getHorzScrollbar()->setUnitIntervalScrollPosition(position);
}

//----------------------------------------------------------------------------//
float ScrollablePane::getVerticalStepSize(void) const
{
    return d_vertStep;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setVerticalStepSize(float step)
{
    d_vertStep = step;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getVerticalOverlapSize(void) const
{
    return d_vertOverlap;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setVerticalOverlapSize(float overlap)
{
    d_vertOverlap = overlap;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getVerticalScrollPosition(void) const
{
    return getVertScrollbar()->getUnitIntervalScrollPosition();
}

//----------------------------------------------------------------------------//
void ScrollablePane::setVerticalScrollPosition(float position)
{
    getVertScrollbar()->setUnitIntervalScrollPosition(position);
}

//----------------------------------------------------------------------------//
void ScrollablePane::initialiseComponents()
{
    // get horizontal scrollbar
    Scrollbar* horzScrollbar = getHorzScrollbar();
    
    // get vertical scrollbar
    Scrollbar* vertScrollbar = getVertScrollbar();
    
    // get scrolled container widget
    ScrolledContainer* container = getScrolledContainer();
    
    // ban properties forwarded from here
    container->banPropertyFromXML(Window::CursorInputPropagationEnabledPropertyName);
    container->banPropertyFromXML("AdjustWidthToContent");
    container->banPropertyFromXML("AdjustHeightToContent");
    horzScrollbar->banPropertyFromXML(Window::AlwaysOnTopPropertyName);
    vertScrollbar->banPropertyFromXML(Window::AlwaysOnTopPropertyName);

    // do a bit of initialisation
    horzScrollbar->setAlwaysOnTop(true);
    vertScrollbar->setAlwaysOnTop(true);
    container->setCursor(getEffectiveCursor());

    // subscribe to events we need to hear about
    vertScrollbar->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&ScrollablePane::handleScrollChange, this));

    horzScrollbar->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&ScrollablePane::handleScrollChange, this));

    d_contentChangedConn = container->subscribeEvent(
            ScrolledContainer::EventSized,
            Event::Subscriber(&ScrollablePane::handleContentAreaChange, this));

    d_autoSizeChangedConn = container->subscribeEvent(
            ScrolledContainer::EventIsSizeAdjustedToContentChanged,
            Event::Subscriber(&ScrollablePane::handleAutoSizePaneChanged, this));
    
    // finalise setup
    configureScrollbars();

    Window::initialiseComponents();
}

//----------------------------------------------------------------------------//
void ScrollablePane::configureScrollbars()
{
    // controls should all be valid by this stage
    Scrollbar* const vertScrollbar = getVertScrollbar();
    Scrollbar* const horzScrollbar = getHorzScrollbar();

    // update vertical scrollbar state
    {
        const bool show = d_forceVertScroll ||
            (getScrolledContainer()->getPixelSize().d_height > getViewableArea().getHeight());

        if (vertScrollbar->isVisible() != show)
        {
            vertScrollbar->setVisible(show);
            notifyScreenAreaChanged();
        }
    }

    // update horizontal scrollbar state
    const bool wasVisibleHorz = horzScrollbar->isVisible();
    {
        const bool show = d_forceHorzScroll ||
            (getScrolledContainer()->getPixelSize().d_width > getViewableArea().getWidth());

        if (wasVisibleHorz != show)
        {
            horzScrollbar->setVisible(show);
            notifyScreenAreaChanged();
        }
    }

    // change in a horizontal scrollbar state might affect viewable area,
    // so update vertical scrollbar state again
    if (wasVisibleHorz != horzScrollbar->isVisible())
    {
        const bool show = d_forceVertScroll ||
            (getScrolledContainer()->getPixelSize().d_height > getViewableArea().getHeight());

        if (vertScrollbar->isVisible() != show)
        {
            vertScrollbar->setVisible(show);
            notifyScreenAreaChanged();
        }
    }
    
    // get viewable area
    const Rectf viewableArea(getViewableArea());
    const auto& paneSize = getScrolledContainer()->getPixelSize();
    
    // set up vertical scroll bar values
    vertScrollbar->setDocumentSize(paneSize.d_height);
    vertScrollbar->setPageSize(viewableArea.getHeight());
    vertScrollbar->setStepSize(std::max(1.0f, viewableArea.getHeight() * d_vertStep));
    vertScrollbar->setOverlapSize(std::max(1.0f, viewableArea.getHeight() * d_vertOverlap));
    vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition());
    
    // set up horizontal scroll bar values
    horzScrollbar->setDocumentSize(paneSize.d_width);
    horzScrollbar->setPageSize(viewableArea.getWidth());
    horzScrollbar->setStepSize(std::max(1.0f, viewableArea.getWidth() * d_horzStep));
    horzScrollbar->setOverlapSize(std::max(1.0f, viewableArea.getWidth() * d_horzOverlap));
    horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition());
}

//----------------------------------------------------------------------------//
void ScrollablePane::scrollContentPane(float dx, float dy, ScrollablePane::ScrollSource /*source*/)
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

    if (dy != 0.f && vertScrollbar->isEffectiveVisible() &&
        (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
    {
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + dy);
    }

    if (dx != 0.f && horzScrollbar->isEffectiveVisible() &&
        (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
    {
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + dx);
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::updateContainerPosition(void)
{
    // basePos is the position represented by the scrollbars
    // (these are negated so pane is scrolled in the correct directions)
    UVector2 basePos(cegui_absdim(-getHorzScrollbar()->getScrollPosition()),
                     cegui_absdim(-getVertScrollbar()->getScrollPosition()));
    
    // this bias is the absolute position that 0 on the scrollbars represent.
    // Allows the pane to function correctly with negatively positioned content.
    UVector2 bias(cegui_absdim(d_contentRect.d_min.x),
                  cegui_absdim(d_contentRect.d_min.y));
    
    // set the new container pane position to be what the scrollbars request
    // minus any bias generated by the location of the content.
    getScrolledContainer()->setPosition(basePos - bias);
}

//----------------------------------------------------------------------------//
bool ScrollablePane::validateWindowRenderer(const WindowRenderer* renderer) const
{
    return dynamic_cast<const ScrollablePaneWindowRenderer*>(renderer) != nullptr;
}

//----------------------------------------------------------------------------//
void ScrollablePane::onContentPaneChanged(WindowEventArgs& e)
{
    fireEvent(EventContentPaneChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onVerticalScrollbarModeChanged(WindowEventArgs& e)
{
    fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onHorizontalScrollbarModeChanged(WindowEventArgs& e)
{
    fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onContentPaneScrolled(WindowEventArgs& e)
{
    updateContainerPosition();
    fireEvent(EventContentPaneScrolled, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool ScrollablePane::handleScrollChange(const EventArgs&)
{
    WindowEventArgs args(this);
    onContentPaneScrolled(args);
    return true;
}

//----------------------------------------------------------------------------//
bool ScrollablePane::handleContentAreaChange(const EventArgs&)
{
    // prevent recursion due to shown/hidden scrollbars
    // FIXME: can't unsubscribe inside a handler! add a method to connection for temporary disabling?
    //d_contentChangedConn->disconnect();
    if (d_suspendContentChangedConn)
        return false;
    d_suspendContentChangedConn = true;
    
    configureScrollbars();

    // get updated extents of the content
    const Rectf contentRect(getScrolledContainer()->getContentPixelRect());

    // calculate any change on the top and left edges.
    const float xChange = contentRect.d_min.x - d_contentRect.d_min.x;
    const float yChange = contentRect.d_min.y - d_contentRect.d_min.y;

    // store new content extents information
    d_contentRect = contentRect;

    // update scrollbar positions (which causes container pane to be moved as needed).
    Scrollbar* const horzScrollbar = getHorzScrollbar();
    horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() - xChange);
    Scrollbar* const vertScrollbar = getVertScrollbar();
    vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() - yChange);
    
    // this call may already have been made if the scroll positions changed. The call
    // is required here for cases where the top/left 'bias' has changed; in which
    // case the scroll position notification may or may not have been fired.
    if (xChange || yChange)
        updateContainerPosition();

    // restore monitoring of the pane size
    // FIXME: can't unsubscribe inside a handler! add a method to connection for temporary disabling?
    //d_contentChangedConn = getScrolledContainer()->subscribeEvent(
    //    ScrolledContainer::EventSized,
    //    Event::Subscriber(&ScrollablePane::handleContentAreaChange, this));
    d_suspendContentChangedConn = false;

    // fire event
    WindowEventArgs args(this);
    onContentPaneChanged(args);
    
    return true;
}

//----------------------------------------------------------------------------//
bool ScrollablePane::handleAutoSizePaneChanged(const EventArgs&)
{
    // sync properties with ones of the container
    auto container = getScrolledContainer();
    d_isWidthAdjustedToContent = container->isWidthAdjustedToContent();
    d_isHeightAdjustedToContent = container->isHeightAdjustedToContent();

    // forward event to client.
    WindowEventArgs args(this);
    fireEvent(EventIsSizeAdjustedToContentChanged, args, EventNamespace);
    return args.handled > 0;
}

//----------------------------------------------------------------------------//
void ScrollablePane::addChild_impl(Element* element)
{
    Window* wnd = dynamic_cast<Window*>(element);
    
    if (!wnd)
        throw InvalidRequestException(
            "ScrollablePane can only have Elements of "
            "type Window added as children (Window path: " +
            getNamePath() + ").");
    
    if (wnd->isAutoWindow())
    {
        // This is an internal widget, so should be added normally.
        Window::addChild_impl(wnd);
    }
    // this is a client window/widget, so should be added to the pane container.
    else
    {
        // container should always be valid by the time we're adding client
        // controls
        getScrolledContainer()->addChild(wnd);
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::removeChild_impl(Element* element)
{
    Window* wnd = static_cast<Window*>(element);
    
    if (wnd->isAutoWindow())
    {
        // This is an internal widget, so should be removed normally.
        Window::removeChild_impl(wnd);
    }
    // this is a client window/widget, so should be removed from the pane
    // container.
    else
    {
        // container should always be valid by the time we're handling client
        // controls
        getScrolledContainer()->removeChild(wnd);
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::onSized(ElementEventArgs& e)
{
    configureScrollbars();
    updateContainerPosition();
    Window::onSized(e);
    
    ++e.handled;
}

//----------------------------------------------------------------------------//
void ScrollablePane::onScroll(ScrollEventArgs& e)
{
    Window::onScroll(e);

    if (Scrollbar::standardProcessing(getVertScrollbar(), getHorzScrollbar(), -e.d_delta, e.d_modifiers.hasAlt()))
        ++e.handled;
}

//----------------------------------------------------------------------------//
void ScrollablePane::onIsSizeAdjustedToContentChanged(ElementEventArgs& /*e*/)
{
    // Forward to the container. handleAutoSizePaneChanged will be called
    // from the inside.
    auto container = getScrolledContainer();
    if (container)
    {
        container->setAdjustWidthToContent(d_isWidthAdjustedToContent);
        container->setAdjustHeightToContent(d_isHeightAdjustedToContent);
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::addScrollablePaneProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(ScrollablePane, USize,
        "ContentSize", "Property to get/set the content pane area size. Will not set dimensions that are "
        "adjusted to the content.",
        &ScrollablePane::setContentPaneSize, &ScrollablePane::getContentPaneSize, USize(UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, bool,
        "SwipeScrolling", "Whether scrolling by swipe in a widget area is enabled. "
        "Value is either \"true\" or \"false\".",
        &ScrollablePane::setSwipeScrollingEnabled, &ScrollablePane::isSwipeScrollingEnabled, false
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, bool,
        "ForceVertScrollbar", "Property to get/set the 'always show' setting for the vertical scroll "
        "bar of the pane.  Value is either \"true\" or \"false\".",
        &ScrollablePane::setAlwaysShowVerticalScrollbar, &ScrollablePane::isVerticalScrollbarAlwaysShown, false /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, bool,
        "ForceHorzScrollbar", "Property to get/set the 'always show' setting for the horizontal "
        "scroll bar of the pane.  Value is either \"true\" or \"false\".",
        &ScrollablePane::setAlwaysShowHorizontalScrollbar, &ScrollablePane::isHorizontalScrollbarAlwaysShown, false /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "HorzStepSize", "Property to get/set the step size for the horizontal Scrollbar.  Value is a float.",
        &ScrollablePane::setHorizontalStepSize, &ScrollablePane::getHorizontalStepSize, 0.1f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "HorzOverlapSize", "Property to get/set the overlap size for the horizontal Scrollbar.  Value is a float.",
        &ScrollablePane::setHorizontalOverlapSize, &ScrollablePane::getHorizontalOverlapSize, 0.01f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "HorzScrollPosition", "Property to get/set the scroll position of the horizontal Scrollbar as a fraction.  Value is a float.",
        &ScrollablePane::setHorizontalScrollPosition, &ScrollablePane::getHorizontalScrollPosition, 0.0f
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "VertStepSize", "Property to get/set the step size for the vertical Scrollbar.  Value is a float.",
        &ScrollablePane::setVerticalStepSize, &ScrollablePane::getVerticalStepSize, 0.1f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "VertOverlapSize", "Property to get/set the overlap size for the vertical Scrollbar.  Value is a float.",
        &ScrollablePane::setVerticalOverlapSize, &ScrollablePane::getVerticalOverlapSize, 0.01f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "VertScrollPosition", "Property to get/set the scroll position of the vertical Scrollbar as a fraction.  Value is a float.",
        &ScrollablePane::setVerticalScrollPosition, &ScrollablePane::getVerticalScrollPosition, 0.0f /* TODO: Inconsistency */
    );
}

//----------------------------------------------------------------------------//
Scrollbar* ScrollablePane::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(VertScrollbarName));
}

//----------------------------------------------------------------------------//
Scrollbar* ScrollablePane::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(HorzScrollbarName));
}

//----------------------------------------------------------------------------//
ScrolledContainer* ScrollablePane::getScrolledContainer() const
{
    return static_cast<ScrolledContainer*>(getChild(ScrolledContainerName));
}

//----------------------------------------------------------------------------//
Rectf ScrollablePane::getViewableArea() const
{
    if (!d_windowRenderer)
        throw InvalidRequestException(
            "This function must be implemented by the window renderer module");
    
    ScrollablePaneWindowRenderer* wr =
        static_cast<ScrollablePaneWindowRenderer*>(d_windowRenderer);
    return wr->getViewableArea();
}

//----------------------------------------------------------------------------//
void ScrollablePane::destroy(void)
{
    // detach from events of content pane
    d_contentChangedConn->disconnect();
    d_autoSizeChangedConn->disconnect();
    
    // now do the cleanup
    Window::destroy();
}

//----------------------------------------------------------------------------//
Window* ScrollablePane::findChildByNamePath_impl(const String& name_path) const
{
    // FIXME: This is horrible
    //
    if (name_path.substr(0, 7) == "__auto_")
        return Window::findChildByNamePath_impl(name_path);
    else
        return Window::findChildByNamePath_impl(ScrolledContainerName + '/' + name_path);
}

//----------------------------------------------------------------------------//
int ScrollablePane::writeChildWindowsXML(XMLSerializer& xml_stream) const
{
    // This is an easy and safe workaround for not writing out the buttonPane and contentPane. While in fact
    // we would eventually want to write these two to XML themselves, we do not want to write out their children
    // but there is no way to control this from inside these windows and currently there is also no way to do it
    // from the outside. This was determined to be the best solution, others would break ABI or are too hacky
    // Negative side-effects: any changes to AutoWindows (properties etc) will be lost in the output
    bool wasContentPaneWritingAllowed = getScrolledContainer()->isWritingXMLAllowed();

    getScrolledContainer()->setWritingXMLAllowed(false);

    int childOutputCount = Window::writeChildWindowsXML(xml_stream);

    getScrolledContainer()->setWritingXMLAllowed(wasContentPaneWritingAllowed);

    // since ScrolledContainer content is actually added to the
    // ScrollablePane window, this overridden function exists to dump those
    // out as if they were our own children.
    const size_t childCount = getContentPane()->getChildCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        getScrolledContainer()->getChildAtIndex(i)->writeXMLToStream(xml_stream);
        ++childOutputCount;
    }

    return childOutputCount;
}

//----------------------------------------------------------------------------//
void ScrollablePane::onMouseButtonDown(MouseButtonEventArgs& e)
{
    Window::onMouseButtonDown(e);

    if (d_swipeScrollingEnabled && e.d_button == MouseButton::Left)
    {
        // we want all cursor inputs from now on
        if (captureInput())
        {
            d_swiping = true;
            d_swipeStartPoint = e.d_localPos;
        }

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::onCursorMove(CursorMoveEventArgs& e)
{
    Window::onCursorMove(e);

    if (d_swiping)
    {
        const glm::vec2 delta(e.d_localPos - d_swipeStartPoint);
        scrollContentPane(-delta.x, -delta.y, ScrollSource::Swipe);
        d_swipeStartPoint = e.d_localPos;
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::onMouseButtonUp(MouseButtonEventArgs& e)
{
    Window::onMouseButtonUp(e);

    if (e.d_button == MouseButton::Left)
    {
        releaseInput();
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::onCaptureLost(WindowEventArgs& e)
{
    Window::onCaptureLost(e);

    // when we lose out hold on the cursor inputs, we are no longer swiping
    d_swiping = false;
}

}
