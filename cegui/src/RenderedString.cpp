/***********************************************************************
    created:    25/05/2009
    author:     Paul Turner
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RenderedString.h"
#include "CEGUI/RenderedStringTextComponent.h"
#include "CEGUI/RenderedStringImageComponent.h"
#include "CEGUI/RenderedStringParser.h"
#include "CEGUI/Exceptions.h"
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
RenderedString::RenderedString()
{
    // set up initial line info
    appendLineBreak();
}

//----------------------------------------------------------------------------//
RenderedString::~RenderedString() = default;
RenderedString::RenderedString(RenderedString&&) noexcept = default;
RenderedString& RenderedString::operator =(RenderedString&&) noexcept = default;

//----------------------------------------------------------------------------//
static const Font* getFontAtIndex(size_t i, const std::vector<uint16_t>& elementIndices,
    const std::vector<RenderedStringComponentPtr>& elements,
    size_t& currFontSourceIdx, const Font* currFont)
{
    // Characters without an associated element use the default font
    if (i >= elementIndices.size())
        return nullptr;

    const auto charElementIdx = elementIndices[i];

    // Font couldn't change if the source element didn't
    if (currFontSourceIdx == charElementIdx)
        return currFont;

    auto textStyle = dynamic_cast<const RenderedStringTextComponent*>(elements[charElementIdx].get());

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
    const std::vector<RenderedStringComponentPtr>& elements)
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
    const std::vector<RenderedStringComponentPtr>& elements, raqm_t*& rq)
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
    if (!rq)
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
        renderedGlyph.advance = ((rqGlyphDir == RAQM_DIRECTION_TTB) ? rqGlyph.y_advance : rqGlyph.x_advance)  * s_26dot6_toFloat;
        renderedGlyph.isRightToLeft = (rqGlyphDir == RAQM_DIRECTION_RTL);
    }

    return true;
}
#endif

//----------------------------------------------------------------------------//
bool RenderedString::renderText(const String& text, RenderedStringParser* parser,
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

        //!!!FIXME TEXT: need no text string in text style elements!
        d_elements.emplace_back(new RenderedStringTextComponent(String::GetEmpty(), defaultFont));
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
bool RenderedString::format(float areaWidth, const Window* hostWindow)
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
void RenderedString::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const glm::vec2& position, const ColourRect* modColours, const Rectf* clipRect) const
{
    glm::vec2 penPosition = position;
    for (const auto& p : d_paragraphs)
        p.createRenderGeometry(out, penPosition, modColours, clipRect, d_elements);

    //???Do CR-LF after paragraphs here, not inside a paragraph? The last paragraph doesn't end with \n!
    //!!!but this would require to get paragraph's last line height here!
}

//----------------------------------------------------------------------------//
void RenderedString::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const Window* refWnd, const size_t line, const glm::vec2& position,
    const ColourRect* modColours, const Rectf* clipRect, float spaceExtra) const
{
    if (line >= d_lines.size())
        throw InvalidRequestException("line number specified is invalid.");

    const float renderHeight = getLineExtent(refWnd, line).d_height;

    glm::vec2 pos = position;
    const size_t end_component = d_lines[line].first + d_lines[line].second;
    for (size_t i = d_lines[line].first; i < end_component; ++i)
    {
        d_components[i]->createRenderGeometry(out, refWnd, pos, modColours, clipRect, renderHeight, spaceExtra);
        pos.x += d_components[i]->getPixelSize(refWnd).d_width;
    }
}

//----------------------------------------------------------------------------//
void RenderedString::appendComponent(const RenderedStringComponent& component)
{
    d_components.push_back(component.clone());
    ++d_lines.back().second;
}

//----------------------------------------------------------------------------//
void RenderedString::clearComponents()
{
    d_components.clear();
    d_lines.clear();
}

//----------------------------------------------------------------------------//
void RenderedString::appendLineBreak()
{
    const size_t first_component = d_lines.empty() ? 0 :
        d_lines.back().first + d_lines.back().second;

    d_lines.push_back({ first_component, 0 });
}

//----------------------------------------------------------------------------//
Sizef RenderedString::getLineExtent(const Window* refWnd, const size_t line) const
{
    if (line >= d_lines.size())
        throw InvalidRequestException("line number specified is invalid.");

    Sizef sz(0.f, 0.f);

    size_t i = d_lines[line].first;
    const size_t end_component = i + d_lines[line].second;
    for (; i < end_component; ++i)
    {
        const Sizef comp_sz = d_components[i]->getPixelSize(refWnd);
        sz.d_width += comp_sz.d_width;
        if (comp_sz.d_height > sz.d_height)
            sz.d_height = comp_sz.d_height;
    }

    return sz;
}

//----------------------------------------------------------------------------//
Sizef RenderedString::getExtent(const Window* refWnd) const
{
    Sizef totalExtent(0.f, 0.f);

    for (size_t i = 0; i < d_lines.size(); ++i)
    {
        const Sizef lineExtent = getLineExtent(refWnd, i);
        totalExtent.d_height += lineExtent.d_height;
        if (totalExtent.d_width < lineExtent.d_width)
            totalExtent.d_width = lineExtent.d_width;
    }

    return totalExtent;
}

//----------------------------------------------------------------------------//
size_t RenderedString::getSpaceCount(const size_t line) const
{
    if (line >= d_lines.size())
        throw InvalidRequestException("line number specified is invalid.");

    size_t space_count = 0;

    const size_t end_component = d_lines[line].first + d_lines[line].second;
    for (size_t i = d_lines[line].first; i < end_component; ++i)
        space_count += d_components[i]->getSpaceCount();

    return space_count;
}

//----------------------------------------------------------------------------//
void RenderedString::setSelection(const Window* refWnd, float start, float end)
{
    const size_t last_component = d_lines[0].second;
    float partial_extent = 0;
    size_t idx = 0;

    // clear last selection from all components
    for (size_t i = 0; i < d_components.size(); i++)
        d_components[i]->setSelection(refWnd, 0, 0);

    for (; idx < last_component; ++idx)
    {
        if (start <= partial_extent + d_components[idx]->getPixelSize(refWnd).d_width)
            break;
         partial_extent += d_components[idx]->getPixelSize(refWnd).d_width;
    }

    start -= partial_extent;
    end -= partial_extent;

    while (end > 0.0f)
    {
        const float comp_extent = d_components[idx]->getPixelSize(refWnd).d_width;
        d_components[idx]->setSelection(refWnd,
                                        start,
                                        (end >= comp_extent) ? comp_extent : end);
        start = 0;
        end -= comp_extent;
        ++idx;
    }
}

//----------------------------------------------------------------------------//
bool RenderedString::split(const Window* refWnd, const size_t line,
    float splitPoint, RenderedString& left)
{
    if (line >= d_lines.size())
        throw InvalidRequestException("line number specified is invalid.");

    left.clearComponents();

    if (d_components.empty())
        return false;

    // Move all components in lines prior to the line being split to the left
    if (line > 0)
    {
        // Move components to left side
        auto cb = d_components.begin();
        auto ce = cb + d_lines[line - 1].first + d_lines[line - 1].second;
        left.d_components.assign(std::make_move_iterator(cb), std::make_move_iterator(ce));
        d_components.erase(cb, ce);

        // Move lines to left side
        auto lb = d_lines.begin();
        auto le = lb + line;
        left.d_lines.assign(lb, le);
        d_lines.erase(lb, le);
    }

    // Find the component where the requested split point lies.
    size_t idx = 0;
    float partialExtent = 0.f;
    float splittedComponentWidth = 0.f;
    const size_t last_component = d_lines[0].second;
    for (; idx < last_component; ++idx)
    {
        splittedComponentWidth = d_components[idx]->getPixelSize(refWnd).d_width;
        partialExtent += splittedComponentWidth;
        if (splitPoint <= partialExtent)
            break;
    }

    // Components up to 'idx' are transfered to 'left'
    if (idx > 0)
    {
        auto cb = d_components.begin();
        auto ce = cb + idx;
        left.d_components.insert(left.d_components.end(), std::make_move_iterator(cb), std::make_move_iterator(ce));
        d_components.erase(cb, ce);
    }

    bool wasWordSplit = false;

    if (idx == last_component)
    {
        // Split point is past the end, move the whole line to left side
        left.d_lines.push_back(d_lines[0]);
        d_lines.erase(d_lines.begin());
    }
    else
    {
        left.appendLineBreak();
        auto& leftLine = left.d_lines[left.getLineCount() - 1];
        leftLine.second += idx;
        d_lines[0].second -= idx;

        // Now split item 'idx' between left and this (right).
        RenderedStringComponent* c = d_components[0].get();
        if (c->canSplit())
        {
            const float splittedComponentStart = partialExtent - splittedComponentWidth;
            const float localPt = splitPoint - splittedComponentStart;
            if (auto lc = c->split(refWnd, localPt, idx == 0, wasWordSplit))
            {
                left.d_components.push_back(std::move(lc));
                ++leftLine.second;
            }
        }
        // Can't split, if component width is >= splitPoint xfer the whole
        // component to it's own line in the left part (FIX #306)
        else if (splittedComponentWidth >= splitPoint)
        {
            left.appendLineBreak();
            left.d_components.push_back(std::move(d_components[0]));
            d_components.erase(d_components.begin());
            ++left.d_lines[left.getLineCount() - 1].second;
            --d_lines[0].second;
        }
    }

    // Fix up lines in this object
    for (size_t comp = 0, i = 0; i < d_lines.size(); ++i)
    {
        d_lines[i].first = comp;
        comp += d_lines[i].second;
    }

    return wasWordSplit;
}

//----------------------------------------------------------------------------//
RenderedString RenderedString::clone() const
{
    //!!!TODO TEXT: rewrite for the new logic!
    RenderedString copy;
    for (const auto& component : d_components)
        copy.d_components.push_back(component->clone());
    copy.d_lines = d_lines;
    return copy;
}

}
