/************************************************************************
	filename: 	CEGUIWindow.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines abstract base class for Window objects
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
#ifndef _CEGUIWindow_h_
#define _CEGUIWindow_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIVector.h"
#include "CEGUIRect.h"
#include "CEGUISize.h"
#include "CEGUIEventSet.h"
#include "CEGUIPropertySet.h"
#include "CEGUISystem.h"
#include "CEGUIInputEvent.h"
#include "CEGUIWindowProperties.h"
#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Mode used for Window size and position metrics.

	Position information for a Window is always 'relative' to it's parent even in Absolute mode.
	In Relative mode, layout is maintained for different screen resolutions, and also  offers the
	ability for child windows to properly adjust their layout as their parent is sized.
*/
enum CEGUIBASE_API MetricsMode
{
	Relative,		//!< Metrics are specified as a decimal fraction of parent Window size.
	Absolute,		//!< Metrics are specified as whole pixels.
	Inherited,		//!< Metrics are inherited from parent.
};


/*!
\brief
	An abstract base class providing common functionality and specifying the required interface for derived classes.

	The Window base class is the only UI object class that the core of the system knows about, for this reason every
	other window, widget, or similar item within the system must be derived from Window.  The base class provides the
	common functionality required by all UI objects, and specifies the minimal interface required to be implemented by
	derived classes.
*/
class CEGUIBASE_API Window : public PropertySet, public EventSet
{
public:
	/*************************************************************************
		Event name constants
	*************************************************************************/
	// generated internally by Window
	static const utf8	ParentSized[];					//!< Parent of this Window has been re-sized.
	static const utf8	SizedEvent[];					//!< Window size has changed
	static const utf8	MovedEvent[];					//!< Window position has changed
	static const utf8	TextChangedEvent[];				//!< Text string for the Window has changed
	static const utf8	FontChangedEvent[];				//!< Font object for the Window has been changed
	static const utf8	AlphaChangedEvent[];			//!< Alpha blend value for the Window has changed
	static const utf8	IDChangedEvent[];				//!< Client assigned ID code for the Window has changed
	static const utf8	ActivatedEvent[];				//!< Window has been activated (has input focus)
	static const utf8	DeactivatedEvent[];				//!< Window has been deactivated (loses input focus)
	static const utf8	ShownEvent[];					//!< Window has been made visible
	static const utf8	HiddenEvent[];					//!< Window has been hidden from view
	static const utf8	EnabledEvent[];					//!< Window has been enabled (interaction is possible)
	static const utf8	DisabledEvent[];				//!< Window has been disabled (interaction is no longer possible)
	static const utf8	MetricsChangedEvent[];			//!< Active metrics mode has been modified
	static const utf8	ClippingChangedEvent[];			//!< Clipping by parent mode has been modified
	static const utf8	ParentDestroyChangedEvent[];	//!< Destruction by parent mode has been modified
	static const utf8	InheritsAlphaChangedEvent[];	//!< Alpha inherited from parent mode has been modified.
	static const utf8	AlwaysOnTopChangedEvent[];		//!< Always on top mode has been modified
	static const utf8	CaptureGainedEvent[];			//!< Window has captured all inputs
	static const utf8	CaptureLostEvent[];				//!< Window has lost it's capture on inputs
	static const utf8	RenderingStartedEvent[];		//!< Rendering of the Window has started
	static const utf8	RenderingEndedEvent[];			//!< Rendering for the Window has finished
	static const utf8	ChildAddedEvent[];				//!< A child Window has been added
	static const utf8	ChildRemovedEvent[];			//!< A child window has been removed
	static const utf8	DestructionStartedEvent[];		//!< Destruction of the Window is about to begin.
	static const utf8	ZChangedEvent[];				//!< The z-order of the window has changed

	// generated externally (inputs)
	static const utf8	MouseEntersEvent[];				//!< Mouse cursor has entered the Window.
	static const utf8	MouseLeavesEvent[];				//!< Mouse cursor has left the Window.
	static const utf8	MouseMoveEvent[];				//!< Mouse cursor was moved within the area of the Window.
	static const utf8	MouseWheelEvent[];				//!< Mouse wheel was scrolled within the Window.
	static const utf8	MouseButtonDownEvent[];			//!< A mouse button was pressed down within the Window.
	static const utf8	MouseButtonUpEvent[];			//!< A mouse button was released within the Window.
	static const utf8	MouseClickEvent[];				//!< A mouse button was clicked (down then up) within the Window.
	static const utf8	MouseDoubleClickEvent[];		//!< A mouse button was double-clicked within the Window.
	static const utf8	MouseTripleClickEvent[];		//!< A mouse button was triple-clicked within the Window.
	static const utf8	KeyDownEvent[];					//!< A key on the keyboard was pressed.
	static const utf8	KeyUpEvent[];					//!< A key on the keyboard was released.
	static const utf8	CharacterEvent[];				//!< A text character was typed on the keyboard.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Window base class

	\param type
		String object holding Window type (usually provided by WindowFactory).

	\param name
		String object holding unique name for the Window.
	*/
	Window(const String& type, const String& name);


	/*!
	\brief
		Destructor for Window base class
	*/
	virtual ~Window(void);


	/*************************************************************************
		Accessor functions
	*************************************************************************/
	/*!
	\brief
		return a String object holding the type name for this Window.

	\return
		String object holding the Window type.
	*/
	const String& getType(void) const		{return d_type;}


	/*!
	\brief
		return a String object holding the name of this Window.

	\return
		String object holding the unique Window name.
	*/
	const String& getName(void) const		{return d_name;}


	/*!
	\brief
		returns whether or not this Window is set to be destroyed when its parent is destroyed.

	\return
		true if the Window will be destroyed when its parent is destroyed, false if it will remain.
	*/
	bool	isDestroyedByParent(void) const		{return d_destroyedByParent;}


	/*!
	\brief
		returns whether or not this Window is an always on top (a.k.a 'topmost') Window.

	\return
		true if this Window is always show on top of other normal windows.  false if the Window has normal z-order behaviour.
	*/
	bool	isAlwaysOnTop(void) const			{return d_alwaysOnTop;}


	/*!
	\brief
		return true if the Window is currently disabled

	\return
		true if the window is disabled, false if the window is enabled.
	*/
	bool	isDisabled(void) const;


	/*!
	\brief
		return true if the Window is currently visible.

		A true return from this function does not mean that the window is not completely obscured by other windows, just that the window
		is processed when rendering and is not hidden.

	\return
		true if the window is drawn, false if the window is hidden and therefore ignored when rendering.
	*/
	bool	isVisible(void) const;


	/*!
	\brief
		return true if this is the active Window (the window that receives inputs)

		Mouse events are always sent to the window containing the mouse cursor regardless of what this reports (unless the window has captured
		inputs).  This mainly refers to where other (keyboard) inputs are sent.

	\return
		true if this window has input focus, or false if it does not.
	*/
	bool	isActive(void) const;


	/*!
	\brief
		return true if this Window is clipped so that its rendering does not pass outside its parent windows area.

	\return
		true if the window will be clipped by its parent window, or false if this windows rendering may pass outside its parents area
	*/
	bool	isClippedByParent(void) const		{return d_clippedByParent;}


	/*!
	\brief
		return the ID code currently assigned to this Window by client code.

	\return
		uint value equal to the currently assigned ID code for this Window.
	*/
	uint	getID(void) const		{return d_ID;}


	/*!
	\brief
		return the number of child Window objects currently attached to this Window.

	\return
		uint value equal to the number of Window objects directly attached to this Window as children.
	*/
	uint   getChildCount(void) const      {return (uint)d_children.size();}


	/*!
	\brief
		returns whether a Window with the specified name is currently attached to this Window as a child.

	\param name
		String object containing the name of the Window to look for.

	\return
		true if a Window named \a name is currently attached to this Window as a child, else false.
	*/
	bool	isChild(const String& name) const;


	/*!
	\brief
		returns whether at least one window with the given ID code is attached as a child.

	\note
		ID codes are client assigned and may or may not be unique, and as such, the return from this function
		will only have meaning to the client code.

	\param ID
		uint ID code to look for.

	\return
		true if a child window was found with the ID code \a ID, or false if no child window was found with the ID \a ID.
	*/
	bool	isChild(uint ID) const;


	/*!
	\brief
		return true if the given Window is a child of this window.

	\param window
		Pointer to the Window object to look for.

	\return
		true if Window object \a window is attached to this window as a child.
	*/
	bool	isChild(const Window* window) const;


	/*!
	\brief
		return a pointer to the child window with the specified name.

		This function will throw an exception if no child object with the given name is attached.  This decision
		was made (over returning NULL if no window was found) so that client code can assume that if the call
		returns it has a valid window pointer.  We provide the isChild() functions for checking if a given window
		is attached.

	\param name
		String object holding the name of the child window to return a pointer to.

	\return
		Pointer to the Window object attached to this window that has the name \a name.

	\exception UnknownObjectException	thrown if no window named \a name is attached to this Window.
	*/
	Window*	getChild(const String& name) const;


	/*!
	\brief
		return a pointer to the first attached child window with the specified ID.

		This function will throw an exception if no child object with the given ID is attached.  This decision
		was made (over returning NULL if no window was found) so that client code can assume that if the call
		returns it has a valid window pointer.  We provide the isChild() functions for checking if a given window
		is attached.

	\param ID
		uint value specifying the ID code of the window to return a pointer to.

	\return
		Pointer to the (first) Window object attached to this window that has the ID code \a ID.

	\exception UnknownObjectException	thrown if no window with the ID code \a ID is attached to this Window.
	*/
	Window* getChild(uint ID) const;


	/*!
	\brief
		return a pointer to the child window that is attached to 'this' at the given index.

	\note
		Window indeces are of limited value to client code, since any time a window is added, removed, or
		it's z-order is changed the indeces all change.

	\param idx
		Index of the child window whos pointer should be returned.  This value is not bounds checked,
		client code should ensure that this is less than the value returned by getChildCount().

	\return
		Pointer to the child window currently attached at index position \a idx
	*/
	Window*	getChildAtIdx(int idx) const		{return d_children[idx];}


	/*!
	\brief
		return a pointer to the Window that currently has input focus starting with this Window.

	\return
		Pointer to the window that is active (has input focus) starting at 'this.  Will return 'this' if this Window is active
		and either no children are attached or if none of the attached children are active.  Returns NULL if this Window (and
		therefore all children) are not active.
	*/
	Window*	getActiveChild(void);
	const Window* getActiveChild(void) const;


	/*!
	\brief
		return true if the specified Window is some ancestor of this Window

	\param name
		String object holding the name of the Window to check for.

	\return
		true if a Window named \a name is an ancestor (parent, or parent of parent, etc) of this Window, or false if not.
	*/
	bool	isAncestor(const String& name) const;


	/*!
	\brief
		return true if any Window with the given ID is some ancestor of this Window.

	\param ID
		uint value specifying the ID to look for.

	\return
		true if an ancestor (parent, or parent of parent, etc) was found with the ID code \a ID, else false.
	*/
	bool	isAncestor(uint ID) const;


	/*!
	\brief
		return true if the specified Window is some ancestor of this Window.

	\param window
		Pointer to the Window object to look for.

	\return
		true if \a window was found to be an ancestor (parent, or parent of parent, etc) of this Window, otherwise false.
	*/
	bool	isAncestor(const Window* window) const;


	/*!
	\brief
		return the Font object active for the Window.

	\return
		Pointer to the Font being used by this Window.  If the window has no assigned font, the default font is returned.
	*/
	const Font*		getFont(void) const;


	/*!
	\brief
		return the current text for the Window

	\return
		A String object that holds the current text for this Window.
	*/
	const String&	getText(void) const		{return d_text;}


	/*!
	\brief
		return true if the Window inherits alpha from its parent(s).

	\return
		true if the Window inherits alpha from its parent(s), false if the alpha for this Window is independant.
	*/
	bool	inheritsAlpha(void) const		{return d_inheritsAlpha;}


	/*!
	\brief
		return the current alpha value set for this Window

	\note
		The alpha value set for any given window may or may not be the final alpha value that is used when rendering.  All window
		objects, by default, inherit alpha from thier parent window(s) - this will blend child windows, relatively, down the line of
		inheritance.  This behaviour can be overridden via the setInheritsAlpha() method.  To return the true alpha value that will be
		applied when rendering, use the getEffectiveAlpha() method.

	\return
		the currently set alpha value for this Window.  Will be between 0.0f and 1.0f.
	*/
	float	getAlpha(void) const			{return d_alpha;}


	/*!
	\brief
		return the effective alpha value that will be used when rendering this window, taking into account inheritance of parent
		window(s) alpha.

	\return
		the effective alpha that will be applied to this Window when rendering.  Will be between 0.0f and 1.0f.
	*/
	float	getEffectiveAlpha(void) const;


	/*!
	\brief
		return a Rect object that describes the Window area.

	\return
		Rect object that describes the area covered by the Window.  The values in the returned Rect are in whatever form is set
		as the current metric type.  The returned Rect is unclipped and relative to the Window objects parent.
	*/
	Rect	getRect(void) const;


	/*!
	\brief
		return a Rect object describing the Window area in screen space.

	\return
		Rect object that describes the area covered by the Window.  The values in the returned Rect are in screen pixels.  The
		returned Rect is clipped as appropriate and depending upon the 'ClippedByParent' setting.

	\note
		This has now been made virtual to ease some customisations that require more specialised clipping requirements.
	*/
	virtual Rect	getPixelRect(void) const;


	/*!
	\brief
		return a Rect object describing the clipped inner area for this window.

	\return
		Rect object that describes, in appropriately clipped screen pixel co-ordinates, the window object's inner rect area.
	*/
	Rect	getInnerRect(void) const;


	/*!
	\brief
		return a Rect object describing the Window area unclipped, in screen space.

	\return
		Rect object that describes the area covered by the Window.  The values in the returned Rect are in screen pixels.  The
		returned rect is fully unclipped.
	*/
	Rect	getUnclippedPixelRect(void) const;


	/*!
	\brief
		Return a Rect object that describes, unclipped, the inner rectangle for this window.  The inner rectangle is
		typically an area that excludes some frame or other rendering that should not be touched by subsequent rendering.

	\return
		Rect object that describes, in unclipped screen pixel co-ordinates, the window object's inner rect area.
	*/
	virtual Rect	getUnclippedInnerRect(void) const;


	/*!
	\brief
		return the Window that currently has inputs captured.

	\return
		Pointer to the Window object that currently has inputs captured, or NULL if no Window has captured input.
	*/
	static	Window*	getCaptureWindow(void)		{return d_captureWindow;}


	/*!
	\brief
		return true if this Window has input captured.

	\return
		true if this Window has captured inputs, or false if some other Window, or no Window, has captured inputs.
	*/
	bool	isCapturedByThis(void) const		{return getCaptureWindow() == this;}


	/*!
	\brief
		return true if a child window has captured inputs.

	\return
		true if inputs are captured by a Window that is attached as a child of this Window, else false.
	*/
	bool	isCapturedByAncestor(void) const	{return isAncestor(getCaptureWindow());}


	/*!
	\brief
		return true if an ancestor window has captured inputs.

	\return
		true if inputs are captured by a Window that is some ancestor (parent, parent of parent, etc) of this Window, else false.
	*/
	bool	isCapturedByChild(void) const		{return isChild(getCaptureWindow());}


	/*!
	\brief
		check if the given position would hit this window.

	\param position
		Point object describing the position to check in screen pixels

	\return
		true if \a position 'hits' this Window, else false.
	*/
	virtual bool	isHit(const Point& position) const;


	/*!
	\brief
		return the child Window that is 'hit' by the given position

	\param position
		Point object that describes the position to check in screen pixels

	\return
		Pointer to the child Window that was hit according to the Point \a position, or NULL if no child window was hit.
	*/
	Window*	getChildAtPosition(const Point& position) const;


	/*!
	\brief
		return the current metrics mode employed by the Window

	\return
		One of the values of the MectricsMode enumerated type, that describes the current metrics in use by the Window.
	*/
	MetricsMode	getMetricsMode(void) const;


	/*!
	\brief
		return the x position of the window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		float value that specifies the x position of the Window relative to it's parent, depending on the metrics system in use for this
		Window, this value will specify either pixels or a decimal fraction of the width of the parent Window.
	*/
	float	getXPosition(void) const;


	/*!
	\brief
		return the y position of the window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		float value that specifies the y position of the Window relative to it's parent, depending on the metrics system in use for this
		Window, this value will specify either pixels or a decimal fraction of the height of the parent Window.
	*/
	float	getYPosition(void) const;


	/*!
	\brief
		return the position of the window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		Point object that describes the position of the Window relative to it's parent, depending on the metrics system in use for this
		Window, the values in the Point will specify either pixels or decimal fractions of the total width and height of the parent.
	*/
	Point	getPosition(void) const;


	/*!
	\brief
		return the width of the Window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		float value that specifies the width of the Window.  Depending upon the metrics system in use for this window, the return
		value will either be in pixels, or as a decimal fraction of the width of the parent Window.
	*/
	float	getWidth(void) const;

	/*!
	\brief
		return the height of the Window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		float value that specifies the height of the Window.  Depending upon the metrics system in use for this window, the return
		value will either be in pixels, or as a decimal fraction of the height of the parent Window.
	*/
	float	getHeight(void) const;


	/*!
	\brief
		return the size of the Window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		Size object that describes the dimensions of the Window.  Depending upon the metrics system in use for this window, the
		values will either be in pixels, or as decimal fractions of the width and height of the parent Window.
	*/
	Size	getSize(void) const;


	/*!
	\brief
		return the parent of this Window.

	\return
		Pointer to the Window object that is the parent of this Window.  This value can be NULL, in which case the Window is a GUI
		Sheet / Root.
	*/
	Window*	getParent(void) const				{return d_parent;}


	/*!
	\brief
		Return the current maximum size for this window.

	\return
		Size object describing the maximum size for this window.  If using absolute co-ordinates the returned object has it's values expressed
		as screen pixels.  If using relative co-ordinates the returned object has it's values expressed as fractions of the current display size.
	*/
	Size	getMaximumSize(void) const;


	/*!
	\brief
		Return the current minimum size for this window.

	\return
		Size object describing the minimum size for this window.  If using absolute co-ordinates the returned object has it's values expressed
		as screen pixels.  If using relative co-ordinates the returned object has it's values expressed as fractions of the current display size.
	*/
	Size	getMinimumSize(void) const;


	/*!
	\brief
		Return a pointer to the mouse cursor image to use when the mouse is within this window.

	\return
		Pointer to the mouse cursor image that will be used when the mouse enters this window.  May return NULL indicating no cursor.
	*/
	const Image*	getMouseCursor(void) const;


	/*!
	\brief
		Return the window area rect in relative metrics.

	\return
		Rect object describing this windows area, relative to the parent window, in parent relative metrics.
	*/
	Rect	getRelativeRect(void) const				{return d_rel_area;}


	/*!
	\brief
		Return the window position in relative metrics.

	\return
		Point object describing this windows position, relative to the parent window, in parent relative metrics.
	*/
	Point	getRelativePosition(void) const			{return d_rel_area.getPosition();}


	/*!
	\brief
		Return the window X position in relative metrics.

	\return
		float value describing this windows X position, relative to the parent window, in parent relative metrics.
	*/
	float	getRelativeXPosition(void) const		{return d_rel_area.d_left;}


	/*!
	\brief
		Return the window Y position in relative metrics.

	\return
		float value describing this windows Y position, relative to the parent window, in parent relative metrics.
	*/
	float	getRelativeYPosition(void) const		{return d_rel_area.d_top;}


	/*!
	\brief
		Return the window size in relative metrics.

	\return
		Size object describing this windows size in parent relative metrics.
	*/
	Size	getRelativeSize(void) const				{return d_rel_area.getSize();}


	/*!
	\brief
		Return the window width in relative metrics.

	\return
		float value describing this windows width in parent relative metrics.
	*/
	float	getRelativeWidth(void) const			{return d_rel_area.getWidth();}


	/*!
	\brief
		Return the window height in relative metrics.

	\return
		float value describing this windows height in parent relative metrics.
	*/
	float	getRelativeHeight(void) const			{return d_rel_area.getHeight();}


	/*!
	\brief
		Return the window area rect in absolute metrics.

	\return
		Rect object describing this windows area, relative to the parent window, in absolute metrics
	*/
	Rect	getAbsoluteRect(void) const				{return d_abs_area;}


	/*!
	\brief
		Return the window position in absolute metrics.

	\return
		Point object describing this windows position, relative to the parent window, in absolute metrics.
	*/
	Point	getAbsolutePosition(void) const			{return d_abs_area.getPosition();}


	/*!
	\brief
		Return the window X position in absolute metrics.

	\return
		float value describing this windows X position, relative to the parent window, in absolute metrics.
	*/
	float	getAbsoluteXPosition(void) const		{return d_abs_area.d_left;}


	/*!
	\brief
		Return the window Y position in absolute metrics.

	\return
		float value describing this windows Y position, relative to the parent window, in absolute metrics.
	*/
	float	getAbsoluteYPosition(void) const		{return d_abs_area.d_top;}


	/*!
	\brief
		Return the window size in absolute metrics.

	\return
		Size object describing this windows size in absolute metrics.
	*/
	Size	getAbsoluteSize(void) const				{return d_abs_area.getSize();}


	/*!
	\brief
		Return the window width in absolute metrics.

	\return
		float value describing this windows width in absolute metrics.
	*/
	float	getAbsoluteWidth(void) const			{return d_abs_area.getWidth();}


	/*!
	\brief
		Return the window height in absolute metrics.

	\return
	float value describing this windows height in absolute metrics.
	*/
	float	getAbsoluteHeight(void) const			{return d_abs_area.getHeight();}


	/*!
	\brief
		Return the user data set for this Window.

		Each Window can have some client assigned data attached to it, this data is not used by the GUI system
		in any way.  Interpretation of the data is entirely application specific.

	\return
		pointer to the user data that is currently set for this window.
	*/
	void*	getUserData(void) const			{return d_userData;}


	/*!
	\brief
		return the x position of the window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\return
		float value that specifies the x position of the Window relative to it's parent, using the specified MetricsMode.
	*/
	float	getXPosition(MetricsMode mode) const;


	/*!
	\brief
		return the y position of the window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\return
		float value that specifies the y position of the Window relative to it's parent,  using the specified MetricsMode.
	*/
	float	getYPosition(MetricsMode mode) const;


	/*!
	\brief
		return the position of the window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\return
		Point object that describes the position of the Window relative to it's parent, using the specified MetricsMode.
	*/
	Point	getPosition(MetricsMode mode) const;


	/*!
	\brief
		return the width of the Window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\return
		float value that specifies the width of the Window using the specified MetricsMode.
	*/
	float	getWidth(MetricsMode mode) const;


	/*!
	\brief
		return the height of the Window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\return
		float value that specifies the height of the Window using the specified MetricsMode.
	*/
	float	getHeight(MetricsMode mode) const;


	/*!
	\brief
		return the size of the Window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\return
		Size object that describes the dimensions of the Window using the specified MetricsMode.
	*/
	Size	getSize(MetricsMode mode) const;


	/*!
	\brief
		return a Rect object that describes the Window area using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\return
		Rect object that describes the area covered by the Window using the specified MetricsMode.
	*/
	Rect	getRect(MetricsMode mode) const;


	/*!
	\brief
		Return whether this window is set to restore old input capture when it loses input capture.

		This is only really useful for certain sub-components for widget writers.

	\return
		- true if the window will restore the previous capture window when it loses input capture.
		- false if the window will set the capture window to NULL when it loses input capture (this is the default behaviour).
	*/
	bool	restoresOldCapture(void) const		{return d_restoreOldCapture;}


	/*************************************************************************
		Manipulator functions
	*************************************************************************/
	/*!
	\brief
		Initialises the Window based object ready for use.

	\note
		This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

	\return
		Nothing
	*/
	virtual void	initialise(void) {}


	/*!
	\brief
		Set whether or not this Window will automatically be destroyed when its parent Window is destroyed.

	\param setting
		set to true to have the Window auto-destroyed when its parent is destroyed (default), or false to have the Window
		remain after its parent is destroyed.

	\return
		Nothing
	*/
	void	setDestroyedByParent(bool setting);


	/*!
	\brief
		Set whether this window is always on top, or not.

	\param setting
		true to have the Window appear on top of all other non always on top windows, or false to allow the window to be covered by other windows.

	\return
		Nothing
	*/
	void	setAlwaysOnTop(bool setting);


	/*!
	\brief
		Set whether this window is enabled or disabled.  A disabled window normally can not be interacted with, and may have different rendering.

	\param setting
		true to enable the Window, and false to disable the Window.

	\return
		Nothing
	*/
	void	setEnabled(bool setting);


	/*!
	\brief
		enable the Window to allow interaction.

	\return
		Nothing
	*/
	void	enable(void)		{setEnabled(true);}


	/*!
	\brief
		disable the Window to prevent interaction.

	\return
		Nothing
	*/
	void	disable(void)		{setEnabled(false);}


	/*!
	\brief
		Set whether the Window is visible or hidden.

	\param setting
		true to make the Window visible, or false to make the Window hidden

	\return
		Nothing
	*/
	void	setVisible(bool setting);


	/*!
	\brief
		show the Window

	\return
		Nothing
	*/
	void	show(void)			{setVisible(true);}


	/*!
	\brief
		hide the Window.

	\return	
		Nothing
	*/
	void	hide(void)			{setVisible(false);}


	/*!
	\brief
		Activate the Window giving it input focus and bringing it to the top of all non always-on-top Windows.

	\note
		A Window cannot be programmatically 'disabled', as such.  To disable a Window, you must activate another one.

	\return
		Nothing
	*/
	void	activate(void);


	/*!
	\brief
		Set whether this Window will be clipped by its parent window(s).

	\param setting
		true to have the Window clipped so that rendering is constrained to within the area of its parent(s), or false to have rendering constrained
		to the screen only.

	\return
		Nothing
	*/
	void	setClippedByParent(bool setting);
	

	/*!
	\brief
		Set the current ID for the Window.

	\param ID
		Client assigned ID code for this Window.  The GUI system assigns no meaning to any IDs, they are a device purely for client code usage.

	\return
		Nothing
	*/
	void	setID(uint ID);


	/*!
	\brief
		Set the current text string for the Window.

	\param text
		String object containing the text that is to be set as the Window text.

	\return
		Nothing
	*/
	void	setText(const String& text);


	/*!
	\brief
		Set the current width of the Window.  Interpretation of the input value \a width is dependant upon the current metrics system set for the Window.

	\param width
		float value that specifies the new width for the window, in units consistent with whatever metrics mode is in operation.

	\return
		Nothing
	*/
	void	setWidth(float width);


	/*!
	\brief
		Set the current height of the Window.  Interpretation of the input value \a height is dependant upon the current metrics system set for the Window.

	\param height
		float value that specifies the new height for the window, in units consistent with whatever metrics mode is in operation.

	\return
		Nothing
	*/
	void	setHeight(float height);


	/*!
	\brief
		Set the current size of the Window.  Interpretation of the input value \a size is dependant upon the current metrics system set for the Window.

	\param size
		Size object that describes the new dimensions for the window, in units consistent with whatever metrics mode is in operation.

	\return
		Nothing
	*/
	void	setSize(const Size& size);


	/*!
	\brief
		Set the current 'x' position of the Window.  Interpretation of the input value \a x is dependant upon the current metrics system set for the Window.

	\param x
		float value that specifies the new x postion of the Window, in units consistent with the current metrics mode.

	\return
		Nothing
	*/
	void	setXPosition(float x);


	/*!
	\brief
		Set the current 'y' position of the Window.  Interpretation of the input value \a y is dependant upon the current metrics system set for the Window.

	\param y
		float value that specifies the new y postion of the Window, in units consistent with the current metrics mode.

	\return
		Nothing
	*/
	void	setYPosition(float y);


	/*!
	\brief
		Set the current position of the Window.  Interpretation of the input value \a position is dependant upon the current metrics system set for the Window.

	\param position
		Point object that describes the new postion of the Window, in units consistent with the current metrics mode.

	\return
		Nothing
	*/
	void	setPosition(const Point& position);


	/*!
	\brief
		Set the current area for the Window, this allows for setting of position and size at the same time.  
		Interpretation of the input value \a area is dependant upon the current metrics system set for the Window.

	\param area
		Rect object that describes the new area for Window, in units consistent with the current metrics mode.

	\return
		Nothing
	*/
	void	setAreaRect(const Rect& area);

	
	/*!
	\brief
		Set the font used by this Window.

	\param font
		Pointer to the Font object to be used by this Window.  If \a font is NULL, the default font will be used.

	\return
		Nothing
	*/
	void	setFont(const Font* font);


	/*!
	\brief
		Set the font used by this Window.

	\param name
		String object holding the name of the Font object to be used by this Window.  If \a name == "", the default font will be used.

	\return
		Nothing

	\exception UnknownObjectException	thrown if the specified Font is unknown within the system.
	*/
	void	setFont(const String& name);


	/*!
	\brief
		Add the named Window as a child of this Window.  If the Window \a name is already attached to a Window, it is detached before
		being added to this Window.

	\param name
		String object holding the name of the Window to be added.

	\return
		Nothing.

	\exception UnknownObjectException	thrown if no Window named \a name exists.
	\exception InvalidRequestException	thrown if Window \a name is an ancestor of this Window, to prevent cyclic Window structures.
	*/
	void	addChildWindow(const String& name);


	/*!
	\brief
		Add the specified Window as a child of this Window.  If the Window \a window is already attached to a Window, it is detached before
		being added to this Window.

	\param window
		Pointer to the Window object to be added.

	\return
		Nothing

	\exception InvalidRequestException	thrown if Window \a window is an ancestor of this Window, to prevent cyclic Window structures.
	*/
	void	addChildWindow(Window* window);


	/*!
	\brief
		Remove the named Window from this windows child list.

	\param name
		String object holding the name of the Window to be removed.  If the Window specified is not attached to this Window, nothing happens.

	\return
		Nothing.
	*/
	void	removeChildWindow(const String& name);


	/*!
	\brief
		Remove the specified Window form this windows child list.

	\param window
		Pointer to the Window object to be removed.  If the \a window is not attached to this Window, then nothing happens.

	\return
		Nothing.
	*/
	void	removeChildWindow(Window* window);


	/*!
	\brief
		Remove the first child Window with the specified ID.  If there is more than one attached Window objects with the specified ID, only the fist
		one encountered will be removed.

	\param ID
		ID number assigned to the Window to be removed.  If no Window with ID code \a ID is attached, nothing happens.

	\return
		Nothing.
	*/
	void	removeChildWindow(uint ID);


	/*!
	\brief
		Move the Window to the top of the z order.

		- If the Window is a non always-on-top window it is moved the the top of all other non always-on-top sibling windows, and the process
			repeated for all ancestors.
		- If the Window is an always-on-top window it is moved to the of of all sibling Windows, and the process repeated for all ancestors.

	\return
		Nothing
	*/
	void	moveToFront();


	/*!
	\brief
		Move the Window to the bottom of the Z order.

		- If the window is non always-on-top the Window is sent to the very bottom of its sibling windows and the process repeated for all ancestors.
		- If the window is always-on-top, the Window is sent to the bottom of all sibling always-on-top windows and the process repeated for all ancestors.

	\return
		Nothing
	*/
	void	moveToBack();


	/*!
	\brief
		Captures input to this window

	\return
		Nothing
	*/
	void	captureInput(void);


	/*!
	\brief
		Releases input capture from this Window.  If this Window does not have inputs captured, nothing happens.

	\return
		Nothing
	*/
	void	releaseInput(void);


	/*!
	\brief
		Set whether this window will remember and restore the previous window that had inputs captured.

	\param setting
		- true: The window will remember and restore the previous capture window.  The CaptureLost event is not fired
		  on the previous window when this window steals input capture.  When this window releases capture, the old capture
		  window is silently restored.

		- false: Input capture works as normal, each window losing capture is signalled via CaptureLost, and upon the final
		  release of capture, no previous setting is restored (this is the default 'normal' behaviour).

	\return
		Nothing
	*/
	void	setRestoreCapture(bool setting);


	/*!
	\brief
		Set the current alpha value for this window.

	\note
		The alpha value set for any given window may or may not be the final alpha value that is used when rendering.  All window
		objects, by default, inherit alpha from thier parent window(s) - this will blend child windows, relatively, down the line of
		inheritance.  This behaviour can be overridden via the setInheritsAlpha() method.  To return the true alpha value that will be
		applied when rendering, use the getEffectiveAlpha() method.

	\param alpha
		The new alpha value for the window.  Value should be between 0.0f and 1.0f.

	\return
		Nothing
	*/
	void	setAlpha(float alpha);


	/*!
	\brief
		Sets whether this Window will inherit alpha from its parent windows.

	\param setting
		true if the Window should use inherited alpha, or false if the Window should have an independant alpha value.

	\return
		Nothing
	*/
	void	setInheritsAlpha(bool setting);


	/*!
	\brief
		Signal the System object to redraw (at least) this Window on the next render cycle.

	\return
		Nothing
	*/
	void	requestRedraw(void) const;


	/*!
	\brief
		set the current metrics mode employed by the Window

	\param mode
		One of the values of the MectricsMode enumerated type, that describes the metrics mode to be used by the Window.

	\return
		Nothing
	*/
	void setMetricsMode(MetricsMode	mode);


	/*!
	\brief
		Set the minimum size for this window.

	\param sz
		Size object describing the minimum size for the window.  For absolute metrics, the Size values are in screen pixels,
		for relative metrics the Size values are relative to the display size.
	*/
	void	setMinimumSize(const Size& sz);


	/*!
	\brief
		Set the maximum size for this window.

	\param sz
		Size object describing the maximum size for the window.  For absolute metrics, the Size values are in screen pixels,
		for relative metrics the Size values are relative to the display size.
	*/
	void	setMaximumSize(const Size& sz);


	/*!
	\brief
		Set the mouse cursor image to be used when the mouse enters this window.

	\param image
		Pointer to the Image object to use as the mouse cursor image when the mouse enters the area for this Window.

	\return
		Nothing.
	*/
	void	setMouseCursor(const Image* image)		{d_mouseCursor = image;}


	/*!
	\brief
		Set the mouse cursor image to be used when the mouse enters this window.

	\param image
		One of the MouseCursorImage enumerated values.

	\return
		Nothing.
	*/
	void	setMouseCursor(MouseCursorImage image)		{d_mouseCursor = (const Image*)image;}


	/*!
	\brief
		Set the mouse cursor image to be used when the mouse enters this window.

	\param imageset
		String object that contains the name of the Imageset that contains the image to be used.

	\param image_name
		String object that contains the name of the Image on \a imageset that is to be used.

	\return
		Nothing.

	\exception UnknownObjectException	thrown if \a imageset is not known, or if \a imageset contains no Image named \a image_name.
	*/
	void	setMouseCursor(const String& imageset, const String& image_name);


	/*!
	\brief
		Set the user data set for this Window.

		Each Window can have some client assigned data attached to it, this data is not used by the GUI system
		in any way.  Interpretation of the data is entirely application specific.

	\param user_data
		pointer to the user data that is to be set for this window.

	\return
		Nothing.
	*/
	void	setUserData(void* user_data)		{d_userData = user_data;}


	/*!
	\brief
		set the x position of the window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\param x
		float value that specifies the x position of the Window relative to it's parent, using the specified MetricsMode.

	\return
		Nothing.
	*/
	void	setXPosition(MetricsMode mode, float x);


	/*!
	\brief
		set the y position of the window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\param y
		float value that specifies the y position of the Window relative to it's parent,  using the specified MetricsMode.

	\return
		Nothing.
	*/
	void	setYPosition(MetricsMode mode, float y);


	/*!
	\brief
		set the position of the window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\param position
		Point object that describes the position of the Window relative to it's parent, using the specified MetricsMode.

	\return
		Nothing
	*/
	void	setPosition(MetricsMode mode, const Point& position);


	/*!
	\brief
		set the width of the Window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\param width
		float value that specifies the width of the Window using the specified MetricsMode.

	\return
		Nothing.
	*/
	void	setWidth(MetricsMode mode, float width);


	/*!
	\brief
		set the height of the Window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\param height
		float value that specifies the height of the Window using the specified MetricsMode.

	\return
		Nothing.
	*/
	void	setHeight(MetricsMode mode, float height);


	/*!
	\brief
		set the size of the Window using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\param size
		Size object that describes the dimensions of the Window using the specified MetricsMode.

	\return
		Nothing.
	*/
	void	setSize(MetricsMode mode, const Size& size);


	/*!
	\brief
		set the Rect that describes the Window area using the specified metrics system.

	\param mode
		One of the MetricsMode enumerated values specifying the metrics system to be used for the return value.

	\param area
		Rect object that describes the area to be covered by the Window using the specified MetricsMode.

	\return
		Nothing.
	*/
	void	setRect(MetricsMode mode, const Rect& area);


	/*************************************************************************
		Co-ordinate and Size Conversion Functions
	*************************************************************************/
	/*!
	\brief
		Convert the given X co-ordinate from absolute to relative metrics.

	\param val
		X co-ordinate specified in pixels relative to this Window (so 0 is this windows left edge).

	\return
		A relative metric value that is equivalent to \a val, given the Window objects current width.
	*/
	float	absoluteToRelativeX(float val) const;


	/*!
	\brief
		Convert the given Y co-ordinate from absolute to relative metrics.

	\param val
		Y co-ordinate specified in pixels relative to this Window (so 0 is this windows top edge).

	\return
		A relative metric value that is equivalent to \a val, given the Window objects current height.
	*/
	float	absoluteToRelativeY(float val) const;


	/*!
	\brief
		Convert the given position from absolute to relative metrics.

	\param pt
		Point object that describes a position specified in pixels relative to this Window (so 0,0 is this windows top-left corner).

	\return
		A Point object describing a relative metric position that is equivalent to \a pt, given the Window objects current size.
	*/
	Point	absoluteToRelative(const Point& pt) const;


	/*!
	\brief
		Convert the given size from absolute to relative metrics.

	\param sze
		Size object that describes a size specified in pixels.

	\return
		A Size object describing a relative metric size that is equivalent to \a sze, given the Window objects current size.
	*/
	Size	absoluteToRelative(const Size& sze) const;


	/*!
	\brief
		Convert the given area from absolute to relative metrics.

	\param rect
		Rect object describing the area specified in pixels relative to this Window.

	\return
		A Rect object describing a relative metric area that is equivalent to \a rect, given the Window objects current size.
	*/
	Rect	absoluteToRelative(const Rect& rect) const;


	/*!
	\brief
		Convert the given X co-ordinate from relative to absolute metrics.

	\param val
		X co-ordinate specified in relative metrics for this Window (so 0 is this windows left edge).

	\return
		An absolute metric value that is equivalent to \a val, given the Window objects current width.
	*/
	float	relativeToAbsoluteX(float val) const;


	/*!
	\brief
		Convert the given Y co-ordinate from relative to absolute metrics.

	\param val
		Y co-ordinate specified in relative metrics for this Window (so 0 is this windows top edge).

	\return
		An absolute metric value that is equivalent to \a val, given the Window objects current height.
	*/
	float	relativeToAbsoluteY(float val) const;


	/*!
	\brief
		Convert the given position from relative to absolute metrics.

	\param pt
		Point object describing a position specified in relative metrics for this Window (so 0,0 is this windows top-left corner).

	\return
		A Point object describing a position in absolute metric values that is equivalent to \a pt, given the Window objects current size.
	*/
	Point	relativeToAbsolute(const Point& pt) const;


	/*!
	\brief
		Convert the given size from relative to absolute metrics.

	\param sze
		Size object describing a size specified in relative metrics for this Window.

	\return
		A Size object that describes a size in absolute metric values that is equivalent to \a sze, given the Window objects current size.
	*/
	Size	relativeToAbsolute(const Size& sze) const;


	/*!
	\brief
		Convert the given area from relative to absolute metrics.

	\param rect
		Rect object describing the area specified in relative metrics for this Window.

	\return
		A Rect object that describes an area in absolute metric values that is equivalent to \a rect, given the Window objects current size.
	*/
	Rect	relativeToAbsolute(const Rect& rect) const;


	/*!
	\brief
		Convert a window co-ordinate value, specified in whichever metrics mode is active, to a screen relative pixel co-ordinate.

	\param x
		x co-ordinate value to be converted

	\return
		float value describing a pixel screen co-ordinate that is equivalent to window co-ordinate \a x.
	*/
	float	windowToScreenX(float x) const;


	/*!
	\brief
		Convert a window co-ordinate value, specified in whichever metrics mode is active, to a screen relative pixel co-ordinate.

	\param y
		y co-ordinate value to be converted

	\return
		float value describing a screen co-ordinate that is equivalent to window co-ordinate \a y.
	*/
	float	windowToScreenY(float y) const;
	
	
	/*!
	\brief
		Convert a window co-ordinate position, specified in whichever metrics mode is active, to a screen relative pixel co-ordinate position.

	\param pt
		Point object describing the position to be converted

	\return
		Point object describing a screen co-ordinate position that is equivalent to window co-ordinate position \a pt.
	*/
	Point	windowToScreen(const Point& pt) const;


	/*!
	\brief
		Convert a window size value, specified in whichever metrics mode is active, to a size in pixels.

	\param sze
		Size object describing the size to be converted

	\return
		Size object describing describing a size in pixels that is equivalent to the window based size \a sze.
	*/
	Size	windowToScreen(const Size& sze) const;


	/*!
	\brief
		Convert a window area, specified in whichever metrics mode is active, to a screen area.

	\param rect
		Rect object describing the area to be converted

	\return
		Rect object describing a screen area that is equivalent to window area \a rect.
	*/
	Rect	windowToScreen(const Rect& rect) const;


	/*!
	\brief
		Convert a screen relative pixel co-ordinate value to a window co-ordinate value, specified in whichever metrics mode is active.

	\param x
		x co-ordinate value to be converted

	\return
		float value describing a window co-ordinate value that is equivalent to screen co-ordinate \a x.
	*/
	float	screenToWindowX(float x) const;


	/*!
	\brief
		Convert a screen relative pixel co-ordinate value to a window co-ordinate value, specified in whichever metrics mode is active.

	\param y
		y co-ordinate value to be converted

	\return
		float value describing a window co-ordinate value that is equivalent to screen co-ordinate \a y.
	*/
	float	screenToWindowY(float y) const;


	/*!
	\brief
		Convert a screen relative pixel position to a window co-ordinate position, specified in whichever metrics mode is active.

	\param pt
		Point object describing the position to be converted

	\return
		Point object describing a window co-ordinate position that is equivalent to screen co-ordinate \a x.
	*/
	Point	screenToWindow(const Point& pt) const;


	/*!
	\brief
		Convert a pixel screen size to a window based size, specified in whichever metrics mode is active.

	\param sze
		Size object describing the size to be converted

	\return
		Size object describing a window based size that is equivalent to screen based size \a sze.
	*/
	Size	screenToWindow(const Size& sze) const;


	/*!
	\brief
		Convert a screen area to a window area, specified in whichever metrics mode is active.

	\param rect
		Rect object describing the area to be converted

	\return
		Rect object describing a window area that is equivalent to screen area \a rect.
	*/
	Rect	screenToWindow(const Rect& rect) const;


	/*************************************************************************
		Main render function.
	*************************************************************************/
	/*!
	\brief
		Causes the Window object to render itself and all of it's attached children

	\return
		Nothing
	*/
	void	render(void);


protected:
	/*************************************************************************
		System object can trigger events directly
	*************************************************************************/
	friend	void System::injectMouseMove(float delta_x, float delta_y);
	friend	void System::injectMouseButtonDown(MouseButton button);
	friend	void System::injectMouseButtonUp(MouseButton button);
	friend	void System::injectChar(utf32 code_point);
	friend	void System::injectKeyDown(uint key_code);
	friend	void System::injectKeyUp(uint key_code);


	/*************************************************************************
		Event trigger methods
	*************************************************************************/
	/*!
	\brief
		Handler called when the window's size changes.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onSized(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's position changes.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onMoved(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's text is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onTextChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's font is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onFontChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's alpha blend value is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onAlphaChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's client assigned ID is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onIDChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window is shown (made visible).

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onShown(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window is hidden.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onHidden(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window is enabled.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onEnabled(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window is disabled.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onDisabled(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's active metrics system is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onMetricsChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's setting for being clipped by it's parent is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onClippingChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's setting for being destroyed automatically be it's parent is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onParentDestroyChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's setting for inheriting alpha-blending is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onInheritsAlphaChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the window's always-on-top setting is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onAlwaysOnTopChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when this window gains capture of mouse inputs.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onCaptureGained(WindowEventArgs& e);


	/*!
	\brief
		Handler called when this window loses capture of mouse inputs.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onCaptureLost(WindowEventArgs& e);


	/*!
	\brief
		Handler called when rendering for this window has started.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onRenderingStarted(WindowEventArgs& e);


	/*!
	\brief
		Handler called when rendering for this window has ended.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onRenderingEnded(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the z-order position of this window has changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onZChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when this window's destruction sequence has begun.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onDestructionStarted(WindowEventArgs& e);


	/*!
	\brief
		Handler called when this window has become the active window.

	\param e
		WindowEventArgs class whose 'window' pointer field is set to the window that previously was active, or NULL for none.
	*/
	virtual void	onActivated(WindowEventArgs& e);


	/*!
	\brief
		Handler called when this window has lost input focus and has been deactivated.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that has now become active, or NULL for none.
	*/
	virtual void	onDeactivated(WindowEventArgs& e);


	/*!
	\brief
		Handler called when this window's parent window has been resized.  If this window is the root / GUI Sheet
		window, this call will be made when the screen size changes.

	\param e
		WindowEventArgs object whose 'window' pointer field is set the the window that caused the event; this is typically either
		this window's parent window, or NULL to indicate the screen size has changed.
	*/
	virtual void	onParentSized(WindowEventArgs& e);


	/*!
	\brief
		Handler called when a child window is added to this window.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that has been added.
	*/
	virtual void	onChildAdded(WindowEventArgs& e);


	/*!
	\brief
		Handler called when a child window is removed from this window.

	\param e
		WindowEventArgs object whose 'window' pointer field is set the window that has been removed.
	*/
	virtual void	onChildRemoved(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the mouse cursor has entered this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseEnters(MouseEventArgs& e);


	/*!
	\brief
		Handler called when the mouse cursor has left this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseLeaves(MouseEventArgs& e);


	/*!
	\brief
		Handler called when the mouse cursor has been moved within this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseMove(MouseEventArgs& e);


	/*!
	\brief
		Handler called when the mouse wheel (z-axis) position changes within this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseWheel(MouseEventArgs& e);


	/*!
	\brief
		Handler called when a mouse button has been depressed within this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseButtonDown(MouseEventArgs& e);


	/*!
	\brief
		Handler called when a mouse button has been released within this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseButtonUp(MouseEventArgs& e);


	/*!
	\brief
		Handler called when a mouse button has been clicked (that is depressed and then released, within a specified time)
		within this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseClicked(MouseEventArgs& e);


	/*!
	\brief
		Handler called when a mouse button has been double-clicked within this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseDoubleClicked(MouseEventArgs& e);


	/*!
	\brief
		Handler called when a mouse button has been triple-clicked within this window's area.

	\param e
		MouseEventArgs object.  All fields are valid.
	*/
	virtual void	onMouseTripleClicked(MouseEventArgs& e);


	/*!
	\brief
		Handler called when a key as been depressed while this window has input focus.

	\param e
		KeyEventArgs object whose 'scancode' field is set to the Key::Scan value representing the key that was pressed, and whose
		'sysKeys' field represents the combination of SystemKey that were active when the event was generated.
	*/
	virtual void	onKeyDown(KeyEventArgs& e);


	/*!
	\brief
		Handler called when a key as been released while this window has input focus.

	\param e
		KeyEventArgs object whose 'scancode' field is set to the Key::Scan value representing the key that was released, and whose
		'sysKeys' field represents the combination of SystemKey that were active when the event was generated.  All other fields should be
		considered as 'junk'.
	*/
	virtual void	onKeyUp(KeyEventArgs& e);


	/*!
	\brief
		Handler called when a character-key has been pressed while this window has input focus.

	\param e
		KeyEventArgs object whose 'codepoint' field is set to the Unicode code point (encoded as utf32) for the character typed, and whose
		'sysKeys' field represents the combination of SystemKey that were active when the event was generated.  All other fields should be
		considered as 'junk'.
	*/
	virtual void	onCharacter(KeyEventArgs& e);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Perform the actual rendering for this Window.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual	void	drawSelf(float z)	= 0;


	/*!
	\brief
		Set the parent window for this window object.

	\param parent
		Pointer to a Window object that is to be assigned as the parent to this Window.

	\return
		Nothing
	*/
	void	setParent(Window* parent);


	/*!
	\brief
		Return the pixel Width of the parent element.  This always returns a valid number.

	\return
		float value that is equal to the pixel width of this Window objects parent
	*/
	float	getParentWidth(void) const;


	/*!
	\brief
		Return the pixel Height of the parent element.  This always returns a valid number.

	\return
		float value that is equal to the pixel height of this Window objects parent
	*/
	float	getParentHeight(void) const;


	/*!
	\brief
		Return the pixel size of the parent element.  This always returns a valid object.

	\return
		Size object that describes the pixel dimensions of this Window objects parent
	*/
	Size	getParentSize(void) const;


	/*!
	\brief
		Return a Rect object that describes, in values relative to \a window, the absolute area described by \a rect.

	\param window
		Pointer to a window object that is to be used as the base for the conversion.  If this is NULL then the size of the
		display, as returned by the renderer object, is used.

	\param rect
		Rect object describing the area, in absolute values, that is to be returned as relative values.

	\return
		Rect object that describes in values relative to \a window, the same area described as absolute values in \a rect.
	*/
	Rect	absoluteToRelative_impl(const Window* window, const Rect& rect) const;
	Size	absoluteToRelative_impl(const Window* window, const Size& sz) const;
	Point	absoluteToRelative_impl(const Window* window, const Point& pt) const;
	float	absoluteToRelativeX_impl(const Window* window, float x) const;
	float	absoluteToRelativeY_impl(const Window* window, float y) const;


	/*!
	\brief
		Return a Rect object that describes, in absolute values offset from \a window, the relative area described by \a rect.

	\param window
		Pointer to a window object that is to be used as the base for the conversion.  If this is NULL then the size of the
		display, as returned by the renderer object, is used.

	\param rect
		Rect object describing the area, in relative values, that is to be returned as absolute values.

	\return
		Rect object that describes in absolute values offset from \a window, the same area described as relative values in \a rect.
	*/
	Rect	relativeToAbsolute_impl(const Window* window, const Rect& rect) const;
	Size	relativeToAbsolute_impl(const Window* window, const Size& sz) const;
	Point	relativeToAbsolute_impl(const Window* window, const Point& pt) const;
	float	relativeToAbsoluteX_impl(const Window* window, float x) const;
	float	relativeToAbsoluteY_impl(const Window* window, float y) const;

	Size	getWindowSize_impl(const Window* window) const;


	/*!
	\brief
		Return the inherited metrics mode.  This is either the metrics mode of our parent, or Relative
		if we have no parent.
	*/
	MetricsMode getInheritedMetricsMode(void) const;


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// child stuff
	typedef	std::vector<Window*>	ChildList;
	ChildList		d_children;			//!< The list of child Window objects attached to this.

	// general data
	MetricsMode		d_metricsMode;		//!< Holds the active metrics mode for this window
	static Window*	d_captureWindow;	//!< Window that has captured inputs
	Window*			d_oldCapture;		//!< The Window that previously had capture (used for restoreOldCapture mode)
	Window*			d_parent;			//!< Holds pointer to the parent window.
	const Font*		d_font;				//!< Holds pointer to the Window objects current Font.
	String			d_text;				//!< Holds the text / label / caption for this Window.
	uint			d_ID;				//!< User ID assigned to this Window
	float			d_alpha;			//!< Alpha transparency setting for the Window
	Rect			d_abs_area;			//!< This Window objects area (pixels relative to parent)
	Rect			d_rel_area;			//!< This Window objects area (decimal fractions relative to parent)
	const Image*	d_mouseCursor;		//!< Holds pointer to the Window objects current mouse cursor image.
	void*			d_userData;			//!< Holds pointer to some user assigned data.

	// maximum and minimum sizes
	Size	d_minSize;					//!< current minimum size for the window (this is always stored in pixels).
	Size	d_maxSize;					//!< current maximum size for the window (this is always stored in pixels).

	// settings
	bool	d_enabled;					//!< true when Window is enabled
	bool	d_visible;					//!< true when Window is visible (that is it will be rendered, but may be obscured so no necesarily really visible)
	bool	d_active;					//!< true when Window is the active Window (receiving inputs).
	bool	d_clippedByParent;			//!< true when Window will be clipped by parent Window area Rect.
	bool	d_destroyedByParent;		//!< true when Window will be auto-destroyed by parent.
	bool	d_alwaysOnTop;				//!< true if Window will be drawn on top of all other Windows
	bool	d_inheritsAlpha;			//!< true if the Window inherits alpha from the parent Window
	bool	d_restoreOldCapture;		//!< true if the Window restores capture to the previous window when it releases capture.

private:
	/*************************************************************************
		Properties for Window base class
	*************************************************************************/
	static	WindowProperties::AbsoluteHeight	d_absHeightProperty;
	static	WindowProperties::AbsoluteMaxSize	d_absMaxSizeProperty;
	static	WindowProperties::AbsoluteMinSize	d_absMinSizeProperty;
	static	WindowProperties::AbsolutePosition	d_absPositionProperty;
	static	WindowProperties::AbsoluteRect		d_absRectProperty;
	static	WindowProperties::AbsoluteSize		d_absSizeProperty;
	static	WindowProperties::AbsoluteWidth		d_absWidthProperty;
	static	WindowProperties::AbsoluteXPosition	d_absXPosProperty;
	static	WindowProperties::AbsoluteYPosition	d_absYPosProperty;
	static	WindowProperties::Alpha				d_alphaProperty;
	static	WindowProperties::AlwaysOnTop		d_alwaysOnTopProperty;
	static	WindowProperties::ClippedByParent	d_clippedByParentProperty;
	static	WindowProperties::DestroyedByParent	d_destroyedByParentProperty;
	static	WindowProperties::Disabled			d_disabledProperty;
	static	WindowProperties::Font				d_fontProperty;
	static	WindowProperties::Height			d_heightProperty;
	static	WindowProperties::ID				d_IDProperty;
	static	WindowProperties::InheritsAlpha		d_inheritsAlphaProperty;
	static	WindowProperties::MetricsMode		d_metricsModeProperty;
	static	WindowProperties::MouseCursorImage	d_mouseCursorProperty;
	static	WindowProperties::Position			d_positionProperty;
	static	WindowProperties::Rect				d_rectProperty;
	static	WindowProperties::RelativeHeight	d_relHeightProperty;
	static	WindowProperties::RelativeMaxSize	d_relMaxSizeProperty;
	static	WindowProperties::RelativeMinSize	d_relMinSizeProperty;
	static	WindowProperties::RelativePosition	d_relPositionProperty;
	static	WindowProperties::RelativeRect		d_relRectProperty;
	static	WindowProperties::RelativeSize		d_relSizeProperty;
	static	WindowProperties::RelativeWidth		d_relWidthProperty;
	static	WindowProperties::RelativeXPosition	d_relXPosProperty;
	static	WindowProperties::RelativeYPosition	d_relYPosProperty;
	static	WindowProperties::RestoreOldCapture	d_restoreOldCaptureProperty;
	static	WindowProperties::Size				d_sizeProperty;
	static	WindowProperties::Text				d_textProperty;
	static	WindowProperties::Visible			d_visibleProperty;
	static	WindowProperties::Width				d_widthProperty;
	static	WindowProperties::XPosition			d_xPosProperty;
	static	WindowProperties::YPosition			d_yPosProperty;


	/*************************************************************************
		Private implementation functions
	*************************************************************************/
	/*!
	\brief
		Add standard CEGUI::Window events
	*/
	void	addStandardEvents(void);

	/*!
	\brief
		Cleanup child windows
	*/
	void	cleanupChildren(void);

	/*!
	\brief
		Add given window to child list at an appropriate position
	*/
	void	addChild_impl(Window* wnd);

	/*!
	\brief
		Remove given window from child list
	*/
	void	removeChild_impl(Window* wnd);

	/*!
	\brief
		Notify 'this' and all siblings of a ZOrder change event
	*/
	void	onZChange_impl(void);


	/*!
	\brief
		Add standard CEGUI::Window properties.
	*/
	void	addStandardProperties(void);

	
	/*************************************************************************
		May not copy or assign Window objects
	*************************************************************************/
	Window(const Window& wnd) {}
	Window& operator=(const Window& wnd) {return *this;}

	/*************************************************************************
		Private implementation Data
	*************************************************************************/
	const String	d_type;			//!< String holding the type name for the Window (is also the name of the WindowFactory that created us)
	const String	d_name;			//!< The name of the window (GUI system unique).
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIWindow_h_
