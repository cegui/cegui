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
#include "CEGUISize.h"
#include "CEGUIEventSet.h"
#include "CEGUIPropertySet.h"
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
enum MetricsMode
{
	Relative,		//!< Metrics are specified as a decimal fraction of parent Window size.
	Absolute,		//!< Metrics are specified as whole pixels.
};


/*!
\brief
	System key flag values
*/
enum SystemKey
{
	LeftMouse		= 0x01,			//!< The left mouse button.
	RightMouse		= 0x02,			//!< The right mouse button.
	Shift			= 0x04,			//!< Either shift key.
	Control			= 0x08,			//!< Either control key.
	MiddleMouse		= 0x10,			//!< The middle mouse button.
	X1Mouse			= 0x20,			//!< The first 'extra' mouse button
	X2Mouse			= 0x40,			//!< The second 'extra' mouse button.
};


/*!
\brief
	An abstract base class providing common functionality and specifying the required interface for derived classes.

	The Window base class is the only UI object class that the core of the system knows about, for this reason every
	other window, widget, or similar item within the system must be derived from Window.  The base class provides the
	common functionality required by all UI objects, and specifies the minimal interface required to be implemented by
	derived classes.
*/
class CEGUIBASE_API Window : public EventSet, public PropertySet
{
public:
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

	bool	isDestroyedByParent(void) const;

	bool	isAlwaysOnTop(void) const;
	bool	isDisabled(void) const;
	bool	isVisible(void) const;
	bool	isActive(void) const;
	bool	isClippedByParent(void) const;

	uint	getID(void) const;
	uint	getChildCount(void) const;

	bool	isChild(const String& name) const;
	bool	isChild(uint ID) const;
	bool	isChild(const Window* window) const;

	Window*	getChild(const String& name) const;
	Window* getChild(uint ID) const;
	Window*	getActiveChild(void) const;

	bool	isAncestor(const String& name) const;
	bool	isAncestor(uint ID) const;
	bool	isAncestor(const Window* window) const;

	const Font*		getFont(void) const;
	const String&	getText(void) const;

	bool	inheritsAlpha(void) const;
	float	getAlpha(void) const;
	float	getEffectiveAlpha(void) const;

	Rect	getRect(void) const;

	Rect	getPixelRect(void) const;
	Rect	getUnclippedPixelRect(void) const;

	static	Window*	getCaptureWindow(void);
	bool	isCapturedByThis(void) const;
	bool	isCapturedByAncestor(void) const;
	bool	isCapturedByChild(void) const;

	bool	isHit(const Point& position) const;
	Window*	getChildAtPosition(const Point& position) const;

	MetricsMode	getMetricsMode(void) const;


	/*!
	\brief
		return the x position of the window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		float value that specifies the x position of the Window relative to it's parent, depending on the metrics system in use for this
		Window, this value will specify either pixels or a decimal fraction of the width of the parent Window.
	*/
	float	getXPos(void) const;


	/*!
	\brief
		return the y position of the window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		float value that specifies the y position of the Window relative to it's parent, depending on the metrics system in use for this
		Window, this value will specify either pixels or a decimal fraction of the height of the parent Window.
	*/
	float	getYPos(void) const;


	/*!
	\brief
		return the position of the window.  Interpretation of return value depends upon the metric type in use by this window.

	\return
		Point object that describes the position of the Window relative to it's parent, depending on the metrics system in use for this
		Window, the values in the Point will specify either pixels or decimal fractions of the total width and height of the parent.
	*/
	Point	getPos(void) const;


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


	/*************************************************************************
		Manipulator functions
	*************************************************************************/
	virtual void	initialise(void);

	void	setDestroyedByParent(bool setting);

	void	setAlwaysOnTop(bool setting);

	void	setEnabled(bool setting);
	void	enable(void);
	void	disable(void);

	void	setVisible(bool setting);
	void	show(void);
	void	hide(void);

	void	activate(void);

	void	setClippedByParent(bool setting);
	
	void	setID(void);

	void	setText(void);

	void	setWidth(float width);
	void	setHeight(float height);
	void	setSize(const Size& size);

	void	setXPos(float x);
	void	setYPos(float y);
	void	setPosition(const Point& position);

	void	setAreaRect(const Rect& area);

	void	setText(const String& text);
	void	setFont(const Font* font);

	void	addChildWindow(const String& name);
	void	addChildWindow(Window* window);

	void	removeChildWindow(const String& name);
	void	removeChildWindow(Window* window);
	void	removeChildWindow(uint ID);

	void	moveToFront();
	void	moveToBack();

	void	captureInput(void);
	void	releaseInput(void);
	void	setRestoreCaptue(bool setting);

	void	setAlpha(float alpha);
	void	setInheritsAlpha(bool setting);


	/*************************************************************************
		Co-ordinate and Size Conversion Functions
	*************************************************************************/
	float	absoluteToRelativeX(float val) const;
	float	absoluteToRelativeY(float val) const;
	Point	absoluteToRelative(const Point& pt) const;
	Size	absoluteToRelative(const Size& pt) const;

	float	relativeToAbsoluteX(float val) const;
	float	relativeToAbsoluteY(float val) const;
	Point	relativeToAbsolute(const Point& pt) const;
	Size	relativeToAbsolute(const Size& pt) const;

	float	windowToScreenX(float x) const;
	float	windowToScreenY(float y) const;
	Point	windowToScreen(const Point& pt) const;
	Size	windowToScreen(const Size& sze) const;

	float	screenToWindowX(float x) const;
	float	screenToWindowY(float y) const;
	Point	screenToWindow(const Point& pt) const;
	Size	screenToWindow(const Size& sze) const;

	/*************************************************************************
		Main render function.
	*************************************************************************/
	virtual void	render(void);

protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	virtual	void	drawSelf(void);

	void	setParent(Window* parent);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// child stuff
	typedef	std::vector<Window*>	ChildList;
	ChildList		d_children;			//!< The list of child Window objects attached to this.

	// general data
	static Window*	d_captureWindow;	//!< Window that has captured inputs
	Window*			d_oldCapture;		//!< The Window that previously had capture (used for restoreOldCapture mode)
	Window*			d_parent;			//!< Holds pointer to the parent window.
	const Font*		d_font;				//!< Holds pointer to the Window objects current Font.
	String			d_text;				//!< Holds the text / label / caption for this Window.
	Rect			d_area;				//!< This Window objects area
	uint			d_ID;				//!< User ID assigned to this Window
	float			d_alpha;			//!< Alpha transparency setting for the Window

	// settings
	bool	d_enabled;					//!< true when Window is enabled
	bool	d_visible;					//!< true when Window is visible (that is it will be rendered, but may be obscured so no necesarily really visible)
	bool	d_active;					//!< true when Window is the active Window (receiving inputs).
	bool	d_clippedByParent;			//!< true when Window will be clipped by parent Window area Rect.
	bool	d_alwaysOnTop;				//!< true if Window will be drawn on top of all other Windows
	bool	d_inheritsAlpha;			//!< true if the Window inherits alpha from the parent Window
	bool	d_restoreOldCapture;		//!< true if the Window restores capture to the previous window when it releases capture.

private:
	/*************************************************************************
		May not copy or assign Window objects
	*************************************************************************/
	Window(const Window& wnd) {}
	Window& operator=(const Window& wnd) {}

	/*************************************************************************
		Private implementation Data
	*************************************************************************/
	String	d_type;			//!< String holding the type name for the Window (is also the name of the WindowFactory that created us)
	String	d_name;			//!< The name of the window (GUI system unique).
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIWindow_h_
