/************************************************************************
	filename: 	WLButton.h
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
#ifndef _WLButton_h_
#define _WLButton_h_

#include "WLModule.h"
#include "elements/CEGUIPushButton.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIRenderableImage.h"
#include "CEGUIRenderableFrame.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Button class for the WindowsLook GUI scheme
*/
class WINDOWSLOOK_API WLButton : public PushButton
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	BackgroundImageName[];			//!< Name of image to use as background for widget.
	static const utf8	NormalLeftImageName[];			//!< Name of image to use for left edge (normal state)
	static const utf8	NormalRightImageName[];			//!< Name of image to use for right edge (normal state)
	static const utf8	NormalTopImageName[];			//!< Name of image to use for top edge (normal state)
	static const utf8	NormalBottomImageName[];		//!< Name of image to use for bottom edge (normal state)
	static const utf8	NormalTopLeftImageName[];		//!< Name of image to use for top-left corner (normal state)
	static const utf8	NormalTopRightImageName[];		//!< Name of image to use for top-right corner (normal state)
	static const utf8	NormalBottomLeftImageName[];	//!< Name of image to use for bottom-left corner (normal state)
	static const utf8	NormalBottomRightImageName[];	//!< Name of image to use for bottom-right corner (normal state)
	static const utf8	HoverLeftImageName[];			//!< Name of image to use for left edge (hover state)
	static const utf8	HoverRightImageName[];			//!< Name of image to use for right edge (hover state)
	static const utf8	HoverTopImageName[];			//!< Name of image to use for top edge (hover state)
	static const utf8	HoverBottomImageName[];			//!< Name of image to use for bottom edge (hover state)
	static const utf8	HoverTopLeftImageName[];		//!< Name of image to use for top-left corner (hover state)
	static const utf8	HoverTopRightImageName[];		//!< Name of image to use for top-right corner (hover state)
	static const utf8	HoverBottomLeftImageName[];		//!< Name of image to use for bottom-left corner (hover state)
	static const utf8	HoverBottomRightImageName[];	//!< Name of image to use for bottom-right corner (hover state)
	static const utf8	PushedLeftImageName[];			//!< Name of image to use for left edge (pushed state)
	static const utf8	PushedRightImageName[];			//!< Name of image to use for right edge (pushed state)
	static const utf8	PushedTopImageName[];			//!< Name of image to use for top edge (pushed state)
	static const utf8	PushedBottomImageName[];		//!< Name of image to use for bottom edge (pushed state)
	static const utf8	PushedTopLeftImageName[];		//!< Name of image to use for top-left corner (pushed state)
	static const utf8	PushedTopRightImageName[];		//!< Name of image to use for top-right corner (pushed state)
	static const utf8	PushedBottomLeftImageName[];	//!< Name of image to use for bottom-left corner (pushed state)
	static const utf8	PushedBottomRightImageName[];	//!< Name of image to use for bottom-right corner (pushed state)
	static const utf8	MouseCursorImageName[];			//!< Name of the image to use for the mouse cursor.

	// colours
	static const colour	NormalPrimaryColour;
	static const colour	NormalSecondaryColour;
	static const colour	HoverPrimaryColour;
	static const colour	HoverSecondaryColour;
	static const colour	PushedPrimaryColour;
	static const colour	PushedSecondaryColour;
	static const colour	DisabledPrimaryColour;
	static const colour	DisabledSecondaryColour;
	static const colour	EnabledTextLabelColour;
	static const colour	DisabledTextLabelColour;

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook Button objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	WLButton(const String& type, const String& name);


	/*!
	\brief
		Destructor for WLButton objects.
	*/
	virtual ~WLButton(void);

	
protected:
	/*************************************************************************
		Overridden Event Handling Functions
	*************************************************************************/
	virtual void	onSized(WindowEventArgs& e);
	virtual void	onAlphaChanged(WindowEventArgs& e);


	/*************************************************************************
		Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		render the button in the normal state.
	*/
	virtual void	drawNormal(float z);

	/*!
	\brief
		render the button in the hover / highlighted state.
	*/
	virtual void	drawHover(float z);

	/*!
	\brief
		render the button in the pushed state.
	*/
	virtual void	drawPushed(float z);

	/*!
	\brief
		render the button in the disabled state
	*/
	virtual void	drawDisabled(float z);

	/*************************************************************************
		Implementation methods
	*************************************************************************/
	/*!
	\brief
		Store sizes of frame edges
	*/
	void	storeFrameSizes(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// frames
	RenderableFrame		d_normalFrame;
	RenderableFrame		d_hoverFrame;
	RenderableFrame		d_pushedFrame;

	// frame image spacing
	float		d_frameLeftSize;
	float		d_frameTopSize;
	float		d_frameRightSize;
	float		d_frameBottomSize;

	// background image
	const Image*		d_background;		//!< Image used for button background.
};


/*!
\brief
	Factory class for producing WLButton objects
*/
class WINDOWSLOOK_API WLButtonFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLButtonFactory(void) : WindowFactory(WLButton::WidgetTypeName) { }
	~WLButtonFactory(void){}


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


#endif	// end of guard _WLButton_h_
