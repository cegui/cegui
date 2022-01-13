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
#ifndef _SVGTesselator_h_
#define _SVGTesselator_h_

#include "CEGUI/Base.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/svg/SVGImage.h"
#include "CEGUI/svg/SVGPaintStyle.h"

#include <glm/glm.hpp>

#include <vector>

namespace CEGUI
{
class GeometryBuffer;
class SVGRect;
class SVGCircle;
class SVGEllipse;
class SVGLine;
class SVGPolyline;
class SVGPolygon;
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

        //! Last left stroke point, lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_lastPointLeft;
        //! Last right stroke point, lying in clockwise direction away from the stroke direction.
        glm::vec2 d_lastPointRight;
        //! Last left stroke fade point, lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_lastFadePointLeft;
        //! Last right stroke fade point, lying in clockwise direction away from the stroke direction.
        glm::vec2 d_lastFadePointRight;

        //! Current left stroke point, lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_currentPointLeft;
        //! Current right stroke point, lying in clockwise direction away from the stroke direction.
        glm::vec2 d_currentPointRight;
        //! Current left stroke fade point, lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_currentFadePointLeft;
        //! Current right stroke fade point, lying in clockwise direction away from the stroke direction.
        glm::vec2 d_currentFadePointRight;

        //! Last left stroke point, lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_subsequentPointLeft;
        //! Last right stroke point, lying in clockwise direction away from the stroke direction.
        glm::vec2 d_subsequentPointRight;
        //! Last left stroke fade point, lying in anti-clockwise direction away from the stroke direction.
        glm::vec2 d_subsequentFadePointLeft;
        //! Last right stroke fade point, lying in clockwise direction away from the stroke direction.
        glm::vec2 d_subsequentFadePointRight;

        //! The vertex we will modify with positions and append to the GeometryBuffer
        ColouredVertex d_strokeVertex;
        //! The vertex we will modify with positions and append to the GeometryBuffer
        ColouredVertex d_strokeFadeVertex;

        //! The maximum of the scalings (either vert or horz). We need this to determine the degree of tesselation
        // of curved elements of the stroke
        float d_maxScale;

        //! Anti-aliasing offsets, the first element represents the offset of the solid stroke, the second is for the
        // offset of the alpha-fade
        glm::vec2 d_antiAliasingOffsets;

        //! Pointer to the previous line point of this segment
        const glm::vec2* d_prevPoint;
        //! Pointer to the current line point of this segment
        const glm::vec2* d_curPoint;
        //! Pointer to the subsequent line point of this segment
        const glm::vec2* d_nextPoint;
    };
    
    /*!
    \brief
        Tesselates an SVGRect and returns the created render geometry.
    
    \param rect
            The SVGRect object that contains the data.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.

    \return
            Returns the tesselated render geometry for this shape.
    */
    static void tesselateRect(std::vector<GeometryBuffer*>& out,
        const SVGRect* rect,
        const SVGImage::SVGImageRenderSettings& render_settings);

    /*!
    \brief
        Tesselates an SVGCircle and returns the created render geometry.

    \param rect
            The SVGCircle object that contains the data.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.

    \return
            Returns the tesselated render geometry for this shape.
    */
    static void tesselateCircle(std::vector<GeometryBuffer*>& out,
        const SVGCircle* circle,
        const SVGImage::SVGImageRenderSettings& render_settings);

    /*!
    \brief
        Tesselates an SVGEllipse and returns the created render geometry.

    \param rect
            The SVGEllipse object that contains the data.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.

    \return
            Returns the tesselated render geometry for this shape.
    */
    static void tesselateEllipse(std::vector<GeometryBuffer*>& out,
        const SVGEllipse* ellipse,
        const SVGImage::SVGImageRenderSettings& render_settings);

    /*!
    \brief
        Tesselates an SVGLine and returns the created render geometry.

    \param line
            The SVGLine object that contains the data.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.

    \return
            Returns the tesselated render geometry for this shape.
    */
    static void tesselateLine(std::vector<GeometryBuffer*>& out,
        const SVGLine* line,
        const SVGImage::SVGImageRenderSettings& render_settings);

    /*!
    \brief
        Tesselates an SVGPolyline and returns the created render geometry.

    \param polyline
            The SVGPolyline object that contains the data.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.

    \return
            Returns the tesselated render geometry for this shape.
    */
    static void tesselatePolyline(std::vector<GeometryBuffer*>& out,
        const SVGPolyline* polyline,
        const SVGImage::SVGImageRenderSettings& render_settings);

    /*!
    \brief
        Tesselates an SVGPolygon and returns the created render geometry.

    \param polyline
            The SVGPolygon object that contains the data.
    \param render_settings
            The ImageRenderSettings for the geometry that will be created.

    \return
            Returns the tesselated render geometry for this shape.
    */
    static void tesselatePolygon(std::vector<GeometryBuffer*>& out,
        const SVGPolygon* polyline,
        const SVGImage::SVGImageRenderSettings& render_settings);

private:
    /*!
	\brief
		Enumeration that defines how two lines are or are not intersecting with each other.
	*/
    enum class LineIntersectResult : int
    {
        Parallel,
        Coincident,
        NotIntersecting,
        Intersecting,

        Count
    };

    //! Constructor.
    SVGTesselator() = default;

    //! Destructor.
    ~SVGTesselator() = default;

    //! Helper function for creating a fill based on a list of polygon points
    static void createFill(const std::vector<glm::vec2>& points,
                           GeometryBuffer& geometry_buffer,
                           const SVGPaintStyle& paint_style,
                           const SVGImage::SVGImageRenderSettings& render_settings,
                           const glm::vec2& scale_factors);

    //! Helper function for creating a stroke based on a list of subsequent points forming the stroke
    static void createStroke(const std::vector<glm::vec2>& points,
                             GeometryBuffer& geometry_buffer,
                             const SVGPaintStyle& paint_style,
                             const SVGImage::SVGImageRenderSettings& render_settings,
                             const glm::vec2& scale_factors,
                             const bool is_shape_closed);

    //! Stroke helper function that determines vertices of a stroke segment and adds them to the geometry buffer
    static void createStrokeLinejoin(StrokeSegmentData& stroke_data,
                                    const SVGImage::SVGImageRenderSettings& render_settings,
                                    const glm::vec2& scale_factors,
                                    const bool draw = true);

    //! Stroke helper function that determines and adds the anti-aliased geometry of a bevel- or rounded-linejoin
    static void createStrokeLinejoinBevelOrRoundAA(StrokeSegmentData &stroke_data,
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
                                                   const bool draw);

    //! Stroke helper function that determines and adds the geometry of a bevel- or rounded-linejoin
    static void createStrokeLinejoinBevelOrRound(StrokeSegmentData &stroke_data,
                                                 const glm::vec2& cur_point,
                                                 const glm::vec2& prev_dir_to_inside,
                                                 const glm::vec2& next_dir_to_inside,
                                                 const glm::vec2& segment_end_left,
                                                 const glm::vec2& segment_end_right,
                                                 const glm::vec2& second_bevel_point,
                                                 const SVGPaintStyle::SVGLinejoin linejoin,
                                                 const bool polygon_is_clockwise,
                                                 const bool draw);

    //! Checks if the vector is left (meaning that the angle is smaller in clockwise direction) of the other vector
    static bool isVectorLeftOfOtherVector(const glm::vec2& vector,
                                          const glm::vec2& vector_other);

    //! Checks if the stroke miter is exceeding the maximum set for it, and if this is the case switches the linejoin to bevel
    static void handleStrokeMiterExceedance(const StrokeSegmentData& stroke_data,
                                            const glm::vec2& cur_point,
                                            const glm::vec2& inner_intersection,
                                            SVGPaintStyle::SVGLinejoin& linejoin);

    //! Calculates the anti-aliased miter-linejoin points and sets the points necessary to form the connection
    static void calculateAAMiterAndSetConnectionPoints(StrokeSegmentData& stroke_data,
                                                        const glm::vec2& segment_end_left_orig,
                                                        const glm::vec2& segment_end_right_orig,
                                                        const bool polygon_is_clockwise,
                                                        const glm::vec2& prev_to_cur,
                                                        const glm::vec2& cur_to_next,
                                                        const glm::vec2& prev_dir_to_inside,
                                                        const glm::vec2& next_dir_to_inside,
                                                        const glm::vec2& scale_factors);

    //! Calculates the scaled vector based on two direction vectors and their perpendicular vectors
    static glm::vec2 calculateScaledCombinedVector(const glm::vec2& scale_factors,
                                                   const glm::vec2& prev_to_cur,
                                                   const glm::vec2& cur_to_next,
                                                   const glm::vec2& prev_dir_to_inside,
                                                   const glm::vec2& next_dir_to_inside,
                                                   const bool polygon_is_clockwise);

    //! Returns the inverse of the factor by which the length of the given unit factor would be increased, when it gets scaled by the given scale factors along the x- and y-axis.
    static float calculateLengthScale(const glm::vec2 &direction, const glm::vec2& scale_factors);

    //! Stroke draw helper function that adds geometry to connect linejoins and linecaps with each other. Creates a connection consisting of 1 quad.
    static void createStrokeSegmentConnection(StrokeSegmentData &stroke_data);

    //! Stroke draw helper function that adds geometry to connect anti-aliased linejoins and linecaps with each other. Creates a connection consisting of 3 quads.
    static void createStrokeSegmentConnectionAA(StrokeSegmentData &stroke_data);

    //! Add the stroke linecap AA geometry
    static void addStrokeLinecapAAGeometryVertices(StrokeSegmentData &stroke_data, const glm::vec2& linecap_left, const glm::vec2& linecap_right,
                                                   const glm::vec2& linecap_fade_left, const glm::vec2& linecap_fade_right);

    //! Stroke draw helper function that adds the linecap depending on linecap type and beginning/end
    static void createStrokeLinecap(StrokeSegmentData& stroke_data,
                                    const SVGImage::SVGImageRenderSettings& render_settings,
                                    const glm::vec2& scale_factors,
                                    const bool is_start);

    //! Helper function to set the stroke-data's last point values
    static void setStrokeDataLastPoints(StrokeSegmentData &stroke_data, const glm::vec2& last_point_left, const glm::vec2& last_point_right);

    //! Helper function to set the stroke-data's last anti-aliased point values
    static void setStrokeDataLastPointsAA(StrokeSegmentData &stroke_data, const glm::vec2& last_point_left,
                                             const glm::vec2& last_point_right, const glm::vec2& last_point_left_fade,
                                             const glm::vec2& last_point_right_fade);

    //! Helper function to set the stroke-data's current point values
    static void setStrokeDataCurrentPoints(StrokeSegmentData &stroke_data, const glm::vec2& current_point_left, const glm::vec2& current_point_right);

    //! Helper function to set the stroke-data's current anti-aliased point values
    static void setStrokeDataCurrentPointsAA(StrokeSegmentData &stroke_data, const glm::vec2& current_point_left,
                                             const glm::vec2& current_point_right, const glm::vec2& current_point_left_fade,
                                             const glm::vec2& current_point_right_fade);

    //! Helper function to set the stroke-data's subsequent point values
    static void setStrokeDataSubsequentPoints(StrokeSegmentData &stroke_data, const glm::vec2& subsequent_point_left, const glm::vec2& subsequent_point_right);

    //! Helper function to set the stroke-data's subsequent anti-aliased point values
    static void setStrokeDataSubsequentPointsAA(StrokeSegmentData &stroke_data, const glm::vec2& subsequent_point_left,
                                             const glm::vec2& subsequent_point_right, const glm::vec2& subsequent_point_left_fade,
                                             const glm::vec2& subsequent_point_right_fade);

    //! Helper function that sets the subsequentPoints as the new lastPoints
    static void setStrokeDataSubsequentPointsAsLastPoints(StrokeSegmentData &stroke_data);

    //! Helper function that sets the anti-aliased subsequentPoints as the new lastPoints
    static void setStrokeDataSubsequentPointsAsLastPointsAA(StrokeSegmentData &stroke_data);

    //! Helper function that sets the lastPoints as the new currentPoints
    static void setStrokeDataLastPointsAsCurrentPoints(StrokeSegmentData &stroke_data);

    //! Helper function that sets the lastPoints as the new currentPoints
    static void setStrokeDataLastPointsAsCurrentPointsAA(StrokeSegmentData &stroke_data);

    //! Stroke helper function that determines if the polygon encompassed by the points is clockwise
    static bool isPolygonClockwise(const glm::vec2& point1,
                                   const glm::vec2& point2,
                                   const glm::vec2& point3);

    //! Helper function that creates and sets the parameters for a coloured geometry buffer
    static void setupGeometryBuffers(std::vector<GeometryBuffer*>& out,
        GeometryBuffer*& fill_geometry_buffer,
        GeometryBuffer*& stroke_geometry_buffer,
        const SVGImage::SVGImageRenderSettings& render_settings,
        const glm::mat3x3& svg_transformation,
        const bool is_fill_needing_stencil);

    //! Helper function for setting an SVG GeometryBuffer's render settings and transformation matrix
    static void setupGeometryBufferSettings(CEGUI::GeometryBuffer* geometry_buffer,
                                            const SVGImage::SVGImageRenderSettings &render_settings,
                                            const glm::mat4& cegui_transformation_matrix);

    //! Turns a matrix as defined by SVG into a matrix that can be used internally by the CEGUI Renderers
    static glm::mat4 createRenderableMatrixFromSVGMatrix(glm::mat3 svg_matrix);

    //! Helper function for getting the fill Colour from an SVGPaintStyle
    static glm::vec4 getFillColour(const SVGPaintStyle& paint_style);

    //! Helper function for getting the stroke Colour from an SVGPaintStyle
    static glm::vec4 getStrokeColour(const SVGPaintStyle& paint_style);

    //! Create the circle's fill
    static void createCircleFill(const std::vector<glm::vec2>& circle_points,
                                 float max_scale,
                                 const SVGPaintStyle& paint_style,
                                 GeometryBuffer& geometry_buffer,
                                 const SVGImage::SVGImageRenderSettings& render_settings,
                                 const glm::vec2& scale_factors);

    //! Calculate the tesselation parameters necessary to calculate the circle points
    static void calculateCircleTesselationParameters(const float radius,
                                                     const float max_scale,
                                                     float& num_segments,
                                                     float& cos_value,
                                                     float& sin_value);

    //! Calculate the circle points
    static void createCirclePoints(const float radius,
                                   const float num_segments,
                                   const float cos_value,
                                   const float sin_value,
                                   std::vector<glm::vec2>& circle_points);

    //! Create the circle's stroke
    static void createCircleStroke(const std::vector<glm::vec2>& circle_points,
                                   float max_scale,
                                   const SVGPaintStyle& paint_style,
                                   GeometryBuffer& geometry_buffer,
                                   const SVGImage::SVGImageRenderSettings& render_settings,
                                   const glm::vec2& scale_factors);

    //! Helper function for creating a triangle strip with filling
    static void createTriangleStripFillGeometry(const std::vector<glm::vec2>& points,
                                                GeometryBuffer& geometry_buffer,
                                                const SVGPaintStyle& paint_style);

    //! Helper function for creating an arc's stroke
    static void createArcStrokeGeometry(std::vector<glm::vec2>& points,
                                        GeometryBuffer& geometry_buffer,
                                        ColouredVertex& stroke_vertex);

    //! Helper function for calculating and creating an anti-aliased stroke for an arc
    static void createArcStrokeAAGeometry(const std::vector<glm::vec2>& points,
                                          const glm::vec2& arc_center_point,
                                          const glm::vec2& arc_draw_origin_point,
                                          StrokeSegmentData& stroke_data,
                                          const glm::vec2& scale_factors,
                                          const bool polygon_is_clockwise,
                                          glm::vec2& linecap_left_AA,
                                          glm::vec2& linecap_right_AA,
                                          glm::vec2& linecap_left_fade,
                                          glm::vec2& linecap_right_fade);

    //! Creates the basic ellipse points
    static void createEllipsePoints(const float radiusX,
                                    const float radiusY,
                                    const float max_scale,
                                    std::vector<glm::vec2>& ellipse_points);

    //! Create the ellipse's fill
    static void createEllipseFill(const std::vector<glm::vec2>& ellipse_points,
                                  const float max_scale,
                                  const SVGPaintStyle& paint_style,
                                  GeometryBuffer& geometry_buffer,
                                  const SVGImage::SVGImageRenderSettings& render_settings,
                                  const glm::vec2& scale_factors);

    //! Create the circle or ellipse's anti-aliased fill points
    static void createCircleOrEllipseFillPointsAA(const std::vector<glm::vec2> &points,
                                                  const glm::vec2 &anti_aliasing_offsets,
                                                  const glm::vec2 &scale_factors,
                                                  std::vector<glm::vec2> &modified_points,
                                                  std::vector<glm::vec2> &fade_points);
    //! Create the ellipse's stroke
    static void createEllipseStroke(const std::vector<glm::vec2>& ellipse_points,
                                    const float max_scale,
                                    const SVGPaintStyle& paint_style,
                                    GeometryBuffer& geometry_buffer,
                                    const SVGImage::SVGImageRenderSettings& render_settings,
                                    const glm::vec2& scale_factors);

    //! Create circle or ellipse stroke points
    static void createCircleOrEllipseStrokePoints(const std::vector<glm::vec2>& points,
                                                  StrokeSegmentData& stroke_data,
                                                  std::vector<glm::vec2>& outer_points,
                                                  std::vector<glm::vec2>& inner_points);

    //! Create anti-aliased circle or ellipse stroke points
    static void createCircleOrEllipseStrokePointsAA(const std::vector<glm::vec2>& points,
                                                    StrokeSegmentData& stroke_data,
                                                    const glm::vec2& scale_factors,
                                                    std::vector<glm::vec2>& outer_points,
                                                    std::vector<glm::vec2>& outer_points_fade,
                                                    std::vector<glm::vec2>& inner_points,
                                                    std::vector<glm::vec2>& inner_points_fade);

    //! Scales the points of an ellipse (originally circle points) so that they match the scaling
    static void scaleEllipsePoints(std::vector<glm::vec2>& circle_points,
                                   const bool isRadiusXBigger,
                                   const float radiusRatio);
    
    //! Helper function for adding an anti-aliasing quad of a stroke to the GeometryBuffer
    static void addStrokeQuadAA(const glm::vec2& point1,
                                const glm::vec2& point2,
                                const glm::vec2& fade_point1,
                                const glm::vec2& fade_point2,
                                GeometryBuffer& geometry_buffer,
                                ColouredVertex& stroke_vertex,
                                ColouredVertex& stroke_fade_vertex);

     //! Helper function for appending a circle fill triangle to a GeometryBuffer
    static void addTriangleGeometry(const glm::vec2& point1,
                                    const glm::vec2& point2,
                                    const glm::vec2& point3,
                                    GeometryBuffer &geometry_buffer,
                                    ColouredVertex &vertex);

    //! Creates the stroke geometry of an arbitrary stroke based on the outer and inner points
    static void createStrokeGeometry(const std::vector<glm::vec2>& outer_circle_points,
                                     const std::vector<glm::vec2>& inner_circle_points,
                                     StrokeSegmentData &stroke_data,
                                     const bool is_surface_closed);

    //! Creates the stroke geometry of an arbitrary anti-aliased stroke, based on the outer and inner points of it
    static void createStrokeGeometryAA(const std::vector<glm::vec2>& outer_points,
                                       const std::vector<glm::vec2>& outer_points_fade,
                                       const std::vector<glm::vec2>& inner_points,
                                       const std::vector<glm::vec2>& inner_points_fade,
                                       StrokeSegmentData &stroke_data,
                                       const bool is_surface_closed);

    //! Creates the anti-aliasing fade fill geometry for an arbitrary anti-aliased fill, based on normal points and fade points
    static void createFillGeometryAAFadeOnly(const std::vector<glm::vec2>& points,
                                             const std::vector<glm::vec2>& points_fade,
                                             const SVGPaintStyle& paint_style,
                                             GeometryBuffer& geometry_buffer,
                                             const bool is_surface_closed);

    //! Calculates the parameters necessary to calculate the arc points
    static void calculateArcTesselationParameters(const float radius,
                                                  const float arc_angle,
                                                  const float max_scale,
                                                  float& num_segments,
                                                  float& tangential_factor,
                                                  float& radial_factor);

    //! Calculates the points of an arc.
    static void createArcPoints(const glm::vec2& center_point,
                                const glm::vec2& start_point,
                                const glm::vec2& end_point,
                                const float num_segments,
                                const float tangential_factor,
                                const float radial_factor,
                                std::vector<glm::vec2>& arc_points);

    //! Function to determine the geometry offsets needed in anti-aliasing, depending on the width
    static void determineAntiAliasingOffsets(float width, glm::vec2& antialiasing_offsets);

    //! Helper function to determine the scale factors in x and y-direction based on the transformation matrix and the image scale
    static glm::vec2 determineScaleFactors(const glm::mat3& transformation, const SVGImage::SVGImageRenderSettings& render_settings);

    //! Intersects two lines and returns the result. Also the intersection point will be given if there is an intersection..
    static LineIntersectResult intersectLines(const glm::vec2& line1_start, const glm::vec2& line1_end,
                                              const glm::vec2& line2_start, const glm::vec2& line2_end,
                                              glm::vec2& intersection);

    //! Create the rectangles fill
    static void createRectangleFill(const SVGPaintStyle& paint_style, std::vector<glm::vec2>& rectangle_points, GeometryBuffer& geometry_buffer);


    //! Helper function to get the min and max x and y coordinates of a list of points
    static void calculateMinMax(const std::vector<glm::vec2>& points, glm::vec2& min, glm::vec2& max);

    //! Helper function to append a fill-quad based on its 4 corner points to the Geometrybuffer
    static void addFillQuad(const glm::vec2& point1,
                            const glm::vec2& point2,
                            const glm::vec2& point3,
                            const glm::vec2& point4,
                            GeometryBuffer& geometry_buffer,
                            ColouredVertex& fill_vertex);

    //! Helper function to append a stroke-quad based on its 4 corner points to the Geometrybuffer
    static void addStrokeQuad(const glm::vec2& point1,
                              const glm::vec2& point2,
                              const glm::vec2& point3,
                              const glm::vec2& point4,
                              GeometryBuffer& geometry_buffer,
                              ColouredVertex& stroke_vertex);

    //! Helper function to append a triangle fan, which is based on a list of points, to the Geometrybuffer
    static void addTriangleFanGeometry(const std::vector<glm::vec2> &points, 
                                       GeometryBuffer& geometry_buffer,
                                       ColouredVertex& coloured_vertex);
};

}

#endif

