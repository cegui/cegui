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
#include "CEGUI/Exceptions.h"
#ifdef CEGUI_USE_RAQM
#include "CEGUI/FreeTypeFont.h"
#include <raqm.h>
#endif
#include <algorithm>

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
static const Font* getElementFont(RenderedStringComponent* element, const Font* currFont, const Font* defaultFont, bool& isFontSource)
{
    // Non-text elements don't break the range of the current font
    auto textStyle = dynamic_cast<const RenderedStringTextComponent*>(element);
    const Font* font = textStyle ? textStyle->getFont() : currFont;
    isFontSource = !!textStyle;

    if (!font)
    {
        if (!defaultFont)
            return false;

        font = defaultFont;
    }

    return font;
}

//----------------------------------------------------------------------------//
static bool layoutParagraph(RenderedParagraph& out, const std::u32string& text,
    size_t start, size_t end, const Font* defaultFont, DefaultParagraphDirection dir,
    const std::vector<uint16_t>& elementIndices,
    const std::vector<RenderedStringComponentPtr>& elements)
{
    //!!!if RAQM defined, may get here as a fallback, use fribidi then!

    // Apply Unicode Bidirectional Algorithm to obtain a string with visual ordering of codepoints
#if defined(CEGUI_BIDI_SUPPORT)
    //!!!FIXME TEXT: IMPROVE BIDI
    // 1. Can avoid virtualization and transfer internals here or into some wrapper
    // 2. UTF-32 text is ready to be processed, need to pass it without redundant conversions
    // 3. Since text can be passed here mutable, can use inplace transform where supported! (minibidi?)
    // 4. Remove state from wrapper
    // 5. Can make universal BIDI function for UTF-8 that converts to UTF-32 inside, not repeat in every impl
    // 6. Can make inplace API variant for UTF-32? Impl that doesn't support it will make a copy internally.
    // 7. Use default paragraph direction where supported: defaultParagraphDir
    // 8. One wrapper function and #ifdef inside?
    // 9. Make preprocessor definitions better for RAQM Fribidi fallback?
    std::vector<int> l2v;
    std::vector<int> v2l;
    std::u32string textVisual;
#if defined (CEGUI_USE_FRIBIDI)
    FribidiVisualMapping().reorderFromLogicalToVisual(text, textVisual, l2v, v2l);
#elif defined (CEGUI_USE_MINIBIDI)
    MinibidiVisualMapping().reorderFromLogicalToVisual(text, textVisual, l2v, v2l);
#else
#error "BIDI configuration is inconsistent, check your config!"
#endif

    // Post-BIDI reindexing
    //!!!originalIndices, elementIndices - permute based on l2v
    //???use std::sort? or just use l2v as an additional indirection level?
    //!!!can do it via macro! #define IDX(x) (l2v[x]) VS #define IDX(x) (x), don't forget to undef it!

#else
    const auto& textVisual = text;
#endif

    // Glyph generation

    // for each codepoint
    //   if references image or widget, process specially
    //   if references a font
    //     collect a range with the same font
    //     process range inside that font, to handle kerning and other font virtualization
    //     apply styling (underline, strikeout etc)

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
        size_t fontSourceElementIdx = std::numeric_limits<size_t>().max(); // NB: intentionally invalid, elementIndices are uint16_t
        const FreeTypeFont* currFont = nullptr;
        size_t fontLen = 0;
        for (size_t i = start; i < end; ++i)
        {
            // Get a font for the current character
            const Font* charFont;
            if (i >= elementIdxCount)
            {
                // Characters without an associated element use the default font
                charFont = defaultFont;
            }
            else
            {
                const auto charElementIdx = elementIndices[i];
                if (fontSourceElementIdx == charElementIdx)
                {
                    // Font couldn't change if the source element didn't
                    charFont = currFont;
                }
                else
                {
                    // Non-text elements have no font and are skipped for index comparison optimization
                    bool isFontSource;
                    charFont = getElementFont(elements[charElementIdx].get(), currFont, defaultFont, isFontSource);
                    if (isFontSource)
                        fontSourceElementIdx = charElementIdx;
                }
            }

            // NB: this check is necessary for failing if the first character has no font
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

        //!!!FIXME TEXT: request per-codepoint load flags in RAQM? May be needed for mixing [anti]aliased fonts.
        if (fontStart == 0)
        {
            //???!!!store flags inside a FreeTypeFont?! must not know these details here!
            const FT_Int32 targetType = range.first->isAntiAliased() ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO;
            const FT_Int32 loadFlags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | targetType;
            if (!raqm_set_freetype_load_flags(rq, loadFlags))
                return false;
        }

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
        auto fontGlyph = font->getGlyphByIndex(rqGlyph.index, true);
        if (!fontGlyph)
            fontGlyph = font->getGlyphForCodepoint(Font::UnicodeReplacementCharacter);

        // A multiplication coefficient to convert 26.6 fixed point values into normal floats
        constexpr float s_26dot6_toFloat = (1.0f / 64.f);

        renderedGlyph.image = fontGlyph ? fontGlyph->getImage(0) : nullptr;
        renderedGlyph.sourceIndex = rqGlyph.cluster + start;
        renderedGlyph.offset.x = rqGlyph.x_offset * s_26dot6_toFloat;
        renderedGlyph.offset.y = rqGlyph.y_offset * s_26dot6_toFloat + font->getBaseline();
        renderedGlyph.advance = ((rqGlyphDir == RAQM_DIRECTION_TTB) ? rqGlyph.y_advance : rqGlyph.x_advance)  * s_26dot6_toFloat;
        renderedGlyph.elementIndex = (renderedGlyph.sourceIndex < elementIdxCount) ?
            elementIndices[renderedGlyph.sourceIndex] :
            (static_cast<uint16_t>(elements.size() - 1));
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

    if (text.empty())
        return true;

    //!!!TODO TEXT: in each elements store logical index of the codepoint (or first-last range for texts)

    // Parse a string and obtain UTF-32 text with placeholders but without tags
    std::u32string utf32Text;
    std::vector<size_t> originalIndices;
    std::vector<uint16_t> elementIndices;
    std::vector<RenderedStringComponentPtr> elements;
    if (parser)
    {
        //!!!???base style will be created empty, fill it outside?, font, color
        //parser->parse(text);

        //!!!in parser have to push new style at every change:
        //[color=x][font=y]text[/font]text2[/color]
        // Multivalue tags are better?
        //[font type=y size=z]text[/font]
        // Also may remove unused styles from output at the end of parsing! Can count uses inside style!
        //???if style can't be created during parsing, reuse previous style on the stack?

        //!!!TODO: bold, italic, underline, strikeout, outline (w/params, incl color), color
        //!!!font must have a set of glyphs for each codepoint? e.g. regular, bold, stroke outline
        //NB: if there is a separate bold font, may need no embolden glyphs in regular font
        //???inside a font, store its regular, bold and italic variants? No italic -> oblique, no bold -> embolden.
        //???need bold-italic? maybe need too!
    }
    else
    {
        //!!!this can be used instead of DefaultRenderedStringParser!

        // When no parser specified, simply ensure that we have out UTF-32 string
#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_32)
        utf32Text = String::convertUtf8ToUtf32(text.c_str(), &originalIndices);
#else
        utf32Text = text; //???can avoid copying? e.g. passing utf32Text further as an arg? or mutability is useful later?
#endif
    }

    // No text or placeholders left after parsing, there is nothing to render
    if (utf32Text.empty())
        return true;

    // There are characters without associated text style. Add default one.
    if (elementIndices.size() < utf32Text.size())
    {
        // Characters without explicit styles require a default font
        if (!defaultFont)
            return false;

        elements.emplace_back(new RenderedStringTextComponent("", defaultFont));
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
        d_paragraphs.push_back({});

        if (end > start)
        {
            // Create a sequence of CEGUI glyphs for this paragraph
#ifdef CEGUI_USE_RAQM
            if (!layoutParagraphWithRaqm(d_paragraphs.back(), utf32Text, start, end, defaultFont, defaultParagraphDir,
                    elementIndices, elements, rq))
#endif
                layoutParagraph(d_paragraphs.back(), utf32Text, start, end, defaultFont, defaultParagraphDir,
                    elementIndices, elements);
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

    const bool adjustSourceIndices = !originalIndices.empty();
    for (auto& p : d_paragraphs)
    {
        for (auto& glyph : p.glyphs)
        {
            // Make source index point to an original string passed in "text" arg
            if (adjustSourceIndices)
                glyph.sourceIndex = static_cast<uint32_t>(originalIndices[glyph.sourceIndex]);

            const auto& element = elements[glyph.elementIndex];
            // always:
            //element->getTopPadding
            //element->getBottomPadding
            // if first:
            //element->getLeftPadding
            // if last:
            //element->getRightPadding
            //!!!handle RTL direction for horizontal padding!
            //!!!note that offset is not counted in advance, so padding must be added to both sometimes!

            // VerticalImageFormatting - store in glyph or get from element? or embed into offset and advance Y?!
            // if embed, image size changes will require recalculation!?
            //!!!NB: this changes per element, no need to calculate for each glyph when creating geometry!

            //!!!Replace placeholder glyphs in paragraphs with images and widgets!

            //!!!only for non-placeholders, real texts!
            //???!!!placeholders must not be justifyable but may be breakable!?
            const auto codePoint = text[glyph.sourceIndex];
            glyph.isJustifyable = (codePoint == ' ');
            glyph.isBreakable = (codePoint == ' ');
        }

        //!!!Calculate paragraph extents for faster horizontal formatting!
    }

    return true;
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
    float partial_extent = 0.f;
    const size_t last_component = d_lines[0].second;
    for (; idx < last_component; ++idx)
    {
        partial_extent += d_components[idx]->getPixelSize(refWnd).d_width;
        if (splitPoint <= partial_extent)
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
            const float localPt = splitPoint - (partial_extent - c->getPixelSize(refWnd).d_width);
            if (auto lc = c->split(refWnd, localPt, idx == 0, wasWordSplit))
            {
                left.d_components.push_back(std::move(lc));
                ++leftLine.second;
            }
        }
        // Can't split, if component width is >= splitPoint xfer the whole
        // component to it's own line in the left part (FIX #306)
        else if (c->getPixelSize(refWnd).d_width >= splitPoint)
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
    RenderedString copy;
    for (const auto& component : d_components)
        copy.d_components.push_back(component->clone());
    copy.d_lines = d_lines;
    return copy;
}

}
