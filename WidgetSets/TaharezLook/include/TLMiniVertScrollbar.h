/************************************************************************
	filename: 	TLMiniVertScrollbar.h
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez mini vertical scroll bar widget.
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
#ifndef _TLMiniVertScrollbar_h_
#define _TLMiniVertScrollbar_h_

#include "TLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Mini vertical scroll-bar bar for the Taharez Gui Scheme.
*/
class TAHAREZLOOK_API TLMiniVertScrollbar: public Scrollbar
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// Progress bar image names
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	ScrollbarBodyImageName[];		//!< Name of image to use for the main body of the scroll bar
	static const utf8	UpButtonNormalImageName[];		//!< Name of image to use for the up button in normal state.
	static const utf8	UpButtonHighlightImageName[];	//!< Name of image to use for the up button in highlighted state.
	static const utf8	DownButtonNormalImageName[];	//!< Name of image to use for the down button in normal state.
	static const utf8	DownButtonHighlightImageName[];	//!< Name of image to use for the down button in the highlighted state.

	// some layout stuff
	static const float	ThumbPositionX;			//!< Relative X co-ordinate for the thumb.
	static const float	ThumbWidth;				//!< Relative width of the thumb.
	static const float	BodyPositionX;			//!< Relative X co-ordinate for the body imagery.
	static const float	BodyWidth;				//!< Relative width for the body imagery.

	// type names for the component widgets
	static const utf8*	ThumbWidgetType;			//!< Type of widget to create for the scroll bar thumb;
	static const utf8*	IncreaseButtonWidgetType;	//!< Type of widget to create for the increase button (down arrow).
	static const utf8*	DecreaseButtonWidgetType;	//!< Type of widget to create for the decrease button (up arrow).


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez mini vertical scroll bar widgets
	*/
	TLMiniVertScrollbar(const String& type, const String& name);


	/*!
	\brief
		Destructor for Taharez mini vertical scroll bar widgets
	*/
	virtual ~TLMiniVertScrollbar(void);


protected:
	/*************************************************************************
		Implementation functions
	*************************************************************************/
	/*
	\brief
		create a PushButton based widget to use as the increase button for this scroll bar.
	*/
	virtual PushButton*	createIncreaseButton(void) const;


	/*!
	\brief
		create a PushButton based widget to use as the decrease button for this scroll bar.
	*/
	virtual PushButton*	createDecreaseButton(void) const;


	/*!
	\brief
		create a Thumb based widget to use as the thumb for this scroll bar.
	*/
	virtual Thumb*	createThumb(void) const;


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
	const Image*	d_body;			//!< Image for body segment.
};


/*!
\brief
	Factory class for producing TLMiniVertScrollbar objects
*/
class TAHAREZLOOK_API TLMiniVertScrollbarFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLMiniVertScrollbarFactory(void) : WindowFactory(TLMiniVertScrollbar::WidgetTypeName) { }
	~TLMiniVertScrollbarFactory(void){}


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


#endif	// end of guard _TLMiniVertScrollbar_h_
