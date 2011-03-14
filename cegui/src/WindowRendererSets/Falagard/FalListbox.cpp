/***********************************************************************
    filename:   FalListbox.cpp
    created:    Mon Jul 4 2005
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
#include "FalListbox.h"
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIListboxItem.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const String FalagardListbox::TypeName("Falagard/Listbox");


    FalagardListbox::FalagardListbox(const String& type) :
        ListboxWindowRenderer(type)
    {
    }

    Rectf FalagardListbox::getListRenderArea(void) const
    {
    	Listbox* lb = (Listbox*)d_window;
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        bool v_visible = lb->getVertScrollbar()->isVisible(true);
        bool h_visible = lb->getHorzScrollbar()->isVisible(true);

        // if either of the scrollbars are visible, we might want to use another text rendering area
        if (v_visible || h_visible)
        {
            String area_name("ItemRenderingArea");

            if (h_visible)
                area_name += "H";
            if (v_visible)
                area_name += "V";
            area_name += "Scroll";

            if (wlf.isNamedAreaDefined(area_name))
                return wlf.getNamedArea(area_name).getArea().getPixelRect(*lb);

            // since that did not exist, try optional alternative base name
            area_name = "ItemRenderArea";
            if (h_visible)
                area_name += "H";
            if (v_visible)
                area_name += "V";
            area_name += "Scroll";

            if (wlf.isNamedAreaDefined(area_name))
                return wlf.getNamedArea(area_name).getArea().getPixelRect(*lb);
        }

        // default to plain ItemRenderingArea
        if (wlf.isNamedAreaDefined("ItemRenderingArea"))
            return wlf.getNamedArea("ItemRenderingArea").getArea().getPixelRect(*lb);
        else
            return wlf.getNamedArea("ItemRenderArea").getArea().getPixelRect(*lb);
    }

    void FalagardListbox::render()
    {
    	Listbox* lb = (Listbox*)d_window;
        // render frame and stuff before we handle the items
        cacheListboxBaseImagery();

        //
        // Render list items
        //
        Vector3f itemPos;
        Sizef itemSize;
        Rectf itemClipper, itemRect;
        const float widest = lb->getWidestItemWidth();

        // calculate position of area we have to render into
        Rectf itemsArea(getListRenderArea());

        // set up some initial positional details for items
        itemPos.d_x = itemsArea.left() - lb->getHorzScrollbar()->getScrollPosition();
        itemPos.d_y = itemsArea.top() - lb->getVertScrollbar()->getScrollPosition();
        itemPos.d_z = 0.0f;

        const float alpha = lb->getEffectiveAlpha();

        // loop through the items
        size_t itemCount = lb->getItemCount();

        for (size_t i = 0; i < itemCount; ++i)
        {
            ListboxItem* listItem = lb->getListboxItemFromIndex(i);
            itemSize.d_height = listItem->getPixelSize().d_height;

            // allow item to have full width of box if this is wider than items
            itemSize.d_width = ceguimax(itemsArea.getWidth(), widest);

            // calculate destination area for this item.
            itemRect.left(itemPos.d_x);
            itemRect.top(itemPos.d_y);
            itemRect.setSize(itemSize);
            itemClipper = itemRect.getIntersection(itemsArea);

            // skip this item if totally clipped
            if (itemClipper.getWidth() == 0)
            {
                itemPos.d_y += itemSize.d_height;
                continue;
            }

            // draw this item
            listItem->draw(lb->getGeometryBuffer(), itemRect, alpha, &itemClipper);

            // update position ready for next item
            itemPos.d_y += itemSize.d_height;
        }

    }

    void FalagardListbox::cacheListboxBaseImagery()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(d_window->isEffectiveDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*d_window);
    }


} // End of  CEGUI namespace section
