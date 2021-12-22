/***********************************************************************
	created:	13/2/2011
	author:		Martin Preisler (reworked from code by Paul D Turner)
	
	purpose:	Defines interfaces for UVector classes
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
#ifndef _CEGUIUVector_h_
#define _CEGUIUVector_h_

#include "CEGUI/UDim.h"

namespace CEGUI
{

/*!
\brief
    Class used as a two dimensional vector (aka a Point)

\par
    A Vector class featuring two UDims as elements.
*/
class UVector2
{
public:
    inline UVector2()
    {}
    
    inline UVector2(const UDim x, const UDim y):
        d_x(x),
        d_y(y)
    {}

    inline UVector2(const UVector2& v):
        d_x(v.d_x),
        d_y(v.d_y)
    {}

    inline UVector2& operator=(const UVector2& other)
    {
        d_x = other.d_x;
        d_y = other.d_y;
        return *this;
    }

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

    inline UVector2 operator* (const float c) const
    {
        return UVector2(d_x * c, d_y * c);
    }

    inline UVector2 operator/(const UVector2& vec) const
    {
        return UVector2(d_x / vec.d_x, d_y / vec.d_y);
    }

    inline UVector2 operator*(const UDim c) const
    {
        return UVector2(d_x * c, d_y * c);
    }

    inline UVector2& operator*=(const UDim c)
    {
        d_x *= c;
        d_y *= c;

        return *this;
    }

    inline UVector2 operator/(const UDim c) const
    {
        return UVector2(d_x / c, d_y / c);
    }

    inline UVector2 operator/=(const UDim c)
    {
        d_x /= c;
        d_y /= c;

        return *this;
    }

    inline bool operator==(const UVector2& vec) const
    {
        return ((d_x == vec.d_x) && (d_y == vec.d_y));
    }

    inline bool operator!=(const UVector2& vec) const
    {
        return !(operator==(vec));
    }

    inline bool isAbsolute() const
    {
        return d_x.isAbsolute() && d_y.isAbsolute();
    }
    
    /*!
    \brief Writes a Vector2 to a stream
    */
    inline friend std::ostream& operator << (std::ostream& s, const UVector2& val)
    {
        s << val.d_x << "," << val.d_y;
        return s;
    }

    /*!
    \brief Extracts a UVector2 from a stream
    */
    inline friend std::istream& operator >> (std::istream& s, UVector2& val)
    {
        s >> optionalChar<'{'> >> val.d_x >> optionalChar<','> >> val.d_y >> optionalChar<'}'>;
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

/*!
\brief
    Class used as a two dimensional vector (aka a Point)

\par
    A Vector class featuring two UDims as elements.
*/
class UVector3
{
public:
    inline UVector3()
    {}
    
    inline UVector3(const UDim x, const UDim y, const UDim z):
        d_x(x),
        d_y(y),
        d_z(z)
    {}

    inline UVector3(const UVector3& v):
        d_x(v.d_x),
        d_y(v.d_y),
        d_z(v.d_z)
    {}

    inline UVector3& operator=(const UVector3& other)
    {
        d_x = other.d_x;
        d_y = other.d_y;
        d_z = other.d_z;
        return *this;
    }

    inline UVector3& operator*=(const UVector3& vec)
    {
        d_x *= vec.d_x;
        d_y *= vec.d_y;
        d_z *= vec.d_z;

        return *this;
    }

    inline UVector3& operator/=(const UVector3& vec)
    {
        d_x /= vec.d_x;
        d_y /= vec.d_y;
        d_z /= vec.d_z;

        return *this;
    }

    inline UVector3& operator+=(const UVector3& vec)
    {
        d_x += vec.d_x;
        d_y += vec.d_y;
        d_z += vec.d_z;

        return *this;
    }

    inline UVector3& operator-=(const UVector3& vec)
    {
        d_x -= vec.d_x;
        d_y -= vec.d_y;
        d_z -= vec.d_z;

        return *this;
    }

    inline UVector3 operator+(const UVector3& vec) const
    {
        return UVector3(d_x + vec.d_x, d_y + vec.d_y, d_z + vec.d_z);
    }

    inline UVector3 operator-(const UVector3& vec) const
    {
        return UVector3(d_x - vec.d_x, d_y - vec.d_y, d_z - vec.d_z);
    }

    inline UVector3 operator*(const UVector3& vec) const
    {
        return UVector3(d_x * vec.d_x, d_y * vec.d_y, d_z * vec.d_z);
    }

    inline UVector3 operator* (const float c)
    {
        return UVector3(d_x * c, d_y * c, d_z * c);
    }

    inline UVector3 operator/(const UVector3& vec) const
    {
        return UVector3(d_x / vec.d_x, d_y / vec.d_y, d_z / vec.d_z);
    }

    inline UVector3 operator*(const UDim c) const
    {
        return UVector3(d_x * c, d_y * c, d_z * c);
    }

    inline UVector3& operator*=(const UDim c)
    {
        d_x *= c;
        d_y *= c;
        d_z *= c;

        return *this;
    }

    inline UVector3 operator/(const UDim c) const
    {
        return UVector3(d_x / c, d_y / c, d_z / c);
    }

    inline UVector3 operator/=(const UDim c)
    {
        d_x /= c;
        d_y /= c;
        d_z /= c;

        return *this;
    }

    inline bool operator==(const UVector3& vec) const
    {
        return ((d_x == vec.d_x) && (d_y == vec.d_y) && (d_z == vec.d_z));
    }

    inline bool operator!=(const UVector3& vec) const
    {
        return !(operator==(vec));
    }

    inline bool isAbsolute() const
    {
        return d_x.isAbsolute() && d_y.isAbsolute() && d_z.isAbsolute();
    }
    
    /*!
    \brief Writes a Vector3 to a stream
    */
    inline friend std::ostream& operator << (std::ostream& s, const UVector3& val)
    {
        s << val.d_x << "," << val.d_y << "," << val.d_z;
        return s;
    }

    /*!
    \brief Extracts a UVector3 from a stream
    */
    inline friend std::istream& operator >> (std::istream& s, UVector3& val)
    {
        s >> optionalChar<'{'> >> val.d_x >> optionalChar<','> >> val.d_y >> optionalChar<','> >> val.d_z >> optionalChar<'}'>;
        return s;
    }

    //! \brief finger saving alias for UVector3(0, 0)
    inline static UVector3 zero()
    {
        return UVector3(TypeSensitiveZero<UDim>(), TypeSensitiveZero<UDim>(), TypeSensitiveZero<UDim>());
    }

    //! \brief finger saving alias for UVector3(1, 1)
    inline static UVector3 one()
    {
        return UVector3(TypeSensitiveOne<UDim>(), TypeSensitiveOne<UDim>(), TypeSensitiveOne<UDim>());
    }
    
    //! \brief finger saving alias for UVector3(1, 0, 0)
    inline static UVector3 one_x()
    {
        return UVector3(TypeSensitiveOne<UDim>(), TypeSensitiveZero<UDim>(), TypeSensitiveZero<UDim>());
    }
    
    //! \brief finger saving alias for UVector3(0, 1, 0)
    inline static UVector3 one_y()
    {
        return UVector3(TypeSensitiveZero<UDim>(), TypeSensitiveOne<UDim>(), TypeSensitiveZero<UDim>());
    }
    
    //! \brief finger saving alias for UVector3(0, 0, 1)
    inline static UVector3 one_z()
    {
        return UVector3(TypeSensitiveZero<UDim>(), TypeSensitiveZero<UDim>(), TypeSensitiveOne<UDim>());
    }

    UDim d_x;
    UDim d_y;
    UDim d_z;
};

} // End of  CEGUI namespace section

#endif
