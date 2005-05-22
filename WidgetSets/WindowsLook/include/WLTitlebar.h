/************************************************************************
	filename: 	WLTitlebar.h
	created:	8/8/2004
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
#ifndef _WLTitlebar_h_
#define _WLTitlebar_h_

#include "WLModule.h"
#include "elements/CEGUITitlebar.h"
#include "CEGUIRenderableFrame.h"
#include "CEGUIRenderableImage.h"
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
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	TopLeftFrameImageName[];		//!< Name of the image to use for the top-left corner of the titlebar.
	static const utf8	TopRightFrameImageName[];		//!< Name of the image to use for the top-right corner of the titlebar.
	static const utf8	BottomLeftFrameImageName[];		//!< Name of the image to use for the bottom-left corner of the titlebar.
	static const utf8	BottomRightFrameImageName[];	//!< Name of the image to use for the bottom-right corner of the titlebar.
	static const utf8	LeftFrameImageName[];			//!< Name of the image to use for the left edge of the titlebar.
	static const utf8	RightFrameImageName[];			//!< Name of the image to use for the right edge of the titlebar.
	static const utf8	TopFrameImageName[];			//!< Name of the image to use for the top edge of the titlebar.
	static const utf8	BottomFrameImageName[];			//!< Name of the image to use for the bottom edge of the titlebar.
	static const utf8	FillImageName[];				//!< Name of the image to use to fill the titlebar.
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


	/*!
	\brief
		Store the sizes for the frame edges
	*/
	void	storeFrameSizes(void);


	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
	virtual void	onDraggingModeChanged(WindowEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	RenderableFrame		d_frame;		//!< Handles the frame for the titlebar.
	RenderableImage		d_fill;			//!< Handles the client clearing brush for the window.

	// stored sizes of frame regions
	float	d_frameLeftSize;			//!< Width of the left frame edge in pixels.
	float	d_frameRightSize;			//!< Width of the right frame edge in pixels.
	float	d_frameTopSize;				//!< Height of the top frame edge in pixels.
	float	d_frameBottomSize;			//!< Height of the bottom frame edge in pixels.
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
	WLTitlebarFactory(void) : WindowFactory(WLTitlebar::WidgetTypeName) { }
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
