/************************************************************************
	filename: 	WLSlider.h
	created:	10/8/2004
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
#ifndef _WLSlider_h_
#define _WLSlider_h_

#include "WLModule.h"
#include "elements/CEGUISlider.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Slider class for the WindowsLook Gui Scheme.
*/
class WINDOWSLOOK_API WLSlider : public Slider
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// Image names
	static const utf8	ImagesetName[];				//!< Name of the imageset to use for rendering.
	static const utf8	TrackLeftImageName[];		//!< Name of the image to use for the left end of the slider track.
	static const utf8	TrackMiddleImageName[];		//!< Name of the image to use for the middle section of the slider track.
	static const utf8	TrackRightImageName[];		//!< Name of the image to use for the right end of the slider track.
	static const utf8	CalibrationMarkImageName[];	//!< Name of the image to use for calibration 'tick' marks.
	static const utf8	MouseCursorImageName[];				//!< Name of the image used for the mouse cursor.

	// window type stuff
	static const utf8	ThumbType[];				//!< Window type to create for the sliders thumb.

	// defaults
	static const float	DefaultTickFrequency;


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook slider widgets
	*/
	WLSlider(const String& type, const String& name);


	/*!
	\brief
		Destructor for WindowsLook slider widgets
	*/
	virtual ~WLSlider(void);


protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		create a Thumb based widget to use as the thumb for this slider.
	*/
	virtual Thumb*	createThumb(void) const;


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
	const Image*	d_trackLeftImage;		//!< Pointer to the image to render as the slider track left end.
	const Image*	d_trackMiddleImage;		//!< Pointer to the image to render as the slider track middle section.
	const Image*	d_trackRightImage;		//!< Pointer to the image to render as the slider track right end.
	const Image*	d_calibrationTickImage;	//!< Pointer to the image to render as the slider calibration tick marks.

	float	d_calibrationFreq;		//!< Holds frequency to render a tick mark (you get 1 tick every d_calibrationFreq 'steps').
};


/*!
\brief
	Factory class for producing WLSlider objects
*/
class WINDOWSLOOK_API WLSliderFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLSliderFactory(void) : WindowFactory((utf8*)"WindowsLook/Slider") { }
	~WLSliderFactory(void){}


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


#endif	// end of guard _WLSlider_h_
