/************************************************************************
	filename: 	CEGUIStaticTextProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface for properties for the StaticText class.
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
#ifndef _CEGUIStaticTextProperties_h_
#define _CEGUIStaticTextProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of StaticTextProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the StaticText class
*/
namespace StaticTextProperties
{
/*!
\brief
	Property to access the text colours for the StaticText widget.

	\par Usage:
		- Name: TextColours
		- Format: "tl:[aarrggbb] tr:[aarrggbb] bl:[aarrggbb] br:[aarrggbb]".

	\par Where:
		- tl:[aarrggbb] is the top-left colour value specified as ARGB (hex).
		- tr:[aarrggbb] is the top-right colour value specified as ARGB (hex).
		- bl:[aarrggbb] is the bottom-left colour value specified as ARGB (hex).
		- br:[aarrggbb] is the bottom-right colour value specified as ARGB (hex).
*/
class TextColours : public Property
{
public:
	TextColours() : Property((utf8*)"TextColours", (utf8*)"Property to get/set the text colours for the StaticText widget.  Value is \"tl:[aarrggbb] tr:[aarrggbb] bl:[aarrggbb] br:[aarrggbb]\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the horizontal formatting mode setting.

	\par Usage:
		- Name: HorzFormatting
		- Format: "[text]".

	\par Where [text] is one of:
		- "LeftAligned"
		- "RightAligned"
		- "HorzCentred"
		- "WordWrapLeftAligned"
		- "WordWrapRightAligned"
		- "WordWrapCentred"
*/
class HorzFormatting : public Property
{
public:
	HorzFormatting() : Property((utf8*)"HorzFormatting", (utf8*)"Property to get/set the horizontal formatting mode.  Value is one of the HorzFormatting strings.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the vertical formatting mode setting.

	\par Usage:
		- Name: VertFormatting
		- Format: "[text]".

	\par Where [text] is one of:
		- "TopAligned"
		- "BottomAligned"
		- "VertCentred"
*/
class VertFormatting : public Property
{
public:
	VertFormatting() : Property((utf8*)"VertFormatting", (utf8*)"Property to get/set the vertical formatting mode.  Value is one of the VertFormatting strings.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};

} // End of  StaticTextProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIStaticTextProperties_h_
