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
#ifndef _SVGShapeStyle_h_
#define _SVGShapeStyle_h_

#include "CEGUI/Base.h"
#include "CEGUI/Colour.h"
#include "CEGUI/GeometryBuffer.h"
#include <glm/glm.hpp>

#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Defines the 'paint' type based on how it is used in SVG. Main purpose is for defining how
    'fill' and 'stroke' are to be drawn.
*/
class CEGUIEXPORT SVGPaint
{
public:
    SVGPaint();

    //! Defines if the paint is to be drawn at all.
    bool d_none;
    //! Defines the colour of the paint.
    glm::vec3 d_colour;
};

/*!
\brief
    Defines the SVGShapeStyle class, which describes the overall shape style of an SVG shape.
*/
class CEGUIEXPORT SVGPaintStyle
{
public:
    /*!
    \brief
        Specifies the shape which shall be used at the end of open subpaths when they are stroked.
    */
    enum class SvgLinecap : int
    {
        //! A simple linear cap through the endpoint.
        Butt,
        //! A rounded cap with the endpoint as center.
        Round,
        //! A simple linear cap that is offset from the endpoint by the stroke width.
        Square,

        Count
    };

    /*!
    \brief
        Specifies the shape which shall be used at the corners of shapes when they are stroked. 
    */
    enum class SVGLinejoin : int
    {
        /*!
        \brief
            Makes two lines join at the intersection points of their outlines. This can be influenced
            by the value set for stroke-miterlimit. In the case the miter is exceedingly long a bevel
            linejoin will then be used for that corner.
        */
        Miter,
        //! A rounded linejoin.
        Round,
        //! A linejoin with two corners.
        Bevel,

        Count
    };


    SVGPaintStyle();

    //! The 'fill' property defines how the interior of a graphical element must be painted.
    SVGPaint d_fill;

    /*!
    \brief
        The 'fill-rule' property indicates the algorithm which must be used to determine what parts
        of the canvas are included inside the shape.
    */
    PolygonFillRule d_fillRule;

    /*!
    \brief
        The opacity setting that will be applied to the fill. Values must be inside range 0.0 
        (fully transparent) to 1.0 (fully opaque). Default is 1.0.
    */
    float d_fillOpacity;

    //! The 'stroke' property defines how the stroke of a graphical element must be painted.
    SVGPaint d_stroke;

    /*!
    \brief
        The width of the stroke which shall be used on the current object.
        No stroke shall be painted for a zero value. A negative value is unsupported and must be
        treated as if the stroke had not been specified. Default is 1.0.
    */
    float d_strokeWidth;

    /*!
    \brief
        Specifies the shape which shall be used at the end of open subpaths when they are stroked.
    */
    SvgLinecap d_strokeLinecap;

    /*!
    \brief
        Specifies the shape which shall be used at the corners of shapes when they are stroked.
    */
    SVGLinejoin d_strokeLinejoin;

    /*!
    \brief
        When two line segments meet at a sharp angle and miter joins have been specified for 'stroke-linejoin',
        it is possible for the miter to extend far beyond the thickness of the line stroking the path. The 
        'stroke-miterlimit' imposes a limit on the ratio of the miter length to the 'stroke-width'. When the
        limit is exceeded, the join must be converted from a miter to a bevel.
        The limit on the ratio of the miter length to the 'stroke-width'. The value of <miterlimit> must be a
        number greater than or equal to 1. Any other value shall be treated as unsupported and processed as if
        the property had not been specified. 
    */
    float d_strokeMiterlimit;

    /*!
    \brief
        Indicates if dashing shall be used. If stroked and dash array is set to none, the line must be drawn solid. 
    */
    bool d_strokeDashArrayNone;

    /*!
    \brief
        Specifies the pattern of dashes and gaps that shall be used to stroke paths.
    */
    std::vector<float> d_strokeDashArray;

    /*!
    \brief
        Specifies the distance into the dash pattern that must be used to start the dash.
    */
    float d_strokeDashOffset;

    /*!
    \brief
        The opacity setting that will be applied to the stroke. Values must be inside range 0.0 
        (fully transparent) to 1.0 (fully opaque). Default is 1.0.
    */
    float d_strokeOpacity;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif

