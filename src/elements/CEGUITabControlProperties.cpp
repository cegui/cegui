/************************************************************************
	filename: 	CEGUITabControlProperties.cpp
	created:	08/08/2004
	author:		Steve Streeting
	
	purpose:	Implements Listbox Property classes
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
#include "elements/CEGUITabControlProperties.h"
#include "elements/CEGUITabControl.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of TabControlProperties namespace section
namespace TabControlProperties
{
String	AbsoluteTabHeight::get(const void* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const TabControl*>(receiver)->getAbsoluteTabHeight());
}


void	AbsoluteTabHeight::set(void* receiver, const String& value)
{
	static_cast<TabControl*>(receiver)->setAbsoluteTabHeight(PropertyHelper::stringToFloat(value));
}

String	RelativeTabHeight::get(const void* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const TabControl*>(receiver)->getRelativeTabHeight());
}


void	RelativeTabHeight::set(void* receiver, const String& value)
{
    static_cast<TabControl*>(receiver)->setRelativeTabHeight(PropertyHelper::stringToFloat(value));
}


String	TabHeight::get(const void* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const TabControl*>(receiver)->getTabHeight());
}


void	TabHeight::set(void* receiver, const String& value)
{
    static_cast<TabControl*>(receiver)->setTabHeight(PropertyHelper::stringToFloat(value));
}



String	AbsoluteTabTextPadding::get(const void* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const TabControl*>(receiver)->getAbsoluteTabTextPadding());
}


void	AbsoluteTabTextPadding::set(void* receiver, const String& value)
{
    static_cast<TabControl*>(receiver)->setAbsoluteTabTextPadding(PropertyHelper::stringToFloat(value));
}

String	RelativeTabTextPadding::get(const void* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const TabControl*>(receiver)->getRelativeTabTextPadding());
}


void	RelativeTabTextPadding::set(void* receiver, const String& value)
{
    static_cast<TabControl*>(receiver)->setRelativeTabTextPadding(PropertyHelper::stringToFloat(value));
}


String	TabTextPadding::get(const void* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const TabControl*>(receiver)->getTabTextPadding());
}


void	TabTextPadding::set(void* receiver, const String& value)
{
    static_cast<TabControl*>(receiver)->setTabTextPadding(PropertyHelper::stringToFloat(value));
}





} // End of  TabControlProperties namespace section

} // End of  CEGUI namespace section
