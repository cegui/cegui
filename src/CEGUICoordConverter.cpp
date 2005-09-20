/************************************************************************
    filename:   CEGUICoordConverter.cpp
    created:    Sun Sep 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGUICoordConverter.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{
float CoordConverter::windowToScreenX(const Window& window, const UDim& x)
{
    return getBaseXValue(window) + x.asAbsolute(window.getPixelSize().d_width);
}

//----------------------------------------------------------------------------//

float CoordConverter::windowToScreenY(const Window& window, const UDim& y)
{
    return getBaseYValue(window) + y.asAbsolute(window.getPixelSize().d_height);
}

//----------------------------------------------------------------------------//

Vector2 CoordConverter::windowToScreen(const Window& window, const UVector2& vec)
{
    return getBaseValue(window) + vec.asAbsolute(window.getPixelSize());
}

//----------------------------------------------------------------------------//

Rect CoordConverter::windowToScreen(const Window& window, const URect& rect)
{
    Rect tmp(rect.asAbsolute(window.getPixelSize()));
    return tmp.offset(getBaseValue(window));
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowX(const Window& window, const UDim& x)
{
    return x.asAbsolute(System::getSingleton().getRenderer()->getWidth()) -
           getBaseXValue(window);
}

//----------------------------------------------------------------------------//

float CoordConverter::screenToWindowY(const Window& window, const UDim& y)
{
    return y.asAbsolute(System::getSingleton().getRenderer()->getHeight()) -
           getBaseYValue(window);
}

//----------------------------------------------------------------------------//

Vector2 CoordConverter::screenToWindow(const Window& window, const UVector2& vec)
{
    return vec.asAbsolute(System::getSingleton().getRenderer()->getSize()) -
           getBaseValue(window);
}

//----------------------------------------------------------------------------//

Rect CoordConverter::screenToWindow(const Window& window, const URect& rect)
{
    Vector2 base(getBaseValue(window));
    Rect pixel(rect.asAbsolute(System::getSingleton().getRenderer()->getSize()));

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

Vector2 CoordConverter::windowToScreen(const Window& window, const Vector2& vec)
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

Vector2 CoordConverter::screenToWindow(const Window& window, const Vector2& vec)
{
    return vec - getBaseValue(window);
}

//----------------------------------------------------------------------------//

Rect CoordConverter::screenToWindow(const Window& window, const Rect& rect)
{
    Vector2 base(getBaseValue(window));

    // negate base position
    base.d_x = -base.d_x;
    base.d_y = -base.d_y;

    Rect tmp(rect);
    return tmp.offset(base);
}

//----------------------------------------------------------------------------//

float CoordConverter::getBaseXValue(const Window& window)
{
    const float parent_width = window.getParentPixelWidth();
    float baseX = window.getParent() ? getBaseXValue(*window.getParent()) : 0;

    baseX += window.getWindowArea().d_min.d_x.asAbsolute(parent_width);

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

    return baseX;
}

//----------------------------------------------------------------------------//

float CoordConverter::getBaseYValue(const Window& window)
{
    const float parent_height = window.getParentPixelHeight();
    float baseY = window.getParent() ? getBaseYValue(*window.getParent()) : 0;

    baseY += window.getWindowArea().d_min.d_y.asAbsolute(parent_height);

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

    return baseY;
}

//----------------------------------------------------------------------------//

Vector2 CoordConverter::getBaseValue(const Window& window)
{
    return Vector2(getBaseXValue(window), getBaseYValue(window));
}

} // End of  CEGUI namespace section
