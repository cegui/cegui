/************************************************************************
	filename: 	CEGUIListHeaderSegmentProperties.h
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for ListHeaderSegment class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#ifndef _CEGUIListHeaderSegmentProperties_h_
#define _CEGUIListHeaderSegmentProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ListHeaderSegmentProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the ListHeaderSegment class
*/
namespace ListHeaderSegmentProperties
{
/*!
\brief
	Property to access the sizable setting of the header segment.

	\par Usage:
		- Name: Sizable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the segment can be sized by the user.
		- "False" to indicate the segment can not be sized by the user.
*/
class Sizable : public Property
{
public:
	Sizable() : Property((utf8*)"Sizable", (utf8*)"Property to get/set the sizable setting of the header segment.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the click-able setting of the header segment.

	\par Usage:
		- Name: Clickable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the segment can be clicked by the user.
		- "False" to indicate the segment can not be clicked by the user.
*/
class Clickable : public Property
{
public:
	Clickable() : Property((utf8*)"Clickable", (utf8*)"Property to get/set the click-able setting of the header segment.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the drag-able setting of the header segment.

	\par Usage:
		- Name: Dragable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the segment can be dragged by the user.
		- "False" to indicate the segment can not be dragged by the user.
*/
class Dragable : public Property
{
public:
	Dragable() : Property((utf8*)"Dragable", (utf8*)"Property to get/set the drag-able setting of the header segment.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the sort direction setting of the header segment.

	\par Usage:
		- Name: SortDirection
		- Format: "[text]"

	\par Where [Text] is one of:
		- "Ascending"
		- "Descending"
		- "None"
*/
class SortDirection : public Property
{
public:
	SortDirection() : Property((utf8*)"SortDirection", (utf8*)"Property to get/set the sort direction setting of the header segment.  Value is the text of one of the SortDirection enumerated value names.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};

} // End of  ListHeaderSegmentProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIListHeaderSegmentProperties_h_
