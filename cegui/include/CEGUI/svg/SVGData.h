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
    void addShape(SVGBasicShape& svg_shape);

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

    /*!
    \brief
        Returns the SVGData's width in pixels.
    \return
        The SVGData's width in pixels.
    */
    float getWidth() const;

    /*!
    \brief
        Sets the SVGData's width in pixels.
    \param width
        The width in pixels.
    */
    void setWidth(float width);

    /*!
    \brief
        Returns the SVGData's height in pixels.
    \return
        The SVGData's height in pixels.
    */
    float getHeight() const;

    /*!
    \brief
        Sets the SVGData's height in pixels.
    \param width
        The height in pixels.
    */
    void setHeight(float height);

protected:
    /*!
    \brief
        The SVGData's width in pixels.
        
        This is the value representing the intrinsic width of the 'SVG document fragment'. It is used in CEGUI
        to determine the clipping area of the SVG image and to scale the image elements in case the Image is
        rendered with horizontal stretching.
    */
    float d_width;
    /*!
    \brief
        The SVGData's height in pixels.
        
        This is the value representing the intrinsic height of the 'SVG document fragment'. It is used in CEGUI
        to determine the clipping area of the SVG image and to scale the image elements in case the Image is
        rendered with vertical stretching.
    */
    float d_height;

    //! The basic shapes that were added to the SVGData
    std::vector<SVGBasicShape*> d_svgBasicShapes;

};

}


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif

