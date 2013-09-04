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
#ifndef _SVGTesselator_h_
#define _SVGTesselator_h_

#include "CEGUI/Base.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/svg/SVGImage.h"
#include "CEGUI/svg/SVGPaintStyle.h"

#include "glm/glm.hpp"

#include <vector>

namespace CEGUI
{
class GeometryBuffer;
class SVGRect;
class SVGCircle;
class SVGLine;
class SVGPolyline;
class SVGPaintStyle;
/*!
\brief
    Defines a static class that provides helper functions for the tesselation of
    SVGBasicShapes.
*/
class CEGUIEXPORT SVGTesselator
{
public:
    
    //! Object containing the data needed for rendering segments
    class StrokeSegmentData
    {
    public:
        StrokeSegmentData(GeometryBuffer& geometry_buffer, const float stroke_half_width, const SVGPaintStyle& paint_style, const float max_scale);

        /*!
        \brief
            Sets the pointers to the line points of this segment
        */
        void setPoints(const glm::vec2& prev_point,
                       const glm::vec2& cur_point,
                       const glm::vec2& next_point);

        //! Half of the width of the stroke
        const float d_strokeHalfWidth;
        //! The geometry buffer we will draw into
        GeometryBuffer& d_geometryBuffer;
        //! The paint style
        const SVGPaintStyle& d_paintStyle;

        //! Previous left stroke point lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_lastPointLeft;
        //! Previous right stroke point lying in clockwise direction away from the stroke direction-.
        glm::vec2 d_lastPointRight;

        //! Previous left stroke fade point lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_lastFadePointLeft;
        //! Previous right stroke fade point lying in clockwise direction away from the stroke direction-.
        glm::vec2 d_lastFadePointRight;

        //! The vertex we will modify with positions and append to the GeometryBuffer
        ColouredVertex d_strokeVertex;
        //! The vertex we will modify with positions and append to the GeometryBuffer
        ColouredVertex d_strokeFadeVertex;

        //! The maximum of the scalings (either vert or horz). We need this to determine the degree of tesselation
        // of curved elements of the stroke
        float d_maxScale;

        //! Pointer to the previous line point of this segment
        const glm::vec2* d_prevPoint;
        //! Pointer to the current line point of this segment
        const glm::vec2* d_curPoint;
        //! Pointer to the subsequent line point of this segment
        const glm::vec2* d_nextPoint;
    };

    /*!
    \brief
        Tesselates an SVGLine and adds the created geometry to the GeometryBuffer
        list.

    \param line
            The SVGLine object that contains the data.
    \param geometry_buffers
            The GeometryBuffer list to which the created geometry will be added.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.
    */
    static void tesselateAndRenderLine(const SVGLine* line,
                                       std::vector<GeometryBuffer*>& geometry_buffers,
                                       const SVGImage::SVGImageRenderSettings& render_settings);
    
    /*!
    \brief
        Tesselates an SVGPolyline and adds the created geometry to the GeometryBuffer
        list.

    \param polyline
            The SVGPolyline object that contains the data.
    \param geometry_buffers
            The GeometryBuffer list to which the created geometry will be added.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.
    */
    static void tesselateAndRenderPolyline(const SVGPolyline* polyline,
                                           std::vector<GeometryBuffer*>& geometry_buffers,
                                           const SVGImage::SVGImageRenderSettings& render_settings);
    /*!
    \brief
        Tesselates an SVGRect and adds the created geometry to the GeometryBuffer
        list.
    
    \param rect
            The SVGRect object that contains the data.
    \param geometry_buffers
            The GeometryBuffer list to which the created geometry will be added.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.
    */
    static void tesselateAndRenderRect(const SVGRect* rect,
                                       std::vector<GeometryBuffer*>& geometry_buffers,
                                       const SVGImage::SVGImageRenderSettings& render_settings);

    static void createRectangleFill(const SVGPaintStyle& paint_style, std::vector<glm::vec2>& rectangle_points, GeometryBuffer& geometry_buffer);

    /*!
    \brief
        Tesselates an SVGCircle and adds the created geometry to the GeometryBuffer
        list.
    
    \param rect
            The SVGCircle object that contains the data.
    \param geometry_buffers
            The GeometryBuffer list to which the created geometry will be added.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.
    */
    static void tesselateAndRenderCircle(const SVGCircle* circle,
                                         std::vector<GeometryBuffer*>& geometry_buffers,
                                         const SVGImage::SVGImageRenderSettings& render_settings);

private:
    //! Constructor.
    SVGTesselator();

    //! Destructor.
    ~SVGTesselator();

    //! Helper function for creating a stroke based on a list of points and the settings
    static void createStroke(const std::vector<glm::vec2>& points,
                             GeometryBuffer& geometry_buffer,
                             const SVGPaintStyle& paint_style,
                             const SVGImage::SVGImageRenderSettings& render_settings,
                             const glm::vec2& scale_factors,
                             const bool is_shape_closed);

    //! Stroke helper function that determines vertices of a stroke segment and adds them to the geometry buffer
    static void createStrokeSegment(StrokeSegmentData& stroke_data,
                                    const SVGImage::SVGImageRenderSettings& render_settings,
                                    const glm::vec2& scale_factors,
                                    const bool draw = true);

    static void createStrokeLinejoinBevelOrRound(StrokeSegmentData &stroke_data,
                                                 const glm::vec2& cur_point,
                                                 const glm::vec2& secondBevelPoint,
                                                 const glm::vec2& segment_end_left,
                                                 const glm::vec2& segment_end_right,
                                                 const SVGPaintStyle::SVGLinejoin linejoin,
                                                 const bool polygon_is_clockwise);

    static void handleStrokeMiterExceedance(const StrokeSegmentData& stroke_data,
                                            const glm::vec2& cur_point,
                                            const glm::vec2& inner_intersection,
                                            SVGPaintStyle::SVGLinejoin& linejoin);

    //! Stroke draw helper function that appends geometry for the connection between two new points and the last points
    static void addStrokeSegmentConnectionGeometry(StrokeSegmentData &stroke_data,
                                                   const glm::vec2& segment_end_left,
                                                   const glm::vec2& segment_end_right,
                                                   const bool draw);

    static void addStrokeSegmentAAConnectionGeometry(StrokeSegmentData& stroke_data,
                                                    const glm::vec2& segment_end_left_orig,
                                                    const glm::vec2& segment_end_right_orig,
                                                    const bool polygon_is_clockwise,
                                                    const glm::vec2& prev_dir_to_inside,
                                                    const glm::vec2& next_dir_to_inside,
                                                    float core_offset,
                                                    float fade_offset,
                                                    const glm::vec2& scale_factors,
                                                    const bool draw);

    static float calculateLengthScale(const glm::vec2 &direction, const glm::vec2& scale_factors);

    static void addStrokeSegmentAAConnectionGeometryVertices(StrokeSegmentData &stroke_data,
                                                             const glm::vec2& segmentFadeLeftEnd, const glm::vec2& segmentLeftEnd,
                                                             const glm::vec2& segmentRightEnd, const glm::vec2& segmentFadeRightEnd);

    //! Stroke draw helper function that adds the linecap depending on linecap type and beginning/end
    static void createLinecap(StrokeSegmentData& stroke_data,
                              const bool is_start);

    //! Stroke helper function that determines if the polygon encompassed by the points is clockwise
    static bool isPolygonClockwise(const glm::vec2& point1,
                                   const glm::vec2& point2,
                                   const glm::vec2& point3);

    //! Helper function that creates and sets the parameters for a coloured geometry buffer
    static GeometryBuffer& setupGeometryBufferColoured(std::vector<GeometryBuffer*>& geometry_buffers,
                                                       const SVGImage::SVGImageRenderSettings& render_settings,
                                                       const glm::mat3x3& svg_transformation);

    //! Turns a matrix as defined by SVG into a matrix that can be used internally by the CEGUI Renderers
    static glm::mat4 createRenderableMatrixFromSVGMatrix(glm::mat3 svg_matrix);

    //! Helper function for getting the fill Colour from an SVGPaintStyle
    static CEGUI::Colour getFillColour(const SVGPaintStyle& paint_style);

    //! Helper function for getting the stroke Colour from an SVGPaintStyle
    static CEGUI::Colour getStrokeColour(const SVGPaintStyle& paint_style);

    
    static void createCircleFill(const float& radius,
                                 float max_scale,
                                 const SVGPaintStyle& paint_style,
                                 GeometryBuffer& geometry_buffer);

    static void calculateCircleTesselationParameters(const float radius,
                                                     const float max_scale,
                                                     float& num_segments,
                                                     float& cos_value,
                                                     float& sin_value);

    static void createCirclePoints(const float radius,
                                   const float num_segments,
                                   const float cos_value,
                                   const float sin_value,
                                   std::vector<glm::vec2>& circle_points);

    static void createCircleStroke(const float& radius,
                                   float max_scale,
                                   const SVGPaintStyle& paint_style,
                                   GeometryBuffer& geometry_buffer);

    //! Helper function for creating a circle's fill
    static void createTriangleStripFillGeometry(std::vector<glm::vec2>& points,
                                         GeometryBuffer& geometry_buffer,
                                         const SVGPaintStyle& paint_style);

    //! Helper function for creating a circle's fill
    static void createArcStrokeGeometry(std::vector<glm::vec2>& points,
                                        GeometryBuffer& geometry_buffer,
                                        ColouredVertex& stroke_vertex);
    

     //! Helper function for appending a circle fill triangle to a GeometryBuffer
    static void addTriangleGeometry(const glm::vec2& point1,
                                    const glm::vec2& point2,
                                    const glm::vec2& point3,
                                    GeometryBuffer &geometry_buffer,
                                    ColouredVertex &vertex);

    static void createCircleStrokeGeometry(const std::vector<glm::vec2>& outer_circle_points,
                                           const std::vector<glm::vec2>& inner_circle_points,
                                           const SVGPaintStyle& paint_style,
                                           GeometryBuffer& geometry_buffer);

    static void calculateArcTesselationParameters(const float radius,
                                                  const float arc_angle,
                                                  const float max_scale,
                                                  float& num_segments,
                                                  float& tangential_factor,
                                                  float& radial_factor);

    static void createArcPoints(const glm::vec2& center_point,
                                const glm::vec2& start_point,
                                const glm::vec2& end_point,
                                const float num_segments,
                                const float tangential_factor,
                                const float radial_factor,
                                std::vector<glm::vec2>& arc_points);

    static void determineAntiAliasingOffsets(float width, float& core_offset, float& fade_offset);

    static glm::vec2 getScaleFactors(const glm::mat3& transformation, const SVGImage::SVGImageRenderSettings& render_settings);
};

}

#endif

