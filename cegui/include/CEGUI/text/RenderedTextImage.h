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
#include "CEGUI/text/RenderedTextElement.h"
#include "CEGUI/ColourRect.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! A text element that represents an embedded image
class CEGUIEXPORT RenderedTextImage : public RenderedTextElement
{
public:

    RenderedTextImage(const Image* image) : d_image(image) {}

    virtual void setupGlyph(RenderedGlyph& glyph, uint32_t codePoint) const override;
    virtual Sizef updateMetrics(const Window* hostWindow) override;
    virtual float getGlyphWidth(const RenderedGlyph& glyph) const override;
    virtual float getHeight() const override { return d_effectiveSize.d_height; }

    virtual void createRenderGeometry(std::vector<GeometryBuffer*>& out, const RenderedGlyph* begin,
        size_t count, glm::vec2& penPosition, const ColourRect* modColours, const Rectf* clipRect,
        float lineHeight, float justifySpaceSize, size_t canCombineFromIdx, const SelectionInfo* selection) const override;

    virtual RenderedTextElementPtr clone() const override;

    void setColour(const ColourRect& colours) { d_colours = colours; }
    void setSize(const Sizef& size, bool aspectLock = false);
    void setUseModulateColour(bool value) { d_useModColour = value; }

protected:

    const Image* d_image = nullptr;
    ColourRect d_colours = ColourRect(0xFFFFFFFF);
    Sizef d_size; //<! target size to render the image at (0.f means natural size at the dimension)
    Sizef d_effectiveSize; //!< An effective size after the last updateMetrics(const Window* hostWindow) call
    bool d_useModColour = true;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
