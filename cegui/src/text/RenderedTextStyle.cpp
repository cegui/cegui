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
#include "CEGUI/text/RenderedTextStyle.h"
#include "CEGUI/text/RenderedTextParagraph.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextStyle::setupGlyph(RenderedGlyph& glyph, uint32_t codePoint) const
{
    // Bake padding into glyph metrics. Text glyphs are never resized and will
    // remain actual. Embedded objects metrics will be calculated in format().
    glyph.offset += getPadding().getPosition();
    glyph.advance += getLeftPadding() + getRightPadding();
    glyph.height = getFont()->getFontHeight() + getTopPadding() + getBottomPadding();

    glyph.isEmbeddedObject = false;
    glyph.isJustifyable = (codePoint == ' ');
    glyph.isBreakable = (codePoint == ' ' || codePoint == '\t' || codePoint == '\r');
    glyph.isWhitespace = glyph.isBreakable;

    //!!!TODO TEXT: how must be padding applied to RTL characters? Should L/R padding be inverted or not?
    //if (glyph.isRightToLeft) ...
}

//----------------------------------------------------------------------------//
void RenderedTextStyle::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const Window* refWnd, const glm::vec2& position, const ColourRect* modColours,
    const Rectf* clipRect) const
{
}

//----------------------------------------------------------------------------//
RenderedTextElementPtr RenderedTextStyle::clone() const
{
    return std::make_unique<RenderedTextStyle>(*this);
}

}
