/************************************************************************
	filename: 	TLCombobox.h
	created:	12/6/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez look combo box class
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
#ifndef _TLCombobox_h_
#define _TLCombobox_h_

#include "TLModule.h"
#include "elements/CEGUICombobox.h"
#include "CEGUIWindowFactory.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Combobox class for the Taharez look Gui scheme
*/
class TAHAREZLOOK_API TLCombobox : public Combobox
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Combobox
	*/
	TLCombobox(const String& type, const String& name);


	/*!
	\brief
		Destructor for Taharez Combobox
	*/
	virtual ~TLCombobox(void);


protected:
		/*************************************************************************
		Overridden Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		Perform rendering for this widget
	*/
	virtual void	drawSelf(float z);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
};

/*!
\brief
	Factory class for producing TLCombobox objects
*/
class TAHAREZLOOK_API TLComboboxFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLComboboxFactory(void) : WindowFactory((utf8*)"Taharez Combobox") { }
	~TLComboboxFactory(void){}


	/*!
	\brief
		Create a new Window object of whatever type this WindowFactory produces.

	\param name
		A unique name that is to be assigned to the newly created Window object

	\return
		Pointer to the new Window object.
	*/
	Window*	createWindow(const String& name);


	/*!
	\brief
		Destroys the given Window object.

	\param window
		Pointer to the Window object to be destroyed.

	\return
		Nothing.
	*/
	virtual void	destroyWindow(Window* window)	 { if (window->getType() == d_type) delete window; }
};

} // End of  CEGUI namespace section

#endif	// end of guard _TLCombobox_h_
