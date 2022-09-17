/***********************************************************************
    created:    19th August 2015
    author:     Lukas Meindl (reworked from code by Paul D Turner and Martin Preisler)
    
    purpose:    Defines 'Rectf' class
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
#ifndef _CEGUIRectf_h_
#define _CEGUIRectf_h_

#include "CEGUI/Sizef.h"
#include "CEGUI/StreamHelper.h"

#include <glm/glm.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\    
    Class encapsulating operations on a Rectangle based on four floats
*/
class Rectf
{
public:
    Rectf() = default;

    inline Rectf(float left, float top, float right, float bottom):
        d_min(left, top),
        d_max(right, bottom)
    {}

    inline Rectf(const glm::vec2& min, const glm::vec2& max):
        d_min(min),
        d_max(max)
    {}

    inline Rectf(const glm::vec2& pos, const Sizef& size):
        d_min(pos.x, pos.y),
        d_max(pos.x + size.d_width, pos.y + size.d_height)
    {}

    Rectf(const Rectf&) = default;
    Rectf(Rectf&&) noexcept = default;
    Rectf& operator =(const Rectf&) = default;
    Rectf& operator =(Rectf&&) noexcept = default;

    inline void left(float v)
    {
        d_min.x = v;
    }

    inline float left() const
    {
        return d_min.x;
    }

    inline void top(float v)
    {
        d_min.y = v;
    }

    inline float top() const
    {
        return d_min.y;
    }

    inline void right(float v)
    {
        d_max.x = v;
    }

    inline float right() const
    {
        return d_max.x;
    }

    inline void bottom(float v)
    {
        d_max.y = v;
    }

    inline float bottom() const
    {
        return d_max.y;
    }

    /*!
    \    
        set the position of the Rectf (leaves size in tact)
    */
    void setPosition(const glm::vec2& min)
    {
        const Sizef size = getSize();
        d_min = min;
        setSize(size);
    }

    /*!
    \    
        Return top-left position of Rectf as a glm::vec2
    */
    const glm::vec2& getPosition() const
    {
        return d_min;
    }

    void setSize(const Sizef& size)
    {
        d_max = d_min + glm::vec2(size.d_width, size.d_height);
    }

    /*!
    \    
        return the size of the Rectf area
    */
    inline Sizef getSize() const
    {
        return Sizef(getWidth(), getHeight());
    }

    void setWidth(float w)
    {
        d_max.x = d_min.x + w;
    }

    /*!
    \    
        return width of Rectf area
    */
    inline float getWidth() const
    {
        return d_max.x - d_min.x;
    }

    void setHeight(float h)
    {
        d_max.y = d_min.y + h;
    }

    /*!
    \    
        return height of Rectf area
    */
    inline float getHeight() const
    {
        return d_max.y - d_min.y;
    }

    inline void set(const glm::vec2& pos, const Sizef& size)
    {
        d_min.x = pos.x;
        d_min.y = pos.y;
        d_max.x = pos.x + size.d_width;
        d_max.y = pos.y + size.d_height;
    }

    bool empty() const { return d_max.x <= d_min.x || d_max.y <= d_min.y; }

    void round()
    {
        d_min.x = std::round(d_min.x);
        d_min.y = std::round(d_min.y);
        d_max.x = std::round(d_max.x);
        d_max.y = std::round(d_max.y);
    }

    /*!
    \    
        return a Rectf that is the intersection of 'this' Rectf with the Rectf 'rect'

    \note
        It can be assumed that if d_left == d_right, or d_top == d_bottom, or getWidth() == 0, or getHeight() == 0, then
        'this' rect was totally outside 'rect'.
    */
    inline Rectf getIntersection(const Rectf& rect) const
    {
        if ((d_max.x > rect.d_min.x) &&
            (d_min.x < rect.d_max.x) &&
            (d_max.y > rect.d_min.y) &&
            (d_min.y < rect.d_max.y))
        {
            Rectf ret;

            // fill in ret with the intersection
            ret.d_min.x = (d_min.x > rect.d_min.x) ? d_min.x : rect.d_min.x;
            ret.d_max.x = (d_max.x < rect.d_max.x) ? d_max.x : rect.d_max.x;
            ret.d_min.y = (d_min.y > rect.d_min.y) ? d_min.y : rect.d_min.y;
            ret.d_max.y = (d_max.y < rect.d_max.y) ? d_max.y : rect.d_max.y;

            return ret;
        }
        else
        {
            return Rectf(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    /*!
    \brief
        Intersects this rect with other rect in place

    \return
        Whether the rect was changed by an intersection
    */
    bool intersect(const Rectf& other)
    {
        bool changed = false;
        if (d_min.x < other.d_min.x)
        {
            d_min.x = other.d_min.x;
            changed = true;
        }
        if (d_min.y < other.d_min.y)
        {
            d_min.y = other.d_min.y;
            changed = true;
        }
        if (d_max.x > other.d_max.x)
        {
            d_max.x = other.d_max.x;
            changed = true;
        }
        if (d_max.y > other.d_max.y)
        {
            d_max.y = other.d_max.y;
            changed = true;
        }
        return changed;
    }

    /*!
    \    
        Applies an offset the Rectf object

    \param pt
        Vector2 object containing the offsets to be applied to the Rectf.

    \return
        this Rectf after the offset is performed
    */
    inline void offset(const glm::vec2& v)
    {
        d_min += v;
        d_max += v;
    }

    inline void offset(float x, float y)
    {
        d_min.x += x;
        d_min.y += y;
        d_max.x += x;
        d_max.y += y;
    }

    /*!
    \    
        Return true if the given Vector2 falls within this Rectf

    \param pt
        Vector2 object describing the position to test.

    \return
        true if position \a pt is within this Rectf's area, else false
    */
    inline bool isPointInRectf(const glm::vec2& pt) const
    {
        return (d_min.x <= pt.x) &&
            (d_max.x > pt.x) &&
            (d_min.y <= pt.y) &&
            (d_max.y > pt.y);
    }

    glm::vec2 clampPointToRect(const glm::vec2& pt) const
    {
        return glm::vec2(
            glm::clamp(pt.x, d_min.x, d_max.x),
            glm::clamp(pt.y, d_min.y, d_max.y));
    }

    /*!
    \    
        check the size of the Rectf object and if it is bigger than \a sz, resize it so it isn't.

    \param sz
        Size object that describes the maximum dimensions that this Rectf should be limited to.

    \return
        'this' Rectf object after the constrain operation
    */
    void constrainSizeMax(const Sizef& size)
    {
        if (getWidth() > size.d_width)
        {
            setWidth(size.d_width);
        }

        if (getHeight() > size.d_height)
        {
            setHeight(size.d_height);
        }
    }


    /*!
    \    
        check the size of the Rectf object and if it is smaller than \a sz, resize it so it isn't.

    \param sz
        Size object that describes the minimum dimensions that this Rectf should be limited to.

    \return
        'this' Rectf object after the constrain operation
    */
    void constrainSizeMin(const Sizef& size)
    {
        if (getWidth() < size.d_width)
        {
            setWidth(size.d_width);
        }

        if (getHeight() < size.d_height)
        {
            setHeight(size.d_height);
        }
    }


    /*!
    \    
        check the size of the Rectf object and if it is bigger than \a max_sz or smaller than \a min_sz, resize it so it isn't.

    \param max_sz
        Size object that describes the maximum dimensions that this Rectf should be limited to.

    \param min_sz
        Size object that describes the minimum dimensions that this Rectf should be limited to.

    \return
        'this' Rectf object after the constrain operation
    */
    void constrainSize(const Sizef& max_sz, const Sizef& min_sz)
    {
        Sizef curr_sz(getSize());

        if (curr_sz.d_width > max_sz.d_width)
        {
            setWidth(max_sz.d_width);
        }
        else if (curr_sz.d_width < min_sz.d_width)
        {
            setWidth(min_sz.d_width);
        }

        if (curr_sz.d_height > max_sz.d_height)
        {
            setHeight(max_sz.d_height);
        }
        else if (curr_sz.d_height < min_sz.d_height)
        {
            setHeight(min_sz.d_height);
        }
    }


    /*************************************************************************
        Operators
    *************************************************************************/
    inline bool operator==(const Rectf& rhs) const
    {
        return ((d_min == rhs.d_min) && (d_max == rhs.d_max));
    }

    inline bool operator!=(const Rectf& rhs) const
    {
        return !operator==(rhs);
    }

    inline Rectf operator*(float scalar) const
    {
        return Rectf(d_min * scalar, d_max * scalar);
    }

    inline Rectf operator*(const glm::vec2& vector) const
    {
        return Rectf(d_min * vector, d_max * vector);
    }

    Rectf& operator*=(float scalar)
    {
        d_min *= scalar;
        d_max *= scalar;
        return *this;
    }

    Rectf& operator*=(const glm::vec2& vector)
    {
        d_min *= vector;
        d_max *= vector;
        return *this;
    }

    Rectf operator+(const Rectf& r) const
    {
        return Rectf(d_min + r.d_min, d_max + r.d_max);
    }

    Rectf operator-(const Rectf& r) const
    {
        return Rectf(d_min - r.d_min, d_max - r.d_max);
    }

    Rectf& operator+=(const Rectf& r)
    {
        d_min += r.d_min;
        d_max += r.d_max;
        return *this;
    }

    /*!
    \brief Writes a Rectf to a stream
    */
    inline friend std::ostream& operator << (std::ostream& s, const Rectf& val)
    {
        s << "l:" << val.d_min.x << " t:" << val.d_min.y << " r:" << val.d_max.x << " b:" << val.d_max.y;
        
        return s;
    }

    /*!
    \brief Extracts a Rectf from a stream
    */
    inline friend std::istream& operator >> (std::istream& s, Rectf& val)
    {
        s >> MandatoryString(" l :") >> val.d_min.x >> MandatoryString(" t :") >> val.d_min.y >>
            MandatoryString(" r :") >> val.d_max.x >> MandatoryString(" b :") >> val.d_max.y;
        return s;
    }
    
    //! \     finger saving alias for zero sized, zero positioned rect
    inline static Rectf zero()
    {
        return Rectf(glm::vec2(0.0f, 0.0f), Sizef::zero());
    }
    
    /*************************************************************************
        Data Fields
    *************************************************************************/
    glm::vec2 d_min = glm::vec2(0.f, 0.f);
    glm::vec2 d_max = glm::vec2(0.f, 0.f);
};

}


#endif    // end of guard _CEGUIRectf_h_

