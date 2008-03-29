/***********************************************************************
    filename:   CEGUITooltipProperties.cpp
    created:    3/3/2005
    author:     Paul D Turner
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
#include "elements/CEGUITooltipProperties.h"
#include "elements/CEGUITooltip.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of TooltipProperties namespace section
namespace TooltipProperties
{
    String HoverTime::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const Tooltip*>(receiver)->getHoverTime());
    }

    void HoverTime::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Tooltip*>(receiver)->setHoverTime(PropertyHelper::stringToFloat(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String DisplayTime::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const Tooltip*>(receiver)->getDisplayTime());
    }

    void DisplayTime::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Tooltip*>(receiver)->setDisplayTime(PropertyHelper::stringToFloat(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String FadeTime::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const Tooltip*>(receiver)->getFadeTime());
    }

    void FadeTime::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Tooltip*>(receiver)->setFadeTime(PropertyHelper::stringToFloat(value));
    }

} // End of  TooltipProperties namespace section
} // End of  CEGUI namespace section
