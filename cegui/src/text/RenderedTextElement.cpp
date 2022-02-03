/***********************************************************************
    created:    27/01/2022
    author:     Vladimir Orlov
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2022 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/text/RenderedTextElement.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextElement::applyVerticalFormatting(float lineHeight, float& posY, float& heightScale) const
{
    switch (d_verticalFormatting)
    {
        case VerticalImageFormatting::BottomAligned:
        {
            const float heightPadded = getHeight();
            posY += lineHeight - heightPadded;
            heightScale = 1.f;
            break;
        }
        case VerticalImageFormatting::CentreAligned:
        {
            const float heightPadded = getHeight();
            posY += (lineHeight - heightPadded) * 0.5f;
            heightScale = 1.f;
            break;
        }
        case VerticalImageFormatting::Stretched:
        {
            const float padding = getTopPadding() + getBottomPadding();
            const float height = getHeight() - padding;
            heightScale = (height > 0.f) ? ((lineHeight - padding) / height) : 0.f;
            break;
        }
        // TopAligned requires no operations
        // Tiled is not supported here
    }
}

}
