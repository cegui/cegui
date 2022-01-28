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
#include "CEGUI/text/RenderedTextParagraph.h"
#include "CEGUI/text/RenderedTextElement.h"
#include "CEGUI/text/RenderedTextStyle.h"
#include "CEGUI/RenderedStringImageComponent.h"
#include "CEGUI/Image.h"
#include "CEGUI/Font.h"
#include <algorithm>

namespace CEGUI
{

//----------------------------------------------------------------------------//
// Calculate the full width of the glyph, including padding
static float getGlyphFullWidth(const RenderedGlyph& glyph, const std::vector<RenderedTextElementPtr>& elements)
{
    if (glyph.isEmbeddedObject)
        return glyph.advance;

    // For text glyphs the full width may be greater than advance due to kerning
    float visualWidth = glyph.image ? glyph.image->getRenderedSize().d_width : 0.f;
    if (auto element = elements[glyph.elementIndex].get())
        visualWidth += (element->getLeftPadding() + element->getRightPadding());
    return std::max(glyph.advance, visualWidth);
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setupGlyphs(const std::u32string& text, const std::vector<size_t>& originalIndices,
    const std::vector<uint16_t>& elementIndices, const std::vector<RenderedTextElementPtr>& elements)
{
    d_linesDirty = true;

    const uint16_t defaultStyleIdx = static_cast<uint16_t>(elements.size() - 1);
    const size_t elementIdxCount = elementIndices.size();
    const bool adjustSourceIndices = !originalIndices.empty();

    for (auto& glyph : d_glyphs)
    {
        const auto utf32SourceIndex = glyph.sourceIndex;

        // Find associated element or associate with default text style
        glyph.elementIndex = (utf32SourceIndex < elementIdxCount) ?
            elementIndices[utf32SourceIndex] :
            defaultStyleIdx;

        // Make source index point to an original string passed in "text" arg
        if (adjustSourceIndices)
            glyph.sourceIndex = static_cast<uint32_t>(originalIndices[utf32SourceIndex]);

        // Setup traits based on the character. This may be overridden by the element.
        const auto codePoint = text[utf32SourceIndex];
        glyph.isJustifyable = (codePoint == ' ');
        glyph.isBreakable = (codePoint == ' ' || codePoint == '\t' || codePoint == '\r');
        glyph.isWhitespace = glyph.isBreakable;

        // Let an element initialize its glyph
        if (auto element = elements[glyph.elementIndex].get())
        {
            glyph.offset += element->getPadding().getPosition();
            elements[glyph.elementIndex]->setupGlyph(glyph);
        }
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::createRenderGeometry(std::vector<GeometryBuffer*>& out, glm::vec2& penPosition,
    const ColourRect* modColours, const Rectf* clipRect, const std::vector<RenderedTextElementPtr>& elements) const
{
    if (d_linesDirty)
        return;

    //???force using cached advance of embedded image instead of current size until re-formatted?

    //!!!pass selection range here, draw selection brush under instances with logical(?) index inside selection!
    //!!!TODO TEXT: how to render selection?! check each glyphs is it inside a selection range?!
    //!!!may switch selection remdering on/off, to optimize rendering text that can't have selection!
    //!!!may simply check selection length, it is effectively the same!
    //???draw background color the same as selection?! use line height

    //!!!TODO TEXT: need default style here! not only colors but also underline flag etc!
    //???how to know where to apply a default style? now this style element is the same as explicit ones! Last is not always default!

    //!!!TODO TEXT: need default halign changing and per-paragraph too!

    //???TODO TEXT: to what buffers really can merge?! need to see rendering order first!
    //???!!!merge between paragraphs?! pass canCombineFromIdx as arg!!!
    //???what about outline glyphs? may be in another texture, or generally the same?
    const auto canCombineFromIdx = out.size();
    const float startX = penPosition.x;

    uint32_t lineStartGlyphIdx = 0;
    for (const auto& line : d_lines)
    {
        for (size_t i = lineStartGlyphIdx; i < line.glyphEndIdx; ++i)
        {
            const auto& glyph = d_glyphs[i];

            // Skip leading whitespaces in word wrapped lines
            if (lineStartGlyphIdx && glyph.isWhitespace)
                continue;

            auto element = elements[glyph.elementIndex].get();
            if (!element)
                continue;

            auto pos = penPosition;
            float heightScale = 1.f;
            switch (element->getVerticalFormatting())
            {
                case VerticalImageFormatting::BottomAligned:
                    pos.y += line.extents.d_height - glyph.height;
                    break;

                case VerticalImageFormatting::CentreAligned:
                    pos.y += (line.extents.d_height - glyph.height) * 0.5f;
                    break;

                case VerticalImageFormatting::Stretched:
                    heightScale = (glyph.height > 0.f) ? (line.extents.d_height / glyph.height) : 0.f;
                    break;

                    // TODO TEXT: Tiled, at least for embedded images?

                    // TopAligned requires no operations
            }

            element->createRenderGeometry(out, glyph, pos, modColours, clipRect, heightScale, canCombineFromIdx);

            penPosition.x += glyph.advance;

            if (glyph.isJustifyable)
                penPosition.x += line.justifySpaceSize;
        }

        // Move the pen to the new line
        penPosition.x = startX;
        penPosition.y += line.extents.d_height;

        lineStartGlyphIdx = line.glyphEndIdx;
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateEmbeddedObjectExtents(const std::vector<RenderedTextElementPtr>& elements, const Window* hostWindow)
{
    for (size_t i = 0; i < d_glyphs.size(); ++i)
    {
        auto& glyph = d_glyphs[i];

        if (!glyph.isEmbeddedObject)
            continue;

        auto element = elements[glyph.elementIndex].get();
        if (!element)
            continue;

        const float prevWidth = glyph.advance;
        const float prevHeight = glyph.height;

        element->setupGlyph(glyph, hostWindow);

        // Any width change in a word wrapped paragraph may lead to line changes
        if (!d_linesDirty && d_wordWrap && glyph.advance != prevWidth)
            d_linesDirty = true;

        // Otherwise much lighter recalculations are needed
        if (!d_linesDirty)
        {
            if (auto line = getGlyphLine(i))
            {
                if (glyph.advance != prevWidth)
                {
                    line->extents.d_width += (glyph.advance - prevWidth);
                    line->horzFmtDirty = true;
                }

                if (glyph.height != prevHeight)
                    line->heightDirty = true;
            }
        }
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateLines(const std::vector<RenderedTextElementPtr>& elements, float areaWidth)
{
    if (!d_linesDirty)
        return;

    d_linesDirty = false;
    d_fitsIntoAreaWidth = true;

    d_lines.clear();

    // Always add the first line of the paragraph
    d_lines.emplace_back();
    auto* currLine = &d_lines.back();

    const auto glyphCount = d_glyphs.size();
    if (!glyphCount)
    {
        currLine->glyphEndIdx = 0;
        return;
    }

    // Word wrapping breakpoint tracking
    size_t lastBreakPointIdx = std::numeric_limits<size_t>().max();
    Line lastBreakPointState;
    float lastBreakPointWidthAdjustment = 0.f;

    float prevGlyphWidth = 0.f;
    uint32_t lineStartGlyphIdx = 0;
    for (size_t i = 0; i < glyphCount; ++i)
    {
        auto& glyph = d_glyphs[i];

        // Break too long lines if word wrapping is enabled
        if (d_wordWrap)
        {
            const float glyphWidth = getGlyphFullWidth(glyph, elements);

            float excessWidth = currLine->extents.d_width + glyphWidth - areaWidth;
            if (excessWidth > 0.f)
            {
                // If there is a good break point for word wrapping in the current line, use it
                if (lastBreakPointIdx < glyphCount)
                {
                    // Transfer everything past the last breakpoint to the new line
                    d_lines.emplace_back();
                    auto newLine = &d_lines.back();
                    newLine->extents = currLine->extents - lastBreakPointState.extents;
                    newLine->justifyableCount = currLine->justifyableCount - lastBreakPointState.justifyableCount;

                    // Restore the current line to the saved state and compensate possible difference
                    // between the last glyph's advance and full width (e.g. due to kerning at the breakpoint)
                    *currLine = lastBreakPointState;
                    currLine->extents.d_width += lastBreakPointWidthAdjustment;
                    currLine->glyphEndIdx = static_cast<uint32_t>(lastBreakPointIdx);
                    lineStartGlyphIdx = currLine->glyphEndIdx;

                    currLine = newLine;

                    // NB: in the new line this glyph is no more a valid breakpoint (infinite loop breaking)
                    lastBreakPointIdx = std::numeric_limits<size_t>().max();

                    // The current glyph may still be wider than our reference area
                    excessWidth = currLine->extents.d_width + glyphWidth - areaWidth;
                }

                // Either no break point was found or the glyph itself is too wide
                if (excessWidth > 0.f)
                {
                    // Track the word broken in the middle
                    d_fitsIntoAreaWidth = false;

                    // If this glyph is not the first in its line, transfer it to its own new line
                    if (i > lineStartGlyphIdx)
                    {
                        // Compensate possible difference (e.g. due to kerning at the breakpoint)
                        currLine->extents.d_width += (prevGlyphWidth - d_glyphs[i - 1].advance);
                        currLine->glyphEndIdx = static_cast<uint32_t>(i);
                        lineStartGlyphIdx = currLine->glyphEndIdx;

                        d_lines.emplace_back();
                        currLine = &d_lines.back();
                    }
                }
            }
            else if (glyph.isBreakable)
            {
                // Remember this glyph as the most recent word wrapping point
                lastBreakPointIdx = i;
                lastBreakPointState = *currLine;
                lastBreakPointWidthAdjustment =
                    (i > lineStartGlyphIdx) ? (prevGlyphWidth - d_glyphs[i - 1].advance) : 0.f;
            }

            prevGlyphWidth = glyphWidth;
        }

        // Count the current glyph in the current line
        currLine->extents.d_width += glyph.advance;
        if (glyph.isJustifyable)
            ++currLine->justifyableCount;
    }

    // Calculate the full width of the last glyph if not done during word wrapping
    if (!d_wordWrap)
        prevGlyphWidth = getGlyphFullWidth(d_glyphs.back(), elements);

    // Count full width for the last glyph in a line
    currLine->extents.d_width += (prevGlyphWidth - d_glyphs.back().advance);
    currLine->glyphEndIdx = glyphCount;

    // Track the paragraph exceeding an area width
    if (!d_wordWrap && currLine->extents.d_width > areaWidth)
        d_fitsIntoAreaWidth = false;
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateLineHeights(float defaultFontHeight)
{
    if (d_linesDirty)
        return;

    uint32_t lineStartGlyphIdx = 0;
    for (auto& line : d_lines)
    {
        if (!line.heightDirty)
        {
            lineStartGlyphIdx = line.glyphEndIdx;
            continue;
        }

        line.heightDirty = false;

        if (lineStartGlyphIdx == line.glyphEndIdx)
        {
            // An empty line uses the height of the previous text glyph
            uint32_t i = lineStartGlyphIdx;
            for (; i > 0; --i)
            {
                if (!d_glyphs[i].isEmbeddedObject)
                {
                    line.extents.d_height = d_glyphs[i].height;
                    break;
                }
            }

            if (!i)
                line.extents.d_height = (!d_glyphs[i].isEmbeddedObject) ? d_glyphs[i].height : defaultFontHeight;
        }
        else
        {
            line.extents.d_height = 0.f;
            for (size_t i = lineStartGlyphIdx; i < line.glyphEndIdx; ++i)
            {
                const float glyphHeight = d_glyphs[i].height;
                if (line.extents.d_height < glyphHeight)
                    line.extents.d_height = glyphHeight;
            }
        }

        lineStartGlyphIdx = line.glyphEndIdx;
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateHorizontalFormatting(float areaWidth)
{
    if (d_linesDirty)
        return;

    for (auto& line : d_lines)
    {
        if (!line.horzFmtDirty)
            continue;

        line.horzFmtDirty = false;

        const bool isLastLine = (&line == &d_lines.back());
        const auto lineHorzFmt = (isLastLine && d_horzFormatting == HorizontalTextFormatting::Justified) ?
            d_lastJustifiedLineHorzFormatting :
            d_horzFormatting;

        switch (lineHorzFmt)
        {
            case HorizontalTextFormatting::RightAligned:
                line.horzOffset = areaWidth - line.extents.d_width;
                line.justifySpaceSize = 0.f;
                break;
            case HorizontalTextFormatting::CentreAligned:
                line.horzOffset = (areaWidth - line.extents.d_width) * 0.5f;
                line.justifySpaceSize = 0.f;
                break;
            case HorizontalTextFormatting::Justified:
                line.horzOffset = 0.f;
                if (line.justifyableCount && line.extents.d_width < areaWidth)
                    line.justifySpaceSize = (areaWidth - line.extents.d_width) / line.justifyableCount;
                break;
        }
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::onAreaWidthChanged()
{
    if (d_wordWrap)
        d_linesDirty = true;
    else if (!d_linesDirty)
        for (auto& line : d_lines)
            line.horzFmtDirty = true;
}

//----------------------------------------------------------------------------//
RenderedTextParagraph::Line* RenderedTextParagraph::getGlyphLine(size_t glyphIndex)
{
    if (d_linesDirty)
        return nullptr;

    auto it = std::upper_bound(d_lines.begin(), d_lines.end(), glyphIndex,
        [](uint32_t value, const Line& elm) { return value < elm.glyphEndIdx; });

    return (it == d_lines.cend()) ? nullptr : &(*it);
}

}
