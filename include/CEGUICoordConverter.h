/************************************************************************
    filename:   CEGUICoordConverter.h
    created:    Sun Sep 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _CEGUICoordConverter_h_
#define _CEGUICoordConverter_h_

#include "CEGUIUDim.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Utility class that helps in converting various types of co-ordinate between
    absolute screen positions and positions offset from the top-left corner of
    a given Window object.
*/
class CEGUIEXPORT CoordConverter
{
public:
    /*!
    \brief
        Convert a window co-ordinate value, specified as a UDim, to a screen
        relative pixel co-ordinate.

    \param window
        Window object to use as a base for the conversion.

    \param x
        UDim x co-ordinate value to be converted

    \return
        float value describing a pixel screen co-ordinate that is equivalent to
        window UDim co-ordinate \a x.
    */
    static float windowToScreenX(const Window& window, const UDim& x);

    /*!
    \brief
        Convert a window pixel co-ordinate value, specified as a float, to a
        screen pixel co-ordinate.

    \param window
        Window object to use as a base for the conversion.

    \param x
        float x co-ordinate value to be converted.

    \return
        float value describing a pixel screen co-ordinate that is equivalent to
        window co-ordinate \a x.
    */
    static float windowToScreenX(const Window& window, const float x);

    /*!
    \brief
        Convert a window co-ordinate value, specified as a UDim, to a screen
        relative pixel co-ordinate.

    \param window
        Window object to use as a base for the conversion.

    \param y
        UDim y co-ordinate value to be converted

    \return
        float value describing a screen co-ordinate that is equivalent to
        window UDim co-ordinate \a y.
    */
    static float windowToScreenY(const Window& window, const UDim& y);

    /*!
    \brief
        Convert a window pixel co-ordinate value, specified as a float, to a
        screen pixel co-ordinate.

    \param window
        Window object to use as a base for the conversion.

    \param y
        float y co-ordinate value to be converted.

    \return
        float value describing a screen co-ordinate that is equivalent to
        window co-ordinate \a y.
    */
    static float windowToScreenY(const Window& window, const float y);

    /*!
    \brief
        Convert a window co-ordinate point, specified as a UVector2, to a
        screen relative pixel co-ordinate point.

    \param window
        Window object to use as a base for the conversion.

    \param vec
        UVector2 object describing the point to be converted

    \return
        Vector2 object describing a screen co-ordinate position that is
        equivalent to window based UVector2 \a vec.
    */
    static Vector2 windowToScreen(const Window& window, const UVector2& vec);

    /*!
    \brief
        Convert a window pixel co-ordinate point, specified as a Vector2, to a
        screen pixel co-ordinate point.

    \param window
        Window object to use as a base for the conversion.

    \param vec
        Vector2 object describing the point to be converted.

    \return
        Vector2 object describing a screen co-ordinate position that is
        equivalent to window based Vector2 \a vec.
    */
    static Vector2 windowToScreen(const Window& window, const Vector2& vec);

    /*!
    \brief
        Convert a window area, specified as a URect, to a screen area.

    \param rect
        URect object describing the area to be converted

    \return
        Rect object describing a screen area that is equivalent to window
        area \a rect.
    */
    static Rect windowToScreen(const Window& window, const URect& rect);

    /*!
    \brief
        Convert a pixel window area, specified as a Rect, to a screen area.

    \param window
        Window object to use as a base for the conversion.

    \param rect
        Rect object describing the area to be converted.

    \return
        Rect object describing a screen area that is equivalent to window
        area \a rect.
    */
    static Rect windowToScreen(const Window& window, const Rect& rect);

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
    static float screenToWindowX(const Window& window, const UDim& x);

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
    static float screenToWindowX(const Window& window, const float x);

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
    static float screenToWindowY(const Window& window, const UDim& y);

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
    static float screenToWindowY(const Window& window, const float y);

    /*!
    \brief
        Convert a screen relative UVector2 point to a window co-ordinate point,
        specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param vec
        UVector2 object describing the point to be converted

    \return
        Vector2 object describing a window co-ordinate point that is equivalent
        to screen based UVector2 point \a vec.
    */
    static Vector2 screenToWindow(const Window& window, const UVector2& vec);

    /*!
    \brief
        Convert a screen Vector2 pixel point to a window co-ordinate point,
        specified in pixels.

    \param window
        Window object to use as a target for the conversion.

    \param vec
        Vector2 object describing the point to be converted.

    \return
        Vector2 object describing a window co-ordinate point that is equivalent
        to screen based Vector2 point \a vec.
    */
    static Vector2 screenToWindow(const Window& window, const Vector2& vec);

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
    static Rect screenToWindow(const Window& window, const URect& rect);

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
    static Rect screenToWindow(const Window& window, const Rect& rect);

private:
    /*!
    \brief
        Return the base X co-ordinate of the given Window object.

    \param window
        Window object to return base position for.

    \return
        float value indicating the base on-screen pixel location of the window
        on the x axis (i.e. The screen co-ord of the window's left edge).
    */
    static float getBaseXValue(const Window& window);

    /*!
    \brief
        Return the base Y co-ordinate of the given Window object.

    \param window
        Window object to return base position for.

    \return
        float value indicating the base on-screen pixel location of the window
        on the y axis (i.e. The screen co-ord of the window's top edge).
    */
    static float getBaseYValue(const Window& window);

    /*!
    \brief
        Return the base position of the given Window object.

    \param window
        Window object to return base position for.

    \return
        Vector2 value indicating the base on-screen pixel location of the window
        object. (i.e. The screen co-ord of the window's top-left corner).
    */
    static Vector2 getBaseValue(const Window& window);
};

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUICoordConverter_h_
