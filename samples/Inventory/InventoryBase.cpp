/***********************************************************************
    created:    Fri Apr 22 2011
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
#include "InventoryBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//------------------------------------------------------------------------------//
void InventoryBase::setContentSize(int width, int height)
{
    d_content.resetSize(width, height);
}

//------------------------------------------------------------------------------//
int InventoryBase::contentWidth() const
{
    return d_content.width();
}

//------------------------------------------------------------------------------//
int InventoryBase::contentHeight() const
{
    return d_content.height();
}

//------------------------------------------------------------------------------//
Sizef InventoryBase::squarePixelSize() const
{
    const Rectf area(gridBasePixelRect());
    return Sizef(area.getWidth() / d_content.width(),
                 area.getHeight() / d_content.height());
}

//------------------------------------------------------------------------------//
int InventoryBase::gridXLocationFromPixelPosition(float x_pixel_location) const
{
    const Rectf area(gridBasePixelRect());

    if (x_pixel_location < static_cast<int>(area.left()) ||
        x_pixel_location >= static_cast<int>(area.right()))
        return -1;

    return static_cast<int>( (x_pixel_location - area.left()) / (area.getWidth() / d_content.width()) );
}

//------------------------------------------------------------------------------//
int InventoryBase::gridYLocationFromPixelPosition(float y_pixel_location) const
{
    const Rectf area(gridBasePixelRect());

    if (y_pixel_location < static_cast<int>(area.top()) ||
        y_pixel_location >= static_cast<int>(area.bottom()))
        return -1;

    return static_cast<int>( (y_pixel_location - area.top()) / (area.getHeight() / d_content.height()) );
}

//------------------------------------------------------------------------------//

} // End of  CEGUI namespace section

