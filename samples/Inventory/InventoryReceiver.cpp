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
#include "InventoryReceiver.h"
#include "InventoryItem.h"

#include <CEGUI/PropertyHelper.h>
#include <CEGUI/Image.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//------------------------------------------------------------------------------//
const String InventoryReceiver::WidgetTypeName("InventoryReceiver");
const String InventoryReceiver::EventNamespace("InventoryReceiver");

//------------------------------------------------------------------------------//
InventoryReceiver::InventoryReceiver(const String& type, const String& name) :
    Window(type, name)
{
    setDragDropTarget(true);
}

//------------------------------------------------------------------------------//
bool InventoryReceiver::addItemAtLocation(InventoryItem& item, int x, int y)
{
    if (itemWillFitAtLocation(item, x, y))
    {
        InventoryReceiver* old_receiver =
            dynamic_cast<InventoryReceiver*>(item.getParent());

        if (old_receiver)
            old_receiver->removeItem(item);

        item.setLocationOnReceiver(x, y);
        writeItemToContentMap(item);
        addChild(&item);

        // set position and size.  This ensures the items visually match the
        // logical content map.


        item.setPosition(UVector2(UDim(static_cast<float>(x) / contentWidth(), 0),
                                  UDim(static_cast<float>(y) / contentHeight(), 0)));
        item.setSize(USize(
            UDim(static_cast<float>(item.contentWidth()) / contentWidth(), 0),
            UDim(static_cast<float>(item.contentHeight()) / contentHeight(), 0)));

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------//
void InventoryReceiver::removeItem(InventoryItem& item)
{
    if (item.getParent() != this ||
        item.locationOnReceiverX() == -1 ||
        item.locationOnReceiverY() == -1)
            return;

    eraseItemFromContentMap(item);
    item.setLocationOnReceiver(-1, -1);
    removeChild(&item);
}

//------------------------------------------------------------------------------//
void InventoryReceiver::writeItemToContentMap(const InventoryItem& item)
{
    if (item.locationOnReceiverX() == -1 || item.locationOnReceiverY() == -1)
        return;

    for (int y = 0; y < item.contentHeight(); ++y)
    {
        const int map_y = item.locationOnReceiverY() + y;

        for (int x = 0; x < item.contentWidth(); ++x)
        {
            const int map_x = item.locationOnReceiverX() + x;

            bool val = d_content.elementAtLocation(map_x, map_y) |
                       item.isSolidAtLocation(x, y);
            d_content.setElementAtLocation(map_x, map_y, val);
        }
    }

    invalidate();
}

//------------------------------------------------------------------------------//
void InventoryReceiver::eraseItemFromContentMap(const InventoryItem& item)
{
    if (item.locationOnReceiverX() == -1 || item.locationOnReceiverY() == -1)
        return;

    for (int y = 0; y < item.contentHeight(); ++y)
    {
        const int map_y = item.locationOnReceiverY() + y;

        for (int x = 0; x < item.contentWidth(); ++x)
        {
            const int map_x = item.locationOnReceiverX() + x;

            bool val = d_content.elementAtLocation(map_x, map_y) &
                       !item.isSolidAtLocation(x, y);
            d_content.setElementAtLocation(map_x, map_y, val);
        }
    }

    invalidate();
}

//------------------------------------------------------------------------------//
bool InventoryReceiver::itemWillFitAtLocation(const InventoryItem& item,
                                              int x, int y)
{
    if (x < 0 || y < 0)
        return false;

    if (x + item.contentWidth() > d_content.width() ||
        y + item.contentHeight() > d_content.height())
            return false;

    const bool already_attached = this == item.getParent();
    // if item is already attatched erase its data from the content map so the
    // test result is reliable.
    if (already_attached)
        eraseItemFromContentMap(item);

    bool result = true;
    for (int item_y = 0; item_y < item.contentHeight() && result; ++item_y)
    {
        for (int item_x = 0; item_x < item.contentWidth() && result; ++item_x)
        {
            if (d_content.elementAtLocation(item_x + x, item_y + y) &&
                item.isSolidAtLocation(item_x, item_y))
                    result = false;
        }
    }

    // re-write item into content map if we erased it earlier.
    if (already_attached)
        writeItemToContentMap(item);

    return result;
}

//------------------------------------------------------------------------------//
void InventoryReceiver::onDragDropItemDropped(DragDropEventArgs &e)
{
    InventoryItem* item = dynamic_cast<InventoryItem*>(e.dragDropItem);

    if (!item)
        return;

    const Sizef square_size(squarePixelSize());

    Rectf item_area(item->getUnclippedOuterRect().get());
    item_area.offset(Vector2f(square_size.d_width / 2, square_size.d_height / 2));

    const int drop_x = gridXLocationFromPixelPosition(item_area.left());
    const int drop_y = gridYLocationFromPixelPosition(item_area.top());

    addItemAtLocation(*item, drop_x, drop_y);
}

//------------------------------------------------------------------------------//
void InventoryReceiver::populateGeometryBuffer()
{
    if (!isUserStringDefined("BlockImage"))
        return;

    const Image* img = PropertyHelper<Image*>::fromString(getUserString("BlockImage"));

    if (!img)
        return;

    const Sizef square_size(squarePixelSize());

    for (int y = 0; y < d_content.height(); ++y)
    {
        for (int x = 0; x < d_content.width(); ++x)
        {
            argb_t colour = 0xFFFFFFFF;
            if (d_content.elementAtLocation(x, y))
                colour = 0xFF0000FF;

            img->render(*d_geometry,
                        Vector2f(x * square_size.d_width + 1, y * square_size.d_height + 1),
                        Sizef(square_size.d_width - 2, square_size.d_height - 2), 0,
                        ColourRect(colour));
        }
    }
}

//------------------------------------------------------------------------------//
Rectf InventoryReceiver::gridBasePixelRect() const
{
    return getChildContentArea().get();
}

//------------------------------------------------------------------------------//

} // End of  CEGUI namespace section

