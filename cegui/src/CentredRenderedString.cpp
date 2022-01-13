/***********************************************************************
    created:    25/05/2009
    author:     Paul Turner
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
#include "CEGUI/CentredRenderedString.h"
#include "CEGUI/RenderedString.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
CentredRenderedString::CentredRenderedString(const RenderedString& string) :
    FormattedRenderedString(string)
{
}

//----------------------------------------------------------------------------//
void CentredRenderedString::format(const Window* refWnd, const Sizef& area_size)
{
    d_offsets.clear();

    d_extent.d_width = 0.f;
    d_extent.d_height = 0.f;

    for (size_t i = 0; i < d_renderedString->getLineCount(); ++i)
    {
        const Sizef lineExtent = d_renderedString->getLineExtent(refWnd, i);

        d_offsets.push_back((area_size.d_width - lineExtent.d_width) / 2.0f);

        // NB: reuse lineExtent here instead of calling RenderedString::getExtent
        d_extent.d_height += lineExtent.d_height;
        if (d_extent.d_width < lineExtent.d_width)
            d_extent.d_width = lineExtent.d_width;
    }
}

//----------------------------------------------------------------------------//
std::vector<GeometryBuffer*> CentredRenderedString::createRenderGeometry(
    const Window* refWnd, const glm::vec2& position, const ColourRect* mod_colours, const Rectf* clip_rect) const
{
    std::vector<GeometryBuffer*> geomBuffers;

    glm::vec2 draw_pos = position;
    for (size_t i = 0; i < d_renderedString->getLineCount(); ++i)
    {
        draw_pos.x = position.x + d_offsets[i];

        auto geom = d_renderedString->createRenderGeometry(refWnd, i, draw_pos, mod_colours, clip_rect, 0.0f);
        geomBuffers.insert(geomBuffers.end(), geom.begin(), geom.end());

        draw_pos.y += d_renderedString->getLineExtent(refWnd, i).d_height;
    }

    return geomBuffers;
}
    
}
