/************************************************************************
	filename: 	CEGUIComboDropList.h
	created:	13/6/2004
	author:		Paul D Turner
	
	purpose:	Interface for the Combobox Drop-List widget base class
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
#ifndef _CEGUIComboDropList_h_
#define _CEGUIComboDropList_h_

#include "elements/CEGUIListbox.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Base class for the combo box drop down list.  This is a specialisation of the Listbox class.
*/
class CEGUIBASE_API ComboDropList : public Listbox
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// Event names
	static const utf8	ListSelectionAccepted[];		//!< Event fired when the user confirms the selection by clicking the mouse.


	/*!
	\brief
		Initialise the Window based object ready for use.

	\note
		This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

	\return
		Nothing
	*/
	virtual void	initialise(void);


	/*************************************************************************
		Constructor & Destructor
	*************************************************************************/
	/*!
	\brief
		Constructor for ComboDropList base class
	*/
	ComboDropList(const String& type, const String& name);


	/*!
	\brief
		Destructor for ComboDropList base class
	*/
	virtual ~ComboDropList(void);


protected:
	/*!
	\brief
		Add drop-list specific events
	*/
	void	addComboDropListEvents(void);

	
	/*************************************************************************
		New event handlers
	*************************************************************************/
	/*!
	\brief
		Handler for when list selection is confirmed.
	*/
	void	onListSelectionAccepted(WindowEventArgs& e);


	/*************************************************************************
		Overridden Event handling
	*************************************************************************/
	virtual void	onMouseMove(MouseEventArgs& e);
	virtual void	onMouseButtonDown(MouseEventArgs& e);
	virtual void	onMouseButtonUp(MouseEventArgs& e);
	virtual void	onCaptureLost(WindowEventArgs& e);
	virtual void	onActivated(WindowEventArgs& e);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIComboDropList_h_
