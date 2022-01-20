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
#include "CEGUI/CoordConverter.h"

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
    const ImageRenderSettings& renderSettings) const
{
    TexturedColouredVertex vbuffer[6];
    if (createVertices(vbuffer, renderSettings))
    {
        auto& buffer = System::getSingleton().getRenderer()->createGeometryBufferTextured();
        buffer.setClippingActive(!!renderSettings.d_clipArea);
        if (renderSettings.d_clipArea)
            buffer.setClippingRegion(*renderSettings.d_clipArea);
        buffer.setTexture("texture0", d_texture);
        buffer.setAlpha(renderSettings.d_alpha);

        buffer.appendGeometry(vbuffer, 6);

        out.push_back(&buffer);
    }
}

//----------------------------------------------------------------------------//
void BitmapImage::addToRenderGeometry(GeometryBuffer& geomBuffer,
    const ImageRenderSettings& renderSettings) const
{
    TexturedColouredVertex vbuffer[6];
    if (createVertices(vbuffer, renderSettings))
        geomBuffer.appendGeometry(vbuffer, 6);
}

//----------------------------------------------------------------------------//
bool BitmapImage::createVertices(TexturedColouredVertex* out, const ImageRenderSettings& renderSettings) const
{
    const Rectf finalRect = calculateRenderArea(renderSettings);
    if (finalRect.empty())
        return false;

    const Rectf& dest = renderSettings.d_destArea;
    const glm::vec2 tex_per_pix(d_imageArea.getWidth() / dest.getWidth(), d_imageArea.getHeight() / dest.getHeight());
    const Rectf destWithOffset(dest.d_min + d_scaledOffset, dest.d_max + d_scaledOffset);
    const Rectf texRect((d_imageArea + ((finalRect - destWithOffset) * tex_per_pix)) * d_texture->getTexelScaling());

    // vertex 0
    out[0].setColour(renderSettings.d_multiplyColours.d_top_left);
    out[0].d_position = glm::vec3(finalRect.left(), finalRect.top(), 0.0f);
    out[0].d_texCoords = glm::vec2(texRect.left(), texRect.top());

    // vertex 1
    out[1].setColour(renderSettings.d_multiplyColours.d_bottom_left);
    out[1].d_position = glm::vec3(finalRect.left(), finalRect.bottom(), 0.0f);
    out[1].d_texCoords = glm::vec2(texRect.left(), texRect.bottom());

    // vertex 2
    out[2].setColour(renderSettings.d_multiplyColours.d_bottom_right);
    out[2].d_position.x = finalRect.right();
    out[2].d_position.z = 0.0f;
    out[2].d_texCoords.x = texRect.right();

    // Quad splitting done from top-left to bottom-right diagonal
    out[2].d_position.y = finalRect.bottom();
    out[2].d_texCoords.y = texRect.bottom();

    // vertex 3
    out[3].setColour(renderSettings.d_multiplyColours.d_top_right);
    out[3].d_position = glm::vec3(finalRect.right(), finalRect.top(), 0.0f);
    out[3].d_texCoords = glm::vec2(texRect.right(), texRect.top());

    // vertex 4
    out[4].setColour(renderSettings.d_multiplyColours.d_top_left);
    out[4].d_position.x = finalRect.left();
    out[4].d_position.z = 0.0f;
    out[4].d_texCoords.x = texRect.left();

    // Quad splitting done from top-left to bottom-right diagonal
    out[4].d_position.y = finalRect.top();
    out[4].d_texCoords.y = texRect.top();

    // vertex 5
    out[5].setColour(renderSettings.d_multiplyColours.d_bottom_right);
    out[5].d_position = glm::vec3(finalRect.right(), finalRect.bottom(), 0.0f);
    out[5].d_texCoords = glm::vec2(texRect.right(), texRect.bottom());

    return true;
}

}
