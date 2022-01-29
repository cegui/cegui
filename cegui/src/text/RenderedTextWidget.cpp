/***********************************************************************
    created:    27/01/2022
    author:     Vladimir Orlov
 *************************************************************************/
 /***************************************************************************
  *   Copyright (C) 2004 - 2022 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/text/RenderedTextWidget.h"
#include "CEGUI/text/RenderedTextParagraph.h"
#include "CEGUI/Window.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
float RenderedTextWidget::getGlyphWidth(const RenderedGlyph& glyph) const
{
    //???TODO TEXT: or return actual current width?!
    return glyph.advance;
}

//----------------------------------------------------------------------------//
void RenderedTextWidget::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const RenderedGlyph* begin, size_t count, glm::vec2& penPosition, const ColourRect* modColours,
    const Rectf* clipRect, float lineHeight, float justifySpaceSize, size_t canCombineFromIdx) const
{
    Window* window = nullptr; //!!!DBG TMP! must get from glyph.userData;
    if (!window)
        return;

    glm::vec2 finalPos = penPosition;

    // Re-adjust for inner-rect of parent
    if (const Window* parent = window->getParent())
        finalPos -= (parent->getUnclippedInnerRect().get().d_min - parent->getUnclippedOuterRect().get().d_min);

    // We do not actually draw the widget, we just move it into position
    const glm::vec2 wposAbs = finalPos + d_padding.d_min;
    window->setPosition(UVector2(UDim(0, wposAbs.x), UDim(0, wposAbs.y)));

    penPosition.x += begin->advance;
}

//----------------------------------------------------------------------------//
RenderedTextElementPtr RenderedTextWidget::clone() const
{
    return std::make_unique<RenderedTextWidget>(*this);
}

}
