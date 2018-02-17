/***********************************************************************
    created:    Wed Feb 16 2011
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIBitmapImage_h_
#define _CEGUIBitmapImage_h_

#include "CEGUI/Image.h"
#include "CEGUI/String.h"
#include "CEGUI/Rectf.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT BitmapImage : public Image
{
public:
    BitmapImage(const String& name);
    BitmapImage(const XMLAttributes& attributes);

    BitmapImage(const String& name, Texture* texture,
               const Rectf& tex_area, const glm::vec2& offset,
               const AutoScaledMode autoscaled, const Sizef& native_res);

    std::vector<GeometryBuffer*> createRenderGeometry(
        const ImageRenderSettings& render_settings) const override;

    void addToRenderGeometry(
        GeometryBuffer& geomBuffer,
        const Rectf& renderArea,
        const Rectf* clipArea,
        const ColourRect& colours) const override;

    /*!
    \brief
        Sets the Texture object of this Image.

    \param texture
        The pointer to the Texture of this Image.
    */
    void setTexture(Texture* texture);

    /*!
    \brief
        Returns the pointer to the Texture object used by this image.

    \return
        The pointer to the Texture object used by this image.
    */
    const Texture* getTexture() const;

protected:
    /*!
    \brief
        Fills the vertices data for the textured quad based on the supplied
        parameters. The supplied pointer must point to an array of size 6
        for the quad.
    */
    void createTexturedQuadVertices(
        TexturedColouredVertex* vbuffer,
        const CEGUI::ColourRect& colours,
        Rectf& finalRect,
        const Rectf& texRect) const;

    /*!
    \brief
        Helper function for calculating the tecture and render areas.
    */
    bool calculateTextureAreaAndRenderArea(
        const Rectf& renderSettingDestArea,
        const Rectf* clippingArea,
        Rectf &finalRect, Rectf &texRect) const;

    //! Texture used by this image.
    Texture* d_texture;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIBitmapImage_h_

