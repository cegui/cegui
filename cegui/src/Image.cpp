/***********************************************************************
    created:    Sat Jun 11 2011
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
#include "CEGUI/Image.h"
#include "CEGUI/Logger.h"
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
Image::Image(const String& name) :
    d_name(name),
    d_imageArea(0.0f, 0.0f, 0.0f, 0.0f),
    d_pixelOffset(0.0f, 0.0f),
    d_autoScaled(AutoScaledMode::Disabled),
    d_nativeResolution(640, 480),
    d_scaledSize(0, 0),
    d_scaledOffset(0, 0)
{
    // force initialisation of the autoscaling fields.
    updateScaledSizeAndOffset(
        System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
Image::Image(const String& name,
             const glm::vec2& pixel_offset,
             const Rectf& image_area,
             AutoScaledMode auto_scaled,
             const Sizef& native_resolution
             ) :
    d_name(name),
    d_imageArea(image_area),
    d_pixelOffset(pixel_offset),
    d_autoScaled(auto_scaled),
    d_nativeResolution(native_resolution),
    d_scaledSize(0, 0),
    d_scaledOffset(0, 0)
{
    // force initialisation of the autoscaling fields.
    updateScaledSizeAndOffset(
        System::getSingleton().getRenderer()->getDisplaySize());
}
    

//----------------------------------------------------------------------------//
Image::~Image()
{
}

//----------------------------------------------------------------------------//
void Image::computeScalingFactors(AutoScaledMode mode,
                                  const Sizef& display_size,
                                  const Sizef& native_display_size,
                                  float& x_scale,
                                  float& y_scale)
{
    if (mode == AutoScaledMode::Disabled)
    {
        x_scale = 1.0f;
        y_scale = 1.0f;
    }
    else if (mode == AutoScaledMode::Vertical)
    {
        x_scale = display_size.d_height / native_display_size.d_height;
        y_scale = x_scale;
    }
    else if (mode == AutoScaledMode::Horizontal)
    {
        x_scale = display_size.d_width / native_display_size.d_width;
        y_scale = x_scale;
    }
    else if (mode == AutoScaledMode::Min)
    {
        x_scale = std::min(display_size.d_width / native_display_size.d_width,
                           display_size.d_height / native_display_size.d_height);
        y_scale = x_scale;
    }
    else if (mode == AutoScaledMode::Max)
    {
        x_scale = std::max(display_size.d_width / native_display_size.d_width,
                           display_size.d_height / native_display_size.d_height);
        y_scale = x_scale;
    }
    else if (mode == AutoScaledMode::Both)
    {
        x_scale = display_size.d_width / native_display_size.d_width;
        y_scale = display_size.d_height / native_display_size.d_height;
    }
    else
    {
        assert(false && "Invalid AutoScaledMode");
    }
}

//----------------------------------------------------------------------------//
void Image::elementStartLocal(const String& element,
                              const XMLAttributes& /*attributes*/)
{
     Logger::getSingleton().logEvent(
        "    [Image] Unknown XML tag encountered: " + element);
}

//----------------------------------------------------------------------------//
void Image::elementEndLocal(const String& element)
{
    if (element == "Image")
        d_completed = true;
}

//----------------------------------------------------------------------------//
void Image::setImageArea(const Rectf& image_area)
{
    d_imageArea = image_area;

    if (d_autoScaled != AutoScaledMode::Disabled)
        updateScaledSizeAndOffset(System::getSingleton().getRenderer()->getDisplaySize());
    else
        d_scaledSize = d_imageArea.getSize();
}

//----------------------------------------------------------------------------//
void Image::setOffset(const glm::vec2& pixel_offset)
{
    d_pixelOffset = pixel_offset;

    if (d_autoScaled != AutoScaledMode::Disabled)
        updateScaledOffset(System::getSingleton().getRenderer()->getDisplaySize());
    else
        d_scaledOffset = d_pixelOffset;
}

//----------------------------------------------------------------------------//
void Image::setAutoScaled(const AutoScaledMode autoscaled)
{
    d_autoScaled = autoscaled;

    if (d_autoScaled != AutoScaledMode::Disabled)
    {
        updateScaledSizeAndOffset(System::getSingleton().getRenderer()->getDisplaySize());
    }
    else
    {
        d_scaledSize = d_imageArea.getSize();
        d_scaledOffset = d_pixelOffset;
    }
}

//----------------------------------------------------------------------------//
void Image::setNativeResolution(const Sizef& native_res)
{
    d_nativeResolution = native_res;

    if (d_autoScaled != AutoScaledMode::Disabled)
        updateScaledSizeAndOffset(System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void Image::notifyDisplaySizeChanged(const Sizef& renderer_display_size)
{
    //If we use autoscaling of any sort we must update the scaled size and offset
    if (d_autoScaled != AutoScaledMode::Disabled)
        updateScaledSizeAndOffset(renderer_display_size);
}

//----------------------------------------------------------------------------//
void Image::updateScaledSizeAndOffset(const Sizef& renderer_display_size)
{
    glm::vec2 scaleFactors;
    computeScalingFactors(d_autoScaled, renderer_display_size, d_nativeResolution,
        scaleFactors.x, scaleFactors.y);

    d_scaledSize = d_imageArea.getSize() * scaleFactors;
    d_scaledOffset = d_pixelOffset * scaleFactors;
}

//----------------------------------------------------------------------------//
void Image::updateScaledSize(const Sizef& renderer_display_size)
{
    glm::vec2 scaleFactors;
    computeScalingFactors(d_autoScaled, renderer_display_size, d_nativeResolution,
        scaleFactors.x, scaleFactors.y);

    d_scaledSize = d_imageArea.getSize() * scaleFactors;
}

//----------------------------------------------------------------------------//
void Image::updateScaledOffset(const Sizef& renderer_display_size)
{
    glm::vec2 scaleFactors;
    computeScalingFactors(d_autoScaled, renderer_display_size, d_nativeResolution,
        scaleFactors.x, scaleFactors.y);

    d_scaledOffset = d_pixelOffset * scaleFactors;
}

}
