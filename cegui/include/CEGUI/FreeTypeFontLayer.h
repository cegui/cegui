/***********************************************************************
	created:	10/17/2017
	author:		David Reepmeyer

	purpose:        Implement font outline feature via FreeType library
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIFreeTypeFontLayer_h_
#define _CEGUIFreeTypeFontLayer_h_

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Enumerated type used when specifying freetype font layer type
 */
enum class FontLayerType : int
{
    //! Standard rendering of a font layer with no outline
    Standard,
    //! Render a font layer with an outline
    Outline,
    //! Render a font layer inside font border
    Inner,
    //! Render a font layer inside font border
    Outer
};
/*!
\brief
    These values determine how the end of opened sub-paths are rendered in a stroke.
 */
enum class FreeTypeLineCap : int
{
    //! The end of lines is rendered as a full stop on the last point itself
    Butt,
    //! The end of lines is rendered as a half-circle around the last point
    Round,
    //! The end of lines is rendered as a square around the last point
    Square
};
enum class FreeTypeLineJoin : int
{
    //! Used to render rounded line joins. Circular arcs are used to join two lines smoothly
    Round,
    /*!
    Used to render beveled line joins. The outer corner of the joined lines is filled by
    enclosing the triangular region of the corner with a straight line between the outer
    corners of each stroke.
    */
    Bevel,
    /*!
    Used to render mitered line joins, with variable bevels if the miter limit is exceeded.
    The intersection of the strokes is clipped at a line perpendicular to the bisector of the
    angle between the strokes, at the distance from the intersection of the segments equal to
    the product of the miter limit value and the border radius. This prevents long spikes being
    created.
    */
    MiterVariable,
    /*!
    Used to render mitered line joins, with fixed bevels if the miter limit is exceeded. The outer
    edges of the strokes for the two segments are extended until they meet at an angle. If the
    segments meet at too sharp an angle (such that the miter would extend from the intersection
    of the segments a distance greater than the product of the miter limit value and the border
    radius), then a bevel join (see above) is used instead. This prevents long spikes being created.
    */
    MiterFixed
};

/*!
\brief
    stores the font layer information for a freetype font that will help for defining
    different layers such as outlines
 */
class FreeTypeFontLayer
{
public:
    //! defines how to render the layer type via freetype
    FontLayerType d_fontLayerType;
    //! defines how many pixels to render the layer via freetype if it is an outline
    unsigned int d_outlinePixels;
    //! defines how the end of opened sub-paths are rendered in a stroke
    FreeTypeLineCap d_lineCap;
    //! these values determine how two joining lines are rendered in a stroker
    FreeTypeLineJoin d_lineJoin;
    //! freetype miter limit for MiterVariable and MiterFixed line join styles
    unsigned int d_miterLimit;
    /*!
        \brief
            Constructor for FreeTypeFontLayer.

        \param fontLayerType
            The type of layer that will be rendered by freetype
    
        \param outlinePixels
            Specifies the pixel size of the outline (only applies if fontLayerType is not Standard).

        \param lineCap
            Defines how the end of opened sub-paths are rendered in a stroke

        \param lineJoin
            Determine how two joining lines are rendered in a stroker

        \param miterLimit
            freetype miter limit for MiterVariable and MiterFixed line join styles
        */
    FreeTypeFontLayer(FontLayerType fontLayerType = FontLayerType::Standard,
        unsigned int outlinePixels = 1,
        FreeTypeLineCap lineCap = FreeTypeLineCap::Round,
        FreeTypeLineJoin lineJoin= FreeTypeLineJoin::Round,
        unsigned int miterLimit = 0)
    : d_fontLayerType(fontLayerType), d_outlinePixels (outlinePixels),
    d_lineCap(lineCap), d_lineJoin (lineJoin), d_miterLimit(miterLimit)
    {
    }
    virtual ~FreeTypeFontLayer() = default;
};

typedef std::vector<FreeTypeFontLayer> FreeTypeFontLayerVector;

} // End of  CEGUI namespace section

#endif //_CEGUIFreeTypeFontLayer_h_


