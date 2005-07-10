/************************************************************************
	filename: 	WLHorzScrollbar.h
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
#ifndef _WLHorzScrollbar_h_
#define _WLHorzScrollbar_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUIRenderableFrame.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Horizontal scroll-bar bar for the WindowsLook Gui Scheme.
*/
class WINDOWSLOOK_API WLHorzScrollbar: public Scrollbar
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
	static const utf8	LeftButtonNormalImageName[];	//!< Name of image to use for the up button in normal state.
	static const utf8	LeftButtonHighlightImageName[];	//!< Name of image to use for the up button in highlighted state.
	static const utf8	RightButtonNormalImageName[];	//!< Name of image to use for the down button in normal state.
	static const utf8	RightButtonHighlightImageName[];//!< Name of image to use for the down button in the highlighted state.
	static const utf8	MouseCursorImageName[];			//!< Name of the image used for the mouse cursor.

	// Colours
	static const colour	BackgroundColour;				//!< Colour to use as container background.

	// some layout stuff
	static const float	MinThumbWidth;					//!< Minimum width of the thumb in pixels.

	// type names for the component widgets
	static const utf8*	ThumbWidgetType;			//!< Type of widget to create for the scroll bar thumb;
	static const utf8*	IncreaseButtonWidgetType;	//!< Type of widget to create for the increase button (right arrow).
	static const utf8*	DecreaseButtonWidgetType;	//!< Type of widget to create for the decrease button (left arrow).


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook horizontal scroll bar widgets
	*/
	WLHorzScrollbar(const String& type, const String& name);


	/*!
	\brief
		Destructor for WindowsLook horizontal scroll bar widgets
	*/
	virtual ~WLHorzScrollbar(void);


protected:
	/*************************************************************************
		Implementation functions
	*************************************************************************/
	/*
	\brief
		create a PushButton based widget to use as the increase button for this scroll bar.
	*/
	virtual PushButton*	createIncreaseButton(const String& name) const;


	/*!
	\brief
		create a PushButton based widget to use as the decrease button for this scroll bar.
	*/
	virtual PushButton*	createDecreaseButton(const String& name) const;


	/*!
	\brief
		create a Thumb based widget to use as the thumb for this scroll bar.
	*/
	virtual Thumb*	createThumb(const String& name) const;


	/*!
	\brief
		layout the scroll bar component widgets
	*/
	virtual void	layoutComponentWidgets(void);


	/*!
	\brief
		update the size and location of the thumb to properly represent the current state of the scroll bar
	*/
	virtual void	updateThumb(void);


	/*!
	\brief
		return value that best represents current scroll bar position given the current location of the thumb.

	\return
		float value that, given the thumb widget position, best represents the current position for the scroll bar.
	*/
	virtual float	getValueFromThumb(void) const;


	/*!
	\brief
		Given window location \a pt, return a value indicating what change should be 
		made to the scroll bar.

	\param pt
		Point object describing a pixel position in window space.

	\return
		- -1 to indicate scroll bar position should be moved to a lower value.
		-  0 to indicate scroll bar position should not be changed.
		- +1 to indicate scroll bar position should be moved to a higher value.
	*/
	virtual float	getAdjustDirectionFromPoint(const Point& pt) const;


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
	RenderableFrame	d_frame;		//!< Frame for the scroll bar container.

	// images
	const Image*	d_background;	//!< Background for the edit box.

	// frame image spacing
	float		d_frameLeftSize;
	float		d_frameTopSize;
	float		d_frameRightSize;
	float		d_frameBottomSize;
};


/*!
\brief
	Factory class for producing WLHorzScrollbar objects
*/
class WINDOWSLOOK_API WLHorzScrollbarFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLHorzScrollbarFactory(void) : WindowFactory(WLHorzScrollbar::WidgetTypeName) { }
	~WLHorzScrollbarFactory(void){}


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


#endif	// end of guard _WLHorzScrollbar_h_
