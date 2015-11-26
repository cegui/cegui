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
#ifndef _BoolArray2D_h_
#define _BoolArray2D_h_

// Start of CEGUI namespace section
namespace CEGUI
{

// Wrapper for a simple bool array.
class BoolArray2D
{
public:
    BoolArray2D();
    BoolArray2D(int width, int height);
    ~BoolArray2D();

    // return the width of the array
    int width() const;

    // return the height of the array.
    int height() const;

    // get the element at the specified location.
    bool elementAtLocation(int x, int y) const;

    // set the element at the specified location.
    void setElementAtLocation(int x, int y, bool value);

    // clear the array to the specified value.
    void clear(bool value = false);

    // set the array size.  content is cleared to 0.
    void resetSize(int width, int height);

private:
    int d_width;
    int d_height;
    bool* d_content;
};

} // End of  CEGUI namespace section

#endif  // end of guard _BoolArray2D_h_

