/***********************************************************************
    created:    19th August 2015
    author:     Lukas Meindl (based on code by Paul D Turner)
    
    purpose:    Defines interface for Sizef class
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
#ifndef _CEGUISizef_h_
#define _CEGUISizef_h_

#include "CEGUI/StreamHelper.h"
#include "CEGUI/Exceptions.h"
#include "AspectMode.h"
#include <glm/glm.hpp>

namespace CEGUI
{

/*!
\brief
    Class that holds a size (width & height) in float values.
*/
class Sizef
{
public:
    Sizef(): d_width(0), d_height(0) {}

    Sizef(const float width, const float height):
        d_width(width),
        d_height(height)
    {}

    Sizef(const Sizef& v):
        d_width(v.d_width),
        d_height(v.d_height)
    {}

    inline Sizef& operator=(const Sizef& other)
    {
        d_width = other.d_width;
        d_height = other.d_height;
        return *this;
    }

    bool operator==(const Sizef& other) const
    {
        return d_width == other.d_width && d_height == other.d_height;
    }

    bool operator!=(const Sizef& other) const
    {
        return !operator==(other);
    }

    Sizef operator*(float c) const
    {
        return Sizef(d_width * c, d_height * c);
    }

    Sizef& operator*=(float c)
    {
        d_width *= c;
        d_height *= c;
        return *this;
    }

    Sizef operator*(const Sizef& s) const
    {
        return Sizef(d_width * s.d_width, d_height * s.d_height);
    }

    Sizef operator*(const glm::vec2& vec) const
    {
        return Sizef(d_width * vec.x, d_height * vec.y);
    }

    Sizef& operator*=(const glm::vec2& vec)
    {
        d_width *= vec.x;
        d_height *= vec.y;
        return *this;
    }

    Sizef operator+(const Sizef& s) const
    {
        return Sizef(d_width + s.d_width, d_height + s.d_height);
    }

    Sizef operator-(const Sizef& s) const
    {
        return Sizef(d_width - s.d_width, d_height - s.d_height);
    }

    /*!
    \brief Writes a Sizef to a stream
    */
    friend std::ostream& operator << (std::ostream& s, const Sizef& val)
    {
        s << "w:" << val.d_width << " h:" << val.d_height;
        
        return s;
    }

    /*!
    \brief Extracts a Sizef from a stream
    */
    friend std::istream& operator >> (std::istream& s, Sizef& val)
    {
        s >> MandatoryString(" w :") >> val.d_width >> MandatoryString(" h :") >> val.d_height;

        return s;
    }

    void clamp(const Sizef& min, const Sizef& max)
    {
        assert(min.d_width <= max.d_width);
        assert(min.d_height <= max.d_height);
        
        if (d_width < min.d_width)
            d_width = min.d_width;
        else if (d_width > max.d_width)
            d_width = max.d_width;
        
        if (d_height < min.d_height)
            d_height = min.d_height;
        else if (d_height > max.d_height)
            d_height = max.d_height;
    }

    void scaleToAspect(AspectMode mode, float ratio)
    {
        if (mode == AspectMode::Ignore)
            return;

        if(d_width <= 0 && d_height <= 0)
            return;

        assert(ratio > 0);

        const float expectedWidth = d_height * ratio;
        bool keepHeight;
        switch (mode)
        {
        case AspectMode::Shrink:
            keepHeight = expectedWidth <= d_width;
            break;
        case AspectMode::Expand:
            keepHeight = expectedWidth >= d_width;
            break;
        case AspectMode::AdjustWidth:
            keepHeight = true;
            break;
        case AspectMode::AdjustHeight:
            keepHeight = false;
            break;
        default:
            throw(InvalidRequestException("Invalid aspect mode."));
        }
        if (keepHeight)
        {
            d_width = expectedWidth;
        }
        else
        {
            d_height = d_width / ratio;
        }
    }

    //! \brief finger saving alias for Sizef(side, side)
    static Sizef square(const float side)
    {
        return Sizef(side, side);
    }

    //! \brief finger saving alias for Sizef(0, 0)
    static Sizef zero()
    {
        return square(0.0f);
    }
    
    //! \brief finger saving alias for Sizef(1, 1)
    static Sizef one()
    {
        return square(1.0f);
    }
    
    //! \brief finger saving alias for Sizef(1, 0)
    static Sizef one_width()
    {
        return Sizef(1.0f, 0.0f);
    }
    
    //! \brief finger saving alias for Sizef(0, 1)
    static Sizef one_height()
    {
        return Sizef(0.0f, 1.0f);
    }

    float d_width;
    float d_height;
};


} // End of  CEGUI namespace section

#endif
