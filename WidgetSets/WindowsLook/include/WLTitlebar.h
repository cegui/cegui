/************************************************************************
	filename: 	WLTitlebar.h
	created:	8/8/2004
	author:		Paul D Turner
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
#ifndef _WLTitlebar_h_
#define _WLTitlebar_h_

#include "WLModule.h"
#include "elements/CEGUITitlebar.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Titlebar class for the WindowsLook GUI scheme
*/
class WINDOWSLOOK_API WLTitlebar : public Titlebar
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	LeftEndSectionImageName[];		//!< Name of the image to use for the left section of the title bar.
	static const utf8	MiddleSectionImageName[];		//!< Name of the image to use for the middle section of the title bar.
	static const utf8	RightEndSectionImageName[];		//!< Name of the image to use for the right section of the title bar.
	static const utf8	NormalCursorImageName[];		//!< Name of the image to use as the mouse cursor for this window.
	static const utf8	NoDragCursorImageName[];		//!< Name of the image to use as mouse cursor when dragging is disabled.

	// colours
	static const colour	ActiveColour;			//!< Colour applied when parent window is active.
	static const colour	InactiveColour;			//!< Colour applied when parent window is inactive.
	static const colour	CaptionColour;			//!< Colour used when rendering the caption text.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Windows Look Titlebar objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	WLTitlebar(const String& type, const String& name);


	/*!
	\brief
		Destructor for WLTitlebar objects.
	*/
	virtual ~WLTitlebar(void);


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
		Overridden event handlers
	*************************************************************************/
	virtual void	onDraggingModeChanged(WindowEventArgs& e);


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
	Factory class for producing WLTitlebar objects
*/
class WINDOWSLOOK_API WLTitlebarFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLTitlebarFactory(void) : WindowFactory((utf8*)"WindowsLook/Titlebar") { }
	~WLTitlebarFactory(void){}


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


#endif	// end of guard _WLTitlebar_h_
