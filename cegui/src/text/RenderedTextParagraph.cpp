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
#include "CEGUI/FontGlyph.h"
#ifdef CEGUI_BIDI_SUPPORT
#include "CEGUI/text/BidiVisualMapping.h"
#endif
#include <algorithm>

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setupGlyphs(const std::u32string& text, const std::vector<size_t>& originalIndices,
    const std::vector<uint16_t>& elementIndices, const std::vector<RenderedTextElementPtr>& elements)
{
    d_linesDirty = true;

    const size_t elementIdxCount = elementIndices.size();
    const uint16_t defaultStyleIdx = static_cast<uint16_t>(elements.size() - 1);
    const bool adjustSourceIndices = !originalIndices.empty();

    for (auto& glyph : d_glyphs)
    {
        const auto utf32SourceIndex = glyph.sourceIndex;

        // Find associated element or associate with default text style
        glyph.elementIndex = (utf32SourceIndex < elementIdxCount) ?
            elementIndices[utf32SourceIndex] :
            defaultStyleIdx;

        // Do element-dependent initialization of the glyph
        if (auto element = elements[glyph.elementIndex].get())
            element->setupGlyph(glyph, text[utf32SourceIndex]);

        // Make source index point to an original text string
        if (adjustSourceIndices)
            glyph.sourceIndex = static_cast<uint32_t>(originalIndices[utf32SourceIndex]);
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::createRenderGeometry(std::vector<GeometryBuffer*>& out, glm::vec2& penPosition,
    const ColourRect* modColours, const Rectf* clipRect, const SelectionInfo* selection,
    const std::vector<RenderedTextElementPtr>& elements) const
{
    if (d_linesDirty)
        return;

    //!!!TODO TEXT: need default style here! not only colors but also underline flag etc!
    //???how to know where to apply a default style? now this style element is the same as explicit ones! Last is not always default!

    //!!!TODO TEXT: need default halign changing and per-paragraph too!

    //???TODO TEXT: to what buffers really can merge?! need to see rendering order first!
    //???!!!merge between paragraphs?! pass canCombineFromIdx as arg!!!
    //???what about outline glyphs? may be in another texture, or generally the same?
    const auto canCombineFromIdx = out.size();
    const float startX = penPosition.x;

    uint32_t i = 0;
    for (const auto& line : d_lines)
    {
        penPosition.x += line.horzOffset;

        //!!!FIXME TEXT: if do this, need to do the same when counting line width etc!
        //// Skip leading whitespaces in word wrapped lines
        //if (i > 0)
        //    while (i < line.glyphEndIdx && d_glyphs[i].isWhitespace)
        //        ++i;

        // Render selection background
        if (selection && selection->bgBrush && selection->end > selection->start)
        {
            //!!!TODO TEXT: render selection / background. Check if a glyph logical index is inside selection range passed.
            // Loop glyphs, collect sequences with the same bg color and image, draw batches! BG params may be defaulted?
            // Selection has a priority over BG. Can quickly skip selection rendering if range is empty. Can't do so with BG!
            //    const float selStartExtent = (d_selectionStart > 0) ? font->getTextExtent(d_text.substr(0, d_selectionStart)) : 0;
            //    const float selEndExtent = font->getTextExtent(d_text.substr(0, d_selectionStart + d_selectionLength));
            //    const Rectf selRect(pos.x + selStartExtent, pos.y, pos.x + selEndExtent, pos.y + line.extents.d_height);
            //    ImageRenderSettings imgRenderSettings(selRect, clipRect, ColourRect(0xFF002FFF));
            //    d_selectionImage->createRenderGeometry(out, imgRenderSettings);
        }

        // Render glyph chunks using their associated elements
        while (i < line.glyphEndIdx)
        {
            const auto startElementIdx = d_glyphs[i].elementIndex;
            const auto start = i;
            do ++i; while (i < line.glyphEndIdx && d_glyphs[i].elementIndex == startElementIdx);

            if (auto element = elements[startElementIdx].get())
                element->createRenderGeometry(out, &d_glyphs[start], i - start, penPosition,
                    modColours, clipRect, line.extents.d_height, line.justifySpaceSize, canCombineFromIdx);
        }

        // Move the pen to the new line
        penPosition.x = startX;
        penPosition.y += line.extents.d_height;
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
            const float glyphWidth = elements[glyph.elementIndex]->getGlyphWidth(glyph);

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
        prevGlyphWidth = elements[d_glyphs.back().elementIndex]->getGlyphWidth(d_glyphs.back());

    // Count full width for the last glyph in a line
    currLine->extents.d_width += (prevGlyphWidth - d_glyphs.back().advance);
    currLine->glyphEndIdx = glyphCount;

    // Track the paragraph exceeding an area width
    if (!d_wordWrap && currLine->extents.d_width > areaWidth)
        d_fitsIntoAreaWidth = false;
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateLineHeights(const std::vector<RenderedTextElementPtr>& elements, float defaultFontHeight)
{
    if (d_linesDirty)
        return;

    uint32_t i = 0;
    for (auto& line : d_lines)
    {
        if (!line.heightDirty)
        {
            i = line.glyphEndIdx;
            continue;
        }

        line.heightDirty = false;

        if (i == line.glyphEndIdx)
        {
            // An empty line has the height of the previous glyph's font
            if (i)
            {
                const auto fontSrc = elements[d_glyphs[i - 1].elementIndex].get();
                line.extents.d_height = fontSrc->getFont()->getFontHeight() +
                    fontSrc->getTopPadding() + fontSrc->getBottomPadding();
            }
            else
            {
                line.extents.d_height = defaultFontHeight;
            }
        }
        else
        {
            // A non-empty line has the height of the tallest element in it
            line.extents.d_height = 0.f;
            while (i < line.glyphEndIdx)
            {
                // Count height of the current element
                const auto elementIdx = d_glyphs[i].elementIndex;
                const float height = elements[elementIdx]->getHeight();
                if (line.extents.d_height < height)
                    line.extents.d_height = height;

                // Skip to the next element
                do ++i; while (i < line.glyphEndIdx && d_glyphs[i].elementIndex == elementIdx);
            }
        }
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
void RenderedTextParagraph::onElementWidthChanged(size_t elementIndex, float diff)
{
    if (d_linesDirty)
        return;

    uint32_t i = 0;
    for (auto& line : d_lines)
    {
        for (; i < line.glyphEndIdx; ++i)
        {
            if (elementIndex == d_glyphs[i].elementIndex)
            {
                // Any width change in a word wrapped paragraph may lead to changes in wrapping
                if (d_wordWrap)
                {
                    d_linesDirty = true;
                    return;
                }

                // Otherwise only line level recalculations are needed
                d_glyphs[i].advance += diff;
                line.extents.d_width += diff;
                line.horzFmtDirty = true;
            }
        }
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::onElementHeightChanged(size_t elementIndex, float diff)
{
    if (d_linesDirty)
        return;

    uint32_t i = 0;
    for (auto& line : d_lines)
    {
        while (i < line.glyphEndIdx)
        {
            if (elementIndex == d_glyphs[i].elementIndex)
            {
                // Invalidate line height and skip to the next line immediately
                line.heightDirty = true;
                i = line.glyphEndIdx;
            }
            else
            {
                ++i;
            }
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
void RenderedTextParagraph::setHorizontalFormatting(HorizontalTextFormatting fmt, bool breakDefault)
{
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setLastJustifiedLineHorizontalFormatting(HorizontalTextFormatting fmt, bool breakDefault)
{
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setWordWrappingEnabled(bool wrap, bool breakDefault)
{
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
