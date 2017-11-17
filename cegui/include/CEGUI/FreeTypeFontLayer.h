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

#include "CEGUI/ColourRect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Enumerated type used when specifying freetype font layer type
 */
enum class FontLayerType : int
{
    /**
     * Standard rendering of a font layer with no outline
     */
    Standard,
    /**
     * Render a font layer with an outline
     */
    Outline,
    /**
     * Render a font layer inside font border
     */
    Inner,
    /**
     * Render a font layer inside font border
     */
    Outer
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
    //! defines colour of the layer
    ColourRect d_colours;

    /*!
        \brief
            Constructor for FreeTypeFontLayer.

        \param fontLayerType
            The type of layer that will be rendered by freetype
    
        \param coloursRect
            Specifies the colour to render the font layer.

        \param outlinePixels
            Specifies the pixel size of the outline (only applies if fontLayerType is not Standard).
        */
    FreeTypeFontLayer(FontLayerType fontLayerType = FontLayerType::Standard,
        ColourRect coloursRect = ColourRect(Colour(1,1,1)),
        unsigned int outlinePixels = 1)
    : d_fontLayerType(fontLayerType), d_colours (coloursRect), d_outlinePixels (outlinePixels)
    {
    }
    virtual ~FreeTypeFontLayer() {}
};

typedef std::vector<FreeTypeFontLayer> FreeTypeFontLayerVector;

} // End of  CEGUI namespace section

#endif //_CEGUIFreeTypeFontLayer_h_


