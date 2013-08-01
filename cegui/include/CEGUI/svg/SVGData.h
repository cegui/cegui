/***********************************************************************
    filename:   SVGData.h
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
#ifndef _SVGData_h_
#define _SVGData_h_

#include "CEGUI/Base.h"

#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


namespace CEGUI
{
class SVGBasicShape;

/*!
\brief
    Defines a class for the SVG data.

    The SVGData class stores data of a vector graphics image based on the SVG Tiny 1.2
    file standard. The SVGData is utilised by the SVGImage class.

    The data for this class can be either be created by parsing it from an SVG file
    or be added manually by a user to draw custom geometry.
*/
class CEGUIEXPORT SVGData : public AllocatedObject<SVGData>
{
public:
    SVGData();

    ~SVGData();

    /*!
    \brief
        Adds a SVGBasicShape to the list of shapes of this class. This class takes ownership
        of the object and will free the memory itself.
    \param svg_shape
        The SVGBasicShape that will be added.
    */
    void addShape(SVGBasicShape* svg_shape);

    /*!
    \brief
        Deletes all shapes in the list and clears the list.
    */
    void destroyShapes();

    /*!
    \brief
        Returns the list of shapes of this class.
    \return
        The list of shapes of this class.
    */
    const std::vector<SVGBasicShape*>& getShapes() const;

protected:
    //! The basic shapes that were added to the SVGData
    std::vector<SVGBasicShape*> d_svgBasicShapes;

};

}


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif

