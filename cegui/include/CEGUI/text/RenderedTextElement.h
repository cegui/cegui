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
#include "CEGUI/Rectf.h"
#include "CEGUI/falagard/Enums.h"
#include "CEGUI/ColourRect.h"
#include <vector>
#include <memory>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
struct SelectionInfo;
struct RenderedGlyph;
using RenderedTextElementPtr = std::unique_ptr<class RenderedTextElement>;

/*!
\brief
    An absract structural part of a rendered text, either styled glyphs,
    embedded objects or any other controller for the rendered text content.
*/
class CEGUIEXPORT RenderedTextElement
{
public:

    virtual ~RenderedTextElement() = default;

    virtual void setupGlyph(RenderedGlyph& glyph, uint32_t codePoint) const = 0;
    virtual Sizef updateMetrics(const Window* hostWindow) = 0;
    virtual float getGlyphWidth(const RenderedGlyph& glyph) const = 0;
    virtual float getHeight() const = 0;

    virtual void createRenderGeometry(std::vector<GeometryBuffer*>& out, const RenderedGlyph* begin,
        size_t count, glm::vec2& penPosition, const ColourRect* modColours, const Rectf* clipRect,
        float lineHeight, float justifySpaceSize, size_t canCombineFromIdx, const SelectionInfo* selection) const = 0;

    virtual RenderedTextElementPtr clone() const = 0;

    void setPadding(const Rectf& padding) { d_padding = padding; }
    void setLeftPadding(const float padding) { d_padding.d_min.x = padding; }
    void setRightPadding(const float padding) { d_padding.d_max.x = padding; }
    void setTopPadding(const float padding) { d_padding.d_min.y = padding; }
    void setBottomPadding(const float padding) { d_padding.d_max.y = padding; }
    const Rectf& getPadding() const { return d_padding; }
    float getLeftPadding() const { return d_padding.d_min.x; }
    float getRightPadding() const { return d_padding.d_max.x; }
    float getTopPadding() const { return d_padding.d_min.y; }
    float getBottomPadding() const { return d_padding.d_max.y; }

    void setVerticalFormatting(VerticalImageFormatting fmt) { d_verticalFormatting = fmt; }
    VerticalImageFormatting getVerticalFormatting() const { return d_verticalFormatting; }

    void applyVerticalFormatting(float lineHeight, float& posY, float& heightScale) const;

    void setFont(Font* font) { d_font = font; }
    Font* getFont() const { return d_font; }

    void setBackgroundColour(const ColourRect& colours) { d_backgroundColours = colours; }
    ColourRect getBackgroundColour() const { return d_backgroundColours; }
    bool hasBackgroundColour() const { return !d_backgroundColours.isFullyTransparent(); }

protected:

    Font*                   d_font = nullptr;
    ColourRect              d_backgroundColours = ColourRect(0x00000000);
    Rectf                   d_padding = Rectf(0.f, 0.f, 0.f, 0.f);
    VerticalImageFormatting d_verticalFormatting = VerticalImageFormatting::BottomAligned;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
