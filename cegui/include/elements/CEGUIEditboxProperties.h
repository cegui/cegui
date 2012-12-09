/***********************************************************************
	filename: 	CEGUIEditboxProperties.h
	created:	9/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to Properties for Editbox class
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
#ifndef _CEGUIEditboxProperties_h_
#define _CEGUIEditboxProperties_h_

#include "../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of EditboxProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Editbox class
*/
namespace EditboxProperties
{
/*!
\brief
	Property to access the read-only setting of the edit box.

	This property offers access to the read-only setting for the Editbox object.

	\par Usage:
		- Name: ReadOnly
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the edit box is read-only.
		- "False" to indicate the edit box is not read-only (text may be edited by user).
*/
class ReadOnly : public Property
{
public:
	ReadOnly() : Property(
		"ReadOnly",
		"Property to get/set the read-only setting for the Editbox.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the mask text setting of the edit box.

	This property offers access to the mask text setting for the Editbox object.

	\par Usage:
		- Name: MaskText
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the text should be masked.
		- "False" to indicate the text should not be masked.
*/
class MaskText : public Property
{
public:
	MaskText() : Property(
		"MaskText",
		"Property to get/set the mask text setting for the Editbox.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the mask text setting of the edit box.

	This property offers access to the mask text setting for the Editbox object.

	\par Usage:
		- Name: MaskCodepoint
		- Format: "[uint]"

	\par Where:
		- [uint] is the Unicode utf32 value of the codepoint used for masking text.
*/
class MaskCodepoint : public Property
{
public:
	MaskCodepoint() : Property(
		"MaskCodepoint",
		"Property to get/set the utf32 codepoint value used for masking text.  Value is \"[uint]\".",
		"42")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the string used for regular expression validation of the edit box text.

	\par Usage:
		- Name: ValidationString
		- Format: "[text]"

	\par Where:
		- [Text] is the string used for validating text entry.
*/
class ValidationString : public Property
{
public:
	ValidationString() : Property(
		"ValidationString",
		"Property to get/set the validation string Editbox.  Value is a text string.",
		".*")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the current carat index.

	\par Usage:
		- Name: CaratIndex
		- Format: "[uint]"

	\par Where:
		- [uint] is the zero based index of the carat position within the text.
*/
class CaratIndex : public Property
{
public:
	CaratIndex() : Property(
		"CaratIndex",
		"Property to get/set the current carat index.  Value is \"[uint]\".",
		"0")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the current selection start index.

	\par Usage:
		- Name: SelectionStart
		- Format: "[uint]"

	\par Where:
		- [uint] is the zero based index of the selection start position within the text.
*/
class SelectionStart : public Property
{
public:
	SelectionStart() : Property(
		"SelectionStart",
		"Property to get/set the zero based index of the selection start position within the text.  Value is \"[uint]\".",
		"0")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the current selection length.

	\par Usage:
		- Name: SelectionLength
		- Format: "[uint]"

	\par Where:
		- [uint] is the length of the selection (as a count of the number of code points selected).
*/
class SelectionLength : public Property
{
public:
	SelectionLength() : Property(
		"SelectionLength",
		"Property to get/set the length of the selection (as a count of the number of code points selected).  Value is \"[uint]\".",
		"0")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the maximum text length for the edit box.

	\par Usage:
		- Name: MaxTextLength
		- Format: "[uint]"

	\par Where:
		- [uint] is the maximum allowed text length (as a count of code points).
*/
class MaxTextLength : public Property
{
public:
	MaxTextLength() : Property(
		"MaxTextLength",
		"Property to get/set the the maximum allowed text length (as a count of code points).  Value is \"[uint]\".",
		"1073741824")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the normal, unselected, text colour used for rendering text.

	\par Usage:
		- Name: NormalTextColour
		- Format: "aarrggbb".

	\par Where:
		- aarrggbb is the ARGB colour value to be used.
*/
class NormalTextColour : public Property
{
public:
	NormalTextColour() : Property(
		"NormalTextColour",
		"Property to get/set the normal, unselected, text colour used for rendering text.  Value is \"aarrggbb\" (hex).",
		"FFFFFFFF")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the colour used for rendering text within the selection area.

	\par Usage:
		- Name: SelectedTextColour
		- Format: "aarrggbb".

	\par Where:
		- aarrggbb is the ARGB colour value to be used.
*/
class SelectedTextColour : public Property
{
public:
	SelectedTextColour() : Property(
		"SelectedTextColour",
		"Property to get/set the colour used for rendering text within the selection area.  Value is \"aarrggbb\" (hex).",
		"FF000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the colour used for rendering the selection highlight when the edit box is active.

	\par Usage:
		- Name: ActiveSelectionColour
		- Format: "aarrggbb".

	\par Where:
		- aarrggbb is the ARGB colour value to be used.
*/
class ActiveSelectionColour : public Property
{
public:
	ActiveSelectionColour() : Property(
		"ActiveSelectionColour",
		"Property to get/set the colour used for rendering the selection highlight when the edit box is active.  Value is \"aarrggbb\" (hex).",
		"FF6060FF")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the colour used for rendering the selection highlight when the edit box is inactive.

	\par Usage:
		- Name: InactiveSelectionColour
		- Format: "aarrggbb".

	\par Where:
		- aarrggbb is the ARGB colour value to be used.
*/
class InactiveSelectionColour : public Property
{
public:
	InactiveSelectionColour() : Property(
		"InactiveSelectionColour",
		"Property to get/set the colour used for rendering the selection highlight when the edit box is inactive.  Value is \"aarrggbb\" (hex).",
		"FF808080")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  EditboxProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIEditboxProperties_h_
