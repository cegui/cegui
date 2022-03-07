/***********************************************************************
    created:    Wed Jul 6 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/WindowRendererSets/Core/MultiColumnList.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/widgets/ListHeader.h"
#include "CEGUI/widgets/ListboxTextItem.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const String FalagardMultiColumnList::TypeName("Core/MultiColumnList");
    const String FalagardMultiColumnList::UnselectedTextColourPropertyName("NormalTextColour");
    const String FalagardMultiColumnList::SelectedTextColourPropertyName("SelectedTextColour");
    const String FalagardMultiColumnList::ActiveSelectionColourPropertyName("ActiveSelectionColour");
    const String FalagardMultiColumnList::InactiveSelectionColourPropertyName("InactiveSelectionColour");


    FalagardMultiColumnList::FalagardMultiColumnList(const String& type) :
        MultiColumnListWindowRenderer(type)
    {
    }

    Rectf FalagardMultiColumnList::getListRenderArea(void) const
    {
        MultiColumnList* w = static_cast<MultiColumnList*>(d_window);
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        bool v_visible = w->getVertScrollbar()->isVisible();
        bool h_visible = w->getHorzScrollbar()->isVisible();

        // if either of the scrollbars are visible, we might want to use another item rendering area
        if (v_visible || h_visible)
        {
            String area_name("ItemRenderingArea");

            if (h_visible)
            {
                area_name += "H";
            }
            if (v_visible)
            {
                area_name += "V";
            }
            area_name += "Scroll";

            if (wlf.isNamedAreaPresent(area_name))
            {
                return wlf.getNamedArea(area_name).getArea().getPixelRect(*w);
            }
        }

        // default to plain ItemRenderingArea
        return wlf.getNamedArea("ItemRenderingArea").getArea().getPixelRect(*w);
    }

    void FalagardMultiColumnList::createRenderGeometry()
    {
        MultiColumnList* w = static_cast<MultiColumnList*>(d_window);
        const ListHeader* header = w->getListHeader();
        const Scrollbar* vertScrollbar = w->getVertScrollbar();
        const Scrollbar* horzScrollbar = w->getHorzScrollbar();

        // render general stuff before we handle the items
        cacheListboxBaseImagery();

        //
        // Render list items
        //
        glm::vec3 itemPos;
        Sizef itemSize;
        Rectf itemClipper, itemRect;

        // calculate position of area we have to render into
        Rectf itemsArea(getListRenderArea());

        // set up initial positional details for items
        itemPos.y = itemsArea.top() - vertScrollbar->getScrollPosition();
        itemPos.z = 0.0f;

        const float alpha = w->getEffectiveAlpha();
        const ColourRect normalTextCol = getOptionalColour(UnselectedTextColourPropertyName, ListboxTextItem::DefaultTextColour);
        const ColourRect selectedTextCol = getOptionalColour(SelectedTextColourPropertyName, ListboxTextItem::DefaultTextColour);
        const ColourRect selectionBgCol = getOptionalColour(
            w->isActive() ? ActiveSelectionColourPropertyName : InactiveSelectionColourPropertyName,
            ListboxItem::DefaultSelectionColour);

        // loop through the items
        for (unsigned int i = 0; i < w->getRowCount(); ++i)
        {
            // set initial x position for this row.
            itemPos.x = itemsArea.left() - horzScrollbar->getScrollPosition();

            // calculate height for this row.
            itemSize.d_height = w->getHighestRowItemHeight(i);

            // loop through the columns in this row
            for (unsigned int j = 0; j < w->getColumnCount(); ++j)
            {
                // allow item to use full width of the column
                itemSize.d_width = CoordConverter::asAbsolute(header->getColumnWidth(j), header->getPixelSize().d_width);

                ListboxItem* item = w->getItemAtGridReference(MCLGridRef(i,j));

                // is the item for this column set?
                if (item)
                {
                    // calculate destination area for this item.
                    itemRect.left(itemPos.x);
                    itemRect.top(itemPos.y);
                    itemRect.setSize(itemSize);
                    itemClipper = itemRect.getIntersection(itemsArea);

                    // skip this item if totally clipped
                    if (itemClipper.getWidth() == 0)
                    {
                        itemPos.x += itemSize.d_width;
                        continue;
                    }

                    // Create render geometry for this item and add it to the Window
                    item->setSelectionColours(selectionBgCol);
                    if (auto textItem = dynamic_cast<ListboxTextItem*>(item))
                        textItem->setTextColours(textItem->isSelected() ? selectedTextCol : normalTextCol);
                    item->createRenderGeometry(w->getGeometryBuffers(), itemRect, alpha, &itemClipper);
                }

                // update position for next column.
                itemPos.x += itemSize.d_width;
            }

            // update position ready for next row
            itemPos.y += itemSize.d_height;
        }
    }

    void FalagardMultiColumnList::cacheListboxBaseImagery()
    {
        const WidgetLookFeel& wlf = getLookNFeel();
        const auto& imagery = wlf.getStateImagery(d_window->isEffectiveDisabled() ? "Disabled"
            : ((d_window->isFocused() && wlf.isStateImageryPresent("EnabledFocused")) ? "EnabledFocused" :
                "Enabled"));
        imagery.render(*d_window);
    }

}
