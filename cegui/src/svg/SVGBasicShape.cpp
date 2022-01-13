/***********************************************************************
    created:    30th July 2013
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/svg/SVGBasicShape.h"

#include "CEGUI/svg/SVGTesselator.h"


// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
SVGBasicShape::SVGBasicShape(const SVGPaintStyle& paint_style,
                             const glm::mat3x3& transformation)
    : d_paintStyle(paint_style)
    , d_transformation(transformation)
{
}

//----------------------------------------------------------------------------//
SVGBasicShape::~SVGBasicShape()
{
}

//----------------------------------------------------------------------------//
SVGRect::SVGRect(const SVGPaintStyle& paint_style,
                 const glm::mat3x3& transformation,
                 const float x,
                 const float y,
                 const float width,
                 const float height,
                 const float rx,
                 const float ry) :
    SVGBasicShape(paint_style, transformation),
    d_x(x),
    d_y(y),
    d_width(width),
    d_height(height),
    d_rx(rx),
    d_ry(ry)
{
}

//----------------------------------------------------------------------------//
void SVGRect::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselateRect(out, this, render_settings);
}

//----------------------------------------------------------------------------//
SVGCircle::SVGCircle(const SVGPaintStyle& paint_style,
                     const glm::mat3x3& transformation,
                     const float cx,
                     const float cy,
                     const float r) :
    SVGBasicShape(paint_style, transformation),
    d_cx(cx),
    d_cy(cy),
    d_r(r)
{
}

//----------------------------------------------------------------------------//
void SVGCircle::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselateCircle(out, this, render_settings);
}

//----------------------------------------------------------------------------//
SVGEllipse::SVGEllipse(const SVGPaintStyle& paint_style,
                       const glm::mat3x3& transformation,
                       const float cx,
                       const float cy,
                       const float rx,
                       const float ry)
    : SVGBasicShape(paint_style, transformation)
    , d_cx(cx)
    , d_cy(cy)
    , d_rx(rx)
    , d_ry(ry)
{
}

//----------------------------------------------------------------------------//
void SVGEllipse::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselateEllipse(out, this, render_settings);
}


//----------------------------------------------------------------------------//
SVGLine::SVGLine(const SVGPaintStyle& paint_style,
                 const glm::mat3x3& transformation,
                 const float x1,
                 const float y1,
                 const float x2,
                 const float y2) :
    SVGBasicShape(paint_style, transformation),
    d_x1(x1),
    d_y1(y1),
    d_x2(x2),
    d_y2(y2)
{
}

//----------------------------------------------------------------------------//
SVGLine::SVGLine(const SVGPaintStyle& paint_style,
                 const glm::mat3x3& transformation,
                 const glm::vec2& line_start,
                 const glm::vec2& line_end)
    : SVGBasicShape(paint_style, transformation)
    , d_x1(line_start.x)
    , d_y1(line_start.y)
    , d_x2(line_end.x)
    , d_y2(line_end.y)
{
}

//----------------------------------------------------------------------------//
void SVGLine::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselateLine(out, this, render_settings);
}

//----------------------------------------------------------------------------//
SVGPolyline::SVGPolyline(const SVGPaintStyle& paint_style,
                         const glm::mat3x3& transformation,
                         const std::vector<glm::vec2>& points) :
    SVGBasicShape(paint_style, transformation),
    d_points(points)
{
}

//----------------------------------------------------------------------------//
void SVGPolyline::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselatePolyline(out, this, render_settings);
}

//----------------------------------------------------------------------------//
SVGPolygon::SVGPolygon(const SVGPaintStyle& paint_style,
                       const glm::mat3x3& transformation,
                       const std::vector<glm::vec2>& points) :
    SVGBasicShape(paint_style, transformation),
    d_points(points)
{
}

//----------------------------------------------------------------------------//
void SVGPolygon::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselatePolygon(out, this, render_settings);
}

//----------------------------------------------------------------------------//
}

