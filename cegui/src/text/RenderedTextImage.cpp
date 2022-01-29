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
void RenderedTextImage::setupGlyph(RenderedGlyph& glyph, uint32_t codePoint) const
{
    glyph.offset = getPadding().getPosition();
    glyph.advance = d_effectiveSize.d_width;

    glyph.isJustifyable = false;
    glyph.isBreakable = true;
    glyph.isWhitespace = false;

    //!!!TODO TEXT: how must be padding applied to RTL characters? Should L/R padding be inverted or not?
    //if (glyph.isRightToLeft) ...
}

//----------------------------------------------------------------------------//
Sizef RenderedTextImage::updateMetrics(const Window* /*hostWindow*/)
{
    const Sizef oldSize = d_effectiveSize;
    if (d_image)
    {
        d_effectiveSize.d_width = ((d_size.d_width > 0.f) ? d_size.d_width : d_image->getRenderedSize().d_width)
            + getLeftPadding() + getRightPadding();
        d_effectiveSize.d_height = ((d_size.d_height > 0.f) ? d_size.d_height : d_image->getRenderedSize().d_height)
            + getTopPadding() + getBottomPadding();
    }
    else
    {
        d_effectiveSize.d_width = 0.f;
        d_effectiveSize.d_height = 0.f;
    }

    return d_effectiveSize - oldSize;
}

//----------------------------------------------------------------------------//
float RenderedTextImage::getGlyphWidth(const RenderedGlyph& glyph) const
{
    //???TODO TEXT: or return d_effectiveSize.d_width?!
    return glyph.advance;
}

//----------------------------------------------------------------------------//
void RenderedTextImage::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const RenderedGlyph* begin, size_t count, glm::vec2& penPosition, const ColourRect* modColours,
    const Rectf* clipRect, float lineHeight, float justifySpaceSize, size_t canCombineFromIdx) const
{
    if (!d_image)
        return;

    glm::vec2 pos = penPosition;
    float heightScale = 1.f;
    applyVerticalFormatting(lineHeight, pos, heightScale);

    const float imgWidth = (d_size.d_width > 0.f) ? d_size.d_width : d_image->getRenderedSize().d_width;
    const float imgHeight = (d_size.d_height > 0.f) ? d_size.d_height : d_image->getRenderedSize().d_height;
    const Sizef imgSize(imgWidth, imgHeight * heightScale);

    ImageRenderSettings settings(Rectf(), clipRect, d_colours);
    if (modColours)
        settings.d_multiplyColours *= *modColours;

    const RenderedGlyph* end = begin + count;
    for (auto glyph = begin; glyph != end; ++glyph)
    {
        settings.d_destArea.set(pos + glyph->offset, imgSize);
        d_image->createRenderGeometry(out, settings);

        pos.x += glyph->advance;
        if (glyph->isJustifyable)
            pos.x += justifySpaceSize;
    }

    penPosition.x = pos.x;
}

//----------------------------------------------------------------------------//
RenderedTextElementPtr RenderedTextImage::clone() const
{
    return std::make_unique<RenderedTextImage>(*this);
}

}
