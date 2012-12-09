/***********************************************************************
    filename:   CEGUIFontGlyph.h
    created:    Sun Jul 19 2009
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIFontGlyph_h_
#define _CEGUIFontGlyph_h_

#include "CEGUIImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    internal class representing a single font glyph.

    For TrueType fonts initially all FontGlyph's are empty
    (getImage() will return 0), but they are filled by demand.
*/
class CEGUIEXPORT FontGlyph
{
public:
    //! Constructor.
    FontGlyph(float advance = 0.0f, const Image* image = 0) :
        d_image(image),
        d_advance(advance)
    {}

    //! Return the CEGUI::Image object rendered for this glyph.
    const Image* getImage() const
    { return d_image; }

    //! Return the parent CEGUI::Imageset object for this glyph.
    const Imageset* getImageset() const
    { return d_image->getImageset(); }

    //! Return the scaled pixel size of the glyph.
    Size getSize(float x_scale, float y_scale) const
    { return Size(getWidth(x_scale), getHeight(y_scale)); }

    //! Return the scaled width of the glyph.
    float getWidth(float x_scale) const
    { return d_image->getWidth() * x_scale; }

    //! Return the scaled height of the glyph.
    float getHeight(float y_scale) const
    { return d_image->getHeight() * y_scale; }

    /*!
    \brief
        Return the rendered advance value for this glyph.

        The rendered advance value is the total number of pixels from the
        current pen position that will be occupied by this glyph when rendered.
    */
    float getRenderedAdvance(float x_scale) const
    { return (d_image->getWidth() + d_image->getOffsetX()) * x_scale; }

    /*!
    \brief
        Return the horizontal advance value for the glyph.

        The returned value is the number of pixels the pen should move
        horizontally to position itself ready to render the next glyph.  This
        is not always the same as the glyph image width or rendererd advance,
        since it allows for horizontal overhangs.
    */
    float getAdvance(float x_scale = 1.0) const
    { return d_advance * x_scale; }

    //! Set the horizontal advance value for the glyph.
    void setAdvance(float advance)
    { d_advance = advance; }

    //! Set the CEGUI::Image object rendered for this glyph.
    void setImage(const Image* image)
    { d_image = image; }

private:
    //! The image which will be rendered for this glyph.
    const Image* d_image;
    //! Amount to advance the pen after rendering this glyph
    float d_advance;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIFontGlyph_h_
