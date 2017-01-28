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
#include "CEGUI/Window.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowX(const Window& window, const UDim& x)
{
    return asAbsolute(x,
        window.getRootContainerSize().d_width) - getBaseXValue(window);
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowY(const Window& window, const UDim& y)
{
    return asAbsolute(y,
        window.getRootContainerSize().d_height) - getBaseYValue(window);
}

//----------------------------------------------------------------------------//

glm::vec2 CoordConverter::screenToWindow(const Window& window, const UVector2& vec)
{
    return asAbsolute(vec, window.getRootContainerSize()) - getBaseValue(window);
}

//----------------------------------------------------------------------------//
Rectf CoordConverter::screenToWindow(const Window& window, const URect& rect)
{
    const glm::vec2 base(getBaseValue(window));
    Rectf pixel(asAbsolute(rect, window.getRootContainerSize()));

    pixel.offset(-base);
    return pixel;
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowX(const Window& window, const float x)
{
    return x - getBaseXValue(window);
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowY(const Window& window, const float y)
{
    return y - getBaseYValue(window);
}

//----------------------------------------------------------------------------//

glm::vec2 CoordConverter::screenToWindow(const Window& window, const glm::vec2& vec)
{
    return vec - getBaseValue(window);
}

//----------------------------------------------------------------------------//

Rectf CoordConverter::screenToWindow(const Window& window, const Rectf& rect)
{
    const glm::vec2 base(getBaseValue(window));

    Rectf tmp(rect);
    tmp.offset(-base);

    return tmp;
}

//----------------------------------------------------------------------------//

CoordConverter::CoordConverter()
{}

//----------------------------------------------------------------------------//

float CoordConverter::getBaseXValue(const Window& window)
{
    const Window* parent = window.getParent();

    const Rectf parent_rect(parent ?
        parent->getChildContentArea(window.isNonClient()).get() :
        Rectf(glm::vec2(0, 0), window.getRootContainerSize())
    );

    const float parent_width = parent_rect.getWidth();
    float baseX = parent_rect.d_min.x;

    baseX += asAbsolute(window.getArea().d_min.d_x, parent_width);

    switch(window.getHorizontalAlignment())
    {
        case HorizontalAlignment::Centre:
            baseX += (parent_width - window.getPixelSize().d_width) * 0.5f;
            break;
        case HorizontalAlignment::Right:
            baseX += parent_width - window.getPixelSize().d_width;
            break;
        default:
            break;
    }

    return alignToPixels(baseX);
}

//----------------------------------------------------------------------------//

float CoordConverter::getBaseYValue(const Window& window)
{
    const Window* parent = window.getParent();

    const Rectf parent_rect(parent ?
        parent->getChildContentArea(window.isNonClient()).get() :
        Rectf(glm::vec2(0, 0), window.getRootContainerSize())
    );

    const float parent_height = parent_rect.getHeight();
    float baseY = parent_rect.d_min.y;

    baseY += asAbsolute(window.getArea().d_min.d_y, parent_height);

    switch(window.getVerticalAlignment())
    {
        case VerticalAlignment::Centre:
            baseY += (parent_height - window.getPixelSize().d_height) * 0.5f;
            break;
        case VerticalAlignment::Bottom:
            baseY += parent_height - window.getPixelSize().d_height;
            break;
        default:
            break;
    }

    return alignToPixels(baseY);
}

//----------------------------------------------------------------------------//

glm::vec2 CoordConverter::getBaseValue(const Window& window)
{
    return glm::vec2(getBaseXValue(window), getBaseYValue(window));
}

} // End of  CEGUI namespace section
