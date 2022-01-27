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
#include "CEGUI/RenderedStringParser.h"
#ifdef CEGUI_BIDI_SUPPORT
#include "CEGUI/BidiVisualMapping.h"
#endif
#ifdef CEGUI_USE_RAQM
#include "CEGUI/FreeTypeFont.h"
#include <raqm.h>
#include <algorithm>
#else
#include "CEGUI/Font.h"
#include "CEGUI/FontGlyph.h"
#endif

namespace CEGUI
{
//----------------------------------------------------------------------------//
RenderedText::~RenderedText() = default;
RenderedText::RenderedText(RenderedText&&) noexcept = default;
RenderedText& RenderedText::operator =(RenderedText&&) noexcept = default;

//----------------------------------------------------------------------------//
static const Font* getFontAtIndex(size_t i, const std::vector<uint16_t>& elementIndices,
    const std::vector<RenderedTextElementPtr>& elements,
    size_t& currFontSourceIdx, const Font* currFont)
{
    // Characters without an associated element use the default font
    if (i >= elementIndices.size())
        return nullptr;

    const auto charElementIdx = elementIndices[i];

    // Font couldn't change if the source element didn't
    if (currFontSourceIdx == charElementIdx)
        return currFont;

    //???TODO TEXT: isEmbeddedObject can be set here or even pre-calculated before this!
    auto textStyle = dynamic_cast<const RenderedTextStyle*>(elements[charElementIdx].get());

    // Non-text elements have no font and are skipped
    if (!textStyle)
        return currFont;

    currFontSourceIdx = charElementIdx;

    return textStyle->getFont();
}

//----------------------------------------------------------------------------//
static bool layoutParagraph(RenderedTextParagraph& out, const std::u32string& text,
    size_t start, size_t end, const Font* defaultFont, DefaultParagraphDirection dir,
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

    out.d_bidiDir = dir;
#else
    const auto& textVisual = text;
#endif

    // Glyph generation

    out.d_glyphs.resize(end - start);

    size_t currFontSourceIdx = std::numeric_limits<size_t>().max(); // NB: intentionally invalid, elementIndices are uint16_t
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
        auto& renderedGlyph = out.d_glyphs[i - start];

        // Get a font for the current character
        const Font* font = getFontAtIndex(logicalIndex, elementIndices, elements, currFontSourceIdx, currFont);
        if (!font)
        {
            if (!defaultFont)
                return false;

            font = defaultFont;
        }

        const FontGlyph* fontGlyph = font->getGlyphForCodepoint(codePoint, true);
        if (!fontGlyph)
            fontGlyph = font->getReplacementGlyph();

        if (fontGlyph)
        {
            const float kerning = font->getKerning(prevGlyph, *fontGlyph);
            renderedGlyph.image = fontGlyph->getImage(0);
            renderedGlyph.offset.x = kerning;
            renderedGlyph.advance = fontGlyph->getAdvance() + kerning;
        }
        else
        {
            renderedGlyph.image = nullptr;
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
    size_t start, size_t end, const Font* defaultFont, DefaultParagraphDirection dir,
    const std::vector<uint16_t>& elementIndices,
    const std::vector<RenderedTextElementPtr>& elements, raqm_t*& rq)
{
    const size_t elementIdxCount = elementIndices.size();

    // Build font ranges an check if we can use raqm before we allocate something big inside it
    std::vector<std::pair<const FreeTypeFont*, size_t>> fontRanges;
    if (start >= elementIdxCount)
    {
        // Only freetype fonts can be laid out with raqm
        auto ftDefaultFont = dynamic_cast<const FreeTypeFont*>(defaultFont);
        if (!ftDefaultFont)
            return false;

        fontRanges.emplace_back(ftDefaultFont, end - start);
    }
    else
    {
        fontRanges.reserve(16);
        size_t fontLen = 0;
        size_t currFontSourceIdx = std::numeric_limits<size_t>().max(); // NB: intentionally invalid, elementIndices are uint16_t
        const FreeTypeFont* currFont = nullptr;
        for (size_t i = start; i < end; ++i)
        {
            // Get a font for the current character
            const Font* charFont = getFontAtIndex(i, elementIndices, elements, currFontSourceIdx, currFont);
            if (!charFont)
            {
                if (!defaultFont)
                    return false;

                charFont = defaultFont;
            }

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
    out.d_bidiDir = (rqDir == RAQM_DIRECTION_RTL) ? DefaultParagraphDirection::RightToLeft : DefaultParagraphDirection::LeftToRight;

    // Glyph generation

    size_t rqGlyphCount = 0;
    raqm_glyph_t* rqGlyphs = raqm_get_glyphs(rq, &rqGlyphCount);
    out.d_glyphs.resize(rqGlyphCount);

    for (size_t i = 0; i < rqGlyphCount; ++i)
    {
        const raqm_glyph_t& rqGlyph = rqGlyphs[i];
        const raqm_direction_t rqGlyphDir = raqm_get_direction_at_index(rq, i);
        auto& renderedGlyph = out.d_glyphs[i];

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

        renderedGlyph.image = fontGlyph ? fontGlyph->getImage(0) : nullptr;
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
bool RenderedText::renderText(const String& text, RenderedStringParser* parser,
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
    {
        // Characters without explicit styles require a default font
        if (!defaultFont)
            return false;

        d_elements.emplace_back(new RenderedTextStyle(defaultFont));
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

        if (end > start)
        {
            // Create and setup a sequence of CEGUI glyphs for this paragraph
#ifdef CEGUI_USE_RAQM
            if (!layoutParagraphWithRaqm(d_paragraphs.back(), utf32Text, start, end, defaultFont, defaultParagraphDir,
                elementIndices, d_elements, rq))
#endif
                layoutParagraph(d_paragraphs.back(), utf32Text, start, end, defaultFont, defaultParagraphDir,
                    elementIndices, d_elements);

            d_paragraphs.back().setupGlyphs(utf32Text, originalIndices, elementIndices, d_elements);
        }

        if (end == textLength)
            break;
        else
            start = end + 1;
    }
    while (true);

#if defined(CEGUI_USE_RAQM)
    if (rq)
        raqm_destroy(rq);
#endif

    return true;
}

//----------------------------------------------------------------------------//
bool RenderedText::format(float areaWidth, const Window* hostWindow)
{
    if (areaWidth < 0.f)
        return false;

    const bool areaWidthChanged = (d_areaWidth != areaWidth);

    d_areaWidth = areaWidth;

    bool fitsIntoAreaWidth = true;

    for (auto& p : d_paragraphs)
    {
        if (areaWidthChanged)
            p.onAreaWidthChanged();

        p.updateEmbeddedObjectExtents(d_elements, hostWindow);
        p.updateLines(d_elements, areaWidth);
        p.updateLineHeights(d_defaultFont->getFontHeight());
        p.updateHorizontalFormatting(areaWidth);

        fitsIntoAreaWidth &= p.isFittingIntoAreaWidth();
    }

    return fitsIntoAreaWidth;
}

//----------------------------------------------------------------------------//
void RenderedText::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const glm::vec2& position, const ColourRect* modColours, const Rectf* clipRect) const
{
    glm::vec2 penPosition = position;
    for (const auto& p : d_paragraphs)
        p.createRenderGeometry(out, penPosition, modColours, clipRect, d_elements);

    //???Do CR-LF after paragraphs here, not inside a paragraph? The last paragraph doesn't end with \n!
    //!!!but this would require to get paragraph's last line height here!
}

}
