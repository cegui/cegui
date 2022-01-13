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
#ifndef _SVGBasicShape_h_
#define _SVGBasicShape_h_

#include "CEGUI/Base.h"

#include "CEGUI/svg/SVGPaintStyle.h"
#include "CEGUI/svg/SVGImage.h"

#include <glm/glm.hpp>

#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class GeometryBuffer;

/*!
\brief
    Defines a class that acts as interface for several classes that store the data of SVG BasicShapes.
*/
class CEGUIEXPORT SVGBasicShape
{
public:
    SVGBasicShape() {}
    SVGBasicShape(const SVGPaintStyle& paint_style,
                  const glm::mat3x3& transformation);

    virtual ~SVGBasicShape();

    /*!
    \brief
        Renders the shape into a new geometry buffer and adds it to the list.
    \param geometry_buffers
        The GeometryBuffer list to which the new geometry of this Image will be added to.
    \param render_settings
        The ImageRenderSettings that contain render settings for new GeometryBuffers.
     */
    virtual void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const SVGImage::SVGImageRenderSettings& render_settings) const = 0;
        

    //! The BasicShape's style, which describes the filling and stroke of the graphical element.
    SVGPaintStyle d_paintStyle;
    //! The matrix transformation to apply to the element.
    glm::mat3x3 d_transformation;
};


/*!
\brief
    Defines a class for storing the data of the SVG 'rect' element based on how it is defined in the SVG standard.

    The 'rect' element defines a rectangle which is axis-aligned with the current user coordinate system.
    Rounded rectangles can be achieved by setting appropriate values for attributes 'rx' and 'ry'.
    http://www.w3.org/TR/SVGTiny12/shapes.html#RectElement
*/
class CEGUIEXPORT SVGRect : public SVGBasicShape
{
public:
    SVGRect()
        : d_x(0.f)
        , d_y(0.f)
        , d_width(0.f)
        , d_height(0.f)
        , d_rx(0.f)
        , d_ry(0.f)
    {}

    SVGRect(const SVGPaintStyle& paint_style, const glm::mat3x3& transformation,
            const float x, const float y,
            const float width, const float height,
            const float rx = 0.0f, const float ry = 0.0f);

    //! Implementation of SVGBasicShape interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const SVGImage::SVGImageRenderSettings& render_settings) const override;

    //! The x-axis coordinate of the side of the rectangle which has the smaller x-axis coordinate value in the current user coordinate system
    float d_x;
    //! The y-axis coordinate of the side of the rectangle which has the smaller y-axis coordinate value in the current user coordinate system.
    float d_y;
    //! The width of the rectangle. A negative value is unsupported. A value of zero disables rendering of the element.
    float d_width;
    //! The height of the rectangle. A negative value is unsupported. A value of zero disables rendering of the element.
    float d_height;
    //! For rounded rectangles, the x-axis radius of the ellipse used to round off the corners of the rectangle. A negative value is unsupported.
    float d_rx;
    //! For rounded rectangles, the y-axis radius of the ellipse used to round off the corners of the rectangle. A negative value is unsupported.
    float d_ry;
};


/*!
\brief
    Defines a class for storing the data of the SVG 'circle' element based on how it is defined in the SVG standard.

    The 'circle' element defines a circle based on a center point and a radius.
    http://www.w3.org/TR/SVGTiny12/shapes.html#CircleElement
*/
class CEGUIEXPORT SVGCircle : public SVGBasicShape
{
public:
    SVGCircle()
        : d_cx(0.f)
        , d_cy(0.f)
        , d_r(0.f)
    {}

    SVGCircle(const SVGPaintStyle& paint_style,
              const glm::mat3x3& transformation,
              const float cx,
              const float cy,
              const float r);

    //! Implementation of SVGBasicShape interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const SVGImage::SVGImageRenderSettings& render_settings) const override;

    //! The x-axis coordinate of the center of the circle. 
    float d_cx;
    //! The y-axis coordinate of the center of the circle. 
    float d_cy;
    //! The radius of the circle. A negative value is unsupported. Default = 0.
    float d_r;
};


/*!
\brief
    Defines a class for storing the data of the SVG 'ellipse' element based on how it is defined in the SVG standard.

    The 'ellipse' element defines an ellipse which is axis-aligned with the current user coordinate system based on a center point and two radii.
    http://www.w3.org/TR/SVGTiny12/shapes.html#EllipseElement
*/
class CEGUIEXPORT SVGEllipse : public SVGBasicShape
{
public:
    SVGEllipse()
        : d_cx(0.f)
        , d_cy(0.f)
        , d_rx(0.f)
        , d_ry(0.f)
    {}

    SVGEllipse(const SVGPaintStyle& paint_style,
               const glm::mat3x3& transformation,
               const float cx,
               const float cy,
               const float rx,
               const float ry);

    //! Implementation of SVGBasicShape interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const SVGImage::SVGImageRenderSettings& render_settings) const override;

    //! The x-axis coordinate of the center of the ellipse. 
    float d_cx;
    //! The y-axis coordinate of the center of the ellipse. 
    float d_cy;
    //! The x-axis radius of the ellipse. A negative value is unsupported. Default = 0.
    float d_rx;
    //! The y-axis radius of the ellipse. A negative value is unsupported. Default = 0.
    float d_ry;
};


/*!
\brief
    Defines a class for storing the data of the SVG 'line' element based on how it is defined in the SVG standard.

    The 'line' element defines a line segment that starts at one point and ends at another.
    http://www.w3.org/TR/SVGTiny12/shapes.html#LineElement
*/
class CEGUIEXPORT SVGLine : public SVGBasicShape
{
public:
    //! Constructors
    SVGLine(const SVGPaintStyle& paint_style, const glm::mat3x3& transformation,
            const float x1, const float y1,
            const float x2, const float y2);

    SVGLine(const SVGPaintStyle& paint_style, const glm::mat3x3& transformation,
            const glm::vec2& line_start, const glm::vec2& line_end);

    SVGLine()
        : d_x1(0.f)
        , d_y1(0.f)
        , d_x2(0.f)
        , d_y2(0.f)
    {}

    //! Implementation of SVGBasicShape interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const SVGImage::SVGImageRenderSettings& render_settings) const override;

    //! The x-axis coordinate of the start of the line
    float d_x1;
    //! The y-axis coordinate of the start of the line
    float d_y1;
    //! The x-axis coordinate of the end of the line
    float d_x2;
    //! The y-axis coordinate of the end of the line
    float d_y2;
};

/*!
\brief
    Defines a class for storing the data of the SVG 'polyline' element based on how it is defined in the SVG standard.

    The 'polyline' element defines a set of connected straight line segments.
    Typically, 'polyline' elements define open shapes.
    http://www.w3.org/TR/SVGTiny12/shapes.html#PolylineElement
*/
class CEGUIEXPORT SVGPolyline : public SVGBasicShape
{
public:
    //! Constructor
    SVGPolyline(const SVGPaintStyle& paint_style,
                const glm::mat3x3& transformation,
                const std::vector<glm::vec2>& points);

    SVGPolyline()
    {}

    //! Implementation of SVGBasicShape interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const SVGImage::SVGImageRenderSettings& render_settings) const override;

    //! The points defining the line
    std::vector<glm::vec2> d_points;
};


/*!
\brief
    Defines a class for storing the data of the SVG 'polygon' element based on how it is defined in the SVG standard.

    The 'polygon' element defines a closed shape consisting of a set of connected straight line segments.
    http://www.w3.org/TR/SVGTiny12/shapes.html#PolygonElement
*/
class CEGUIEXPORT SVGPolygon : public SVGBasicShape
{
public:
    //! Constructor
    SVGPolygon(const SVGPaintStyle& paint_style,
              const glm::mat3x3& transformation,
              const std::vector<glm::vec2>& points);

    SVGPolygon()
    {}

    //! Implementation of SVGBasicShape interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const SVGImage::SVGImageRenderSettings& render_settings) const override;

    //! The points defining the line
    std::vector<glm::vec2> d_points;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif

