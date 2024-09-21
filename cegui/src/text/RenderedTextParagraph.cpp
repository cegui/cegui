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
#include "CEGUI/text/Font.h"
#include "CEGUI/text/FontGlyph.h"
#include "CEGUI/text/TextUtils.h"
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/GeometryBuffer.h"
#include <algorithm>

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setupGlyphs(const std::u32string& text,
    const std::vector<uint16_t>& elementIndices, const std::vector<RenderedTextElementPtr>& elements)
{
    d_linesDirty = true;

    const size_t elementIdxCount = elementIndices.size();
    const uint16_t defaultStyleIdx = static_cast<uint16_t>(elements.size() - 1);

    bool breakNext = false;
    for (auto& glyph : d_glyphs)
    {
        // NB: remapSourceIndices should not have been called yet!
        const auto utf32SourceIndex = glyph.sourceIndex;

        // Find associated element or associate with default text style
        glyph.elementIndex = (utf32SourceIndex < elementIdxCount) ?
            elementIndices[utf32SourceIndex] :
            defaultStyleIdx;

        const char32_t codePoint = text[utf32SourceIndex];

        // Do element-dependent initialization of the glyph
        if (auto element = elements[glyph.elementIndex].get())
            element->setupGlyph(glyph, codePoint);

        // Always breakable after whitespace and before/after wordbreak characters
        const bool isBreakable = (TextUtils::UTF32_NON_BREAKABLE_CHARACTERS.find(codePoint) == std::u32string::npos);
        const bool isBreakableWhitespace = (glyph.isWhitespace && isBreakable);
        if (breakNext)
        {
            glyph.isBreakable = true;
            breakNext = isBreakableWhitespace;
        }
        else
        {
            glyph.isBreakable = isBreakable && (TextUtils::UTF32_WORDBREAK_CHARACTERS.find(codePoint) != std::u32string::npos);
            breakNext = glyph.isBreakable || isBreakableWhitespace;
        }
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::remapSourceIndices(const std::vector<size_t>& originalIndices, size_t sourceLength)
{
    if (originalIndices.empty())
        return;

    const auto mapSize = originalIndices.size();

    for (auto& glyph : d_glyphs)
    {
        const auto utf32Index = glyph.sourceIndex;
        glyph.sourceIndex = static_cast<uint32_t>(originalIndices[utf32Index]);

        // Cache source length to detect being in the middle of multibyte glyph
        const auto nextIndex = (utf32Index + 1 < mapSize) ? originalIndices[utf32Index + 1] : sourceLength;
        glyph.sourceLength = static_cast<uint8_t>(nextIndex - glyph.sourceIndex);
    }

    d_sourceStartIndex = static_cast<uint32_t>(
        (d_sourceStartIndex < mapSize) ? originalIndices[d_sourceStartIndex] : sourceLength);
    d_sourceEndIndex = static_cast<uint32_t>(
        (d_sourceEndIndex < mapSize) ? originalIndices[d_sourceEndIndex] : sourceLength);
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::createRenderGeometry(std::vector<GeometryBuffer*>& out, glm::vec2& penPosition,
    const ColourRect* modColours, const Rectf* clipRect, const SelectionInfo* selection,
    const std::vector<RenderedTextElementPtr>& elements) const
{
    if (d_linesDirty)
        return;

    // Advance the pen to the start of the next paragraph
    auto initialPenPos = penPosition;
    penPosition.y += d_height + d_padding_y;

    // Determine the range of visible lines
    const auto lineCount = d_lines.size();
    size_t lineStart = 0;
    size_t lineEnd = lineCount;
    if (clipRect)
    {
        if (clipRect->empty())
            return;

        auto penPos = initialPenPos;
        for (size_t lineIdx = 0; lineIdx < lineCount; ++lineIdx)
        {
            const auto& line = d_lines[lineIdx];
            if (penPos.y >= clipRect->bottom())
            {
                lineEnd = lineIdx;
                break;
            }

            penPos.y += line.extents.d_height + line.padding_y;
            if (penPos.y <= clipRect->top())
            {
                lineStart = lineIdx + 1;
                initialPenPos.y = penPos.y;
            }
        }
    }

    if (lineStart >= lineEnd)
        return;

    const auto canCombineFromIdx = out.size();

    auto bgBuffer = &System::getSingleton().getRenderer()->createGeometryBufferColoured();

    // Render background
    auto penPos = initialPenPos;
    for (size_t lineIdx = lineStart; lineIdx < lineEnd; ++lineIdx)
    {
        const auto& line = d_lines[lineIdx];
        const float lineBottom = penPos.y + line.extents.d_height + line.padding_y;

        penPos.x = initialPenPos.x + line.horzOffset;

        Rectf rect(penPos.x, penPos.y, penPos.x, lineBottom);
        for (auto i = line.glyphStartIdx; i < line.glyphEndIdx; /**/)
        {
            const auto startElementIdx = d_glyphs[i].elementIndex;
            do
            {
                penPos.x += d_glyphs[i].advance;
                if (i < line.glyphSkipStartIdx && d_glyphs[i].isJustifiable)
                    penPos.x += line.justifySpaceSize;

                ++i;
            }
            while (i < line.glyphEndIdx && d_glyphs[i].elementIndex == startElementIdx);

            if (elements[startElementIdx]->hasBackgroundColour())
            {
                rect.d_max.x = penPos.x;
                bgBuffer->appendSolidRect(clipRect ? rect.getIntersection(*clipRect) : rect,
                    elements[startElementIdx]->getBackgroundColour());
            }
        }

        penPos.y = lineBottom;
    }

    // Render selection background (can be semi-transparent so must go after the main background)
    if (selection && selection->end > selection->start)
    {
        ImageRenderSettings settings(Rectf(), clipRect, selection->bgColours);

        penPos = initialPenPos;
        for (size_t lineIdx = lineStart; lineIdx < lineEnd; ++lineIdx)
        {
            const auto& line = d_lines[lineIdx];
            const float lineBottom = penPos.y + line.extents.d_height + line.padding_y;

            penPos.x = initialPenPos.x + line.horzOffset;

            settings.d_destArea.d_min = penPos;
            settings.d_destArea.d_max.x = penPos.x;
            settings.d_destArea.d_max.y = lineBottom;
            for (auto i = line.glyphStartIdx; i < line.glyphEndIdx; ++i)
            {
                const auto& glyph = d_glyphs[i];
                const bool hasSelection = !settings.d_destArea.empty();

                float glyphWidth = glyph.advance;
                if (i < line.glyphSkipStartIdx && glyph.isJustifiable)
                    glyphWidth += line.justifySpaceSize;

                if (glyph.sourceIndex >= selection->start && glyph.sourceIndex < selection->end)
                {
                    if (!hasSelection)
                        settings.d_destArea.d_max.x = settings.d_destArea.d_min.x;
                    settings.d_destArea.d_max.x += glyphWidth;
                }
                else
                {
                    // Draw currently collected selection and reset range
                    if (hasSelection)
                    {
                        if (selection->bgBrush)
                            selection->bgBrush->createRenderGeometry(out, settings, canCombineFromIdx);
                        else
                            bgBuffer->appendSolidRect(clipRect ? settings.d_destArea.getIntersection(*clipRect) : settings.d_destArea, selection->bgColours);
                        settings.d_destArea.d_min.x = settings.d_destArea.d_max.x;
                    }
                    settings.d_destArea.d_min.x += glyphWidth;
                }
            }

            // Draw selection to the end of the line
            if (!settings.d_destArea.empty())
            {
                if (selection->bgBrush)
                    selection->bgBrush->createRenderGeometry(out, settings, canCombineFromIdx);
                else
                    bgBuffer->appendSolidRect(clipRect ? settings.d_destArea.getIntersection(*clipRect) : settings.d_destArea, selection->bgColours);
            }

            penPos.y = lineBottom;
        }
    }

    // Apply accumulated background geometry
    if (bgBuffer->getVertexCount())
        out.push_back(bgBuffer);
    else
        System::getSingleton().getRenderer()->destroyGeometryBuffer(*bgBuffer);

    // Render main geometry
    penPos = initialPenPos;
    for (size_t lineIdx = lineStart; lineIdx < lineEnd; ++lineIdx)
    {
        const auto& line = d_lines[lineIdx];

        penPos.x = initialPenPos.x + line.horzOffset;

        // Render glyph chunks using their associated elements. Don't render skipped whitespace.
        for (auto i = line.glyphStartIdx; i < line.glyphSkipStartIdx; /**/)
        {
            const auto startElementIdx = d_glyphs[i].elementIndex;
            const auto start = i;
            do ++i; while (i < line.glyphSkipStartIdx && d_glyphs[i].elementIndex == startElementIdx);

            elements[startElementIdx]->createRenderGeometry(out, &d_glyphs[start], i - start, penPos, modColours,
                clipRect, line.extents.d_height, line.justifySpaceSize, canCombineFromIdx, selection);
        }

        penPos.y += line.extents.d_height + line.padding_y;
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
    auto currLineIt = d_lines.begin();
    (*currLineIt).glyphStartIdx = 0;

    const auto glyphCount = d_glyphs.size();
    if (!glyphCount)
    {
        (*currLineIt).glyphEndIdx = 0;
        (*currLineIt).glyphSkipStartIdx = 0;
        return;
    }

    // The difference between the full width and advance of the current glyph.
    // It is required for calculating the line width if it ends with this glyph.
    // This difference may be e.g. due to kerning at the line break point.
    float widthAdvanceDiff = 0.f;

    size_t breakIdx = std::numeric_limits<size_t>().max();
    size_t skipStartIdx = std::numeric_limits<size_t>().max();
    uint16_t breakJustifiableCount = 0;
    float breakLineWidth = 0.f;
    uint16_t visibleJustifiableCount = 0;
    float visibleLineWidth = 0.f;

    for (size_t i = 0; i < glyphCount; ++i)
    {
        auto currLine = &(*currLineIt);

        // Break too long lines if word wrapping is enabled
        if (d_wordWrap)
        {
            const float glyphWidth = elements[d_glyphs[i].elementIndex]->getGlyphWidth(d_glyphs[i]);
            const float lineWidth = currLine->extents.d_width + glyphWidth;
            if (lineWidth > areaWidth)
            {
                uint32_t wrapIdx = std::numeric_limits<uint32_t>().max();
                if (breakIdx < glyphCount)
                {
                    // There is a point suitable for word wrapping in the current line
                    wrapIdx = static_cast<uint32_t>(breakIdx);
                    currLine->extents.d_width = breakLineWidth;
                    currLine->justifiableCount = breakJustifiableCount;

                    // Don't break again at the same glyph to prevent infinite looping
                    breakIdx = std::numeric_limits<size_t>().max();
                }
                else
                {
                    // No break point was found. Remember that the word was broken in the middle.
                    d_fitsIntoAreaWidth = false;

                    // If this glyph is not the first in its line, wrap it to the new line. Being the first
                    // means that the glyph itself is wider than an area. Such glyph is printed as is, with overflow.
                    if (i > currLine->glyphStartIdx)
                    {
                        wrapIdx = static_cast<uint32_t>(i);
                        currLine->extents.d_width += widthAdvanceDiff;
                    }
                }

                if (wrapIdx < glyphCount)
                {
                    if (d_skipWrappedWhitespace)
                    {
                        // Find next non-whitespace. Don't skip anything if only whitespace remain.
                        for (uint32_t skipEndIdx = wrapIdx; skipEndIdx < glyphCount; ++skipEndIdx)
                        {
                            if (!d_glyphs[skipEndIdx].isWhitespace)
                            {
                                wrapIdx = skipEndIdx;
                                break;
                            }
                        }
                    }

                    currLine->glyphEndIdx = wrapIdx;
                    currLine->glyphSkipStartIdx = std::min(static_cast<uint32_t>(skipStartIdx), wrapIdx);
                    skipStartIdx = std::numeric_limits<size_t>().max();

                    // FIXME: right-to-left word wrapping must be implemented completely differently,
                    // this is just a hack to make it work at least to some extent
                    if (d_glyphs[wrapIdx].isRightToLeft)
                    {
                        const auto newLineIdx = std::distance(d_lines.begin(), currLineIt);
                        d_lines.emplace(currLineIt);
                        currLineIt = d_lines.begin() + newLineIdx;
                    }
                    else
                    {
                        d_lines.emplace_back();
                        currLineIt = std::prev(d_lines.end());
                    }

                    (*currLineIt).glyphStartIdx = wrapIdx;

                    // Cleared because the first glyph is not added to the line yet
                    widthAdvanceDiff = 0.f;

                    // Restart from the first wrapped glyph, 'i' will be incremented by the loop
                    i = wrapIdx - 1;
                    continue;
                }
            }
            else
            {
                if (i > currLine->glyphStartIdx && d_glyphs[i].isBreakable)
                {
                    // Remember this glyph as the most recent word wrapping point
                    breakIdx = i;
                    breakJustifiableCount = visibleJustifiableCount;
                    breakLineWidth = visibleLineWidth;
                }

                if (!d_skipWrappedWhitespace || !d_glyphs[i].isWhitespace)
                {
                    // Accept this glyph as the last visible content in the line
                    skipStartIdx = i + 1;
                    visibleJustifiableCount = currLine->justifiableCount;
                    if (d_glyphs[i].isJustifiable)
                        ++visibleJustifiableCount;
                    visibleLineWidth = lineWidth;
                }
            }

            widthAdvanceDiff = glyphWidth - d_glyphs[i].advance;
        }

        // Count the current glyph in the current line
        // NB: 'i' might have changed in the word wrapping code
        currLine->extents.d_width += d_glyphs[i].advance;
        if (d_glyphs[i].isJustifiable)
            ++currLine->justifiableCount;
    }

    // Calculate widthAdvanceDiff of the last glyph if not done during word wrapping
    if (!d_wordWrap)
    {
        const auto& glyph = d_glyphs.back();
        widthAdvanceDiff = elements[glyph.elementIndex]->getGlyphWidth(glyph) - glyph.advance;
    }

    auto currLine = &(*currLineIt);
    currLine->extents.d_width += widthAdvanceDiff;
    currLine->glyphEndIdx = static_cast<uint32_t>(glyphCount);
    currLine->glyphSkipStartIdx = static_cast<uint32_t>(glyphCount);

    // Track the paragraph exceeding an area width
    if (!d_wordWrap && currLine->extents.d_width > areaWidth)
        d_fitsIntoAreaWidth = false;
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateLineHeights(const std::vector<RenderedTextElementPtr>& elements, float defaultFontHeight)
{
    if (d_linesDirty)
        return;

    // TODO: get the whole style (not only font height) from the corresponding element of '\n'! There can be 
    //       a string like this: "\n\n[colour="..."][font="..."]\n\n", last two paragraphs must honor style changes!

    d_padding_y = 0.f;

    if (d_lines.empty())
    {
        d_height = defaultFontHeight;
        return;
    }

    d_height = 0.f;

    for (auto& line : d_lines)
    {
        if (!line.heightDirty)
        {
            d_height += line.extents.d_height;
            d_padding_y += line.padding_y;
            continue;
        }

        line.heightDirty = false;

        if (line.glyphStartIdx == line.glyphEndIdx)
        {
            // An empty line has the height of the previous glyph's font
            if (line.glyphStartIdx)
            {
                const auto fontSrc = elements[d_glyphs[line.glyphStartIdx - 1].elementIndex].get();
                line.extents.d_height = fontSrc->getFont()->getFontHeight();
                line.padding_y = fontSrc->getTopPadding() + fontSrc->getBottomPadding();
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
            for (auto i = line.glyphStartIdx; i < line.glyphEndIdx; /**/)
            {
                // Count height of the current element
                const auto elementIdx = d_glyphs[i].elementIndex;
                const float height = elements[elementIdx]->getHeight();
                if (line.extents.d_height < height)
                {
                    line.extents.d_height = height;
                    line.padding_y = elements[elementIdx]->getTopPadding() + elements[elementIdx]->getBottomPadding();
                }

                // Skip to the next element
                do ++i; while (i < line.glyphEndIdx && d_glyphs[i].elementIndex == elementIdx);
            }
        }

        d_height += line.extents.d_height;
        d_padding_y += line.padding_y;
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

        HorizontalTextFormatting lineHorzFmt;
        if (d_horzFormatting == HorizontalTextFormatting::Justified && (&line == &d_lines.back()))
            lineHorzFmt = d_lastJustifiedLineFormatting;
        else if (d_horzFormatting == HorizontalTextFormatting::Bidi)
            lineHorzFmt = (d_bidiDir == DefaultParagraphDirection::RightToLeft) ? HorizontalTextFormatting::RightAligned : HorizontalTextFormatting::LeftAligned;
        else
            lineHorzFmt = d_horzFormatting;

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
                if (line.justifiableCount && line.extents.d_width < areaWidth)
                    line.justifySpaceSize = (areaWidth - line.extents.d_width) / line.justifiableCount;
                break;
        }
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::accumulateExtents(Rectf& extents) const
{
    if (d_linesDirty)
        return;

    for (const auto& line : d_lines)
    {
        extents.d_max.y += line.extents.d_height + line.padding_y;

        if(extents.d_max.x < line.extents.d_width)
            extents.d_max.x = line.extents.d_width;
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::onElementWidthChanged(size_t elementIndex, float diff)
{
    const auto glyphCount = static_cast<uint32_t>(d_glyphs.size());
    auto firstGlyphIndex = glyphCount;

    // Glyph advances must be updated in any case
    for (uint32_t i = 0; i < glyphCount; ++i)
    {
        if (elementIndex == d_glyphs[i].elementIndex)
        {
            d_glyphs[i].advance += diff;
            if (firstGlyphIndex > i)
                firstGlyphIndex = i;
        }
    }

    if (d_linesDirty || firstGlyphIndex == glyphCount)
        return;

    // Any width change in a word wrapped paragraph may lead to changes in wrapping
    if (d_wordWrap)
    {
        d_linesDirty = true;
        return;
    }

    // Otherwise only line level recalculations are needed
    for (auto& line : d_lines)
    {
        for (auto i = std::max(line.glyphStartIdx, firstGlyphIndex); i < line.glyphEndIdx; ++i)
        {
            if (elementIndex == d_glyphs[i].elementIndex)
            {
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

    for (auto& line : d_lines)
    {
        for (auto i = line.glyphStartIdx; i < line.glyphEndIdx; /**/)
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
    else if (!d_linesDirty && d_horzFormatting != HorizontalTextFormatting::LeftAligned)
        for (auto& line : d_lines)
            line.horzFmtDirty = true;
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setHorizontalFormatting(HorizontalTextFormatting fmt, bool breakDefault)
{
    if (breakDefault)
        d_defaultHorzFormatting = false;

    if (fmt == d_horzFormatting)
        return;

    d_horzFormatting = fmt;

    if (!d_linesDirty)
        for (auto& line : d_lines)
            line.horzFmtDirty = true;
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setLastJustifiedLineFormatting(HorizontalTextFormatting fmt, bool breakDefault)
{
    if (breakDefault)
        d_defaultLastJustifiedLineFormatting = false;

    if (fmt == d_lastJustifiedLineFormatting)
        return;

    d_lastJustifiedLineFormatting = fmt;

    if (!d_linesDirty && d_horzFormatting == HorizontalTextFormatting::Justified)
        for (auto& line : d_lines)
            line.horzFmtDirty = true;
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::setWordWrapEnabled(bool wrap, bool breakDefault)
{
    if (breakDefault)
        d_defaultWordWrap = false;

    if (wrap == d_wordWrap)
        return;

    d_wordWrap = wrap;
    d_linesDirty = true;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getGlyphLineIndex(size_t glyphIndex) const
{
    if (d_linesDirty)
        return npos;

    // NB: lines can be not sorted by glyph index because of right-to-left word wrapping
    //const auto it = std::upper_bound(d_lines.cbegin(), d_lines.cend(), glyphIndex,
    //    [](uint32_t value, const Line& elm) { return value < elm.glyphEndIdx; });

    const auto it = std::find_if(d_lines.cbegin(), d_lines.cend(),
        [glyphIndex](const Line& elm) { return elm.glyphStartIdx <= glyphIndex && glyphIndex < elm.glyphEndIdx; });

    return (it == d_lines.cend()) ? npos : std::distance(d_lines.cbegin(), it);
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getTextIndexAtPoint(const glm::vec2& pt, float areaWidth, float* outRelPos) const
{
    return d_glyphs.empty() ? d_sourceStartIndex : getTextIndex(getGlyphIndexAtPoint(pt, areaWidth, outRelPos));
}

//----------------------------------------------------------------------------//
bool RenderedTextParagraph::getTextIndexBounds(Rectf& out, bool* outRtl, size_t textIndex,
    const std::vector<RenderedTextElementPtr>& elements, float areaWidth) const
{
    if (d_linesDirty)
        return false;

    const auto glyphIndex = getNearestGlyphIndex(textIndex);
    if (glyphIndex != npos)
        return getGlyphBounds(out, outRtl, glyphIndex, elements, areaWidth);

    // No glyph found, return the end of the paragraph
    if (d_lines.empty())
    {
        out.d_min.y = 0.f;
        out.d_min.x = 0.f;
    }
    else
    {
        const auto& lastLine = d_lines.back();
        out.d_min.y = d_height - lastLine.extents.d_height;
        out.d_min.x = lastLine.horzOffset;
        if (d_bidiDir != DefaultParagraphDirection::RightToLeft)
            out.d_min.x += lastLine.extents.d_width;
    }

    out.d_max.y = d_height;
    out.d_max.x = out.d_min.x;
    if (outRtl)
        *outRtl = (d_bidiDir == DefaultParagraphDirection::RightToLeft);

    return true;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getGlyphIndexAtPoint(const glm::vec2& pt, float areaWidth, float* outRelPos) const
{
    if (d_linesDirty || pt.y < 0.f)
        return npos;

    float lineLocalY = pt.y;
    for (size_t i = 0; i < d_lines.size(); ++i)
    {
        const auto& line = d_lines[i];

        if (line.heightDirty)
            return npos;

        if (lineLocalY <= line.extents.d_height)
            return getNearestGlyphIndex(i, pt.x, areaWidth, outRelPos);

        lineLocalY -= line.extents.d_height;
    }

    return npos;
}

//----------------------------------------------------------------------------//
bool RenderedTextParagraph::getGlyphBounds(Rectf& out, bool* outRtl, size_t glyphIndex,
    const std::vector<RenderedTextElementPtr>& elements, float areaWidth) const
{
    if (d_linesDirty || glyphIndex >= d_glyphs.size())
        return false;

    float lineY = 0.f;
    for (const auto& line : d_lines)
    {
        if (line.heightDirty)
            return false;

        if (glyphIndex < line.glyphStartIdx || glyphIndex >= line.glyphEndIdx)
        {
            lineY += line.extents.d_height;
            continue;
        }

        const auto& glyph = d_glyphs[glyphIndex];
        const auto element = elements[glyph.elementIndex].get();

        out.d_min.y = lineY;
        float scale = 1.f;
        element->applyVerticalFormatting(line.extents.d_height, out.d_min.y, scale);

        out.setHeight(element->getHeight());

        out.d_min.x = line.horzOffset;
        for (auto i = line.glyphStartIdx; i < glyphIndex; ++i)
        {
            out.d_min.x += d_glyphs[i].advance;
            if (i < line.glyphSkipStartIdx)
            {
                // Only not skipped glyphs participate in justification
                if (d_glyphs[i].isJustifiable)
                    out.d_min.x += line.justifySpaceSize;
            }
            else
            {
                // Skipped whitespace glyph is always clamped to the end of the line
                if (out.d_min.x > areaWidth)
                    break;
            }
        }

        out.setWidth(std::max(glyph.advance, element->getGlyphWidth(glyph)));

        if (glyphIndex < line.glyphSkipStartIdx)
        {
            if (glyph.isJustifiable)
                out.d_max.x += line.justifySpaceSize;
        }
        else
        {
            if (out.d_min.x > areaWidth)
                out.d_min.x = areaWidth;
            if (out.d_max.x > areaWidth)
                out.d_max.x = areaWidth;
        }

        if (outRtl)
            *outRtl = glyph.isRightToLeft;

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getTextIndex(size_t glyphIndex) const
{
    return (glyphIndex < d_glyphs.size()) ? d_glyphs[glyphIndex].sourceIndex : npos;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getTextIndex(size_t lineIndex, float offsetX, float areaWidth, float* outRelPos) const
{
    if (d_glyphs.empty())
        return d_sourceStartIndex;

    const auto idx = getNearestGlyphIndex(lineIndex, offsetX, areaWidth, outRelPos);
    if (idx == npos)
        return npos;

    if (idx >= d_glyphs.size())
        return d_sourceEndIndex;

    return d_glyphs[std::min(static_cast<uint32_t>(idx), d_lines[lineIndex].glyphEndIdx - 1)].sourceIndex;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getNearestGlyphIndex(size_t textIndex) const
{
    size_t nearestIdx = npos;
    size_t nearestSrcIdx = npos;
    for (size_t i = 0; i < d_glyphs.size(); ++i)
    {
        // Check if the glyph represents this very textIndex
        const auto srcIndex = d_glyphs[i].sourceIndex;
        if (srcIndex <= textIndex && textIndex < srcIndex + d_glyphs[i].sourceLength)
            return i;

        // Track the nearest existing glyph that goes after the requested position
        if (textIndex < srcIndex && srcIndex < nearestSrcIdx)
        {
            nearestIdx = i;
            nearestSrcIdx = srcIndex;
        }
    }

    return nearestIdx;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getNearestGlyphIndex(size_t lineIndex, float offsetX, float areaWidth, float* outRelPos) const
{
    const auto lineCount = d_lines.size();
    if (d_linesDirty || lineCount <= lineIndex)
        return npos;

    const auto& line = d_lines[lineIndex];
    const bool isRtl = (d_bidiDir == DefaultParagraphDirection::RightToLeft);

    // Point is to the left of the beginning
    if (offsetX < line.horzOffset)
    {
        if (outRelPos)
            *outRelPos = 0.f;
        return (isRtl && lineIndex + 1 == lineCount) ? d_glyphs.size() : line.glyphStartIdx;
    }

    float nextGlyphStart = line.horzOffset;
    for (auto i = line.glyphStartIdx; i < line.glyphEndIdx; ++i)
    {
        const float currGlyphStart = nextGlyphStart;
        nextGlyphStart += d_glyphs[i].advance;
        if (i < line.glyphSkipStartIdx)
        {
            if (d_glyphs[i].isJustifiable)
                nextGlyphStart += line.justifySpaceSize;
        }
        else
        {
            // Skipped whitespace glyphs are clamped to the area width, so extending past the
            // text bounds means that we skipped everything to the end of the line
            if (nextGlyphStart > areaWidth)
                break;
        }

        if (offsetX < nextGlyphStart)
        {
            if (outRelPos)
            {
                const float relPos = (offsetX - currGlyphStart) / (nextGlyphStart - currGlyphStart);
                *outRelPos = d_glyphs[i].isRightToLeft ? (1.f - relPos) : relPos;
            }
            return i;
        }
    }

    // Point is to the right of the end
    if (outRelPos)
        *outRelPos = 0.f;
    return (isRtl || lineIndex + 1 < lineCount) ? (line.glyphEndIdx - 1) : line.glyphEndIdx;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getLineIndex(size_t textIndex) const
{
    if (d_linesDirty || d_lines.empty())
        return npos;

    const auto idx = getNearestGlyphIndex(textIndex);
    return (idx != npos) ? getGlyphLineIndex(idx) : (d_lines.size() - 1);
}

//----------------------------------------------------------------------------//
float RenderedTextParagraph::getLineOffsetY(size_t lineIndex) const
{
    if (d_linesDirty || d_lines.size() <= lineIndex)
        return 0.f;

    float lineOffsetY = 0.f;
    for (size_t i = 0; i < lineIndex; ++i)
    {
        const auto& line = d_lines[i];
        if (line.heightDirty)
            break;
        lineOffsetY += line.extents.d_height + line.padding_y;
    }

    return lineOffsetY;
}

//----------------------------------------------------------------------------//
float RenderedTextParagraph::getLineHeight(size_t lineIndex) const
{
    if (d_linesDirty || d_lines.size() <= lineIndex || d_lines[lineIndex].heightDirty)
        return 0.f;
    return d_lines[lineIndex].extents.d_height + d_lines[lineIndex].padding_y;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getLineStartTextIndex(size_t lineIndex) const
{
    if (d_linesDirty || d_lines.size() <= lineIndex)
        return npos;

    if (d_glyphs.empty())
        return d_sourceStartIndex;

    return d_glyphs[d_lines[lineIndex].glyphStartIdx].sourceIndex;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::getLineEndTextIndex(size_t lineIndex) const
{
    if (d_linesDirty || d_lines.size() <= lineIndex)
        return npos;

    if (lineIndex + 1 == d_lines.size())
        return d_sourceEndIndex;

    return d_glyphs[d_lines[lineIndex].glyphEndIdx - 1].sourceIndex;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::nextTextIndex(size_t textIndex) const
{
    const auto idx = getNearestGlyphIndex(textIndex);
    return (idx + 1 < d_glyphs.size() && idx != npos) ? d_glyphs[idx + 1].sourceIndex : d_sourceEndIndex;
}

//----------------------------------------------------------------------------//
size_t RenderedTextParagraph::prevTextIndex(size_t textIndex) const
{
    if (textIndex >= d_sourceEndIndex)
    {
        // Handle paragraph ending (newline) which has no glyph
        const auto idx = getNearestGlyphIndex(d_sourceEndIndex - 1);
        return (idx == npos) ? d_sourceEndIndex : d_glyphs[idx].sourceIndex;
    }
    else
    {
        const auto idx = getNearestGlyphIndex(textIndex);
        return (idx == npos) ? d_sourceEndIndex : idx ? d_glyphs[idx - 1].sourceIndex : d_sourceStartIndex;
    }
}

}
