/***********************************************************************
    created:    19th August 2015
    author:     Lukas Meindl (based on code by Paul D Turner)
    
    purpose:    Defines interface for USize class
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
#ifndef _CEGUIUSize_h_
#define _CEGUIUSize_h_

#include "CEGUI/UDim.h"
#include <glm/glm.hpp>

namespace CEGUI
{
/*!
\brief
    Class that holds a size (width & height) in UDim values.
*/
class USize
{
public:
    inline USize()
    {}

    inline USize(const UDim width, const UDim height):
        d_width(width),
        d_height(height)
    {}

    inline USize(const USize& v):
        d_width(v.d_width),
        d_height(v.d_height)
    {}

    inline USize& operator=(const USize& other)
    {
        d_width = other.d_width;
        d_height = other.d_height;
        return *this;
    }

    inline bool operator==(const USize& other) const
    {
        return d_width == other.d_width && d_height == other.d_height;
    }

    inline bool operator!=(const USize& other) const
    {
        return !operator==(other);
    }

    inline USize operator*(const UDim c) const
    {
        return USize(d_width * c, d_height * c);
    }

    inline USize operator*(const USize& s) const
    {
        return USize(d_width * s.d_width, d_height * s.d_height);
    }

    inline USize operator*(const glm::vec2& vec) const
    {
        return USize(d_width * vec.x, d_height * vec.y);
    }

    inline USize operator*(const float x)
    {
        return (*this * UDim(x, x));
    }

    inline USize operator+(const USize& s) const
    {
        return USize(d_width + s.d_width, d_height + s.d_height);
    }

    inline USize operator-(const USize& s) const
    {
        return USize(d_width - s.d_width, d_height - s.d_height);
    }

    inline bool isAbsolute() const
    {
        return d_width.isAbsolute() && d_height.isAbsolute();
    }

    /*!
    \brief Writes a USize to a stream
    */
    inline friend std::ostream& operator << (std::ostream& s, const USize& val)
    {
        s << "{" << val.d_width << "," << val.d_height << "}";
        return s;
    }

    /*!
    \brief Extracts a USize from a stream
    */
    inline friend std::istream& operator >> (std::istream& s, USize& val)   
    {
        // Format is: " { { %g , %g } , { %g , %g } } " but we are lenient regarding the format, so this is also allowed: " { %g %g } { %g %g } "
        s >> mandatoryChar<'{'> >> val.d_width >> optionalChar<','> >> val.d_height >> optionalChar<'}'>;
        return s;
    }

    //! \brief finger saving alias for USize(side, side)
    inline static USize square(const UDim side)
    {
        return USize(side, side);
    }

    //! \brief finger saving alias for USize(0, 0)
    inline static USize zero()
    {
        return square(TypeSensitiveZero<UDim>());
    }
    
    //! \brief finger saving alias for USize(1, 1)
    inline static USize one()
    {
        return square(TypeSensitiveOne<UDim>());
    }
    
    //! \brief finger saving alias for USize(1, 0)
    inline static USize one_width()
    {
        return USize(TypeSensitiveOne<UDim>(), TypeSensitiveZero<UDim>());
    }
    
    //! \brief finger saving alias for USize(0, 1)
    inline static USize one_height()
    {
        return USize(TypeSensitiveZero<UDim>(), TypeSensitiveOne<UDim>());
    }

    UDim d_width;
    UDim d_height;
};

} // End of  CEGUI namespace section

#endif
