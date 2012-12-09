/***********************************************************************
	filename: 	CEGUITabControlProperties.h
	created:	08/08/2004
	author:		Steve Streeting
	
	purpose:	Interface to TabControl property classes
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
#ifndef _CEGUITabControlProperties_h_
#define _CEGUITabControlProperties_h_

#include "../CEGUIProperty.h"


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

/*!
\brief
Property to query/set the position of the button pane in tab control.

\par Usage:
- Name: TabPanePosition
- Format: "top" | "bottom"

*/
class TabPanePosition : public Property
{
public:
    TabPanePosition() : Property(
		"TabPanePosition", 
		"Property to get/set the position of the buttons pane.",
		"top")
    {}

    String	get(const PropertyReceiver* receiver) const;
    void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  TabControlProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIListboxProperties_h_
