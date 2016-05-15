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

#include "CEGUI/FontGlyph.h"
#include "FreeTypeFont.h"

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
    FreeTypeFontGlyph(FreeTypeFont* freeTypeFont, unsigned int glyphIndex,
        float advance = 0.0f, Image* image = nullptr, bool valid = false)
        : FontGlyph(advance, image, valid)
        , d_freeTypeFont(freeTypeFont)
        , d_glyphIndex(glyphIndex)
    {}

    ~FreeTypeFontGlyph()
    {}

    float getRenderedAdvance(
        const FontGlyph* nextGlyph,
        float x_scale) const override;

    /*!
    \brief 
        Returns the FreeType glyph index for this glyph.
    */
    unsigned int getGlyphIndex() const;

    /*!
    \brief 
        Sets the FreeType glyph index for this glyph.
    */
    void setGlyphIndex(const unsigned int value);
private:
    FreeTypeFont* d_freeTypeFont;

    unsigned int d_glyphIndex;
};

}

#endif
