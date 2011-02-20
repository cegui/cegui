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

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

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
class CEGUIEXPORT UDim :
    public AllocatedObject<UDim>
{
public:
    UDim() {}
    UDim(float scale, float offset) : d_scale(scale), d_offset(offset) {}
    UDim(const UDim& v): d_scale(v.d_scale), d_offset(v.d_offset) {}
    ~UDim() {}

    UDim operator+(const UDim& other) const
    {
        return UDim(d_scale + other.d_scale, d_offset + other.d_offset);
    }
    UDim operator-(const UDim& other) const
    {
        return UDim(d_scale - other.d_scale, d_offset - other.d_offset);
    }
    UDim operator*(const float val) const
    {
        return UDim(d_scale * val, d_offset * val);
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

// we need to allow UVector2 to be multiplied by floats, this is the most elegant way to do that
inline Vector2<UDim> operator * (const Vector2<UDim>& v, const float c)
{
    return Vector2<UDim>(v.d_x * c, v.d_y * c);
}

typedef Vector2<UDim> UVector2;

// we need to allow URect to be multiplied by floats, this is the most elegant way to do that
inline Rect<UDim> operator * (const Rect<UDim>& v, const float c)
{
    return Rect<UDim>(v.d_min * c, v.d_max * c);
}

typedef Size<UDim> USize;
typedef Rect<UDim> URect;

/*!
\brief
    Class encapsulating the 'Unified Box' - this is usually used for margin

\par
    top, left, right and bottom represent offsets on each edge

\note
    Name taken from W3 'box model'
*/
class CEGUIEXPORT UBox :
    public AllocatedObject<UBox>
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
    
    UBox operator*(const float val) const
    {
        return UBox(
                   d_top * val, d_left * val,
                   d_bottom * val, d_right * val);
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


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIUDim_h_

