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
#include "CEGUI/text/Font.h"
#include "CEGUI/text/FontGlyph.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextStyle::setupGlyph(RenderedGlyph& glyph, uint32_t codePoint) const
{
    // Bake padding into glyph metrics. Text glyphs are never resized and will remain actual.
    glyph.offset += getPadding().getPosition();
    glyph.advance += getLeftPadding() + getRightPadding();

    // Setup traits based on the character
    glyph.isJustifyable = (codePoint == ' ');
    glyph.isBreakable = (codePoint == ' ' || codePoint == '\t' || codePoint == '\r');
    glyph.isWhitespace = glyph.isBreakable;

    //!!!TODO TEXT: how must be padding applied to RTL characters? Should L/R padding be inverted or not?
    //if (glyph.isRightToLeft) ...
}

//----------------------------------------------------------------------------//
float RenderedTextStyle::getGlyphWidth(const RenderedGlyph& glyph) const
{
    const Image* image = glyph.fontGlyph ? glyph.fontGlyph->getImage(0) : nullptr;
    const float visualWidth = image ? image->getRenderedSize().d_width : 0.f;
    return std::max(glyph.advance, visualWidth + getLeftPadding() + getRightPadding());
}

//----------------------------------------------------------------------------//
float RenderedTextStyle::getHeight() const
{
    return d_font ? d_font->getFontHeight() : 0.f;
}

//----------------------------------------------------------------------------//
void RenderedTextStyle::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const RenderedGlyph* begin, size_t count, glm::vec2& penPosition, const ColourRect* modColours,
    const Rectf* clipRect, float lineHeight, float justifySpaceSize, size_t canCombineFromIdx) const
{
    glm::vec2 pos = penPosition;
    float heightScale = 1.f;
    applyVerticalFormatting(lineHeight, pos, heightScale);

    ImageRenderSettings settings(Rectf(), clipRect, d_colours, 1.f, true);
    if (modColours)
        settings.d_multiplyColours *= *modColours;

    const RenderedGlyph* end = begin + count;
    for (auto glyph = begin; glyph != end; ++glyph)
    {
        // Render the main image of the glyph
        if (glyph->fontGlyph)
        {
            if (auto image = glyph->fontGlyph->getImage(0))
            {
                Sizef size = image->getRenderedSize();
                size.d_height *= heightScale;
                settings.d_destArea.set(pos + glyph->offset, size);
                image->createRenderGeometry(out, settings, canCombineFromIdx);
            }

            if (d_outlineSize > 0.f)
            {
                // TODO TEXT: draw outline, or do this before main graphics?
                //???draw all outlines, then all main glyphs, to simplify buffer combining?
            }
        }

        pos.x += glyph->advance;
        if (glyph->isJustifyable)
            pos.x += justifySpaceSize;
    }

    if (d_underline)
    {
        // TODO TEXT: draw underline
    }

    if (d_strikeout)
    {
        // TODO TEXT: draw strikeout
    }

    penPosition.x = pos.x;
}

//----------------------------------------------------------------------------//
RenderedTextElementPtr RenderedTextStyle::clone() const
{
    return std::make_unique<RenderedTextStyle>(*this);
}

}
