/************************************************************************
	filename: 	CEGUITabControlProperties.h
	created:	08/08/2004
	author:		Steve Streeting
	
	purpose:	Interface to TabControl property classes
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
#ifndef _CEGUITabControlProperties_h_
#define _CEGUITabControlProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of TabControlProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Listbox class
*/
namespace TabControlProperties
{
/*!
\brief
	Property to access the tab height setting of the tab control.

	\par Usage:
		- Name: TabHeight
		- Format: "{scale,offset}" (Unified Dimension)

*/
class TabHeight : public Property
{
public:
	TabHeight() : Property(
		"TabHeight",
		"Property to get/set the height of the tabs.",
		"{0.050000,0.000000}")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
Property to access the tab text padding setting of the tab control.

\par Usage:
- Name: TabTextPadding
- Format: "{scale,offset}" (Unified Dimension)

*/
class TabTextPadding : public Property
{
public:
    TabTextPadding() : Property(
		"TabTextPadding", 
		"Property to get/set the padding either side of the tab buttons.",
		"{0.000000,5.000000}")
	{}

    String	get(const PropertyReceiver* receiver) const;
    void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  TabControlProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIListboxProperties_h_
