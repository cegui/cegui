/***********************************************************************
    filename:   SVGData.cpp
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
#include "CEGUI/svg/SVGData.h"

#include "CEGUI/svg/SVGBasicShape.h"


// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
SVGData::SVGData()
{
}

//----------------------------------------------------------------------------//
SVGData::~SVGData()
{
    destroyShapes();
}

//----------------------------------------------------------------------------//
void SVGData::addShape(SVGBasicShape* svg_shape)
{
    d_svgBasicShapes.push_back(svg_shape);
}

//----------------------------------------------------------------------------//
void SVGData::destroyShapes()
{
    const unsigned int shape_count = d_svgBasicShapes.size();
    for(unsigned int i = 0; i < shape_count; ++i)
        delete d_svgBasicShapes[i];

    d_svgBasicShapes.clear();
}

//----------------------------------------------------------------------------//
const std::vector<SVGBasicShape*>& SVGData::getShapes() const
{
    return d_svgBasicShapes;
}


//----------------------------------------------------------------------------//
}

