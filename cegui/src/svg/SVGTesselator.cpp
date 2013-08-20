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
    CEGUI::GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings);

    const glm::vec3& strokeColourValues = polyline->d_paintStyle.d_stroke.d_colour;
    const CEGUI::Colour stroke_colour(strokeColourValues.x, strokeColourValues.y, strokeColourValues.z, polyline->d_paintStyle.d_strokeOpacity);
    const SVGPolyline::PolylinePointsList& points = polyline->d_points;
    const float stroke_half_width = polyline->d_paintStyle.d_strokeWidth * 0.5f;

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

        CEGUI::ColouredVertex linePositionVertex;
        glm::vec2 vertexPosition;
        linePositionVertex.colour_val = stroke_colour;

        linePositionVertex.position = glm::vec3(prevPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(prevPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(prevPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);
    }
}


//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderRect(const SVGRect* rect,
                                           std::vector<GeometryBuffer*>& geometry_buffers,
                                           const SVGImage::SVGImageRenderSettings& render_settings)
{
    CEGUI::GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings);

    const glm::vec3& fill_colour_values = rect->d_paintStyle.d_fill.d_colour;
    const CEGUI::Colour fill_colour(fill_colour_values.x, fill_colour_values.y,
                                    fill_colour_values.z, rect->d_paintStyle.d_fillOpacity);

    //Draw the rectangle fill
    CEGUI::ColouredVertex rectFillVertex;
    glm::vec2 vertexPosition;
    rectFillVertex.colour_val = fill_colour;

    const float& rectX1 = rect->d_x;
    const float& rectY1 = rect->d_y;
    const float rectX2 = rect->d_x + rect->d_width;
    const float rectY2 = rect->d_y + rect->d_height;

    rectFillVertex.position = glm::vec3(rectX1, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX1, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX2, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX2, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX1, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX2, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderCircle(const SVGCircle* circle,
                                             std::vector<GeometryBuffer*>& geometry_buffers,
                                             const SVGImage::SVGImageRenderSettings& render_settings)
{
    CEGUI::GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings);

    float max_scale = std::max(render_settings.d_scaleFactor.d_x, render_settings.d_scaleFactor.d_y);

    const glm::vec3& fill_colour_values = circle->d_paintStyle.d_fill.d_colour;
    const CEGUI::Colour fill_colour(fill_colour_values.x, fill_colour_values.y,
                                    fill_colour_values.z, circle->d_paintStyle.d_fillOpacity);

    //Circle fill vertex
    CEGUI::ColouredVertex circle_fill_vertex;
    circle_fill_vertex.colour_val = fill_colour;

    glm::vec2 circle_position(circle->d_cx, circle->d_cy);
    const float& r = circle->d_r;

    // The numeric value in this can be set freely. The lower, the more segments there will be, making the circle appear rounder.
    static const float circle_render_roundness_value = 1.5f;
    const float segment_length = circle_render_roundness_value / max_scale;
    float theta = std::acos( 1 - ( segment_length / r ) );
    float num_segments = (2.0f * 3.1415926f) / theta;

    //precalculate the sine and cosine
	float c = std::cosf(theta);
	float s = std::sinf(theta);
	float t;

    // we start at angle = 0 
    glm::vec2 current_pos(r, 0);
    
	for(int i = 0; i < num_segments; i++) 
	{ 
        circle_fill_vertex.position = glm::vec3(circle_position + glm::vec2(circle->d_r, 0.0f), 0.0f);
        geometry_buffer.appendVertex(circle_fill_vertex);

        circle_fill_vertex.position = glm::vec3(circle_position + current_pos, 0.0f);
        geometry_buffer.appendVertex(circle_fill_vertex);

		//apply the rotation matrix
		t = current_pos.x;
		current_pos.x = c * current_pos.x - s * current_pos.y;
		current_pos.y = s * t + c * current_pos.y;

        circle_fill_vertex.position = glm::vec3(circle_position + current_pos, 0.0f);
        geometry_buffer.appendVertex(circle_fill_vertex);
	} 
}

//----------------------------------------------------------------------------//
CEGUI::GeometryBuffer& SVGTesselator::setupGeometryBufferColoured(std::vector<GeometryBuffer*>& geometry_buffers, const SVGImage::SVGImageRenderSettings& render_settings)
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

    return geometry_buffer;
}

//----------------------------------------------------------------------------//
}

