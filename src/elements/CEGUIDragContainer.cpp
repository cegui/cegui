/***********************************************************************
	filename: 	CEGUIDragContainer.cpp
	created:	14/2/2005
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
#include "elements/CEGUIDragContainer.h"
#include "CEGUIImageset.h"
#include "CEGUICoordConverter.h"
#include <math.h>

// Start of CEGUI namespace section
namespace CEGUI
{
    //////////////////////////////////////////////////////////////////////////
    // Window type string
    const String DragContainer::WidgetTypeName("DragContainer");
    // Event Strings
    const String DragContainer::EventNamespace("DragContainer");
    const String DragContainer::EventDragStarted("DragStarted");
    const String DragContainer::EventDragEnded("DragEnded");
    const String DragContainer::EventDragPositionChanged("DragPositionChanged");
    const String DragContainer::EventDragEnabledChanged("DragEnabledChanged");
    const String DragContainer::EventDragAlphaChanged("DragAlphaChanged");
    const String DragContainer::EventDragMouseCursorChanged("DragMouseCursorChanged");
    const String DragContainer::EventDragThresholdChanged("DragThresholdChanged");
    const String DragContainer::EventDragDropTargetChanged("DragDropTargetChanged");
    // Properties
    DragContainerProperties::DragAlpha       DragContainer::d_dragAlphaProperty;
    DragContainerProperties::DragCursorImage DragContainer::d_dragCursorImageProperty;
    DragContainerProperties::DraggingEnabled DragContainer::d_dragEnabledProperty;
    DragContainerProperties::DragThreshold   DragContainer::d_dragThresholdProperty;

    //////////////////////////////////////////////////////////////////////////

    DragContainer::DragContainer(const String& type, const String& name) :
        Window(type, name),
        d_draggingEnabled(true),
        d_leftMouseDown(false),
        d_dragging(false),
        d_dragThreshold(8.0f),
        d_dragAlpha(0.5f),
        d_dropTarget(0),
        d_dragCursorImage((const Image*)DefaultMouseCursor),
        d_dropflag(false)
    {
        addDragContainerProperties();
    }

    DragContainer::~DragContainer(void)
    {
    }

    bool DragContainer::isDraggingEnabled(void) const
    {
        return d_draggingEnabled;
    }

    void DragContainer::setDraggingEnabled(bool setting)
    {
        if (d_draggingEnabled != setting)
        {
            d_draggingEnabled = setting;
            WindowEventArgs args(this);
            onDragEnabledChanged(args);
        }
    }

    bool DragContainer::isBeingDragged(void) const
    {
        return d_dragging;
    }

    float DragContainer::getPixelDragThreshold(void) const
    {
        return d_dragThreshold;
    }

    void DragContainer::setPixelDragThreshold(float pixels)
    {
        if (d_dragThreshold != pixels)
        {
            d_dragThreshold = pixels;
            WindowEventArgs args(this);
            onDragThresholdChanged(args);
        }
    }

    float DragContainer::getDragAlpha(void) const
    {
        return d_dragAlpha;
    }

    void DragContainer::setDragAlpha(float alpha)
    {
        if (d_dragAlpha != alpha)
        {
            d_dragAlpha = alpha;
            WindowEventArgs args(this);
            onDragAlphaChanged(args);
        }
    }

    const Image* DragContainer::getDragCursorImage(void) const
    {
        if (d_dragCursorImage == (const Image*)DefaultMouseCursor)
        {
            return System::getSingleton().getDefaultMouseCursor();
        }
        else
        {
            return d_dragCursorImage;
        }
    }

    void DragContainer::setDragCursorImage(const Image* image)
    {
        if (d_dragCursorImage != image)
        {
            d_dragCursorImage = image;
            WindowEventArgs args(this);
            onDragMouseCursorChanged(args);
        }
    }

    void DragContainer::setDragCursorImage(MouseCursorImage image)
    {
        setDragCursorImage((const Image*)image);
    }

    void DragContainer::setDragCursorImage(const String& imageset, const String& image)
    {
        setDragCursorImage(&ImagesetManager::getSingleton().getImageset(imageset)->getImage(image));
    }

    Window* DragContainer::getCurrentDropTarget(void) const
    {
        return d_dropTarget;
    }

    void DragContainer::addDragContainerProperties(void)
    {
        addProperty(&d_dragEnabledProperty);
        addProperty(&d_dragAlphaProperty);
        addProperty(&d_dragThresholdProperty);
        addProperty(&d_dragCursorImageProperty);
    }

    bool DragContainer::isDraggingThresholdExceeded(const Point& local_mouse)
    {
        // calculate amount mouse has moved.
        float	deltaX = fabsf(local_mouse.d_x - d_dragPoint.d_x.asAbsolute(d_pixelSize.d_width));
        float	deltaY = fabsf(local_mouse.d_y - d_dragPoint.d_y.asAbsolute(d_pixelSize.d_height));

        // see if mouse has moved far enough to start dragging operation
        return (deltaX > d_dragThreshold || deltaY > d_dragThreshold) ? true : false;
    }

    void DragContainer::initialiseDragging(void)
    {
        // only proceed if dragging is actually enabled
        if (d_draggingEnabled)
        {
            // initialise drag moving state
            d_storedClipState = d_clippedByParent;
            setClippedByParent(false);
            d_storedAlpha = d_alpha;
            setAlpha(d_dragAlpha);
            d_startPosition = getPosition();

            d_dragging = true;

            // Now drag mode is set, change cursor as required
            updateActiveMouseCursor();
        }
    }

    void DragContainer::doDragging(const Point& local_mouse)
    {
        // calculate amount to move
        UVector2 offset(cegui_absdim(local_mouse.d_x), cegui_absdim(local_mouse.d_y));
        offset -= d_dragPoint;

        // set new position
        setPosition(getPosition() + offset);

        // Perform event notification
        WindowEventArgs args(this);
        onDragPositionChanged(args);
    }

    void DragContainer::updateActiveMouseCursor(void) const
    {
        MouseCursor::getSingleton().setImage(d_dragging ? getDragCursorImage() : getMouseCursor());
    }

    void DragContainer::onMouseButtonDown(MouseEventArgs& e)
    {
        Window::onMouseButtonDown(e);

        if (e.button == LeftButton)
        {
            // ensure all inputs come to us for now
            if (captureInput())
            {
                // get position of mouse as co-ordinates local to this window.
                Vector2 localPos = CoordConverter::screenToWindow(*this, e.position);

                // store drag point for possible sizing or moving operation.
                d_dragPoint.d_x = cegui_absdim(localPos.d_x);
                d_dragPoint.d_y = cegui_absdim(localPos.d_y);
                d_leftMouseDown = true;
            }

            e.handled = true;
        }

    }

    void DragContainer::onMouseButtonUp(MouseEventArgs& e)
    {
        Window::onMouseButtonUp(e);

        if (e.button == LeftButton)
        {
            if (d_dragging)
            {
                // fire off event
                WindowEventArgs args(this);
                onDragEnded(args);
            }

            // release our capture on the input data
            releaseInput();
            e.handled = true;
        }
    }

    void DragContainer::onMouseMove(MouseEventArgs& e)
    {
        Window::onMouseMove(e);

        // get position of mouse as co-ordinates local to this window.
        Vector2 localMousePos = CoordConverter::screenToWindow(*this, e.position);

        // handle dragging
        if (d_dragging)
        {
            doDragging(localMousePos);
       }
        // not dragging
        else
        {
            // if mouse button is down (but we're not yet being dragged)
            if (d_leftMouseDown)
            {
                if (isDraggingThresholdExceeded(localMousePos))
                {
                    // Trigger the event
                    WindowEventArgs args(this);
                    onDragStarted(args);
                }
            }
        }
    }

    void DragContainer::onCaptureLost(WindowEventArgs& e)
    {
        Window::onCaptureLost(e);

        // reset state
        if (d_dragging)
        {
            // restore windows 'normal' state.
            d_dragging = false;
            setPosition(d_startPosition);
            setClippedByParent(d_storedClipState);
            setAlpha(d_storedAlpha);

            // restore normal mouse cursor
            updateActiveMouseCursor();
        }

        d_leftMouseDown = false;
        d_dropTarget = 0;

        e.handled = true;
    }

    void DragContainer::onAlphaChanged(WindowEventArgs& e)
    {
        // store new value and re-set dragging alpha as required.
        if (d_dragging)
        {
            d_storedAlpha = d_alpha;
            d_alpha = d_dragAlpha;
        }

        Window::onAlphaChanged(e);
    }

    void DragContainer::onClippingChanged(WindowEventArgs& e)
    {
        // store new value and re-set clipping for drag as required.
        if (d_dragging)
        {
            d_storedClipState = d_clippedByParent;
            d_clippedByParent = false;
        }

        Window::onClippingChanged(e);
    }

    void DragContainer::onMoved(WindowEventArgs& e)
    {
        Window::onMoved(e);
        if (d_dropflag)
        {
            d_startPosition = getPosition();
        }
    }

    void DragContainer::onDragStarted(WindowEventArgs& e)
    {
        initialiseDragging();

        fireEvent(EventDragStarted, e, EventNamespace);
    }

    void DragContainer::onDragEnded(WindowEventArgs& e)
    {
        fireEvent(EventDragEnded, e, EventNamespace);

        // did we drop over a window?
        if (d_dropTarget)
        {
            // set flag - we need to detect if the position changed in a DragDropItemDropped
            d_dropflag = true;
            // Notify that item was dropped in the target window
            d_dropTarget->notifyDragDropItemDropped(this);
            // reset flag
            d_dropflag = false;
        }
    }

    void DragContainer::onDragPositionChanged(WindowEventArgs& e)
    {
        fireEvent(EventDragPositionChanged, e, EventNamespace);

        Window* root;

        if (0 != (root = System::getSingleton().getGUISheet()))
        {
            // this hack with the 'enabled' state is so that getChildAtPosition
            // returns something useful instead of a pointer back to 'this'.
            // This hack is only acceptable because I am CrazyEddie!
            bool wasEnabled = d_enabled;
            d_enabled = false;
            // find out which child of root window has the mouse in it
            Window* eventWindow = root->getTargetChildAtPosition(MouseCursor::getSingleton().getPosition());
            d_enabled = wasEnabled;

            // use root itself if no child was hit
            if (!eventWindow)
            {
                eventWindow = root;
            }

            // if the window with the mouse is different to current drop target
            if (eventWindow != d_dropTarget)
            {
                DragDropEventArgs args(eventWindow);
                args.dragDropItem = this;
                onDragDropTargetChanged(args);
            }
        }
    }

    void DragContainer::onDragEnabledChanged(WindowEventArgs& e)
    {
        fireEvent(EventDragEnabledChanged, e, EventNamespace);

        // abort current drag operation if dragging gets disabled part way through
        if (!d_draggingEnabled && d_dragging)
        {
            releaseInput();
        }
    }

    void DragContainer::onDragAlphaChanged(WindowEventArgs& e)
    {
        fireEvent(EventDragAlphaChanged, e, EventNamespace);

        if (d_dragging)
        {
            d_alpha = d_storedAlpha;
            onAlphaChanged(e);
        }
    }

    void DragContainer::onDragMouseCursorChanged(WindowEventArgs& e)
    {
        fireEvent(EventDragMouseCursorChanged, e, EventNamespace);

        updateActiveMouseCursor();
    }

    void DragContainer::onDragThresholdChanged(WindowEventArgs& e)
    {
        fireEvent(EventDragThresholdChanged, e, EventNamespace);
    }

    void DragContainer::onDragDropTargetChanged(DragDropEventArgs& e)
    {
        fireEvent(EventDragDropTargetChanged, e, EventNamespace);

        // Notify old target that drop item has left
        if (d_dropTarget)
        {
            d_dropTarget->notifyDragDropItemLeaves(this);
        }

        // update to new target
        d_dropTarget = e.window;

        while ((d_dropTarget != 0) && !d_dropTarget->isDragDropTarget())
            d_dropTarget = d_dropTarget->getParent();

        // Notify new target window that someone has dragged a DragContainer over it
        if (d_dropTarget)
            d_dropTarget->notifyDragDropItemEnters(this);
    }

} // End of  CEGUI namespace section
