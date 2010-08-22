/***********************************************************************
	filename: 	CEGUIVector.h
	created:	14/3/2004
	author:		Paul D Turner
	
	purpose:	Defines interfaces for Vector classes
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
#ifndef _CEGUIVector_h_
#define _CEGUIVector_h_

#include "CEGUIBase.h"
#include "CEGUISize.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
 Class used as a two dimensional vector (aka a Point)
*/
class CEGUIEXPORT Vector2
{
public:
    Vector2(void) {}
    Vector2(float x, float y) : d_x(x), d_y(y) {}
    Vector2(const Vector2& v) : d_x(v.d_x), d_y(v.d_y) {}

    Vector2& operator*=(const Vector2& vec)
    {
        d_x *= vec.d_x;
        d_y *= vec.d_y;

        return *this;
    }

    Vector2& operator/=(const Vector2& vec)
    {
        d_x /= vec.d_x;
        d_y /= vec.d_y;

        return *this;
    }

    Vector2& operator+=(const Vector2& vec)
    {
        d_x += vec.d_x;
        d_y += vec.d_y;

        return *this;
    }

    Vector2& operator-=(const Vector2& vec)
    {
        d_x -= vec.d_x;
        d_y -= vec.d_y;

        return *this;
    }

    Vector2 operator+(const Vector2& vec) const
    {
        return Vector2(d_x + vec.d_x, d_y + vec.d_y);
    }

    Vector2 operator-(const Vector2& vec) const
    {
        return Vector2(d_x - vec.d_x, d_y - vec.d_y);
    }

    Vector2 operator*(const Vector2& vec) const
    {
        return Vector2(d_x * vec.d_x, d_y * vec.d_y);
    }

	Vector2 operator*(float c) const
    {
        return Vector2(d_x * c, d_y * c);
    }

    bool operator==(const Vector2& vec) const
    {
        return ((d_x == vec.d_x) && (d_y == vec.d_y));
    }

    bool operator!=(const Vector2& vec) const
    {
        return !(operator==(vec));
    }

    Size asSize() const     { return Size(d_x, d_y); }

    float d_x, d_y;
};

/*!
\brief
 Point class
*/
typedef Vector2  Point;


/*!
\brief
 Class used as a three dimensional vector
*/
class CEGUIEXPORT Vector3
{
public:
    Vector3(void) {}
    Vector3(float x, float y, float z) : d_x(x), d_y(y), d_z(z) {}
    Vector3(const Vector3& v) : d_x(v.d_x), d_y(v.d_y), d_z(v.d_z) {}

    bool operator==(const Vector3& vec) const
    {
        return ((d_x == vec.d_x) && (d_y == vec.d_y) && (d_z == vec.d_z));
    }

    bool operator!=(const Vector3& vec) const
    {
        return !(operator==(vec));
    }

	Vector3 operator*(float c) const
	{
		return Vector3(d_x * c, d_y * c, d_z * c);
	}

	Vector3 operator+(const Vector3& v) const
	{
		return Vector3(d_x + v.d_x, d_y + v.d_y, d_z + v.d_z);
	}

    float d_x, d_y, d_z;
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIVector_h_

