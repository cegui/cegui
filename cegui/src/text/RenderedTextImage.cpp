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
#include "CEGUI/text/RenderedTextImage.h"
#include "CEGUI/text/RenderedTextParagraph.h"
#include "CEGUI/Image.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextImage::setupGlyph(RenderedGlyph& glyph, const Window* /*hostWindow*/) const
{
    if (d_image)
    {
        const float imgWidth = (d_size.d_width > 0.f) ? d_size.d_width : d_image->getRenderedSize().d_width;
        const float imgHeight = (d_size.d_height > 0.f) ? d_size.d_height : d_image->getRenderedSize().d_height;
        glyph.advance = imgWidth + getLeftPadding() + getRightPadding();
        glyph.height = imgHeight + getTopPadding() + getBottomPadding();
    }
    else
    {
        glyph.advance = 0.f;
        glyph.height = 0.f;
    }

    glyph.isEmbeddedObject = true;
    glyph.isJustifyable = false;
    glyph.isWhitespace = false;

    //!!!TODO TEXT: how must be padding applied to RTL objects? Should L/R padding be inverted or not?
    //if (glyph.isRightToLeft) ...
}

//----------------------------------------------------------------------------//
void RenderedTextImage::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const RenderedGlyph& glyph, const glm::vec2& pos, const ColourRect* modColours,
    const Rectf* clipRect, float heightScale, size_t canCombineFromIdx) const
{
    if (!d_image)
        return;

    ColourRect finalColours = d_colours;
    if (modColours)
        finalColours *= *modColours;

    const float imgWidth = (d_size.d_width > 0.f) ? d_size.d_width : d_image->getRenderedSize().d_width;
    const float imgHeight = (d_size.d_height > 0.f) ? d_size.d_height : d_image->getRenderedSize().d_height;
    ImageRenderSettings settings(Rectf(pos.x, pos.y, imgWidth, imgHeight * heightScale), clipRect, finalColours);
    d_image->createRenderGeometry(out, settings);
}

//----------------------------------------------------------------------------//
RenderedTextElementPtr RenderedTextImage::clone() const
{
    return std::make_unique<RenderedTextImage>(*this);
}

}
