/***********************************************************************
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
#include "CEGUI/Renderer.h"
#include "CEGUI/Vertex.h"
#include <glm/glm.hpp>
#if (GLM_VERSION_MAJOR == 0 && GLM_VERSION_MINOR <= 9 && GLM_VERSION_PATCH <= 3)
    #include <glm/gtx/constants.hpp>
#else
    #include <glm/gtc/constants.hpp>
#endif

#include <cmath>


// Start of CEGUI namespace section
namespace CEGUI
{
//Internal numeric value for  circle roundness. The lower, the better tesselated the
//circle will be. We will set it to an, for our needs, appropriate fixed value.
const float CircleRoundnessValue = 0.8f;

//----------------------------------------------------------------------------//
SVGTesselator::StrokeSegmentData::StrokeSegmentData(GeometryBuffer& geometry_buffer,
                                                    const float stroke_half_width,
                                                    const SVGPaintStyle& paint_style,
                                                    const float max_scale) :
    d_strokeHalfWidth(stroke_half_width),
    d_geometryBuffer(geometry_buffer),
    d_paintStyle(paint_style),
    d_maxScale(max_scale),
    d_prevPoint(nullptr),
    d_curPoint(nullptr),
    d_nextPoint(nullptr)
{
    // Get and add the stroke colour
    d_strokeVertex.d_colour = SVGTesselator::getStrokeColour(paint_style);
    // Set the z coordinate
    d_strokeVertex.d_position.z = 0.0f;
    //Create the fade stroke colour from the normal colour and set the alpha to 0
    d_strokeFadeVertex = d_strokeVertex;
    d_strokeFadeVertex.d_colour.w = 0.0f;
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
void SVGTesselator::tesselateRect(std::vector<GeometryBuffer*>& out,
    const SVGRect* rect,
    const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::mat3x3 transformation = glm::mat3(
        1.0f, 0.0f, rect->d_x,
        0.0f, 1.0f, rect->d_y,
        0.0f, 0.0f, 1.0f) * rect->d_transformation;

    //Setup the required Geometrybuffers
    GeometryBuffer* fill_geometry_buffer;
    GeometryBuffer* stroke_geometry_buffer;

    setupGeometryBuffers(out, fill_geometry_buffer, stroke_geometry_buffer,
        render_settings, transformation, false);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = rect->d_paintStyle;

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = determineScaleFactors(rect->d_transformation, render_settings);

    //Make a list of rectangle (corner) points
    std::vector<glm::vec2> rectangle_points;
    rectangle_points.push_back(glm::vec2(0.0f, 0.0f));
    rectangle_points.push_back(glm::vec2(0.0f, rect->d_height));
    rectangle_points.push_back(glm::vec2(rect->d_width, rect->d_height));
    rectangle_points.push_back(glm::vec2(rect->d_width, 0.0f));

    //Create and append the rectangle's fill geometry
    createTriangleStripFillGeometry(rectangle_points, *fill_geometry_buffer, paint_style);

    //Create and append the rectangle's stroke geometry
    createStroke(rectangle_points, *stroke_geometry_buffer,
        paint_style, render_settings, scale_factors, true);
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateCircle(std::vector<GeometryBuffer*>& out,
    const SVGCircle* circle,
    const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::mat3x3 transformation = glm::mat3(
        1.0f, 0.0f, circle->d_cx,
        0.0f, 1.0f, circle->d_cy,
        0.0f, 0.0f, 1.0f) * circle->d_transformation;

    //Setup the required Geometrybuffers
    GeometryBuffer* fill_geometry_buffer;
    GeometryBuffer* stroke_geometry_buffer;

    setupGeometryBuffers(out,
        fill_geometry_buffer, stroke_geometry_buffer,
        render_settings, transformation, false);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = circle->d_paintStyle;

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = determineScaleFactors(circle->d_transformation, render_settings);

    //We need this to determine the degree of tesselation required for the curved elements
    float max_scale = std::max(render_settings.d_scaleFactor.x, render_settings.d_scaleFactor.y);

    //Get the radius
    const float& radius = circle->d_r;

    //Precalculate values needed for the circle tesselation
    float num_segments, cos_value, sin_value;
    calculateCircleTesselationParameters(radius, max_scale, num_segments, cos_value, sin_value);

    //Create circle points
    std::vector<glm::vec2> circle_points;
    createCirclePoints(radius, num_segments, cos_value, sin_value, circle_points);

    if (circle_points.size() > 2)
    {
        //Create and append the circle's fill geometry
        createCircleFill(circle_points, max_scale, paint_style, *fill_geometry_buffer, render_settings, scale_factors);
        //Create and append the circle's stroke geometry
        createCircleStroke(circle_points, max_scale, paint_style, *stroke_geometry_buffer, render_settings, scale_factors);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateEllipse(
    std::vector<GeometryBuffer*>& out,
    const SVGEllipse* ellipse,
    const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::mat3x3 transformation = glm::mat3(
        1.0f, 0.0f, ellipse->d_cx,
        0.0f, 1.0f, ellipse->d_cy,
        0.0f, 0.0f, 1.0f) * ellipse->d_transformation;

    //Setup the required GeometryBuffers
    GeometryBuffer* fill_geometry_buffer;
    GeometryBuffer* stroke_geometry_buffer;

    setupGeometryBuffers(out,
        fill_geometry_buffer, stroke_geometry_buffer,
        render_settings, transformation, false);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = ellipse->d_paintStyle;

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = determineScaleFactors(ellipse->d_transformation, render_settings);

    //We need this to determine the degree of tesselation required for the curved elements
    float max_scale = std::max(render_settings.d_scaleFactor.x, render_settings.d_scaleFactor.y);

    //Get the radii
    const float& radiusX = ellipse->d_rx;
    const float& radiusY = ellipse->d_ry;

    //Create ellipse points
    std::vector<glm::vec2> ellipse_points;
    createEllipsePoints(radiusX, radiusY, max_scale, ellipse_points);

    if (ellipse_points.size() > 2)
    {
        //Create and append the ellipse's fill geometry
        createEllipseFill(ellipse_points, max_scale, paint_style, *fill_geometry_buffer, render_settings, scale_factors);
        //Create and append the ellipse's stroke geometry
        createEllipseStroke(ellipse_points, max_scale, paint_style, *stroke_geometry_buffer, render_settings, scale_factors);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateLine(
    std::vector<GeometryBuffer*>& out,
    const SVGLine* line,
    const SVGImage::SVGImageRenderSettings& render_settings)
{
    //Setup the required Geometrybuffers
    //TODO: This is the only case where we have no fill geometry, need to refactor code and remove.
    GeometryBuffer* fill_geometry_buffer;
    GeometryBuffer* stroke_geometry_buffer;

    setupGeometryBuffers(out,
        fill_geometry_buffer, stroke_geometry_buffer,
        render_settings, line->d_transformation, false);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = line->d_paintStyle;

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = determineScaleFactors(line->d_transformation, render_settings);

    //Create the line points and add them to the stroke points list
    std::vector<glm::vec2> points;
    points.push_back(glm::vec2(line->d_x1, line->d_y1));
    points.push_back(glm::vec2(line->d_x2, line->d_y2));

    //Create and append the polyline's stroke geometry
    createStroke(points, *stroke_geometry_buffer, paint_style, render_settings, scale_factors, false);
}


//----------------------------------------------------------------------------//
void SVGTesselator::tesselatePolyline(
    std::vector<GeometryBuffer*>& out,
    const SVGPolyline* polyline,
    const SVGImage::SVGImageRenderSettings& render_settings)
{
    //Setup the required Geometrybuffers
    GeometryBuffer* fill_geometry_buffer;
    GeometryBuffer* stroke_geometry_buffer;

    setupGeometryBuffers(out,
        fill_geometry_buffer, stroke_geometry_buffer,
        render_settings, polyline->d_transformation, false);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = polyline->d_paintStyle;

    //Getting the points defining the polyline
    const std::vector<glm::vec2>& points = polyline->d_points;

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = determineScaleFactors(polyline->d_transformation, render_settings);

    //Create and append the polyline's fill geometry
    createFill(points, *fill_geometry_buffer, paint_style, render_settings, scale_factors);

    //Create and append the polyline's stroke geometry
    createStroke(points, *stroke_geometry_buffer, paint_style, render_settings, scale_factors, false);
}

//----------------------------------------------------------------------------//
void SVGTesselator::tesselatePolygon(
    std::vector<GeometryBuffer*>& out,
    const SVGPolygon* polygon,
    const SVGImage::SVGImageRenderSettings& render_settings)
{
    //Setup the required Geometrybuffers
    GeometryBuffer* fill_geometry_buffer;
    GeometryBuffer* stroke_geometry_buffer;

    setupGeometryBuffers(out,
        fill_geometry_buffer, stroke_geometry_buffer,
        render_settings, polygon->d_transformation, false);

    //The shape's paint styles
    const SVGPaintStyle& paint_style = polygon->d_paintStyle;

    //Getting the points defining the polyline
    const std::vector<glm::vec2>& points = polygon->d_points;

    //Get the final scale by extracting the scale from the matrix and combining it with the image scale
    glm::vec2 scale_factors = determineScaleFactors(polygon->d_transformation, render_settings);

    //Create and append the polyline's fill geometry
    createFill(points, *fill_geometry_buffer, paint_style, render_settings, scale_factors);

    //Create and append the polyline's stroke geometry
    createStroke(points, *stroke_geometry_buffer, paint_style, render_settings, scale_factors, true);
}

//----------------------------------------------------------------------------//
void SVGTesselator::setupGeometryBuffers(
    std::vector<GeometryBuffer*>& out,
    GeometryBuffer*& fill_geometry_buffer,
    GeometryBuffer*& stroke_geometry_buffer,
    const SVGImage::SVGImageRenderSettings& render_settings,
    const glm::mat3x3& svg_transformation,
    const bool is_fill_needing_stencil)
{
    //Calculate the transformation matrix for the CEGUI rendering system based on the SVG transformation matrix
    glm::mat4 cegui_transformation_matrix = createRenderableMatrixFromSVGMatrix(svg_transformation);

    fill_geometry_buffer = &System::getSingleton().getRenderer()->createGeometryBufferColoured();

    setupGeometryBufferSettings(fill_geometry_buffer, render_settings, cegui_transformation_matrix);
    out.push_back(fill_geometry_buffer);

    //TODO Ident: For gradients ability we will also need to perform a check for the DS_type here to see if we need a seperate buffer or not
    if(!is_fill_needing_stencil)
    {
        //We can use the GeometryBuffer of the fill also for the stroke
        stroke_geometry_buffer = fill_geometry_buffer;
    }
    else
    {
        stroke_geometry_buffer = &System::getSingleton().getRenderer()->createGeometryBufferColoured();

        setupGeometryBufferSettings(stroke_geometry_buffer, render_settings, cegui_transformation_matrix);
        out.push_back(stroke_geometry_buffer);
    }
}

//----------------------------------------------------------------------------//
glm::vec4 SVGTesselator::getFillColour(const SVGPaintStyle &paint_style)
{
    const glm::vec3& fill_colour_values = paint_style.d_fill.d_colour;
    return glm::vec4(fill_colour_values.x, fill_colour_values.y, fill_colour_values.z, paint_style.d_fillOpacity);
}

//----------------------------------------------------------------------------//
glm::vec4 SVGTesselator::getStrokeColour(const SVGPaintStyle &paint_style)
{
    const glm::vec3& stroke_colour_values = paint_style.d_stroke.d_colour;
    return glm::vec4(stroke_colour_values.x, stroke_colour_values.y, stroke_colour_values.z, paint_style.d_strokeOpacity);
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
SVGTesselator::LineIntersectResult SVGTesselator::intersectLines(const glm::vec2& line1_start, const glm::vec2& line1_end,
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
            return LineIntersectResult::Coincident;
        else
            return LineIntersectResult::Parallel;
    }

    float ua = nume_a / denom;
    float ub = nume_b / denom;

    if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
    {
        //Get the intersection point
        intersection.x = line1_start.x + ua*(line1_end.x - line1_start.x);
        intersection.y = line1_start.y + ua*(line1_end.y - line1_start.y);

        return LineIntersectResult::Intersecting;
    }

    return LineIntersectResult::NotIntersecting;
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
    float max_scale = std::max(render_settings.d_scaleFactor.x, render_settings.d_scaleFactor.y);

    // Create an object containing all the data we need for our segment processing
    StrokeSegmentData stroke_data(geometry_buffer, paint_style.d_strokeWidth * 0.5f, paint_style, max_scale);

    // If doing anti-aliasing, get the anti-aliasing offsets
    if(render_settings.d_antiAliasing)
        determineAntiAliasingOffsets(stroke_data.d_paintStyle.d_strokeWidth, stroke_data.d_antiAliasingOffsets);

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
    intersectLines(prev_point + prev_vec_to_inside, cur_point + prev_vec_to_inside,
                   next_point + next_vec_to_inside, cur_point + next_vec_to_inside,
                   inner_intersection);

    // The outer connection point of the stroke
    glm::vec2 outer_point;
    // Reference to the end-points of our stroke segment
    const glm::vec2& segment_end_left = polygon_is_clockwise ? outer_point : inner_intersection;
    const glm::vec2& segment_end_right = polygon_is_clockwise ? inner_intersection : outer_point;

    //If the stroke miter is exceeded we fall back to bevel
    if(linejoin == SVGPaintStyle::SVGLinejoin::Miter)
        handleStrokeMiterExceedance(stroke_data, cur_point, inner_intersection, linejoin);

    //Switch through the types and render them if required 
    if(linejoin == SVGPaintStyle::SVGLinejoin::Miter)
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
    else if(linejoin == SVGPaintStyle::SVGLinejoin::Bevel || linejoin == SVGPaintStyle::SVGLinejoin::Round)
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
void SVGTesselator::determineAntiAliasingOffsets
  (float /*width*/, glm::vec2& antialiasing_offsets)
{
    float& core_offset = antialiasing_offsets.x;
    float& fade_offset = antialiasing_offsets.y;

    core_offset = -0.5f;
    fade_offset = 0.5f;
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeLinecap(StrokeSegmentData& stroke_data,
                                        const SVGImage::SVGImageRenderSettings& render_settings,
                                        const glm::vec2& scale_factors,
                                        const bool is_start)
{
    const SVGPaintStyle::SvgLinecap& linecap = stroke_data.d_paintStyle.d_strokeLinecap;
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
    if(linecap == SVGPaintStyle::SvgLinecap::Square)
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

    if( linecap == SVGPaintStyle::SvgLinecap::Butt || linecap == SVGPaintStyle::SvgLinecap::Square )
    {
        if(render_settings.d_antiAliasing)
        {
            float length_cap_scale = calculateLengthScale(linecap_dir, scale_factors);
            glm::vec2 linecap_offset_vec = length_cap_scale * linecap_dir;
            linecap_left_AA += stroke_data.d_antiAliasingOffsets.x * linecap_offset_vec;
            linecap_right_AA += stroke_data.d_antiAliasingOffsets.x * linecap_offset_vec;
            linecap_left_fade += stroke_data.d_antiAliasingOffsets.y * linecap_offset_vec;
            linecap_right_fade += stroke_data.d_antiAliasingOffsets.y * linecap_offset_vec;

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
    if(linecap == SVGPaintStyle::SvgLinecap::Round)
    {
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

            setStrokeDataLastPointsAA(stroke_data, linecap_left_AA, linecap_right_AA,
                                      linecap_left_fade, linecap_right_fade);
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
    glm::vec2 vec_to_corner = calculateScaledCombinedVector(scale_factors, prev_to_cur, cur_to_next,
                                                            prev_dir_to_inside, next_dir_to_inside, polygon_is_clockwise);

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
void SVGTesselator::createTriangleStripFillGeometry(const std::vector<glm::vec2>& points,
                                                    GeometryBuffer& geometry_buffer,
                                                    const SVGPaintStyle& paint_style)
{
    if(points.size() < 3 || paint_style.d_fill.d_none)
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
void SVGTesselator::createCircleFill(const std::vector<glm::vec2>& circle_points,
                                     float /*max_scale*/,
                                     const SVGPaintStyle& paint_style,
                                     GeometryBuffer& geometry_buffer,
                                     const SVGImage::SVGImageRenderSettings& render_settings,
                                     const glm::vec2& scale_factors)
{
    if(paint_style.d_fill.d_none)
        return;

    //Append the geometry based on the the circle points
    if(!render_settings.d_antiAliasing)
        createTriangleStripFillGeometry(circle_points, geometry_buffer, paint_style);
    else
    {
        //We calculate the anti-aliasing offsets based on an arbitrary width
        glm::vec2 antiAliasingOffsets;
        determineAntiAliasingOffsets(10.0f, antiAliasingOffsets);

        std::vector<glm::vec2> circle_modified_points;
        std::vector<glm::vec2> circle_fade_points;
        createCircleOrEllipseFillPointsAA(circle_points, antiAliasingOffsets, scale_factors,
                                          circle_modified_points, circle_fade_points);

        createTriangleStripFillGeometry(circle_modified_points, geometry_buffer, paint_style);
        createFillGeometryAAFadeOnly(circle_modified_points, circle_fade_points, paint_style, geometry_buffer, true);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCircleStroke(const std::vector<glm::vec2>& circle_points,
                                       float max_scale,
                                       const SVGPaintStyle& paint_style,
                                       GeometryBuffer& geometry_buffer,
                                       const SVGImage::SVGImageRenderSettings& render_settings,
                                       const glm::vec2& scale_factors)
{
    if(paint_style.d_stroke.d_none || paint_style.d_strokeWidth == 0.0f)
    return;

    StrokeSegmentData stroke_data(geometry_buffer, paint_style.d_strokeWidth * 0.5f, paint_style, max_scale);

    if(!render_settings.d_antiAliasing)
    {
        //Calculate stroke points
        std::vector<glm::vec2> outer_circle_points;
        std::vector<glm::vec2> inner_circle_points;
        createCircleOrEllipseStrokePoints(circle_points, stroke_data, outer_circle_points, inner_circle_points);

        //Create the geometry from the points
        createStrokeGeometry(outer_circle_points, inner_circle_points, stroke_data, true);
    }
    else
    {
        determineAntiAliasingOffsets(stroke_data.d_paintStyle.d_strokeWidth, stroke_data.d_antiAliasingOffsets);

        //Calculate stroke points
        std::vector<glm::vec2> outer_circle_points;
        std::vector<glm::vec2> outer_circle_points_fade;
        std::vector<glm::vec2> inner_circle_points;
        std::vector<glm::vec2> inner_circle_points_fade;
        createCircleOrEllipseStrokePointsAA(circle_points, stroke_data, scale_factors, outer_circle_points,
                                            outer_circle_points_fade, inner_circle_points, inner_circle_points_fade);

        //Create the geometry from the points
        createStrokeGeometryAA(outer_circle_points, outer_circle_points_fade, inner_circle_points, inner_circle_points_fade,
                               stroke_data, true);
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
                                              const bool /*polygon_is_clockwise*/,
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
            addStrokeQuadAA(current_normal_point, previous_normal_point, current_fade_point, previous_fade_point,
                            stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex, stroke_data.d_strokeFadeVertex);

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
    for(int i = 0; i < num_segments - 1.0f; ++i) 
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
void SVGTesselator::createEllipseFill(const std::vector<glm::vec2>& ellipse_points,
                                      const float /*max_scale*/,
                                      const SVGPaintStyle& paint_style,
                                      GeometryBuffer& geometry_buffer,
                                      const SVGImage::SVGImageRenderSettings& render_settings,
                                      const glm::vec2& scale_factors)
{
    if(paint_style.d_fill.d_none)
        return;

    //Append the geometry based on the the ellipse points
    if(!render_settings.d_antiAliasing)
        createTriangleStripFillGeometry(ellipse_points, geometry_buffer, paint_style);
    else
    {
        //We calculate the anti-aliasing offsets based on an arbitrary width
        glm::vec2 antiAliasingOffsets;
        determineAntiAliasingOffsets(10.0f, antiAliasingOffsets);

        std::vector<glm::vec2> ellipse_modified_points;
        std::vector<glm::vec2> ellipse_fade_points;
        createCircleOrEllipseFillPointsAA(ellipse_points, antiAliasingOffsets, scale_factors,
                                          ellipse_modified_points, ellipse_fade_points);

        createTriangleStripFillGeometry(ellipse_modified_points, geometry_buffer, paint_style);
        createFillGeometryAAFadeOnly(ellipse_modified_points, ellipse_fade_points, paint_style, geometry_buffer, true);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createEllipseStroke(const std::vector<glm::vec2>& ellipse_points,
                                        const float max_scale,
                                        const SVGPaintStyle& paint_style,
                                        GeometryBuffer& geometry_buffer,
                                        const SVGImage::SVGImageRenderSettings& render_settings,
                                        const glm::vec2& scale_factors)
{
    if(paint_style.d_stroke.d_none || paint_style.d_strokeWidth == 0.0f)
        return;

    StrokeSegmentData stroke_data(geometry_buffer, paint_style.d_strokeWidth * 0.5f, paint_style, max_scale);

    if(!render_settings.d_antiAliasing)
    {
        //Calculate stroke points
        std::vector<glm::vec2> outer_ellipse_points;
        std::vector<glm::vec2> inner_ellipse_points;
        createCircleOrEllipseStrokePoints(ellipse_points, stroke_data, outer_ellipse_points, inner_ellipse_points);

        //Create the geometry from the points
        createStrokeGeometry(outer_ellipse_points, inner_ellipse_points, stroke_data, true);
    }
    else
    {
        determineAntiAliasingOffsets(stroke_data.d_paintStyle.d_strokeWidth, stroke_data.d_antiAliasingOffsets);

        //Calculate stroke points
        std::vector<glm::vec2> outer_ellipse_points;
        std::vector<glm::vec2> outer_ellipse_points_fade;
        std::vector<glm::vec2> inner_ellipse_points;
        std::vector<glm::vec2> inner_ellipse_points_fade;
        createCircleOrEllipseStrokePointsAA(ellipse_points, stroke_data, scale_factors, outer_ellipse_points,
                                            outer_ellipse_points_fade, inner_ellipse_points, inner_ellipse_points_fade);

        //Create the geometry from the points
        createStrokeGeometryAA(outer_ellipse_points, outer_ellipse_points_fade, inner_ellipse_points, inner_ellipse_points_fade,
                               stroke_data, true);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::scaleEllipsePoints(std::vector<glm::vec2>& circle_points,
                                       const bool isRadiusXBigger,
                                       const float radiusRatio)
{
    const size_t circlePointsCount = circle_points.size();
    if(isRadiusXBigger)
    {
        for(size_t i = 0; i < circlePointsCount; ++i)
            circle_points[i].y *= radiusRatio;
    }
    else
    {
        for(size_t i = 0; i < circlePointsCount; ++i)
            circle_points[i].x *= radiusRatio;
    }
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
        linejoin = SVGPaintStyle::SVGLinejoin::Bevel;
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

        if(linejoin == SVGPaintStyle::SVGLinejoin::Bevel)
        {
            //Simply add a triangle for the bevel
            addTriangleGeometry(segment_end_left, segment_end_right, second_bevel_point, stroke_data.d_geometryBuffer, stroke_data.d_strokeVertex);
        }
        else if(linejoin == SVGPaintStyle::SVGLinejoin::Round)
        {
            //Determine the linejoin angle
            float arc_angle = std::acos( glm::dot(prev_dir_to_inside, next_dir_to_inside) );

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
void SVGTesselator::createStrokeLinejoinBevelOrRoundAA
  (StrokeSegmentData &stroke_data,
   const SVGImage::SVGImageRenderSettings&,
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
    ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    const glm::vec2& inner_point = polygon_is_clockwise ? segment_end_right : segment_end_left;
    const glm::vec2& outer_point = polygon_is_clockwise ? segment_end_left : segment_end_right;

    // Get the scaled vector for the inner AA points
    glm::vec2 inner_scaled_vec = calculateScaledCombinedVector(scale_factors, prev_to_cur, cur_to_next,
                                                               prev_dir_to_inside, next_dir_to_inside, false);
    // Calculate the offset vectors for the inner points from the original point
    glm::vec2 core_offset_vec_inner = stroke_data.d_antiAliasingOffsets.x * inner_scaled_vec;
    glm::vec2 fade_offset_vec_inner = stroke_data.d_antiAliasingOffsets.y * inner_scaled_vec;
    // Calculate the inner positions of our bevel
    const glm::vec2 inner_AA = inner_point + core_offset_vec_inner;
    const glm::vec2 inner_fade_AA = inner_point + fade_offset_vec_inner;

    // Get the dir to the edge between the two outer corner points and its orthogonal direction
    glm::vec2 edge_dir;
    if (!(second_bevel_point == outer_point))
        edge_dir = glm::normalize(second_bevel_point - outer_point);
    else //TODO Ident: Check if this is a sufficient workaround
        edge_dir = prev_to_cur;
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
    glm::vec2 intersection_point;
    if(could_vectors_overlap)
        are_lines_overlapping = ( intersectLines(outer_point, outer_fade_AA, second_bevel_point, outer2_fade_AA, intersection_point) == LineIntersectResult::Intersecting );

   
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
        if(draw && linejoin == SVGPaintStyle::SVGLinejoin::Bevel)
        {
            //Add the geometry for bevel
            addTriangleGeometry(outer2_AA, inner_AA, outer_AA,
                                geometry_buffer, stroke_vertex);

            addStrokeQuadAA(outer_AA, outer2_AA, outer_fade_AA, outer2_fade_AA,
                            geometry_buffer, stroke_vertex, stroke_fade_vertex);
        }
        else if(draw && linejoin == SVGPaintStyle::SVGLinejoin::Round)
        {
            //Add the geometry for rounded linejoin
            float arc_angle = std::acos( glm::dot(prev_dir_to_inside, next_dir_to_inside) );

            //Get the parameters
            float num_segments, tangential_factor, radial_factor;
            calculateArcTesselationParameters(stroke_data.d_strokeHalfWidth, arc_angle, stroke_data.d_maxScale,
                                              num_segments, tangential_factor, radial_factor);

            //Get the arc points
            std::vector<glm::vec2> arc_points;
            createArcPoints(cur_point, outer_point, second_bevel_point, num_segments,
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
glm::vec2 SVGTesselator::determineScaleFactors(const glm::mat3& transformation, const SVGImage::SVGImageRenderSettings& render_settings)
{
    glm::vec2 scale ( glm::length(glm::vec3(transformation[0].x, transformation[1].x, transformation[2].x)), 
                      glm::length(glm::vec3(transformation[0].y, transformation[1].y, transformation[2].y)) );

    scale *= glm::vec2( render_settings.d_scaleFactor.x, render_settings.d_scaleFactor.y );
    scale = 1.0f / scale;

    return scale;
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
glm::vec2 SVGTesselator::calculateScaledCombinedVector(const glm::vec2& scale_factors,
                                                       const glm::vec2& prev_to_cur,
                                                       const glm::vec2& cur_to_next,
                                                       const glm::vec2& prev_dir_to_inside,
                                                       const glm::vec2& next_dir_to_inside,
                                                       const bool polygon_is_clockwise)
{
    bool almost_parallel = glm::dot(prev_dir_to_inside, next_dir_to_inside)/ ( glm::length(prev_dir_to_inside) * glm::length(next_dir_to_inside) ) > 0.9999f;

    if(!almost_parallel)
    {
        // Get the scaled-widths of the incoming and outgoing line segments
        float length_scale1 = calculateLengthScale(prev_dir_to_inside, scale_factors);
        float length_scale2 = calculateLengthScale(next_dir_to_inside, scale_factors);

        glm::vec2 vec_to_outer_corner = glm::normalize(prev_to_cur * length_scale2 - cur_to_next * length_scale1);

        // Calculate scale-distorted direction
        // glm::vec2 vec_to_corner = glm::normalize(prev_to_cur * length_scale2 - cur_to_next * length_scale1);

        // Calculate how much we need to offset along the direction depending on the angle
        /* We get the distance to our new corner using a factor. The factor would, in case we didn't prepare for non-uniform scaling, just consist
        of a simple vector projection. However, here we also need to multiply the local stroke width's scale factor (in the direction of the stroke)
        to get currect results. We have two alternative calculations available for this with the same results.
        Alternative version:  length_scale2 / glm::dot( (polygon_is_clockwise ? -next_dir_to_inside : next_dir_to_inside) , vec_to_corner );
        */
        float length_to_corner = length_scale1 / glm::dot( (polygon_is_clockwise ? -prev_dir_to_inside : prev_dir_to_inside) , vec_to_outer_corner );
 
        return vec_to_outer_corner * length_to_corner;
    }
    //TODO Ident: Check if this fix is valid in all cases
    else
    {
        //If we are near-parallel we need to calculate the corner vector differently
        float length_scale1 = calculateLengthScale(prev_dir_to_inside, scale_factors);

        glm::vec2 vec_to_outer_corner = 0.5f * ( next_dir_to_inside + prev_dir_to_inside );

        float length_to_corner = length_scale1 / glm::dot( (polygon_is_clockwise ? -prev_dir_to_inside : prev_dir_to_inside) , vec_to_outer_corner );
 
        
        return vec_to_outer_corner * length_to_corner;
    }
}

//----------------------------------------------------------------------------//
bool SVGTesselator::isVectorLeftOfOtherVector(const glm::vec2& vector,
                                              const glm::vec2& vector_other)
{
    float dot_result = (vector.x * -vector_other.y) + (vector.y * vector_other.x);
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
void SVGTesselator::createFill(const std::vector<glm::vec2>& points,
                               GeometryBuffer& geometry_buffer,
                               const SVGPaintStyle& paint_style,
                               const SVGImage::SVGImageRenderSettings&,
                               const glm::vec2& /*scale_factors*/)
{
    if(points.size() < 3 || paint_style.d_fill.d_none)
        return;

    //Create the rectangle fill vertex
    ColouredVertex fill_vertex(glm::vec3(), getFillColour(paint_style));

    //Switches the stencil mode on
    geometry_buffer.setStencilRenderingActive(paint_style.d_fillRule);

    addTriangleFanGeometry(points, geometry_buffer, fill_vertex);
    //Set the vertex count to the quad's vertex count
    geometry_buffer.setStencilPostRenderingVertexCount(6);

    //Calculate the axis-aligned bounding box for the vertices that we will use to create a minimally sized quad
    glm::vec2 min, max;
    calculateMinMax(points, min, max);

    //Add the quad
    addFillQuad(min, glm::vec2(min.x, max.y), glm::vec2(max.x, min.y), max,
                geometry_buffer, fill_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::calculateMinMax(const std::vector<glm::vec2>& points,
                                    glm::vec2& min,
                                    glm::vec2& max)
{
    if(points.empty())
        return;

    min = points[0];
    max = points[0];

    const size_t points_count = points.size();
    for(size_t i = 1; i < points_count; ++i)
    {
        min = glm::min(points[i], min);
        max = glm::max(points[i], max);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createEllipsePoints(const float radiusX, 
                                        const float radiusY,
                                        const float max_scale,
                                        std::vector<glm::vec2>& ellipse_points)
{
    bool isRadiusXBigger;
    float biggerRadius, radiusRatio;
    if(radiusX >= radiusY)
    {
        biggerRadius = radiusX;
        isRadiusXBigger = true;
        radiusRatio = radiusY / radiusX;
    }
    else
    {
        biggerRadius = radiusY;
        isRadiusXBigger = false;
        radiusRatio = radiusX / radiusY;
    }

    //Precalculate values needed for the circle tesselation
    float num_segments, cos_value, sin_value;
    calculateCircleTesselationParameters(biggerRadius, max_scale, num_segments, cos_value, sin_value);

    //Create the circle points
    createCirclePoints(biggerRadius, num_segments, cos_value, sin_value, ellipse_points);

    //Scale the circle so we get an ellipse
    scaleEllipsePoints(ellipse_points, isRadiusXBigger, radiusRatio);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCircleOrEllipseStrokePoints(const std::vector<glm::vec2>& points,
                                                      StrokeSegmentData& stroke_data,
                                                      std::vector<glm::vec2>& outer_points,
                                                      std::vector<glm::vec2>& inner_points)
{
    const size_t points_count = points.size();
    for(size_t i = 0; i < points_count; ++i) 
    {
        size_t index1 = (i == points_count - 1) ? 0 : (i + 1);
        size_t index2 = (i == 0) ? (points_count - 1) : (i - 1);

        glm::vec2 direction = glm::normalize(points[index1] - points[index2]);
        direction = glm::vec2(direction.y, -direction.x);

        outer_points.push_back( points[i] + direction * stroke_data.d_strokeHalfWidth );
        inner_points.push_back( points[i] - direction * stroke_data.d_strokeHalfWidth );
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCircleOrEllipseStrokePointsAA(const std::vector<glm::vec2>& points,
                                                        StrokeSegmentData& stroke_data,
                                                        const glm::vec2& scale_factors,
                                                        std::vector<glm::vec2>& outer_points,
                                                        std::vector<glm::vec2>& outer_points_fade,
                                                        std::vector<glm::vec2>& inner_points,
                                                        std::vector<glm::vec2>& inner_points_fade)
{
    const size_t points_count = points.size();
    for(size_t i = 0; i < points_count; ++i) 
    {
        size_t index1 = (i == points_count - 1) ? 0 : (i + 1);
        size_t index2 = (i == 0) ? (points_count - 1) : (i - 1);

        glm::vec2 direction = glm::normalize(points[index1] - points[index2]);
        direction = glm::vec2(direction.y, -direction.x);

        float length_scale = calculateLengthScale(direction, scale_factors);
        glm::vec2 scaled_dir_vec = direction * length_scale;
        glm::vec2 stroke_offset_vec = direction * stroke_data.d_strokeHalfWidth;

        outer_points.push_back( points[i] + stroke_offset_vec + scaled_dir_vec * stroke_data.d_antiAliasingOffsets.x);
        outer_points_fade.push_back( points[i] + stroke_offset_vec + scaled_dir_vec * stroke_data.d_antiAliasingOffsets.y);
        inner_points.push_back( points[i] - stroke_offset_vec - scaled_dir_vec * stroke_data.d_antiAliasingOffsets.x);
        inner_points_fade.push_back( points[i] - stroke_offset_vec - scaled_dir_vec * stroke_data.d_antiAliasingOffsets.y);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createRectangleFill(const SVGPaintStyle& paint_style, std::vector<glm::vec2>& rectangle_points, GeometryBuffer& geometry_buffer)
{
    if(paint_style.d_fill.d_none)
        return;

    //Get colours
    const glm::vec4 fill_colour = getFillColour(paint_style);

    //Create the rectangle fill vertex
    ColouredVertex rectFillVertex(glm::vec3(), fill_colour);
    //Apply z-depth
    rectFillVertex.d_position.z = 0.0f;

    //Add the rectangle fill vertices
    rectFillVertex.d_position.x = rectangle_points[0].x;
    rectFillVertex.d_position.y = rectangle_points[0].y;
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.x = rectangle_points[1].x;
    rectFillVertex.d_position.y = rectangle_points[1].y;
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.x = rectangle_points[2].x;
    rectFillVertex.d_position.y = rectangle_points[2].y;
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.x = rectangle_points[2].x;
    rectFillVertex.d_position.y = rectangle_points[2].y;
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.x = rectangle_points[0].x;
    rectFillVertex.d_position.y = rectangle_points[0].y;
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.d_position.x = rectangle_points[3].x;
    rectFillVertex.d_position.y = rectangle_points[3].y;
    geometry_buffer.appendVertex(rectFillVertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegmentConnection(StrokeSegmentData& stroke_data)
{
    // Add the geometry
    ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    stroke_vertex.d_position.x = stroke_data.d_currentPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_currentPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_currentPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);
}


//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeSegmentConnectionAA(StrokeSegmentData &stroke_data)
{ 
    ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    //Fade1
    stroke_fade_vertex.d_position.x = stroke_data.d_currentFadePointLeft.x;
    stroke_fade_vertex.d_position.y = stroke_data.d_currentFadePointLeft.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_fade_vertex.d_position.x = stroke_data.d_lastFadePointLeft.x;
    stroke_fade_vertex.d_position.y = stroke_data.d_lastFadePointLeft.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_vertex.d_position.x = stroke_data.d_currentPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_currentPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_fade_vertex.d_position.x = stroke_data.d_lastFadePointLeft.x;
    stroke_fade_vertex.d_position.y = stroke_data.d_lastFadePointLeft.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);


    //Core
    stroke_vertex.d_position.x = stroke_data.d_currentPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_currentPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_currentPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointLeft.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointLeft.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);


    //Fade1
    stroke_vertex.d_position.x = stroke_data.d_currentPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_currentPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_fade_vertex.d_position.x = stroke_data.d_currentFadePointRight.x;
    stroke_fade_vertex.d_position.y = stroke_data.d_currentFadePointRight.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_fade_vertex.d_position.x = stroke_data.d_currentFadePointRight.x;
    stroke_fade_vertex.d_position.y = stroke_data.d_currentFadePointRight.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_vertex.d_position.x = stroke_data.d_lastPointRight.x;
    stroke_vertex.d_position.y = stroke_data.d_lastPointRight.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_fade_vertex.d_position.x = stroke_data.d_lastFadePointRight.x;
    stroke_fade_vertex.d_position.y = stroke_data.d_lastFadePointRight.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::addStrokeLinecapAAGeometryVertices(StrokeSegmentData &stroke_data,
                                                       const glm::vec2& linecap_left, const glm::vec2& linecap_right,
                                                       const glm::vec2& linecap_fade_left, const glm::vec2& linecap_fade_right)
{ 
    ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    stroke_fade_vertex.d_position.x = linecap_fade_left.x;
    stroke_fade_vertex.d_position.y = linecap_fade_left.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_fade_vertex.d_position.x = linecap_fade_right.x;
    stroke_fade_vertex.d_position.y = linecap_fade_right.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.x = linecap_left.x;
    stroke_vertex.d_position.y = linecap_left.y;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_vertex.d_position.x = linecap_left.x;
    stroke_vertex.d_position.y = linecap_left.y;
    geometry_buffer.appendVertex(stroke_vertex);
    stroke_fade_vertex.d_position.x = linecap_fade_right.x;
    stroke_fade_vertex.d_position.y = linecap_fade_right.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);
    stroke_vertex.d_position.x = linecap_right.x;
    stroke_vertex.d_position.y = linecap_right.y;
    geometry_buffer.appendVertex(stroke_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::addTriangleGeometry(const glm::vec2& point1,
                                        const glm::vec2& point2,
                                        const glm::vec2& point3,
                                        GeometryBuffer &geometry_buffer,
                                        ColouredVertex &vertex)
{
    vertex.d_position.x = point1.x;
    vertex.d_position.y = point1.y;
    geometry_buffer.appendVertex(vertex);

    vertex.d_position.x = point2.x;
    vertex.d_position.y = point2.y;
    geometry_buffer.appendVertex(vertex);

    vertex.d_position.x = point3.x;
    vertex.d_position.y = point3.y;
    geometry_buffer.appendVertex(vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeGeometry(const std::vector<glm::vec2>& outer_points,
                                         const std::vector<glm::vec2>& inner_points,
                                         StrokeSegmentData &stroke_data,
                                         const bool is_surface_closed)
{
    ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    size_t circle_points_count = outer_points.size();
    for(size_t i = 0; i < circle_points_count - 1; ++i)
        addStrokeQuad(outer_points[i], outer_points[i + 1], inner_points[i], inner_points[i + 1],
                      geometry_buffer, stroke_vertex);

    if(is_surface_closed)
        addStrokeQuad(outer_points[circle_points_count - 1], outer_points[0], inner_points[circle_points_count - 1], inner_points[0],
                      geometry_buffer, stroke_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createStrokeGeometryAA(const std::vector<glm::vec2>& outer_points,
                                           const std::vector<glm::vec2>& outer_points_fade,
                                           const std::vector<glm::vec2>& inner_points,
                                           const std::vector<glm::vec2>& inner_points_fade,
                                           StrokeSegmentData &stroke_data,
                                           const bool is_surface_closed)
{
    ColouredVertex& stroke_vertex = stroke_data.d_strokeVertex;
    ColouredVertex& stroke_fade_vertex = stroke_data.d_strokeFadeVertex;
    GeometryBuffer& geometry_buffer = stroke_data.d_geometryBuffer;

    size_t points_count = outer_points.size();
    for(size_t i = 0; i < points_count - 1; ++i)
    {
        addStrokeQuadAA(outer_points[i], outer_points[i + 1], outer_points_fade[i], outer_points_fade[i + 1],
                        geometry_buffer, stroke_vertex, stroke_fade_vertex);
        addStrokeQuad(outer_points[i], outer_points[i + 1], inner_points[i], inner_points[i + 1],
                        geometry_buffer, stroke_vertex);
        addStrokeQuadAA(inner_points[i], inner_points[i + 1], inner_points_fade[i], inner_points_fade[i + 1],
                        geometry_buffer, stroke_vertex, stroke_fade_vertex);
    }

    if(is_surface_closed)
    {
        addStrokeQuadAA(outer_points[points_count - 1], outer_points[0], outer_points_fade[points_count - 1], outer_points_fade[0],
                        geometry_buffer, stroke_vertex, stroke_fade_vertex);
        addStrokeQuad(outer_points[points_count - 1], outer_points[0], inner_points[points_count - 1], inner_points[0],
                        geometry_buffer, stroke_vertex);
        addStrokeQuadAA(inner_points[points_count - 1], inner_points[0], inner_points_fade[points_count - 1], inner_points_fade[0],
                        geometry_buffer, stroke_vertex, stroke_fade_vertex);
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::createFillGeometryAAFadeOnly(const std::vector<glm::vec2>& points,
                                                 const std::vector<glm::vec2>& points_fade,
                                                 const SVGPaintStyle& paint_style,
                                                 GeometryBuffer& geometry_buffer,
                                                 const bool is_surface_closed)
{
    // Get and add the fill colour
    ColouredVertex fill_vertex;
    fill_vertex.d_colour = SVGTesselator::getFillColour(paint_style);
    // Set the z coordinate
    fill_vertex.d_position.z = 0.0f;
    //Create the fade fill vertex from the fill vertex and set its alpha to 0
    ColouredVertex fill_fade_vertex = fill_vertex;
    fill_fade_vertex.d_colour.w = 0.0f;

    size_t points_count = points.size();
    for(size_t i = 0; i < points_count - 1; ++i)
        addStrokeQuadAA(points[i], points[i + 1], points_fade[i], points_fade[i + 1],
                        geometry_buffer, fill_vertex, fill_fade_vertex);

    if(is_surface_closed)
        addStrokeQuadAA(points[points_count - 1], points[0], points_fade[points_count - 1], points_fade[0],
                        geometry_buffer, fill_vertex, fill_fade_vertex);
}


//----------------------------------------------------------------------------//
void SVGTesselator::addFillQuad(const glm::vec2& point1,
                                const glm::vec2& point2,
                                const glm::vec2& point3,
                                const glm::vec2& point4,
                                GeometryBuffer& geometry_buffer,
                                ColouredVertex& fill_vertex)
{
    fill_vertex.d_position.x = point1.x;
    fill_vertex.d_position.y = point1.y;
    geometry_buffer.appendVertex(fill_vertex);

    fill_vertex.d_position.x = point2.x;
    fill_vertex.d_position.y = point2.y;
    geometry_buffer.appendVertex(fill_vertex);

    fill_vertex.d_position.x = point3.x;
    fill_vertex.d_position.y = point3.y;
    geometry_buffer.appendVertex(fill_vertex);

    fill_vertex.d_position.x = point3.x;
    fill_vertex.d_position.y = point3.y;
    geometry_buffer.appendVertex(fill_vertex);

    fill_vertex.d_position.x = point2.x;
    fill_vertex.d_position.y = point2.y;
    geometry_buffer.appendVertex(fill_vertex);

    fill_vertex.d_position.x = point4.x;
    fill_vertex.d_position.y = point4.y;
    geometry_buffer.appendVertex(fill_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::addStrokeQuad(const glm::vec2& point1,
                                  const glm::vec2& point2,
                                  const glm::vec2& point3,
                                  const glm::vec2& point4,
                                  GeometryBuffer& geometry_buffer,
                                  ColouredVertex& stroke_vertex)
{
    stroke_vertex.d_position.x = point1.x;
    stroke_vertex.d_position.y = point1.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = point2.x;
    stroke_vertex.d_position.y = point2.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = point3.x;
    stroke_vertex.d_position.y = point3.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = point3.x;
    stroke_vertex.d_position.y = point3.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = point2.x;
    stroke_vertex.d_position.y = point2.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = point4.x;
    stroke_vertex.d_position.y = point4.y;
    geometry_buffer.appendVertex(stroke_vertex);
}


//----------------------------------------------------------------------------//
void SVGTesselator::addStrokeQuadAA(const glm::vec2& point1,
                                    const glm::vec2& point2,
                                    const glm::vec2& fade_point1,
                                    const glm::vec2& fade_point2,
                                    GeometryBuffer& geometry_buffer,
                                    ColouredVertex& stroke_vertex,
                                    ColouredVertex& stroke_fade_vertex)
{
    stroke_fade_vertex.d_position.x = fade_point1.x;
    stroke_fade_vertex.d_position.y = fade_point1.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_fade_vertex.d_position.x = fade_point2.x;
    stroke_fade_vertex.d_position.y = fade_point2.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_vertex.d_position.x = point1.x;
    stroke_vertex.d_position.y = point1.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_vertex.d_position.x = point1.x;
    stroke_vertex.d_position.y = point1.y;
    geometry_buffer.appendVertex(stroke_vertex);

    stroke_fade_vertex.d_position.x = fade_point2.x;
    stroke_fade_vertex.d_position.y = fade_point2.y;
    geometry_buffer.appendVertex(stroke_fade_vertex);

    stroke_vertex.d_position.x = point2.x;
    stroke_vertex.d_position.y = point2.y;
    geometry_buffer.appendVertex(stroke_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::createCircleOrEllipseFillPointsAA(const std::vector<glm::vec2> &points,
                                                      const glm::vec2 &anti_aliasing_offsets,
                                                      const glm::vec2 &scale_factors,
                                                      std::vector<glm::vec2> &modified_points,
                                                      std::vector<glm::vec2> &fade_points)
{
    const size_t points_count = points.size();
    for(size_t i = 0; i < points_count; ++i)
    {
        size_t index1 = (i == points_count - 1) ? 0 : (i + 1);
        size_t index2 = (i == 0) ? (points_count - 1) : (i - 1);

        glm::vec2 direction = glm::normalize(points[index1] - points[index2]);
        direction = glm::vec2(direction.y, -direction.x);

        float length_scale = calculateLengthScale(direction, scale_factors);
        direction *= length_scale;

        modified_points.push_back( points[i] + direction * anti_aliasing_offsets.x );
        fade_points.push_back( points[i] + direction * anti_aliasing_offsets.y );
    }
}

//----------------------------------------------------------------------------//
void SVGTesselator::addTriangleFanGeometry(const std::vector<glm::vec2> &points,
                                           GeometryBuffer& geometry_buffer,
                                           ColouredVertex& coloured_vertex)
{
    //Fixed triangle fan point
    const glm::vec2& point1 = points[0];

    const size_t maximum_index = points.size() - 1;
    for(size_t i = 1; i < maximum_index; ++i)
        addTriangleGeometry(point1, points[i], points[i + 1], geometry_buffer, coloured_vertex);
}

//----------------------------------------------------------------------------//
void SVGTesselator::setupGeometryBufferSettings(CEGUI::GeometryBuffer* geometry_buffer,
                                                const SVGImage::SVGImageRenderSettings &render_settings,
                                                const glm::mat4& cegui_transformation_matrix)
{
    if(render_settings.d_clipArea)
    {
        geometry_buffer->setClippingActive(true);
        geometry_buffer->setClippingRegion(*render_settings.d_clipArea);
    }
    else
        geometry_buffer->setClippingActive(false);

    geometry_buffer->setScale(glm::vec2(render_settings.d_scaleFactor.x, render_settings.d_scaleFactor.y));
    geometry_buffer->setCustomTransform(cegui_transformation_matrix);
    geometry_buffer->setAlpha(render_settings.d_alpha);
}

//----------------------------------------------------------------------------//
}

