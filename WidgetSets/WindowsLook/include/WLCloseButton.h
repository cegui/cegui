/************************************************************************
	filename: 	WLCloseButton.h
	created:	9/8/2004
	author:		Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#ifndef _WLCloseButton_h_
#define _WLCloseButton_h_

#include "WLButton.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Customised push-button used for the close button control on WindowsLook
	frame windows.  This is required due to the custom clipping required
	for these windows
*/
class WINDOWSLOOK_API WLCloseButton : public WLButton
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook Close Button objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	WLCloseButton(const String& type, const String& name) : WLButton(type, name) {}


	/*!
	\brief
		Destructor for WLCloseButton objects.
	*/
	virtual ~WLCloseButton(void) {}


	/*!
	\brief
		return a Rect object describing the Window area in screen space.

	\return
		Rect object that describes the area covered by the Window.  The values in the returned Rect are in screen pixels.  The
		returned Rect is clipped as appropriate and depending upon the 'ClippedByParent' setting.

	\note
		This has now been made virtual to ease some customisations that require more specialised clipping requirements.
	*/
	virtual Rect	getPixelRect(void) const;
};


/*!
\brief
	Factory class for producing WLCloseButton objects
*/
class WINDOWSLOOK_API WLCloseButtonFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLCloseButtonFactory(void) : WindowFactory(WLCloseButton::WidgetTypeName) { }
	~WLCloseButtonFactory(void){}


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


#endif	// end of guard _WLCloseButton_h_
