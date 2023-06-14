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
#include "CEGUI/BitmapImage.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/System.h" // this being here is a bit nasty IMO
#include <algorithm>

namespace CEGUI
{
const String ImageTypeAttribute( "type" );
const String ImageNameAttribute( "name" );
const String ImageTextureAttribute( "texture" );
const String ImageXPosAttribute( "xPos" );
const String ImageYPosAttribute( "yPos" );
const String ImageWidthAttribute( "width" );
const String ImageHeightAttribute( "height" );
const String ImageXOffsetAttribute( "xOffset" );
const String ImageYOffsetAttribute( "yOffset" );
const String ImageAutoScaledAttribute( "autoScaled" );
const String ImageNativeHorzResAttribute( "nativeHorzRes" );
const String ImageNativeVertResAttribute( "nativeVertRes" );

//----------------------------------------------------------------------------//
BitmapImage::BitmapImage(const String& name) :
    Image(name)
{
}

//----------------------------------------------------------------------------//
BitmapImage::BitmapImage(const XMLAttributes& attributes)
    : Image(attributes.getValueAsString(ImageNameAttribute),
          glm::vec2(static_cast<float>(attributes.getValueAsInteger(ImageXOffsetAttribute, 0)),
                    static_cast<float>(attributes.getValueAsInteger(ImageYOffsetAttribute, 0))),
          Rectf(glm::vec2(static_cast<float>(attributes.getValueAsInteger(ImageXPosAttribute, 0)),
                          static_cast<float>(attributes.getValueAsInteger(ImageYPosAttribute, 0))),
                Sizef(static_cast<float>(attributes.getValueAsInteger(ImageWidthAttribute, 0)),
                      static_cast<float>(attributes.getValueAsInteger(ImageHeightAttribute, 0)))),
          PropertyHelper<AutoScaledMode>::fromString(attributes.getValueAsString(ImageAutoScaledAttribute)),
          Sizef(static_cast<float>(attributes.getValueAsInteger(ImageNativeHorzResAttribute, 640)),
                static_cast<float>(attributes.getValueAsInteger(ImageNativeVertResAttribute, 480)))),
    d_texture(&System::getSingleton().getRenderer()->getTexture(
              attributes.getValueAsString(ImageTextureAttribute)))
{
}

//----------------------------------------------------------------------------//
BitmapImage::BitmapImage(const String& name, Texture* texture,
                       const Rectf& pixel_area, const glm::vec2& pixel_offset,
                       const AutoScaledMode autoscaled, const Sizef& native_res)
    : Image(name, pixel_offset, pixel_area, autoscaled, native_res)
    , d_texture(texture)
{}

//----------------------------------------------------------------------------//
void BitmapImage::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const ImageRenderSettings& renderSettings, size_t canCombineFromIdx) const
{
    TexturedColouredVertex vbuffer[6];
    if (!createVertices(vbuffer, renderSettings))
        return;

    // Try to find an existing buffer suitable for combining. Note that we
    // don't check the whole 'out' because geometry ordering may be important.
    GeometryBuffer* buffer = nullptr;
    if (canCombineFromIdx < out.size())
    {
        // TODO: need more checks than just a texture? Clipping region, alpha.
        auto it = std::find_if(out.cbegin() + canCombineFromIdx, out.cend(),
            [tex = d_texture](const GeometryBuffer* buffer)
        {
            return tex == buffer->getMainTexture();
        });
        if (it != out.end())
            buffer = *it;
    }

    if (!buffer)
    {
        buffer = &System::getSingleton().getRenderer()->createGeometryBufferTextured();
        buffer->setClippingActive(!!renderSettings.d_clipArea);
        if (renderSettings.d_clipArea)
            buffer->setClippingRegion(*renderSettings.d_clipArea);
        buffer->setMainTexture(d_texture);
        buffer->setAlpha(renderSettings.d_alpha);
        out.push_back(buffer);
    }

    buffer->appendGeometry(vbuffer, 6);
}

//----------------------------------------------------------------------------//
bool BitmapImage::createVertices(TexturedColouredVertex* out, const ImageRenderSettings& renderSettings) const
{
    Rectf destRect = renderSettings.d_destArea;
    destRect.offset(d_scaledOffset);

    if (renderSettings.d_alignToPixels)
    {
        destRect.round();

        // Rounding might shrink a very small rect into an empty one
        if (destRect.empty())
            return false;
    }

    // When not clipped, we draw the whole image
    Rectf texRect = d_imageArea;

    // Apply clipping if needed
    if (renderSettings.d_clipArea)
    {
        const Rectf unclippedDestRect = destRect;
        if (destRect.intersect(*renderSettings.d_clipArea))
        {
            if (destRect.empty())
                return false;

            const glm::vec2 scaleDestToImg(
                d_imageArea.getWidth() / unclippedDestRect.getWidth(),
                d_imageArea.getHeight() / unclippedDestRect.getHeight());

            texRect += (destRect - unclippedDestRect) * scaleDestToImg;
        }
    }

    // Turn pixels into normalized texture coords
    texRect *= d_texture->getTexelScaling();

    // vertex 0
    out[0].setColour(renderSettings.d_multiplyColours.d_top_left);
    out[0].d_position = glm::vec3(destRect.left(), destRect.top(), 0.0f);
    out[0].d_texCoords = glm::vec2(texRect.left(), texRect.top());

    // vertex 1
    out[1].setColour(renderSettings.d_multiplyColours.d_bottom_left);
    out[1].d_position = glm::vec3(destRect.left(), destRect.bottom(), 0.0f);
    out[1].d_texCoords = glm::vec2(texRect.left(), texRect.bottom());

    // vertex 2
    out[2].setColour(renderSettings.d_multiplyColours.d_bottom_right);
    out[2].d_position.x = destRect.right();
    out[2].d_position.z = 0.0f;
    out[2].d_texCoords.x = texRect.right();

    // Quad splitting done from top-left to bottom-right diagonal
    out[2].d_position.y = destRect.bottom();
    out[2].d_texCoords.y = texRect.bottom();

    // vertex 3
    out[3].setColour(renderSettings.d_multiplyColours.d_top_right);
    out[3].d_position = glm::vec3(destRect.right(), destRect.top(), 0.0f);
    out[3].d_texCoords = glm::vec2(texRect.right(), texRect.top());

    // vertex 4
    out[4].setColour(renderSettings.d_multiplyColours.d_top_left);
    out[4].d_position.x = destRect.left();
    out[4].d_position.z = 0.0f;
    out[4].d_texCoords.x = texRect.left();

    // Quad splitting done from top-left to bottom-right diagonal
    out[4].d_position.y = destRect.top();
    out[4].d_texCoords.y = texRect.top();

    // vertex 5
    out[5].setColour(renderSettings.d_multiplyColours.d_bottom_right);
    out[5].d_position = glm::vec3(destRect.right(), destRect.bottom(), 0.0f);
    out[5].d_texCoords = glm::vec2(texRect.right(), texRect.bottom());

    return true;
}

}
