/***********************************************************************
    created:    19th August 2015
    author:     Lukas Meindl (reworked from code by Paul D Turner and Martin Preisler)
    
    purpose:    Defines 'URect' class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIURect_h_
#define _CEGUIURect_h_

#include "CEGUI/UVector.h"
#include "CEGUI/USize.h"

namespace CEGUI
{
/*!
\brief
    Class encapsulating operations on a Rectangle based on four UDim values
*/
class URect
{
public:
    inline URect()
    {}

    inline URect(const UDim& left, const UDim& top, const UDim& right, const UDim& bottom):
        d_min(left, top),
        d_max(right, bottom)
    {}

    inline URect(const UVector2& min, const UVector2& max):
        d_min(min),
        d_max(max)
    {}

    inline URect(const UVector2& pos, const USize& size):
        d_min(pos),
        d_max(pos + UVector2(size.d_width, size.d_height))
    {}

    inline URect(const URect& r):
        d_min(r.d_min),
        d_max(r.d_max)
    {}

    inline URect& operator=(const URect& rhs)
    {
        d_min = rhs.d_min;
        d_max = rhs.d_max;

        return *this;
    }

    inline void left(const UDim& v)
    {
        d_min.d_x = v;
    }

    inline const UDim& left() const
    {
        return d_min.d_x;
    }

    inline void top(const UDim& v)
    {
        d_min.d_y = v;
    }

    inline const UDim& top() const
    {
        return d_min.d_y;
    }

    inline void right(const UDim& v)
    {
        d_max.d_x = v;
    }

    inline const UDim& right() const
    {
        return d_max.d_x;
    }

    inline void bottom(const UDim& v)
    {
        d_max.d_y = v;
    }

    inline const UDim& bottom() const
    {
        return d_max.d_y;
    }

    /*!
    \brief
        set the position of the URect (leaves size in tact)
    */
    void setPosition(const UVector2& min)
    {
        const USize size = getSize();
        d_min = min;
        setSize(size);
    }

    /*!
    \brief
        Return top-left position of URect as a Vector2
    */
    const UVector2& getPosition() const
    {
        return d_min;
    }

    void setSize(const USize& size)
    {
        d_max = d_min + UVector2(size.d_width, size.d_height);
    }

    /*!
    \brief
        return the size of the URect area
    */
    inline USize getSize() const
    {
        return USize(getWidth(), getHeight());
    }

    inline void setPositionAndSize(const UVector2& pos, const USize& size)
    {
        d_min = pos;
        d_max = pos + UVector2(size.d_width, size.d_height);
    }

    void setWidth(const UDim& w)
    {
        d_max.d_x = d_min.d_x + w;
    }

    /*!
    \brief
        return width of URect area
    */
    inline UDim getWidth() const
    {
        return d_max.d_x - d_min.d_x;
    }

    void setHeight(const UDim& h)
    {
        d_max.d_y = d_min.d_y + h;
    }

    /*!
    \brief
        return height of URect area
    */
    inline UDim getHeight() const
    {
        return d_max.d_y - d_min.d_y;
    }

    /*!
    \brief
        Applies an offset the URect object

    \param pt
        Vector2 object containing the offsets to be applied to the URect.

    \return
        this URect after the offset is performed
    */
    inline void offset(const UVector2& v)
    {
        d_min += v;
        d_max += v;
    }



    /*************************************************************************
        Operators
    *************************************************************************/
    inline bool operator==(const URect& rhs) const
    {
        return ((d_min == rhs.d_min) && (d_max == rhs.d_max));
    }

    inline bool operator!=(const URect& rhs) const
    {
        return !operator==(rhs);
    }

    inline URect operator*(UDim scalar) const
    {
        return URect(d_min * scalar, d_max * scalar);
    }

    inline URect operator*(UVector2 vector) const
    {
        return URect(d_min * vector.d_x, d_max * vector.d_y);
    }

    inline URect operator * (const float c)
    {
        return URect(d_min * c, d_max * c);
    }

    const URect& operator*=(UDim scalar)
    {
        d_min *= scalar;
        d_max *= scalar;
        return *this;
    }

    URect operator+(const URect& r) const
    {
        return URect(d_min + r.d_min, d_max + r.d_max);
    }

    URect operator-(const URect& r) const
    {
        return URect(d_min - r.d_min, d_max - r.d_max);
    }

    inline bool isAbsolute() const
    {
        return d_min.isAbsolute() && d_max.isAbsolute();
    }

    /*!
    \brief Writes a URect to a stream
    */
    inline friend std::ostream& operator << (std::ostream& s, const URect& val)
    {
        s << "{" << val.d_min << "," << val.d_max << "}";
        return s;
    }

    /*!
    \brief Extracts a URect from a stream
    */
    inline friend std::istream& operator >> (std::istream& s, URect& val)
    {
        // Format is:  { { %g , %g } , { %g , %g } , { %g , %g } , { %g , %g } }" 
        // but we are lenient regarding the format, so this is also allowed: " { %g %g } { %g %g } { %g %g } { %g %g }"
        s >> optionalChar<'{'> >> val.d_min >> optionalChar<','> >> val.d_max >> optionalChar<'}'>;
        return s;
    }
    
    //! \brief finger saving alias for zero sized, zero positioned rect
    inline static URect zero()
    {
        return URect(UVector2::zero(), USize::zero());
    }
    
    /*************************************************************************
        Data Fields
    *************************************************************************/
    UVector2 d_min;
    UVector2 d_max;

    // d_min.d_x is former d_left
    // d_min.d_y is former d_top
    // d_max.d_x is former d_right
    // d_max.d_y is former d_bottom
};

} // End of  CEGUI namespace section


#endif

