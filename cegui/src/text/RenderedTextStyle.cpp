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
    glyph.isJustifyable = (codePoint == ' ');
    glyph.isBreakable = (codePoint == ' ' || codePoint == '\t' || codePoint == '\r');
    glyph.isWhitespace = glyph.isBreakable;

    //!!!TODO TEXT: how must be padding applied to RTL characters? Should L/R padding be inverted or not?
    //if (glyph.isRightToLeft) ...
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
    float heightScale = 1.f;
    applyVerticalFormatting(lineHeight, pos.y, heightScale);

    const auto normalColour = modColours ? d_colours * (*modColours) : d_colours;
    const auto selectedColour = (!selection) ? normalColour :
        modColours ? selection->textColours * (*modColours) : selection->textColours;

    bool selected = false;
    ImageRenderSettings settings(Rectf(), clipRect, normalColour, 1.f, true);

    float effectStart = pos.x;
    auto effectBuffer = (d_underline || d_strikeout) ?
        &System::getSingleton().getRenderer()->createGeometryBufferColoured() :
        nullptr;

    const RenderedGlyph* end = begin + count;
    for (auto glyph = begin; glyph != end; ++glyph)
    {
        const bool newSelected = selection && glyph->sourceIndex >= selection->start && glyph->sourceIndex < selection->end;
        if (selected != newSelected)
        {
            drawEffects(effectBuffer, effectStart, pos.x, pos.y, settings.d_multiplyColours, clipRect);
            effectStart = pos.x;
            selected = newSelected;
            settings.d_multiplyColours = selected ? selectedColour : normalColour;
        }

        // Render the main image of the glyph
        const FontGlyph* fontGlyph = d_font->getGlyph(glyph->fontGlyphIndex);
        if (fontGlyph)
        {
            if (auto image = fontGlyph->getImage())
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

    if (effectBuffer)
    {
        drawEffects(effectBuffer, effectStart, pos.x, pos.y, settings.d_multiplyColours, clipRect);

        if (effectBuffer->getVertexCount())
            out.push_back(effectBuffer);
        else
            System::getSingleton().getRenderer()->destroyGeometryBuffer(*effectBuffer);
    }

    penPosition.x = pos.x;
}

//----------------------------------------------------------------------------//
static inline void drawLine(GeometryBuffer* effectBuffer, float left, float right,
    float top, float bottom, const ColourRect& colours, const Rectf* clipRect)
{
    if (clipRect && (top >= clipRect->bottom() || bottom <= clipRect->top()))
        return;

    ColouredVertex v[6];
    v[0].setColour(colours.d_top_left);
    v[0].d_position = glm::vec3(left, top, 0.0f);
    v[1].setColour(colours.d_bottom_left);
    v[1].d_position = glm::vec3(left, bottom, 0.0f);
    v[2].setColour(colours.d_bottom_right);
    v[2].d_position = glm::vec3(right, bottom, 0.0f);
    v[3].setColour(colours.d_top_right);
    v[3].d_position = glm::vec3(right, top, 0.0f);
    v[4].setColour(colours.d_top_left);
    v[4].d_position = glm::vec3(left, top, 0.0f);
    v[5].setColour(colours.d_bottom_right);
    v[5].d_position = glm::vec3(right, bottom, 0.0f);

    effectBuffer->appendGeometry(v, 6);
}

//----------------------------------------------------------------------------//
void RenderedTextStyle::drawEffects(GeometryBuffer* effectBuffer, float left,
    float right, float y, const ColourRect& colours, const Rectf* clipRect) const
{
    if (!effectBuffer || !d_font)
        return;

    if (clipRect)
    {
        if (left < clipRect->left())
            left = clipRect->left();
        if (right > clipRect->right())
            right = clipRect->right();
    }

    if (left >= right)
        return;

    y = std::round(y + d_font->getBaseline());

    if (d_underline)
    {
        const float top = y - d_font->getUnderlineTop();
        const float bottom = top + std::max(1.f, std::round(d_font->getUnderlineThickness()));
        drawLine(effectBuffer, left, right, top, bottom, colours, clipRect);
    }

    if (d_strikeout)
    {
        const float top = y - d_font->getStrikeoutTop();
        const float bottom = top + std::max(1.f, std::round(d_font->getStrikeoutThickness()));
        drawLine(effectBuffer, left, right, top, bottom, colours, clipRect);
    }
}

//----------------------------------------------------------------------------//
RenderedTextElementPtr RenderedTextStyle::clone() const
{
    return std::make_unique<RenderedTextStyle>(*this);
}

}
