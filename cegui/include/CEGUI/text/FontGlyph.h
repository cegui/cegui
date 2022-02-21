/***********************************************************************
    created:    Sun Jul 19 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIFontGlyph_h_
#define _CEGUIFontGlyph_h_

#include "CEGUI/Image.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    internal class representing a single font glyph.

    For TrueType fonts initially all FontGlyphs are empty
    (getImage() will return nullptr), but they are filled by demand.
*/
class CEGUIEXPORT FontGlyph
{
public:

    FontGlyph(char32_t codePoint, float advance = 0.f, Image* image = nullptr)
        : d_image(image)
        , d_advance(advance)
        , d_codePoint(codePoint)
    {
    }

    virtual ~FontGlyph() = default;

    /*!
    \brief
        Return the rendered advance value for this glyph.

        The rendered advance value is the total number of pixels from the
        current pen position that will be occupied by this glyph when rendered.
    */
    float getRenderedAdvance() const
    {
        return d_image ? d_image->getRenderedSize().d_width + d_image->getRenderedOffset().x : 0.f;
    }

    /*!
    \brief
        Return the horizontal advance value for the glyph.

        The returned value is the number of pixels the pen should move
        horizontally to position itself ready to render the next glyph.  This
        is not always the same as the glyph image width or rendered advance,
        since it allows for horizontal overhangs.
    */
    float getAdvance() const { return d_advance; }

    //! Set the horizontal advance value for the glyph.
    void setAdvance(float advance) { d_advance = advance; }

    //! Return the CEGUI::Image object rendered for this glyph.
    Image* getImage() const { return d_image; }

    //! Set the CEGUI::Image object rendered for this glyph.
    void setImage(Image* image) { d_image = image; }

    //! Returns the code point that this glyph is based on
    char32_t getCodePoint() const { return d_codePoint; }

private:

    //! The image which will be rendered for this glyph.
    Image* d_image;
    //! Amount to advance the pen after rendering this glyph
    float d_advance;
    //! Code point
    const char32_t d_codePoint;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
