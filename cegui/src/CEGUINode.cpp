/***********************************************************************
    filename:   CEGUINode.cpp
    created:    18/8/2011
    author:     Martin Preisler

    purpose:    Implements the Node class
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

#include "CEGUINode.h"
#include "CEGUICoordConverter.h"
#include "CEGUISystem.h"

// Start of CEGUI namespace section
namespace CEGUI
{

const String Node::EventNamespace("Node");

const String Node::EventSized("Sized");
const String Node::EventParentSized("ParentSized");
const String Node::EventMoved("Moved");
const String Node::EventHorizontalAlignmentChanged("HorizontalAlignmentChanged");
const String Node::EventVerticalAlignmentChanged("VerticalAlignmentChanged");
const String Node::EventRotated("Rotated");

//----------------------------------------------------------------------------//
Node::Node():
    d_parent(0),
    
    d_nonClient(false),
    
    d_area(cegui_reldim(0), cegui_reldim(0), cegui_reldim(0), cegui_reldim(0)),
    d_horizontalAlignment(HA_LEFT),
    d_verticalAlignment(VA_TOP),
    d_minSize(cegui_reldim(0), cegui_reldim(0)),
    d_maxSize(cegui_reldim(1), cegui_reldim(1)),
    d_pixelSize(0.0f, 0.0f),
    d_rotation(Quaternion::IDENTITY),
    
    d_unclippedOuterRect(this, &Node::getUnclippedOuterRect_impl),
    d_unclippedInnerRect(this, &Node::getUnclippedInnerRect_impl)
{}

//----------------------------------------------------------------------------//
Node::~Node()
{}

//----------------------------------------------------------------------------//
Node::Node(const Node&):
    d_unclippedOuterRect(this, &Node::getUnclippedOuterRect_impl),
    d_unclippedInnerRect(this, &Node::getUnclippedInnerRect_impl)
{}

//----------------------------------------------------------------------------//
void Node::setArea(const UVector2& pos, const USize& size)
{
    // Limit the value we set to something that's within the constraints
    // specified via the min and max size settings.

    // get size of 'base' - i.e. the size of the parent region.
    const Sizef base_sz((d_parent && !d_nonClient) ?
                         d_parent->getUnclippedInnerRect().get().getSize() :
                         getParentPixelSize());

    USize newsz(size);
    constrainToMinSize(base_sz, newsz);
    constrainToMaxSize(base_sz, newsz);

    setArea_impl(pos, newsz);
}

//----------------------------------------------------------------------------//
void Node::notifyScreenAreaChanged(bool recursive /* = true */)
{
    d_unclippedOuterRect.invalidateCache();
    d_unclippedInnerRect.invalidateCache();
    
    d_outerRectClipperValid = false;
    d_innerRectClipperValid = false;
    d_hitTestRectValid = false;

    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    //updateGeometryRenderSettings();

    // inform children that their screen area must be updated
    if (recursive)
    {
        const size_t child_count = getChildCount();
        for (size_t i = 0; i < child_count; ++i)
            d_children[i]->notifyScreenAreaChanged();
    }
}

//----------------------------------------------------------------------------//
void Node::setArea_impl(const UVector2& pos, const USize& size,
                        bool topLeftSizing, bool fireEvents)
{
    // we make sure the screen areas are recached when this is called as we need
    // it in most cases
    d_unclippedOuterRect.invalidateCache();
    d_unclippedInnerRect.invalidateCache();
    
    d_outerRectClipperValid = false;
    d_innerRectClipperValid = false;
    d_hitTestRectValid = false;

    // notes of what we did
    bool moved = false, sized;

    // save original size so we can work out how to behave later on
    const Sizef oldSize(d_pixelSize);

    // calculate pixel sizes for everything, so we have a common format for
    // comparisons.
    Sizef absMax(CoordConverter::asAbsolute(d_maxSize,
        System::getSingleton().getRenderer()->getDisplaySize()));
    Sizef absMin(CoordConverter::asAbsolute(d_minSize,
        System::getSingleton().getRenderer()->getDisplaySize()));

    const Sizef base_size((d_parent && !d_nonClient) ?
                           d_parent->getUnclippedInnerRect().get().getSize() :
                           getParentPixelSize());

    d_pixelSize = CoordConverter::asAbsolute(size, base_size);

    // limit new pixel size to: minSize <= newSize <= maxSize
    d_pixelSize.clamp(absMin, absMax);

    if (d_aspectMode != AM_IGNORE)
    {
        // make sure we respect current aspect mode and ratio
        d_pixelSize.scaleToAspect(d_aspectMode, d_aspectRatio);

        // make sure we haven't blown any of the hard limits
        // still maintain the aspect when we do this
        if (d_aspectMode == AM_SHRINK)
        {
            float ratio = 1.0f;
            // check that we haven't blown the min size
            if (d_pixelSize.d_width < absMin.d_width)
            {
                ratio = absMin.d_width / d_pixelSize.d_width;
            }
            if (d_pixelSize.d_height < absMin.d_height)
            {
                const float newRatio = absMin.d_height / d_pixelSize.d_height;
                if (newRatio > ratio)
                    ratio = newRatio;
            }

            d_pixelSize.d_width *= ratio;
            d_pixelSize.d_height *= ratio;
        }
        else if (d_aspectMode == AM_EXPAND)
        {
            float ratio = 1.0f;
            // check that we haven't blown the min size
            if (d_pixelSize.d_width > absMax.d_width)
            {
                ratio = absMax.d_width / d_pixelSize.d_width;
            }
            if (d_pixelSize.d_height > absMax.d_height)
            {
                const float newRatio = absMax.d_height / d_pixelSize.d_height;
                if (newRatio > ratio)
                    ratio = newRatio;
            }

            d_pixelSize.d_width *= ratio;
            d_pixelSize.d_height *= ratio;
        }
        // NOTE: When the hard min max limits are unsatisfiable with the aspect lock mode,
        //       the result won't be limited by both limits!
    }

    d_area.setSize(size);
    sized = (d_pixelSize != oldSize);

    // If this is a top/left edge sizing op, only modify position if the size
    // actually changed.  If it is not a sizing op, then position may always
    // change.
    if (!topLeftSizing || sized)
    {
        // only update position if a change has occurred.
        if (pos != d_area.d_min)
        {
            d_area.setPosition(pos);
            moved = true;
        }
    }

    // fire events as required
    if (fireEvents)
    {
        NodeEventArgs args(this);

        if (moved)
        {
            onMoved(args);
            // reset handled so 'sized' event can re-use (since  wo do not care
            // about it)
            args.handled = 0;
        }

        if (sized)
            onSized(args);
    }

    if (moved || sized)
        System::getSingleton().updateWindowContainingMouse();

    // update geometry position and clipping if nothing from above appears to
    // have done so already (NB: may be occasionally wasteful, but fixes bugs!)
    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    //if (!d_outerUnclippedRectValid)
    //    updateGeometryRenderSettings();
}

//----------------------------------------------------------------------------//
void Node::setHorizontalAlignment(const HorizontalAlignment alignment)
{
    if (d_horizontalAlignment == alignment)
        return;

    d_horizontalAlignment = alignment;

    NodeEventArgs args(this);
    onHorizontalAlignmentChanged(args);
}

//----------------------------------------------------------------------------//
void Node::setVerticalAlignment(const VerticalAlignment alignment)
{
    if (d_verticalAlignment == alignment)
        return;

    d_verticalAlignment = alignment;

    NodeEventArgs args(this);
    onVerticalAlignmentChanged(args);
}

//----------------------------------------------------------------------------//
void Node::setMinSize(const USize& size)
{
    d_minSize = size;

    // Apply set minimum size to the windows set size.
    // We can't use code in setArea[_impl] to adjust the set size, because
    // that code has to ensure that it's possible for a size constrained
    // window to 'recover' it's original (scaled) sizing when the constraint
    // no longer needs to be applied.

    // get size of 'base' - i.e. the size of the parent region.
    const Sizef base_sz((d_parent && !d_nonClient) ?
                         d_parent->getUnclippedInnerRect().get().getSize() :
                         getParentPixelSize());

    USize wnd_sz(getSize());

    if (constrainToMinSize(base_sz, wnd_sz))
        setSize(wnd_sz);
}

//----------------------------------------------------------------------------//
void Node::setMaxSize(const USize& size)
{
    d_maxSize = size;

    // Apply set maximum size to the windows set size.
    // We can't use code in setArea[_impl] to adjust the set size, because
    // that code has to ensure that it's possible for a size constrained
    // window to 'recover' it's original (scaled) sizing when the constraint
    // no longer needs to be applied.

    // get size of 'base' - i.e. the size of the parent region.
    const Sizef base_sz((d_parent && !d_nonClient) ?
                         d_parent->getUnclippedInnerRect().get().getSize() :
                         getParentPixelSize());

    USize wnd_sz(getSize());

    if (constrainToMaxSize(base_sz, wnd_sz))
        setSize(wnd_sz);
}

//----------------------------------------------------------------------------//
void Node::setAspectMode(AspectMode mode)
{
    if (d_aspectMode == mode)
        return;

    d_aspectMode = mode;

    // ensure the area is calculated with the new aspect mode
    // TODO: This potentially wastes effort, we should just mark it as dirty
    //       and postpone the calculation for as long as possible
    setArea(getArea());
}

//----------------------------------------------------------------------------//
void Node::setAspectRatio(float ratio)
{
    if (d_aspectRatio == ratio)
        return;

    d_aspectRatio = ratio;

    // ensure the area is calculated with the new aspect ratio
    // TODO: This potentially wastes effort, we should just mark it as dirty
    //       and postpone the calculation for as long as possible
    setArea(getArea());
}

//----------------------------------------------------------------------------//
Sizef Node::getParentPixelSize() const
{
    return d_parent ?
           d_parent->d_pixelSize :
           System::getSingleton().getRenderer()->getDisplaySize();
}

//----------------------------------------------------------------------------//
void Node::setRotation(const Quaternion& rotation)
{
    d_rotation = rotation;

    NodeEventArgs args(this);
    onRotated(args);
}

//----------------------------------------------------------------------------//
bool Node::constrainToMinSize(const Sizef& base_sz, USize& sz) const
{
    const Sizef pixel_sz(CoordConverter::asAbsolute(sz, base_sz));
    const Sizef min_sz(CoordConverter::asAbsolute(d_minSize,
        System::getSingleton().getRenderer()->getDisplaySize()));

    bool size_changed = false;

    // check width is not less than the minimum
    if (pixel_sz.d_width < min_sz.d_width)
    {
        sz.d_width.d_offset = ceguimin(sz.d_width.d_offset, d_minSize.d_width.d_offset);

        sz.d_width.d_scale = (base_sz.d_width != 0.0f) ?
            (min_sz.d_width - sz.d_width.d_offset) / base_sz.d_width :
            0.0f;

        size_changed = true;
    }

    // check height is not less than the minimum
    if (pixel_sz.d_height < min_sz.d_height)
    {
        sz.d_height.d_offset = ceguimin(sz.d_height.d_offset, d_minSize.d_height.d_offset);

        sz.d_height.d_scale = (base_sz.d_height != 0.0f) ?
            (min_sz.d_height - sz.d_height.d_offset) / base_sz.d_height :
            0.0f;

        size_changed = true;
    }

    return size_changed;
}

//----------------------------------------------------------------------------//
bool Node::constrainToMaxSize(const Sizef& base_sz, USize& sz) const
{
    const Sizef pixel_sz(CoordConverter::asAbsolute(sz, base_sz));
    const Sizef max_sz(CoordConverter::asAbsolute(d_maxSize,
        System::getSingleton().getRenderer()->getDisplaySize()));

    bool size_changed = false;

    // check width is not greater than the maximum
    if (pixel_sz.d_width > max_sz.d_width)
    {
        sz.d_width.d_offset = ceguimax(sz.d_width.d_offset, d_maxSize.d_width.d_offset);

        sz.d_width.d_scale = (base_sz.d_width != 0.0f) ?
            (max_sz.d_width - sz.d_width.d_offset) / base_sz.d_width :
            0.0f;

        size_changed = true;
    }

    // check height is not greater than the maximum
    if (pixel_sz.d_height > max_sz.d_height)
    {
        sz.d_height.d_offset = ceguimax(sz.d_height.d_offset, d_maxSize.d_height.d_offset);

        sz.d_height.d_scale = (base_sz.d_height != 0.0f) ?
            (max_sz.d_height - sz.d_height.d_offset) / base_sz.d_height :
            0.0f;

        size_changed = true;
    }

    return size_changed;
}

//----------------------------------------------------------------------------//
void Node::onSized(NodeEventArgs& e)
{
    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    /*// resize the underlying RenderingWindow if we're using such a thing
    if (d_surface && d_surface->isRenderingWindow())
        static_cast<RenderingWindow*>(d_surface)->setSize(getPixelSize());
    */
    // screen area changes when we're resized.
    // NB: Called non-recursive since the onParentSized notifications will deal
    // more selectively with child Window cases.
    notifyScreenAreaChanged(false);

    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    /*
    // we need to layout loonfeel based content first, in case anything is
    // relying on that content for size or positioning info (i.e. some child
    // is used to establish inner-rect position or size).
    //
    // TODO: The subsequent onParentSized notification for those windows cause
    // additional - unneccessary - work; we should look to optimise that.
    performChildWindowLayout();
    */

    // inform children their parent has been re-sized
    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        NodeEventArgs args(this);
        d_children[i]->onParentSized(args);
    }
    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    /*
    invalidate();*/

    fireEvent(EventSized, e, EventNamespace);
}


//----------------------------------------------------------------------------//
void Node::onParentSized(NodeEventArgs& e)
{
    // set window area back on itself to cause minimum and maximum size
    // constraints to be applied as required.  (fire no events though)

    setArea_impl(d_area.getPosition(), d_area.getSize(), false, false);

    const bool moved =
        ((d_area.d_min.d_x.d_scale != 0) || (d_area.d_min.d_y.d_scale != 0) ||
         (d_horizontalAlignment != HA_LEFT) || (d_verticalAlignment != VA_TOP));
    const bool sized =
        ((d_area.d_max.d_x.d_scale != 0) || (d_area.d_max.d_y.d_scale != 0) ||
         isInnerRectSizeChanged());

    // now see if events should be fired.
    if (moved)
    {
        NodeEventArgs args(this);
        onMoved(args);
    }

    if (sized)
    {
        NodeEventArgs args(this);
        onSized(args);
    }

    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    // if we were not moved or sized, do child layout anyway!
    /*if (!(moved || sized))
        performChildWindowLayout();*/

    fireEvent(EventParentSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Node::onMoved(NodeEventArgs& e)
{
    notifyScreenAreaChanged();

    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    /*
    // handle invalidation of surfaces and trigger needed redraws
    if (d_parent)
    {
        d_parent->invalidateRenderingSurface();
        // need to redraw some geometry if parent uses a caching surface
        if (d_parent->getTargetRenderingSurface().isRenderingWindow())
            System::getSingleton().signalRedraw();
    }*/

    fireEvent(EventMoved, e, EventNamespace);
}

void Node::onHorizontalAlignmentChanged(NodeEventArgs& e)
{
    notifyScreenAreaChanged();

    fireEvent(EventHorizontalAlignmentChanged, e, EventNamespace);   
}
   
void Node::onVerticalAlignmentChanged(NodeEventArgs& e)
{
    notifyScreenAreaChanged();

    fireEvent(EventVerticalAlignmentChanged, e, EventNamespace);   
}

//----------------------------------------------------------------------------//
void Node::onRotated(NodeEventArgs& e)
{
    // URGENT: This has to go into CEGUI::Window once it inherits CEGUI::Node
    // if we have no surface set, enable the auto surface
    /*if (!d_surface)
    {
        Logger::getSingleton().logEvent("Window::setRotation - "
            "Activating AutoRenderingSurface on Window '" + d_name +
            "' to enable rotation support.");

        setUsingAutoRenderingSurface(true);

        // still no surface?  Renderer or HW must not support what we need :(
        if (!d_surface)
        {
            Logger::getSingleton().logEvent("Window::setRotation - "
                "Failed to obtain a suitable ReneringWindow surface for "
                "Window '" + d_name + "'.  Rotation will not be available.",
                Errors);

            return;
        }
    }

    // ensure surface we have is the right type
    if (!d_surface->isRenderingWindow())
    {
        Logger::getSingleton().logEvent("Window::setRotation - "
            "Window '" + d_name + "' has a manual RenderingSurface that is not "
            "a RenderingWindow.  Rotation will not be available.", Errors);

        return;
    }

    // Checks / setup complete!  Now we can finally set the rotation.
    static_cast<RenderingWindow*>(d_surface)->setRotation(d_rotation);
    static_cast<RenderingWindow*>(d_surface)->setPivot(
        Vector3f(d_pixelSize.d_width / 2.0f, d_pixelSize.d_height / 2.0f, 0.0f));
    */
    fireEvent(EventRotated, e, EventNamespace);
}

} // End of  CEGUI namespace section
