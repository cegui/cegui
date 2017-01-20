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


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


namespace CEGUI
{

class FreeTypeFont;

/*!
\brief
    Internal class representing a single FreeType font glyph.

    For TrueType fonts initially all FontGlyphs are empty
    (getImage() will return a nullptr), but they are filled by demand.
*/
class CEGUIEXPORT FreeTypeFontGlyph : public FontGlyph
{
public:
    FreeTypeFontGlyph(char32_t codePoint, unsigned int glyphIndex,
        float advance = 0.0f, Image* image = nullptr, bool valid = false)
        : FontGlyph(codePoint, advance, image)
        , d_glyphIndex(glyphIndex)
        , d_initialised(valid)
    {}

    ~FreeTypeFontGlyph()
    {}

    float getRenderedAdvance(
    ) const override;

    //! mark the FontGlyph as initialised
    void markAsInitialised();

    //! return whether the glyph is valid
    bool isInitialised() const;

    void setLsbDelta(const long lsbDelta);
    long getLsbDelta() const;
    void setRsbDelta(const long rsbDelta);
    long getRsbDelta() const;

    unsigned int getGlyphIndex() const;


private:
    //! The difference between hinted and unhinted left side bearing while auto-hinting is active. Zero otherwise.
    long d_lsbDelta = 0;
    long d_rsbDelta = 0;

    //! The index of the glyph in FreeType
    const unsigned int d_glyphIndex;

    //! Says whether this glyph is initialised or not
    bool d_initialised = false;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
