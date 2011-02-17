/***********************************************************************
    filename:   CEGUIRenderedStringImageComponent.cpp
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
#include "CEGUIRenderedStringImageComponent.h"
#include "CEGUIImageManager.h"
#include "CEGUIImage.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
RenderedStringImageComponent::RenderedStringImageComponent() :
    d_image(0),
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
        d_image = 0;
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
void RenderedStringImageComponent::draw(GeometryBuffer& buffer,
                                        const Vector2<>& position,
                                        const ColourRect* mod_colours,
                                        const Rect* clip_rect,
                                        const float vertical_space,
                                        const float /*space_extra*/) const
{
    if (!d_image)
        return;

    CEGUI::Rect dest(position.d_x, position.d_y, 0, 0);
    float y_scale = 1.0f;

    // handle formatting options
    switch (d_verticalFormatting)
    {
    case VF_BOTTOM_ALIGNED:
        dest.d_top += vertical_space - getPixelSize().d_height;
        break;

    case VF_CENTRE_ALIGNED:
        dest.d_top += (vertical_space - getPixelSize().d_height) / 2 ;
        break;

    case VF_STRETCHED:
        y_scale = vertical_space / getPixelSize().d_height;
        break;

    case VF_TOP_ALIGNED:
        // nothing additional to do for this formatting option.
        break;

    default:
        CEGUI_THROW(InvalidRequestException(
            "RenderedStringImageComponent::draw: "
            "unknown VerticalFormatting option specified."));
    }

    Size<> sz(d_image->getRenderedSize());
    if (d_size.d_width != 0.0)
        sz.d_width = d_size.d_width;
    if (d_size.d_height != 0.0)
        sz.d_height = d_size.d_height;
    
    sz.d_height *= y_scale;
    dest.setSize(sz);

    // apply padding to position
    dest.offset(d_padding.getPosition());

    // apply modulative colours if needed.
    ColourRect final_cols(d_colours);
    if (mod_colours)
        final_cols *= *mod_colours;

    // draw the image.
    d_image->render(buffer, dest, clip_rect, final_cols);
}

//----------------------------------------------------------------------------//
Size<> RenderedStringImageComponent::getPixelSize() const
{
    Size<> sz(0, 0);

    if (d_image)
    {
        sz = d_image->getRenderedSize();
        if (d_size.d_width != 0.0)
            sz.d_width = d_size.d_width;
        if (d_size.d_height != 0.0)
            sz.d_height = d_size.d_height;
        sz.d_width += (d_padding.d_left + d_padding.d_right);
        sz.d_height += (d_padding.d_top + d_padding.d_bottom);
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
    float /*split_point*/, bool /*first_component*/)
{
    CEGUI_THROW(InvalidRequestException(
        "RenderedStringImageComponent::split: this "
        "component does not support being split."));
}

//----------------------------------------------------------------------------//
RenderedStringImageComponent* RenderedStringImageComponent::clone() const
{
    return CEGUI_NEW_AO RenderedStringImageComponent(*this);
}

//----------------------------------------------------------------------------//
size_t RenderedStringImageComponent::getSpaceCount() const
{
    // images do not have spaces.
    return 0;
}

//----------------------------------------------------------------------------//
void RenderedStringImageComponent::setSize(const Size<>& sz)
{
    d_size = sz;
}

//----------------------------------------------------------------------------//
const Size<>& RenderedStringImageComponent::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
