/***********************************************************************
	filename: 	CEGUIComboboxProperties.h
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to Combobox property classes
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
#ifndef _CEGUIComboboxProperties_h_
#define _CEGUIComboboxProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ComboboxProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Combobox class
*/
namespace ComboboxProperties
{
/*!
\brief
	Property to access the read-only setting of the edit box.

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
		- Name: EditSelectionStart
		- Format: "[uint]"

	\par Where:
		- [uint] is the zero based index of the selection start position within the text.
*/
class EditSelectionStart : public Property
{
public:
	EditSelectionStart() : Property(
		"EditSelectionStart",
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
		- Name: EditSelectionLength
		- Format: "[uint]"

	\par Where:
		- [uint] is the length of the selection (as a count of the number of code points selected).
*/
class EditSelectionLength : public Property
{
public:
	EditSelectionLength() : Property(
		"EditSelectionLength",
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
		- Name: MaxEditTextLength
		- Format: "[uint]"

	\par Where:
		- [uint] is the maximum allowed text length (as a count of code points).
*/
class MaxEditTextLength : public Property
{
public:
	MaxEditTextLength() : Property(
		"MaxEditTextLength",
		"Property to get/set the the maximum allowed text length (as a count of code points).  Value is \"[uint]\".",
		"1073741824")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the sort setting of the list box.

	\par Usage:
		- Name: SortList
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the list items should be sorted.
		- "False" to indicate the list items should not be sorted.
*/
class SortList : public Property
{
public:
	SortList() : Property(
		"SortList",
		"Property to get/set the sort setting of the list box.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the 'always show' setting for the vertical scroll bar of the list box.

	\par Usage:
		- Name: ForceVertScrollbar
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that the vertical scroll bar will always be shown.
		- "False" to indicate that the vertical scroll bar will only be shown when it is needed.
*/
class ForceVertScrollbar : public Property
{
public:
	ForceVertScrollbar() : Property(
		"ForceVertScrollbar",
		"Property to get/set the 'always show' setting for the vertical scroll bar of the list box.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the 'always show' setting for the horizontal scroll bar of the list box.

	\par Usage:
		- Name: ForceHorzScrollbar
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that the horizontal scroll bar will always be shown.
		- "False" to indicate that the horizontal scroll bar will only be shown when it is needed.
*/
class ForceHorzScrollbar : public Property
{
public:
	ForceHorzScrollbar() : Property(
		"ForceHorzScrollbar",
		"Property to get/set the 'always show' setting for the horizontal scroll bar of the list box.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the 'single click mode' setting for the combo box.

	\par Usage:
		- Name: SingleClickMode
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate that the box will operate in single click mode
		- "False" to indicate that the box will not operate in single click mode
*/
class SingleClickMode : public Property
{
public:
	SingleClickMode() : Property(
		"SingleClickMode",
		"Property to get/set the 'single click mode' setting for the combo box.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


} // End of  ComboboxProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIComboboxProperties_h_
