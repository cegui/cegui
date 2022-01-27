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

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! A text element that represents an embedded widget
class CEGUIEXPORT RenderedTextWidget : public RenderedTextElement
{
public:

    virtual void setupGlyph(RenderedGlyph& glyph, uint32_t codePoint) const override;

    virtual void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const Window* refWnd, const glm::vec2& position, const ColourRect* modColours,
        const Rectf* clipRect) const override;

    virtual RenderedTextElementPtr clone() const override;

protected:

    //widget name, will be resolved when update glyph
    //???update glyph with setupGlyph? rename isEmbeddedObject to isDynamicObject?! will be able to easily
    //switch images to static objects if don't want to support runtime resizing
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
