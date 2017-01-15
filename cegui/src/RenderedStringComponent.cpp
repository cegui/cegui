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
#include "CEGUI/RenderedStringComponent.h"

#include "CEGUI/ImageManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
RenderedStringComponent* RenderedStringComponent::split(
  const Window* ref_wnd, float split_point, bool first_component)
{
    bool was_word_split = false;
    return split(ref_wnd, split_point, first_component, was_word_split);
}

//----------------------------------------------------------------------------//
RenderedStringComponent::RenderedStringComponent() :
    d_padding(0, 0, 0, 0),
    d_selectionImage(nullptr)
{
}

//----------------------------------------------------------------------------//
RenderedStringComponent::~RenderedStringComponent()
{
}

//----------------------------------------------------------------------------//
void RenderedStringComponent::setVerticalTextFormatting(VerticalTextFormatting fmt)
{
    d_verticalTextFormatting = fmt;
}

//----------------------------------------------------------------------------//
VerticalTextFormatting RenderedStringComponent::getVerticalTextFormatting() const
{
    return d_verticalTextFormatting;
}

//----------------------------------------------------------------------------//
void RenderedStringComponent::setPadding(const Rectf& padding)
{
    d_padding = padding;
}

//----------------------------------------------------------------------------//
void RenderedStringComponent::setLeftPadding(const float padding)
{
    d_padding.d_min.x = padding;
}

//----------------------------------------------------------------------------//
void RenderedStringComponent::setRightPadding(const float padding)
{
    d_padding.d_max.x = padding;
}

//----------------------------------------------------------------------------//
void RenderedStringComponent::setTopPadding(const float padding)
{
    d_padding.d_min.y = padding;
}

//----------------------------------------------------------------------------//
void RenderedStringComponent::setBottomPadding(const float padding)
{
    d_padding.d_max.x = padding;
}

//----------------------------------------------------------------------------//
const Rectf& RenderedStringComponent::getPadding() const
{
    return d_padding;
}

//----------------------------------------------------------------------------//
float RenderedStringComponent::getLeftPadding() const
{
    return d_padding.d_min.x;
}

//----------------------------------------------------------------------------//
float RenderedStringComponent::getRightPadding() const
{
    return d_padding.d_max.x;
}

//----------------------------------------------------------------------------//
float RenderedStringComponent::getTopPadding() const
{
    return d_padding.d_min.y;
}

//----------------------------------------------------------------------------//
float RenderedStringComponent::getBottomPadding() const
{
    return d_padding.d_max.y;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
