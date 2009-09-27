/***********************************************************************
	filename: 	FalagardStaticTextProperties.h
	created:	17/9/2005
	author:		Tomas L Olsen (based on code by Paul D Turner)
	
	purpose:	Interface for properties for the FalagardStaticText class.
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
#ifndef _FalagardStaticTextProperties_h_
#define _FalagardStaticTextProperties_h_

#include "../../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of FalagardStaticTextProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the FalagardStaticText class
*/
namespace FalagardStaticTextProperties
{

/*!
\brief
	Property to access the text colours for the FalagardStaticText widget.

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
	TextColours() : Property(
		"TextColours",
		"Property to get/set the text colours for the FalagardStaticText widget.  Value is \"tl:[aarrggbb] tr:[aarrggbb] bl:[aarrggbb] br:[aarrggbb]\".",
		"tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
		- "HorzJustified"
		- "WordWrapLeftAligned"
		- "WordWrapRightAligned"
		- "WordWrapCentred"
		- "WordWrapJustified"
*/
class HorzFormatting : public Property
{
public:
	HorzFormatting() : Property(
		"HorzFormatting",
		"Property to get/set the horizontal formatting mode.  Value is one of the HorzFormatting strings.",
		"LeftAligned")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	VertFormatting() : Property(
		"VertFormatting",
		"Property to get/set the vertical formatting mode.  Value is one of the VertFormatting strings.",
		"VertCentred")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for enabling the vertical scroll bar.

	\par Usage:
		- Name: VertScrollbar
		- Format: "[text]"

		\par Where [Text] is:
		- "True" to indicate the scroll bar is enabled and will be shown when needed.
		- "False" to indicate the scroll bar is disabled and will never be shown
*/
class VertScrollbar : public Property
{
public:
	VertScrollbar() : Property(
		"VertScrollbar",
		"Property to get/set the setting for the vertical scroll bar.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for enabling the horizontal scroll bar.

	\par Usage:
		- Name: HorzScrollbar
		- Format: "[text]"

		\par Where [Text] is:
		- "True" to indicate the scroll bar is enabled and will be shown when needed.
		- "False" to indicate the scroll bar is disabled and will never be shown
*/
class HorzScrollbar : public Property
{
public:
	HorzScrollbar() : Property(
		"HorzScrollbar",
		"Property to get/set the setting for the horizontal scroll bar.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Read-only property to access the current horizontal extent of the formatted
    StaticText string.

    \par Usage:
        - Name: HorzExtent
        - Format: float value indicating the pixel extent.
*/
class HorzExtent : public Property
{
public:
    HorzExtent() : Property(
        "HorzExtent",
        "Property to get the current horizontal extent of the formatted text "
        "string.  Value is a float indicating the pixel extent.",
        "0")
    {}

    String get(const PropertyReceiver* receiver) const;
    void set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Read-only property to access the current vertical extent of the formatted
    StaticText string.

    \par Usage:
        - Name: VertExtent
        - Format: float value indicating the pixel extent.
*/
class VertExtent : public Property
{
public:
    VertExtent() : Property(
        "VertExtent",
        "Property to get the current vertical extent of the formatted text "
        "string.  Value is a float indicating the pixel extent.",
        "0")
    {}

    String get(const PropertyReceiver* receiver) const;
    void set(PropertyReceiver* receiver, const String& value);
};

} // End of  FalagardStaticTextProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _FalagardStaticTextProperties_h_
