/***********************************************************************
	filename: 	CEGUIVector.h
	created:	13/2/2011
	author:		Martin Preisler (reworked from code by Paul D Turner)
	
	purpose:	Defines interfaces for Vector classes
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
#ifndef _CEGUIUVector2_h_
#define _CEGUIUVector2_h_

#include "CEGUI/UDim.h"
#include "CEGUI/GLM.h"
#include <typeinfo>
#include <ostream>

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Class used as a two dimensional vector (aka a Point)

\par
    This class is templated now, this allows us to use it as a Vector2 of floats,
    ints or even UDims without replicating the code all over the place.

\par
    For a simple Vector2 of floats (what was called Vector2 before), use glm::vec2
    as the T template parameter defaults to float to save fingertips.
*/
class UVector2:
    public AllocatedObject<UVector2>
{
public:
    typedef UDim value_type;

    inline UVector2()
    {}

    inline UVector2(const UDim& x, const UDim& y):
        d_x(x),
        d_y(y)
    {}

    inline UVector2(const UVector2& v):
        d_x(v.d_x),
        d_y(v.d_y)
    {}

    inline UVector2& operator*=(const UVector2& vec)
    {
        d_x *= vec.d_x;
        d_y *= vec.d_y;

        return *this;
    }

    inline UVector2& operator/=(const UVector2& vec)
    {
        d_x /= vec.d_x;
        d_y /= vec.d_y;

        return *this;
    }

    inline UVector2& operator+=(const UVector2& vec)
    {
        d_x += vec.d_x;
        d_y += vec.d_y;

        return *this;
    }

    inline UVector2& operator-=(const UVector2& vec)
    {
        d_x -= vec.d_x;
        d_y -= vec.d_y;

        return *this;
    }

    inline UVector2 operator+(const UVector2& vec) const
    {
        return UVector2(d_x + vec.d_x, d_y + vec.d_y);
    }

    inline UVector2 operator-(const UVector2& vec) const
    {
        return UVector2(d_x - vec.d_x, d_y - vec.d_y);
    }

    inline UVector2 operator*(const UVector2& vec) const
    {
        return UVector2(d_x * vec.d_x, d_y * vec.d_y);
    }

    inline UVector2 operator/(const UVector2& vec) const
    {
        return UVector2(d_x / vec.d_x, d_y / vec.d_y);
    }

    inline UVector2 operator*(const UDim& c) const
    {
        return UVector2(d_x * c, d_y * c);
    }

    inline UVector2& operator*=(const UDim& c)
    {
        d_x *= c;
        d_y *= c;

        return *this;
    }

    inline UVector2 operator/(const UDim& c) const
    {
        return UVector2(d_x / c, d_y / c);
    }

    inline bool operator==(const UVector2& vec) const
    {
        return ((d_x == vec.d_x) && (d_y == vec.d_y));
    }

    inline bool operator!=(const UVector2& vec) const
    {
        return !(operator==(vec));
    }

    /*!
    \brief allows writing the vector2 to std ostream
    */
    inline friend std::ostream& operator << (std::ostream& s, const UVector2& v)
    {
        s << "CEGUI::UVector2(" << v.d_x << ", " << v.d_y << ")";
        return s;
    }

    //! \brief finger saving alias for UVector2(0, 0)
    inline static UVector2 zero()
    {
        return UVector2(TypeSensitiveZero<UDim>(), TypeSensitiveZero<UDim>());
    }

    //! \brief finger saving alias for UVector2(1, 1)
    inline static UVector2 one()
    {
        return UVector2(TypeSensitiveOne<UDim>(), TypeSensitiveOne<UDim>());
    }
    
    //! \brief finger saving alias for UVector2(1, 0)
    inline static UVector2 one_x()
    {
        return UVector2(TypeSensitiveOne<UDim>(), TypeSensitiveZero<UDim>());
    }
    
    //! \brief finger saving alias for UVector2(0, 1)
    inline static UVector2 one_y()
    {
        return UVector2(TypeSensitiveZero<UDim>(), TypeSensitiveOne<UDim>());
    }

    UDim d_x;
    UDim d_y;
};

// we need to allow UVector2 to be multiplied by floats, this is the most elegant way to do that
inline UVector2 operator * (const UVector2& v, const float c)
{
    return UVector2(v.d_x * c, v.d_y * c);
}

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIVector_h_
