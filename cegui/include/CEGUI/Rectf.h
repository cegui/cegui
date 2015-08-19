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

#include "CEGUI/Vector.h"
#include "CEGUI/Sizef.h"

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
    inline Rectf()
    {}

    inline Rectf(const float& left, const float& top, const float& right, const float& bottom):
        d_min(left, top),
        d_max(right, bottom)
    {}

    inline Rectf(const Vector2<float>& min, const Vector2<float>& max):
        d_min(min),
        d_max(max)
    {}

    // FIXME: Temporary for glm::vec2 transition
    inline Rectf(const glm::vec2& min, const glm::vec2& max):
        d_min(min.x, min.y),
        d_max(max.x, max.y)
    {}

    inline Rectf(const Vector2<float>& pos, const Sizef& size):
        d_min(pos),
        d_max(pos + Vector2<float>(size.d_width, size.d_height))
    {}

    // FIXME: Temporary for glm::vec2 transition
    inline Rectf(const glm::vec2& pos, const Sizef& size):
        d_min(pos.x, pos.y),
        d_max(pos.x + size.d_width, pos.y + size.d_height)
    {}

    inline Rectf(const Rectf& r):
        d_min(r.d_min),
        d_max(r.d_max)
    {}

    inline Rectf& operator=(const Rectf& rhs)
    {
        d_min = rhs.d_min;
        d_max = rhs.d_max;

        return *this;
    }

    inline void left(const float& v)
    {
        d_min.d_x = v;
    }

    inline const float& left() const
    {
        return d_min.d_x;
    }

    inline void top(const float& v)
    {
        d_min.d_y = v;
    }

    inline const float& top() const
    {
        return d_min.d_y;
    }

    inline void right(const float& v)
    {
        d_max.d_x = v;
    }

    inline const float& right() const
    {
        return d_max.d_x;
    }

    inline void bottom(const float& v)
    {
        d_max.d_y = v;
    }

    inline const float& bottom() const
    {
        return d_max.d_y;
    }

    /*!
    \    
        set the position of the Rectf (leaves size in tact)
    */
    void setPosition(const Vector2<float>& min)
    {
        const Sizef size = getSize();
        d_min = min;
        setSize(size);
    }

    // FIXME: Temporary for glm::vec2 transition
    void setPosition(const glm::vec2& min)
    {
        const Sizef size = getSize();
        d_min = Vector2<float>(min.x, min.y);
        setSize(size);
    }

    /*!
    \    
        Return top-left position of Rectf as a Vector2<float>
    */
    const Vector2<float>& getPosition() const
    {
        return d_min;
    }

    // FIXME: Temporary for glm::vec2 transition
    glm::vec2 getPositionGLM() const
    {
        return glm::vec2(d_min.d_x, d_min.d_y);
    }

    void setSize(const Sizef& size)
    {
        d_max = d_min + Vector2<float>(size.d_width, size.d_height);
    }

    /*!
    \    
        return the size of the Rectf area
    */
    inline Sizef getSize() const
    {
        return Sizef(getWidth(), getHeight());
    }

    void setWidth(const float& w)
    {
        d_max.d_x = d_min.d_x + w;
    }

    /*!
    \    
        return width of Rectf area
    */
    inline float getWidth() const
    {
        return d_max.d_x - d_min.d_x;
    }

    void setHeight(const float& h)
    {
        d_max.d_y = d_min.d_y + h;
    }

    /*!
    \    
        return height of Rectf area
    */
    inline float getHeight() const
    {
        return d_max.d_y - d_min.d_y;
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
        if ((d_max.d_x > rect.d_min.d_x) &&
            (d_min.d_x < rect.d_max.d_x) &&
            (d_max.d_y > rect.d_min.d_y) &&
            (d_min.d_y < rect.d_max.d_y))
        {
            Rectf ret;

            // fill in ret with the intersection
            ret.d_min.d_x = (d_min.d_x > rect.d_min.d_x) ? d_min.d_x : rect.d_min.d_x;
            ret.d_max.d_x = (d_max.d_x < rect.d_max.d_x) ? d_max.d_x : rect.d_max.d_x;
            ret.d_min.d_y = (d_min.d_y > rect.d_min.d_y) ? d_min.d_y : rect.d_min.d_y;
            ret.d_max.d_y = (d_max.d_y < rect.d_max.d_y) ? d_max.d_y : rect.d_max.d_y;

            return ret;
        }
        else
        {
            return Rectf(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    /*!
    \    
        Applies an offset the Rectf object

    \param pt
        Vector2 object containing the offsets to be applied to the Rectf.

    \return
        this Rectf after the offset is performed
    */
    inline void offset(const Vector2<float>& v)
    {
        d_min += v;
        d_max += v;
    }

    // FIXME: Temporary for glm::vec2 transition
    inline void offset(const glm::vec2& v)
    {
        d_min += Vector2<float>(v.x, v.y);
        d_max += Vector2<float>(v.x, v.y);
    }

    /*!
    \    
        Return true if the given Vector2 falls within this Rectf

    \param pt
        Vector2 object describing the position to test.

    \return
        true if position \a pt is within this Rectf's area, else false
    */
    inline bool isPointInRectf(const Vector2<float>& v) const
    {
        if ((d_min.d_x >  v.d_x) ||
            (d_max.d_x <= v.d_x) ||
            (d_min.d_y >  v.d_y) ||
            (d_max.d_y <= v.d_y))
        {
            return false;
        }

        return true;
    }

    // FIXME: Temporary for glm::vec2 transition
    inline bool isPointInRectf(const glm::vec2& v) const
    {
        if ((d_min.d_x >  v.x) ||
            (d_max.d_x <= v.x) ||
            (d_min.d_y >  v.y) ||
            (d_max.d_y <= v.y))
        {
            return false;
        }

        return true;
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

    inline Rectf operator*(Vector2<float> vector) const
    {
        return Rectf(d_min * vector, d_max * vector);
    }

    // FIXME: Temporary for glm::vec2 transition
    inline Rectf operator*(glm::vec2 vector) const
    {
        return Rectf(d_min * Vector2<float>(vector.x, vector.y), d_max * Vector2<float>(vector.x, vector.y));
    }

    const Rectf& operator*=(float scalar)
    {
        d_min *= scalar;
        d_max *= scalar;
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

    inline friend std::ostream& operator << (std::ostream& s, const Rectf& val)
    {
        s << "{" << val.d_min << "," << val.d_min << "}";
        return s;
    }
    
    //! \     finger saving alias for zero sized, zero positioned rect
    inline static Rectf zero()
    {
        return Rectf(Vector2<float>::zero(), Sizef::zero());
    }
    
    /*************************************************************************
        Data Fields
    *************************************************************************/
    Vector2<float> d_min;
    Vector2<float> d_max;

    // d_min.d_x is former d_left
    // d_min.d_y is former d_top
    // d_max.d_x is former d_right
    // d_max.d_y is former d_bottom
};

}


#endif    // end of guard _CEGUIRectf_h_

