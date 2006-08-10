/***********************************************************************
	filename: 	CEGUIScrollablePane.cpp
	created:	1/3/2005
	author:		Paul D Turner
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
#include "elements/CEGUIScrollablePane.h"
#include "elements/CEGUIScrolledContainer.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUIWindowManager.h"
#include "CEGUIExceptions.h"
#include <math.h>

// Start of CEGUI namespace section
namespace CEGUI
{
    const String ScrollablePane::WidgetTypeName("CEGUI/ScrollablePane");

    //////////////////////////////////////////////////////////////////////////
    // Event name constants
    const String ScrollablePane::EventNamespace("ScrollablePane");
    const String ScrollablePane::EventContentPaneChanged("ContentPaneChanged");
    const String ScrollablePane::EventVertScrollbarModeChanged("VertScrollbarModeChanged");
    const String ScrollablePane::EventHorzScrollbarModeChanged("HorzScrollbarModeChanged");
    const String ScrollablePane::EventAutoSizeSettingChanged("AutoSizeSettingChanged");
    const String ScrollablePane::EventContentPaneScrolled("ContentPaneScrolled");
    // Child Widget name suffix constants
    const String ScrollablePane::VertScrollbarNameSuffix( "__auto_vscrollbar__" );
    const String ScrollablePane::HorzScrollbarNameSuffix( "__auto_hscrollbar__" );
    const String ScrollablePane::ScrolledContainerNameSuffix( "__auto_container__" );
    // Property objects
    ScrollablePaneProperties::ForceHorzScrollbar   ScrollablePane::d_horzScrollbarProperty;
    ScrollablePaneProperties::ForceVertScrollbar   ScrollablePane::d_vertScrollbarProperty;
    ScrollablePaneProperties::ContentPaneAutoSized ScrollablePane::d_autoSizedProperty;
    ScrollablePaneProperties::ContentArea          ScrollablePane::d_contentAreaProperty;
    ScrollablePaneProperties::HorzStepSize         ScrollablePane::d_horzStepProperty;
    ScrollablePaneProperties::HorzOverlapSize      ScrollablePane::d_horzOverlapProperty;
    ScrollablePaneProperties::HorzScrollPosition   ScrollablePane::d_horzScrollPositionProperty;
    ScrollablePaneProperties::VertStepSize         ScrollablePane::d_vertStepProperty;
    ScrollablePaneProperties::VertOverlapSize      ScrollablePane::d_vertOverlapProperty;
    ScrollablePaneProperties::VertScrollPosition   ScrollablePane::d_vertScrollPositionProperty;
    //////////////////////////////////////////////////////////////////////////

    /*************************************************************************
        ScrollablePaneWindowRenderer
    *************************************************************************/
    ScrollablePaneWindowRenderer::ScrollablePaneWindowRenderer(const String& name) :
        WindowRenderer(name, ScrollablePane::EventNamespace)
    {
    }

    //////////////////////////////////////////////////////////////////////////
    ScrollablePane::ScrollablePane(const String& type, const String& name) :
        Window(type, name),
        d_forceVertScroll(false),
        d_forceHorzScroll(false),
        d_contentRect(0, 0, 0, 0),
        d_vertStep(0.1f),
        d_vertOverlap(0.01f),
        d_horzStep(0.1f),
        d_horzOverlap(0.01f)
    {
        addScrollablePaneProperties();
		
		// create scrolled container widget
        ScrolledContainer* container =
            static_cast<ScrolledContainer*>(WindowManager::getSingleton().createWindow(
                ScrolledContainer::WidgetTypeName, d_name + ScrolledContainerNameSuffix));
        // add scrolled container widget as child
        addChildWindow(container);
    }

    ScrollablePane::~ScrollablePane(void)
    {
    }

    const ScrolledContainer* ScrollablePane::getContentPane(void) const
    {
        return getScrolledContainer();
    }

    bool ScrollablePane::isVertScrollbarAlwaysShown(void) const
    {
        return d_forceVertScroll;
    }

    void ScrollablePane::setShowVertScrollbar(bool setting)
    {
        if (d_forceVertScroll != setting)
        {
            d_forceVertScroll = setting;

            configureScrollbars();
            WindowEventArgs args(this);
            onVertScrollbarModeChanged(args);
        }
    }

    bool ScrollablePane::isHorzScrollbarAlwaysShown(void) const
    {
        return d_forceHorzScroll;
    }

    void ScrollablePane::setShowHorzScrollbar(bool setting)
    {
        if (d_forceHorzScroll != setting)
        {
            d_forceHorzScroll = setting;

            configureScrollbars();
            WindowEventArgs args(this);
            onHorzScrollbarModeChanged(args);
        }
    }

    bool ScrollablePane::isContentPaneAutoSized(void) const
    {
        return getScrolledContainer()->isContentPaneAutoSized();
    }

    void ScrollablePane::setContentPaneAutoSized(bool setting)
    {
        getScrolledContainer()->setContentPaneAutoSized(setting);
    }

    const Rect& ScrollablePane::getContentPaneArea(void) const
    {
        return getScrolledContainer()->getContentArea();
    }

    void ScrollablePane::setContentPaneArea(const Rect& area)
    {
        getScrolledContainer()->setContentArea(area);
    }

    float ScrollablePane::getHorizontalStepSize(void) const
    {
        return d_horzStep;
    }

    void ScrollablePane::setHorizontalStepSize(float step)
    {
        d_horzStep = step;
        configureScrollbars();
    }

    float ScrollablePane::getHorizontalOverlapSize(void) const
    {
        return d_horzOverlap;
    }

    void ScrollablePane::setHorizontalOverlapSize(float overlap)
    {
        d_horzOverlap = overlap;
        configureScrollbars();
    }

    float ScrollablePane::getHorizontalScrollPosition(void) const
    {
        Scrollbar* horzScrollbar = getHorzScrollbar();
        float docSz = horzScrollbar->getDocumentSize();
        return (docSz != 0) ? horzScrollbar->getScrollPosition() / docSz : 0.0f;
    }

    void ScrollablePane::setHorizontalScrollPosition(float position)
    {
        Scrollbar* horzScrollbar = getHorzScrollbar();
        horzScrollbar->setScrollPosition(horzScrollbar->getDocumentSize() * position);
    }

    float ScrollablePane::getVerticalStepSize(void) const
    {
        return d_vertStep;
    }

    void ScrollablePane::setVerticalStepSize(float step)
    {
        d_vertStep = step;
        configureScrollbars();
    }

    float ScrollablePane::getVerticalOverlapSize(void) const
    {
        return d_vertOverlap;
    }

    void ScrollablePane::setVerticalOverlapSize(float overlap)
    {
        d_vertOverlap = overlap;
        configureScrollbars();
    }

    float ScrollablePane::getVerticalScrollPosition(void) const
    {
        Scrollbar* vertScrollbar = getVertScrollbar();
        float docSz = vertScrollbar->getDocumentSize();
        return (docSz != 0) ? vertScrollbar->getScrollPosition() / docSz : 0.0f;
    }

    void ScrollablePane::setVerticalScrollPosition(float position)
    {
        Scrollbar* vertScrollbar = getVertScrollbar();
        vertScrollbar->setScrollPosition(vertScrollbar->getDocumentSize() * position);
    }

    void ScrollablePane::initialiseComponents(void)
    {
        // get horizontal scrollbar
        Scrollbar* horzScrollbar = getHorzScrollbar();

        // get vertical scrollbar
        Scrollbar* vertScrollbar = getVertScrollbar();

        // get scrolled container widget
        ScrolledContainer* container = getScrolledContainer();

        // do a bit of initialisation
        horzScrollbar->setAlwaysOnTop(true);
        vertScrollbar->setAlwaysOnTop(true);
        // container pane is always same size as this parent pane,
        // scrolling is actually implemented via positioning and clipping tricks.
        container->setSize(UVector2(cegui_reldim(1.0f), cegui_reldim(1.0f)));

        // subscribe to events we need to hear about
        vertScrollbar->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&ScrollablePane::handleScrollChange, this));
        horzScrollbar->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&ScrollablePane::handleScrollChange, this));
        d_contentChangedConn = container->subscribeEvent(
            ScrolledContainer::EventContentChanged,
            Event::Subscriber(&ScrollablePane::handleContentAreaChange, this));
        d_autoSizeChangedConn = container->subscribeEvent(
            ScrolledContainer::EventAutoSizeSettingChanged,
            Event::Subscriber(&ScrollablePane::handleAutoSizePaneChanged, this));

        // finalise setup
        configureScrollbars();
    }

    void ScrollablePane::configureScrollbars(void)
    {
        // controls should all be valid by this stage
        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        // enable required scrollbars
        vertScrollbar->setVisible(isVertScrollbarNeeded());
        horzScrollbar->setVisible(isHorzScrollbarNeeded());

        // Check if the addition of the horizontal scrollbar means we
        // now also need the vertical bar.
        if (horzScrollbar->isVisible())
        {
            vertScrollbar->setVisible(isVertScrollbarNeeded());
        }

        performChildWindowLayout();

        // get viewable area
        Rect viewableArea(getViewableArea());

        // set up vertical scroll bar values
        vertScrollbar->setDocumentSize(fabsf(d_contentRect.getHeight()));
        vertScrollbar->setPageSize(viewableArea.getHeight());
        vertScrollbar->setStepSize(ceguimax(1.0f, viewableArea.getHeight() * d_vertStep));
        vertScrollbar->setOverlapSize(ceguimax(1.0f, viewableArea.getHeight() * d_vertOverlap));
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition());

        // set up horizontal scroll bar values
        horzScrollbar->setDocumentSize(fabsf(d_contentRect.getWidth()));
        horzScrollbar->setPageSize(viewableArea.getWidth());
        horzScrollbar->setStepSize(ceguimax(1.0f, viewableArea.getWidth() * d_horzStep));
        horzScrollbar->setOverlapSize(ceguimax(1.0f, viewableArea.getWidth() * d_horzOverlap));
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition());
    }

    bool ScrollablePane::isHorzScrollbarNeeded(void) const
    {
        return ((fabs(d_contentRect.getWidth()) > getViewableArea().getWidth()) || d_forceHorzScroll);
    }

    bool ScrollablePane::isVertScrollbarNeeded(void) const
    {
        return ((fabs(d_contentRect.getHeight()) > getViewableArea().getHeight()) || d_forceVertScroll);
    }

    void ScrollablePane::updateContainerPosition(void)
    {
        // basePos is the position represented by the scrollbars
        // (these are negated so pane is scrolled in the correct directions)
        UVector2 basePos(cegui_absdim(-getHorzScrollbar()->getScrollPosition()),
                         cegui_absdim(-getVertScrollbar()->getScrollPosition()));

        // this bias is the absolute position that 0 on the scrollbars represent.
        // effectively removes un-used empty space from the pane.
        UVector2 bias(cegui_absdim(d_contentRect.d_left),
                      cegui_absdim(d_contentRect.d_top));

        // set the new container pane position to be what the scrollbars request
        // minus any bias generated by the location of the content.
        getScrolledContainer()->setPosition(basePos - bias);
    }

    void ScrollablePane::onContentPaneChanged(WindowEventArgs& e)
    {
        fireEvent(EventContentPaneChanged, e, EventNamespace);
    }

    void ScrollablePane::onVertScrollbarModeChanged(WindowEventArgs& e)
    {
        fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
    }

    void ScrollablePane::onHorzScrollbarModeChanged(WindowEventArgs& e)
    {
        fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
    }

    void ScrollablePane::onAutoSizeSettingChanged(WindowEventArgs& e)
    {
        fireEvent(EventAutoSizeSettingChanged, e, EventNamespace);
    }

    void ScrollablePane::onContentPaneScrolled(WindowEventArgs& e)
    {
        updateContainerPosition();
        fireEvent(EventContentPaneScrolled, e, EventNamespace);
    }

    bool ScrollablePane::handleScrollChange(const EventArgs& e)
    {
        WindowEventArgs args(this);
        onContentPaneScrolled(args);
        return true;
    }

    bool ScrollablePane::handleContentAreaChange(const EventArgs& e)
    {
        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        // get updated extents of the content
        Rect contentArea(getScrolledContainer()->getContentArea());

        // calculate any change on the top and left edges.
        float xChange = contentArea.d_left - d_contentRect.d_left;
        float yChange = contentArea.d_top - d_contentRect.d_top;

        // store new content extents information
        d_contentRect = contentArea;

        configureScrollbars();

        // update scrollbar positions (which causes container pane to be moved as needed).
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() - xChange);
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() - yChange);

        // this call may already have been made if the scroll positions changed.  The call
        // is required here for cases where the top/left 'bias' has changed; in which
        // case the scroll position notification may or may not have been fired.
        if (xChange || yChange)
        {
            updateContainerPosition();
        }
    
        // fire event
        WindowEventArgs args(this);
        onContentPaneChanged(args);

        return true;
    }

    bool ScrollablePane::handleAutoSizePaneChanged(const EventArgs& e)
    {
        // just forward event to client.
        WindowEventArgs args(this);
        fireEvent(EventAutoSizeSettingChanged, args, EventNamespace);
        return args.handled;
    }

    void ScrollablePane::addChild_impl(Window* wnd)
    {
        // null is not a valid window pointer!
        assert(wnd != 0);

        // See if this is an internally generated window (will have AutoWidgetNameSuffix in the name)
        if (wnd->getName().find(AutoWidgetNameSuffix) != String::npos)
        {
            // This is an internal widget, so should be added normally.
            Window::addChild_impl(wnd);
        }
        // this is a client window/widget, so should be added to the pane container.
        else
        {
            // container should always be valid by the time we're adding client controls
            getScrolledContainer()->addChildWindow(wnd);
        }
    }

    void ScrollablePane::removeChild_impl(Window* wnd)
    {
        // null is not a valid window pointer!
        assert(wnd != 0);

        // See if this is an internally generated window (will have AutoWidgetNameSuffix in the name)
        if (wnd->getName().find(AutoWidgetNameSuffix) != String::npos)
        {
            // This is an internal widget, so should be removed normally.
            Window::removeChild_impl(wnd);
        }
        // this is a client window/widget, so should be removed from the pane container.
        else
        {
            // container should always be valid by the time we're handling client controls
            getScrolledContainer()->removeChildWindow(wnd);
        }
    }

    void ScrollablePane::onSized(WindowEventArgs& e)
    {
        Window::onSized(e);
        configureScrollbars();
        updateContainerPosition();

        e.handled = true;
    }

    void ScrollablePane::onMouseWheel(MouseEventArgs& e)
    {
        // base class processing.
        Window::onMouseWheel(e);

        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        if (vertScrollbar->isVisible() && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
        {
            vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.wheelChange);
        }
        else if (horzScrollbar->isVisible() && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
        {
            horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.wheelChange);
        }

        e.handled = true;
    }

    void ScrollablePane::addScrollablePaneProperties(void)
    {
        addProperty(&d_horzScrollbarProperty);
        addProperty(&d_vertScrollbarProperty);
        addProperty(&d_autoSizedProperty);
        addProperty(&d_contentAreaProperty);
        addProperty(&d_horzStepProperty);
        addProperty(&d_horzOverlapProperty);
        addProperty(&d_horzScrollPositionProperty);
        addProperty(&d_vertStepProperty);
        addProperty(&d_vertOverlapProperty);
        addProperty(&d_vertScrollPositionProperty);
    }

    Scrollbar* ScrollablePane::getVertScrollbar() const
    {
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(
                                    getName() + VertScrollbarNameSuffix));
    }

    Scrollbar* ScrollablePane::getHorzScrollbar() const
    {
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(
                                    getName() + HorzScrollbarNameSuffix));
    }

    ScrolledContainer* ScrollablePane::getScrolledContainer() const
    {
        return static_cast<ScrolledContainer*>(
                    WindowManager::getSingleton().getWindow(
                    getName() + ScrolledContainerNameSuffix));
    }

    Rect ScrollablePane::getViewableArea() const
    {
        if (d_windowRenderer != 0)
        {
            ScrollablePaneWindowRenderer* wr = (ScrollablePaneWindowRenderer*)d_windowRenderer;
            return wr->getViewableArea();
        }
        else
        {
            //return getViewableArea_impl();
            throw InvalidRequestException("ScrollablePane::getViewableArea - This function must be implemented by the window renderer module");
        }
    }

    void ScrollablePane::destroy(void)
    {
        // detach from events on content pane
        d_contentChangedConn->disconnect();
        d_autoSizeChangedConn->disconnect();

        // now do the cleanup
        Window::destroy();
    }

} // End of  CEGUI namespace section
