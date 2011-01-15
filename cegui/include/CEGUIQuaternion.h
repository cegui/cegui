/***********************************************************************
	filename: 	CEGUIQuaternion.h
	created:	2/1/2011
	author:		Martin Preisler
	
	purpose:	Defines interface for the Quaternion class
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
#ifndef _CEGUIQuaternion_h_
#define _CEGUIQuaternion_h_

#include "CEGUIBase.h"
#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Class to represent rotation, avoids Gimbal lock
*/
class CEGUIEXPORT Quaternion :
    public AllocatedObject<Quaternion>
{
public:
    inline Quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f):
        d_w(w),
        d_x(x),
        d_y(y),
        d_z(z)
	{}

    inline Quaternion(const Quaternion& v):
        d_w(v.d_w),
        d_x(v.d_x),
        d_y(v.d_y),
        d_z(v.d_z)
    {}

    static Quaternion eulerAnglesRadians(float x, float y, float z);
    static Quaternion eulerAnglesDegrees(float x, float y, float z);
    
    inline Quaternion& operator = (const Quaternion& v)
	{
		d_w = v.d_w;
		d_x = v.d_x;
		d_y = v.d_y;
		d_z = v.d_z;

		return *this;
	}

    inline bool operator == (const Quaternion& v) const
	{
		return (d_w == v.d_w) && (d_x == v.d_x) && (d_y == v.d_y) && (d_z == v.d_z);
	}

    inline bool operator != (const Quaternion& v) const
	{
		return (d_w != v.d_w) || (d_x != v.d_x) || (d_y != v.d_y) || (d_z != v.d_z);
	}

    inline Quaternion operator - () const
    {
        return Quaternion(-d_w, -d_x, -d_y, -d_z);
    }

    inline Quaternion operator * (float v) const
    {
        return Quaternion(d_w * v, d_x * v, d_y * v, d_z * v);
    }

    inline friend Quaternion operator * (float v, const Quaternion& q)
    {
        return Quaternion(v * q.d_w, v * q.d_x, v * q.d_y, v * q.d_z);
    }

    inline float dot(const Quaternion& v) const
    {
        return d_w * v.d_w + d_x * v.d_x + d_y * v.d_y + d_z * v.d_z;
    }

    inline Quaternion operator + (const Quaternion& v) const
    {
        return Quaternion(d_w + v.d_w, d_x + v.d_x, d_y + v.d_y, d_z + v.d_z);
    }

    inline float length() const
    {
        return sqrtf((d_w * d_w) + (d_x * d_x) + (d_y * d_y) + (d_z * d_z));
    }

    inline float normalise()
    {
        const float len = length();
        const float factor = 1.0f / len;
        *this = *this * factor;

        return len;
    }

    static Quaternion slerp(float t, const Quaternion& left, const Quaternion& right, bool shortestPath = false);

    static const Quaternion ZERO;
    static const Quaternion IDENTITY;

	float d_w;
    float d_x;
    float d_y;
    float d_z;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIQuaternion_h_
