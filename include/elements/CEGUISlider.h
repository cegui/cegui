/************************************************************************
	filename: 	CEGUISlider.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for Slider widget
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
#ifndef _CEGUISlider_h_
#define _CEGUISlider_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Base class for Slider widgets.
*/
class CEGUIBASE_API Slider : public Window
{
public:
	/*************************************************************************
		Event name constants
	*************************************************************************/
	static const utf8	ValueChanged[];			//!< Event fired when the slider value changes.


	/*************************************************************************
		Accessors
	*************************************************************************/
	/*!
	\brief
		return the current slider value.

	\return
		ulong value equal to the sliders current value.
	*/
	ulong	getCurrentValue(void) const			{return d_value;}


	/*!
	\brief
		return the maximum value set for this widget

	\return
		ulong value equal to the currently set maximum value for this slider.
	*/
	ulong	getMaxValue(void) const				{return d_maxValue;}


	/*************************************************************************
		Manipulators
	*************************************************************************/
	/*!
	\brief
		Initialises the Window based object ready for use.

	\note
		This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

	\return
		Nothing
	*/
	virtual	void	initialise(void);


	/*!
	\brief
		set the maximum value for the slider.  Note that the minimum value is fixed at 0.

	\param maxVal
		ulong value specifying the maximum value for this slider widget.

	\return
		Nothing.
	*/
	void	setMaxValue(ulong maxVal);


	/*!
	\brief
		set the current slider value.

	\param value
		ulong value specifying the new value for this slider widget.

	\return
		Nothing.
	*/
	void	setCurrentValue(ulong value);


protected:
	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Slider base class constructor
	*/
	Slider(const String& type, const String& name);


	/*!
	\brief
		Slider base class destructor
	*/
	virtual ~Slider(void);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Add slider specific events
	*/
	void	addSliderEvents(void);


	/*!
	\brief
		create a Thumb based widget to use as the thumb for this slider.
	*/
	virtual Thumb*	createThumb(void) const		= 0;


	/*!
	\brief
		layout the slider component widgets
	*/
	virtual void	layoutComponentWidgets(void)	= 0;


	/*!
	\brief
		update the size and location of the thumb to properly represent the current state of the slider
	*/
	virtual void	updateThumb(void)	= 0;


	/*!
	\brief
		return value that best represents current slider value given the current location of the thumb.

	\return
		ulong value that, given the thumb widget position, best represents the current value for the slider.
	*/
	virtual ulong	getValueFromThumb(void) const	= 0;


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
	virtual int		getAdjustDirectionFromPoint(const Point& pt) const	= 0;


	/*!
	\brief
		handler function for when thumb moves.
	*/
	void	handleThumbMoved(const EventArgs& e);


	/*************************************************************************
		New event handlers for slider widget
	*************************************************************************/
	/*!
	\brief
		Handler triggered when the slider value changes
	*/
	virtual void	onValueChanged(WindowEventArgs& e);


	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
	virtual void	onMouseButtonDown(MouseEventArgs& e);
	virtual void	onSized(EventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	ulong	d_value;		//!< current slider value
	ulong	d_maxValue;		//!< slider maximum value (minimum is fixed at 0)

	// Pointers to the controls that make up the slider
	Thumb*	d_thumb;		//!< widget used to represent the 'thumb' of the slider.
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUISlider_h_
