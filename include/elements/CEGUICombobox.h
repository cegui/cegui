/************************************************************************
	filename: 	CEGUICombobox.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for Combobox widget
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
#ifndef _CEGUICombobox_h_
#define _CEGUICombobox_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Base class for the Combobox widget
*/
class CEGUIBASE_API Combobox : public Window
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// event names


	/*************************************************************************
		Editbox Accessors
	*************************************************************************/
	/*!
	\brief
		return true if the Editbox has input focus.

	\return
		true if the Editbox has keyboard input focus, false if the Editbox does not have keyboard input focus.
	*/
	bool	hasInputFocus(void) const;


	/*!
	\brief
		return true if the Editbox is read-only.

	\return
		true if the Editbox is read only and can't be edited by the user, false if the Editbox is not
		read only and may be edited by the user.
	*/
	bool	isReadOnly(void) const;


	/*!
	\brief
		return true if the Editbox text is valid given the currently set validation string.

	\note
		It is possible to programmatically set 'invalid' text for the Editbox by calling setText.  This has certain
		implications since if invalid text is set, whatever the user types into the box will be rejected when the input
		is validated.

	\note
		Validation is performed by means of a regular expression.  If the text matches the regex, the text is said to have passed
		validation.  If the text does not match with the regex then the text fails validation.

	\return
		true if the current Editbox text passes validation, false if the text does not pass validation.
	*/
	bool	isTextValid(void) const;


	/*!
	\brief
		return the currently set validation string

	\note
		Validation is performed by means of a regular expression.  If the text matches the regex, the text is said to have passed
		validation.  If the text does not match with the regex then the text fails validation.

	\return
		String object containing the current validation regex data
	*/
	const String&	getValidationString(void) const;


	/*!
	\brief
		return the current position of the carat.

	\return
		Index of the insert carat relative to the start of the text.
	*/
	ulong	getCaratIndex(void) const;


	/*!
	\brief
		return the current selection start point.

	\return
		Index of the selection start point relative to the start of the text.  If no selection is defined this function returns
		the position of the carat.
	*/
	ulong	getSelectionStartIndex(void) const;


	/*!
	\brief
		return the current selection end point.

	\return
		Index of the selection end point relative to the start of the text.  If no selection is defined this function returns
		the position of the carat.
	*/
	ulong	getSelectionEndIndex(void) const;

	
	/*!
	\brief
		return the length of the current selection (in code points / characters).

	\return
		Number of code points (or characters) contained within the currently defined selection.
	*/
	ulong	getSelectionLength(void) const;


	/*!
	\brief
		return the maximum text length set for this Editbox.

	\return
		The maximum number of code points (characters) that can be entered into this Editbox.

	\note
		Depending on the validation string set, the actual length of text that can be entered may be less than the value
		returned here (it will never be more).
	*/
	ulong	getMaxTextLength(void) const;


	/*!
	\brief
		return the currently set colour to be used for rendering Editbox text in the
		normal, unselected state.

	\return
		colour value describing the ARGB colour that is currently set.
	*/
	colour	getNormalTextColour(void) const;


	/*!
	\brief
		return the currently set colour to be used for rendering the Editbox text when within the
		selected region.

	\return
		colour value describing the ARGB colour that is currently set.
	*/
	colour	getSelectedTextColour(void) const;


	/*!
	\brief
		return the currently set colour to be used for rendering the Editbox selection highlight
		when the Editbox is active.

	\return
		colour value describing the ARGB colour that is currently set.
	*/
	colour	getNormalSelectBrushColour(void) const;


	/*!
	\brief
		return the currently set colour to be used for rendering the Editbox selection highlight
		when the Editbox is inactive.

	\return
		colour value describing the ARGB colour that is currently set.
	*/
	colour	getInactiveSelectBrushColour(void) const;


	/*************************************************************************
		List Accessors
	*************************************************************************/

	/*************************************************************************
		Editbox Manipulators
	*************************************************************************/
	/*!
	\brief
		Specify whether the Editbox is read-only.

	\param setting
		true if the Editbox is read only and can't be edited by the user, false if the Editbox is not
		read only and may be edited by the user.

	\return
		Nothing.
	*/
	void	setReadOnly(bool setting);


	/*!
	\brief
		Set the text validation string.

	\note
		Validation is performed by means of a regular expression.  If the text matches the regex, the text is said to have passed
		validation.  If the text does not match with the regex then the text fails validation.

	\param validation_string
		String object containing the validation regex data to be used.

	\return
		Nothing.
	*/
	void	setValidationString(const String& validation_string);


	/*!
	\brief
		Set the current position of the carat.

	\param carat_pos
		New index for the insert carat relative to the start of the text.  If the value specified is greater than the
		number of characters in the Editbox, the carat is positioned at the end of the text.

	\return
		Nothing.
	*/
	void	setCaratIndex(ulong carat_pos);


	/*!
	\brief
		Define the current selection for the Editbox

	\param start_pos
		Index of the starting point for the selection.  If this value is greater than the number of characters in the Editbox, the
		selection start will be set to the end of the text.

	\param end_pos
		Index of the ending point for the selection.  If this value is greater than the number of characters in the Editbox, the
		selection start will be set to the end of the text.

	\return
		Nothing.
	*/
	void	setSelection(ulong start_pos, ulong end_pos);
	

	/*!
	\brief
		set the maximum text length for this Editbox.

	\param max_len
		The maximum number of code points (characters) that can be entered into this Editbox.

	\note
		Depending on the validation string set, the actual length of text that can be entered may be less than the value
		set here (it will never be more).

	\return
		Nothing.
	*/
	void	setMaxTextLength(ulong max_len);


	/*!
	\brief
		Set the colour to be used for rendering Editbox text in the normal, unselected state.

	\param col
		colour value describing the ARGB colour that is to be used.

	\return
		Nothing.
	*/
	void	setNormalTextColour(colour col);


	/*!
	\brief
		Set the colour to be used for rendering the Editbox text when within the
		selected region.

	\return
		colour value describing the ARGB colour that is currently set.
	*/
	void	setSelectedTextColour(colour col);


	/*!
	\brief
		Set the colour to be used for rendering the Editbox selection highlight
		when the Editbox is active.

	\param col
		colour value describing the ARGB colour that is to be used.

	\return
		Nothing.
	*/
	void	setNormalSelectBrushColour(colour col);


	/*!
	\brief
		Set the colour to be used for rendering the Editbox selection highlight
		when the Editbox is inactive.

	\param col
		colour value describing the ARGB colour that is to be used.

	\return
		Nothing.
	*/
	void	setInactiveSelectBrushColour(colour col);


	/*************************************************************************
		List Manipulators
	*************************************************************************/


protected:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Combobox base class
	*/
	Combobox(const String& type, const String& name);


	/*!
	\brief
		Destructor for Combobox base class
	*/
	virtual ~Combobox(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Editbox*	d_editbox;
	Listbox*	d_droplist;
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUICombobox_h_
