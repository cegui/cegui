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

#include "CEGUI/FreeTypeFontGlyph.h"

//#include <freetype/freetype.h>

namespace CEGUI
{

float FreeTypeFontGlyph::getRenderedAdvance(
    const FontGlyph* nextGlyph,
    float x_scale
    ) const
{
    bool isFollowedByAnotherCharacter = (nextGlyph != nullptr);
    const FreeTypeFontGlyph* nextGlyphFT = dynamic_cast<const FreeTypeFontGlyph*>(nextGlyph);

    if(isFollowedByAnotherCharacter && nextGlyphFT == nullptr)
    {
        InvalidRequestException("FreeTypeFontGlyph::getRenderedAdvance - Attempted to "
        "cast following Font Glyph to a FreeTypeFontGlyph has failed. This should not "
        "occur because FreeTypeFontGlyphs shall be followed by FreeTypeFontGlyphs only.");
    }

    float sizeX = getImage()->getRenderedSize().d_width + getImage()->getRenderedOffset().x;
    sizeX *= x_scale;

    // Last character, no kerning is done
    if(!isFollowedByAnotherCharacter)
    {
       return sizeX;
    }

    // Determine kerning
    FT_Vector kerning;

    const FT_Face& face = d_freeTypeFont->getFontFace();
    FT_Kerning_Mode kerningMode = d_freeTypeFont->getKerningMode();
    
    if(face == nullptr)
    {
        throw InvalidRequestException(
            "FreeTypeFontGlyph::getRenderedAdvance - Attempted to access Font "
            "Face of a FreeType font, but it is not set to a valid Face.");
    }

    const unsigned int& leftGlyphIndex = d_glyphIndex;
    const unsigned int rightGlyphIndex = nextGlyphFT->getGlyphIndex();


    FT_Error error = FT_Get_Kerning(
        face,
        leftGlyphIndex,
        rightGlyphIndex,
        kerningMode,
        &kerning);

    if(error != 0)
    {
        throw InvalidRequestException(
            "FreeTypeFontGlyph::getRenderedAdvance - Kerning returned with "
            "error code " + error);
    }

    sizeX += kerning.x * x_scale;



    return sizeX;
}

unsigned int FreeTypeFontGlyph::getGlyphIndex() const
{
    return d_glyphIndex;
}

void FreeTypeFontGlyph::setGlyphIndex(unsigned int val)
{
    d_glyphIndex = val;
}

}
