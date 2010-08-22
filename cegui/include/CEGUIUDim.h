/***********************************************************************
    filename:   CEGUIUDim.h
    created:    Tue May 31 2005
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
#ifndef _CEGUIUDim_h_
#define _CEGUIUDim_h_

#include "CEGUIRect.h"
#include "CEGUIVector.h"

// some macros to aid in the creation of UDims
#define cegui_absdim(x)     CEGUI::UDim(0,(x))
#define cegui_reldim(x)     CEGUI::UDim((x),0)


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class representing a unified dimension; that is a dimension that has both
    a relative 'scale' portion and and absolute 'offset' portion.
*/
class CEGUIEXPORT UDim
{
public:
    UDim() {}
    UDim(float scale, float offset) : d_scale(scale), d_offset(offset) {}
    UDim(const UDim& v): d_scale(v.d_scale), d_offset(v.d_offset) {}
    ~UDim() {}

    float asAbsolute(float base) const
    {
        return PixelAligned(base * d_scale) + d_offset;
    }
    float asRelative(float base) const
    {
        return (base != 0.0f) ? d_offset / base + d_scale : 0.0f;
    }

    UDim operator+(const UDim& other) const
    {
        return UDim(d_scale + other.d_scale, d_offset + other.d_offset);
    }
    UDim operator-(const UDim& other) const
    {
        return UDim(d_scale - other.d_scale, d_offset - other.d_offset);
    }
    UDim operator*(const UDim& other) const
    {
        return UDim(d_scale * other.d_scale, d_offset * other.d_offset);
    }
    UDim operator/(const UDim& other) const
    {
        // division by zero sets component to zero.  Not technically correct
        // but probably better than exceptions and/or NaN values.
        return UDim(other.d_scale == 0.0f ? 0.0f : d_scale / other.d_scale,
                    other.d_offset == 0.0f ? 0.0f : d_offset / other.d_offset);
    }

    const UDim& operator+=(const UDim& other)
    {
        d_scale += other.d_scale;
        d_offset += other.d_offset;
        return *this;
    }
    const UDim& operator-=(const UDim& other)
    {
        d_scale -= other.d_scale;
        d_offset -= other.d_offset;
        return *this;
    }
    const UDim& operator*=(const UDim& other)
    {
        d_scale *= other.d_scale;
        d_offset *= other.d_offset;
        return *this;
    }
    const UDim& operator/=(const UDim& other)
    {
        // division by zero sets component to zero.  Not technically correct
        // but probably better than exceptions and/or NaN values.
        d_scale = (other.d_scale == 0.0f ? 0.0f : d_scale / other.d_scale);
        d_offset = (other.d_offset == 0.0f ? 0.0f : d_offset / other.d_offset);
        return *this;
    }

    bool operator==(const UDim& other) const
    {
        return d_scale == other.d_scale && d_offset == other.d_offset;
    }
    bool operator!=(const UDim& other) const
    {
        return !operator==(other);
    }

    float d_scale, d_offset;
};

/*!
\brief
    Two dimensional vector class built using unified dimensions (UDims).
    The UVector2 class is used for representing both positions and sizes.
*/
class CEGUIEXPORT UVector2
{
public:
    UVector2() {}
    UVector2(const UDim& x, const UDim& y) : d_x(x), d_y(y) {}
    UVector2(const UVector2& v): d_x(v.d_x), d_y(v.d_y) {}
    ~UVector2() {}

    Vector2 asAbsolute(const Size& base) const
    {
        return Vector2(d_x.asAbsolute(base.d_width), d_y.asAbsolute(base.d_height));
    }
    Vector2 asRelative(const Size& base) const
    {
        return Vector2(d_x.asRelative(base.d_width), d_y.asRelative(base.d_height));
    }

    UVector2 operator+(const UVector2& other) const
    {
        return UVector2(d_x + other.d_x, d_y + other.d_y);
    }
    UVector2 operator-(const UVector2& other) const
    {
        return UVector2(d_x - other.d_x, d_y - other.d_y);
    }
    UVector2 operator/(const UVector2& other) const
    {
        return UVector2(d_x / other.d_x, d_y / other.d_y);
    }
    UVector2 operator*(const UVector2& other) const
    {
        return UVector2(d_x * other.d_x, d_y * other.d_y);
    }

    const UVector2& operator+=(const UVector2& other)
    {
        d_x += other.d_x;
        d_y += other.d_y;
        return *this;
    }
    const UVector2& operator-=(const UVector2& other)
    {
        d_x -= other.d_x;
        d_y -= other.d_y;
        return *this;
    }
    const UVector2& operator/=(const UVector2& other)
    {
        d_x /= other.d_x;
        d_y /= other.d_y;
        return *this;
    }
    const UVector2& operator*=(const UVector2& other)
    {
        d_x *= other.d_x;
        d_y *= other.d_y;
        return *this;
    }

    UVector2 operator+(const UDim& dim) const
    {
        return UVector2(d_x + dim, d_y + dim);
    }
    UVector2 operator-(const UDim& dim) const
    {
        return UVector2(d_x - dim, d_y - dim);
    }
    UVector2 operator/(const UDim& dim) const
    {
        return UVector2(d_x / dim, d_y / dim);
    }
    UVector2 operator*(const UDim& dim) const
    {
        return UVector2(d_x * dim, d_y * dim);
    }

    const UVector2& operator+=(const UDim& dim)
    {
        d_x += dim;
        d_y += dim;
        return *this;
    }
    const UVector2& operator-=(const UDim& dim)
    {
        d_x -= dim;
        d_y -= dim;
        return *this;
    }
    const UVector2& operator/=(const UDim& dim)
    {
        d_x /= dim;
        d_y /= dim;
        return *this;
    }
    const UVector2& operator*=(const UDim& dim)
    {
        d_x *= dim;
        d_y *= dim;
        return *this;
    }

    bool operator==(const UVector2& other) const
    {
        return d_x == other.d_x && d_y == other.d_y;
    }
    bool operator!=(const UVector2& other) const
    {
        return !operator==(other);
    }

    UDim d_x, d_y;
};

/*!
\brief
    Area rectangle class built using unified dimensions (UDims).
*/
class CEGUIEXPORT URect
{
public:
    URect() {}

    URect(const UVector2& min, const UVector2& max) : d_min(min), d_max(max) {}

    URect(const UDim& left, const UDim& top, const UDim& right, const UDim& bottom)
    {
        d_min.d_x = left;
        d_min.d_y = top;
        d_max.d_x = right;
        d_max.d_y = bottom;
    }

    URect(const URect& v): d_min(v.d_min), d_max(v.d_max) {}

    ~URect() {}

    Rect asAbsolute(const Size& base) const
    {
        return Rect(
                   d_min.d_x.asAbsolute(base.d_width),
                   d_min.d_y.asAbsolute(base.d_height),
                   d_max.d_x.asAbsolute(base.d_width),
                   d_max.d_y.asAbsolute(base.d_height)
               );
    }

    Rect asRelative(const Size& base) const
    {
        return Rect(
                   d_min.d_x.asRelative(base.d_width),
                   d_min.d_y.asRelative(base.d_height),
                   d_max.d_x.asRelative(base.d_width),
                   d_max.d_y.asRelative(base.d_height)
               );
    }

    const UVector2& getPosition() const
    {
        return d_min;
    }
    UVector2 getSize() const
    {
        return d_max - d_min;
    }
    UDim getWidth() const
    {
        return d_max.d_x - d_min.d_x;
    }
    UDim getHeight() const
    {
        return d_max.d_y - d_min.d_y;
    }

    void setPosition(const UVector2& pos)
    {
        UVector2 sz(d_max - d_min);
        d_min = pos;
        d_max = d_min + sz;
    }

    void setSize(const UVector2& sz)
    {
        d_max = d_min + sz;
    }

    void setWidth(const UDim& w)
    {
        d_max.d_x = d_min.d_x + w;
    }
    void setHeight(const UDim& h)
    {
        d_max.d_y = d_min.d_y + h;
    }

    void offset(const UVector2& sz)
    {
        d_min += sz;
        d_max += sz;
    }

    URect operator*(const UDim& dim) const
    {
        return URect(d_min * dim, d_max * dim);
    }

    URect operator+(const URect& r) const
    {
        return URect(d_min + r.d_min, d_max + r.d_max);
    }

    UVector2 d_min, d_max;
};

/*!
\brief
    Class encapsulating the 'Unified Box' - this is usually used for margin

\par
    top, left, right and bottom represent offsets on each edge

\note
    Name taken from W3 'box model'
*/
class CEGUIEXPORT UBox
{
public:
    UBox():
            d_top(),
            d_left(),
            d_bottom(),
            d_right()
    {}

    UBox(const UDim& margin):
            d_top(margin),
            d_left(margin),
            d_bottom(margin),
            d_right(margin)
    {}

    UBox(const UDim& top, const UDim& left, const UDim& bottom, const UDim& right):
            d_top(top),
            d_left(left),
            d_bottom(bottom),
            d_right(right)
    {}

    UBox(const UBox& b):
            d_top(b.d_top),
            d_left(b.d_left),
            d_bottom(b.d_bottom),
            d_right(b.d_right)
    {}

    /*************************************************************************
        Operators
    *************************************************************************/
    bool operator==(const UBox& rhs) const
    {
        return ((d_top == rhs.d_top) &&
                (d_left == rhs.d_left) &&
                (d_bottom == rhs.d_bottom) &&
                (d_right == rhs.d_right));
    }

    bool operator!=(const UBox& rhs) const
    {
        return !operator==(rhs);
    }

    UBox& operator=(const UBox& rhs)
    {
        d_top = rhs.d_top;
        d_left = rhs.d_left;
        d_bottom = rhs.d_bottom;
        d_right = rhs.d_right;

        return *this;
    }

    UBox operator*(const UDim& dim) const
    {
        return UBox(
                   d_top * dim, d_left * dim,
                   d_bottom * dim, d_right * dim);
    }

    UBox operator+(const UBox& b) const
    {
        return UBox(
                   d_top + b.d_top, d_left + b.d_left,
                   d_bottom + b.d_bottom, d_right + b.d_right);
    }

    /*************************************************************************
        Data Fields
    *************************************************************************/
    UDim d_top;
    UDim d_left;
    UDim d_bottom;
    UDim d_right;
};

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIUDim_h_

