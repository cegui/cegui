/***********************************************************************
	filename: 	CEGUITooltipProperties.h
	created:	3/3/2005
	author:		Paul D Turner
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
#ifndef _CEGUITooltipProperties_h_
#define _CEGUITooltipProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of TooltipProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Tooltip class
*/
namespace TooltipProperties
{
    /*!
    \brief
        Property to access the timout that must expire before the tooltip gets activated.

    \par Usage:
        - Name: HoverTime
        - Format: "[float]".

    \par Where:
        - [float] specifies the number of seconds the mouse must hover stationary on a widget before the tooltip gets activated.
    */
    class HoverTime : public Property
    {
    public:
        HoverTime() : Property(
            "HoverTime",
            "Property to get/set the hover timeout value in seconds.  Value is a float.",
            "0.400000")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the time after which the tooltip automatically de-activates itself.

    \par Usage:
        - Name: DisplayTime
        - Format: "[float]".

    \par Where:
        - [float] specifies the number of seconds after which the tooltip will deactivate itself if the mouse has remained stationary.
    */
    class DisplayTime : public Property
    {
    public:
        DisplayTime() : Property(
            "DisplayTime",
            "Property to get/set the display timeout value in seconds.  Value is a float.",
            "7.500000")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the duration of the fade effect for the tooltip.

    \par Usage:
        - Name: FadeTime
        - Format: "[float]".

    \par Where:
        - [float] specifies the number of seconds over which the fade in / fade out effect will happen.
    */
    class FadeTime : public Property
    {
    public:
        FadeTime() : Property(
            "FadeTime",
            "Property to get/set duration of the fade effect in seconds.  Value is a float.",
            "0.330000")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
    };

} // End of  TooltipProperties namespace section
} // End of  CEGUI namespace section


#endif	// end of guard _CEGUITooltipProperties_h_
