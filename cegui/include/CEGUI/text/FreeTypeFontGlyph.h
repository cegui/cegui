/***********************************************************************
    created:    7th May 2016
    author:     Lukas Meindl <bitbucket@lukasmeindl.at>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIFreeTypeFontGlyph_h_
#define _CEGUIFreeTypeFontGlyph_h_

#include "CEGUI/text/FontGlyph.h"

namespace CEGUI
{

/*!
\brief
    Internal class representing a single FreeType font glyph.

    For TrueType fonts initially all FontGlyphs are empty
    (getImage() will return a nullptr), but they are filled by demand.
*/
class CEGUIEXPORT FreeTypeFontGlyph : public FontGlyph
{
public:

    FreeTypeFontGlyph(char32_t codePoint, uint32_t glyphIndex, float advance = 0.f, Image* image = nullptr)
        : FontGlyph(codePoint, advance, image)
        , d_glyphIndex(glyphIndex)
    {}

    uint32_t getGlyphIndex() const { return d_glyphIndex; }

    void setLsbDelta(int32_t lsbDelta) { d_lsbDelta = lsbDelta; }
    int32_t getLsbDelta() const { return d_lsbDelta; }
    void setRsbDelta(int32_t rsbDelta) { d_rsbDelta = rsbDelta; }
    int32_t getRsbDelta() const { return d_rsbDelta; }

private:

    //! The index of the glyph in FreeType
    const uint32_t d_glyphIndex;

    //! The difference between hinted and unhinted side bearing while auto-hinting is active. Zero otherwise.
    int32_t d_lsbDelta = 0;
    int32_t d_rsbDelta = 0;
};

}

#endif
