/************************************************************************
	filename: 	TLFrameWindow.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for Taharez Look Frame Window class.
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
#ifndef _TLFrameWindow_h_
#define _TLFrameWindow_h_

#include "TLModule.h"
#include "elements/CEGUIFrameWindow.h"
#include "CEGUIRenderableFrame.h"
#include "CEGUIRenderableImage.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Concrete FramwWindow class for the Taharez Look widget set.	
*/
class TAHAREZLOOK_API TLFrameWindow : public FrameWindow
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// Image related stuff
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	TopLeftFrameImageName[];		//!< Name of the image to use for the top-left corner of the frame.
	static const utf8	TopRightFrameImageName[];		//!< Name of the image to use for the top-right corner of the frame.
	static const utf8	BottomLeftFrameImageName[];		//!< Name of the image to use for the bottom-left corner of the frame.
	static const utf8	BottomRightFrameImageName[];	//!< Name of the image to use for the bottom-right corner of the frame.
	static const utf8	LeftFrameImageName[];			//!< Name of the image to use for the left edge of the frame.
	static const utf8	RightFrameImageName[];			//!< Name of the image to use for the right edge of the frame.
	static const utf8	TopFrameImageName[];			//!< Name of the image to use for the top edge of the frame.
	static const utf8	BottomFrameImageName[];			//!< Name of the image to use for the bottom edge of the frame.
	static const utf8	ClientBrushImageName[];			//!< Name of the image to use as the client area brush.

	// window type stuff
	static const utf8	TitlebarType[];					//!< Window type to create for the title bar.
	static const utf8	CloseButtonType[];				//!< Window type to create for the close button.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Look frame window objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLFrameWindow(const String& type, const String& name);


	/*!
	\brief
		Destructor for TLFrameWindow objects.
	*/
	virtual ~TLFrameWindow(void);


	/*!
	\brief
		Return a Rect object that describes, unclipped, the inner rectangle for this window.  The inner rectangle is
		typically an area that excludes some frame or other rendering that should not be touched by subsequent rendering.

	\return
		Rect object that describes, in unclipped screen pixel co-ordinates, the window object's inner rect area.
	*/
	virtual Rect	getUnclippedInnerRect(void) const;


protected:
	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
	virtual void	onSized(EventArgs& e);
	virtual void	onAlphaChanged(EventArgs& e);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Create a control based upon the Titlebar base class to be used as the title bar for this window.

	\return
		Pointer to an object who's class derives from Titlebar
	*/
	virtual Titlebar*	createTitlebar(void) const;


	/*!
	\brief
		Create a control based upon the PushButton base class, to be used at the close button for the window.

	\return
		Pointer to an object who's class derives from PushButton.
	*/
	virtual PushButton*	createCloseButton(void) const;


	/*!
	\brief
		Setup size and position for the title bar and close button widgets attached to this window

	\return
		Nothing.
	*/
	virtual void	layoutComponentWidgets();


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
		Implementation Data
	*************************************************************************/
	RenderableFrame		d_frame;			//!< Handles the frame for the window.
	RenderableImage		d_clientbrush;		//!< Handles the client clearing brush for the window.

	// stored sizes of frame regions
	float	d_frameLeftSize;				//!< Width of the left frame edge in pixels.
	float	d_frameRightSize;				//!< Width of the right frame edge in pixels.
	float	d_frameTopSize;				//!< Height of the top frame edge in pixels.
	float	d_frameBottomSize;			//!< Height of the bottom frame edge in pixels.
};


/*!
\brief
	Factory class for producing TLFrameWindow objects
*/
class TAHAREZLOOK_API TLFrameWindowFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Frame Window factory class.
	*/
	TLFrameWindowFactory(void) : WindowFactory((utf8*)"Taharez Frame Window") { }


	/*
	\brief
		Destructor for Taharez Frame Window factory class.
	*/
	~TLFrameWindowFactory(void){}


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

#endif	// end of guard _TLFrameWindow_h_
