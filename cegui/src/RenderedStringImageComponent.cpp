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
#include "CEGUI/Image.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
RenderedStringImageComponent::RenderedStringImageComponent(const Image* image) :
    d_image(image),
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
    d_size(0, 0)
{
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setSelection(const Window* /*ref_wnd*/,
                                                const float start, const float end)
{
    d_selected = (start != end);
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const Window* ref_wnd,
    const glm::vec2& position,
    const ColourRect* mod_colours,
    const Rectf* clip_rect,
    const float vertical_space,
    const float /*space_extra*/) const
{
    if (!d_image)
        return;

    // render the selection if needed
    if (d_selectionImage && d_selected)
    {
        const Rectf select_area(position, getPixelSize(ref_wnd));
        ImageRenderSettings imgRenderSettings(select_area, clip_rect, ColourRect(0xFF002FFF));
        d_selectionImage->createRenderGeometry(out, imgRenderSettings);
    }

    Rectf dest(position.x, position.y, 0, 0);
    float y_scale = 1.0f;

    // handle formatting options
    switch (d_verticalFormatting)
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

        // TODO TEXT: Tiled?

        default:
            throw InvalidRequestException("unsupported VerticalImageFormatting option specified.");
    }

    dest.setWidth((d_size.d_width > 0.f) ? d_size.d_width : d_image->getRenderedSize().d_width);
    dest.setHeight(((d_size.d_height > 0.f) ? d_size.d_height : d_image->getRenderedSize().d_height) * y_scale);

    // apply padding to position
    dest.offset(d_padding.getPosition());

    // apply modulative colours if needed.
    ColourRect final_cols(d_colours);
    if (mod_colours)
        final_cols *= *mod_colours;

    // Create the render geometry for the image
    ImageRenderSettings imgRenderSettings(dest, clip_rect, final_cols);
    d_image->createRenderGeometry(out, imgRenderSettings);
}

//----------------------------------------------------------------------------//
Sizef RenderedStringImageComponent::getPixelSize(const Window* /*ref_wnd*/) const
{
    Sizef sz(0, 0);

    if (d_image)
    {
        sz.d_width = (d_size.d_width > 0.f) ? d_size.d_width : d_image->getRenderedSize().d_width;
        sz.d_height = (d_size.d_height > 0.f) ? d_size.d_height : d_image->getRenderedSize().d_height;
        sz.d_width += (d_padding.d_min.x + d_padding.d_max.x);
        sz.d_height += (d_padding.d_min.y + d_padding.d_max.y);
    }

    return sz;
}

//----------------------------------------------------------------------------//
RenderedStringComponentPtr RenderedStringImageComponent::split(
    const Window* /*ref_wnd*/ ,float /*split_point*/, bool /*first_component*/,
    bool& /*was_word_split*/)
{
    throw InvalidRequestException("this component does not support being split.");
}

//----------------------------------------------------------------------------//
RenderedStringComponentPtr RenderedStringImageComponent::clone() const
{
    return std::make_unique<RenderedStringImageComponent>(*this);
}

}
