/***********************************************************************
    created:    Fri Jul 8 2005
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
#include "CEGUI/WindowRendererSets/Core/TabButton.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/widgets/TabButton.h"
#include "CEGUI/widgets/TabControl.h"

namespace CEGUI
{
const String FalagardTabButton::TypeName("Core/TabButton");

//----------------------------------------------------------------------------//
FalagardTabButton::FalagardTabButton(const String& type) :
    WindowRenderer(type, "TabButton")
{
}

//----------------------------------------------------------------------------//
void FalagardTabButton::createRenderGeometry()
{
    TabButton* w = static_cast<TabButton*>(d_window);
    TabControl* tc = w->getParent() ? dynamic_cast<TabControl*>(w->getParent()->getParent()) : nullptr;
    String prefix((tc && tc->getTabPanePosition() == TabControl::TabPanePosition::Bottom) ? "Bottom" : "Top");

    String state;
    if (w->isEffectiveDisabled())
        state = "Disabled";
    else if (w->isSelected())
        state = "Selected";
    else if (w->isPushed())
        state = "Pushed";
    else if (w->isHovering())
        state = "Hover";
    else if (w->isFocused())
        state = "Focused";
    else
        state = "Normal";

    const WidgetLookFeel& wlf = getLookNFeel();
    if (!wlf.isStateImageryPresent(prefix + state))
    {
        state = "Normal";
        if (!wlf.isStateImageryPresent(prefix + state))
            prefix = "";
    }

    wlf.getStateImagery(prefix + state).render(*w);
}

//----------------------------------------------------------------------------//
Sizef FalagardTabButton::getContentSize() const
{
    if (!d_window->getGUIContextPtr())
        return Sizef(0.f, 0.f);

    TabControl* tc = d_window->getParent() ? dynamic_cast<TabControl*>(d_window->getParent()->getParent()) : nullptr;
    if (!tc)
        return Sizef(0.f, 0.f);

    const WidgetLookFeel& lnf = getLookNFeel();

    String state((tc->getTabPanePosition() == TabControl::TabPanePosition::Bottom) ? "Bottom" : "Top");
    state += "Normal";
    if (!lnf.isStateImageryPresent(state))
        state = "Normal";

    // Find a text component responsible for a text
    //!!!FIXME: duplicated code, see FalagardTooltip::getTextComponentExtents!
    const auto& layerSpecs = lnf.getStateImagery(state).getLayerSpecifications();
    for (auto& layerSpec : layerSpecs)
    {
        const auto& sectionSpecs = layerSpec.getSectionSpecifications();
        for (auto& sectionSpec : sectionSpecs)
        {
            const auto& texts = lnf.getImagerySection(sectionSpec.getSectionName()).getTextComponents();
            if (!texts.empty())
                return texts.front().getTextExtent(*d_window);
        }
    }

    return Sizef(0.f, 0.f);
}

}
