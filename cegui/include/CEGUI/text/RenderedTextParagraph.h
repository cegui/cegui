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
#ifndef _CEGUIRenderedTextParagraph_h_
#define _CEGUIRenderedTextParagraph_h_

#include "CEGUI/Sizef.h"
#include "CEGUI/DefaultParagraphDirection.h"
#include "CEGUI/falagard/Enums.h"
#include <glm/glm.hpp>
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{

struct CEGUIEXPORT RenderedGlyph
{
    const Image* image; //!< Actual graphics of this glyph, may be nullptr for missing font glyphs and some embedded objects

    glm::vec2 offset; //!< Offset from the current pen position where the content must be drawn
    float advance;    //!< Shift to be applied to the pen after drawing this glyph
    float height;     //!< The full height of this glyph, including padding, used for vertical alignment

    uint32_t sourceIndex;  //!< Starting index of the corresponding sequence in the logical text
    uint16_t elementIndex; //!< Index of controlling RenderedStringComponent, stored instead of pointer to reduce struct size

    bool isJustifyable : 1;    //!< This glyph can be expanded in a justified text
    bool isBreakable : 1;      //!< This glyph can be transferred to the next line due to word wrapping
    bool isWhitespace : 1;     //!< This glyph is a whitespace that should not be rendered at the wrapped line start
    bool isRightToLeft : 1;    //!< Is this glyph directed from right to left? This affects caret etc.
    bool isEmbeddedObject : 1; //!< Is this glyph a placeholder for an embedded object
};

class CEGUIEXPORT RenderedTextParagraph
{
public:

    struct Line
    {
        uint32_t glyphEndIdx = std::numeric_limits<uint32_t>().max();
        Sizef    extents;
        float    horzOffset = 0.f;
        float    justifySpaceSize = 0.f;
        uint16_t justifyableCount = 0;
    };

    RenderedTextParagraph()
        : wordWrap(false)
        , defaultWordWrap(true)
        , defaultHorzFormatting(true)
        , defaultLastJustifiedLineHorzFormatting(true)
        , linesDirty(true)
    {}

    void updateLineHeights(float defaultFontHeight);
    void updateHorizontalFormatting(float areaWidth);

    std::vector<RenderedGlyph> glyphs;
    std::vector<Line> lines;

    DefaultParagraphDirection bidiDir = DefaultParagraphDirection::Automatic;
    HorizontalTextFormatting horzFormatting = HorizontalTextFormatting::Justified;
    HorizontalTextFormatting lastJustifiedLineHorzFormatting = HorizontalTextFormatting::LeftAligned;
    bool wordWrap : 1;

    bool defaultWordWrap : 1;
    bool defaultHorzFormatting : 1;
    bool defaultLastJustifiedLineHorzFormatting : 1;

    //bool dynamicGlyphSizesDirty : 1;
    bool linesDirty : 1;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
