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
#include "CEGUI/text/RenderedText.h"
#include "CEGUI/text/RenderedTextParagraph.h"
#include "CEGUI/text/RenderedTextStyle.h"
#include "CEGUI/text/TextParser.h"
#include "CEGUI/text/TextUtils.h"
#ifdef CEGUI_BIDI_SUPPORT
#include "CEGUI/text/BidiVisualMapping.h"
#endif
#ifdef CEGUI_USE_RAQM
#include "CEGUI/text/FreeTypeFont.h"
#include <raqm.h>
#include <algorithm>
#else
#include "CEGUI/text/Font.h"
#include "CEGUI/text/FontGlyph.h"
#endif

namespace CEGUI
{
//----------------------------------------------------------------------------//
RenderedText::RenderedText() = default;
RenderedText::~RenderedText() = default;
RenderedText::RenderedText(RenderedText&&) noexcept = default;
RenderedText& RenderedText::operator =(RenderedText&&) noexcept = default;

//----------------------------------------------------------------------------//
static bool layoutParagraph(RenderedTextParagraph& out, const std::u32string& text,
    size_t start, size_t end, DefaultParagraphDirection dir,
    const std::vector<uint16_t>& elementIndices,
    const std::vector<RenderedTextElementPtr>& elements)
{
    // Apply Unicode Bidirectional Algorithm to obtain a string with visual ordering of codepoints
#if defined(CEGUI_BIDI_SUPPORT)
    //!!!TODO TEXT: implement partial inplace BIDI, can pass mutable "text" here!
    std::vector<int> l2v;
    std::vector<int> v2l;
    std::u32string textVisual;
    if (!BidiVisualMapping::applyBidi(text.c_str() + start, end - start, textVisual, l2v, v2l, dir))
        return false;

    out.setBidiDirection(dir);
#else
    const auto& textVisual = text;
#endif

    // Glyph generation

    out.glyphs().resize(end - start);

    const Font* currFont = nullptr;
    const FontGlyph* prevGlyph = nullptr;

    for (size_t i = start; i < end; ++i)
    {
#if defined(CEGUI_BIDI_SUPPORT)
        const size_t visualIndex = i - start;
        const size_t logicalIndex = v2l[visualIndex] + start;
#else
        const size_t visualIndex = i;
        const size_t logicalIndex = i;
#endif

        const auto codePoint = textVisual[visualIndex];
        auto& renderedGlyph = out.glyphs()[i - start];

        const auto elementIndex = (logicalIndex < elementIndices.size()) ?
            elementIndices[logicalIndex] :
            elements.size() - 1;

        // Get a font for the current character
        Font* font = elements[elementIndex]->getFont();
        if (!font)
            return false;

        renderedGlyph.fontGlyphIndex = font->getGlyphIndexForCodepoint(codePoint);
        auto fontGlyph = font->loadGlyph(renderedGlyph.fontGlyphIndex);
        if (fontGlyph)
        {
            const float kerning = font->getKerning(prevGlyph, *fontGlyph);
            renderedGlyph.offset.x = kerning;
            renderedGlyph.advance = fontGlyph->getAdvance() + kerning;
        }
        else
        {
            renderedGlyph.offset.x = 0.f;
            renderedGlyph.advance = 0.f;
        }
        renderedGlyph.sourceIndex = static_cast<uint32_t>(logicalIndex);
        renderedGlyph.sourceLength = 1;
        renderedGlyph.offset.y = font->getBaseline();
#if defined(CEGUI_BIDI_SUPPORT)
        renderedGlyph.isRightToLeft = (BidiVisualMapping::getBidiCharType(codePoint) == BidiCharType::RIGHT_TO_LEFT);
#else
        renderedGlyph.isRightToLeft = false;
#endif

        prevGlyph = fontGlyph;
    }

    return true;
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_RAQM
static bool layoutParagraphWithRaqm(RenderedTextParagraph& out, const std::u32string& text,
    size_t start, size_t end, DefaultParagraphDirection dir,
    const std::vector<uint16_t>& elementIndices,
    const std::vector<RenderedTextElementPtr>& elements, raqm_t*& rq)
{
    const size_t elementIdxCount = elementIndices.size();

    // Build font ranges an check if we can use raqm before we allocate something big inside it
    std::vector<std::pair<FreeTypeFont*, size_t>> fontRanges;
    if (start >= elementIdxCount)
    {
        // Only freetype fonts can be laid out with raqm
        auto ftDefaultFont = dynamic_cast<FreeTypeFont*>(elements.back()->getFont());
        if (!ftDefaultFont)
            return false;

        fontRanges.emplace_back(ftDefaultFont, end - start);
    }
    else
    {
        fontRanges.reserve(16);
        size_t fontLen = 0;
        FreeTypeFont* currFont = nullptr;
        for (size_t i = start; i < end; ++i)
        {
            const auto elementIndex = (i < elementIndices.size()) ?
                elementIndices[i] :
                elements.size() - 1;

            // Get a font for the current character
            Font* charFont = elements[elementIndex]->getFont();
            if (!charFont)
                return false;

            if (charFont != currFont)
            {
                // Only freetype fonts can be laid out with raqm
                auto ftCharFont = dynamic_cast<FreeTypeFont*>(charFont);
                if (!ftCharFont)
                    return false;

                if (fontLen)
                    fontRanges.emplace_back(currFont, fontLen);

                currFont = ftCharFont;
                fontLen = 0;
            }

            ++fontLen;
        }

        // Add the final range
        if (fontLen)
            fontRanges.emplace_back(currFont, fontLen);
    }

    // Now we know that we can use raqm for this paragraph
    if (rq)
    {
        raqm_clear_contents(rq);
    }
    else
    {
        rq = raqm_create();

        const raqm_direction_t raqmParagraphDir =
            (dir == DefaultParagraphDirection::RightToLeft) ? RAQM_DIRECTION_RTL :
            (dir == DefaultParagraphDirection::Automatic) ? RAQM_DIRECTION_DEFAULT :
            RAQM_DIRECTION_LTR;
        if (!raqm_set_par_direction(rq, raqmParagraphDir))
            return false;
    }

    // Assign only the paragraph text to raqm object
    if (!raqm_set_text(rq, reinterpret_cast<const uint32_t*>(text.c_str() + start), end - start))
        return false;

    // Assign font ranges to raqm
    size_t fontStart = 0;
    for (auto& range : fontRanges)
    {
        if (!raqm_set_freetype_face_range(rq, range.first->getFontFace(), fontStart, range.second))
            return false;

        if (!raqm_set_freetype_load_flags_range(rq, range.first->getGlyphLoadFlags(), fontStart, range.second))
            return false;

        fontStart += range.second;
        range.second = fontStart; // Change (font, len) into sorted (font, end) for glyph font detection below
    }

    if (!raqm_layout(rq))
        return false;

    const raqm_direction_t rqDir = raqm_get_par_resolved_direction(rq);
    out.setBidiDirection(
        (rqDir == RAQM_DIRECTION_RTL) ? DefaultParagraphDirection::RightToLeft :
        (rqDir == RAQM_DIRECTION_DEFAULT) ? DefaultParagraphDirection::Automatic :
        DefaultParagraphDirection::LeftToRight);

    // Glyph generation

    size_t rqGlyphCount = 0;
    raqm_glyph_t* rqGlyphs = raqm_get_glyphs(rq, &rqGlyphCount);
    out.glyphs().resize(rqGlyphCount);

    for (size_t i = 0; i < rqGlyphCount; ++i)
    {
        const raqm_glyph_t& rqGlyph = rqGlyphs[i];
        const raqm_direction_t rqGlyphDir = raqm_get_direction_at_index(rq, i);
        auto& renderedGlyph = out.glyphs()[i];

        // Find a font for our glyph
        auto it = std::upper_bound(fontRanges.begin(), fontRanges.end(), rqGlyph.cluster,
            [](uint32_t value, const std::pair<const FreeTypeFont*, size_t>& elm)
        {
            return value < elm.second;
        });

        FreeTypeFont* font = (*it).first;
        renderedGlyph.fontGlyphIndex = font->getGlyphIndexByFreetypeIndex(rqGlyph.index);

        font->loadGlyph(renderedGlyph.fontGlyphIndex);

        // A multiplication coefficient to convert 26.6 fixed point values into normal floats
        constexpr float s_26dot6_toFloat = (1.0f / 64.f);

        renderedGlyph.sourceIndex = static_cast<uint32_t>(rqGlyph.cluster + start);
        renderedGlyph.sourceLength = 1;
        renderedGlyph.offset.x = rqGlyph.x_offset * s_26dot6_toFloat;
        renderedGlyph.offset.y = rqGlyph.y_offset * s_26dot6_toFloat + font->getBaseline();
        renderedGlyph.advance = ((rqGlyphDir == RAQM_DIRECTION_TTB) ? rqGlyph.y_advance : rqGlyph.x_advance) * s_26dot6_toFloat;
        renderedGlyph.isRightToLeft = (rqGlyphDir == RAQM_DIRECTION_RTL);
    }

    return true;
}
#endif

//----------------------------------------------------------------------------//
bool RenderedText::renderText(const String& text, TextParser* parser,
    Font* defaultFont, DefaultParagraphDirection defaultParagraphDir)
{
    d_paragraphs.clear();
    d_elements.clear();
    d_defaultFont = defaultFont;

    if (text.empty())
        return true;

    // Parse a string and obtain UTF-32 text with embedded object placeholders but without tags
    std::u32string utf32Text;
    std::vector<size_t> originalIndices;
    std::vector<uint16_t> elementIndices;
    if (!parser || !parser->parse(text, utf32Text, originalIndices, elementIndices, d_elements))
    {
        // If no parser specified or parsing failed, render the text verbatim
#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_32)
        utf32Text = String::convertUtf8ToUtf32(text.c_str(), &originalIndices);
#else
        originalIndices.clear();
        utf32Text = text.getString();
#endif
    }

    // No text or placeholders left after parsing, there is nothing to render
    if (utf32Text.empty())
        return true;

    const size_t utf32TextLength = utf32Text.size();

    // There are characters without associated text style. Add a default one.
    if (elementIndices.size() < utf32TextLength)
        d_elements.emplace_back(new RenderedTextStyle());

    // Characters without an explicit font must use a default font
    for (auto& element : d_elements)
    {
        if (element->getFont())
            continue;

        if (!defaultFont)
            return false;

        element->setFont(defaultFont);
    }

#ifdef CEGUI_USE_RAQM
    raqm_t* rq = nullptr;
#endif

    // Perform layouting per paragraph
    size_t start = 0;
    DefaultParagraphDirection lastBidiDir = DefaultParagraphDirection::LeftToRight;
    do
    {
        size_t end = utf32Text.find_first_of(TextUtils::UTF32_NEWLINE_CHARACTERS, start);
        if (end == std::u32string::npos)
            end = utf32TextLength;

        // Always create a paragraph (new line), even if it is empty
        d_paragraphs.emplace_back(static_cast<uint32_t>(start), static_cast<uint32_t>(end));
        auto& p = d_paragraphs.back();

        if (end > start)
        {
            // Create and setup a sequence of CEGUI glyphs for this paragraph
#ifdef CEGUI_USE_RAQM
            if (!layoutParagraphWithRaqm(p, utf32Text, start, end, defaultParagraphDir, elementIndices, d_elements, rq))
#endif
                layoutParagraph(p, utf32Text, start, end, defaultParagraphDir, elementIndices, d_elements);

            // Inherit explicit direction from the previous text for direction neutral paragraphs
            if (p.getBidiDirection() == DefaultParagraphDirection::Automatic)
                p.setBidiDirection(lastBidiDir);
            else
                lastBidiDir = p.getBidiDirection();

            p.setupGlyphs(utf32Text, elementIndices, d_elements);
        }

        p.remapSourceIndices(originalIndices, text.size());

        if (end == utf32TextLength)
            break;

        // \r\n (CRLF) should be treated as a single newline according to Unicode spec
        if (end < utf32TextLength - 1 && utf32Text[end] == '\r' && utf32Text[end + 1] == '\n')
            ++end;

        start = end + 1;
    }
    while (true);

    // Push default formatting to paragraphs
    // NB: there should not be early exit when unchanged, paragraphs will handle this
    setHorizontalFormatting(d_horzFormatting);
    setLastJustifiedLineFormatting(d_lastJustifiedLineFormatting);
    setWordWrapEnabled(d_wordWrap);

#if defined(CEGUI_USE_RAQM)
    if (rq)
        raqm_destroy(rq);
#endif

    return true;
}

//----------------------------------------------------------------------------//
void RenderedText::updateDynamicObjectExtents(const Window* hostWindow)
{
    // Update metrics of dynamic objects and notify the text about their resizing
    for (size_t i = 0; i < d_elements.size(); ++i)
    {
        const auto diff = d_elements[i]->updateMetrics(hostWindow);
        if (diff.d_width)
            for (auto& p : d_paragraphs)
                p.onElementWidthChanged(i, diff.d_width);
        if (diff.d_height)
            for (auto& p : d_paragraphs)
                p.onElementHeightChanged(i, diff.d_height);
    }
}

//----------------------------------------------------------------------------//
bool RenderedText::updateFormatting(float areaWidth)
{
    if (areaWidth < 0.f)
        return false;

    const bool areaWidthChanged = (d_areaWidth != areaWidth);
    d_areaWidth = areaWidth;

    const float defaultFontHeight = d_defaultFont ? d_defaultFont->getFontHeight() : 0.f;

    Rectf extents;
    bool fitsIntoAreaWidth = true;
    for (auto& p : d_paragraphs)
    {
        if (areaWidthChanged)
            p.onAreaWidthChanged();

        p.updateLines(d_elements, areaWidth);
        p.updateLineHeights(d_elements, defaultFontHeight);
        p.updateHorizontalFormatting(areaWidth);

        p.accumulateExtents(extents);

        fitsIntoAreaWidth &= p.isFittingIntoAreaWidth();
    }

    d_extents = extents.getSize();
    if (d_paragraphs.empty())
        d_extents.d_height = defaultFontHeight;

    return fitsIntoAreaWidth;
}

//----------------------------------------------------------------------------//
void RenderedText::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const glm::vec2& position, const ColourRect* modColours, const Rectf* clipRect,
    const SelectionInfo* selection) const
{
    glm::vec2 penPosition = position;
    for (const auto& p : d_paragraphs)
        p.createRenderGeometry(out, penPosition, modColours, clipRect, selection, d_elements);
}

//----------------------------------------------------------------------------//
RenderedText RenderedText::clone() const
{
    RenderedText copy;
    for (const auto& component : d_elements)
        copy.d_elements.push_back(component->clone());
    copy.d_paragraphs = d_paragraphs;
    copy.d_defaultFont = d_defaultFont;
    copy.d_areaWidth = d_areaWidth;
    copy.d_horzFormatting = d_horzFormatting;
    copy.d_lastJustifiedLineFormatting = d_lastJustifiedLineFormatting;
    copy.d_wordWrap = d_wordWrap;
    return copy;
}

//----------------------------------------------------------------------------//
void RenderedText::setHorizontalFormatting(HorizontalTextFormatting fmt)
{
    d_horzFormatting = fmt;

    for (auto& p : d_paragraphs)
        if (p.isHorzFormattingDefault())
            p.setHorizontalFormatting(fmt, false);
}

//----------------------------------------------------------------------------//
void RenderedText::setLastJustifiedLineFormatting(HorizontalTextFormatting fmt)
{
    d_lastJustifiedLineFormatting = fmt;

    for (auto& p : d_paragraphs)
        if (p.isLastJustifiedLineFormattingDefault())
            p.setLastJustifiedLineFormatting(fmt, false);
}

//----------------------------------------------------------------------------//
void RenderedText::setWordWrapEnabled(bool wrap)
{
    d_wordWrap = wrap;

    for (auto& p : d_paragraphs)
        if (p.isWordWrapDefault())
            p.setWordWrapEnabled(wrap, false);
}

//----------------------------------------------------------------------------//
size_t RenderedText::getLineCount() const
{
    size_t count = 0;
    for (const auto& p : d_paragraphs)
        count += p.getLineCount();
    return count;
}

//----------------------------------------------------------------------------//
bool RenderedText::isFittingIntoAreaWidth() const
{
    for (const auto& p : d_paragraphs)
        if (!p.isFittingIntoAreaWidth())
            return false;
    return true;
}

//----------------------------------------------------------------------------//
size_t RenderedText::getTextIndexAtPoint(const glm::vec2& pt, float* outRelPos) const
{
    if (pt.y < 0.f)
        return 0;

    if (pt.y > d_extents.d_height)
        return npos;

    glm::vec2 localPt = pt;
    for (size_t i = 0; i < d_paragraphs.size(); ++i)
    {
        const auto& p = d_paragraphs[i];

        // Find the paragraph at the given height range
        const float paragraphHeight = p.getHeight();
        if (localPt.y > paragraphHeight)
        {
            localPt.y -= paragraphHeight;
            continue;
        }

        const auto idx = p.getTextIndexAtPoint(localPt, d_areaWidth, outRelPos);

        // No text at point means the end of the paragraph
        if (idx == RenderedTextParagraph::npos)
            return d_paragraphs[i].getSourceEndIndex();

        return idx;
    }

    return npos;
}

//----------------------------------------------------------------------------//
bool RenderedText::getTextIndexBounds(size_t textIndex, Rectf& out, bool* outRtl) const
{
    // We still have an empty line when there is no text at all
    if (d_paragraphs.empty())
    {
        switch (d_horzFormatting)
        {
            case HorizontalTextFormatting::CentreAligned: out.d_min.x = d_areaWidth * 0.5f; break;
            case HorizontalTextFormatting::RightAligned: out.d_min.x = d_areaWidth; break;
            default: out.d_min.x = 0.f; break;
        }
        out.d_max.x = out.d_min.x;
        out.d_min.y = 0.f;
        out.d_max.y = d_defaultFont ? d_defaultFont->getFontHeight() : 0.f;
        if (outRtl)
            *outRtl = false;
        return true;
    }

    float offsetY;
    const auto idx = findParagraphIndex(textIndex, offsetY);

    if (!d_paragraphs[idx].getTextIndexBounds(out, outRtl, textIndex, d_elements, d_areaWidth))
        return false;

    out.d_min.y += offsetY;
    out.d_max.y += offsetY;
    return true;
}

//----------------------------------------------------------------------------//
size_t RenderedText::nextTextIndex(size_t textIndex) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);

    const auto idx = d_paragraphs[parIdx].nextTextIndex(textIndex);
    if (textIndex == d_paragraphs[parIdx].getSourceEndIndex() && parIdx + 1 < d_paragraphs.size())
        return d_paragraphs[parIdx + 1].getSourceStartIndex();
    return idx;
}

//----------------------------------------------------------------------------//
size_t RenderedText::prevTextIndex(size_t textIndex) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);

    const auto idx = d_paragraphs[parIdx].prevTextIndex(textIndex);
    if (textIndex == d_paragraphs[parIdx].getSourceStartIndex() && parIdx > 0)
        return d_paragraphs[parIdx - 1].getSourceEndIndex();
    return idx;
}

//----------------------------------------------------------------------------//
size_t RenderedText::lineUpTextIndex(size_t textIndex, float desiredOffsetX) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    const auto& par = d_paragraphs[parIdx];
    const auto lineIdx = par.getLineIndex(textIndex);

    if (lineIdx)
        return par.getTextIndex(lineIdx - 1, desiredOffsetX, d_areaWidth);

    if (!parIdx)
        return textIndex;

    const auto& prevPar = d_paragraphs[parIdx - 1];
    return prevPar.getTextIndex(prevPar.getLineCount() - 1, desiredOffsetX, d_areaWidth);
}

//----------------------------------------------------------------------------//
size_t RenderedText::lineDownTextIndex(size_t textIndex, float desiredOffsetX) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    const auto& par = d_paragraphs[parIdx];
    const auto lineIdx = par.getLineIndex(textIndex);

    if (lineIdx + 1 < par.getLineCount())
        return par.getTextIndex(lineIdx + 1, desiredOffsetX, d_areaWidth);

    if (parIdx + 1 >= d_paragraphs.size())
        return textIndex;

    return d_paragraphs[parIdx + 1].getTextIndex(0, desiredOffsetX, d_areaWidth);
}

//----------------------------------------------------------------------------//
size_t RenderedText::pageUpTextIndex(size_t textIndex, float desiredOffsetX, float pageHeight) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    const auto& par = d_paragraphs[parIdx];
    const auto lineIdx = par.getLineIndex(textIndex);
    offsetY += par.getLineOffsetY(lineIdx);
    offsetY += par.getLineHeight(lineIdx) * 0.5f;

    return getTextIndexAtPoint(glm::vec2(desiredOffsetX, std::max(0.f, offsetY - pageHeight)));
}

//----------------------------------------------------------------------------//
size_t RenderedText::pageDownTextIndex(size_t textIndex, float desiredOffsetX, float pageHeight) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    const auto& par = d_paragraphs[parIdx];
    const auto lineIdx = par.getLineIndex(textIndex);
    offsetY += par.getLineOffsetY(lineIdx);
    offsetY += par.getLineHeight(lineIdx) * 0.5f;

    return getTextIndexAtPoint(glm::vec2(desiredOffsetX, std::min(d_extents.d_height, offsetY + pageHeight)));
}

//----------------------------------------------------------------------------//
size_t RenderedText::lineStartTextIndex(size_t textIndex) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    const auto& par = d_paragraphs[parIdx];
    return par.getLineStartTextIndex(par.getLineIndex(textIndex));
}

//----------------------------------------------------------------------------//
size_t RenderedText::lineEndTextIndex(size_t textIndex) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    const auto& par = d_paragraphs[parIdx];
    return par.getLineEndTextIndex(par.getLineIndex(textIndex));
}

//----------------------------------------------------------------------------//
size_t RenderedText::paragraphStartTextIndex(size_t textIndex) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    return d_paragraphs[parIdx].getSourceStartIndex();
}

//----------------------------------------------------------------------------//
size_t RenderedText::paragraphEndTextIndex(size_t textIndex) const
{
    if (d_paragraphs.empty())
        return npos;

    float offsetY;
    const auto parIdx = findParagraphIndex(textIndex, offsetY);
    return d_paragraphs[parIdx].getSourceEndIndex();
}

//----------------------------------------------------------------------------//
size_t RenderedText::endTextIndex() const
{
    return d_paragraphs.empty() ? npos : d_paragraphs.back().getSourceEndIndex();
}

//----------------------------------------------------------------------------//
size_t RenderedText::findParagraphIndex(size_t textIndex, float& offsetY) const
{
    offsetY = 0.f;

    const auto paragraphCount = d_paragraphs.size();
    for (size_t i = 0; i < paragraphCount; ++i)
    {
        const auto& p = d_paragraphs[i];
        if (i + 1 == paragraphCount || d_paragraphs[i + 1].getSourceStartIndex() > textIndex)
            return i;

        offsetY += p.getHeight();
    }

    return paragraphCount;
}

}
