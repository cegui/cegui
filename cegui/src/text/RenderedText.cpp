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
        const Font* font = elements[elementIndex]->getFont();
        if (!font)
            return false;

        const FontGlyph* fontGlyph = font->getGlyphForCodepoint(codePoint, true);
        if (!fontGlyph)
            fontGlyph = font->getReplacementGlyph();

        if (fontGlyph)
        {
            const float kerning = font->getKerning(prevGlyph, *fontGlyph);
            renderedGlyph.fontGlyph = fontGlyph;
            renderedGlyph.offset.x = kerning;
            renderedGlyph.advance = fontGlyph->getAdvance() + kerning;
        }
        else
        {
            renderedGlyph.fontGlyph = nullptr;
            renderedGlyph.offset.x = 0.f;
            renderedGlyph.advance = 0.f;
        }
        renderedGlyph.sourceIndex = static_cast<uint32_t>(logicalIndex);
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
    std::vector<std::pair<const FreeTypeFont*, size_t>> fontRanges;
    if (start >= elementIdxCount)
    {
        // Only freetype fonts can be laid out with raqm
        auto ftDefaultFont = dynamic_cast<const FreeTypeFont*>(elements.back()->getFont());
        if (!ftDefaultFont)
            return false;

        fontRanges.emplace_back(ftDefaultFont, end - start);
    }
    else
    {
        fontRanges.reserve(16);
        size_t fontLen = 0;
        const FreeTypeFont* currFont = nullptr;
        for (size_t i = start; i < end; ++i)
        {
            const auto elementIndex = (i < elementIndices.size()) ?
                elementIndices[i] :
                elements.size() - 1;

            // Get a font for the current character
            const Font* charFont = elements[elementIndex]->getFont();
            if (!charFont)
                return false;

            if (charFont != currFont)
            {
                // Only freetype fonts can be laid out with raqm
                auto ftCharFont = dynamic_cast<const FreeTypeFont*>(charFont);
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

        if (!raqm_set_freetype_load_flags_range(rq, range.first->getGlyphLoadFlags(0), fontStart, range.second))
            return false;

        fontStart += range.second;
        range.second = fontStart; // Change (font, len) into sorted (font, end) for glyph font detection below
    }

    if (!raqm_layout(rq))
        return false;

    const raqm_direction_t rqDir = raqm_get_par_resolved_direction(rq);
    out.setBidiDirection((rqDir == RAQM_DIRECTION_RTL) ?
        DefaultParagraphDirection::RightToLeft :
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

        const FreeTypeFont* font = (*it).first;
        const FontGlyph* fontGlyph = font->getGlyphByIndex(rqGlyph.index, true);
        if (!fontGlyph)
            fontGlyph = font->getReplacementGlyph();

        // A multiplication coefficient to convert 26.6 fixed point values into normal floats
        constexpr float s_26dot6_toFloat = (1.0f / 64.f);

        renderedGlyph.fontGlyph = fontGlyph;
        renderedGlyph.sourceIndex = static_cast<uint32_t>(rqGlyph.cluster + start);
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
    const Font* defaultFont, DefaultParagraphDirection defaultParagraphDir)
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
        utf32Text = text.getString(); //???can avoid copying? e.g. passing utf32Text further as an arg? or mutability is useful later?
#endif
    }

    // No text or placeholders left after parsing, there is nothing to render
    if (utf32Text.empty())
        return true;

    // There are characters without associated text style. Add a default one.
    if (elementIndices.size() < utf32Text.size())
        d_elements.emplace_back(new RenderedTextStyle());

    // Characters without an explicit font ust use a default font
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
    const size_t textLength = utf32Text.size();
    size_t start = 0;
    do
    {
        size_t end = utf32Text.find('\n', start);
        if (end == std::u32string::npos)
            end = textLength;

        // Always create a paragraph (new line), even if it is empty
        d_paragraphs.emplace_back();
        auto& p = d_paragraphs.back();
        p.setSourceStartIndex(start);

        if (end > start)
        {
            // Create and setup a sequence of CEGUI glyphs for this paragraph
            //!!!TODO TEXT: move into paragraph? Seems right but how to reuse raqm_t?!
#ifdef CEGUI_USE_RAQM
            if (!layoutParagraphWithRaqm(p, utf32Text, start, end, defaultParagraphDir,
                elementIndices, d_elements, rq))
#endif
                layoutParagraph(p, utf32Text, start, end, defaultParagraphDir,
                    elementIndices, d_elements);

            //!!!TODO TEXT: set non-default formatting of this paragraph if specified explicitly!

            p.setupGlyphs(utf32Text, originalIndices, elementIndices, d_elements);
        }

        if (end == textLength)
            break;
        else
            start = end + 1;
    }
    while (true);

    // Push default formatting to paragraphs
    // NB: there should not be early exit when unchanged, paragraphs will handle this
    setHorizontalFormatting(d_horzFormatting);
    setLastJustifiedLineHorizontalFormatting(d_lastJustifiedLineHorzFormatting);
    setWordWrappingEnabled(d_wordWrap);

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

    Rectf extents;
    bool fitsIntoAreaWidth = true;
    for (auto& p : d_paragraphs)
    {
        if (areaWidthChanged)
            p.onAreaWidthChanged();

        p.updateLines(d_elements, areaWidth);
        p.updateLineHeights(d_elements, d_defaultFont ? d_defaultFont->getFontHeight() : 0.f);
        p.updateHorizontalFormatting(areaWidth);

        p.accumulateExtents(extents);

        fitsIntoAreaWidth &= p.isFittingIntoAreaWidth();
    }

    d_extents = extents.getSize();

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
    copy.d_lastJustifiedLineHorzFormatting = d_lastJustifiedLineHorzFormatting;
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
void RenderedText::setLastJustifiedLineHorizontalFormatting(HorizontalTextFormatting fmt)
{
    d_lastJustifiedLineHorzFormatting = fmt;

    for (auto& p : d_paragraphs)
        if (p.isLastJustifiedLineHorzFormattingDefault())
            p.setLastJustifiedLineHorizontalFormatting(fmt, false);
}

//----------------------------------------------------------------------------//
void RenderedText::setWordWrappingEnabled(bool wrap)
{
    d_wordWrap = wrap;

    for (auto& p : d_paragraphs)
        if (p.isWordWrappingDefault())
            p.setWordWrappingEnabled(wrap, false);
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
size_t RenderedText::getTextIndexAtPoint(const glm::vec2& pt) const
{
    if (pt.y < 0.f || pt.y > d_extents.d_height)
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

        const auto idx = p.getTextIndexAtPoint(localPt);

        // No text at point means the end of the paragraph
        if (idx == RenderedTextParagraph::npos && i + 1 < d_paragraphs.size())
            return d_paragraphs[i + 1].getSourceStartIndex() - 1;

        return idx;
    }

    return npos;
}

//----------------------------------------------------------------------------//
bool RenderedText::getTextIndexBounds(size_t textIndex, Rectf& out) const
{
    float offsetY;
    const auto idx = findParagraphIndex(textIndex, offsetY);
    if (idx >= d_paragraphs.size())
        return false; //!!!TODO TEXT: return end-of-text rect!

    //???FIXME TEXT: need bool result? always return sane rect?!

    if (!d_paragraphs[idx].getTextIndexBounds(out, textIndex, d_elements))
        return false;

    out.d_min.y += offsetY;
    out.d_max.y += offsetY;
    return true;
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
