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

#include <cmath>


// Start of CEGUI namespace section
namespace CEGUI
{
//Internal numeric value for  circle roundness. The lower, the better tesselated the
//circle will be. We will set it to an, for our needs, appropriate fixed value.
const float CircleRoundnessValue = 1.5f;

//----------------------------------------------------------------------------//
SVGTesselator::StrokeSegmentData::StrokeSegmentData(GeometryBuffer& geometry_buffer,
                                                    const float stroke_half_width,
                                                    const SVGPaintStyle& paint_style,
                                                    const float max_scale) :
    d_geometryBuffer(geometry_buffer),
    d_strokeHalfWidth(stroke_half_width),
    d_paintStyle(paint_style),
    d_maxScale(max_scale),
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
void SVGTesselator::tesselateAndRenderLine(const SVGLine* line,
                                           std::vector<GeometryBuffer*>& geometry_buffers,
                                           const SVGImage::SVGImageRenderSettings& render_settings)
{
    GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings, line->d_transformation);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = line->d_paintStyle;

    //Create the line points and add them to the stroke points list
    std::vector<glm::vec2> points;
    points.push_back(glm::vec2(line->d_x1, line->d_y1));
    points.push_back(glm::vec2(line->d_x2, line->d_y2));

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = getScaleFactors(line->d_transformation, render_settings);

    //Create and append the polyline's stroke geometry
    createStroke(points, geometry_buffer, paint_style, render_settings, scale_factors, false);
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderPolyline(const SVGPolyline* polyline,
                                               std::vector<GeometryBuffer*>& geometry_buffers,
                                               const SVGImage::SVGImageRenderSettings& render_settings)
{
    GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings, polyline->d_transformation);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = polyline->d_paintStyle;

    //Getting the points defining the polyline
    const SVGPolyline::PolylinePointsList& points = polyline->d_points;

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = getScaleFactors(polyline->d_transformation, render_settings);

    //Create and append the polyline's stroke geometry
    createStroke(points, geometry_buffer, paint_style, render_settings, scale_factors, false);
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

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = getScaleFactors(rect->d_transformation, render_settings);

    //Make a list of rectangle (corner) points
    std::vector<glm::vec2> rectangle_points;
    rectangle_points.push_back( glm::vec2(0.0f,          0.0f          ) );
    rectangle_points.push_back( glm::vec2(0.0f,          rect->d_height) );
    rectangle_points.push_back( glm::vec2(rect->d_width, rect->d_height) );
    rectangle_points.push_back( glm::vec2(rect->d_width, 0.0f          ) );
  
    //Create and append the rectangle's fill geometry
    createTriangleStripFillGeometry(rectangle_points, geometry_buffer, paint_style);

    //Create and append the rectangle's stroke geometry
    createStroke(rectangle_points, geometry_buffer, paint_style, render_settings, scale_factors, true);
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderCircle(const SVGCircle* circle,
                                             std::vector<GeometryBuffer*>& geometry_buffers,
                                             const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::mat3x3 transformation = glm::mat3(1.0f, 0.0f, circle->d_cx,
                                           0.0f, 1.0f, circle->d_cy,
                                           0.0f, 0.0f, 1.0f ) * circle->d_transformation;

    //The shape's paint styles
    const SVGPaintStyle& paint_style = circle->d_paintStyle;

    //We need this to determine the degree of tesselation required for the curved elements
    float max_scale = std::max(render_settings.d_scaleFactor.d_x, render_settings.d_scaleFactor.d_y);

    //Get the radius
    const float& radius = circle->d_r;

    GeometryBuffer& geometry_buffer = setupGeometryBufferColoured(geometry_buffers, render_settings, transformation);
  
    //Create and append the circle's fill geometry
    createCircleFill(radius, max_scale, paint_style, geometry_buffer);
    //Create and append the circle's stroke geometry
    createCircleStroke(radius, max_scale, paint_style, geometry_buffer);
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
                                 GeometryBuffer& geometry_buffer,
                                 const SVGPaintStyle& paint_style,
                                 const SVGImage::SVGImageRenderSettings& render_settings,
                                 const glm::vec2& scale_factors,
                                 const bool is_shape_closed)
{
    if(points.size() <= 1 || paint_style.d_stroke.d_none || paint_style.d_strokeWidth == 0.0f)
        return;

    // Calculate half of the stroke width
    const float stroke_half_width = paint_style.d_strokeWidth * 0.5f;

    //We need this to determine the degree of tesselation required for the curved elements
    float max_scale = std::max(render_settings.d_scaleFactor.d_x, render_settings.d_scaleFactor.d_y);

    // Create an object containing all the data we need for our segment processing
    StrokeSegmentData stroke_data(geometry_buffer, stroke_half_width, paint_style, max_scale);

    // Get and add the stroke colour
    stroke_data.d_strokeVertex.d_colour = getStrokeColour(paint_style);
    // Set the z coordinate
    stroke_data.d_strokeVertex.d_position.z = 0.0f;
    //Create the fade stroke colour from the normal colour
    stroke_data.d_strokeFadeVertex = stroke_data.d_strokeVertex;
    //and set the alpha to 0
    stroke_data.d_strokeFadeVertex.d_colour.setAlpha(0.0f);

    const size_t points_count = points.size();
    size_t i = 0;
   
    // Handle start of stroke
    if(!is_shape_closed)
    {
        //Create linecap
        stroke_data.setPoints(points[0], points[0], points[1]);
        createLinecap(stroke_data, render_settings, scale_factors, true);

        ++i;
    }
    else
    {
        //Get the first two stroke points without drawing anything
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[0]);
        createStrokeSegment(stroke_data, render_settings, scale_factors, false);

        //Add the segment connected via the first point
        stroke_data.setPoints(points[points_count - 1], points[i], points[i + 1]);
        createStrokeSegment(stroke_data, render_settings, scale_factors, true);

        ++i;
    }

    // Handle segments between start and end of the stroke
    for(; i < points_count - 1; ++i)
    {       
        stroke_data.setPoints(points[i - 1], points[i], points[i + 1]);
        createStrokeSegment(stroke_data, render_settings, scale_factors, true);
    }

    // Handle end of stroke
    if(!is_shape_closed)
    {
        //Create linecap
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[points_count - 1]);
        createLinecap(stroke_data, render_settings, scale_factors, false);
    }
    else
    {
        //Add the segment connected via the last point
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[0]);
        createStrokeSegment(stroke_data, render_settings, scale_factors, true);
    }
}

glm::vec2 getDirectionVectorTiltedRight(const glm::vec2& start, const glm::vec2& end)
{
    return glm::normalize( glm::vec2(end.y - start.y, start.x - end.x) );
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegment(StrokeSegmentData& stroke_data,
                                        const SVGImage::SVGImageRenderSettings& render_settings,
                                        const glm::vec2& scale_factors,
                                        const bool draw)
{
    SVGPaintStyle::SVGLinejoin linejoin = stroke_data.d_paintStyle.d_strokeLinejoin;
    const glm::vec2& prev_point = *stroke_data.d_prevPoint;
    const glm::vec2& cur_point = *stroke_data.d_curPoint;
    const glm::vec2& next_point = *stroke_data.d_nextPoint;

    // Check if our corner points form a clockwise or anticlockwise polygon
    bool polygon_is_clockwise = isPolygonClockwise(prev_point, cur_point, next_point);
    float direction_sign = polygon_is_clockwise ? 1.0f : -1.0f;

    glm::vec2 prev_dir_to_inside( direction_sign * getDirectionVectorTiltedRight(prev_point, cur_point) );
    glm::vec2 prev_vec_to_inside = stroke_data.d_strokeHalfWidth * prev_dir_to_inside;

    glm::vec2 next_dir_to_inside( direction_sign * getDirectionVectorTiltedRight(cur_point, next_point) );
    glm::vec2 next_vec_to_inside = stroke_data.d_strokeHalfWidth * next_dir_to_inside;

    // We calculate the intersection of the inner lines along the stroke
    glm::vec2 inner_intersection;
    int intersection_result = intersect(prev_point + prev_vec_to_inside, cur_point + prev_vec_to_inside,
                                        next_point + next_vec_to_inside, cur_point + next_vec_to_inside,
                                        inner_intersection);

    // The outer connection point of the stroke
    glm::vec2 outer_point;
    // Reference to the end-points of our stroke segment
    const glm::vec2& segment_end_left = polygon_is_clockwise ? outer_point : inner_intersection;
    const glm::vec2& segment_end_right = polygon_is_clockwise ? inner_intersection : outer_point;

    //Get the anti-aliasing offsets
    float core_offset, fade_offset;
    determineAntiAliasingOffsets(stroke_data.d_paintStyle.d_strokeWidth, core_offset, fade_offset);

    //If the stroke miter is exceeded we fall back to bevel
    if(linejoin == SVGPaintStyle::SLJ_MITER)
        handleStrokeMiterExceedance(stroke_data, cur_point, inner_intersection, linejoin);

    //Switch through the types and render them if required 
    if(linejoin == SVGPaintStyle::SLJ_MITER)
    {
        //We calculate the connection point of the outer lines
        outer_point = cur_point + cur_point - inner_intersection;

        if(!render_settings.d_antiAliasing)
            createStrokeSegmentConnection(stroke_data, segment_end_left, segment_end_right, draw);
        else
            calculateAndAddStrokeSegmentAAConnection(stroke_data, segment_end_left, segment_end_right,
                                                 polygon_is_clockwise, prev_dir_to_inside, next_dir_to_inside, core_offset,
                                                 fade_offset, scale_factors, draw);
    }
    else if(linejoin == SVGPaintStyle::SLJ_BEVEL ||
            linejoin == SVGPaintStyle::SLJ_ROUND)
    {
        //Is the first bevel corner point
        outer_point = cur_point - prev_vec_to_inside;
        //The second bevel corner point
        const glm::vec2 secondBevelPoint = cur_point - next_vec_to_inside;

        createStrokeSegmentConnection(stroke_data, segment_end_left, segment_end_right, draw);

        if(draw)
        {
            createStrokeLinejoinBevelOrRound(stroke_data, cur_point, secondBevelPoint,
                                             segment_end_left, segment_end_right, linejoin,
                                             polygon_is_clockwise);
        }

        stroke_data.d_lastPointLeft = polygon_is_clockwise ? secondBevelPoint : segment_end_left;
        stroke_data.d_lastPointRight = polygon_is_clockwise ? segment_end_right : secondBevelPoint;
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::determineAntiAliasingOffsets(float width, float& core_offset, float& fade_offset)
{
	float remainder = width - static_cast<int>(width);
	
    if ( width >= 6.0f)
    {
		float ff = width - 6.0f;
		core_offset = 2.5f + ff * 0.50f;
        fade_offset = 1.08f;
	}
    else if ( width >= 5.0f)
    {
		core_offset = 1.9f + remainder * 0.6f;
        fade_offset=1.08f;
	} 
    else if ( width >= 4.0f)
    {
		core_offset = 1.44f + remainder * 0.46f;
        fade_offset=1.08f;
	}
    else if ( width >= 3.0f)
    {
		core_offset = 0.96f + remainder * 0.48f;
        fade_offset=1.08f;
	}
     else if ( width >= 2.0f)
    {
		core_offset = 0.38f + remainder * 0.58f;
        fade_offset=1.08f;
	} 
     else if ( width >= 1.0f)
    {
		core_offset = 0.05f + remainder * 0.33f;
        fade_offset = 0.768f + 0.312f * remainder;
	}
	else if ( width >= 0.0f)
    {
		core_offset = 0.05f;
        fade_offset=0.768f;
	}

    core_offset -= width * 0.5f;
}

void line(glm::vec2 start, glm::vec2 end, //coordinates of the line
          float width, CEGUI::Colour& colour)
{
	float		t, R;
	//determineAntiAliasingOffsets(width, t, R);
	float		f = width - static_cast<int>(width);
	float		A = colour.getAlpha();

    CEGUI::Colour fade_colour = colour;
    fade_colour.setAlpha(0.0f);

	//determine angle of the line to horizontal
	glm::vec2	coreThick; //core thickness of a line
	glm::vec2	fadingEdge; //fading edge of a line
	glm::vec2	cap; //cap of a line
	double		ALW = 0.01;
	glm::vec2	direction = end - start;

    if (std::abs(direction.x) < ALW)
    {
        //vertical
        coreThick = glm::vec2(t, 0.0f);
        fadingEdge = glm::vec2(R, 0.0f);
        if (width <= 1.0f)
        {
            coreThick.x = 0.5f;
            fadingEdge.x = 0.0f;
        }
    }
    else if (std::abs(direction.y) < ALW)
    {
        //horizontal
        coreThick = glm::vec2(0.0f, t);
        fadingEdge = glm::vec2(0.0f, R);
        if ( width <= 1.0f)
        {
            coreThick.y = 0.5f;
            fadingEdge.y = 0.0f;
        }
    }
    else
    {
        direction = glm::normalize( glm::vec2( -direction.y, direction.x ) );
        cap = glm::vec2( -direction.y, direction.x );
        coreThick = t * direction;
        fadingEdge = R * direction;
    }

	start += cap * 0.5f;
	end -= cap * 0.5f;

/*
	//draw the line by triangle strip
	glm::vec2 line_vertex[] =
		{
			start-coreThick-fadingEdge-cap, //fading edge1
			end-coreThick-fadingEdge+cap,
			start-coreThick-cap,	  //core
			end-coreThick+cap,
			start+coreThick-cap,
			end+coreThick+cap,
			start+coreThick+fadingEdge-cap, //fading edge2
			end+coreThick+fadingEdge+cap
		};
    CEGUI::Colour line_color[]=
    {
        fade_colour
        fade_colour
        colour
        colour
        colour
        colour
        fade_colour
        fade_colour
    };*/

}

//----------------------------------------------------------------------------//
void SVGTesselator::createLinecap(StrokeSegmentData& stroke_data,
                                  const SVGImage::SVGImageRenderSettings& render_settings,
                                  const glm::vec2& scale_factors,
                                  const bool is_start)
{
    const SVGPaintStyle::SVGLinecap& linecap = stroke_data.d_paintStyle.d_strokeLinecap;
    const glm::vec2& point1 = is_start ? *stroke_data.d_curPoint : *stroke_data.d_prevPoint;
    const glm::vec2& point2 = is_start ? *stroke_data.d_nextPoint : *stroke_data.d_curPoint;

    // Direction towards the linecap
    glm::vec2 linecap_dir( glm::normalize(point2 - point1) );
    // Direction to the left side in linecap direction
    glm::vec2 dir_to_outside( -linecap_dir.y, linecap_dir.x );
    glm::vec2 vec_to_outside = stroke_data.d_strokeHalfWidth * dir_to_outside;
    //Turn around direction to the linecap if we are at the starting cap 
    if(is_start)
        linecap_dir *= -1.0f;

    glm::vec2 linecap_left, linecap_right, linecap_fade_left, linecap_fade_right;
    glm::vec2 linecap_center_point = *stroke_data.d_curPoint;

    if(linecap == SVGPaintStyle::SLC_SQUARE)
        linecap_center_point += stroke_data.d_strokeHalfWidth * linecap_dir;

    linecap_left = linecap_center_point + vec_to_outside;
    linecap_right = linecap_center_point - vec_to_outside;

    if(render_settings.d_antiAliasing && 
        ( linecap == SVGPaintStyle::SLC_BUTT || SVGPaintStyle::SLC_SQUARE) )
    {
        float length_side_scale = calculateLengthScale(dir_to_outside, scale_factors);
        float length_cap_scale = calculateLengthScale(linecap_dir, scale_factors);
        linecap_fade_left = linecap_left + length_side_scale * dir_to_outside + length_cap_scale * linecap_dir;
        linecap_fade_right = linecap_right - length_side_scale * dir_to_outside + length_cap_scale * linecap_dir; 

        addStrokeLinecapAAGeometryVertices(stroke_data, linecap_left, linecap_right, linecap_fade_left, linecap_fade_right);
    }
    
    if(linecap == SVGPaintStyle::SLC_ROUND)
    {
        const glm::vec2& cur_point = *stroke_data.d_curPoint;
        static const float half_circle_angle = glm::pi<float>();

        //Get the parameters
        float num_segments, tangential_factor, radial_factor;
        calculateArcTesselationParameters(stroke_data.d_strokeHalfWidth, half_circle_angle, stroke_data.d_maxScale,
                                          num_segments, tangential_factor, radial_factor);

        //Get the arc points
        std::vector<glm::vec2> arc_points;
        if(is_start)
            createArcPoints(cur_point, linecap_left, linecap_right,
                            num_segments, tangential_factor, radial_factor,
                            arc_points);
        else
            createArcPoints(cur_point, linecap_right, linecap_left,
                            num_segments, tangential_factor, radial_factor,
                            arc_points);

        createArcStrokeGeometry(arc_points, stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex);
    }

    if(!render_settings.d_antiAliasing)
        createStrokeSegmentConnection(stroke_data, linecap_left, linecap_right, !is_start);
    else
        createStrokeSegmentAAConnectionGeometry(stroke_data, linecap_left, linecap_right,
                                                linecap_fade_left, linecap_fade_right, !is_start);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegmentConnection(StrokeSegmentData& stroke_data,
                                                  const glm::vec2& segment_end_left,
                                                  const glm::vec2& segment_end_right,
                                                  const bool draw)
{
    if(draw)
    {
        // Add the geometry
        CEGUI::ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
        GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segment_end_left;
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segment_end_left;
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segment_end_right;
        geometry_buffer.appendVertex(stroke_vertex);
    }

    //! We set our last point values
    stroke_data.d_lastPointLeft = segment_end_left;
    stroke_data.d_lastPointRight = segment_end_right;
}

//----------------------------------------------------------------------------//
void SVGTesselator::calculateAndAddStrokeSegmentAAConnection(StrokeSegmentData& stroke_data,
                                                             const glm::vec2& segment_end_left_orig,
                                                             const glm::vec2& segment_end_right_orig,
                                                             const bool polygon_is_clockwise,
                                                             const glm::vec2& prev_dir_to_inside,
                                                             const glm::vec2& next_dir_to_inside,
                                                             float core_offset,
                                                             float fade_offset,
                                                             const glm::vec2& scale_factors,
                                                             const bool draw)
{ 
    // Get the changed widths of the incoming and outgoing line segments
    float length_scale1 = calculateLengthScale(prev_dir_to_inside, scale_factors);
    float length_scale2 = calculateLengthScale(next_dir_to_inside, scale_factors);

    // Calculate scale-distorted direction
    glm::vec2 prev_to_cur(-prev_dir_to_inside.y, prev_dir_to_inside.x);
    glm::vec2 next_to_cur(next_dir_to_inside.y, -next_dir_to_inside.x) ;
    glm::vec2 direction = glm::normalize(prev_to_cur * length_scale2 + next_to_cur * length_scale1);


    // Calculate how much we need to offset along the direction depending on the angle
    float length_to_corner;

    /* We get the distance to our new corner using a factor. The factor would, in case we didn't prepare for non-uniform scaling, just consist
    of a simple vector projection. However, here we also need to multiply the local stroke width's scale factor (in the direction of the stroke)
    to get currect results. As an alternative calculation we could use saled_length_2 and next_dir_to_inside instead the respective variables, which
    has to lead to the same result.
    */
    if(polygon_is_clockwise) 
        length_to_corner = length_scale1 / glm::dot( -prev_dir_to_inside, direction );
    else
        length_to_corner = length_scale1 / glm::dot( prev_dir_to_inside, direction );
    
    direction *= length_to_corner;

    // Calculate the offset vectors from the original area
    glm::vec2 core_offset_vec = core_offset * direction;
    glm::vec2 fade_offset_vec = fade_offset * direction;
    // Calculate the segment positions
    const glm::vec2 segmentFadeLeftEnd = segment_end_left_orig + fade_offset_vec;
    const glm::vec2 segmentLeftEnd = segment_end_left_orig + core_offset_vec;
    const glm::vec2 segmentRightEnd = segment_end_right_orig - core_offset_vec;
    const glm::vec2 segmentFadeRightEnd = segment_end_right_orig - fade_offset_vec;

    // If we want to draw we have to combine the vertices
    createStrokeSegmentAAConnectionGeometry(stroke_data, segmentLeftEnd, segmentRightEnd,
                                            segmentFadeLeftEnd, segmentFadeRightEnd, draw);
}

//----------------------------------------------------------------------------//
void SVGTesselator::addTriangleGeometry(const glm::vec2& point1,
                                        const glm::vec2& point2,
                                        const glm::vec2& point3,
                                        GeometryBuffer &geometry_buffer,
                                        ColouredVertex &vertex)
{
    vertex.d_position.swizzle(glm::X, glm::Y) = point1;
    geometry_buffer.appendVertex(vertex);

    vertex.d_position.swizzle(glm::X, glm::Y) = point2;
    geometry_buffer.appendVertex(vertex);

    vertex.d_position.swizzle(glm::X, glm::Y) = point3;
    geometry_buffer.appendVertex(vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createTriangleStripFillGeometry(std::vector<glm::vec2>& points,
                                                    GeometryBuffer& geometry_buffer,
                                                    const SVGPaintStyle& paint_style)
{
    if(paint_style.d_fill.d_none)
        return;

    //Create the rectangle fill vertex
    ColouredVertex fill_vertex(glm::vec3(), getFillColour(paint_style));

    //Fixed triangle fan point
    const glm::vec2& point1 = points[0];

    const size_t maximum_index = points.size() - 1;
    for(size_t i = 1; i < maximum_index; ++i)
        addTriangleGeometry(point1, points[i], points[i + 1], geometry_buffer, fill_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCircleFill(const float& radius,
                                     float max_scale,
                                     const SVGPaintStyle& paint_style,
                                     GeometryBuffer& geometry_buffer)
{
    if(paint_style.d_fill.d_none)
        return;

    //Precalculate values needed for the circle tesselation
    float num_segments, cos_value, sin_value;
    calculateCircleTesselationParameters(radius, max_scale, num_segments, cos_value, sin_value);

    //Create the circle points
    std::vector<glm::vec2> circle_points;
    createCirclePoints(radius, num_segments, cos_value, sin_value, circle_points);

    //Generate the geometry from the the circle points
    createTriangleStripFillGeometry(circle_points, geometry_buffer, paint_style);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCircleStroke(const float& radius,
                                       float max_scale,
                                       const SVGPaintStyle& paint_style,
                                       GeometryBuffer& geometry_buffer)
{
    if(paint_style.d_stroke.d_none || paint_style.d_strokeWidth == 0.0f)
        return;

    const float& stroke_width =  paint_style.d_strokeWidth;

    float outer_radius = stroke_width * 0.5f + radius;
    float inner_radius = -stroke_width * 0.5f + radius;

    //Precalculate values needed for the circle tesselation
    float num_segments, cos_value, sin_value;
    calculateCircleTesselationParameters(radius, max_scale, num_segments, cos_value, sin_value);

    //Create the outer and inner circle points
    std::vector<glm::vec2> outer_circle_points;
    createCirclePoints(outer_radius, num_segments, cos_value, sin_value, outer_circle_points);
    std::vector<glm::vec2> inner_circle_points;
    createCirclePoints(inner_radius, num_segments, cos_value, sin_value, inner_circle_points);

    createCircleStrokeGeometry(outer_circle_points, inner_circle_points, paint_style, geometry_buffer);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCircleStrokeGeometry(const std::vector<glm::vec2>& outer_circle_points,
                                               const std::vector<glm::vec2>& inner_circle_points,
                                               const SVGPaintStyle& paint_style,
                                               GeometryBuffer& geometry_buffer)
{
    //Create the rectangle fill vertex
    ColouredVertex stroke_vertex(glm::vec3(), getStrokeColour(paint_style));

    size_t circle_points_count = outer_circle_points.size();
    for(size_t i = 0; i < circle_points_count; ++i)
    {
        int next_index = i + 1;
        if(next_index == circle_points_count)
            next_index = 0;

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = outer_circle_points[i];
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = outer_circle_points[next_index];
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = inner_circle_points[i];
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = inner_circle_points[i];
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = outer_circle_points[next_index];
        geometry_buffer.appendVertex(stroke_vertex);

        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = inner_circle_points[next_index];
        geometry_buffer.appendVertex(stroke_vertex);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCirclePoints(const float radius,
                                       const float num_segments,
                                       const float cos_value,
                                       const float sin_value,
                                       std::vector<glm::vec2>& circle_points)
{
    //Create the circle points
    //We start at angle = 0 
    glm::vec2 current_pos(radius, 0.0f);
    for(int i = 0; i < num_segments; i++) 
    { 
        //Rotate
        float temp_value = current_pos.x;
        current_pos.x = cos_value * current_pos.x - sin_value * current_pos.y;
        current_pos.y = sin_value * temp_value + cos_value * current_pos.y;

        circle_points.push_back(current_pos);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::calculateCircleTesselationParameters(const float radius,
                                                         const float max_scale,
                                                         float& num_segments,
                                                         float& cos_value,
                                                         float& sin_value)
{
    //Adapt the tesselation to the scale
    float segment_length  = CircleRoundnessValue / max_scale;
    float theta = std::acos( 1.0f - ( segment_length / radius ) );

    static const float two_pi = 2.0f * glm::pi<float>(); 
    //Calculate the number of segments using 360° as angle and using theta
    num_segments = two_pi / theta;

    //Precalculate values we will need for our circle tesselation
    cos_value = std::cos(theta);
    sin_value = std::sin(theta);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createArcStrokeGeometry(std::vector<glm::vec2>& points,
                                           GeometryBuffer& geometry_buffer,
                                           ColouredVertex& stroke_vertex)
{
    //Fixed triangle fan point
    const glm::vec2& point1 = points[0];

    const size_t maximum_index = points.size() - 1;
    for(size_t i = 1; i < maximum_index; ++i)
        addTriangleGeometry(point1, points[i], points[i + 1], geometry_buffer, stroke_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::calculateArcTesselationParameters(const float radius,
                                                      const float arc_angle,
                                                      const float max_scale,
                                                      float& num_segments,
                                                      float& tangential_factor,
                                                      float& radial_factor)
{
    //Adapt the tesselation to the scale
    float segment_length  = CircleRoundnessValue / max_scale;
    float theta = std::acos( 1.0f - ( segment_length / radius ) );

    //Calculate the number of segments from the arc angle and theta
	num_segments = arc_angle / theta;

    //Precalculate values we will need for our arc tesselation
	tangential_factor = std::tan(theta);
	radial_factor = std::cos(theta);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createArcPoints(const glm::vec2& center_point,
                                    const glm::vec2& start_point,
                                    const glm::vec2& end_point,
                                    const float num_segments,
                                    const float tangential_factor,
                                    const float radial_factor,
                                    std::vector<glm::vec2>& arc_points)
{
    //Add the start points of the arc to our list
    arc_points.push_back(start_point);
    //Set the current position to be the arc's start point in object coordinates
    glm::vec2 current_pos = start_point - center_point;

    //Calculate the arc points, skip last segment because that will be our endpoint
    for(int i = 0; i < num_segments - 1.0f; i++) 
    { 
		glm::vec2 temp( -current_pos.y, current_pos.x );

		current_pos += temp * tangential_factor;
		current_pos *= radial_factor;

        arc_points.push_back(center_point + current_pos);
    }

    //Add the end point of the arc to our list
    arc_points.push_back(end_point);
}

//----------------------------------------------------------------------------//
void SVGTesselator::handleStrokeMiterExceedance(const StrokeSegmentData& stroke_data,
                                                const glm::vec2& cur_point,
                                                const glm::vec2& inner_intersection,
                                                SVGPaintStyle::SVGLinejoin& linejoin)
{
    //If the miter length we exceeds the limit we will use a regular bevel instead
    const float& miterlimit = stroke_data.d_paintStyle.d_strokeMiterlimit;

    float half_miter_extension = glm::length(cur_point - inner_intersection);
    if(half_miter_extension > (miterlimit * stroke_data.d_strokeHalfWidth))
        linejoin = SVGPaintStyle::SLJ_BEVEL;
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeLinejoinBevelOrRound(StrokeSegmentData &stroke_data,
                                                     const glm::vec2& cur_point,
                                                     const glm::vec2& secondBevelPoint,
                                                     const glm::vec2& segment_end_left,
                                                     const glm::vec2& segment_end_right,
                                                     const SVGPaintStyle::SVGLinejoin linejoin,
                                                     const bool polygon_is_clockwise)
{
    if(linejoin == SVGPaintStyle::SLJ_BEVEL)
    {
        CEGUI::ColouredVertex& stroke_fill_vertex = stroke_data.d_strokeVertex;
        GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

        addTriangleGeometry(segment_end_left, segment_end_right, secondBevelPoint, geometry_buffer, stroke_fill_vertex);
    }
    else if(linejoin == SVGPaintStyle::SLJ_ROUND)
    {
        float arc_angle;
        if(polygon_is_clockwise)
            arc_angle = std::acos( glm::dot( glm::normalize(segment_end_left - cur_point), glm::normalize(secondBevelPoint - cur_point) ) );
        else
            arc_angle = std::acos( glm::dot( glm::normalize(segment_end_right - cur_point), glm::normalize(secondBevelPoint - cur_point)) );

        //Get the parameters
        float num_segments, tangential_factor, radial_factor;
        calculateArcTesselationParameters(stroke_data.d_strokeHalfWidth, arc_angle, stroke_data.d_maxScale,
                                          num_segments, tangential_factor, radial_factor);


        //Get the arc points
        std::vector<glm::vec2> arc_points;

        if(polygon_is_clockwise)
        {
            arc_points.push_back(segment_end_right);
            createArcPoints(cur_point, secondBevelPoint, segment_end_left,
                            num_segments, tangential_factor, radial_factor,
                            arc_points);
        }
        else
        {
            arc_points.push_back(segment_end_left);
            createArcPoints(cur_point, segment_end_right, secondBevelPoint,
                            num_segments, tangential_factor, radial_factor,
                            arc_points);
        }

        createArcStrokeGeometry(arc_points, stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createRectangleFill(const SVGPaintStyle& paint_style, std::vector<glm::vec2>& rectangle_points, GeometryBuffer& geometry_buffer)
{
    if(paint_style.d_fill.d_none)
        return;

    //Get colours
    const Colour fill_colour = getFillColour(paint_style);

    //Create the rectangle fill vertex
    ColouredVertex rectFillVertex(glm::vec3(), fill_colour);
    //Apply z-depth
    rectFillVertex.d_position.z = 0.0f;

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
}

//----------------------------------------------------------------------------//
glm::vec2 SVGTesselator::getScaleFactors(const glm::mat3& transformation, const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::vec2 scale ( glm::length(transformation[0]), glm::length(transformation[1]) );
    scale *= glm::vec2( render_settings.d_scaleFactor.d_x, render_settings.d_scaleFactor.d_y );
    scale = 1.f / scale;

    return scale;
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegmentAAConnectionGeometry(StrokeSegmentData &stroke_data, const glm::vec2& segmentLeftEnd, const glm::vec2& segmentRightEnd,
                                                            const glm::vec2& segmentFadeLeftEnd, const glm::vec2& segmentFadeRightEnd, const bool draw)
{ 
    if(draw)
    {
        CEGUI::ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
        CEGUI::ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
        GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

        //Fade1
        stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointLeft;
        geometry_buffer.appendVertex(stroke_fade_vertex);
        stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = segmentFadeLeftEnd;
        geometry_buffer.appendVertex(stroke_fade_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = segmentFadeLeftEnd;
        geometry_buffer.appendVertex(stroke_fade_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segmentLeftEnd;
        geometry_buffer.appendVertex(stroke_vertex);
        //Core
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segmentLeftEnd;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segmentLeftEnd;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segmentRightEnd;
        geometry_buffer.appendVertex(stroke_vertex);
        //Fade1
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segmentRightEnd;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointRight;
        geometry_buffer.appendVertex(stroke_fade_vertex);
        stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointRight;
        geometry_buffer.appendVertex(stroke_fade_vertex);
        stroke_vertex.d_position.swizzle(glm::X, glm::Y) = segmentRightEnd;
        geometry_buffer.appendVertex(stroke_vertex);
        stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = segmentFadeRightEnd;
        geometry_buffer.appendVertex(stroke_fade_vertex);
    }

    // We set our lastPoint values to the current values
    stroke_data.d_lastPointLeft = segmentLeftEnd;
    stroke_data.d_lastPointRight = segmentRightEnd;
    stroke_data.d_lastFadePointLeft = segmentFadeLeftEnd;
    stroke_data.d_lastFadePointRight = segmentFadeRightEnd;
}

//----------------------------------------------------------------------------//
void SVGTesselator::addStrokeLinecapAAGeometryVertices(StrokeSegmentData &stroke_data,
                                                       const glm::vec2& linecap_left, const glm::vec2& linecap_right,
                                                       const glm::vec2& linecap_fade_left, const glm::vec2& linecap_fade_right)
{ 
    CEGUI::ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    CEGUI::ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_fade_left;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_fade_right;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_left;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_left;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_fade_right;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = linecap_right;
    geometry_buffer.appendVertex(stroke_vertex);
}

//----------------------------------------------------------------------------//
float SVGTesselator::calculateLengthScale(const glm::vec2 &direction, const glm::vec2& scale_factors)
{
    static const float half_pi = glm::half_pi<float>();
    static const float one_over_pi_half = 1.0f / half_pi;

    float angle_to_y_axis = std::acos( std::abs(direction.y) );
    float angle_to_x_axis = half_pi - angle_to_y_axis;
    return one_over_pi_half * (scale_factors.x * angle_to_y_axis + scale_factors.y * angle_to_x_axis);
}

//----------------------------------------------------------------------------//
}

