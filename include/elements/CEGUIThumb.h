/************************************************************************
	filename: 	CEGUIThumb.h
	created:	25/4/2004
	author:		Paul D Turner
	
	purpose:	Interface for a 'Thumb' widget.  Intended to be used as
				part of other widgets such as scrollers and sliders.
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
#ifndef _CEGUIThumb_h_
#define _CEGUIThumb_h_

#include "CEGUIPushButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Base class for Thumb widget.
	
	The thumb widget is used to compose other widgets (like sliders and scroll bars).  You would
	not normally need to use this widget directly unless you are making a new widget of some type.
*/
class CEGUIBASE_API Thumb : public PushButton
{
public:
	/*************************************************************************
		Event name constants
	*************************************************************************/
	// generated internally by Window
	static const utf8	ThumbPositionChanged[];			//!< The position of the thumb widget has changed.


	/*************************************************************************
		Accessor Functions
	*************************************************************************/ 
	/*!
	\brief
		return whether hot-tracking is enabled or not.

	\return
		true if hot-tracking is enabled.  false if hot-tracking is disabled.
	*/
	bool	isHotTracked(void) const			{return d_hotTrack;}

	/*!
	\brief
		return whether the thumb is movable on the vertical axis.

	\return
		true if the thumb is movable along the vertical axis.
		false if the thumb is fixed on the vertical axis.
	*/
	bool	isVertFree(void) const				{return d_vertFree;}

	/*!
	\brief
		return whether the thumb is movable on the horizontal axis.

	\return
		true if the thumb is movable along the horizontal axis.
		false if the thumb is fixed on the horizontal axis.
	*/
	bool	isHorzFree(void) const				{return d_horzFree;}


	/*************************************************************************
		Manipulator Functions
	*************************************************************************/
	/*!
	\brief
		set whether the thumb uses hot-tracking.

	\param setting
		true to enable hot-tracking.  false to disable hot-tracking.

	\return
		Nothing.
	*/
	void	setHotTracked(bool setting)				{d_hotTrack = setting;}


	/*!
	\brief
		set whether thumb is movable on the vertical axis.

	\param setting
		true to allow movement of thumb along the vertical axis.  false to fix thumb on the vertical axis.

	\return
		nothing.
	*/
	void	setVertFree(bool setting)						{d_vertFree = setting;}


	/*!
	\brief
		set whether thumb is movable on the horizontal axis.

	\param setting
		true to allow movement of thumb along the horizontal axis.  false to fix thumb on the horizontal axis.

	\return
		nothing.
	*/
	void	setHorzFree(bool setting)						{d_horzFree = setting;}
	

	/*!
	\brief
		set the movement range of the thumb for the vertical axis.

		The values specified here are relative to the parent window for the thumb, and are specified in whichever
		metrics mode is active for the widget.

	\param min
		the minimum setting for the thumb on the vertical axis.

	\param max
		the maximum setting for the thumb on the vertical axis.

	\return
		Nothing.
	*/
	void	setVertRange(float min, float max);


	/*!
	\brief
		set the movement range of the thumb for the horizontal axis.

		The values specified here are relative to the parent window for the thumb, and are specified in whichever
		metrics mode is active for the widget.

	\param min
		the minimum setting for the thumb on the horizontal axis.

	\param max
		the maximum setting for the thumb on the horizontal axis.

	\return
		Nothing.
	*/
	void	setHorzRange(float min, float max);


protected:
	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Thumb objects
	*/
	Thumb(const String& type, const String& name);


	/*!
	\brief
		Destructor for Thumb objects
	*/
	virtual ~Thumb(void);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Add thumb specific events
	*/
	void	addThumbEvents(void);


	/*************************************************************************
		New Thumb Events
	*************************************************************************/
	/*!
	\brief
		event triggered internally when the position of the thumb
	*/
	virtual void	onThumbPositionChanged(WindowEventArgs& e);



	/*************************************************************************
		Overridden event handling routines
	*************************************************************************/
	virtual void	onMouseMove(MouseEventArgs& e);
	virtual void	onMouseButtonDown(MouseEventArgs& e);
	virtual void	onCaptureLost(EventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// general settings
	bool	d_hotTrack;					//!< true if events are to be sent real-time, else just when thumb is released
	bool	d_vertFree;					//!< true if thumb is movable vertically
	bool	d_horzFree;					//!< true if thumb is movable horizontally

	// operational limits
	float	d_vertMin, d_vertMax;		//!< vertical range
	float	d_horzMin, d_horzMax;		//!< horizontal range

	// internal state
	bool	d_beingDragged;				//!< true if thumb is being dragged
	Point	d_dragPoint;				//!< point where we are being dragged at.
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIThumb_h_
