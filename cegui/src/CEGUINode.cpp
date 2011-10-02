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

#include <algorithm>

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
const String Node::EventChildAdded("ChildAdded");
const String Node::EventChildRemoved("ChildRemoved");
const String Node::EventZOrderChanged("ZOrderChanged");
const String Node::EventNonClientChanged("NonClientChanged");

//----------------------------------------------------------------------------//
Node::Node():
    d_parent(0),
    
    d_nonClient(false),
    
    d_area(cegui_reldim(0), cegui_reldim(0), cegui_reldim(0), cegui_reldim(0)),
    d_horizontalAlignment(HA_LEFT),
    d_verticalAlignment(VA_TOP),
    d_minSize(cegui_reldim(0), cegui_reldim(0)),
    d_maxSize(cegui_reldim(1), cegui_reldim(1)),
    d_aspectMode(AM_IGNORE),
    d_aspectRatio(1.0 / 1.0),
    d_pixelAligned(true),
    d_pixelSize(0.0f, 0.0f),
    d_rotation(Quaternion::IDENTITY),
    
    d_unclippedOuterRect(this, &Node::getUnclippedOuterRect_impl),
    d_unclippedInnerRect(this, &Node::getUnclippedInnerRect_impl)
{
    addNodeProperties();
}

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

    // inform children that their screen area must be updated
    if (recursive)
    {
        const size_t child_count = getChildCount();
        for (size_t i = 0; i < child_count; ++i)
            d_children[i]->notifyScreenAreaChanged();
    }
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

    // TODO: We want an Event and more smart rect update handling
    
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

    // TODO: We want an Event and more smart rect update handling
    
    // ensure the area is calculated with the new aspect ratio
    // TODO: This potentially wastes effort, we should just mark it as dirty
    //       and postpone the calculation for as long as possible
    setArea(getArea());
}

//----------------------------------------------------------------------------//
void Node::setPixelAligned(const bool setting)
{
    if (d_pixelAligned == setting)
        return;
    
    d_pixelAligned = setting;
    
    // TODO: We want an Event and more smart rect update handling
    
    // ensure the area is calculated with the new pixel aligned setting
    // TODO: This potentially wastes effort, we should just mark it as dirty
    //       and postpone the calculation for as long as possible
    setArea(getArea());
}

//----------------------------------------------------------------------------//
Sizef Node::calculatePixelSize(bool skipAllPixelAlignment) const
{
    // TODO: skip all pixel alignment!
    
    // calculate pixel sizes for everything, so we have a common format for
    // comparisons.
    const Sizef absMin(CoordConverter::asAbsolute(d_minSize,
        System::getSingleton().getRenderer()->getDisplaySize()));
    const Sizef absMax(CoordConverter::asAbsolute(d_maxSize,
        System::getSingleton().getRenderer()->getDisplaySize()));

    Sizef base_size;
    if (skipAllPixelAlignment)
    {
        base_size = Sizef((d_parent && !d_nonClient) ?
                           d_parent->getUnclippedInnerRect().getFresh(true).getSize() :
                           getParentPixelSize(true));
    }
    else
    {
        base_size = Sizef((d_parent && !d_nonClient) ?
                           d_parent->getUnclippedInnerRect().get().getSize() :
                           getParentPixelSize());
    }

    Sizef ret = CoordConverter::asAbsolute(d_area.getSize(), base_size, false);

    // limit new pixel size to: minSize <= newSize <= maxSize
    ret.clamp(absMin, absMax);

    if (d_aspectMode != AM_IGNORE)
    {
        // make sure we respect current aspect mode and ratio
        ret.scaleToAspect(d_aspectMode, d_aspectRatio);

        // make sure we haven't blown any of the hard limits
        // still maintain the aspect when we do this
        if (d_aspectMode == AM_SHRINK)
        {
            float ratio = 1.0f;
            // check that we haven't blown the min size
            if (ret.d_width < absMin.d_width)
            {
                ratio = absMin.d_width / ret.d_width;
            }
            if (ret.d_height < absMin.d_height)
            {
                const float newRatio = absMin.d_height / ret.d_height;
                if (newRatio > ratio)
                    ratio = newRatio;
            }

            ret.d_width *= ratio;
            ret.d_height *= ratio;
        }
        else if (d_aspectMode == AM_EXPAND)
        {
            float ratio = 1.0f;
            // check that we haven't blown the min size
            if (ret.d_width > absMax.d_width)
            {
                ratio = absMax.d_width / ret.d_width;
            }
            if (ret.d_height > absMax.d_height)
            {
                const float newRatio = absMax.d_height / ret.d_height;
                if (newRatio > ratio)
                    ratio = newRatio;
            }

            ret.d_width *= ratio;
            ret.d_height *= ratio;
        }
        // NOTE: When the hard min max limits are unsatisfiable with the aspect lock mode,
        //       the result won't be limited by both limits!
    }
    
    if (d_pixelAligned)
    {
        ret.d_width = CoordConverter::alignToPixels(ret.d_width);
        ret.d_height = CoordConverter::alignToPixels(ret.d_height);
    }
    
    return ret;
}

//----------------------------------------------------------------------------//
Sizef Node::getParentPixelSize(bool skipAllPixelAlignment) const
{
    if (d_parent)
    {
        return skipAllPixelAlignment ? d_parent->calculatePixelSize(true) : d_parent->getPixelSize();
    }
    else
    {
        return System::getSingleton().getRenderer()->getDisplaySize();
    }
}

//----------------------------------------------------------------------------//
void Node::setRotation(const Quaternion& rotation)
{
    d_rotation = rotation;

    NodeEventArgs args(this);
    onRotated(args);
}

//----------------------------------------------------------------------------//
void Node::addChild(Node* node)
{
    // don't add null window or ourself as a child
    // TODO: IMO we should throw exceptions in both of these cases
    if (!node || node == this)
        return;

    addChild_impl(node);
    NodeEventArgs args(node);
    onChildAdded(args);
}

//----------------------------------------------------------------------------//
void Node::removeChild(Node* node)
{
    removeChild_impl(node);
    NodeEventArgs args(node);
    onChildRemoved(args);
}

//----------------------------------------------------------------------------//
bool Node::isChild(const Node* node) const
{
    return std::find(d_children.begin(), d_children.end(), node) != d_children.end();
}

//----------------------------------------------------------------------------//
bool Node::isAncestor(const Node* node) const
{
    if (!d_parent)
    {
        // no parent, no ancestor, nothing can be our ancestor
        return false;
    }
    
    return d_parent == node || d_parent->isAncestor(node);
}

//----------------------------------------------------------------------------//
void Node::setNonClient(const bool setting)
{
    if (setting == d_nonClient)
    {
        return;
    }
    
    d_nonClient = setting;

    NodeEventArgs args(this);
    onNonClientChanged(args);
}

//----------------------------------------------------------------------------//
const Node::CachedRectf& Node::getClientChildContentArea() const
{
    return getUnclippedInnerRect();
}

//----------------------------------------------------------------------------//
const Node::CachedRectf& Node::getNonClientChildContentArea() const
{
    return getUnclippedOuterRect();
}

void Node::addNodeProperties()
{
    const String propertyOrigin("Node");
    
    CEGUI_DEFINE_PROPERTY(Node, URect,
        "Area", "Property to get/set the windows unified area rectangle. Value is a \"URect\".",
        &Node::setArea, &Node::getArea, URect(UDim(0, 0), UDim(0, 0), UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY_NO_XML(Node, UVector2,
        "Position", "Property to get/set the windows unified position. Value is a \"UVector2\".",
        &Node::setPosition, &Node::getPosition, UVector2(UDim(0, 0), UDim(0, 0))
    );
    
    CEGUI_DEFINE_PROPERTY_NO_XML(Node, USize,
        "Size", "Property to get/set the windows unified size.  Value is a \"USize\".",
        &Node::setSize, &Node::getSize, USize(UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(Node, USize,
        "MinSize", "Property to get/set the windows unified minimum size.  Value is a \"USize\".",
        &Node::setMinSize, &Node::getMinSize, USize(UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(Node, USize,
        "MaxSize", "Property to get/set the windows unified maximum size.  Value is a \"USize\".",
        &Node::setMaxSize, &Node::getMaxSize, USize(UDim(1, 0), UDim(1, 0))
    );
    
    CEGUI_DEFINE_PROPERTY(Node, AspectMode,
        "AspectMode", "Property to get/set the 'aspect mode' setting for the Window. Value is either \"Ignore\", \"Shrink\" or \"Expand\".",
        &Node::setAspectMode, &Node::getAspectMode, AM_IGNORE
    );

    CEGUI_DEFINE_PROPERTY(Node, float,
        "AspectRatio", "Property to get/set the aspect ratio. Only applies when aspect mode is not \"Ignore\".",
        &Node::setAspectRatio, &Node::getAspectRatio, 1.0 / 1.0
    );
    
    CEGUI_DEFINE_PROPERTY(Node, bool,
        "PixelAligned", "Property to get/set the Node's size and position should be pixel aligned.  "
        "Value is either \"True\" or \"False\".",
        &Node::setPixelAligned, &Node::isPixelAligned, true
    );
    
    CEGUI_DEFINE_PROPERTY(Node, Quaternion,
        "Rotation", "Property to get/set the windows rotation factors.  Value is "
        "\"w:[w_float] x:[x_float] y:[y_float] z:[z_float] (Quaternion)\".",
        &Node::setRotation, &Node::getRotation, Quaternion(1.0,0.0,0.0,0.0)
    );
    
    CEGUI_DEFINE_PROPERTY(Node, bool,
        "NonClient", "Property to get/set the 'non-client' setting for the Window.  "
        "Value is either \"True\" or \"False\".",
        &Node::setNonClient, &Node::isNonClient, false
    );
}

//----------------------------------------------------------------------------//
void Node::setArea_impl(const UVector2& pos, const USize& size,
                        bool topLeftSizing, bool fireEvents)
{
    // we make sure the screen areas are recached when this is called as we need
    // it in most cases
    d_unclippedOuterRect.invalidateCache();
    d_unclippedInnerRect.invalidateCache();

    // notes of what we did
    bool moved = false, sized;

    // save original size so we can work out how to behave later on
    const Sizef oldSize(d_pixelSize);

    d_area.setSize(size);
    d_pixelSize = calculatePixelSize();

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
}

//----------------------------------------------------------------------------//
void Node::setParent(Node* parent)
{
    d_parent = parent;
}

//----------------------------------------------------------------------------//
void Node::addChild_impl(Node* node)
{
    // if node is attached elsewhere, detach it first (will fire normal events)
    Node* const old_parent = node->getParentNode();
    if (old_parent)
        old_parent->removeChild(node);
    
    // add node to child list
    d_children.push_back(node);

    // set the parent window
    node->setParent(this);

    // update area rects and content for the added window
    node->notifyScreenAreaChanged(true);
    
    // correctly call parent sized notification if needed.
    if (!old_parent || old_parent->getPixelSize() != getPixelSize())
    {
        NodeEventArgs args(this);
        node->onParentSized(args);
    }
}

//----------------------------------------------------------------------------//
void Node::removeChild_impl(Node* node)
{
    // find this window in the child list
    ChildList::iterator it = std::find(d_children.begin(), d_children.end(), node);

    // if the window was found in the child list
    if (it != d_children.end())
    {
        // remove window from child list
        d_children.erase(it);
        // reset windows parent so it's no longer this window.
        node->setParent(0);
    }
}

//----------------------------------------------------------------------------//
Rectf Node::getUnclippedOuterRect_impl(bool skipAllPixelAlignment) const
{
    const Sizef pixel_size = skipAllPixelAlignment ? calculatePixelSize(true) : getPixelSize();
    Rectf ret(Vector2f(0, 0), pixel_size);
    
    const Node* parent = getParentNode();
    
    Rectf parent_rect;
    if (parent)
    {
        const CachedRectf& base = parent->getChildContentArea(isNonClient());
        parent_rect = skipAllPixelAlignment ? base.getFresh(true) : base.get();
    }
    else
    {
        parent_rect = Rectf(Vector2f(0, 0), System::getSingleton().getRenderer()->getDisplaySize());
    }
    
    const Sizef parent_size = parent_rect.getSize();
    
    Vector2f offset = parent_rect.d_min + CoordConverter::asAbsolute(getArea().d_min, parent_size);

    switch (getHorizontalAlignment())
    {
        case HA_CENTRE:
            offset.d_x += (parent_size.d_width - pixel_size.d_width) * 0.5f;
            break;
        case HA_RIGHT:
            offset.d_x += parent_size.d_width - pixel_size.d_width;
            break;
        default:
            break;
    }
    
    switch (getVerticalAlignment())
    {
        case VA_CENTRE:
            offset.d_y += (parent_size.d_height - pixel_size.d_height) * 0.5f;
            break;
        case VA_BOTTOM:
            offset.d_y += parent_size.d_height - pixel_size.d_height;
            break;
        default:
            break;
    }

    if (d_pixelAligned && !skipAllPixelAlignment)
    {
        offset = Vector2f(CoordConverter::alignToPixels(offset.d_x),
                          CoordConverter::alignToPixels(offset.d_y));
    }
    
    ret.offset(offset);
    return ret;
}

//----------------------------------------------------------------------------//
Rectf Node::getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const
{
    return skipAllPixelAlignment ? getUnclippedOuterRect().getFresh(true) : getUnclippedOuterRect().get();
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
    // screen area changes when we're resized.
    // NB: Called non-recursive since the onParentSized notifications will deal
    // more selectively with child Window cases.
    notifyScreenAreaChanged(false);

    // inform children their parent has been re-sized
    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        NodeEventArgs args(this);
        d_children[i]->onParentSized(args);
    }

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

    fireEvent(EventParentSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Node::onMoved(NodeEventArgs& e)
{
    notifyScreenAreaChanged();

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
    fireEvent(EventRotated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Node::onChildAdded(NodeEventArgs& e)
{
    fireEvent(EventChildAdded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Node::onChildRemoved(NodeEventArgs& e)
{
    fireEvent(EventChildRemoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Node::onNonClientChanged(NodeEventArgs& e)
{
    // TODO: Trigger update of size and position information if needed

    fireEvent(EventNonClientChanged, e, EventNamespace);
}

} // End of  CEGUI namespace section
