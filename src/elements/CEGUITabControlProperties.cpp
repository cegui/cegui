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


} // End of  TabControlProperties namespace section

} // End of  CEGUI namespace section
