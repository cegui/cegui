/***********************************************************************
    created:    Thu Jul 7 2005
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
#include "CEGUI/WindowRendererSets/Core/Tooltip.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
const String FalagardTooltip::TypeName("Core/Tooltip");

//----------------------------------------------------------------------------//
void FalagardTooltip::createRenderGeometry()
{
    auto& imagery = getLookNFeel().getStateImagery(d_window->isEffectiveDisabled() ? "Disabled" : "Enabled");
    imagery.render(*d_window);
}

//----------------------------------------------------------------------------//
Sizef FalagardTooltip::getContentSize() const
{
    auto& lnf = getLookNFeel();
    Sizef sz = getTextComponentExtents(lnf);
    const Rectf textArea(lnf.getNamedArea("TextArea").getArea().getPixelRect(*d_window));
    const Rectf wndArea(CoordConverter::asAbsolute(d_window->getArea(), d_window->getParentPixelSize()));
    sz.d_width = CoordConverter::alignToPixels(sz.d_width + wndArea.getWidth() - textArea.getWidth());
    sz.d_height = CoordConverter::alignToPixels(sz.d_height + wndArea.getHeight() - textArea.getHeight());
    return sz;
}

//----------------------------------------------------------------------------//
Sizef FalagardTooltip::getTextComponentExtents(const WidgetLookFeel& lnf) const
{
    // Find a text component responsible for a tooltip text
    const auto& layerSpecs = lnf.getStateImagery("Enabled").getLayerSpecifications();
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

    return d_window->getPixelSize();
}

}
