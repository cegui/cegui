/***********************************************************************
	filename: 	CEGUIScrollablePaneProperties.cpp
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
#include "elements/CEGUIScrollablePaneProperties.h"
#include "elements/CEGUIScrollablePane.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ScrollablePaneProperties namespace section
namespace ScrollablePaneProperties
{
    String ContentPaneAutoSized::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(static_cast<const ScrollablePane*>(receiver)->isContentPaneAutoSized());
    }

    void ContentPaneAutoSized::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setContentPaneAutoSized(PropertyHelper<bool>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String ContentArea::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<Rectf >::toString(static_cast<const ScrollablePane*>(receiver)->getContentPaneArea());
    }

    void ContentArea::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setContentPaneArea(PropertyHelper<Rectf >::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String ForceVertScrollbar::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(static_cast<const ScrollablePane*>(receiver)->isVertScrollbarAlwaysShown());
    }

    void ForceVertScrollbar::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setShowVertScrollbar(PropertyHelper<bool>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String ForceHorzScrollbar::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(static_cast<const ScrollablePane*>(receiver)->isHorzScrollbarAlwaysShown());
    }

    void ForceHorzScrollbar::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setShowHorzScrollbar(PropertyHelper<bool>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////
    
    String HorzStepSize::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const ScrollablePane*>(receiver)->getHorizontalStepSize());
    }

    void HorzStepSize::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setHorizontalStepSize(PropertyHelper<float>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String HorzOverlapSize::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const ScrollablePane*>(receiver)->getHorizontalOverlapSize());
    }

    void HorzOverlapSize::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setHorizontalOverlapSize(PropertyHelper<float>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String HorzScrollPosition::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const ScrollablePane*>(receiver)->getHorizontalScrollPosition());
    }

    void HorzScrollPosition::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setHorizontalScrollPosition(PropertyHelper<float>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String VertStepSize::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const ScrollablePane*>(receiver)->getHorizontalStepSize());
    }

    void VertStepSize::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setVerticalStepSize(PropertyHelper<float>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String VertOverlapSize::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const ScrollablePane*>(receiver)->getHorizontalOverlapSize());
    }

    void VertOverlapSize::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setVerticalOverlapSize(PropertyHelper<float>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String VertScrollPosition::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const ScrollablePane*>(receiver)->getHorizontalScrollPosition());
    }

    void VertScrollPosition::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrollablePane*>(receiver)->setVerticalScrollPosition(PropertyHelper<float>::fromString(value));
    }

} // End of  ScrollablePaneProperties namespace section
} // End of  CEGUI namespace section
