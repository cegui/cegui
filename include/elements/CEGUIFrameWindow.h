/************************************************************************
	filename: 	CEGUIFrameWindow.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for FrameWindow
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
#ifndef _CEGUIFrameWindow_h_
#define _CEGUIFrameWindow_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Abstract base class for a movable, sizable, window with a title-bar and a frame.
*/
class CEGUIBASE_API FrameWindow : public Window
{
public:
	/*************************************************************************
		Constants	
	*************************************************************************/
	// additional event names for this window
	static const utf8	RollupToggled[];		//!< Fired when the rollup (shade) state of the window changes
	static const utf8	CloseClicked[];			//!< Fired when the close button for the window is clicked.

	// other bits
	static const float	DefaultSizingBorderSize;	//!< Default size for the sizing border (in pixels)


	/*!
	\brief
		Enumeration that defines the set of possible locations for the mouse on a frame windows sizing border.
	*/
	enum SizingLocation {
		SizingNone,			//!< Position is not a sizing location.
		SizingTopLeft,		//!< Position will size from the top-left.
		SizingTopRight,		//!< Position will size from the top-right.
		SizingBottomLeft,	//!< Position will size from the bottom left.
		SizingBottomRight,	//!< Position will size from the bottom right.
		SizingTop,			//!< Position will size from the top.
		SizingLeft,			//!< Position will size from the left.
		SizingBottom,		//!< Position will size from the bottom.
		SizingRight,		//!< Position will size from the right.
	};

	/*!
	\brief
		Initialises the Window based object ready for use.

	\note
		This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

	\return
		Nothing
	*/
	virtual void	initialise(void);
	
	
	/*!
	\brief
		Return whether this window is sizable.  Note that this requires that the window have an enabled frame and that sizing itself is enabled

	\return
		true if the window can be sized, false if the window can not be sized
	*/
	bool	isSizingEnabled(void) const					{return d_sizingEnabled && isFrameEnabled();}


	/*!
	\brief
		Return whether the frame for this window is enabled.

	\return
		true if the frame for this window is enabled, false if the frame for this window is disabled.
	*/
	bool	isFrameEnabled(void) const					{return d_frameEnabled;}


	/*!
	\brief
		Return whether the title bar for this window is enabled.

	\return
		true if the window has a title bar and it is enabled, false if the window has no title bar or if the title bar is disabled.
	*/	
	bool	isTitleBarEnabled(void) const				{return (d_titlebar != NULL) && !((Window*)d_titlebar)->isDisabled();}


	/*!
	\brief
		Return whether this close button for this window is enabled.

	\return
		true if the window has a close button and it is enabled, false if the window either has no close button or if the close button is disabled.
	*/
	bool	isCloseButtonEnabled(void) const			{return (d_closeButton != NULL) && !((Window*)d_closeButton)->isDisabled();}


	/*!
	\brief
		Return whether roll up (a.k.a shading) is enabled for this window.

	\return
		true if roll up is enabled, false if roll up is disabled.
	*/
	bool	isRollupEnabled(void) const					{return d_rollupEnabled;}


	/*!
	\brief
		Return whether the window is currently rolled up (a.k.a shaded).

	\return
		true if the window is rolled up, false if the window is not rolled up.
	*/
	bool	isRolledup(void) const						{return d_rolledup;}


	/*!
	\brief
		Return the thickness of the sizing border.

	\return
		float value describing the thickness of the sizing border in screen pixels.
	*/
	float	getSizingBorderThickness(void) const		{return d_borderSize;}


	/*!
	\brief
		Enables or disables sizing for this window.

	\param setting
		set to true to enable sizing (also requires frame to be enabled), or false to disable sizing.

	\return
		nothing
	*/
	void	setSizingEnabled(bool setting);


	/*!
	\brief
		Enables or disables the frame for this window.

	\param setting
		set to true to enable the frame for this window, or false to disable the frame for this window.

	\return
		Nothing.
	*/
	void	setFrameEnabled(bool setting);


	/*!
	\brief
		Enables or disables the title bar for the frame window.

	\param setting
		set to true to enable the title bar (if one is attached), or false to disable the title bar.

	\return
		Nothing.
	*/
	void	setTitleBarEnabled(bool setting);


	/*!
	\brief
		Enables or disables the close button for the frame window.

	\param setting
		Set to true to enable the close button (if one is attached), or false to disable the close button.

	\return
		Nothing.
	*/
	void	setCloseButtonEnabled(bool setting);


	/*!
	\brief
		Enables or disables roll-up (shading) for this window.

	\param setting
		Set to true to enable roll-up for the frame window, or false to disable roll-up.

	\return
		Nothing.
	*/
	void	setRollupEnabled(bool setting);


	/*!
	\brief
		Toggles the state of the window between rolled-up (shaded) and normal sizes.  This requires roll-up to be enabled.

	\return
		Nothing
	*/
	void	toggleRollup(void);


	/*!
	\brief
		Set the size of the sizing border for this window.

	\param pixels
		float value specifying the thickness for the sizing border in screen pixels.

	\return
		Nothing.
	*/
	void	setSizingBorderThickness(float pixels)		{d_borderSize = pixels;}


	/*!
	\brief
		Set the font to use for the title bar text

	\param name
		String object holding the name of the font to use.

	\return
		Nothing.
	*/
	void	setTitlebarFont(const String& name);


	/*!
	\brief
		Set the font to use for the title bar text

	\param font
		Pointer to the font to use.

	\return
		Nothing.
	*/
	void	setTitlebarFont(Font* font);


	/*!
	\brief
		Move the window by the pixel offsets specified in \a offset.

		This is intended for internal system use - it is the method by which the title bar moves the frame window.

	\param offset
		Vector2 object containing the offsets to apply (offsets are in screen pixels).

	\return
		Nothing.
	*/
	void	offsetPixelPosition(const Vector2& offset);


	/*!
	\brief
		Return whether this FrameWindow can be moved by dragging the title bar.

	\return
		true if the Window will move when the user drags the title bar, false if the window will not move.
	*/
	bool	isDragMovingEnabled(void) const		{return d_dragMovable;}


	/*!
	\brief
		Set whether this FrameWindow can be moved by dragging the title bar.

	\param setting
		true if the Window should move when the user drags the title bar, false if the window should not move.

	\return
		Nothing.
	*/
	void	setDragMovingEnabled(bool setting);


protected:
	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for FrameWindow objects.
	*/
	FrameWindow(const String& name, const String& type);

	/*!
	\brief
		Destructor for FramwWindow objects.
	*/
	virtual ~FrameWindow(void);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Create a control based upon the Titlebar base class to be used as the title bar for this window.

	\return
		Pointer to an object who's class derives from Titlebar
	*/
	virtual Titlebar*	createTitlebar(void) const		= 0;


	/*!
	\brief
		Create a control based upon the PushButton base class, to be used at the close button for the window.

	\return
		Pointer to an object who's class derives from PushButton.
	*/
	virtual PushButton*	createCloseButton(void) const	= 0;


	/*!
	\brief
		Setup size and position for the title bar and close button widgets attached to this window

	\return
		Nothing.
	*/
	virtual void	layoutComponentWidgets()		= 0;


	/*!
	\brief
		move the window's left edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

	\param delta
		float value that specifies the amount to move the window edge, and in which direction.  Positive values make window smaller.
	*/
	void	moveLeftEdge(float delta);


	/*!
	\brief
		move the window's right edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

	\param delta
		float value that specifies the amount to move the window edge, and in which direction.  Positive values make window larger.
	*/
	void	moveRightEdge(float delta);


	/*!
	\brief
		move the window's top edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

	\param delta
		float value that specifies the amount to move the window edge, and in which direction.  Positive values make window smaller.
	*/
	void	moveTopEdge(float delta);


	/*!
	\brief
		move the window's bottom edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

	\param delta
		float value that specifies the amount to move the window edge, and in which direction.  Positive values make window larger.
	*/
	void	moveBottomEdge(float delta);


	/*!
	\brief
		check local pixel co-ordinate point 'pt' and return one of the
		SizingLocation enumerated values depending where the point falls on
		the sizing border.

	\param pt
		Point object describing, in pixels, the window relative offset to check.

	\return
		One of the SizingLocation enumerated values that describe which part of
		the sizing border that \a pt corresponded to, if any.
	*/
	SizingLocation FrameWindow::getSizingBorderAtPoint(const Point& pt) const;

 
	/*!
	\brief
		return true if given SizingLocation is on left edge.

	\param loc
		SizingLocation value to be checked.

	\return
		true if \a loc is on the left edge.  false if \a loc is not on the left edge.
	*/
	bool	isLeftSizingLocation(SizingLocation loc) const			{return ((loc == SizingLeft) || (loc == SizingTopLeft) || (loc == SizingBottomLeft));}


	/*!
	\brief
		return true if given SizingLocation is on right edge.

	\param loc
		SizingLocation value to be checked.

	\return
		true if \a loc is on the right edge.  false if \a loc is not on the right edge.
	*/
	bool	isRightSizingLocation(SizingLocation loc) const			{return ((loc == SizingRight) || (loc == SizingTopRight) || (loc == SizingBottomRight));}


	/*!
	\brief
		return true if given SizingLocation is on top edge.

	\param loc
		SizingLocation value to be checked.

	\return
		true if \a loc is on the top edge.  false if \a loc is not on the top edge.
	*/
	bool	isTopSizingLocation(SizingLocation loc) const			{return ((loc == SizingTop) || (loc == SizingTopLeft) || (loc == SizingTopRight));}


	/*!
	\brief
		return true if given SizingLocation is on bottom edge.

	\param loc
		SizingLocation value to be checked.

	\return
		true if \a loc is on the bottom edge.  false if \a loc is not on the bottom edge.
	*/
	bool	isBottomSizingLocation(SizingLocation loc) const		{return ((loc == SizingBottom) || (loc == SizingBottomLeft) || (loc == SizingBottomRight));}


	/*!
	\brief
		Add frame window specific events
	*/
	void	addFrameWindowEvents(void);


	/*!
	\brief
		Method to respond to close button click events and fire our close event
	*/
	void	closeClickHandler(const EventArgs& e);


	/*!
	\brief
		Set the appropriate mouse cursor for the given window-relative pixel point.
	*/
	void	setCursorForPoint(const Point& pt) const;


	/*!
	\brief
		Return a Rect that describes, in window relative pixel co-ordinates, the outer edge of the sizing area for this window.
	*/
	virtual	Rect	getSizingRect(void) const		{return Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight());}


	/*************************************************************************
		New events for Frame Windows
	*************************************************************************/
	/*!
	\brief
		Event generated internally whenever the roll-up / shade state of the window
		changes.
	*/
	virtual void	onRollupToggled(WindowEventArgs& e);


	/*!
	\brief
		Event generated internally whenever the close button is clicked.
	*/
	virtual void	onCloseClicked(WindowEventArgs& e);


	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
	virtual void	onMouseMove(MouseEventArgs& e);
	virtual void	onMouseButtonDown(MouseEventArgs& e);
	virtual void	onMouseButtonUp(MouseEventArgs& e);
	virtual void	onCaptureLost(WindowEventArgs& e);
	virtual void	onSized(WindowEventArgs& e);
	virtual void	onParentSized(WindowEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// frame data
	bool	d_frameEnabled;		//!< true if window frame should be drawn.

	// window roll-up data
	bool	d_rollupEnabled;	//!< true if roll-up of window is allowed.
	bool	d_rolledup;			//!< true if window is rolled up.
	Size	d_abs_openSize;		//!< stores original size of window when rolled-up.
	Size	d_rel_openSize;		//!< stores original size of window when rolled-up.

	// drag-sizing data
	bool	d_sizingEnabled;	//!< true if sizing is enabled for this window.
	bool	d_beingSized;		//!< true if window is being sized.
	float	d_borderSize;		//!< thickness of the sizing border around this window
	Point	d_dragPoint;		//!< point window is being dragged at.

	// composite controls
	Titlebar*	d_titlebar;				//!< points to the title bar widget.
	PushButton*	d_closeButton;			//!< points to close button widget.

	// images for cursor when on sizing border
	const Image*	d_nsSizingCursor;		//!< North/South sizing cursor image.
	const Image*	d_ewSizingCursor;		//!< East/West sizing cursor image.
	const Image*	d_nwseSizingCursor;		//!< North-West/South-East cursor image.
	const Image*	d_neswSizingCursor;		//!< North-East/South-West cursor image.

	bool	d_dragMovable;		//!< true if the window will move when dragged by the title bar.
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIFrameWindow_h_
