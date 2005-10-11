/************************************************************************
    filename:   FalListbox.cpp
    created:    Mon Jul 4 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "FalListbox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIListboxItem.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardListbox::WidgetTypeName[] = "Falagard/Listbox";


    FalagardListbox::FalagardListbox(const String& type, const String& name) :
        Listbox(type, name)
    {
    }

    FalagardListbox::~FalagardListbox()
    {
    }

    Rect FalagardListbox::getListRenderArea(void) const
    {
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        bool v_visible = getVertScrollbar()->isVisible(true);
        bool h_visible = getHorzScrollbar()->isVisible(true);

        // if either of the scrollbars are visible, we might want to use another text rendering area
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

            if (wlf.isNamedAreaDefined(area_name))
            {
                return wlf.getNamedArea(area_name).getArea().getPixelRect(*this);
            }
        }

        // default to plain ItemRenderingArea
        return wlf.getNamedArea("ItemRenderingArea").getArea().getPixelRect(*this);
    }

    void FalagardListbox::populateRenderCache()
    {
        // render frame and stuff before we handle the items
        cacheListboxBaseImagery();

        //
        // Render list items
        //
        Vector3 itemPos;
        Size    itemSize;
        Rect    itemClipper, itemRect;
        float   widest = getWidestItemWidth();

        // calculate position of area we have to render into
        Rect itemsArea(getListRenderArea());

        // set up some initial positional details for items
        itemPos.d_x = itemsArea.d_left - getHorzScrollbar()->getScrollPosition();
        itemPos.d_y = itemsArea.d_top - getVertScrollbar()->getScrollPosition();
        itemPos.d_z = System::getSingleton().getRenderer()->getZLayer(3) - System::getSingleton().getRenderer()->getCurrentZ();

        float alpha = getEffectiveAlpha();

        // loop through the items
        size_t itemCount = getItemCount();

        for (size_t i = 0; i < itemCount; ++i)
        {
            itemSize.d_height = d_listItems[i]->getPixelSize().d_height;

            // allow item to have full width of box if this is wider than items
            itemSize.d_width = ceguimax(itemsArea.getWidth(), widest);

            // calculate destination area for this item.
            itemRect.d_left = itemPos.d_x;
            itemRect.d_top  = itemPos.d_y;
            itemRect.setSize(itemSize);
            itemClipper = itemRect.getIntersection(itemsArea);

            // skip this item if totally clipped
            if (itemClipper.getWidth() == 0)
            {
                itemPos.d_y += itemSize.d_height;
                continue;
            }

            // draw this item
            d_listItems[i]->draw(d_renderCache, itemRect, itemPos.d_z, alpha, &itemClipper);

            // update position ready for next item
            itemPos.d_y += itemSize.d_height;
        }

    }

    void FalagardListbox::cacheListboxBaseImagery()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(isDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*this);
    }


} // End of  CEGUI namespace section
