/***********************************************************************
    created:    Fri Apr 22 2011
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _InventoryItem_h_
#define _InventoryItem_h_

#include "InventoryBase.h"
#include <CEGUI/widgets/DragContainer.h>

// Start of CEGUI namespace section
namespace CEGUI
{
// class representing an item that can be drag/dropped between
// InventoryReceiver objects.
class InventoryItem : public DragContainer, public InventoryBase
{
public:
    static const String WidgetTypeName;
    static const String EventNamespace;

    InventoryItem(const String& type, const String& name);

    // return whether the specified content location is solid.
    bool isSolidAtLocation(int x, int y) const;

    // set the content based on the given array.  Input array must
    // hold enough data given the current content size.
    void setItemLayout(const bool* layout);

    // return current X grid location on InventoryReceiver.  -1 means
    // invalid / not set.  This is typically set and invalidated by the
    // InventoryReciever::addItemAtLocation and InventoryReciever::removeItem
    // calls respectively.
    int locationOnReceiverX() const;

    // return current Y grid location on InventoryReceiver.  -1 means
    // invalid / not set.  This is typically set and invalidated by the
    // InventoryReciever::addItemAtLocation and InventoryReciever::removeItem
    // calls respectively.
    int locationOnReceiverY() const;

    // set the current grid location.  Note this does not update anything as
    // far as the receiver goes.  You will not normally call this directly, see
    // InventoryReciever::addItemAtLocation instead.
    void setLocationOnReceiver(int x, int y);

    // returns whether the current drag/drop target is a valid drop location for
    // this InventoryItem.  Only meaningful if isBeingDragged returns true.
    bool currentDropTargetIsValid() const;

    // base class overrides
    bool isHit(const Vector2f& position, const bool allow_disabled = false) const;
    void setContentSize(int width, int height);

protected:
    // base class overrides
    void onMoved(ElementEventArgs& e);
    void onDragDropTargetChanged(DragDropEventArgs& e);
    void populateGeometryBuffer();
    Rectf gridBasePixelRect() const;

    // whether the current drag / drop target is a valid drop location for this
    // item
    bool d_validDropTarget;
    // where our data is on our receivers content map.
    int d_receiverLocationX;
    // where our data is on our receivers content map.
    int d_receiverLocationY;
};

} // End of  CEGUI namespace section

#endif  // end of guard _InventoryItem_h_

