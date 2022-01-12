/***********************************************************************
    created:    Sun Sep 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUICoordConverter_h_
#define _CEGUICoordConverter_h_

#include "CEGUI/Rectf.h"
#include "CEGUI/URect.h"

namespace CEGUI
{
class Element;

/*!
\brief
    Utility class that helps in converting various types of co-ordinate between
    absolute screen positions and positions offset from the top-left corner of
    a given Window object.
*/
class CEGUIEXPORT CoordConverter
{
public:

    // Must not be constructed
    CoordConverter() = delete;

    /*!
    \brief
        Static method used to return a float value rounded to the nearest integer.

        This method is used throughout the library to ensure that elements are
        kept at integer pixel positions on the display if user wishes so.

    \param x
        Expression to be rounded to nearest whole number

    \return
        \a x after having been rounded
        
    \see Node::setPixelAligned
    */
    static float alignToPixels(float x) { return std::round(x); }
    
    /*!
    \brief
        converts given UDim to absolute value
    */
    static float asAbsolute(const UDim& u, float base, bool pixelAlign = true)
    {
        return pixelAlign ? alignToPixels(base * u.d_scale + u.d_offset) : base * u.d_scale + u.d_offset;
    }

    /*!
    \brief
        converts given UDim to relative value
    */
    static float asRelative(const UDim& u, float base)
    {
        return (base != 0.0f) ? u.d_offset / base + u.d_scale : 0.0f;
    }

    /*!
    \brief
        converts given UVector2 to absolute glm::vec2
    */
    static glm::vec2 asAbsolute(const UVector2& v, const Sizef& base, bool pixelAlign = true)
    {
        return glm::vec2(asAbsolute(v.d_x, base.d_width, pixelAlign), asAbsolute(v.d_y, base.d_height, pixelAlign));
    }

    /*!
    \brief
        converts given UVector2 to relative glm::vec2
    */
    static glm::vec2 asRelative(const UVector2& v, const Sizef& base)
    {
        return glm::vec2(asRelative(v.d_x, base.d_width), asRelative(v.d_y, base.d_height));
    }

	/*!
    \brief
        converts given USize to absolute Sizef
    */
    static Sizef asAbsolute(const USize& v, const Sizef& base, bool pixelAlign = true)
    {
        return Sizef(asAbsolute(v.d_width, base.d_width, pixelAlign), asAbsolute(v.d_height, base.d_height, pixelAlign));
    }

    /*!
    \brief
        converts given USize to relative Sizef
    */
    static Sizef asRelative(const USize& v, const Sizef& base)
    {
        return Sizef(asRelative(v.d_width, base.d_width), asRelative(v.d_height, base.d_height));
    }

    static Rectf asAbsolute(const URect& r, const Sizef& base, bool pixelAlign = true)
    {
        return Rectf(
                   asAbsolute(r.d_min.d_x, base.d_width,  pixelAlign),
                   asAbsolute(r.d_min.d_y, base.d_height, pixelAlign),
                   asAbsolute(r.d_max.d_x, base.d_width,  pixelAlign),
                   asAbsolute(r.d_max.d_y, base.d_height, pixelAlign)
               );
    }

    static Rectf asRelative(const URect& r, const Sizef& base)
    {
        return Rectf(
                   asRelative(r.d_min.d_x, base.d_width),
                   asRelative(r.d_min.d_y, base.d_height),
                   asRelative(r.d_max.d_x, base.d_width),
                   asRelative(r.d_max.d_y, base.d_height)
               );
    }

    /*!
    \brief
        Convert a screen relative UDim co-ordinate value to a window co-ordinate
        value, specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param x
        UDim x co-ordinate value to be converted

    \return
        float value describing a window co-ordinate value that is equivalent to
        screen UDim co-ordinate \a x.
    */
    static float screenToWindowX(const Element& element, const UDim& x);

    /*!
    \brief
        Convert a screen pixel co-ordinate value to a window co-ordinate
        value, specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param x
        float x co-ordinate value to be converted.

    \return
        float value describing a window co-ordinate value that is equivalent to
        screen co-ordinate \a x.
    */
    static float screenToWindowX(const Element& element, const float x);

    /*!
    \brief
        Convert a screen relative UDim co-ordinate value to a window co-ordinate
        value, specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param y
        UDim y co-ordinate value to be converted

    \return
        float value describing a window co-ordinate value that is equivalent to
        screen UDim co-ordinate \a y.
    */
    static float screenToWindowY(const Element& element, const UDim& y);

    /*!
    \brief
        Convert a screen pixel co-ordinate value to a window co-ordinate
        value, specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param y
        UDim y co-ordinate value to be converted.

    \return
        float value describing a window co-ordinate value that is equivalent to
        screen co-ordinate \a y.
    */
    static float screenToWindowY(const Element& element, const float y);

    /*!
    \brief
        Convert a screen relative UVector2 point to a window co-ordinate point,
        specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param vec
        UVector2 object describing the point to be converted

    \return
        glm::vec2 object describing a window co-ordinate point that is equivalent
        to screen based UVector2 point \a vec.
    */
    static glm::vec2 screenToWindow(const Element& element, const UVector2& vec);

    /*!
    \brief
        Convert a screen Vector2 pixel point to a window co-ordinate point,
        specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param vec
        Vector2 object describing the point to be converted.

    \return
        glm::vec2 object describing a window co-ordinate point that is equivalent
        to screen based Vector2 point \a vec.
    */
    static glm::vec2 screenToWindow(const Element& element, const glm::vec2& vec);

    /*!
    \brief
        Convert a URect screen area to a window area, specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param rect
        URect object describing the area to be converted

    \return
        Rect object describing a window area that is equivalent to URect screen
        area \a rect.
    */
    static Rectf screenToWindow(const Element& element, const URect& rect);

    /*!
    \brief
        Convert a Rect screen pixel area to a window area, specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param rect
        Rect object describing the area to be converted.

    \return
        Rect object describing a window area that is equivalent to Rect screen
        area \a rect.
    */
    static Rectf screenToWindow(const Element& element, const Rectf& rect);
};

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUICoordConverter_h_
