/************************************************************************
	filename: 	TLSlider.h
	created:	22/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez slider widget.
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
#ifndef _TLSlider_h_
#define _TLSlider_h_

#include "TLModule.h"
#include "elements/CEGUISlider.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Slider class for the Taharez Gui Scheme.
*/
class TAHAREZLOOK_API TLSlider : public Slider
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// Image names
	static const utf8	ImagesetName[];				//!< Name of the imageset to use for rendering.
	static const utf8	ContainerImageName[];		//!< Name of the image to use for rendering the slider container

	// window type stuff
	static const utf8*	ThumbType;					//!< Window type to create for the sliders thumb.

	// layout constants
	static const float	ContainerPaddingX;


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez slider widgets
	*/
	TLSlider(const String& type, const String& name);


	/*!
	\brief
		Destructor for Taharez slider widgets
	*/
	virtual ~TLSlider(void);


protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		create a Thumb based widget to use as the thumb for this slider.
	*/
	virtual Thumb*	createThumb(const String& name) const;


	/*!
	\brief
		layout the slider component widgets
	*/
	virtual void	layoutComponentWidgets(void);


	/*!
	\brief
		update the size and location of the thumb to properly represent the current state of the slider
	*/
	virtual void	updateThumb(void);


	/*!
	\brief
		return value that best represents current slider value given the current location of the thumb.

	\return
		float value that, given the thumb widget position, best represents the current value for the slider.
	*/
	virtual float	getValueFromThumb(void) const;


	/*!
	\brief
		Given window location \a pt, return a value indicating what change should be 
		made to the slider.

	\param pt
		Point object describing a pixel position in window space.

	\return
		- -1 to indicate slider should be moved to a lower setting.
		-  0 to indicate slider should not be moved.
		- +1 to indicate slider should be moved to a higher setting.
	*/
	virtual float	getAdjustDirectionFromPoint(const Point& pt) const;


	/*************************************************************************
		Overridden Rendering Functions
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
		Implementation data
	*************************************************************************/
	const Image*	d_container;		//!< Pointer to the image to render as the slider container.
};


/*!
\brief
	Factory class for producing TLSlider objects
*/
class TAHAREZLOOK_API TLSliderFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLSliderFactory(void) : WindowFactory(TLSlider::WidgetTypeName) { }
	~TLSliderFactory(void){}


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


#endif	// end of guard _TLSlider_h_
