/************************************************************************
	filename: 	WLProgressBar.h
	created:	10/8/2004
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
#ifndef _WLProgressBar_h_
#define _WLProgressBar_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIRenderableFrame.h"
#include "elements/CEGUIProgressBar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Standard progress bar for the WindowsLook Gui Scheme.
*/
class WINDOWSLOOK_API WLProgressBar : public ProgressBar
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// image name constants
	static const utf8	ImagesetName[];					//!< Name of the Imageset containing the imagery to use.
	static const utf8	TopLeftFrameImageName[];		//!< Name of the image to use for the top-left corner of the frame.
	static const utf8	TopRightFrameImageName[];		//!< Name of the image to use for the top-right corner of the frame.
	static const utf8	BottomLeftFrameImageName[];		//!< Name of the image to use for the bottom-left corner of the frame.
	static const utf8	BottomRightFrameImageName[];	//!< Name of the image to use for the bottom-right corner of the frame.
	static const utf8	LeftFrameImageName[];			//!< Name of the image to use for the left edge of the frame.
	static const utf8	RightFrameImageName[];			//!< Name of the image to use for the right edge of the frame.
	static const utf8	TopFrameImageName[];			//!< Name of the image to use for the top edge of the frame.
	static const utf8	BottomFrameImageName[];			//!< Name of the image to use for the bottom edge of the frame.
	static const utf8	BackgroundImageName[];			//!< Name of the image to use as the background area brush.
	static const utf8	MouseCursorImageName[];			//!< Name of the image used for the mouse cursor.

	// colours
	static colour		ContainerBackgroundColour;		//!< Colour to use for container background.
	static colour		DefaultProgressColour;			//!< Default colour used for progress bar.

	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook progress bar objects
	*/
	WLProgressBar(const String& type, const String& name);


	/*!
	\brief
		Destructor for WindowsLook progress bar objects
	*/
	virtual ~WLProgressBar(void);

protected:
	/*************************************************************************
		Rendering layers
	*************************************************************************/
	static const uint	ContainerLayer;		//!< Layer to use for the container.
	static const uint	ProgressLayer;		//!< Layer to use for actual progress bar rendering.


	/*************************************************************************
		Overridden Event Handling Functions
	*************************************************************************/
	virtual void	onSized(WindowEventArgs& e);
	virtual void	onAlphaChanged(WindowEventArgs& e);


	/*************************************************************************
		Overridden Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		Perform rendering for this widget
	*/
	virtual void	drawSelf(float z);


	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		Store sizes of frame edges
	*/
	void	storeFrameSizes(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	const Image*	d_background;	//!< empty background image
	RenderableFrame	d_frame;		//!< Frame for the edit box.

	// frame image spacing
	float		d_frameLeftSize;
	float		d_frameTopSize;
	float		d_frameRightSize;
	float		d_frameBottomSize;

	// colours
	colour		d_progressColour;	//!< Colour used for rendering the progress bar.
};


/*!
\brief
	Factory class for producing WLProgressBar objects
*/
class WINDOWSLOOK_API WLProgressBarFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLProgressBarFactory(void) : WindowFactory(WLProgressBar::WidgetTypeName) { }
	~WLProgressBarFactory(void){}


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


#endif	// end of guard _WLProgressBar_h_
