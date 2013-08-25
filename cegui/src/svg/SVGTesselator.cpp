/***********************************************************************
    filename:   SVGTesselator.cpp
    created:    1st August 2013
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
#include "CEGUI/svg/SVGTesselator.h"

#include "CEGUI/svg/SVGBasicShape.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/System.h"
#include "CEGUI/Vertex.h"

#include "glm/gtx/constants.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
SVGTesselator::StrokeSegmentData::StrokeSegmentData(GeometryBuffer& geometry_buffer,
                                                    const float stroke_half_width,
                                                    const SVGPaintStyle& paint_style) :
    d_geometryBuffer(geometry_buffer),
    d_strokeHalfWidth(stroke_half_width),
    d_paintStyle(paint_style),
    d_prevPoint(0),
    d_curPoint(0),
    d_nextPoint(0)
{

}

//----------------------------------------------------------------------------//
void SVGTesselator::StrokeSegmentData::setPoints(const glm::vec2& prev_point,
                                                 const glm::vec2& cur_point,
                                                 const glm::vec2& next_point)
{
    d_prevPoint = &prev_point;
    d_curPoint = &cur_point;
    d_nextPoint = &next_point;
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderPolyline(const SVGPolyline* polyline,
                                               std::vector<GeometryBuffer*>& geometry_buffers,
                                               const SVGImage::SVGImageRenderSettings& render_settings)
{
    GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings, polyline->d_transformation);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = polyline->d_paintStyle;

    //Get colours
    const Colour fill_colour = getFillColour(paint_style);
    const Colour stroke_colour = getStrokeColour(paint_style);

    //Get the half of the stroke width
    const float stroke_half_width = paint_style.d_strokeWidth * 0.5f;

    //Getting the points defining the polyline
    const SVGPolyline::PolylinePointsList& points = polyline->d_points;

    //Create the polyline stroke vertex
    ColouredVertex line_stroke_vertex(glm::vec3(), stroke_colour);

    //Create and append the polyline's stroke geometry
    createStroke(points, paint_style, geometry_buffer, false);
}


//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderRect(const SVGRect* rect,
                                           std::vector<GeometryBuffer*>& geometry_buffers,
                                           const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::mat3x3 transformation = glm::mat3(1.0f, 0.0f, rect->d_x,
                                           0.0f, 1.0f, rect->d_y,
                                           0.0f, 0.0f, 1.0f ) * rect->d_transformation;
    GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings, transformation);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = rect->d_paintStyle;

    //Get colours
    const Colour fill_colour = getFillColour(paint_style);

    //Create the rectangle fill vertex
    ColouredVertex rectFillVertex(glm::vec3(), fill_colour);
    //Apply z-depth
    rectFillVertex.d_position.z = 0.0f;

    //Make a list of rectangle (corner) points
    std::vector<glm::vec2> rectangle_points;
    rectangle_points.push_back( glm::vec2(0.0f,          0.0f          ) );
    rectangle_points.push_back( glm::vec2(0.0f,          rect->d_height) );
    rectangle_points.push_back( glm::vec2(rect->d_width, rect->d_height) );
    rectangle_points.push_back( glm::vec2(rect->d_width, 0.0f          ) );
  
    //Add the rectangle fill vertices
    rectFillVertex.d_position.swizzle(glm::X, glm::Y) = rectangle_points[0];
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.swizzle(glm::X, glm::Y) = rectangle_points[1];
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.swizzle(glm::X, glm::Y) = rectangle_points[2];
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.swizzle(glm::X, glm::Y) = rectangle_points[2];
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.swizzle(glm::X, glm::Y) = rectangle_points[0];
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.swizzle(glm::X, glm::Y) = rectangle_points[3];
    geometry_buffer.appendVertex(rectFillVertex);

    //Create and append the rectangle's stroke geometry
    createStroke(rectangle_points, paint_style, geometry_buffer, true);
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderCircle(const SVGCircle* circle,
                                             std::vector<GeometryBuffer*>& geometry_buffers,
                                             const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::mat3x3 transformation = glm::mat3(1.0f, 0.0f, circle->d_cx,
                                           0.0f, 1.0f, circle->d_cy,
                                           0.0f, 0.0f, 1.0f ) * circle->d_transformation;
    GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings, transformation);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = circle->d_paintStyle;

    //Get colours
    const Colour fill_colour = getFillColour(paint_style);
    const Colour stroke_colour = getStrokeColour(paint_style);

    //We need this to determine the resolution in which we will render the circle segments
    float max_scale = std::max(render_settings.d_scaleFactor.d_x, render_settings.d_scaleFactor.d_y);

    //Create the rectangle fill vertex
    ColouredVertex circle_fill_vertex(glm::vec3(), fill_colour);

    const float& r = circle->d_r;

    // The numeric value in this can be set freely. The lower, the more segments there will be, making the circle appear rounder.
    static const float circle_roundness_value = 1.5f;
    const float segment_length = circle_roundness_value / max_scale;
    float theta = std::acos( 1 - ( segment_length / r ) );
    float num_segments = (2.0f * glm::pi<float>()) / theta;

    //precalculate the sine and cosine
	float c = std::cosf(theta);
	float s = std::sinf(theta);
	float t;

    // we start at angle = 0 
    glm::vec2 current_pos(r, 0);
    
	for(int i = 0; i < num_segments; i++) 
	{ 
        circle_fill_vertex.d_position = glm::vec3(circle->d_r, 0.0f, 0.0f);
        geometry_buffer.appendVertex(circle_fill_vertex);

        circle_fill_vertex.d_position = glm::vec3(current_pos, 0.0f);
        geometry_buffer.appendVertex(circle_fill_vertex);

		//apply the rotation matrix
		t = current_pos.x;
		current_pos.x = c * current_pos.x - s * current_pos.y;
		current_pos.y = s * t + c * current_pos.y;

        circle_fill_vertex.d_position = glm::vec3(current_pos, 0.0f);
        geometry_buffer.appendVertex(circle_fill_vertex);
	} 
}

//----------------------------------------------------------------------------//
GeometryBuffer& SVGTesselator::setupGeometryBufferColoured(std::vector<GeometryBuffer*>& geometry_buffers,
                                                           const SVGImage::SVGImageRenderSettings& render_settings,
                                                           const glm::mat3x3& svg_transformation)
{
    GeometryBuffer& geometry_buffer = System::getSingleton().getRenderer()->createGeometryBufferColoured();
    geometry_buffers.push_back(&geometry_buffer);

    if(render_settings.d_clipArea)
    {
        geometry_buffer.setClippingActive(true);
        geometry_buffer.setClippingRegion(*render_settings.d_clipArea);
    }
    else
        geometry_buffer.setClippingActive(false);

    geometry_buffer.setScale(render_settings.d_scaleFactor);

    glm::mat4 cegui_transform = createRenderableMatrixFromSVGMatrix(svg_transformation);
    geometry_buffer.setCustomTransform(cegui_transform);

    return geometry_buffer;
}

//----------------------------------------------------------------------------//
Colour SVGTesselator::getFillColour(const SVGPaintStyle &paint_style)
{
    const glm::vec3& fill_colour_values = paint_style.d_fill.d_colour;
    return Colour(fill_colour_values.x, fill_colour_values.y, fill_colour_values.z, paint_style.d_fillOpacity);
}

//----------------------------------------------------------------------------//
Colour SVGTesselator::getStrokeColour(const SVGPaintStyle &paint_style)
{
    const glm::vec3& stroke_colour_values = paint_style.d_stroke.d_colour;
    return Colour(stroke_colour_values.x, stroke_colour_values.y, stroke_colour_values.z, paint_style.d_strokeOpacity);
}

//----------------------------------------------------------------------------//
glm::mat4 SVGTesselator::createRenderableMatrixFromSVGMatrix(glm::mat3 svg_matrix)
{
    return glm::mat4(svg_matrix[0].x, svg_matrix[1].x, 0.0f, 0.0f,
                     svg_matrix[0].y, svg_matrix[1].y, 0.0f, 0.0f,
                     0.0f,            0.0f,            1.0f, 0.0f,
                     svg_matrix[0].z, svg_matrix[1].z, 0.0f, 1.0f);
}

enum LineIntersectResult { PARALLEL, COINCIDENT, NOT_INTERSECTING, INTERESECTING };

//----------------------------------------------------------------------------//
LineIntersectResult intersect(const glm::vec2& line1_start, const glm::vec2& line1_end,
                              const glm::vec2& line2_start, const glm::vec2& line2_end,
                              glm::vec2& intersection)
{
    float denom = ((line2_end.y - line2_start.y)*(line1_end.x - line1_start.x)) -
                  ((line2_end.x - line2_start.x)*(line1_end.y - line1_start.y));

    float nume_a = ((line2_end.x - line2_start.x)*(line1_start.y - line2_start.y)) -
                   ((line2_end.y - line2_start.y)*(line1_start.x - line2_start.x));

    float nume_b = ((line1_end.x - line1_start.x)*(line1_start.y - line2_start.y)) -
                   ((line1_end.y - line1_start.y)*(line1_start.x - line2_start.x));

    if(denom == 0.0f)
    {
        if(nume_a == 0.0f && nume_b == 0.0f)
            return COINCIDENT;
        else
            return PARALLEL;
    }

    float ua = nume_a / denom;
    float ub = nume_b / denom;

    if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
    {
        //Get the intersection point
        intersection.x = line1_start.x + ua*(line1_end.x - line1_start.x);
        intersection.y = line1_start.y + ua*(line1_end.y - line1_start.y);

        return INTERESECTING;
    }

    return NOT_INTERSECTING;
}

//----------------------------------------------------------------------------//
bool SVGTesselator::isPolygonClockwise(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3)
{
    return ( (point2.x-point1.x)*(point3.y-point1.y) - (point3.x-point1.x)*(point2.y-point1.y) ) < 0.0f;
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStroke(const std::vector<glm::vec2>& points,
                                 const SVGPaintStyle& paint_style,
                                 GeometryBuffer& geometry_buffer,
                                 const bool is_shape_closed)
{
    if(points.size() <= 1)
        return;

    // Calculate half of the stroke width
    const float stroke_half_width = paint_style.d_strokeWidth * 0.5f;
    // Create an object containing all the data we need for our segment processing
    StrokeSegmentData stroke_data(geometry_buffer, stroke_half_width, paint_style);

    // Get and add the stroke colour
    stroke_data.d_strokeVertex.d_colour = getStrokeColour(paint_style);
    // Set the z coordinate
    stroke_data.d_strokeVertex.d_position.z = 0.0f;

    const size_t points_count = points.size();
    size_t i = 0;
   
    // Handle start of stroke
    if(!is_shape_closed)
    {
        //Create linecap
        stroke_data.setPoints(points[0], points[0], points[1]);
        createLinecap(stroke_data, true);

        ++i;
    }
    else
    {
        //Get the first two stroke points without drawing anything
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[0]);
        createStrokeSegment(stroke_data, false);

        //Add the segment connected via the first point
        stroke_data.setPoints(points[points_count - 1], points[i], points[i + 1]);
        createStrokeSegment(stroke_data);

        ++i;
    }

    // Handle segments between start and end of the stroke
    for(; i < points_count - 1; ++i)
    {       
        stroke_data.setPoints(points[i - 1], points[i], points[i + 1]);
        createStrokeSegment(stroke_data);
    }

    // Handle end of stroke
    if(!is_shape_closed)
    {
        //Create linecap
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[points_count - 1]);
        createLinecap(stroke_data, false);
    }
    else
    {
        //Add the segment connected via the last point
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[0]);
        createStrokeSegment(stroke_data);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegment(StrokeSegmentData& stroke_data,
                                        const bool draw)
{
    const SVGPaintStyle::SVGLinejoin& linejoin = stroke_data.d_paintStyle.d_strokeLinejoin;
    const glm::vec2& prev_point = *stroke_data.d_prevPoint;
    const glm::vec2& cur_point = *stroke_data.d_curPoint;
    const glm::vec2& next_point = *stroke_data.d_nextPoint;

    bool polygon_is_clockwise = isPolygonClockwise(prev_point, cur_point, next_point);
    float direction_sign = 1.0f;
    if(!polygon_is_clockwise)
        direction_sign = -1.0f;

    glm::vec2 prevToCur( direction_sign * glm::normalize(cur_point - prev_point) );
    glm::vec2 prevDirToInside = stroke_data.d_strokeHalfWidth * glm::vec2( prevToCur.y, -prevToCur.x );

    glm::vec2 curToNext( direction_sign * glm::normalize(next_point - cur_point) );
    glm::vec2 nextDirToInside = stroke_data.d_strokeHalfWidth * glm::vec2( curToNext.y, -curToNext.x );

    // We calculate the intersection of the inner lines along the stroke
    glm::vec2 inner_intersection;
    int intersection_result = intersect(prev_point + prevDirToInside, cur_point + prevDirToInside,
                                        next_point + nextDirToInside, cur_point + nextDirToInside,
                                        inner_intersection);

    // The outer connection point of the stroke
    glm::vec2 outer_point;
    const glm::vec2& segmentEndLeft = polygon_is_clockwise ? outer_point : inner_intersection;
    const glm::vec2& segmentEndRight = polygon_is_clockwise ? inner_intersection : outer_point;

    if(linejoin == SVGPaintStyle::SLJ_MITER)
    {
        //We calculate the connection point of the outer lines
        outer_point = cur_point + cur_point - inner_intersection;

        if(draw)
        {
            CEGUI::ColouredVertex& stroke_fill_vertex = stroke_data.d_strokeVertex;
            GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndLeft;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndLeft;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndRight;
            geometry_buffer.appendVertex(stroke_fill_vertex);
        }

        stroke_data.d_lastPointLeft = segmentEndLeft;
        stroke_data.d_lastPointRight = segmentEndRight;
    }
    else if(linejoin == SVGPaintStyle::SLJ_BEVEL ||
        linejoin == SVGPaintStyle::SLJ_ROUND)
    {
        //Is the first bevel corner point
        outer_point = cur_point - prevDirToInside;
        //The second bevel corner point
        glm::vec2 secondBevelPoint = cur_point - nextDirToInside;

        if(draw)    
        {
            //Draw part until the bevel
            CEGUI::ColouredVertex& stroke_fill_vertex = stroke_data.d_strokeVertex;
            GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndLeft;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndLeft;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
            geometry_buffer.appendVertex(stroke_fill_vertex);

            stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndRight;
            geometry_buffer.appendVertex(stroke_fill_vertex);
        }


        if(linejoin == SVGPaintStyle::SLJ_BEVEL)
        {
            if(draw)
            {
                //Draw the bevel
                CEGUI::ColouredVertex& stroke_fill_vertex = stroke_data.d_strokeVertex;
                GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

                stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndLeft;
                geometry_buffer.appendVertex(stroke_fill_vertex);

                stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = segmentEndRight;
                geometry_buffer.appendVertex(stroke_fill_vertex);

                stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = secondBevelPoint;
                geometry_buffer.appendVertex(stroke_fill_vertex);
            }

            if(polygon_is_clockwise)
            {
                stroke_data.d_lastPointLeft = secondBevelPoint;
                stroke_data.d_lastPointRight = segmentEndRight;
            }
            else
            {
                stroke_data.d_lastPointLeft = segmentEndLeft;
                stroke_data.d_lastPointRight = secondBevelPoint;
            }
        }
    }



}

//----------------------------------------------------------------------------//
void SVGTesselator::createLinecap(StrokeSegmentData& stroke_data,
                                  const bool is_start)
{
    const SVGPaintStyle::SVGLinecap& linecap = stroke_data.d_paintStyle.d_strokeLinecap;
    const glm::vec2& point1 = is_start ? *stroke_data.d_curPoint : *stroke_data.d_prevPoint;
    const glm::vec2& point2 = is_start ? *stroke_data.d_nextPoint : *stroke_data.d_curPoint;

    glm::vec2 linecapDir( glm::normalize(point2 - point1) );
    glm::vec2 vecToOutside = stroke_data.d_strokeHalfWidth * glm::vec2( -linecapDir.y, linecapDir.x );
    
    glm::vec2 linecap_left, linecap_right;

    if(linecap == SVGPaintStyle::SLC_BUTT ||
       linecap == SVGPaintStyle::SLC_ROUND )
    {
        const glm::vec2& cur_point = *stroke_data.d_curPoint;

        linecap_left = cur_point + vecToOutside;
        linecap_right = cur_point - vecToOutside;
    }
    else if(linecap == SVGPaintStyle::SLC_SQUARE)
    {
        const glm::vec2 cur_point = *stroke_data.d_curPoint - stroke_data.d_strokeHalfWidth * linecapDir;

        linecap_left = cur_point + vecToOutside;
        linecap_right = cur_point - vecToOutside;
    }


    if(!is_start)
    {
        CEGUI::ColouredVertex& stroke_fill_vertex = stroke_data.d_strokeVertex;
        GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;
    
        stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
        geometry_buffer.appendVertex(stroke_fill_vertex);

        stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
        geometry_buffer.appendVertex(stroke_fill_vertex);

        stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_left;
        geometry_buffer.appendVertex(stroke_fill_vertex);

        stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_left;
        geometry_buffer.appendVertex(stroke_fill_vertex);

        stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
        geometry_buffer.appendVertex(stroke_fill_vertex);

        stroke_fill_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_right;
        geometry_buffer.appendVertex(stroke_fill_vertex);
    }

    stroke_data.d_lastPointLeft = linecap_left;
    stroke_data.d_lastPointRight = linecap_right;
}

//----------------------------------------------------------------------------//
}

