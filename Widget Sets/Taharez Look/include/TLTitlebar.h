/************************************************************************
	filename: 	TLTitlebar.h
	created:	9/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez Look Titlebar widget
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
#ifndef _TLTitlebar_h_
#define _TLTitlebar_h_

#include "TLModule.h"
#include "elements/CEGUITitlebar.h"
#include "CEGUIWindowFactory.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Titlebar class for the TaharezLook GUI scheme
*/
class TAHAREZLOOK_API TLTitlebar : public Titlebar
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	LeftEndSectionImageName[];		//!< Name of the image to use for the left section of the title bar.
	static const utf8	MiddleSectionImageName[];		//!< Name of the image to use for the middle section of the title bar.
	static const utf8	RightEndSectionImageName[];		//!< Name of the image to use for the right section of the title bar.

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Look Titlebar objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLTitlebar(const String& type, const String& name);


	/*!
	\brief
		Destructor for TLTitlebar objects.
	*/
	virtual ~TLTitlebar(void);

protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Perform the actual rendering for this Window.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual	void	drawSelf(float z);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// cache image objects used for rendering
	const Image*	d_leftImage;		//!< Image object used for the left edge of the title bar.
	const Image*	d_middleImage;		//!< Image object used for the middle section of the title bar.
	const Image*	d_rightImage;		//!< Image object used for the right edge of the title bar.
};



/*!
\brief
	Factory class for producing TLTitlebar objects
*/
class TAHAREZLOOK_API TLTitlebarFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLTitlebarFactory(void) : WindowFactory((utf8*)"Taharez Titlebar") { }
	~TLTitlebarFactory(void){}


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


#endif	// end of guard _TLTitlebar_h_
