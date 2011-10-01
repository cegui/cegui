/************************************************************************
    filename:   FalItemListbox.cpp
    created:    Mon Mar 20 2006
    author:     Tomas Lindquist Olsen
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/WindowRendererSets/Falagard/ItemListbox.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/elements/ItemListbox.h"
#include "CEGUI/CoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const String FalagardItemListbox::TypeName("Falagard/ItemListbox");

    FalagardItemListbox::FalagardItemListbox(const String& type) :
        ItemListBaseWindowRenderer(type),
        d_widgetLookAssigned(false)
    {
    }

    void FalagardItemListbox::render()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(d_window->isEffectiveDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*d_window);
    }

    Rectf FalagardItemListbox::getItemRenderArea(void) const
    {
        ItemListbox* lb = static_cast<ItemListbox*>(d_window);
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        bool v_visible = lb->getVertScrollbar()->isVisible();
        bool h_visible = lb->getHorzScrollbar()->isVisible();

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

    void FalagardItemListbox::onLookNFeelAssigned()
    {
        d_widgetLookAssigned = true;
    }

    void FalagardItemListbox::onLookNFeelUnassigned()
    {
        d_widgetLookAssigned = false;
    }

    Rectf FalagardItemListbox::getUnclippedInnerRect() const
    {
        if (!d_widgetLookAssigned)
            return d_window->getUnclippedOuterRect();

        const Rectf lr(getItemRenderArea());
        return CoordConverter::windowToScreen(*d_window, lr);
    }

} // End of  CEGUI namespace section
