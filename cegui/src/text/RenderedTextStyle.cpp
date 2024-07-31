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
#include "CEGUI/text/TextUtils.h"
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/Vertex.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextStyle::setupGlyph(RenderedGlyph& glyph, uint32_t codePoint) const
{
    // Bake padding into glyph metrics. Text glyphs are never resized and will remain actual.
    glyph.offset += getPadding().getPosition();
    glyph.advance += getLeftPadding() + getRightPadding();

    // Setup traits based on the character
    glyph.isJustifiable = (codePoint == ' ');
    glyph.isWhitespace = (TextUtils::UTF32_WHITESPACE_CHARACTERS.find(codePoint) != std::u32string::npos);
}

//----------------------------------------------------------------------------//
float RenderedTextStyle::getGlyphWidth(const RenderedGlyph& glyph) const
{
    const FontGlyph* fontGlyph = d_font->getGlyph(glyph.fontGlyphIndex);
    const Image* image = fontGlyph ? fontGlyph->getImage() : nullptr;
    const float visualWidth = image ? image->getRenderedSize().d_width : 0.f;
    return std::max(glyph.advance, visualWidth + getLeftPadding() + getRightPadding());
}

//----------------------------------------------------------------------------//
float RenderedTextStyle::getHeight() const
{
    return d_font ? d_font->getFontHeight() : 0.f;
}

//----------------------------------------------------------------------------//
void RenderedTextStyle::createRenderGeometry(std::vector<GeometryBuffer*>& out, const RenderedGlyph* begin,
    size_t count, glm::vec2& penPosition, const ColourRect* modColours, const Rectf* clipRect,
    float lineHeight, float justifySpaceSize, size_t canCombineFromIdx, const SelectionInfo* selection) const
{
    if (!begin || !count)
        return;

    glm::vec2 pos = penPosition;
    glm::vec2 scale(1.f, 1.f);
    applyVerticalFormatting(lineHeight, pos.y, scale.y);
    const glm::vec2 scaleDiff(scale.x - 1.f, scale.y - 1.f);

    const RenderedGlyph* end = begin + count;

    auto effectBuffer = (d_underline || d_strikeout) ?
        &System::getSingleton().getRenderer()->createGeometryBufferColoured() :
        nullptr;

    // Render the outline
    if (d_outlineSize > 0.f)
    {
        const auto outlineColour = (modColours && d_useModColour) ? d_outlineColours * (*modColours) : d_outlineColours;
        ImageRenderSettings outlineSettings(Rectf(), clipRect, outlineColour, 1.f, true);

        for (auto glyph = begin; glyph != end; ++glyph)
        {
            //!!!TODO TEXT: can optimize out map search by caching outline object, like:
            // auto outline = font->getOutline(size);
            // for each glyph: outline->getOrCreateImage(glyphIdx)!
            if (auto image = d_font->getOutline(glyph->fontGlyphIndex, d_outlineSize))
            {
                outlineSettings.d_destArea.set(pos + glyph->offset * scale + image->getRenderedOffset() * scaleDiff, image->getRenderedSize() * scale);
                image->createRenderGeometry(out, outlineSettings, canCombineFromIdx);
            }

            pos.x += glyph->advance;
            if (glyph->isJustifiable)
                pos.x += justifySpaceSize;
        }

        if (effectBuffer)
            drawEffects(effectBuffer, penPosition.x, pos.x, pos.y, scale, d_outlineColours, clipRect, true);
    }

    // Render main images of glyphs

    const auto normalColour = (modColours && d_useModColour) ? d_colours * (*modColours) : d_colours;
    const auto selectedColour = (!selection) ? normalColour :
        (modColours && d_useModColour) ? selection->textColours * (*modColours) : selection->textColours;

    bool selected = false;
    ImageRenderSettings settings(Rectf(), clipRect, normalColour, 1.f, true);

    pos.x = penPosition.x;
    float effectStart = pos.x;

    for (auto glyph = begin; glyph != end; ++glyph)
    {
        const bool newSelected = selection && glyph->sourceIndex >= selection->start && glyph->sourceIndex < selection->end;
        if (selected != newSelected)
        {
            drawEffects(effectBuffer, effectStart, pos.x, pos.y, scale, settings.d_multiplyColours, clipRect, false);
            effectStart = pos.x;
            selected = newSelected;
            settings.d_multiplyColours = selected ? selectedColour : normalColour;
        }

        if (const FontGlyph* fontGlyph = d_font->getGlyph(glyph->fontGlyphIndex))
        {
            if (auto image = fontGlyph->getImage())
            {
                settings.d_destArea.set(pos + glyph->offset * scale + image->getRenderedOffset() * scaleDiff, image->getRenderedSize() * scale);
                image->createRenderGeometry(out, settings, canCombineFromIdx);
            }
        }

        pos.x += glyph->advance;
        if (glyph->isJustifiable)
            pos.x += justifySpaceSize;
    }

    if (effectBuffer)
    {
        drawEffects(effectBuffer, effectStart, pos.x, pos.y, scale, settings.d_multiplyColours, clipRect, false);

        if (effectBuffer->getVertexCount())
            out.push_back(effectBuffer);
        else
            System::getSingleton().getRenderer()->destroyGeometryBuffer(*effectBuffer);
    }

    penPosition.x = pos.x;
}

//----------------------------------------------------------------------------//
void RenderedTextStyle::drawEffects(GeometryBuffer* effectBuffer, float left,
    float right, float y, const glm::vec2& scale, const ColourRect& colours,
    const Rectf* clipRect, bool outline) const
{
    if (!effectBuffer || !d_font || left >= right)
        return;

    const float inflation = outline ? d_outlineSize : 0.f;
    left = (left - inflation) * scale.x;
    right = (right + inflation) * scale.x;

    if (d_underline)
    {
        const float localTop = d_font->getBaseline() - d_font->getUnderlineTop() - inflation;
        const float localHeight = d_font->getUnderlineThickness() + inflation + inflation;

        const float top = std::round(y + localTop * scale.y);
        const float bottom = std::round(top + std::max(1.f, localHeight * scale.y));
        const Rectf rect(left, top, right, bottom);
        effectBuffer->appendSolidRect(clipRect ? rect.getIntersection(*clipRect) : rect, colours);
    }

    if (d_strikeout)
    {
        const float localTop = d_font->getBaseline() - d_font->getStrikeoutTop() - inflation;
        const float localHeight = d_font->getStrikeoutThickness() + inflation + inflation;

        const float top = std::round(y + localTop * scale.y);
        const float bottom = std::round(top + std::max(1.f, localHeight * scale.y));
        const Rectf rect(left, top, right, bottom);
        effectBuffer->appendSolidRect(clipRect ? rect.getIntersection(*clipRect) : rect, colours);
    }
}

//----------------------------------------------------------------------------//
RenderedTextElementPtr RenderedTextStyle::clone() const
{
    return std::make_unique<RenderedTextStyle>(*this);
}

}
