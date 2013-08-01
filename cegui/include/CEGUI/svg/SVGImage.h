/***********************************************************************
    filename:   SVGImage.h
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
#ifndef _SVGImage_h_
#define _SVGImage_h_

#include "CEGUI/Image.h"

// Start of CEGUI namespace section
namespace CEGUI
{
class SVGData;

/*!
\brief
    Defines the SVGImage class, which describes a vector graphics image that can be
    rendered by CEGUI.

*/
class CEGUIEXPORT SVGImage :
    public Image
{
public:
    SVGImage(const String& name);
    SVGImage(const String& name, SVGData* svg_data);
    SVGImage(const XMLAttributes& attributes);

    // Implement CEGUI::Image interface
    void render(GeometryBuffer& buffer,
                const Rectf& dest_area,
                const Rectf* clip_area,
                const bool clipping_enabled,
                const ColourRect& colours) const;

    /*!
    \brief
        Sets the SVGData object of this SVGImage.

    \param texture
        The pointer to the SVGData object to be used by this SVGImage. Can be 0.
    */
    void setSVGData(SVGData* svg_Data);

    /*!
    \brief
        Returns the pointer to the SVGData object of this SVGImage.

    \return
        The pointer to the SVGData object of this SVGImage. Can be 0.
    */
    SVGData* getSVGData();

protected:
    /*!
        \brief
        Reference to the SVGData used as basis for drawing. The SVGData can be shared
        between SVGImages. Changing the SVGData object will result in changes between all
        SVGImages.
    */
    SVGData* d_svgData;
};

}

#endif

