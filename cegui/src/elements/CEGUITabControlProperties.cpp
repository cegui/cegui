/***********************************************************************
	filename: 	CEGUITabControlProperties.cpp
	created:	08/08/2004
	author:		Steve Streeting
	
	purpose:	Implements Listbox Property classes
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
#include "elements/CEGUITabControlProperties.h"
#include "elements/CEGUITabControl.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of TabControlProperties namespace section
namespace TabControlProperties
{

String	TabHeight::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::udimToString(static_cast<const TabControl*>(receiver)->getTabHeight());
}


void	TabHeight::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<TabControl*>(receiver)->setTabHeight(PropertyHelper::stringToUDim(value));
}


String	TabTextPadding::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::udimToString(static_cast<const TabControl*>(receiver)->getTabTextPadding());
}


void	TabTextPadding::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<TabControl*>(receiver)->setTabTextPadding(PropertyHelper::stringToUDim(value));
}


String	TabPanePosition::get(const PropertyReceiver* receiver) const
{
    TabControl::TabPanePosition tpp =
        static_cast<const TabControl*>(receiver)->getTabPanePosition ();
    if (tpp == TabControl::Top)
        return String ("Top");
    return String ("Bottom");
}


void	TabPanePosition::set(PropertyReceiver* receiver, const String& value)
{
    TabControl::TabPanePosition tpp;
    if ((value == "top") || (value == "Top"))
        tpp = TabControl::Top;
    else if ((value == "bottom") || (value == "Bottom"))
        tpp = TabControl::Bottom;
    else
        return;
    static_cast<TabControl*>(receiver)->setTabPanePosition (tpp);
}


} // End of  TabControlProperties namespace section

} // End of  CEGUI namespace section
