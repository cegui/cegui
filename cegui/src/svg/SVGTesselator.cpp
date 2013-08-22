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

// Start of CEGUI namespace section
namespace CEGUI
{

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
    const float stroke_half_width = polyline->d_paintStyle.d_strokeWidth * 0.5f;

    //Getting the points defining the polyline
    const SVGPolyline::PolylinePointsList& points = polyline->d_points;

    //Create the polyline stroke vertex
    ColouredVertex line_stroke_vertex(glm::vec3(), stroke_colour);

    //Draw the line segments TODO Ident: Draw them seamlessly connected with bevels and stuff provided optionally
    size_t points_count = points.size();
    for (size_t j = 1; j < points_count; ++j)
    {
        const glm::vec2& prevPos = points.at(j - 1);
        const glm::vec2& currentPos = points.at(j);

        // Normalize and tilt the 2D direction vector by 90° to get the vector pointing in the offset direction
        glm::vec2 offsetVector = currentPos - prevPos;
        offsetVector = glm::normalize(offsetVector);
        offsetVector = glm::vec2(offsetVector.y, -offsetVector.x) * stroke_half_width;

        line_stroke_vertex.d_position = glm::vec3(prevPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(line_stroke_vertex);

        line_stroke_vertex.d_position = glm::vec3(currentPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(line_stroke_vertex);

        line_stroke_vertex.d_position = glm::vec3(currentPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(line_stroke_vertex);

        line_stroke_vertex.d_position = glm::vec3(currentPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(line_stroke_vertex);

        line_stroke_vertex.d_position = glm::vec3(prevPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(line_stroke_vertex);

        line_stroke_vertex.d_position = glm::vec3(prevPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(line_stroke_vertex);
    }
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
    const Colour stroke_colour = getStrokeColour(paint_style);

    //Create the rectangle fill vertex
    ColouredVertex rectFillVertex(glm::vec3(), fill_colour);

    const float rectX1 = 0.0f;
    const float rectY1 = 0.0f;
    const float& rectX2 = rect->d_width;
    const float& rectY2 = rect->d_height;

    rectFillVertex.d_position = glm::vec3(rectX1, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position = glm::vec3(rectX1, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position = glm::vec3(rectX2, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position = glm::vec3(rectX2, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position = glm::vec3(rectX1, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position = glm::vec3(rectX2, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);
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
    float num_segments = (2.0f * 3.1415926535897932384626433832795f) / theta;

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


//----------------------------------------------------------------------------//
}

