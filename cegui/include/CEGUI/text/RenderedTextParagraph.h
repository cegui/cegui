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
#pragma once
#include "CEGUI/text/DefaultParagraphDirection.h"
#include "CEGUI/falagard/Enums.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/Rectf.h"
#include <vector>

// NB: these are internal classes, please use RenderedText in the client code

namespace CEGUI
{
using RenderedTextElementPtr = std::unique_ptr<class RenderedTextElement>;

struct CEGUIEXPORT SelectionInfo
{
    const Image* bgBrush = nullptr;
    ColourRect   bgColours = 0x800080FF;
    ColourRect   textColours = 0;         //!< Zero means keeping an existing text colour
    size_t       start = 0;
    size_t       end = 0;
};

struct RenderedGlyph
{
    const FontGlyph* fontGlyph; //!< Actual font glyph, may be nullptr for missing glyphs and for embedded objects

    glm::vec2 offset; //!< Offset from the current pen position where the content must be drawn
    float advance;    //!< Shift to be applied to the pen after drawing this glyph

    uint32_t sourceIndex;  //!< Starting index of the corresponding sequence in the logical text
    uint16_t elementIndex; //!< Index of controlling RenderedTextElement, stored instead of pointer to reduce struct size

    bool isJustifyable : 1;    //!< This glyph can be expanded in a justified text
    bool isBreakable : 1;      //!< This glyph can be transferred to the next line due to word wrapping
    bool isWhitespace : 1;     //!< This glyph is a whitespace that should not be rendered at the wrapped line start
    bool isRightToLeft : 1;    //!< Is this glyph directed from right to left? This affects caret etc.
};

class RenderedTextParagraph
{
public:

    static constexpr size_t npos = std::numeric_limits<size_t>().max();

    RenderedTextParagraph()
        : d_wordWrap(false)
        , d_skipWrappedWhitespace(true)
        , d_defaultWordWrap(true)
        , d_defaultHorzFormatting(true)
        , d_defaultLastJustifiedLineFormatting(true)
        , d_linesDirty(true)
        , d_fitsIntoAreaWidth(false)
    {}

    void setupGlyphs(const std::u32string& text,
        const std::vector<uint16_t>& elementIndices, const std::vector<RenderedTextElementPtr>& elements);
    void remapSourceIndices(const std::vector<size_t>& originalIndices, size_t sourceLength);

    void createRenderGeometry(std::vector<GeometryBuffer*>& out, glm::vec2& penPosition,
        const ColourRect* modColours, const Rectf* clipRect, const SelectionInfo* selection,
        const std::vector<RenderedTextElementPtr>& elements) const;

    //! Build paragraph lines with optional word wrapping, cache line widths
    void updateLines(const std::vector<RenderedTextElementPtr>& elements, float areaWidth);
    //! Update cached line heights
    void updateLineHeights(const std::vector<RenderedTextElementPtr>& elements, float defaultFontHeight);
    //! Update horizontal alignment of lines
    void updateHorizontalFormatting(float areaWidth);

    void accumulateExtents(Rectf& extents) const;
    float getHeight() const { return d_height; }

    void onElementWidthChanged(size_t elementIndex, float diff);
    void onElementHeightChanged(size_t elementIndex, float diff);
    void onAreaWidthChanged();

    void setHorizontalFormatting(HorizontalTextFormatting fmt, bool breakDefault);
    void setLastJustifiedLineFormatting(HorizontalTextFormatting fmt, bool breakDefault);
    void setWordWrapEnabled(bool wrap, bool breakDefault);

    bool isHorzFormattingDefault() const { return d_defaultHorzFormatting; }
    bool isLastJustifiedLineFormattingDefault() const { return d_defaultLastJustifiedLineFormatting; }
    bool isWordWrapDefault() const { return d_defaultWordWrap; }
    bool isFittingIntoAreaWidth() const { return d_fitsIntoAreaWidth; }
    //!!!TODO TEXT: float getRequiredWidthChangeToFit() or like that - for word-wrapped count from line start to break point, otherwise lineW-areaW

    void setBidiDirection(DefaultParagraphDirection dir) { d_bidiDir = dir; }
    DefaultParagraphDirection getBidiDirection() const { return d_bidiDir; }

    std::vector<RenderedGlyph>& glyphs() { return d_glyphs; }
    const std::vector<RenderedGlyph>& glyphs() const { return d_glyphs; }

    void setSourceIndexRange(uint32_t start, uint32_t end) { d_sourceStartIndex = start; d_sourceEndIndex = end; }
    uint32_t getSourceStartIndex() const { return d_sourceStartIndex; }
    uint32_t getSourceEndIndex() const { return d_sourceEndIndex; }

    size_t getLineCount() const { return d_linesDirty ? 1 : d_lines.size(); }
    size_t getTextIndexAtPoint(const glm::vec2& pt) const;
    bool getTextIndexBounds(Rectf& out, size_t textIndex, const std::vector<RenderedTextElementPtr>& elements) const;
    size_t getGlyphIndexAtPoint(const glm::vec2& pt) const;
    bool getGlyphBounds(Rectf& out, size_t glyphIndex, const std::vector<RenderedTextElementPtr>& elements) const;
    size_t getTextIndex(size_t glyphIndex) const;
    size_t getTextIndex(size_t lineIndex, float offsetX) const;
    size_t getNearestGlyphIndex(size_t textIndex) const;
    size_t getNearestGlyphIndex(size_t lineIndex, float offsetX) const;
    size_t getLineIndex(size_t textIndex) const;

    size_t nextTextIndex(size_t textIndex) const;
    size_t prevTextIndex(size_t textIndex) const;

protected:

    struct Line
    {
        Line() : heightDirty(true), horzFmtDirty(true) {}

        uint32_t glyphEndIdx = std::numeric_limits<uint32_t>().max();
        Sizef    extents;
        float    horzOffset = 0.f;
        float    justifySpaceSize = 0.f;
        uint16_t justifyableCount = 0;
        bool     heightDirty : 1;
        bool     horzFmtDirty : 1;
    };

    size_t getGlyphLineIndex(size_t glyphIndex) const;
    uint32_t skipWrappedWhitespace(uint32_t start, uint32_t end) const;

    std::vector<RenderedGlyph> d_glyphs;
    std::vector<Line> d_lines;

    //???TODO TEXT: store end idx instead?
    uint32_t d_sourceStartIndex = 0;  //!< Starting index of the paragraph in the logical text
    uint32_t d_sourceEndIndex = 0;  //!< Starting index of the paragraph in the logical text
    float d_height = 0.f;

    DefaultParagraphDirection d_bidiDir = DefaultParagraphDirection::Automatic;
    HorizontalTextFormatting d_horzFormatting = HorizontalTextFormatting::LeftAligned;
    HorizontalTextFormatting d_lastJustifiedLineFormatting = HorizontalTextFormatting::LeftAligned;
    bool d_wordWrap : 1;
    bool d_skipWrappedWhitespace : 1;

    bool d_defaultWordWrap : 1;
    bool d_defaultHorzFormatting : 1;
    bool d_defaultLastJustifiedLineFormatting : 1;

    bool d_linesDirty : 1;
    bool d_fitsIntoAreaWidth : 1;
};

}
