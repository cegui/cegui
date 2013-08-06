/***********************************************************************
    filename:   SVGShapeStyle.h
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

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Defines the 'paint' type based on how it is used in SVG. Main purpose is for defining how
    'fill' and 'stroke' are to be drawn.
*/
class CEGUIEXPORT SVGPaint : public AllocatedObject<SVGPaint>
{
public:
    bool d_none;
    CEGUI::Colour d_colour;
};

/*!
\brief
    Defines the SVGShapeStyle class, which describes the overall shape style of an SVG shape.
*/
class CEGUIEXPORT SVGShapeStyle : public AllocatedObject<SVGShapeStyle>
{
public:
    SVGShapeStyle();

    //! The 'stroke' property defines how the stroke of a graphical element must be painted.
    SVGPaint d_stroke;
    //! The 'fill' property defines how the interior of a graphical element must be painted.
    SVGPaint d_fill;
    /*!
    \brief
        The width of the stroke which shall be used on the current object.
        No stroke shall be painted for a zero value. A negative value is unsupported and must be
        treated as if the stroke had not been specified. (default = 1.0)
    */
    float d_strokeWidthLength;
    /*!
    \brief
        The opacity setting that will be applied to the shape. Values must be inside range 0.0 
        (fully transparent) to 1.0 (fully opaque). (default = 1.0)
    */
    float d_opacityValue;
};

}

#endif

