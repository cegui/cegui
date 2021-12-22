/***********************************************************************
    created:    Sun Sep 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/CoordConverter.h"
#include "CEGUI/Element.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowX(const Element& element, const UDim& x)
{
    return asAbsolute(x, element.getRootContainerSize().d_width) - element.getUnclippedOuterRect().get().d_min.x;
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowY(const Element& element, const UDim& y)
{
    return asAbsolute(y, element.getRootContainerSize().d_height) - element.getUnclippedOuterRect().get().d_min.y;
}

//----------------------------------------------------------------------------//

glm::vec2 CoordConverter::screenToWindow(const Element& element, const UVector2& vec)
{
    return asAbsolute(vec, element.getRootContainerSize()) - element.getUnclippedOuterRect().get().d_min;
}

//----------------------------------------------------------------------------//
Rectf CoordConverter::screenToWindow(const Element& element, const URect& rect)
{
    Rectf pixel(asAbsolute(rect, element.getRootContainerSize()));
    pixel.offset(-element.getUnclippedOuterRect().get().d_min);
    return pixel;
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowX(const Element& element, const float x)
{
    return x - element.getUnclippedOuterRect().get().d_min.x;
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowY(const Element& element, const float y)
{
    return y - element.getUnclippedOuterRect().get().d_min.y;
}

//----------------------------------------------------------------------------//

glm::vec2 CoordConverter::screenToWindow(const Element& element, const glm::vec2& vec)
{
    return vec - element.getUnclippedOuterRect().get().d_min;
}

//----------------------------------------------------------------------------//

Rectf CoordConverter::screenToWindow(const Element& element, const Rectf& rect)
{
    Rectf tmp(rect);
    tmp.offset(-element.getUnclippedOuterRect().get().d_min);
    return tmp;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
