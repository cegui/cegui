/***********************************************************************
    filename:   SVGImage.cpp
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
#include "CEGUI/svg/SVGImage.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/svg/SVGTesselator.h"
#include "CEGUI/svg/SVGData.h"


// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
SVGImage::SVGImage(const String& name) :
    Image(name),
    d_svgData(0)
{
}

//----------------------------------------------------------------------------//
SVGImage::SVGImage(const String& name, SVGData* svg_data) :
    Image(name),
    d_svgData(svg_data)
{
}

//----------------------------------------------------------------------------//
SVGImage::SVGImage(const XMLAttributes& attributes) :
    Image("") //TODO define and fill XML elements
{
}

//----------------------------------------------------------------------------//
void SVGImage::setSVGData(SVGData* svg_Data)
{
    d_svgData = svg_Data;
}

//----------------------------------------------------------------------------//
SVGData* SVGImage::getSVGData()
{
    return d_svgData;
}

//----------------------------------------------------------------------------//
void SVGImage::render(GeometryBuffer& buffer, const Rectf& dest_area,
                        const Rectf* clip_area, const bool clipping_enabled,
                        const ColourRect& colours) const
{
    buffer.setClippingActive(clipping_enabled);
    if(clipping_enabled)
        buffer.setClippingRegion(*clip_area);

    const std::vector<SVGBasicShape*>& shapes = d_svgData->getShapes();
    //SVGTesselator::tesselateAndAddPolyline(buffer, 
}

//----------------------------------------------------------------------------//
}

