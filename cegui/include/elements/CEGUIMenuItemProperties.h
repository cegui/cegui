/************************************************************************
    filename:   CEGUIMenuItemProperties.h
    created:    Mon Oct 18 2010
    author:     Robert Stoll
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIMenuItemProperties_h_
#define _CEGUIMenuItemProperties_h_

#include "../CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of MenuItemProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface for the MenuItem class
*/
namespace MenuItemProperties
{
/*!
\brief
    Property to specify an offset for the popup menu position.

    \par Usage:
        - Name: PopupOffset
        - Format: "{{[xs],[xo]},{[ys],[yo]}}"

    \par Where:
        - [xs] is a floating point value describing the relative scale
          value for the offset x-coordinate.
        - [xo] is a floating point value describing the absolute offset
          value for the offset x-coordinate.
        - [ys] is a floating point value describing the relative scale
          value for the offset y-coordinate.
        - [yo] is a floating point value describing the absolute offset
          value for the offset y-coordinate.
*/
class PopupOffset : public Property
{
public:
    PopupOffset() : Property(
            "PopupOffset",
            "Property to specify an offset for the popup menu position. Value is a UVector2 property value.",
            "{{0,0},{0,0}}")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
Property to specify the time, which has to elapse before the popup window is opened/closed if the hovering state changes

\par Usage:
- Name: AutoPopupTimeout
- Format: "[Float]"

\par Where [Float] is:
- wait time in seconds
- set to 0.0f if you want no automatic popups
*/
class AutoPopupTimeout : public Property
{
public:
    AutoPopupTimeout() : Property(
            "AutoPopupTimeout",
            "Property to specify the time, which has to elapse before the popup window is opened/closed if the hovering state changes. Value is a float property value.",
            "0.0f")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};
} // End of  MenuItemProperties namespace section
} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIMenuItemProperties_h_
