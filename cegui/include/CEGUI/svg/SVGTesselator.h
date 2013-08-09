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
#include "CEGUI/svg/SVGImage.h"

#include <vector>

namespace CEGUI
{
class GeometryBuffer;
class SVGPolyline;
class SVGRect;

/*!
\brief
    Defines a static class that provides helper functions for the tesselation of
    SVGBasicShapes.
*/
class CEGUIEXPORT SVGTesselator
{
public:
    
    /*!
    \brief
        Tesselates a SVG Polyline and adds the created geometry to the GeometryBuffer
        list.

\param polyline
            The SVG Polyline object that contains the data.
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
        Tesselates a SVG Rect and adds the created geometry to the GeometryBuffer
        list.
    
        \param rect
            The SVG Polyline object that contains the data.
        \param geometry_buffers
            The GeometryBuffer list to which the created geometry will be added.
        \param render_settings
            The ImageRenderSettings for the geometry that will be created.
    */
    static void tesselateAndRenderRect(const SVGRect* rect,
                                       std::vector<GeometryBuffer*>& geometry_buffers,
                                       const SVGImage::SVGImageRenderSettings& render_settings);

private:
    //! Constructor.
    SVGTesselator();

    //! Destructor.
    ~SVGTesselator();
};

}

#endif

