/***********************************************************************
	filename: 	CEGUIRect.h
	created:	14/2/2011
	author:		Martin Preisler (reworked from code by Paul D Turner)
	
	purpose:	Defines 'Rect' class
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
#ifndef _CEGUIRect_h_
#define _CEGUIRect_h_

#include "CEGUI/UVector2.h"
#include "CEGUI/Size.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Class encapsulating operations on a Rectangle
*/
class Rectf:
    public AllocatedObject<Rectf>
{
public:
    typedef float value_type;

    inline Rectf()
    {}

    inline Rectf(const float left, const float top, const float right, const float bottom):
        d_min(left, top),
        d_max(right, bottom)
    {}

    inline Rectf(const glm::vec2& min, const glm::vec2& max):
        d_min(min),
        d_max(max)
    {}

    inline Rectf(const glm::vec2& pos, const Sizef& size):
        d_min(pos),
        d_max(pos + glm::vec2(size.d_width, size.d_height))
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
	\brief
		set the position of the Rectf (leaves size in tact)
	*/
	void setPosition(const glm::vec2& min)
    {
        const Sizef size = getSize();
        d_min = min;
        setSize(size);
    }

    /*!
	\brief
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
	\brief
		return the size of the Rectf area
	*/
	inline Sizef getSize() const
    {
        return Sizef(getWidth(), getHeight());
    }

    void setWidth(const float w)
    {
        d_max.x = d_min.x + w;
    }

	/*!
	\brief
		return width of Rectf area
	*/
	inline float getWidth() const
    {
        return d_max.x - d_min.x;
    }

    void setHeight(const float h)
    {
        d_max.y = d_min.y + h;
    }

	/*!
	\brief
		return height of Rectf area
	*/
	inline float getHeight() const
    {
        return d_max.y - d_min.y;
    }

	/*!
	\brief
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

	/*!
	\brief
		Return true if the given Vector2 falls within this Rectf

	\param pt
		Vector2 object describing the position to test.

	\return
		true if position \a pt is within this Rectf's area, else false
	*/
	inline bool isPointInRect(const glm::vec2& v) const
    {
		if ((d_min.x >  v.x) ||
		    (d_max.x <= v.x) ||
		    (d_min.y >  v.y) ||
		    (d_max.y <= v.y))
	    {
		    return false;
	    }

	    return true;
    }

	/*!
	\brief
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
	\brief
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
	\brief
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
    
    inline friend std::ostream& operator << (std::ostream& s, const Rectf& v)
    {
        s << "CEGUI::Rectf(" << v.d_min.x << ", " << v.d_min.y << ", " << v.d_max.x << ", " << v.d_max.y << ")";
        return s;
    }
    
    //! \brief finger saving alias for zero sized, zero positioned rect
    inline static Rectf zero()
    {
        return Rectf(glm::vec2(0, 0), Sizef::zero());
    }
    
    /*************************************************************************
        Data Fields
    *************************************************************************/
    glm::vec2 d_min;
    glm::vec2 d_max;

    // d_min.d_x is former d_left
    // d_min.d_y is former d_top
    // d_max.d_x is former d_right
    // d_max.d_y is former d_bottom
};

/*!
\brief
	Class encapsulating operations on a unified dim rectangle
*/
class URect:
    public AllocatedObject<URect>
{
public:
    typedef UDim value_type;

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
		Return top-left position of URect as a Vector2<T>
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
    
    inline friend std::ostream& operator << (std::ostream& s, const URect& v)
    {
        s << "CEGUI::URect(" << v.d_min.d_x << ", " << v.d_min.d_y << ", " << v.d_max.d_x << ", " << v.d_max.d_y << ")";
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

// we need to allow URect to be multiplied by floats, this is the most elegant way to do that
inline URect operator * (const URect& v, const float c)
{
    return URect(v.d_min * c, v.d_max * c);
}

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIRect_h_

