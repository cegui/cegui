/***********************************************************************
	filename: 	CEGUIScrolledContainer.cpp
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
#include "elements/CEGUIScrolledContainer.h"

// Start of CEGUI namespace section
namespace CEGUI
{   //////////////////////////////////////////////////////////////////////////
    // static data definitions
    //////////////////////////////////////////////////////////////////////////
    // Widget type name string
    const String ScrolledContainer::WidgetTypeName("ScrolledContainer");
    // Event string constants
    const String ScrolledContainer::EventNamespace("ScrolledContainer");
    const String ScrolledContainer::EventContentChanged("ContentChanged");
    const String ScrolledContainer::EventAutoSizeSettingChanged("AutoSizeSettingChanged");
    // properties
    ScrolledContainerProperties::ContentPaneAutoSized ScrolledContainer::d_autoSizedProperty;
    ScrolledContainerProperties::ContentArea          ScrolledContainer::d_contentAreaProperty;
    ScrolledContainerProperties::ChildExtentsArea     ScrolledContainer::d_childExtentsAreaProperty;
    //////////////////////////////////////////////////////////////////////////
    
    ScrolledContainer::ScrolledContainer(const String& type, const String& name) :
        Window(type, name),
        d_contentArea(0, 0, 0, 0),
        d_autosizePane(true)
    {
        addScrolledContainerProperties();
    }

    ScrolledContainer::~ScrolledContainer(void)
    {
    }

    bool ScrolledContainer::isContentPaneAutoSized(void) const
    {
        return d_autosizePane;
    }

    void ScrolledContainer::setContentPaneAutoSized(bool setting)
    {
        if (d_autosizePane != setting)
        {
            d_autosizePane = setting;

            // Fire events
            WindowEventArgs args1(this);
            onAutoSizeSettingChanged(args1);
        }
    }

    const Rect& ScrolledContainer::getContentArea(void) const
    {
        return d_contentArea;
    }

    void ScrolledContainer::setContentArea(const Rect& area)
    {
        if (!d_autosizePane)
        {
            d_contentArea = area;
            
            // Fire event
            WindowEventArgs args(this);
            onContentChanged(args);
       }

    }

    Rect ScrolledContainer::getChildExtentsArea(void) const
    {
        size_t childCount = getChildCount();

        // set up initial content area to match first child.
        if (childCount != 0)
        {
            Window* wnd = getChildAtIdx(0);
            Rect extents(wnd->getArea().asAbsolute(d_pixelSize));

            // control var starts at 1 since we already dealt with 0 above
            for (size_t i = 1; i < childCount; ++i)
            {
                wnd = getChildAtIdx(i);
                Rect area(wnd->getArea().asAbsolute(d_pixelSize));

                if (area.d_left < extents.d_left)
                    extents.d_left = area.d_left;

                if (area.d_top < extents.d_top)
                    extents.d_top = area.d_top;

                if (area.d_right > extents.d_right)
                    extents.d_right = area.d_right;

                if (area.d_bottom > extents.d_bottom)
                    extents.d_bottom = area.d_bottom;
            }

            return extents;
        }
        else
        {
            return Rect(0, 0, 0, 0);
        }
    }

    void ScrolledContainer::onContentChanged(WindowEventArgs& e)
    {
        if (d_autosizePane)
        {
            d_contentArea = getChildExtentsArea();
        }

        fireEvent(EventContentChanged, e, EventNamespace);
    }

    void ScrolledContainer::onAutoSizeSettingChanged(WindowEventArgs& e)
    {
        fireEvent(EventAutoSizeSettingChanged, e, EventNamespace);

        if (d_autosizePane)
        {
            WindowEventArgs args(this);
            onContentChanged(args);
        }
    }

    bool ScrolledContainer::handleChildSized(const EventArgs& e)
    {
        // Fire event that notifies that a child's area has changed.
        WindowEventArgs args(this);
        onContentChanged(args);
        return true;
    }

    bool ScrolledContainer::handleChildMoved(const EventArgs& e)
    {
        // Fire event that notifies that a child's area has changed.
        WindowEventArgs args(this);
        onContentChanged(args);
        return true;
    }

    Rect ScrolledContainer::getUnclippedInnerRect_impl(void) const
    {
        // return inner rect of our parent when possible,
        // or of the screen when not.
        return d_parent ?
            d_parent->getUnclippedInnerRect_impl() :
            System::getSingleton().getRenderer()->getRect();
    }

    void ScrolledContainer::onChildAdded(WindowEventArgs& e)
    {
        Window::onChildAdded(e);

        // subscribe to some events on this child
        d_eventConnections.insert(std::make_pair(e.window,
            e.window->subscribeEvent(Window::EventSized,
                Event::Subscriber(&ScrolledContainer::handleChildSized, this))));
        d_eventConnections.insert(std::make_pair(e.window,
            e.window->subscribeEvent(Window::EventMoved,
                Event::Subscriber(&ScrolledContainer::handleChildMoved, this))));

        // perform notification.
        WindowEventArgs args(this);
        onContentChanged(args);
    }

    void ScrolledContainer::onChildRemoved(WindowEventArgs& e)
    {
        Window::onChildRemoved(e);

        // disconnect from events for this window.
        ConnectionTracker::iterator conn;
        while ((conn = d_eventConnections.find(e.window)) != d_eventConnections.end())
        {
            conn->second->disconnect();
            d_eventConnections.erase(conn);
        }

        // perform notification.
        WindowEventArgs args(this);
        onContentChanged(args);
    }

    void ScrolledContainer::onParentSized(WindowEventArgs& e)
    {
        Window::onParentSized(e);

        // perform notification.
        WindowEventArgs args(this);
        onContentChanged(args);
    }

    void ScrolledContainer::addScrolledContainerProperties(void)
    {
        addProperty(&d_autoSizedProperty);
        addProperty(&d_contentAreaProperty);
        addProperty(&d_childExtentsAreaProperty);
    }

} // End of  CEGUI namespace section
