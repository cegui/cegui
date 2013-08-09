/***********************************************************************
    filename:   SVGBasicShape.h
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
#include "CEGUI/Vector.h"

#include "CEGUI/svg/SVGShapeStyle.h"
#include "CEGUI/svg/SVGImage.h"

#include "glm/glm.hpp"

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
class CEGUIEXPORT SVGBasicShape : public AllocatedObject<SVGBasicShape>
{
public:
    SVGBasicShape();

    virtual ~SVGBasicShape();

    /*!
    \brief
        Renders the shape into a new geometry buffer and adds it to the list.
    \param geometry_buffers
        The GeometryBuffer list to which the new geometry of this Image will be added to.
    \param render_settings
        The ImageRenderSettings that contain render settings for new GeometryBuffers.
     */
    virtual void render(std::vector<GeometryBuffer*>& geometry_buffers,
                const SVGImage::SVGImageRenderSettings& render_settings) const = 0;
        


protected:

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
    SVGRect(const float x, const float y,
            const float width, const float height,
            const float rx = 0.0f, const float ry = 0.0f);

    //! Implementation of SVGBasicShape interface
    void render(std::vector<GeometryBuffer*>& geometry_buffers,
                const SVGImage::SVGImageRenderSettings& render_settings) const;

    //! The BasicShape's style, which describes the filling and stroke of the graphical element.
    SVGShapeStyle d_shapeStyle;

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
    Defines a class for storing the data of the SVG 'polyline' element based on how it is defined in the SVG standard.

    The 'polyline' element defines a set of connected straight line segments.
    Typically, 'polyline' elements define open shapes.
    http://www.w3.org/TR/SVGTiny12/shapes.html#PolylineElement
*/
class CEGUIEXPORT SVGPolyline : public SVGBasicShape
{
public:
/*
    SVGPolyline();

    ~SVGPolyline();*/

    //! Implementation of SVGBasicShape interface
    void render(std::vector<GeometryBuffer*>& geometry_buffers,
                const SVGImage::SVGImageRenderSettings& render_settings) const;

    //! The BasicShape's style, which describes the filling and stroke of the graphical element.
    SVGShapeStyle d_shapeStyle;

    typedef std::vector<glm::vec2> PolylinePointsList;
    PolylinePointsList d_points;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif

