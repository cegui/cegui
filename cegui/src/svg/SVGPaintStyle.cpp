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
#include "CEGUI/svg/SVGPaintStyle.h"


// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
SVGPaint::SVGPaint() :
    d_none(false),
    d_colour(0.0f, 0.0f, 0.0f)
{
}

//----------------------------------------------------------------------------//
SVGPaintStyle::SVGPaintStyle() :
    d_fill(),
    d_fillRule(PolygonFillRule::NonZero),
    d_fillOpacity(1.0f),
    d_stroke(),
    d_strokeWidth(1.0f),
    d_strokeLinecap(SvgLinecap::Butt),
    d_strokeLinejoin(SVGLinejoin::Miter),
    d_strokeMiterlimit(4.0f),
    d_strokeDashArrayNone(false),
    d_strokeDashArray(),
    d_strokeDashOffset(0.0f),
    d_strokeOpacity(1.0f)
{
}


//----------------------------------------------------------------------------//
}

