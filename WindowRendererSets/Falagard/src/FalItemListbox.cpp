/************************************************************************
    filename:   FalItemListbox.cpp
    created:    Mon Mar 20 2006
    author:     Tomas Lindquist Olsen
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)
 
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
#include "FalItemListbox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "elements/CEGUIItemListbox.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardItemListbox::TypeName[] = "Falagard/ItemListbox";

    FalagardItemListbox::FalagardItemListbox(const String& type) :
        ItemListBaseWindowRenderer(type)
    {
    }

    void FalagardItemListbox::render()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(d_window->isDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*d_window);
    }

    Rect FalagardItemListbox::getItemRenderArea(void) const
    {
        ItemListbox* lb = static_cast<ItemListbox*>(d_window);
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        bool v_visible = lb->getVertScrollbar()->isVisible(true);
        bool h_visible = lb->getHorzScrollbar()->isVisible(true);

        // if either of the scrollbars are visible, we might want to use another text rendering area
        if (v_visible || h_visible)
        {
            String area_name("ItemRenderArea");

            if (h_visible)
            {
                area_name.push_back('H');
            }
            if (v_visible)
            {
                area_name.push_back('V');
            }
            area_name += "Scroll";

            if (wlf.isNamedAreaDefined(area_name))
            {
                return wlf.getNamedArea(area_name).getArea().getPixelRect(*lb);
            }
        }

        // default to plain ItemRenderArea
        return wlf.getNamedArea("ItemRenderArea").getArea().getPixelRect(*lb);
    }

} // End of  CEGUI namespace section
