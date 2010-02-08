/***********************************************************************
	filename: 	CEGUIRadioButton.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for RadioButton widget
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
#ifndef _CEGUIRadioButton_h_
#define _CEGUIRadioButton_h_

#include "../CEGUIBase.h"
#include "CEGUIButtonBase.h"
#include "CEGUIRadioButtonProperties.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Base class to provide the logic for Radio Button widgets.

*/
class CEGUIEXPORT RadioButton : public ButtonBase
{
public:
	static const String EventNamespace;				//!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

	/*************************************************************************
		Event name constants
	*************************************************************************/
	// generated internally by Window
    /** Event fired when the selected state of the radio button changes.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the RadioButton whose selected state has
     * changed.
     */
	static const String EventSelectStateChanged;


	/*************************************************************************
		Accessor Functions
	*************************************************************************/ 
	/*!
	\brief
		return true if the radio button is selected (has the checkmark)

	\return
		true if this widget is selected, false if the widget is not selected.
	*/
	bool	isSelected(void) const				{return d_selected;}

	
	/*!
	\brief
		return the groupID assigned to this radio button

	\return
		ulong value that identifies the Radio Button group this widget belongs to.
	*/
	ulong	getGroupID(void) const				{return d_groupID;}


	/*!
	\brief
		Return a pointer to the RadioButton object within the same group as this RadioButton, that
		is currently selected.

	\return
		Pointer to the RadioButton object that is the RadioButton within the same group as this RadioButton,
		and is attached to the same parent window as this RadioButton, that is currently selected.
		Returns NULL if no button within the group is selected, or if 'this' is not attached to a parent window.
	*/
	RadioButton*	getSelectedButtonInGroup(void) const;


	/*************************************************************************
		Manipulator Functions
	*************************************************************************/
	/*!
	\brief
		set whether the radio button is selected or not

	\param select
		true to put the radio button in the selected state, false to put the radio button in the
		deselected state.  If changing to the selected state, any previously selected radio button
		within the same group is automatically deselected.

	\return
		Nothing.
	*/
	void	setSelected(bool select);

	
	/*!
	\brief
		set the groupID for this radio button

	\param group
		ulong value specifying the radio button group that this widget belongs to.

	\return	
		Nothing.
	*/
	void	setGroupID(ulong group);


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	RadioButton(const String& type, const String& name);
	virtual ~RadioButton(void);


protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Deselect any selected radio buttons attached to the same parent within the same group
		(but not do not deselect 'this').
	*/
	void	deselectOtherButtonsInGroup(void) const;


	/*!
	\brief
		Return whether this window was inherited from the given class name at some point in the inheritance hierarchy.

	\param class_name
		The class name that is to be checked.

	\return
		true if this window was inherited from \a class_name. false if not.
	*/
	virtual bool	testClassName_impl(const String& class_name) const
	{
		if (class_name=="RadioButton")	return true;
		return ButtonBase::testClassName_impl(class_name);
	}


	/*************************************************************************
		New Radio Button Events
	*************************************************************************/
	/*!
	\brief
		event triggered internally when the select state of the button changes.
	*/
	virtual void	onSelectStateChanged(WindowEventArgs& e);


	/*************************************************************************
		Overridden Event handlers
	*************************************************************************/
	virtual void	onMouseButtonUp(MouseEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool		d_selected;				// true when radio button is selected (has checkmark)
	ulong		d_groupID;				// radio button group ID


private:
	/*************************************************************************
		Static Properties for this class
	*************************************************************************/
	static RadioButtonProperties::Selected	d_selectedProperty;
	static RadioButtonProperties::GroupID	d_groupIDProperty;


	/*************************************************************************
		Private methods
	*************************************************************************/
	void	addRadioButtonProperties(void);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIRadioButton_h_
