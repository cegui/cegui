/***********************************************************************
    filename:   BitmapImage.cpp
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
    d_texture(0),
    d_area(0.0f, 0.0f, 0.0f, 0.0f)
{
}

//----------------------------------------------------------------------------//
BitmapImage::BitmapImage(const XMLAttributes& attributes) :
    Image(attributes.getValueAsString(ImageNameAttribute),
          Vector2f(static_cast<float>(attributes.getValueAsInteger(ImageXOffsetAttribute, 0)),
                   static_cast<float>(attributes.getValueAsInteger(ImageYOffsetAttribute, 0))),
          Sizef(static_cast<float>(attributes.getValueAsInteger(ImageWidthAttribute, 0)),
                static_cast<float>(attributes.getValueAsInteger(ImageHeightAttribute, 0)) ),
          PropertyHelper<AutoScaledMode>::fromString(attributes.getValueAsString(ImageAutoScaledAttribute)),
          Sizef(static_cast<float>(attributes.getValueAsInteger(ImageNativeHorzResAttribute, 640)),
                static_cast<float>(attributes.getValueAsInteger(ImageNativeVertResAttribute, 480)))  ),
    d_texture(&System::getSingleton().getRenderer()->getTexture(
              attributes.getValueAsString(ImageTextureAttribute))),
    d_area(Vector2f(static_cast<float>(attributes.getValueAsInteger(ImageXPosAttribute, 0)),
                    static_cast<float>(attributes.getValueAsInteger(ImageYPosAttribute, 0))),
           d_pixelSize)
{


    // force initialisation of the autoscaling fields.
    notifyDisplaySizeChanged(
        System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
BitmapImage::BitmapImage(const String& name, Texture* texture,
                       const Rectf& pixel_area, const Vector2f& pixel_offset,
                       const AutoScaledMode autoscaled, const Sizef& native_res) :
    Image(name,
          pixel_offset,
          pixel_area.getSize(),
          autoscaled,
          native_res),
    d_texture(texture),
    d_area(pixel_area)
{
    // force initialisation of the autoscaling fields.
    updateScaledSizeAndOffset(
        System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void BitmapImage::setArea(const Rectf& pixel_area)
{
    d_area = pixel_area;
    d_pixelSize = pixel_area.getSize();

    if (d_autoScaled != ASM_Disabled)
        updateScaledSize(
            System::getSingleton().getRenderer()->getDisplaySize());
    else
        d_scaledSize = d_pixelSize;
}


//----------------------------------------------------------------------------//
void BitmapImage::setTexture(Texture* texture)
{
    d_texture = texture;
}

//----------------------------------------------------------------------------//
void BitmapImage::render(GeometryBuffer& buffer, const Rectf& dest_area,
                        const Rectf* clip_area, const bool clipping_enabled,
                        const ColourRect& colours) const
{
    const QuadSplitMode quad_split_mode(TopLeftToBottomRight);

    Rectf dest(dest_area);
    // apply rendering offset to the destination Rect
    dest.offset(d_scaledOffset);

    // get the rect area that we will actually draw to (i.e. perform clipping)
    Rectf final_rect(clip_area ? dest.getIntersection(*clip_area) : dest );

    // check if rect was totally clipped
    if ((final_rect.getWidth() == 0) || (final_rect.getHeight() == 0))
        return;

    // Obtain correct scale values from the texture
    const Vector2f& scale = d_texture->getTexelScaling();
    const Vector2f tex_per_pix(d_area.getWidth() / dest.getWidth(), d_area.getHeight() / dest.getHeight());

    // calculate final, clipped, texture co-ordinates
    const Rectf tex_rect((d_area.d_min + ((final_rect.d_min - dest.d_min) * tex_per_pix)) * scale,
                          (d_area.d_max + ((final_rect.d_max - dest.d_max) * tex_per_pix)) * scale);

    // URGENT FIXME: Shouldn't this be in the hands of the user?
    final_rect.d_min.d_x = CoordConverter::alignToPixels(final_rect.d_min.d_x);
    final_rect.d_min.d_y = CoordConverter::alignToPixels(final_rect.d_min.d_y);
    final_rect.d_max.d_x = CoordConverter::alignToPixels(final_rect.d_max.d_x);
    final_rect.d_max.d_y = CoordConverter::alignToPixels(final_rect.d_max.d_y);

    TexturedColouredVertex vbuffer[6];

    // vertex 0
    vbuffer[0].position   = Vector3f(final_rect.left(), final_rect.top(), 0.0f);
    vbuffer[0].colour_val = colours.d_top_left;
    vbuffer[0].tex_coords = Vector2f(tex_rect.left(), tex_rect.top());

    // vertex 1
    vbuffer[1].position   = Vector3f(final_rect.left(), final_rect.bottom(), 0.0f);
    vbuffer[1].colour_val = colours.d_bottom_left;
    vbuffer[1].tex_coords = Vector2f(tex_rect.left(), tex_rect.bottom());

    // vertex 2
    vbuffer[2].position.d_x   = final_rect.right();
    vbuffer[2].position.d_z   = 0.0f;
    vbuffer[2].colour_val     = colours.d_bottom_right;
    vbuffer[2].tex_coords.d_x = tex_rect.right();

    // top-left to bottom-right diagonal
    if (quad_split_mode == TopLeftToBottomRight)
    {
        vbuffer[2].position.d_y   = final_rect.bottom();
        vbuffer[2].tex_coords.d_y = tex_rect.bottom();
    }
    // bottom-left to top-right diagonal
    else
    {
        vbuffer[2].position.d_y   = final_rect.top();
        vbuffer[2].tex_coords.d_y = tex_rect.top();
    }

    // vertex 3
    vbuffer[3].position   = Vector3f(final_rect.right(), final_rect.top(), 0.0f);
    vbuffer[3].colour_val = colours.d_top_right;
    vbuffer[3].tex_coords = Vector2f(tex_rect.right(), tex_rect.top());

    // vertex 4
    vbuffer[4].position.d_x   = final_rect.left();
    vbuffer[4].position.d_z   = 0.0f;
    vbuffer[4].colour_val     = colours.d_top_left;
    vbuffer[4].tex_coords.d_x = tex_rect.left();

    // top-left to bottom-right diagonal
    if (quad_split_mode == TopLeftToBottomRight)
    {
        vbuffer[4].position.d_y   = final_rect.top();
        vbuffer[4].tex_coords.d_y = tex_rect.top();
    }
    // bottom-left to top-right diagonal
    else
    {
        vbuffer[4].position.d_y   = final_rect.bottom();
        vbuffer[4].tex_coords.d_y = tex_rect.bottom();
    }

    // vertex 5
    vbuffer[5].position = Vector3f(final_rect.right(), final_rect.bottom(), 0.0f);
    vbuffer[5].colour_val= colours.d_bottom_right;
    vbuffer[5].tex_coords = Vector2f(tex_rect.right(), tex_rect.bottom());

    buffer.setClippingActive(clipping_enabled);
    if(clipping_enabled)
        buffer.setClippingRegion(*clip_area);
    buffer.setTexture(d_texture);
    buffer.appendGeometry(vbuffer, 6);
}




//----------------------------------------------------------------------------//
const Texture* BitmapImage::getTexture() const
{
    return d_texture;
}

} // End of  CEGUI namespace section

