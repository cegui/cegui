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
    glm::vec2 scale_factors = determineScaleFactors(line->d_transformation, render_settings);

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
    glm::vec2 scale_factors = determineScaleFactors(polyline->d_transformation, render_settings);

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
    glm::vec2 scale_factors = determineScaleFactors(rect->d_transformation, render_settings);

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
    if(points.size() < 2 || paint_style.d_stroke.d_none || paint_style.d_strokeWidth == 0.0f)
        return;

    //We need this to determine the degree of tesselation required for the curved elements
    float max_scale = std::max(render_settings.d_scaleFactor.d_x, render_settings.d_scaleFactor.d_y);

    // Create an object containing all the data we need for our segment processing
    StrokeSegmentData stroke_data(geometry_buffer, paint_style.d_strokeWidth * 0.5f, paint_style, max_scale);

    // If doing anti-aliasing, get the anti-aliasing offsets
    if(render_settings.d_antiAliasing)
        determineAntiAliasingOffsets(stroke_data.d_paintStyle.d_strokeWidth, stroke_data.d_antiAliasingOffsets);
    // Get and add the stroke colour
    stroke_data.d_strokeVertex.d_colour = getStrokeColour(paint_style);
    // Set the z coordinate
    stroke_data.d_strokeVertex.d_position.z = 0.0f;
    //Create the fade stroke colour from the normal colour and set the alpha to 0
    stroke_data.d_strokeFadeVertex = stroke_data.d_strokeVertex;
    stroke_data.d_strokeFadeVertex.d_colour.setAlpha(0.0f);

    const size_t points_count = points.size();
    size_t i = 0;
   
    // Handle the beginning of the stroke considering that the shape might be open and therefore needs linecaps
    if(!is_shape_closed)
    {
        //Create the starting linecap
        stroke_data.setPoints(points[0], points[0], points[1]);
        createStrokeLinecap(stroke_data, render_settings, scale_factors, true);

        ++i;
    }
    else
    {
        //Get the first two stroke points without drawing anything
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[0]);
        createStrokeLinejoin(stroke_data, render_settings, scale_factors, false);

        if(!render_settings.d_antiAliasing)
            setStrokeDataSubsequentPointsAsLastPoints(stroke_data);
        else
            setStrokeDataSubsequentPointsAsLastPointsAA(stroke_data);


        //Add the segment connected via the first point
        stroke_data.setPoints(points[points_count - 1], points[i], points[i + 1]);
        createStrokeLinejoin(stroke_data, render_settings, scale_factors, true);

        if(!render_settings.d_antiAliasing)
        {
            createStrokeSegmentConnection(stroke_data);
            setStrokeDataSubsequentPointsAsLastPoints(stroke_data);
        }
        else
        {
            createStrokeSegmentConnectionAA(stroke_data);
            setStrokeDataSubsequentPointsAsLastPointsAA(stroke_data);
        }

        ++i;
    }

    // Handle segments between start and end of the stroke
    for(; i < points_count - 1; ++i)
    {       
        stroke_data.setPoints(points[i - 1], points[i], points[i + 1]);
        createStrokeLinejoin(stroke_data, render_settings, scale_factors, true);

        if(!render_settings.d_antiAliasing)
        {
            createStrokeSegmentConnection(stroke_data);
            setStrokeDataSubsequentPointsAsLastPoints(stroke_data);
        }
        else
        {
            createStrokeSegmentConnectionAA(stroke_data);
            setStrokeDataSubsequentPointsAsLastPointsAA(stroke_data);
        }
    }

    // Handle the end of the stroke considering that the shape might be open and therefore needs linecaps
    if(!is_shape_closed)
    {
        //Set out last points as current points so we do not override them with he linecap creation
        if(!render_settings.d_antiAliasing)
            setStrokeDataLastPointsAsCurrentPoints(stroke_data);
        else
            setStrokeDataLastPointsAsCurrentPointsAA(stroke_data);

        //Create linecap
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[points_count - 1]);
        createStrokeLinecap(stroke_data, render_settings, scale_factors, false);

        //Connect to the linecap
        if(!render_settings.d_antiAliasing)
            createStrokeSegmentConnection(stroke_data);
        else
            createStrokeSegmentConnectionAA(stroke_data);
    }
    else
    {
        //Add the segment connected via the last point
        stroke_data.setPoints(points[points_count - 2], points[points_count - 1], points[0]);
        createStrokeLinejoin(stroke_data, render_settings, scale_factors, true);

        if(!render_settings.d_antiAliasing)
            createStrokeSegmentConnection(stroke_data);
        else
            createStrokeSegmentConnectionAA(stroke_data);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeLinejoin(StrokeSegmentData& stroke_data,
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

    glm::vec2 prev_to_cur( glm::normalize(cur_point - prev_point) );
    glm::vec2 prev_dir_to_inside ( direction_sign * glm::vec2(prev_to_cur.y, -prev_to_cur.x) );
    glm::vec2 prev_vec_to_inside = stroke_data.d_strokeHalfWidth * prev_dir_to_inside;

    glm::vec2 cur_to_next( glm::normalize(next_point - cur_point) );
    glm::vec2 next_dir_to_inside( direction_sign * glm::vec2(cur_to_next.y, -cur_to_next.x) );
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

    //If the stroke miter is exceeded we fall back to bevel
    if(linejoin == SVGPaintStyle::SLJ_MITER)
        handleStrokeMiterExceedance(stroke_data, cur_point, inner_intersection, linejoin);

    //Switch through the types and render them if required 
    if(linejoin == SVGPaintStyle::SLJ_MITER)
    {
        //We calculate the connection point of the outer lines
        outer_point = cur_point + cur_point - inner_intersection;

        if(!render_settings.d_antiAliasing)
        {
            setStrokeDataCurrentPoints(stroke_data, segment_end_left, segment_end_right);
            setStrokeDataSubsequentPoints(stroke_data, segment_end_left, segment_end_right);
        }
        else
            calculateAAMiterAndSetConnectionPoints(stroke_data, segment_end_left, segment_end_right, polygon_is_clockwise,
                                                   prev_to_cur, cur_to_next, prev_dir_to_inside, next_dir_to_inside, scale_factors);
    }
    else if(linejoin == SVGPaintStyle::SLJ_BEVEL || linejoin == SVGPaintStyle::SLJ_ROUND)
    {
        //Is the first bevel corner point
        outer_point = cur_point - prev_vec_to_inside;
        //The second bevel corner point
        const glm::vec2 second_bevel_point = cur_point - next_vec_to_inside;

        if(!render_settings.d_antiAliasing)
            createStrokeLinejoinBevelOrRound(stroke_data, cur_point, prev_dir_to_inside, next_dir_to_inside, segment_end_left, segment_end_right,
                                             second_bevel_point, linejoin, polygon_is_clockwise, draw);
        else
            createStrokeLinejoinBevelOrRoundAA(stroke_data, render_settings, scale_factors, cur_point, second_bevel_point,
                                               segment_end_left, segment_end_right, prev_to_cur, cur_to_next,
                                               prev_dir_to_inside, next_dir_to_inside, linejoin, polygon_is_clockwise, draw);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::determineAntiAliasingOffsets(float width, glm::vec2& antialiasing_offsets)
{
	float remainder = width - static_cast<int>(width);

    float& core_offset = antialiasing_offsets.x;
    float& fade_offset = antialiasing_offsets.y;
	
    if ( width >= 6.0f)
    {
		float ff = width - 6.0f;
		core_offset = 2.5f + ff * 0.50f;
        fade_offset = 1.08f;
	}
    else if ( width >= 5.0f)
    {
		core_offset = 1.9f + remainder * 0.6f;
        fade_offset = 1.08f;
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
        fade_offset = 1.08f;
	} 
     else if ( width >= 1.0f)
    {
		core_offset = 0.05f + remainder * 0.33f;
        fade_offset = 0.768f + 0.312f * remainder;
	}
	else if ( width >= 0.0f)
    {
		core_offset = 0.05f;
        fade_offset = 0.768f;
	}

    core_offset -= width * 0.5f;

    core_offset *= 5.f;
    fade_offset *= 5.f;
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeLinecap(StrokeSegmentData& stroke_data,
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

    glm::vec2 linecap_left, linecap_right, linecap_left_fade, linecap_right_fade;
    glm::vec2 linecap_center_point = *stroke_data.d_curPoint;

    //We offset the linecap points in case we want a squared cap
    if(linecap == SVGPaintStyle::SLC_SQUARE)
        linecap_center_point += stroke_data.d_strokeHalfWidth * linecap_dir;

    //We get the lincap points
    linecap_left = linecap_center_point + vec_to_outside;
    linecap_right = linecap_center_point - vec_to_outside;
    glm::vec2 linecap_left_AA, linecap_right_AA;

    //We determine our linecap points for anti-aliasing if required. Also we call the draw commands for squared and butt linecaps.
    if(render_settings.d_antiAliasing)
    {
        //We calculate the stretch factors in directions of our offsets and then offset the vertices
        float length_side_scale = calculateLengthScale(dir_to_outside, scale_factors);
        glm::vec2 lineside_offset_vec = length_side_scale * dir_to_outside;

        linecap_left_fade = linecap_left + stroke_data.d_antiAliasingOffsets.y * lineside_offset_vec;
        linecap_right_fade = linecap_right + stroke_data.d_antiAliasingOffsets.y * -lineside_offset_vec;
        linecap_left_AA = linecap_left + stroke_data.d_antiAliasingOffsets.x * lineside_offset_vec;
        linecap_right_AA = linecap_right + stroke_data.d_antiAliasingOffsets.x * -lineside_offset_vec;
    }

    if( linecap == SVGPaintStyle::SLC_BUTT || linecap == SVGPaintStyle::SLC_SQUARE )
    {
        if(render_settings.d_antiAliasing)
        {
            float length_cap_scale = calculateLengthScale(linecap_dir, scale_factors);
            glm::vec2 linecap_offset_vec = length_cap_scale * linecap_dir;
            linecap_left_fade += stroke_data.d_antiAliasingOffsets.y * linecap_offset_vec;
            linecap_right_fade += stroke_data.d_antiAliasingOffsets.y * linecap_offset_vec;
            linecap_left_AA += stroke_data.d_antiAliasingOffsets.x * linecap_offset_vec;
            linecap_right_AA += stroke_data.d_antiAliasingOffsets.x * linecap_offset_vec;

            //Create the outer AA quad of the butt or square linecap
            addStrokeLinecapAAGeometryVertices(stroke_data, linecap_left_AA, linecap_right_AA, linecap_left_fade, linecap_right_fade);
            //Add the anti-aliased connection points to the stroke data
            setStrokeDataLastPointsAA(stroke_data, linecap_left_AA, linecap_right_AA, linecap_left_fade, linecap_right_fade);
        }
        else
            //Add the connection points to the stroke data
            setStrokeDataLastPoints(stroke_data, linecap_left, linecap_right);
    }

    //In case we got rounded linecaps we want to determine our points first and draw then
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
        createArcPoints(linecap_center_point, linecap_left, linecap_right, num_segments,
                        is_start ? tangential_factor : -tangential_factor, radial_factor, arc_points);
        
        if(!render_settings.d_antiAliasing)
        {
            //Calculate the arc points
            createArcStrokeGeometry(arc_points, stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex);
            
            setStrokeDataLastPoints(stroke_data, linecap_left, linecap_right);
        }
        else
        {
            //Calculate the anti-aliased arc points
            createArcStrokeAAGeometry(arc_points, linecap_center_point, linecap_center_point, stroke_data, scale_factors,
                                      is_start, linecap_left_AA, linecap_right_AA, linecap_left_fade, linecap_right_fade);

            setStrokeDataLastPointsAAWithCenter(stroke_data, linecap_left_AA, linecap_right_AA, linecap_left_fade,
                                                linecap_right_fade, linecap_center_point);
        }
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::calculateAAMiterAndSetConnectionPoints(StrokeSegmentData& stroke_data,
                                                             const glm::vec2& segment_end_left_orig,
                                                             const glm::vec2& segment_end_right_orig,
                                                             const bool polygon_is_clockwise,
                                                             const glm::vec2& prev_to_cur,
                                                             const glm::vec2& cur_to_next,
                                                             const glm::vec2& prev_dir_to_inside,
                                                             const glm::vec2& next_dir_to_inside,
                                                             const glm::vec2& scale_factors)
{
    glm::vec2 vec_to_corner = calculateScaledCombinedVector(scale_factors, prev_dir_to_inside, next_dir_to_inside,
                                                            prev_to_cur, cur_to_next, polygon_is_clockwise);

    // Calculate the offset vectors from the original area
    glm::vec2 core_offset_vec = stroke_data.d_antiAliasingOffsets.x * vec_to_corner;
    glm::vec2 fade_offset_vec = stroke_data.d_antiAliasingOffsets.y * vec_to_corner;
    // Calculate the segment positions
    const glm::vec2 segmentFadeLeftEnd = segment_end_left_orig + fade_offset_vec;
    const glm::vec2 segmentLeftEnd = segment_end_left_orig + core_offset_vec;
    const glm::vec2 segmentRightEnd = segment_end_right_orig - core_offset_vec;
    const glm::vec2 segmentFadeRightEnd = segment_end_right_orig - fade_offset_vec;

    // If we want to draw we have to combine the vertices
    setStrokeDataCurrentPointsAA(stroke_data, segmentLeftEnd, segmentRightEnd, segmentFadeLeftEnd, segmentFadeRightEnd);
    setStrokeDataSubsequentPointsAA(stroke_data, segmentLeftEnd, segmentRightEnd, segmentFadeLeftEnd, segmentFadeRightEnd);
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
void SVGTesselator::createArcStrokeAAGeometry(const std::vector<glm::vec2>& points,
                                              const glm::vec2& arc_center_point,
                                              const glm::vec2& arc_draw_origin_point,
                                              StrokeSegmentData& stroke_data,
                                              const glm::vec2& scale_factors,
                                              const bool polygon_is_clockwise,
                                              glm::vec2& linecap_left_AA,
                                              glm::vec2& linecap_right_AA,
                                              glm::vec2& linecap_left_fade,
                                              glm::vec2& linecap_right_fade)
{
    if(points.empty())
        return;

    glm::vec2 previous_normal_point, previous_fade_point;

    //Draw all arc parts
    const size_t index_limit = points.size();
    for(size_t i = 0; i < index_limit; ++i)
    {
        bool is_first = (i == 0);
        bool is_last = (i == index_limit-1);

        //Calculate the scale vector for our offset
        glm::vec2 vec_to_outside = glm::normalize(points[i] - arc_center_point);
        float lengthScale = calculateLengthScale(vec_to_outside, scale_factors);
        vec_to_outside *= lengthScale;

        // Calculate the positions
        glm::vec2 current_normal_point = points[i] + vec_to_outside * stroke_data.d_antiAliasingOffsets.x; 
        glm::vec2 current_fade_point = points[i] + vec_to_outside * stroke_data.d_antiAliasingOffsets.y; 


        //Once we determined the first AA points we need to set the references to them
        if(is_first)
        {
            linecap_left_AA = current_normal_point;
            linecap_left_fade = current_fade_point;
        }
        else
        {
            addStrokeAAQuad(stroke_data, current_normal_point, previous_normal_point,
                current_fade_point, previous_fade_point);

            addTriangleGeometry(current_normal_point, previous_normal_point, arc_draw_origin_point,
                stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex);
        }

        if(is_last)
        {
            linecap_right_AA = current_normal_point;
            linecap_right_fade = current_fade_point;
        }

        previous_normal_point = current_normal_point;
        previous_fade_point = current_fade_point;
    }
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
    //Add the start points of the arc to our list and set the current position to be the 
    arc_points.push_back(start_point);
    // arc's start point in object coordinates
    glm::vec2 current_pos( start_point - center_point );

    //Calculate the arc points, skip last segment because that will be our endpoint
    for(int i = 0; i < num_segments - 1.0f; i++) 
    { 
		glm::vec2 temp( -current_pos.y, current_pos.x );

		current_pos += temp * tangential_factor;
		current_pos *= radial_factor;

        arc_points.push_back( center_point + current_pos );
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
                                                     const glm::vec2& prev_dir_to_inside,
                                                     const glm::vec2& next_dir_to_inside,
                                                     const glm::vec2& segment_end_left,
                                                     const glm::vec2& segment_end_right,
                                                     const glm::vec2& second_bevel_point,
                                                     const SVGPaintStyle::SVGLinejoin linejoin,
                                                     const bool polygon_is_clockwise,
                                                     const bool draw)
{
    if(draw)
    {
        setStrokeDataCurrentPoints(stroke_data, segment_end_left, segment_end_right);


        if(linejoin == SVGPaintStyle::SLJ_BEVEL)
        {
            //Simply add a triangle for the bevel
            addTriangleGeometry(segment_end_left, segment_end_right, second_bevel_point, stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex);
        }
        else if(linejoin == SVGPaintStyle::SLJ_ROUND)
        {
            //Determine the linejoin angle
            float arc_angle = std::acos( glm::dot(-prev_dir_to_inside, -next_dir_to_inside) );

            //Get the parameters
            float num_segments, tangential_factor, radial_factor;
            calculateArcTesselationParameters(stroke_data.d_strokeHalfWidth, arc_angle, stroke_data.d_maxScale,
                                              num_segments, tangential_factor, radial_factor);

            //Get the arc points and add them to the geometrybuffer
            std::vector<glm::vec2> arc_points;
            arc_points.push_back(polygon_is_clockwise ? segment_end_right : segment_end_left);

            createArcPoints(cur_point, polygon_is_clockwise ? second_bevel_point : segment_end_right,
                            polygon_is_clockwise ? segment_end_left : second_bevel_point, num_segments,
                            tangential_factor, radial_factor, arc_points);

            createArcStrokeGeometry(arc_points, stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex);
        }
    }

    setStrokeDataSubsequentPoints(stroke_data, polygon_is_clockwise ? second_bevel_point : segment_end_left, polygon_is_clockwise ? segment_end_right : second_bevel_point);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeLinejoinBevelOrRoundAA(StrokeSegmentData &stroke_data,
                                                       const SVGImage::SVGImageRenderSettings& render_settings,
                                                       const glm::vec2& scale_factors,
                                                       const glm::vec2& cur_point,
                                                       const glm::vec2& second_bevel_point,
                                                       const glm::vec2& segment_end_left,
                                                       const glm::vec2& segment_end_right,
                                                       const glm::vec2& prev_to_cur,
                                                       const glm::vec2& cur_to_next,
                                                       const glm::vec2& prev_dir_to_inside,
                                                       const glm::vec2& next_dir_to_inside,
                                                       const SVGPaintStyle::SVGLinejoin linejoin,
                                                       const bool polygon_is_clockwise,
                                                       const bool draw)
{
    CEGUI::ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    const glm::vec2& inner_point = polygon_is_clockwise ? segment_end_right : segment_end_left;
    const glm::vec2& outer_point = polygon_is_clockwise ? segment_end_left : segment_end_right;


    // Get the scaled vector for the inner AA points
    glm::vec2 inner_scaled_vec = calculateScaledCombinedVector(scale_factors, prev_dir_to_inside, next_dir_to_inside,
                                                               prev_to_cur, cur_to_next, false);
    // Calculate the offset vectors for the inner points from the original point
    glm::vec2 core_offset_vec_inner = stroke_data.d_antiAliasingOffsets.x * inner_scaled_vec;
    glm::vec2 fade_offset_vec_inner = stroke_data.d_antiAliasingOffsets.y * inner_scaled_vec;
    // Calculate the inner positions of our bevel
    const glm::vec2 inner_AA = inner_point + core_offset_vec_inner;
    const glm::vec2 inner_fade_AA = inner_point + fade_offset_vec_inner;

    // Get the dir to the edge between the two outer corner points and its orthogonal direction
    glm::vec2 edge_dir = glm::normalize(second_bevel_point - outer_point);
    glm::vec2 edge_perpendicular_dir( polygon_is_clockwise ? edge_dir.y : -edge_dir.y,
        polygon_is_clockwise ? -edge_dir.x : edge_dir.x);

    // Get the scaled-widths of the perpendicular directions of the edges
    float length_scale1 = calculateLengthScale(prev_dir_to_inside, scale_factors);
    float length_scale2 = calculateLengthScale(edge_perpendicular_dir, scale_factors);
    float length_scale3 = calculateLengthScale(next_dir_to_inside, scale_factors);

    // Calculate scale-distorted direction
    glm::vec2 fadecorner_vec1 = prev_to_cur * length_scale2 - edge_dir * length_scale1;
    glm::vec2 fadecorner_vec2 = edge_dir * length_scale3 - cur_to_next * length_scale2;          

    //We do not need to normalize at this point, as our result after the dot product division gives us the same result in either case
    //Apply dot product scales
    fadecorner_vec1 /= glm::dot(-prev_dir_to_inside, fadecorner_vec1);
    fadecorner_vec2 /= glm::dot(-edge_perpendicular_dir, fadecorner_vec2);
    //Apply length factors
    fadecorner_vec1 *= length_scale1;
    fadecorner_vec2 *= length_scale2;

    // Calculate the segment positions
    glm::vec2 outer_AA = outer_point + stroke_data.d_antiAliasingOffsets.x * fadecorner_vec1;
    glm::vec2 outer_fade_AA = outer_point + stroke_data.d_antiAliasingOffsets.y * fadecorner_vec1;
    glm::vec2 outer2_AA = second_bevel_point + stroke_data.d_antiAliasingOffsets.x * fadecorner_vec2;
    glm::vec2 outer2_fade_AA = second_bevel_point + stroke_data.d_antiAliasingOffsets.y * fadecorner_vec2;

    //The lines to the points can overlap in case the vectors point to different directions, which happens in extreme scale cases.
    // Normally the 2 points can be merged into 1 point here, so we need to consider this case
    bool could_vectors_overlap = !isVectorLeftOfOtherVector(polygon_is_clockwise ? fadecorner_vec1 : fadecorner_vec2,
                                                            polygon_is_clockwise ? fadecorner_vec2 : fadecorner_vec1);
    bool are_lines_overlapping = false;
    if(could_vectors_overlap)
        are_lines_overlapping = ( intersect(outer_point, outer_fade_AA, second_bevel_point, outer2_fade_AA, glm::vec2()) == INTERESECTING );

   
    if(are_lines_overlapping)
    {
        //In case of an overlap we fall back to just using single vertex, similar to the miter linejoin
        glm::vec2 outer_point_miter = cur_point + cur_point - inner_point;

        // Calculate the corrected outer positions
        const glm::vec2 outer_AA_corrected = outer_point_miter - core_offset_vec_inner;
        const glm::vec2 outer_AA_fade_corrected = outer_point_miter - fade_offset_vec_inner;

        // Set the connection
        if(polygon_is_clockwise)
        {
            setStrokeDataCurrentPointsAA(stroke_data, outer_AA_corrected, inner_AA,
                                         outer_AA_fade_corrected, inner_fade_AA);
            setStrokeDataSubsequentPointsAA(stroke_data, outer_AA_corrected, inner_AA,
                                            outer_AA_fade_corrected, inner_fade_AA);
        }
        else
        {
            setStrokeDataCurrentPointsAA(stroke_data, inner_AA, outer_AA_corrected,
                                         inner_fade_AA, outer_AA_fade_corrected);
            setStrokeDataSubsequentPointsAA(stroke_data, inner_AA, outer_AA_corrected,
                                inner_fade_AA, outer_AA_fade_corrected);
        }
    }
    else 
    { 
        if(draw && linejoin == SVGPaintStyle::SLJ_BEVEL)
        {
            //Add the geometry for bevel
            addTriangleGeometry(outer2_AA, inner_AA, outer_AA,
                                geometry_buffer, stroke_vertex);

            addStrokeAAQuad(stroke_data, outer_AA, outer2_AA, outer_fade_AA, outer2_fade_AA);
        }
        else if(draw && linejoin == SVGPaintStyle::SLJ_ROUND)
        {
            //Add the geometry for rounded linejoin
            float arc_angle = std::acos( glm::dot(-prev_dir_to_inside, -next_dir_to_inside) );

            //Get the parameters
            float num_segments, tangential_factor, radial_factor;
            calculateArcTesselationParameters(stroke_data.d_strokeHalfWidth, arc_angle, stroke_data.d_maxScale,
                                              num_segments, tangential_factor, radial_factor);

            //Get the arc points
            std::vector<glm::vec2> arc_points;
            createArcPoints(cur_point, outer_AA, outer2_AA, num_segments,
                            polygon_is_clockwise ? -tangential_factor : tangential_factor, radial_factor, arc_points);

            createArcStrokeAAGeometry(arc_points, cur_point, inner_AA, stroke_data, scale_factors,
                                      !polygon_is_clockwise, outer_AA, outer2_AA, outer_fade_AA, outer2_fade_AA);
        }


        // We add the geometry of the segment that connects to the last linecap/linejoin
        if(polygon_is_clockwise)
        {
            setStrokeDataCurrentPointsAA(stroke_data, outer_AA, inner_AA, outer_fade_AA, inner_fade_AA);
            setStrokeDataSubsequentPointsAA(stroke_data, outer2_AA, inner_AA, outer2_fade_AA, inner_fade_AA);
        }
        else
        {
            setStrokeDataCurrentPointsAA(stroke_data, inner_AA, outer_AA, inner_fade_AA, outer_fade_AA);
            setStrokeDataSubsequentPointsAA(stroke_data, inner_AA, outer2_AA, inner_fade_AA, outer2_fade_AA);
        }
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
glm::vec2 SVGTesselator::determineScaleFactors(const glm::mat3& transformation, const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::vec2 scale ( glm::length(glm::vec3(transformation[0].x, transformation[1].x, transformation[2].x)), 
                      glm::length(glm::vec3(transformation[0].y, transformation[1].y, transformation[2].y)) );
    scale *= glm::vec2( render_settings.d_scaleFactor.d_x, render_settings.d_scaleFactor.d_y );
    scale = 1.f / scale;

    return scale;
}


//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegmentConnection(StrokeSegmentData& stroke_data)
{
    // Add the geometry
    CEGUI::ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
}


//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegmentConnectionAA(StrokeSegmentData &stroke_data)
{ 
    CEGUI::ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    CEGUI::ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    //Fade1
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentFadePointLeft;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointLeft;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointLeft;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    //Core
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    //Fade1
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentFadePointRight;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentFadePointRight;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointRight;
    geometry_buffer.appendVertex(stroke_fade_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegmentAAConnectionWithCenter(StrokeSegmentData &stroke_data)
{ 
    CEGUI::ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    CEGUI::ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    //Fade1
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentFadePointLeft;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointLeft;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointLeft;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    //Core to center points #1
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentCenterPoint;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentCenterPoint;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointLeft;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastCenterPoint;
    geometry_buffer.appendVertex(stroke_vertex);
    //Core to center points #2
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentCenterPoint;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastCenterPoint;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastCenterPoint;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    //Fade1
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentFadePointRight;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_currentFadePointRight;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastPointRight;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.swizzle(glm::X, glm::Y) = stroke_data.d_lastFadePointRight;
    geometry_buffer.appendVertex(stroke_fade_vertex);
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

    float angle_to_x_axis = std::acos( std::abs(direction.x) );
    float angle_to_y_axis = half_pi - angle_to_x_axis;
    return one_over_pi_half * (scale_factors.x * angle_to_y_axis + scale_factors.y * angle_to_x_axis);
}

//----------------------------------------------------------------------------//
void SVGTesselator::addStrokeAAQuad(StrokeSegmentData &stroke_data,
                                    const glm::vec2& point1,
                                    const glm::vec2& point2,
                                    const glm::vec2& fade_point1,
                                    const glm::vec2& fade_point2)
{
    CEGUI::ColouredVertex& vertex = stroke_data.d_strokeVertex;
    CEGUI::ColouredVertex& fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    fade_vertex.d_position.swizzle(glm::X, glm::Y) = fade_point1;
    geometry_buffer.appendVertex(fade_vertex);

    fade_vertex.d_position.swizzle(glm::X, glm::Y) = fade_point2;
    geometry_buffer.appendVertex(fade_vertex);

    vertex.d_position.swizzle(glm::X, glm::Y) = point1;
    geometry_buffer.appendVertex(vertex);

    vertex.d_position.swizzle(glm::X, glm::Y) = point1;
    geometry_buffer.appendVertex(vertex);

    fade_vertex.d_position.swizzle(glm::X, glm::Y) = fade_point2;
    geometry_buffer.appendVertex(fade_vertex);

    vertex.d_position.swizzle(glm::X, glm::Y) = point2;
    geometry_buffer.appendVertex(vertex);
}

//----------------------------------------------------------------------------//
glm::vec2 SVGTesselator::calculateScaledCombinedVector(const glm::vec2& scale_factors,
                                                       const glm::vec2& prev_dir_to_inside,
                                                       const glm::vec2& next_dir_to_inside,
                                                       const glm::vec2& prev_to_cur,
                                                       const glm::vec2& cur_to_next,
                                                       const bool polygon_is_clockwise)
{
    // Get the scaled-widths of the incoming and outgoing line segments
    float length_scale1 = calculateLengthScale(prev_dir_to_inside, scale_factors);
    float length_scale2 = calculateLengthScale(next_dir_to_inside, scale_factors);

    glm::vec2 vec_to_corner = glm::normalize(prev_to_cur * length_scale2 - cur_to_next * length_scale1);

    // Calculate scale-distorted direction
    // glm::vec2 vec_to_corner = glm::normalize(prev_to_cur * length_scale2 - cur_to_next * length_scale1);

    // Calculate how much we need to offset along the direction depending on the angle
    /* We get the distance to our new corner using a factor. The factor would, in case we didn't prepare for non-uniform scaling, just consist
    of a simple vector projection. However, here we also need to multiply the local stroke width's scale factor (in the direction of the stroke)
    to get currect results. We have two alternative calculations available for this with the same results.
    */
    float length_to_corner = length_scale1 / glm::dot( (polygon_is_clockwise ? -prev_dir_to_inside : prev_dir_to_inside) , vec_to_corner );
    // Alternative version:  length_scale2 / glm::dot( (polygon_is_clockwise ? -next_dir_to_inside : next_dir_to_inside) , vec_to_corner );

    return vec_to_corner * length_to_corner;
}

//----------------------------------------------------------------------------//
bool SVGTesselator::isVectorLeftOfOtherVector(const glm::vec2& vector_left,
                                              const glm::vec2& vector_right)
{
    float dot_result = (vector_left.x * -vector_right.y) + (vector_left.y * vector_right.x);
    return dot_result > 0.0f;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataLastPoints(StrokeSegmentData &stroke_data,
                                            const glm::vec2& last_point_left,
                                            const glm::vec2& last_point_right)
{
    //We set our lastPoint values
    stroke_data.d_lastPointLeft = last_point_left;
    stroke_data.d_lastPointRight = last_point_right;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataLastPointsAA(StrokeSegmentData &stroke_data,
                                                 const glm::vec2& last_point_left,
                                                 const glm::vec2& last_point_right,
                                                 const glm::vec2& last_point_left_fade,
                                                 const glm::vec2& last_point_right_fade)
{
    // We set our lastPoint values
    stroke_data.d_lastPointLeft = last_point_left;
    stroke_data.d_lastPointRight = last_point_right;
    stroke_data.d_lastFadePointLeft = last_point_left_fade;
    stroke_data.d_lastFadePointRight = last_point_right_fade;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataLastPointsAAWithCenter(StrokeSegmentData &stroke_data,
                                                           const glm::vec2& last_point_left,
                                                           const glm::vec2& last_point_right,
                                                           const glm::vec2& last_point_left_fade,
                                                           const glm::vec2& last_point_right_fade,
                                                           const glm::vec2& last_center_point)
{
    // We set our lastPoint values
    stroke_data.d_lastPointLeft = last_point_left;
    stroke_data.d_lastPointRight = last_point_right;
    stroke_data.d_lastFadePointLeft = last_point_left_fade;
    stroke_data.d_lastFadePointRight = last_point_right_fade;
    stroke_data.d_lastCenterPoint = last_center_point;
}


//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataCurrentPoints(StrokeSegmentData &stroke_data,
                                               const glm::vec2& current_point_left,
                                               const glm::vec2& current_point_right)
{
    //We set our currentPoint values
    stroke_data.d_currentPointLeft = current_point_left;
    stroke_data.d_currentPointRight = current_point_right;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataCurrentPointsAA(StrokeSegmentData &stroke_data,
                                                 const glm::vec2& current_point_left,
                                                 const glm::vec2& current_point_right,
                                                 const glm::vec2& current_point_left_fade,
                                                 const glm::vec2& current_point_right_fade)
{
    // We set our currentPoint values
    stroke_data.d_currentPointLeft = current_point_left;
    stroke_data.d_currentPointRight = current_point_right;
    stroke_data.d_currentFadePointLeft = current_point_left_fade;
    stroke_data.d_currentFadePointRight = current_point_right_fade;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataCurrentPointsAAWithCenter(StrokeSegmentData &stroke_data,
                                                           const glm::vec2& current_point_left,
                                                           const glm::vec2& current_point_right,
                                                           const glm::vec2& current_point_left_fade,
                                                           const glm::vec2& current_point_right_fade,
                                                           const glm::vec2& current_center_point)
{
    // We set our currentPoint values
    stroke_data.d_currentPointLeft = current_point_left;
    stroke_data.d_currentPointRight = current_point_right;
    stroke_data.d_currentFadePointLeft = current_point_left_fade;
    stroke_data.d_currentFadePointRight = current_point_right_fade;
    stroke_data.d_currentCenterPoint = current_center_point;
}


//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataSubsequentPoints(StrokeSegmentData &stroke_data,
                                                  const glm::vec2& subsequent_point_left,
                                                  const glm::vec2& subsequent_point_right)
{
    //We set our subsequentPoint values
    stroke_data.d_subsequentPointLeft = subsequent_point_left;
    stroke_data.d_subsequentPointRight = subsequent_point_right;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataSubsequentPointsAA(StrokeSegmentData &stroke_data,
                                                 const glm::vec2& subsequent_point_left,
                                                 const glm::vec2& subsequent_point_right,
                                                 const glm::vec2& subsequent_point_left_fade,
                                                 const glm::vec2& subsequent_point_right_fade)
{
    // We set our subsequentPoint values
    stroke_data.d_subsequentPointLeft = subsequent_point_left;
    stroke_data.d_subsequentPointRight = subsequent_point_right;
    stroke_data.d_subsequentFadePointLeft = subsequent_point_left_fade;
    stroke_data.d_subsequentFadePointRight = subsequent_point_right_fade;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataSubsequentPointsAAWithCenter(StrokeSegmentData &stroke_data,
                                                           const glm::vec2& subsequent_point_left,
                                                           const glm::vec2& subsequent_point_right,
                                                           const glm::vec2& subsequent_point_left_fade,
                                                           const glm::vec2& subsequent_point_right_fade,
                                                           const glm::vec2& subsequent_center_point)
{
    // We set our subsequentPoint values
    stroke_data.d_subsequentPointLeft = subsequent_point_left;
    stroke_data.d_subsequentPointRight = subsequent_point_right;
    stroke_data.d_subsequentFadePointLeft = subsequent_point_left_fade;
    stroke_data.d_subsequentFadePointRight = subsequent_point_right_fade;
    stroke_data.d_subsequentCenterPoint = subsequent_center_point;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataSubsequentPointsAsLastPoints(StrokeSegmentData &stroke_data)
{
    stroke_data.d_lastPointLeft = stroke_data.d_subsequentPointLeft;
    stroke_data.d_lastPointRight = stroke_data.d_subsequentPointRight;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataSubsequentPointsAsLastPointsAA(StrokeSegmentData &stroke_data)
{
    stroke_data.d_lastPointLeft = stroke_data.d_subsequentPointLeft;
    stroke_data.d_lastPointRight = stroke_data.d_subsequentPointRight;
    stroke_data.d_lastFadePointLeft = stroke_data.d_subsequentFadePointLeft;
    stroke_data.d_lastFadePointRight = stroke_data.d_subsequentFadePointRight;
}


//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataLastPointsAsCurrentPoints(StrokeSegmentData &stroke_data)
{
    stroke_data.d_currentPointLeft = stroke_data.d_lastPointLeft;
    stroke_data.d_currentPointRight = stroke_data.d_lastPointRight;
}

//----------------------------------------------------------------------------//
void SVGTesselator::setStrokeDataLastPointsAsCurrentPointsAA(StrokeSegmentData &stroke_data)
{
    stroke_data.d_currentPointLeft = stroke_data.d_lastPointLeft;
    stroke_data.d_currentPointRight = stroke_data.d_lastPointRight;
    stroke_data.d_currentFadePointLeft = stroke_data.d_lastFadePointLeft;
    stroke_data.d_currentFadePointRight = stroke_data.d_lastFadePointRight;
}



//----------------------------------------------------------------------------//
}

