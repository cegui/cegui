/************************************************************************
	filename: 	CEGUIButtonBase.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for ButtonBase widget
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
#ifndef _CEGUIButtonBase_h_
#define _CEGUIButtonBase_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Base class for all the 'button' type widgets (push button, radio button, check-box, etc)
*/
class CEGUIBASE_API ButtonBase : public Window
{
public:
	/*************************************************************************
		Accessor type functions
	*************************************************************************/
	/*!
	\brief
		return true if user is hovering over this widget (or it's pushed and user is not over it for highlight)

	\return
		true if the user is hovering or if the button is pushed and the mouse is not over the button.  Otherwise return false.
	*/
	bool	isHovering(void) const;


	/*!
	\brief
		Return true if the button widget is in the pushed state.

	\return
		true if the button-type widget is pushed, false if the widget is not pushed.
	*/
	bool	isPushed(void) const			{return d_pushed;}


protected:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for ButtonBase objects
	*/
	ButtonBase(const String& type, const String& name);


	/*!
	\brief
		Destructor for ButtonBase objects
	*/
	virtual ~ButtonBase(void);


	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
	virtual void	onMouseButtonDown(MouseEventArgs& e);
	virtual void	onMouseButtonUp(MouseEventArgs& e);
	virtual void	onCaptureLost(EventArgs& e);


	/*************************************************************************
		Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		Perform the rendering for this widget.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual	void	drawSelf(float z);


	/*!
	\brief
		Render the button-type widget in it's 'normal' state

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual void	drawNormal(float z)		= 0;


	/*!
	\brief
		Render the button-type widget in it's 'hover' (highlighted) state

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual void	drawHover(float z)			{drawNormal(z);}


	/*!
	\brief
		Render the button-type widget in it's 'pushed' state

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual void	drawPushed(float z)		{drawNormal(z);}


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool	d_pushed;			//!< true when widget is pushed
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIButtonBase_h_
