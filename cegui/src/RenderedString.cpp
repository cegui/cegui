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
static bool layoutParagraph(RenderedParagraph& out, const std::u32string& text,
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

    out.bidiDir = dir;
#else
    const auto& textVisual = text;
#endif

    // Glyph generation

    out.glyphs.resize(end - start);

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
        auto& renderedGlyph = out.glyphs[i - start];

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
            renderedGlyph.image = fontGlyph->getImage(0);
            renderedGlyph.offset.x = font->getKerning(prevGlyph, *fontGlyph);
            renderedGlyph.advance = fontGlyph->getAdvance();
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
static bool layoutParagraphWithRaqm(RenderedParagraph& out, const std::u32string& text,
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
    out.bidiDir = (rqDir == RAQM_DIRECTION_RTL) ? DefaultParagraphDirection::RightToLeft : DefaultParagraphDirection::LeftToRight;

    // Glyph generation

    size_t rqGlyphCount = 0;
    raqm_glyph_t* rqGlyphs = raqm_get_glyphs(rq, &rqGlyphCount);
    out.glyphs.resize(rqGlyphCount);

    for (size_t i = 0; i < rqGlyphCount; ++i)
    {
        const raqm_glyph_t& rqGlyph = rqGlyphs[i];
        const raqm_direction_t rqGlyphDir = raqm_get_direction_at_index(rq, i);
        auto& renderedGlyph = out.glyphs[i];

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
            // Create a sequence of CEGUI glyphs for this paragraph
#ifdef CEGUI_USE_RAQM
            if (!layoutParagraphWithRaqm(d_paragraphs.back(), utf32Text, start, end, defaultFont, defaultParagraphDir,
                    elementIndices, d_elements, rq))
#endif
                layoutParagraph(d_paragraphs.back(), utf32Text, start, end, defaultFont, defaultParagraphDir,
                    elementIndices, d_elements);
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

    // Do common setup for generated glyphs
    const uint16_t defaultStyleIdx = static_cast<uint16_t>(d_elements.size() - 1);
    const size_t elementIdxCount = elementIndices.size();
    const bool adjustSourceIndices = !originalIndices.empty();
    for (auto& p : d_paragraphs)
    {
        for (auto& glyph : p.glyphs)
        {
            // Find associated element or associate with default text style
            glyph.elementIndex = (glyph.sourceIndex < elementIdxCount) ?
                elementIndices[glyph.sourceIndex] :
                defaultStyleIdx;

            // Make source index point to an original string passed in "text" arg
            if (adjustSourceIndices)
                glyph.sourceIndex = static_cast<uint32_t>(originalIndices[glyph.sourceIndex]);

            const RenderedStringComponent* element = d_elements[glyph.elementIndex].get();
            auto textStyle = static_cast<const RenderedStringTextComponent*>(element);
            glyph.isEmbeddedObject = !textStyle;

            glyph.offset += element->getPadding().getPosition();

            //!!!TODO TEXT: how must be padding applied to RTL characters? Should L/R padding be inverted or not?
            //if (glyph.isRightToLeft) ...

            // FIXME TEXT: virtual RenderedTextElement::setupGlyph() instead of dynamic casts?!
            if (glyph.isEmbeddedObject)
            {
                if (auto embeddedImage = dynamic_cast<const RenderedStringImageComponent*>(element))
                    glyph.image = embeddedImage->getImage();

                glyph.isJustifyable = false;
                glyph.isBreakable = true; // May be not always, but for now this is OK
                glyph.isWhitespace = false;
            }
            else
            {
                // Bake padding into glyph metrics. Text glyphs are never resized and will
                // remain actual. Embedded objects metrics will be calculated in format().
                glyph.advance += element->getLeftPadding() + element->getRightPadding();
                glyph.height = textStyle->getFont()->getFontHeight() +
                    element->getTopPadding() + element->getBottomPadding();

                const auto codePoint = text[glyph.sourceIndex];
                glyph.isJustifyable = (codePoint == ' ');
                glyph.isBreakable = (codePoint == ' ' || codePoint == '\t' || codePoint == '\r');
                glyph.isWhitespace = glyph.isBreakable;
            }
        }
    }

    return true;
}

//----------------------------------------------------------------------------//
float RenderedString::format(float areaWidth, const Window* hostWindow)
{
    //!!!FIXME TEXT: for word wrapped lines this is the next glyph size only, not the whole wrapped tail of the word!
    //???calc and cache per paragraph?! move code from here to a paragraph class?! too many paragraph loops!
    //!!!invalidate paragraph dirty flags from RenderedString! don't store areaWidth etc in each!
    float maxExcessWidth = std::numeric_limits<float>().lowest();

    // Update extents of dynamically sizeable objects
    for (auto& p : d_paragraphs)
    {
        for (auto& glyph : p.glyphs)
        {
            if (!glyph.isEmbeddedObject)
                continue;

            auto element = d_elements[glyph.elementIndex].get();
            if (!element)
                continue;

            // NB: padding is already counted inside
            const Sizef size = element->getPixelSize(hostWindow);

            if (!p.linesDirty)
            {
                if (size.d_width != glyph.advance)
                {
                    if (p.wordWrap)
                        p.linesDirty = true;
                    else
                    {
                        //get line, patch its width += (glyphSize.d_width - glyph.advance);
                    }
                }

                if (!p.linesDirty && size.d_height != glyph.height)
                {
                    //get line, invalidate its height
                }
            }

            glyph.advance = size.d_width;
            glyph.height = size.d_height;
        }
    }

    // Build paragraph lines with optional word wrapping, cache line widths
    for (auto& p : d_paragraphs)
    {
        if (!p.linesDirty)
            continue;

        p.linesDirty = false;

        p.lines.clear();

        // Always add the first line of the paragraph
        p.lines.emplace_back();
        auto* currLine = &p.lines.back();

        const auto glyphCount = p.glyphs.size();
        if (!glyphCount)
        {
            currLine->glyphEndIdx = 0;
            continue;
        }

        // Word wrapping breakpoint tracking
        size_t lastBreakPointIdx = std::numeric_limits<size_t>().max();
        RenderedParagraph::Line lastBreakPointState;
        float lastBreakPointWidthAdjustment = 0.f;

        float prevGlyphWidth = 0.f;
        uint32_t lineStartGlyphIdx = 0;
        for (size_t i = 0; i < glyphCount; ++i)
        {
            auto& glyph = p.glyphs[i];

            // Break too long lines if word wrapping is enabled
            if (p.wordWrap)
            {
                // Calculate the full width of the glyph, including padding
                float glyphWidth = glyph.advance;
                if (!glyph.isEmbeddedObject)
                {
                    // For text glyphs the full width may be greater than advance due to kerning
                    float visualWidth = glyph.image ? glyph.image->getRenderedSize().d_width : 0.f;
                    if (auto element = d_elements[glyph.elementIndex].get())
                        visualWidth += (element->getLeftPadding() + element->getRightPadding());
                    if (glyphWidth < visualWidth)
                        glyphWidth = visualWidth;
                }

                float excessWidth = currLine->extents.d_width + glyphWidth - areaWidth;
                if (excessWidth > 0.f)
                {
                    // If there is a good break point for word wrapping in the current line, use it
                    if (lastBreakPointIdx < glyphCount)
                    {
                        // Transfer everything past the last breakpoint to the new line
                        p.lines.emplace_back();
                        auto newLine = &p.lines.back();
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
                        if (maxExcessWidth < excessWidth)
                            maxExcessWidth = excessWidth;

                        // If this glyph is not the first in its line, transfer it to its own new line
                        if (i > lineStartGlyphIdx)
                        {
                            // Compensate possible difference (e.g. due to kerning at the breakpoint)
                            currLine->extents.d_width += (prevGlyphWidth - p.glyphs[i - 1].advance);
                            currLine->glyphEndIdx = static_cast<uint32_t>(i);
                            lineStartGlyphIdx = currLine->glyphEndIdx;

                            p.lines.emplace_back();
                            currLine = &p.lines.back();
                        }
                    }
                }
                else if (glyph.isBreakable)
                {
                    // Remember this glyph as the most recent word wrapping point
                    lastBreakPointIdx = i;
                    lastBreakPointState = *currLine;
                    lastBreakPointWidthAdjustment =
                        (i > lineStartGlyphIdx) ? (prevGlyphWidth - p.glyphs[i - 1].advance) : 0.f;
                }

                prevGlyphWidth = glyphWidth;
            }

            // Count the current glyph in the current line
            currLine->extents.d_width += glyph.advance;
            if (glyph.isJustifyable)
                ++currLine->justifyableCount;
        }

        // Track the paragraph exceeding an area width
        if (!p.wordWrap)
        {
            const float excessWidth = p.lines.back().extents.d_width - areaWidth;
            if (maxExcessWidth < excessWidth)
                maxExcessWidth = excessWidth;
        }

        // Count full width for the last glyph in a line
        currLine->extents.d_width += (prevGlyphWidth - p.glyphs.back().advance);
        currLine->glyphEndIdx = glyphCount;
    }

    // Update cached line heights
    for (auto& p : d_paragraphs)
    {
        uint32_t lineStartGlyphIdx = 0;
        for (auto& line : p.lines)
        {
            //if (p/line.heightDirty)
            {
                //p/line.heightDirty = false;

                if (lineStartGlyphIdx == line.glyphEndIdx)
                {
                    // An empty line uses the height of the previous text glyph
                    uint32_t i = lineStartGlyphIdx;
                    for (; i > 0; --i)
                    {
                        if (!p.glyphs[i].isEmbeddedObject)
                        {
                            line.extents.d_height = p.glyphs[i].height;
                            break;
                        }
                    }

                    if (!i)
                        line.extents.d_height = (!p.glyphs[i].isEmbeddedObject) ? p.glyphs[i].height : d_defaultFont->getFontHeight();
                }
                else
                {
                    line.extents.d_height = 0.f;
                    for (size_t i = lineStartGlyphIdx; i < line.glyphEndIdx; ++i)
                    {
                        const float glyphHeight = p.glyphs[i].height;
                        if (line.extents.d_height < glyphHeight)
                            line.extents.d_height = glyphHeight;
                    }
                }
            }

            lineStartGlyphIdx = line.glyphEndIdx;
        }
    }

    // Update horizontal alignment of lines
    for (auto& p : d_paragraphs)
    {
        for (auto& line : p.lines)
        {
            const bool isLastLine = (&line == &p.lines.back());
            const auto lineHorzFmt = (isLastLine && p.horzFormatting == HorizontalTextFormatting::Justified) ?
                p.lastJustifiedLineHorzFormatting :
                p.horzFormatting;

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

    return maxExcessWidth;
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


////////////////////// NEW CODE WIP:

    //???force using cached advance of embedded image instead of current size until re-formatted?

    //!!!pass selection range here, draw selection brush under instances with logical(?) index inside selection!

    //!!!TODO TEXT: need default style here! not only colors but also underline flag etc!
    //???how to know where to apply a default style? now this style element is the same as explicit ones!

    //???!!!render all lines at once?! no need in per-line rendering? or want to generate geometry for a line range?!
    if (line >= d_paragraphs.size())
        return;

    const auto& p = d_paragraphs[line];

    //???when actualized?! embedded objects might have been resized. Update embedded only here?!
    //!!!glyphs never change, use this fact! only embedded objects do. E.g. may cache only text glyph extents
    //and count of embedded objects, and now add sizes of embedded objects until count reached!
    //!!!need to distinguish embedded objects from text glyphs fast! nullptr image? or special flag?!
    //const float renderHeight = p.extents.d_height;

    //!!!!!!!!FIXME TEXT: modColours may be null!!!!!! and it's not the same as default text, bg or outline color

    //!!!TODO TEXT: how to render selection?! check each glyphs is it inside a selection range?!
    //!!!may switch selection remdering on/off, to optimize rendering text that can't have selection!
    //!!!may simply check selection length, it is effectively the same!

    //!!!TODO TEXT: ensure that necessary adjustment happens before this, or enable alignToPixels here
    ImageRenderSettings settings(Rectf(), clipRect);//, 0xFFFFFFFF, 1.f, true);

    //???TODO TEXT: to what buffers really can merge?! need to see rendering order first!
    const auto canCombineFromIdx = out.size();

    //!!!!FIXME TEXT: need to know current line!!!
    constexpr float lineHeight = 0.f;
    constexpr float lineJustifySpacing = 0.f;

    //!!!TODO TEXT: may be useful to make pen modifyable, in-out arg!
    glm::vec2 penPosition = position;
    for (auto& glyph : p.glyphs)
    {
        //!!!skip all isWhitespace at the start of the word-wrapped line (not first line in a paragraph)

        auto verticalFmt = VerticalImageFormatting::BottomAligned;
        if (auto element = d_elements[glyph.elementIndex].get())
            verticalFmt = element->getVerticalFormatting();

        //!!!text inside the same line and element (non-embedded) is aligned exactly the same! can avoid recalc!

        Rectf dest(position.x, position.y, 0.f, 0.f);
        float y_scale = 1.0f;
        switch (verticalFmt)
        {
            case VerticalImageFormatting::BottomAligned:
                dest.d_min.y += lineHeight - glyph.height;
                break;

            case VerticalImageFormatting::CentreAligned:
                dest.d_min.y += (lineHeight - glyph.height) * 0.5f;
                break;

            case VerticalImageFormatting::Stretched:
                y_scale = (glyph.height > 0.f) ? (lineHeight / glyph.height) : 0.f;
                break;

                // TODO TEXT: Tiled?

                // TopAligned requires no operations
        }

        //!!!for embedded objects it is in advance and height, but w/out padding!
        //dest.setWidth((d_size.d_width > 0.f) ? d_size.d_width : d_image->getRenderedSize().d_width);
        //dest.setHeight(((d_size.d_height > 0.f) ? d_size.d_height : d_image->getRenderedSize().d_height) * y_scale);

        //???virtual Element::setupRenderer(RenderedGlyph&)? Will set colors etc inside based on the RenderedGlyph?
        //or even virtual Element::render(RenderedGlyph&)
        //or ranged - virtual Element::render(RenderedGlyph& from, RenderedGlyph& to), to minimize virtual calls!
        //???return rendered size from this function?!
        //!!!NB: element can treat union of pointers FontGlyph/Image/Window correctly without 'type' field in RenderedGlyph!

        if (glyph.image)
        {
            settings.d_destArea = Rectf(penPosition + glyph.offset, glyph.image->getRenderedSize());
            //!!!TODO TEXT:
            //settings.d_multiplyColours = (layer < layerColours.size()) ? layerColours[layer] : fallbackColour;

            glyph.image->createRenderGeometry(out, settings, canCombineFromIdx);
        }

        //!!!TODO TEXT:
        //render outline if required for the curent glyph! must be already baked, because outline size is dynamic!
        //use outline color from element or default one
        //render underline, strikeout
        //???TODO TEXT: how to store outline image? Store FontGlyph* instead of images? Or a separate Image* field?
        //Or new RenderedGlyph w/out advance or with special flag?

        penPosition.x += glyph.advance;

        if (glyph.isJustifyable)
            penPosition.x += lineJustifySpacing;
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
