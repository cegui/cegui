/***********************************************************************
    created:    14/2/2005
    author:        Paul D Turner
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
#include "CEGUI/widgets/DragContainer.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/RenderingContext.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/GUIContext.h"
#include <cmath>

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
const String DragContainer::EventDragCursorChanged("DragCursorChanged");
const String DragContainer::EventDragThresholdChanged("DragThresholdChanged");
const String DragContainer::EventDragDropTargetChanged("DragDropTargetChanged");

//////////////////////////////////////////////////////////////////////////

DragContainer::DragContainer(const String& type, const String& name) :
    Window(type, name),
    d_dropTarget(nullptr),
    d_dragIndicatorImage(nullptr),
    d_dragThreshold(8.0f),
    d_dragAlpha(0.5f),
    d_draggingEnabled(true),
    d_leftPointerHeld(false),
    d_dragging(false),
    d_stickyMode(false),
    d_pickedUp(false),
    d_usingFixedDragOffset(false)
{
    addDragContainerProperties();
}

//----------------------------------------------------------------------------//
void DragContainer::setDraggingEnabled(bool setting)
{
    if (d_draggingEnabled != setting)
    {
        d_draggingEnabled = setting;
        WindowEventArgs args(this);
        onDragEnabledChanged(args);
    }
}

//----------------------------------------------------------------------------//
void DragContainer::setPixelDragThreshold(float pixels)
{
    if (d_dragThreshold != pixels)
    {
        d_dragThreshold = pixels;
        WindowEventArgs args(this);
        onDragThresholdChanged(args);
    }
}

//----------------------------------------------------------------------------//
void DragContainer::setDragAlpha(float alpha)
{
    if (d_dragAlpha != alpha)
    {
        d_dragAlpha = alpha;
        WindowEventArgs args(this);
        onDragAlphaChanged(args);
    }
}

//----------------------------------------------------------------------------//
const Image* DragContainer::getDragIndicatorImage() const
{
    return d_dragIndicatorImage ? d_dragIndicatorImage :
        d_guiContext ? d_guiContext->getDefaultCursorImage() :
        nullptr;
}

//----------------------------------------------------------------------------//
void DragContainer::setDragIndicatorImage(const Image* image)
{
    if (d_dragIndicatorImage != image)
    {
        d_dragIndicatorImage = image;
        WindowEventArgs args(this);
        onDragCursorChanged(args);
    }
}

//----------------------------------------------------------------------------//
void DragContainer::setDragIndicatorImage(const String& name)
{
    setDragIndicatorImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void DragContainer::addDragContainerProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(DragContainer, bool,
        "DraggingEnabled", "Property to get/set the state of the dragging enabled setting for the DragContainer.  Value is either \"true\" or \"false\".",
        &DragContainer::setDraggingEnabled, &DragContainer::isDraggingEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(DragContainer, float,
        "DragAlpha", "Property to get/set the dragging alpha value.  Value is a float.",
        &DragContainer::setDragAlpha, &DragContainer::getDragAlpha, 0.5f
    );

    CEGUI_DEFINE_PROPERTY(DragContainer, float,
        "DragThreshold", "Property to get/set the dragging threshold value.  Value is a float.",
        &DragContainer::setPixelDragThreshold, &DragContainer::getPixelDragThreshold, 8.0f /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(DragContainer, Image*,
        "DragIndicatorImage", "Property to get/set the cursor image used when dragging.  Value should be \"set:<imageset name> image:<image name>\".",
        &DragContainer::setDragIndicatorImage, &DragContainer::getDragIndicatorImage, nullptr
    );

    CEGUI_DEFINE_PROPERTY(DragContainer, bool,
        "StickyMode", "Property to get/set the state of the sticky mode setting for the "
            "DragContainer.  Value is either \"true\" or \"false\".",
        &DragContainer::setStickyModeEnabled, &DragContainer::isStickyModeEnabled, true /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(DragContainer, UVector2,
        "FixedDragOffset", "Property to get/set the state of the fixed dragging offset "
            "setting for the DragContainer.  "
            "Value is a UVector2 property value.",
        &DragContainer::setFixedDragOffset, &DragContainer::getFixedDragOffset, UVector2::zero()
    );

    CEGUI_DEFINE_PROPERTY(DragContainer, bool,
        "UseFixedDragOffset", "Property to get/set the setting that control whether the fixed "
            "dragging offset will be used.  "
            "Value is either \"true\" or \"false\".",
        &DragContainer::setUsingFixedDragOffset, &DragContainer::isUsingFixedDragOffset, false /* TODO: Inconsistency */
    );
}

//----------------------------------------------------------------------------//
bool DragContainer::isDraggingThresholdExceeded(const glm::vec2& local_cursor)
{
    return d_dragThreshold < std::fabs(local_cursor.x - CoordConverter::asAbsolute(d_dragPoint.d_x, d_pixelSize.d_width)) ||
        d_dragThreshold < std::fabs(local_cursor.y - CoordConverter::asAbsolute(d_dragPoint.d_y, d_pixelSize.d_height));
}

//----------------------------------------------------------------------------//
void DragContainer::doDragging(const CursorInputEventArgs& e)
{
    const auto offset = e.d_localPos - CoordConverter::asAbsolute(d_usingFixedDragOffset ? d_fixedDragOffset : d_dragPoint, d_pixelSize);
    if (offset.x == 0.f && offset.y == 0.f)
        return;

    auto pos = getPosition();
    pos.d_x.d_offset += offset.x;
    pos.d_y.d_offset += offset.y;
    setPosition(pos);

    updateDropTarget();

    CursorMoveEventArgs args(e, offset);
    onDragPositionChanged(args);
}

//----------------------------------------------------------------------------//
void DragContainer::endDragging(bool restorePosition)
{
    d_leftPointerHeld = false;
    d_pickedUp = false;
    d_targetDestroyConnection.disconnect();

    if (d_dragging)
    {
        // restore normal state of the window
        d_dragging = false;

        if (restorePosition)
            setPosition(d_startPosition);

        setClippedByParent(d_storedClipState);
        setAlpha(d_storedAlpha);

        WindowEventArgs args(this);
        onDragEnded(args);

        // restore normal cursor
        updateActiveCursorImage();
    }

    d_dropTarget = nullptr;
}

//----------------------------------------------------------------------------//
void DragContainer::cancelDragging()
{
    if (d_dragging)
        releaseInput();
}

//----------------------------------------------------------------------------//
void DragContainer::updateActiveCursorImage() const
{
    if (d_guiContext)
        d_guiContext->setCursorImage(d_dragging ? getDragIndicatorImage() : getEffectiveCursor());
}

//----------------------------------------------------------------------------//
void DragContainer::onMouseButtonDown(MouseButtonEventArgs& e)
{
    Window::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left)
    {
        // ensure all inputs come to us for now
        if (captureInput())
        {
            // store drag point for possible sizing or moving operation.
            d_dragPoint.d_x = cegui_absdim(e.d_localPos.x);
            d_dragPoint.d_y = cegui_absdim(e.d_localPos.y);
            d_leftPointerHeld = true;
        }

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void DragContainer::onMouseButtonUp(MouseButtonEventArgs& e)
{
    Window::onMouseButtonUp(e);

    if (e.d_button == MouseButton::Left)
    {
        // Handle dropping
        if (d_dragging)
        {
            // Target could change even if we didn't move. Ensure we drop correctly.
            updateDropTarget();

            if (d_dropTarget)
            {
                // Keep position changes made in a DragDropItemDropped
                const UVector2 prevPos = getPosition();

                // Try dropping. Continue sticky dragging if it is not accepted by the target.
                if (!d_dropTarget->notifyDragDropItemDropped(this) && d_pickedUp)
                    return;

                endDragging(prevPos == getPosition());
            }
        }

        // Release input capture anyway
        releaseInput();

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void DragContainer::onClick(MouseButtonEventArgs& e)
{
    Window::onClick(e);

    if (e.d_button == MouseButton::Left)
    {
        ++e.handled;

        // Perform picking up in a sticky mode
        if (!d_dragging && d_stickyMode && !d_pickedUp)
        {
            CursorInputEventArgs args(this, e.d_globalPos, e.d_buttons, e.d_modifiers);
            onDragStarted(args);
            if (d_dragging)
                d_pickedUp = true;
        }
    }
}

//----------------------------------------------------------------------------//
void DragContainer::onCursorMove(CursorMoveEventArgs& e)
{
    Window::onCursorMove(e);

    if (d_dragging)
    {
        doDragging(e);
    }
    else if (d_leftPointerHeld && isDraggingThresholdExceeded(e.d_localPos))
    {
        CursorInputEventArgs args(this, e.d_globalPos, e.d_buttons, e.d_modifiers);
        onDragStarted(args);
    }
}

//----------------------------------------------------------------------------//
void DragContainer::onCaptureLost(WindowEventArgs& e)
{
    Window::onCaptureLost(e);
    d_dropTarget = nullptr;
    endDragging(true);
    ++e.handled;
}

//----------------------------------------------------------------------------//
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

//----------------------------------------------------------------------------//
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

//----------------------------------------------------------------------------//
void DragContainer::onDragStarted(CursorInputEventArgs& e)
{
    if (!d_draggingEnabled || d_dragging)
        return;

    // initialise drag moving state
    d_storedClipState = d_clippedByParent;
    setClippedByParent(false);
    d_storedAlpha = d_alpha;
    setAlpha(d_dragAlpha);
    d_startPosition = getPosition();

    d_dragging = true;

    fireEvent(EventDragStarted, e, EventNamespace);

    // Handle possible endDragging() caused by event handlers
    if (!d_dragging)
        return;

    // Now drag mode is set, change cursor as required
    updateActiveCursorImage();

    // Immediately update position relative to the cursor
    doDragging(e);
}

//----------------------------------------------------------------------------//
void DragContainer::onDragEnded(WindowEventArgs& e)
{
    fireEvent(EventDragEnded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void DragContainer::onDragPositionChanged(CursorMoveEventArgs& e)
{
    fireEvent(EventDragPositionChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void DragContainer::updateDropTarget()
{
    if (!d_guiContext || !d_guiContext->getRootWindow())
        return;

    // find out which child of root window has the cursor in it
    Window* newTarget = d_guiContext->getRootWindow()->getTargetChildAtPosition(
        d_guiContext->getCursorPosition(), false, this);

    // use root itself if no child was hit
    if (!newTarget)
        newTarget = d_guiContext->getRootWindow();

    while (newTarget && !newTarget->isDragDropTarget())
        newTarget = newTarget->getParent();

    if (newTarget == d_dropTarget)
        return;

    // Notify old target that drop item has left
    if (d_dropTarget)
        d_dropTarget->notifyDragDropItemLeaves(this);

    // update to new target
    d_dropTarget = newTarget;

    // Notify new target window that someone has dragged a DragContainer over it
    if (d_dropTarget)
    {
        d_targetDestroyConnection = d_dropTarget->subscribeEvent(EventDestructionStarted, [this]()
        {
            d_targetDestroyConnection.disconnect();
            d_dropTarget = nullptr;
            updateDropTarget();
        });

        d_dropTarget->notifyDragDropItemEnters(this);
    }
    else
    {
        d_targetDestroyConnection.disconnect();
    }

    DragDropEventArgs args(d_dropTarget, this);
    onDragDropTargetChanged(args);
}

//----------------------------------------------------------------------------//
void DragContainer::onDragEnabledChanged(WindowEventArgs& e)
{
    fireEvent(EventDragEnabledChanged, e, EventNamespace);

    // abort current drag operation if dragging gets disabled part way through
    if (!d_draggingEnabled)
        cancelDragging();
}

//----------------------------------------------------------------------------//
void DragContainer::onDragAlphaChanged(WindowEventArgs& e)
{
    fireEvent(EventDragAlphaChanged, e, EventNamespace);

    if (d_dragging)
    {
        d_alpha = d_storedAlpha;
        onAlphaChanged(e);
    }
}

//----------------------------------------------------------------------------//
void DragContainer::onDragCursorChanged(WindowEventArgs& e)
{
    fireEvent(EventDragCursorChanged, e, EventNamespace);

    updateActiveCursorImage();
}

//----------------------------------------------------------------------------//
void DragContainer::onDragThresholdChanged(WindowEventArgs& e)
{
    fireEvent(EventDragThresholdChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void DragContainer::onDragDropTargetChanged(DragDropEventArgs& e)
{
    fireEvent(EventDragDropTargetChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void DragContainer::getRenderingContext_impl(RenderingContext& ctx) const
{
    // if not dragging, do the default thing.
    if (!d_dragging)
    {
        Window::getRenderingContext_impl(ctx);
        return;
    }

    // otherwise, switch rendering onto root rendering surface
    const Window* root = getRootWindow();
    ctx.surface = root->getTargetRenderingSurface();
    // ensure root window is only used as owner if it really is.
    ctx.owner = root->getRenderingSurface() == ctx.surface ? root : nullptr;
    // ensure use of correct offset for the surface we're targetting
    ctx.offset = ctx.owner ? ctx.owner->getOuterRectClipper().getPosition() : glm::vec2(0.f, 0.f);
    // draw to overlay queue
    ctx.queue = RenderQueueID::Overlay;
}

//----------------------------------------------------------------------------//
bool DragContainer::pickUp(bool force_sticky /*= false*/)
{
    if (d_pickedUp)
        return true;

    if (!d_draggingEnabled || !d_guiContext)
        return false;

    // see if we need to force sticky mode switch
    // can only pick up if sticky
    if (!d_stickyMode)
    {
        if (force_sticky)
            d_stickyMode = true;
        else
            return false;
    }

    activate();

    if (captureInput())
    {
        // set the dragging point to the centre of the container.
        d_dragPoint.d_x = cegui_absdim(d_pixelSize.d_width / 2.f);
        d_dragPoint.d_y = cegui_absdim(d_pixelSize.d_height / 2.f);

        // initialise the dragging state
        CursorInputEventArgs args(this, d_guiContext->getCursorPosition(), d_guiContext->getMouseButtons(), d_guiContext->getModifierKeys());
        onDragStarted(args);
        if (d_dragging)
            d_pickedUp = true;
    }

    return d_pickedUp;
}

}
