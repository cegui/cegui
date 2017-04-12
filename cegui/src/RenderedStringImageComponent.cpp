/***********************************************************************
    created:    24/05/2009
    author:     Paul Turner
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RenderedStringImageComponent.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Image.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
RenderedStringImageComponent::RenderedStringImageComponent() :
    d_image(nullptr),
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
    d_size(0, 0)
{
}

//----------------------------------------------------------------------------//
RenderedStringImageComponent::RenderedStringImageComponent(const String& name) :
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
    d_size(0, 0)
{
    setImage(name);
}

//----------------------------------------------------------------------------//
RenderedStringImageComponent::RenderedStringImageComponent(const Image* image) :
    d_image(image),
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
    d_size(0, 0)
{
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setImage(const String& name)
{
    if (!name.empty())
    {
        d_image = &ImageManager::getSingleton().get(name);
    }
    else
    {
        d_image = nullptr;
    }
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setImage(const Image* image)
{
    d_image = image;
}

//----------------------------------------------------------------------------//
const Image* RenderedStringImageComponent::getImage() const
{
    return d_image;
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setColours(const ColourRect& cr)
{
    d_colours = cr;
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setColours(const Colour& c)
{
    d_colours.setColours(c);
}

//----------------------------------------------------------------------------//
const ColourRect& RenderedStringImageComponent::getColours() const
{
    return d_colours;
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setSelection(const Window* /*ref_wnd*/,
                                                const float start, const float end)
{
    d_selected = (start != end);
}

//----------------------------------------------------------------------------//
std::vector<GeometryBuffer*> RenderedStringImageComponent::createRenderGeometry(
    const Window* ref_wnd,
    const glm::vec2& position,
    const ColourRect* mod_colours,
    const Rectf* clip_rect,
    const float vertical_space,
    const float /*space_extra*/) const
{
    if (!d_image)
    {
        return std::vector<GeometryBuffer*>();
    }

    Rectf dest(position.x, position.y, 0, 0);
    float y_scale = 1.0f;

    // handle formatting options
    switch (d_verticalImageFormatting)
    {
    case VerticalImageFormatting::BottomAligned:
        dest.d_min.y += vertical_space - getPixelSize(ref_wnd).d_height;
        break;

    case VerticalImageFormatting::CentreAligned:
        dest.d_min.y += (vertical_space - getPixelSize(ref_wnd).d_height) / 2 ;
        break;

    case VerticalImageFormatting::Stretched:
        y_scale = vertical_space / getPixelSize(ref_wnd).d_height;
        break;

    case VerticalImageFormatting::TopAligned:
        // nothing additional to do for this formatting option.
        break;

    default:
        throw InvalidRequestException(
            "unknown VerticalImageFormatting option specified.");
    }

    Sizef sz(d_image->getRenderedSize());
    if (d_size.d_width != 0.0)
        sz.d_width = d_size.d_width;
    if (d_size.d_height != 0.0)
        sz.d_height = d_size.d_height;
    
    sz.d_height *= y_scale;
    dest.setSize(sz);

    // apply padding to position
    dest.offset(d_padding.getPosition());

    // render the selection if needed
    if (d_selectionImage && d_selected)
    {
        const Rectf select_area(position, getPixelSize(ref_wnd));

        ImageRenderSettings imgRenderSettings(
            select_area, clip_rect, true, ColourRect(0xFF002FFF));

        auto geomBuffers = d_selectionImage->createRenderGeometry(imgRenderSettings);

        geomBuffers.insert(geomBuffers.end(), geomBuffers.begin(), geomBuffers.end());
    }

    // apply modulative colours if needed.
    ColourRect final_cols(d_colours);
    if (mod_colours)
        final_cols *= *mod_colours;

    // Create the render geometry for the image
    ImageRenderSettings imgRenderSettings(
        dest, clip_rect, true, final_cols);

    std::vector<GeometryBuffer*> imageGeomBuffers =
        d_image->createRenderGeometry(imgRenderSettings);

    return imageGeomBuffers;
}

//----------------------------------------------------------------------------//
Sizef RenderedStringImageComponent::getPixelSize(const Window* /*ref_wnd*/) const
{
    Sizef sz(0, 0);

    if (d_image)
    {
        sz = d_image->getRenderedSize();
        if (d_size.d_width != 0.0)
            sz.d_width = d_size.d_width;
        if (d_size.d_height != 0.0)
            sz.d_height = d_size.d_height;
        sz.d_width += (d_padding.d_min.x + d_padding.d_max.x);
        sz.d_height += (d_padding.d_min.y + d_padding.d_max.y);
    }

    return sz;
}

//----------------------------------------------------------------------------//
bool RenderedStringImageComponent::canSplit() const
{
    return false;
}

//----------------------------------------------------------------------------//
RenderedStringImageComponent* RenderedStringImageComponent::split(
    const Window* /*ref_wnd*/ ,float /*split_point*/, bool /*first_component*/,
    bool& /*was_word_split*/)
{
    throw InvalidRequestException(
        "this component does not support being split.");
}

//----------------------------------------------------------------------------//
RenderedStringImageComponent* RenderedStringImageComponent::clone() const
{
    return new RenderedStringImageComponent(*this);
}

//----------------------------------------------------------------------------//
size_t RenderedStringImageComponent::getSpaceCount() const
{
    // images do not have spaces.
    return 0;
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setSize(const Sizef& sz)
{
    d_size = sz;
}

//----------------------------------------------------------------------------//
const Sizef& RenderedStringImageComponent::getSize() const
{
    return d_size;
}


void RenderedStringImageComponent::setVerticalImageFormatting(VerticalImageFormatting verticalImageFormatting)
{
    d_verticalImageFormatting = verticalImageFormatting;
}

VerticalImageFormatting RenderedStringImageComponent::getVerticalImageFormatting() const
{
    return d_verticalImageFormatting;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
