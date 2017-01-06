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
#include "CEGUI/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/System.h" // this being here is a bit nasty IMO
#include "CEGUI/CoordConverter.h"

// Start of CEGUI namespace section
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
    Image(name),
    d_texture(nullptr)
{
}

//----------------------------------------------------------------------------//
BitmapImage::BitmapImage(const XMLAttributes& attributes) :
    Image(attributes.getValueAsString(ImageNameAttribute),
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
                       const AutoScaledMode autoscaled, const Sizef& native_res) :
    Image(name,
          pixel_offset,
          pixel_area,
          autoscaled,
          native_res),
    d_texture(texture)
{}

//----------------------------------------------------------------------------//
void BitmapImage::setTexture(Texture* texture)
{
    d_texture = texture;
}

//----------------------------------------------------------------------------//
std::vector<GeometryBuffer*> BitmapImage::createRenderGeometry(const ImageRenderSettings& render_settings) const
{
    Rectf texRect;
    Rectf finalRect;

    bool isFullClipped = calculateTextureAreaAndRenderArea(
        render_settings.d_destArea, render_settings.d_clipArea,
        finalRect, texRect);

    if(isFullClipped)
    {
        return std::vector<GeometryBuffer*>();
    }

    TexturedColouredVertex vbuffer[6];
    const CEGUI::ColourRect& colours = render_settings.d_multiplyColours;

    createTexturedQuadVertices(vbuffer, colours, finalRect, texRect);


    CEGUI::GeometryBuffer& buffer = System::getSingleton().getRenderer()->createGeometryBufferTextured();

    buffer.setClippingActive(render_settings.d_clippingEnabled);
    if(render_settings.d_clippingEnabled)
        buffer.setClippingRegion(*render_settings.d_clipArea);
    buffer.setTexture("texture0", d_texture);
    buffer.appendGeometry(vbuffer, 6);
    buffer.setAlpha(render_settings.d_alpha);

    std::vector<GeometryBuffer*> geomBuffers;
    geomBuffers.push_back(&buffer);
    return geomBuffers;
}


void BitmapImage::addToRenderGeometry(
    GeometryBuffer& geomBuffer,
    const Rectf& renderArea,
    const Rectf* clipArea,
    const ColourRect& colours) const
{
    Rectf texRect;
    Rectf finalRect;

    bool isFullClipped = calculateTextureAreaAndRenderArea(
        renderArea, clipArea,
        finalRect, texRect);

    if (isFullClipped)
    {
        return;
    }
    
    TexturedColouredVertex vbuffer[6];
    createTexturedQuadVertices(vbuffer, colours, finalRect, texRect);

    geomBuffer.appendGeometry(vbuffer, 6);
}


bool BitmapImage::calculateTextureAreaAndRenderArea(
    const Rectf& renderSettingDestArea,
    const Rectf* clippingArea,
    Rectf& finalRect, Rectf& texRect) const
{
    Rectf dest(renderSettingDestArea);
    // apply rendering offset to the destination Rect
    dest.offset(d_scaledOffset);

    // get the rect area that we will actually draw to (i.e. perform clipping)
    finalRect = Rectf(clippingArea ? dest.getIntersection(*clippingArea) : dest);

    // check if rect was totally clipped
    if ((finalRect.getWidth() == 0) || (finalRect.getHeight() == 0))
        return true;

    // Obtain correct scale values from the texture
    const glm::vec2& texel_scale = d_texture->getTexelScaling();
    const glm::vec2 tex_per_pix(d_imageArea.getWidth() / dest.getWidth(), d_imageArea.getHeight() / dest.getHeight());

    // calculate final, clipped, texture co-ordinates
    texRect = Rectf((d_imageArea + ((finalRect - dest) * tex_per_pix)) * texel_scale);

    // Positions have to be rounded because the glyphs images have to be grid-aligned
    finalRect.d_min.x = CoordConverter::alignToPixels(finalRect.d_min.x);
    finalRect.d_min.y = CoordConverter::alignToPixels(finalRect.d_min.y);
    finalRect.d_max.x = CoordConverter::alignToPixels(finalRect.d_max.x);
    finalRect.d_max.y = CoordConverter::alignToPixels(finalRect.d_max.y);

    return false;
}

void BitmapImage::createTexturedQuadVertices(
    TexturedColouredVertex* vbuffer,
    const CEGUI::ColourRect &colours,
    Rectf &finalRect,
    const Rectf &texRect) const
{
    // vertex 0
    vbuffer[0].setColour(colours.d_top_left);
    vbuffer[0].d_position = glm::vec3(finalRect.left(), finalRect.top(), 0.0f);
    vbuffer[0].d_texCoords = glm::vec2(texRect.left(), texRect.top());

    // vertex 1
    vbuffer[1].setColour(colours.d_bottom_left);
    vbuffer[1].d_position = glm::vec3(finalRect.left(), finalRect.bottom(), 0.0f);
    vbuffer[1].d_texCoords = glm::vec2(texRect.left(), texRect.bottom());

    // vertex 2
    vbuffer[2].setColour(colours.d_bottom_right);
    vbuffer[2].d_position.x = finalRect.right();
    vbuffer[2].d_position.z = 0.0f;
    vbuffer[2].d_texCoords.x = texRect.right();

    // Quad splitting done from top-left to bottom-right diagonal
    vbuffer[2].d_position.y = finalRect.bottom();
    vbuffer[2].d_texCoords.y = texRect.bottom();


    // vertex 3
    vbuffer[3].setColour(colours.d_top_right);
    vbuffer[3].d_position = glm::vec3(finalRect.right(), finalRect.top(), 0.0f);
    vbuffer[3].d_texCoords = glm::vec2(texRect.right(), texRect.top());

    // vertex 4
    vbuffer[4].setColour(colours.d_top_left);
    vbuffer[4].d_position.x = finalRect.left();
    vbuffer[4].d_position.z = 0.0f;
    vbuffer[4].d_texCoords.x = texRect.left();

    // Quad splitting done from top-left to bottom-right diagonal
    vbuffer[4].d_position.y = finalRect.top();
    vbuffer[4].d_texCoords.y = texRect.top();

    // vertex 5
    vbuffer[5].setColour(colours.d_bottom_right);
    vbuffer[5].d_position = glm::vec3(finalRect.right(), finalRect.bottom(), 0.0f);
    vbuffer[5].d_texCoords = glm::vec2(texRect.right(), texRect.bottom());
}

//----------------------------------------------------------------------------//
const Texture* BitmapImage::getTexture() const
{
    return d_texture;
}

} // End of  CEGUI namespace section

