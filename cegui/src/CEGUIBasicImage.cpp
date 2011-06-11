/***********************************************************************
    filename:   CEGUIBasicImage.cpp
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
#include "CEGUIBasicImage.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUITexture.h"
#include "CEGUIVertex.h"
#include "CEGUIColourRect.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUISystem.h" // this being here is a bit nasty IMO

// Start of CEGUI namespace section
namespace CEGUI
{
const String ImageTypeAttribute( "Type" );
const String ImageNameAttribute( "Name" );
const String ImageTextureAttribute( "Texture" );
const String ImageXPosAttribute( "XPos" );
const String ImageYPosAttribute( "YPos" );
const String ImageWidthAttribute( "Width" );
const String ImageHeightAttribute( "Height" );
const String ImageXOffsetAttribute( "XOffset" );
const String ImageYOffsetAttribute( "YOffset" );
const String ImageAutoScaledAttribute( "AutoScaled" );
const String ImageNativeHorzResAttribute( "NativeHorzRes" );
const String ImageNativeVertResAttribute( "NativeVertRes" );

//----------------------------------------------------------------------------//
BasicImage::BasicImage(const String& name) :
    d_name(name),
    d_texture(0),
    d_pixelSize(0, 0),
    d_area(0.0f, 0.0f, 0.0f, 0.0f),
    d_pixelOffset(0.0f, 0.0f),
    d_autoscaled(false),
    d_nativeResolution(640, 480),
    d_scaledSize(0, 0),
    d_scaledOffset(0, 0)
{
}

//----------------------------------------------------------------------------//
BasicImage::BasicImage(const XMLAttributes& attributes) :
    d_name(attributes.getValueAsString(ImageNameAttribute)),
    d_texture(&System::getSingleton().getRenderer()->getTexture(
              attributes.getValueAsString(ImageTextureAttribute))),
    d_pixelSize(attributes.getValueAsInteger(ImageWidthAttribute, 0),
                attributes.getValueAsInteger(ImageHeightAttribute, 0)),
    d_area(Vector2f(attributes.getValueAsInteger(ImageXPosAttribute, 0),
                    attributes.getValueAsInteger(ImageYPosAttribute, 0)),
           d_pixelSize),
    d_pixelOffset(Vector2f(
        attributes.getValueAsInteger(ImageXOffsetAttribute, 0),
        attributes.getValueAsInteger(ImageYOffsetAttribute, 0))),
    d_autoscaled(attributes.getValueAsBool(ImageAutoScaledAttribute)),
    d_nativeResolution(Sizef(
        attributes.getValueAsFloat(ImageNativeHorzResAttribute, 640),
        attributes.getValueAsFloat(ImageNativeVertResAttribute, 480)))
{
    // force initialisation of the autoscaling fields.
    notifyDisplaySizeChanged(
        System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
BasicImage::BasicImage(const String& name, Texture* texture,
                       const Rectf& pixel_area, const Vector2f& pixel_offset,
                       const bool autoscaled, const Sizef& native_res) :
    d_name(name),
    d_texture(texture),
    d_pixelSize(pixel_area.getSize()),
    d_area(pixel_area),
    d_pixelOffset(pixel_offset),
    d_autoscaled(autoscaled),
    d_nativeResolution(native_res)

{
    // force initialisation of the autoscaling fields.
    notifyDisplaySizeChanged(
        System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void BasicImage::setTexture(Texture* texture)
{
    d_texture = texture;
}

//----------------------------------------------------------------------------//
void BasicImage::setArea(const Rectf& pixel_area)
{
    d_area = pixel_area;
    d_pixelSize = pixel_area.getSize();

    if (d_autoscaled)
        notifyDisplaySizeChanged(
            System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void BasicImage::setOffset(const Vector2f& pixel_offset)
{
    d_pixelOffset = pixel_offset;

    if (d_autoscaled)
        notifyDisplaySizeChanged(
            System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void BasicImage::setAutoScaled(const bool autoscaled)
{
    d_autoscaled = autoscaled;

    if (d_autoscaled)
    {
        notifyDisplaySizeChanged(
            System::getSingleton().getRenderer()->getDisplaySize());
    }
    else
    {
        d_scaledSize = d_pixelSize;
        d_scaledOffset = d_pixelOffset;
    }
}

//----------------------------------------------------------------------------//
void BasicImage::setNativeResolution(const Sizef& native_res)
{
    d_nativeResolution = native_res;

    if (d_autoscaled)
        notifyDisplaySizeChanged(
            System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
const String& BasicImage::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& BasicImage::getRenderedSize() const
{
    return d_scaledSize;
}

//----------------------------------------------------------------------------//
const Vector2f& BasicImage::getRenderedOffset() const
{
    return d_scaledOffset;
}

//----------------------------------------------------------------------------//
void BasicImage::render(GeometryBuffer& buffer, const Rectf& dest_area,
                        const Rectf* clip_area, const ColourRect& colours) const
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

    final_rect.d_min.d_x = PixelAligned(final_rect.d_min.d_x);
    final_rect.d_min.d_y = PixelAligned(final_rect.d_min.d_y);
    final_rect.d_max.d_x = PixelAligned(final_rect.d_max.d_x);
    final_rect.d_max.d_y = PixelAligned(final_rect.d_max.d_y);

    Vertex vbuffer[6];

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

    buffer.setActiveTexture(d_texture);
    buffer.appendGeometry(vbuffer, 6);
}

//----------------------------------------------------------------------------//
void BasicImage::notifyDisplaySizeChanged(const Sizef& size)
{
    // TODO: This could be cleaned up by adding some more operators to
    // Sizef and Vector2f

    const float x_scale =
        d_autoscaled ? size.d_width / d_nativeResolution.d_width : 1.0f;
    const float y_scale =
        d_autoscaled ? size.d_height / d_nativeResolution.d_height : 1.0f;

    d_scaledSize.d_width = d_pixelSize.d_width * x_scale;
    d_scaledSize.d_height = d_pixelSize.d_height * y_scale;
    d_scaledOffset.d_x = d_pixelOffset.d_x * x_scale;
    d_scaledOffset.d_y = d_pixelOffset.d_y * y_scale;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

