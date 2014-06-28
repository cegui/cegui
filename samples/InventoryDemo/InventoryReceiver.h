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
#ifndef _InventoryReceiver_h_
#define _InventoryReceiver_h_

#include "InventoryBase.h"
#include <CEGUI/Window.h>

// Start of CEGUI namespace section
namespace CEGUI
{
class InventoryItem;

// Class representing a container for InventoryItem objects.
class InventoryReceiver : public Window, public InventoryBase
{
public:
    static const String WidgetTypeName;
    static const String EventNamespace;

    InventoryReceiver(const String& type, const String& name);

    // return whether the given item will fit at the given content grid
    // co-ordinates.
    bool itemWillFitAtLocation(const InventoryItem& item, int x, int y);

    // adds the given item at the given content grid co-ordinates.  The item
    // is only added if the item will fit at the given location.  Returns
    // true if the item was added successfully.
    bool addItemAtLocation(InventoryItem& item, int x, int y);

    // removes the given item.
    void removeItem(InventoryItem& item);

protected:
    // write the item's layout data into the content map.
    void writeItemToContentMap(const InventoryItem& item);
    // erase the item's layout data from the content map.
    void eraseItemFromContentMap(const InventoryItem& item);

    // base class overrides
    void onDragDropItemDropped(DragDropEventArgs &e);
    void populateGeometryBuffer();
    Rectf gridBasePixelRect() const;
};

} // End of  CEGUI namespace section

#endif  // end of guard _InventoryReceiver_h_

