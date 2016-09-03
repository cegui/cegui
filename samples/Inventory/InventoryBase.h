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
#ifndef _InventoryBase_h_
#define _InventoryBase_h_

#include "BoolArray2D.h"
#include <CEGUI/Rect.h>

// Start of CEGUI namespace section
namespace CEGUI
{
// common base class for the grid based inventory components.
class InventoryBase
{
public:
    // returns the pixel size of a single square in the content grid.
    virtual Sizef squarePixelSize() const;

    // set the size of the content grid and initialise it.
    virtual void setContentSize(int width, int height);

    // return the width of the content grid, in grid squares.
    int contentWidth() const;

    // return the height of the content grid, in grid squares.
    int contentHeight() const;

    // return the grid x co-ordinate that corresponds to the given screen
    // pixel co-ordinate.
    int gridXLocationFromPixelPosition(float x_pixel_location) const;

    // return the grid y co-ordinate that corresponds to the given screen
    // pixel co-ordinate.
    int gridYLocationFromPixelPosition(float y_pixel_location) const;

protected:
    // return the screen rect where the content grid is rendered.
    virtual Rectf gridBasePixelRect() const = 0;

    // array holding the content grid data.
    BoolArray2D d_content;
};

} // End of  CEGUI namespace section

#endif  // end of guard _InventoryBase_h_

