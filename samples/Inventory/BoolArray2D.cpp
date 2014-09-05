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
#include "BoolArray2D.h"
#include <stdexcept>

// Start of CEGUI namespace section
namespace CEGUI
{
//------------------------------------------------------------------------------//
BoolArray2D::BoolArray2D() :
    d_width(0),
    d_height(0),
    d_content(0)
{
}

//------------------------------------------------------------------------------//
BoolArray2D::BoolArray2D(int width, int height) :
    d_content(0)
{
    resetSize(width, height);
}

//------------------------------------------------------------------------------//
BoolArray2D::~BoolArray2D()
{
    delete[] d_content;
}
    
//------------------------------------------------------------------------------//
int BoolArray2D::width() const
{
    return d_width;
}

//------------------------------------------------------------------------------//
int BoolArray2D::height() const
{
    return d_height;
}

//------------------------------------------------------------------------------//
bool BoolArray2D::elementAtLocation(int x, int y) const
{
    if (x < 0 || x >= d_width || y < 0 || y >= d_height)
        throw std::out_of_range(
            "BoolArray2D::elementAtLocation: "
            "location out of range");

    return d_content[y * d_width + x];
}

//------------------------------------------------------------------------------//
void BoolArray2D::setElementAtLocation(int x, int y, bool value)
{
    if (x < 0 || x >= d_width || y < 0 || y >= d_height)
        throw std::out_of_range(
            "BoolArray2D::setElementAtLocation: "
            "location out of range");

    d_content[y * d_width + x] = value;
}

//------------------------------------------------------------------------------//
void BoolArray2D::clear(bool value /*= false*/)
{
    if (!d_content)
        return;

    const int sz = d_width * d_height;
    for (int i = 0; i < sz; ++i)
        d_content[i] = value;

}

//------------------------------------------------------------------------------//
void BoolArray2D::resetSize(int width, int height)
{
    if (d_width != width || d_height != height)
    {
        delete[] d_content;
        d_width = width;
        d_height = height;
        d_content = new bool[width * height];
    }

    clear();
}

//------------------------------------------------------------------------------//

} // End of  CEGUI namespace section

