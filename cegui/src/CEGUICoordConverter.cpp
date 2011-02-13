/***********************************************************************
    filename:   CEGUICoordConverter.cpp
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
#include "CEGUICoordConverter.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{
float CoordConverter::windowToScreenX(const Window& window, const UDim& x)
{
    return getBaseXValue(window) + asAbsolute(x, window.getPixelSize().d_width);
}

//----------------------------------------------------------------------------//

float CoordConverter::windowToScreenY(const Window& window, const UDim& y)
{
    return getBaseYValue(window) + asAbsolute(y, window.getPixelSize().d_height);
}

//----------------------------------------------------------------------------//

Vector2<> CoordConverter::windowToScreen(const Window& window, const UVector2& vec)
{
    return getBaseValue(window) + asAbsolute(vec, window.getPixelSize());
}

//----------------------------------------------------------------------------//

Rect CoordConverter::windowToScreen(const Window& window, const URect& rect)
{
    Rect tmp(asAbsolute(rect, window.getPixelSize()));
    return tmp.offset(getBaseValue(window));
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowX(const Window& window, const UDim& x)
{
    return asAbsolute(x,
        System::getSingleton().getRenderer()->getDisplaySize().d_width) -
           getBaseXValue(window);
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowY(const Window& window, const UDim& y)
{
    return asAbsolute(y,
        System::getSingleton().getRenderer()->getDisplaySize().d_height) -
           getBaseYValue(window);
}

//----------------------------------------------------------------------------//

Vector2<> CoordConverter::screenToWindow(const Window& window, const UVector2& vec)
{
    return asAbsolute(vec, 
        System::getSingleton().getRenderer()->getDisplaySize()) -
           getBaseValue(window);
}

//----------------------------------------------------------------------------//

Rect CoordConverter::screenToWindow(const Window& window, const URect& rect)
{
    Vector2<> base(getBaseValue(window));
    Rect pixel(
        asAbsolute(rect, System::getSingleton().getRenderer()->getDisplaySize()));

    // negate base position
    base.d_x = -base.d_x;
    base.d_y = -base.d_y;

    return pixel.offset(base);
}

//----------------------------------------------------------------------------//

float CoordConverter::windowToScreenX(const Window& window, const float x)
{
    return getBaseXValue(window) + x;
}

//----------------------------------------------------------------------------//

float CoordConverter::windowToScreenY(const Window& window, const float y)
{
    return getBaseYValue(window) + y;
}

//----------------------------------------------------------------------------//

Vector2<> CoordConverter::windowToScreen(const Window& window, const Vector2<>& vec)
{
    return getBaseValue(window) + vec;
}

//----------------------------------------------------------------------------//

Rect CoordConverter::windowToScreen(const Window& window, const Rect& rect)
{
    Rect tmp(rect);
    return tmp.offset(getBaseValue(window));
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

Vector2<> CoordConverter::screenToWindow(const Window& window, const Vector2<>& vec)
{
    return vec - getBaseValue(window);
}

//----------------------------------------------------------------------------//

Rect CoordConverter::screenToWindow(const Window& window, const Rect& rect)
{
    Vector2<> base(getBaseValue(window));

    // negate base position
    base.d_x = -base.d_x;
    base.d_y = -base.d_y;

    Rect tmp(rect);
    return tmp.offset(base);
}

//----------------------------------------------------------------------------//

CoordConverter::CoordConverter()
{}

//----------------------------------------------------------------------------//

float CoordConverter::getBaseXValue(const Window& window)
{
    const Window* parent = window.getParent();

    const Rect parent_rect(parent ?
        parent->getChildWindowContentArea(window.isNonClientWindow()) :
        Rect(Vector2<>(0, 0),
             System::getSingleton().getRenderer()->getDisplaySize())
    );

    const float parent_width = parent_rect.getWidth();
    float baseX = parent_rect.d_left;

    baseX += asAbsolute(window.getArea().d_min.d_x, parent_width);

    switch(window.getHorizontalAlignment())
    {
        case HA_CENTRE:
            baseX += (parent_width - window.getPixelSize().d_width) * 0.5f;
            break;
        case HA_RIGHT:
            baseX += parent_width - window.getPixelSize().d_width;
            break;
        default:
            break;
    }

    return PixelAligned(baseX);
}

//----------------------------------------------------------------------------//

float CoordConverter::getBaseYValue(const Window& window)
{
    const Window* parent = window.getParent();

    const Rect parent_rect(parent ?
        parent->getChildWindowContentArea(window.isNonClientWindow()) :
        Rect(Vector2<>(0, 0),
             System::getSingleton().getRenderer()->getDisplaySize())
    );

    const float parent_height = parent_rect.getHeight();
    float baseY = parent_rect.d_top;

    baseY += asAbsolute(window.getArea().d_min.d_y, parent_height);

    switch(window.getVerticalAlignment())
    {
        case VA_CENTRE:
            baseY += (parent_height - window.getPixelSize().d_height) * 0.5f;
            break;
        case VA_BOTTOM:
            baseY += parent_height - window.getPixelSize().d_height;
            break;
        default:
            break;
    }

    return PixelAligned(baseY);
}

//----------------------------------------------------------------------------//

Vector2<> CoordConverter::getBaseValue(const Window& window)
{
    return Vector2<>(getBaseXValue(window), getBaseYValue(window));
}

} // End of  CEGUI namespace section
