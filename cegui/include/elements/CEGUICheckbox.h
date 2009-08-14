/***********************************************************************
	filename: 	CEGUICheckbox.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for Checkbox Widget
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
#ifndef _CEGUICheckbox_h_
#define _CEGUICheckbox_h_

#include "../CEGUIBase.h"
#include "CEGUIButtonBase.h"
#include "CEGUICheckboxProperties.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Base class providing logic for Check-box widgets
*/
class CEGUIEXPORT Checkbox : public ButtonBase
{
public:
	static const String EventNamespace;				//!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

	/*************************************************************************
		Event name constants
	*************************************************************************/
	// generated internally by Window
	static const String EventCheckStateChanged;			//!< The check-state of the widget has changed.


	/*************************************************************************
		Accessor Functions
	*************************************************************************/
	/*!
	\brief
		return true if the check-box is selected (has the checkmark)

	\return
		true if the widget is selected and has the check-mark, false if the widget
		is not selected and does not have the check-mark.
	*/
	bool	isSelected(void) const						{return d_selected;}


	/*************************************************************************
		Manipulator Functions
	*************************************************************************/
	/*!
	\brief
		set whether the check-box is selected or not

	\param select
		true to select the widget and give it the check-mark.  false to de-select the widget and
		remove the check-mark.

	\return
		Nothing.
	*/
	void	setSelected(bool select);


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Checkbox class.
	*/
	Checkbox(const String& type, const String& name);


	/*!
	\brief
		Destructor for Checkbox class.
	*/
	virtual ~Checkbox(void);


protected:
	/*************************************************************************
		New event handlers
	*************************************************************************/
	/*!
	\brief
		event triggered internally when state of check-box changes
	*/
	virtual void	onSelectStateChange(WindowEventArgs& e);


	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
	virtual void	onMouseButtonUp(MouseEventArgs& e);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
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
		if (class_name=="Checkbox")	return true;
		return ButtonBase::testClassName_impl(class_name);
	}


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool		d_selected;					//!< true if check-box is selected (has checkmark)

private:
	/*************************************************************************
		Static Properties for this class
	*************************************************************************/
	static CheckboxProperties::Selected	d_selectedProperty;


	/*************************************************************************
		Private methods
	*************************************************************************/
	void	addCheckboxProperties(void);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUICheckbox_h_
