/************************************************************************
	filename: 	CEGUIWindow.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the Window base class
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
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIMouseCursor.h"
#include <algorithm>
#include <cmath>
#include <stdio.h>

// Start of CEGUI namespace section
namespace CEGUI
{
const String Window::EventNamespace("Window");

/*************************************************************************
	Definitions for Window base class Properties
*************************************************************************/
WindowProperties::AbsoluteHeight	Window::d_absHeightProperty;
WindowProperties::AbsoluteMaxSize	Window::d_absMaxSizeProperty;
WindowProperties::AbsoluteMinSize	Window::d_absMinSizeProperty;
WindowProperties::AbsolutePosition	Window::d_absPositionProperty;
WindowProperties::AbsoluteRect		Window::d_absRectProperty;
WindowProperties::AbsoluteSize		Window::d_absSizeProperty;
WindowProperties::AbsoluteWidth		Window::d_absWidthProperty;
WindowProperties::AbsoluteXPosition	Window::d_absXPosProperty;
WindowProperties::AbsoluteYPosition	Window::d_absYPosProperty;
WindowProperties::Alpha				Window::d_alphaProperty;
WindowProperties::AlwaysOnTop		Window::d_alwaysOnTopProperty;
WindowProperties::ClippedByParent	Window::d_clippedByParentProperty;
WindowProperties::DestroyedByParent	Window::d_destroyedByParentProperty;
WindowProperties::Disabled			Window::d_disabledProperty;
WindowProperties::Font				Window::d_fontProperty;
WindowProperties::Height			Window::d_heightProperty;
WindowProperties::ID				Window::d_IDProperty;
WindowProperties::InheritsAlpha		Window::d_inheritsAlphaProperty;
WindowProperties::MetricsMode		Window::d_metricsModeProperty;
WindowProperties::MouseCursorImage	Window::d_mouseCursorProperty;
WindowProperties::Position			Window::d_positionProperty;
WindowProperties::Rect				Window::d_rectProperty;
WindowProperties::RelativeHeight	Window::d_relHeightProperty;
WindowProperties::RelativeMaxSize	Window::d_relMaxSizeProperty;
WindowProperties::RelativeMinSize	Window::d_relMinSizeProperty;
WindowProperties::RelativePosition	Window::d_relPositionProperty;
WindowProperties::RelativeRect		Window::d_relRectProperty;
WindowProperties::RelativeSize		Window::d_relSizeProperty;
WindowProperties::RelativeWidth		Window::d_relWidthProperty;
WindowProperties::RelativeXPosition	Window::d_relXPosProperty;
WindowProperties::RelativeYPosition	Window::d_relYPosProperty;
WindowProperties::RestoreOldCapture	Window::d_restoreOldCaptureProperty;
WindowProperties::Size				Window::d_sizeProperty;
WindowProperties::Text				Window::d_textProperty;
WindowProperties::Visible			Window::d_visibleProperty;
WindowProperties::Width				Window::d_widthProperty;
WindowProperties::XPosition			Window::d_xPosProperty;
WindowProperties::YPosition			Window::d_yPosProperty;
WindowProperties::ZOrderChangeEnabled	Window::d_zOrderChangeProperty;


/*************************************************************************
	static data definitions
*************************************************************************/
Window*	Window::d_captureWindow		= NULL;


/*************************************************************************
	Event name constants
*************************************************************************/
const String Window::EventParentSized( (utf8*)"ParentSized" );
const String Window::EventSized( (utf8*)"Sized" );
const String Window::EventMoved( (utf8*)"Moved" );
const String Window::EventTextChanged( (utf8*)"TextChanged" );
const String Window::EventFontChanged( (utf8*)"FontChanged" );
const String Window::EventAlphaChanged( (utf8*)"AlphaChanged" );
const String Window::EventIDChanged( (utf8*)"IDChanged" );
const String Window::EventActivated( (utf8*)"Activated" );
const String Window::EventDeactivated( (utf8*)"Deactivated" );
const String Window::EventShown( (utf8*)"Shown" );
const String Window::EventHidden( (utf8*)"Hidden" );
const String Window::EventEnabled( (utf8*)"Enabled" );
const String Window::EventDisabled( (utf8*)"Disabled" );
const String Window::EventMetricsModeChanged( (utf8*)"MetricsChanged" );
const String Window::EventClippedByParentChanged( (utf8*)"ClippingChanged" );
const String Window::EventDestroyedByParentChanged( (utf8*)"DestroyedByParentChanged" );
const String Window::EventInheritsAlphaChanged( (utf8*)"InheritAlphaChanged" );
const String Window::EventAlwaysOnTopChanged( (utf8*)"AlwaysOnTopChanged" );
const String Window::EventInputCaptureGained( (utf8*)"CaptureGained" );
const String Window::EventInputCaptureLost( (utf8*)"CaptureLost" );
const String Window::EventRenderingStarted( (utf8*)"StartRender" );
const String Window::EventRenderingEnded( (utf8*)"EndRender" );
const String Window::EventChildAdded( (utf8*)"AddedChild" );
const String Window::EventChildRemoved( (utf8*)"RemovedChild" );
const String Window::EventDestructionStarted( (utf8*)"DestructStart" );
const String Window::EventZOrderChanged( (utf8*)"ZChanged" );
const String Window::EventMouseEnters( (utf8*)"MouseEnter" );
const String Window::EventMouseLeaves( (utf8*)"MouseLeave" );
const String Window::EventMouseMove( (utf8*)"MouseMove" );
const String Window::EventMouseWheel( (utf8*)"MouseWheel" );
const String Window::EventMouseButtonDown( (utf8*)"MouseButtonDown" );
const String Window::EventMouseButtonUp( (utf8*)"MouseButtonUp" );
const String Window::EventMouseClick( (utf8*)"MouseClick" );
const String Window::EventMouseDoubleClick( (utf8*)"MouseDoubleClick" );
const String Window::EventMouseTripleClick( (utf8*)"MouseTripleClick" );
const String Window::EventKeyDown( (utf8*)"KeyDown" );
const String Window::EventKeyUp( (utf8*)"KeyUp" );
const String Window::EventCharacterKey( (utf8*)"CharacterKey" );
	
	
/*************************************************************************
	Constructor
*************************************************************************/
Window::Window(const String& type, const String& name) :
	d_type(type),
	d_name(name)
{
	// basic set-up
	d_metricsMode	= Relative;
	d_parent		= NULL;
	d_font			= NULL;
	d_ID			= 0;
	d_alpha			= 1.0f;
	d_mouseCursor	= (const Image*)DefaultMouseCursor;
	d_userData		= NULL;

	// basic settings
	d_enabled			= true;
	d_visible			= true;
	d_active			= false;
	d_clippedByParent	= true;
	d_destroyedByParent	= true;
	d_alwaysOnTop		= false;
	d_inheritsAlpha		= true;
	d_restoreOldCapture	= false;
	d_zOrderingEnabled	= true;

	// position and size
	d_abs_area = Rect(0, 0, 0, 0);
	d_rel_area = Rect(0, 0, 0, 0);

	// add events
	addStandardEvents();

	// set initial min/max sizes.  These should normally be re-set in derived classes to something appropriate.
	setMinimumSize(Size(0.0f, 0.0f));
	setMaximumSize(Size(1.0f, 1.0f));

	// add properties
	addStandardProperties();
}

/*************************************************************************
	Destructor
*************************************************************************/
Window::~Window(void)
{
	releaseInput();

	// signal our imminent destruction
	WindowEventArgs args(this);
	onDestructionStarted(args);

	// double check we are detached from parent
	if (d_parent != NULL)
	{
		d_parent->removeChildWindow(this);
	}

	cleanupChildren();
}


/*************************************************************************
	return true if the Window is currently disabled	
*************************************************************************/
bool Window::isDisabled(void) const
{
	bool parDisabled = (d_parent == NULL) ? false : d_parent->isDisabled();

	return (!d_enabled) || parDisabled;
}


/*************************************************************************
	return true if the Window is currently visible.
*************************************************************************/
bool Window::isVisible(void) const
{
	bool parVisible = (d_parent == NULL) ? true : d_parent->isVisible();

	return d_visible && parVisible;
}


/*************************************************************************
	return true if this is the active Window
	(the window that receives inputs)	
*************************************************************************/
bool Window::isActive(void) const
{
	bool parActive = (d_parent == NULL) ? true : d_parent->isActive();

	return d_active && parActive;
}


/*************************************************************************
	returns whether a Window with the specified name is currently
	attached to this Window as a child.
*************************************************************************/
bool Window::isChild(const String& name) const
{
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getName() == name)
		{
			return true;
		}

	}

	return false;
}

/*************************************************************************
	returns whether at least one window with the given ID code is
	attached as a child.
*************************************************************************/
bool Window::isChild(uint ID) const
{
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getID() == ID)
		{
			return true;
		}

	}

	return false;
}


/*************************************************************************
	return true if the given Window is a child of this window.
*************************************************************************/
bool Window::isChild(const Window* window) const
{
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i] == window)
		{
			return true;
		}

	}

	return false;
}


/*************************************************************************
	return a pointer to the child window with the specified name.
*************************************************************************/
Window* Window::getChild(const String& name) const
{
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getName() == name)
		{
			return d_children[i];
		}

	}

	throw UnknownObjectException((utf8*)"Window::getChild - The Window object named '" + name +"' is not attached to Window '" + d_name + "'.");
}


/*************************************************************************
	return a pointer to the first attached child window with the
	specified ID.
*************************************************************************/
Window* Window::getChild(uint ID) const
{
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getID() == ID)
		{
			return d_children[i];
		}

	}

	// TODO: Update exception to include ID code
	char strbuf[16];
	sprintf(strbuf, "%X", ID);
	throw UnknownObjectException("Window::getChild - The Window with ID: '" + std::string(strbuf) + "' is not attached to Window '" + d_name + "'.");
}


/*************************************************************************
	return a pointer to the Window that currently has input focus
	starting with this Window.
*************************************************************************/
Window* Window::getActiveChild(void)
{
	return const_cast<Window*>(static_cast<const Window*>(this)->getActiveChild());
}


/*************************************************************************
	return a pointer to the Window that currently has input focus
	starting with this Window.
*************************************************************************/
const Window* Window::getActiveChild(void) const
{
	// are children can't be active if we are not
	if (!isActive())
	{
		return NULL;
	}

	uint pos = getChildCount();

	while (pos-- > 0)
	{
		// don't need full backward scan for activeness as we already know 'this' is active
		if (d_children[pos]->d_active)
		{
			return d_children[pos]->getActiveChild();
		}

	}

	// no child was active, therefore we are the topmost active window
	return this;
}


/*************************************************************************
	return true if the specified Window is some ancestor of this Window
*************************************************************************/
bool Window::isAncestor(const String& name) const
{
	// if we have no ancestor then 'name' can't be ancestor
	if (d_parent == NULL)
	{
		return false;
	}

	// check our immediate parent
	if (d_parent->getName() == name)
	{
		return true;
	}

	// not out parent, check back up the family line
	return d_parent->isAncestor(name);
}


/*************************************************************************
	return true if any Window with the given ID is some ancestor of
	this Window.	
*************************************************************************/
bool Window::isAncestor(uint ID) const
{
	// return false if we have no ancestor
	if (d_parent == NULL)
	{
		return false;
	}

	// check our immediate parent
	if (d_parent->getID() == ID)
	{
		return true;
	}

	// not our parent, check back up the family line
	return d_parent->isAncestor(ID);
}


/*************************************************************************
	return true if the specified Window is some ancestor of this Window.	
*************************************************************************/
bool Window::isAncestor(const Window* window) const
{
	// if we have no parent, then return false
	if (d_parent == NULL)
	{
		return false;
	}

	// check our immediate parent
	if (d_parent == window)
	{
		return true;
	}

	// not our parent, check back up the family line
	return d_parent->isAncestor(window);
}


/*************************************************************************
	return the Font object active for the Window.
*************************************************************************/
const Font* Window::getFont(void) const
{
	if (d_font == NULL)
	{
		return System::getSingleton().getDefaultFont();
	}

	return d_font;
}


/*************************************************************************
	return the effective alpha value that will be used when rendering
	this window, taking into account inheritance of parent window(s)
	alpha.
*************************************************************************/
float Window::getEffectiveAlpha(void) const
{
	if ((d_parent == NULL) || (!inheritsAlpha()))
	{
		return d_alpha;
	}

	return d_alpha * d_parent->getEffectiveAlpha();
}


/*************************************************************************
	return a Rect object that describes the Window area.	
*************************************************************************/
Rect Window::getRect(void) const
{
	if (getMetricsMode() == Relative)
	{
		return d_rel_area;
	}
	else
	{
		return d_abs_area;
	}

}


/*************************************************************************
	return a Rect object describing the Window area in screen space.
*************************************************************************/
Rect Window::getPixelRect(void) const
{
	// clip to parent?
	if (isClippedByParent() && (d_parent != NULL))
	{
		return getUnclippedPixelRect().getIntersection(d_parent->getInnerRect());
	}
	// else, clip to screen
	else
	{
		return getUnclippedPixelRect().getIntersection(System::getSingleton().getRenderer()->getRect());
	}

}


/*************************************************************************
	return a Rect object describing the clipping area for this window.
*************************************************************************/
Rect Window::getInnerRect(void) const
{
	// clip to parent?
	if (isClippedByParent() && (d_parent != NULL))
	{
		return getUnclippedInnerRect().getIntersection(d_parent->getInnerRect());
	}
	// else, clip to screen
	else
	{
		return getUnclippedInnerRect().getIntersection(System::getSingleton().getRenderer()->getRect());
	}

}


/*************************************************************************
	return a Rect object describing the Window area unclipped, in
	screen space.	
*************************************************************************/
Rect Window::getUnclippedPixelRect(void) const
{
	if (getMetricsMode() == Relative)
	{
		return windowToScreen(Rect(0, 0, 1, 1));
	}
	else
	{
		return windowToScreen(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
	}
}


/*************************************************************************
	Return a Rect object that describes, unclipped, the inner rectangle
	for this window.  The inner rectangle is typically an area that
	excludes some frame or other rendering that should not be touched by
	subsequent rendering.
*************************************************************************/
Rect Window::getUnclippedInnerRect(void) const
{
	return getUnclippedPixelRect();
}


/*************************************************************************
	check if the given position would hit this window.	
*************************************************************************/
bool Window::isHit(const Point& position) const
{
	// cannot be hit if we are disabled.
	if (isDisabled())
	{
		return false;
	}

	Rect clipped_area(getPixelRect());

	if (clipped_area.getWidth() == 0)
	{
		return false;
	}

	return clipped_area.isPointInRect(position);
}

/*************************************************************************
	return the child Window that is 'hit' by the given position
*************************************************************************/
Window* Window::getChildAtPosition(const Point& position) const
{
	ChildList::const_reverse_iterator	child, end;

	end = d_children.rend();

	for (child = d_children.rbegin(); child != end; ++child)
	{
		if ((*child)->isVisible())
		{
			// recursively scan children of this child windows...
			Window* wnd = (*child)->getChildAtPosition(position);

			// return window pointer if we found a 'hit' down the chain somewhere
			if (wnd != NULL)
			{
				return wnd;
			}
			// none of our childs children were hit, 
			else
			{
				// see if this child is hit and return it's pointer if it is
				if ((*child)->isHit(position))
				{
					return (*child);
				}

			}

		}

	}

	// nothing hit
	return NULL;
}


/*************************************************************************
	return the current metrics mode employed by the Window
*************************************************************************/
MetricsMode Window::getMetricsMode(void) const
{
	if (d_metricsMode == Inherited)
	{
		return getInheritedMetricsMode();
	}

	return d_metricsMode;
}


/*************************************************************************
	return the x position of the window.  Interpretation of return value
	depends upon the metric type in use by this window.
*************************************************************************/
float Window::getXPosition(void) const
{
	if (getMetricsMode() == Relative)
	{
		return d_rel_area.d_left;
	}

	return d_abs_area.d_left;
}


/*************************************************************************
	return the y position of the window.  Interpretation of return value
	depends upon the metric type in use by this window.
*************************************************************************/
float Window::getYPosition(void) const
{
	if (getMetricsMode() == Relative)
	{
		return d_rel_area.d_top;
	}

	return d_abs_area.d_top;
}


/*************************************************************************
	return the position of the window.  Interpretation of return value
	depends upon the metric type in use by this window.
*************************************************************************/
Point Window::getPosition(void) const
{
	if (getMetricsMode() == Relative)
	{
		return d_rel_area.getPosition();
	}

	return d_abs_area.getPosition();
}


/*************************************************************************
	return the width of the Window.  Interpretation of return value
	depends upon the metric type in use by this window.
*************************************************************************/
float Window::getWidth(void) const
{
	if (getMetricsMode() == Relative)
	{
		return d_rel_area.getWidth();
	}

	return d_abs_area.getWidth();
}


/*************************************************************************
	return the height of the Window.  Interpretation of return value
	depends upon the metric type in use by this window.
*************************************************************************/
float Window::getHeight(void) const
{
	if (getMetricsMode() == Relative)
	{
		return d_rel_area.getHeight();
	}

	return d_abs_area.getHeight();
}


/*************************************************************************
	return the size of the Window.  Interpretation of return value
	depends upon the metric type in use by this window.
*************************************************************************/
Size Window::getSize(void) const
{
	if (getMetricsMode() == Relative)
	{
		return d_rel_area.getSize();
	}

	return d_abs_area.getSize();
}


/*************************************************************************
	true to have the Window appear on top of all other non always on top
	windows, or false to allow the window to be covered by other windows.
*************************************************************************/
void Window::setAlwaysOnTop(bool setting)
{
	// only react to an actual change
	if (isAlwaysOnTop() != setting)
	{
		d_alwaysOnTop = setting;

		// move us in front of sibling windows with the same 'always-on-top' setting as we have.
		if (d_parent != NULL)
		{
			Window* org_parent = d_parent;

			org_parent->removeChild_impl(this);
			org_parent->addChild_impl(this);

			onZChange_impl();
		}

		WindowEventArgs args(this);
		onAlwaysOnTopChanged(args);
	}

}


/*************************************************************************
	Set whether this window is enabled or disabled.  A disabled window
	normally can not be interacted with, and may have different rendering.
*************************************************************************/
void Window::setEnabled(bool setting)
{
	// only react if setting has changed
	if (d_enabled != setting)
	{
		d_enabled = setting;
        WindowEventArgs args(this);
		d_enabled ? onEnabled(args) : onDisabled(args);
	}

}


/*************************************************************************
	Set whether the Window is visible or hidden.
*************************************************************************/
void Window::setVisible(bool setting)
{
	// only react if setting has changed
	if (d_visible != setting)
	{
		d_visible = setting;
        WindowEventArgs args(this);
		d_visible ? onShown(args) : onHidden(args);
	}

}


/*************************************************************************
	Activate the Window giving it input focus and bringing it to the top
	of all non always-on-top Windows.
*************************************************************************/
void Window::activate(void)
{
	// force complete release of input capture.
	// NB: This is not done via releaseCapture() because that has
	// different behaviour depending on the restoreOldCapture setting.
	if ((d_captureWindow != NULL) && (d_captureWindow != this))
	{
		Window* tmpCapture = d_captureWindow;
		d_captureWindow = NULL;

		WindowEventArgs args(NULL);
		tmpCapture->onCaptureLost(args);
	}

	moveToFront();
}


/*************************************************************************
	Deactivate the window.  No further inputs will be received by the
	window until it is re-activated either programmatically or by the
	user interacting with the gui.
*************************************************************************/
void Window::deactivate(void)
{
	ActivationEventArgs args(this);
	args.otherWindow = NULL;
	onDeactivated(args);
}


/*************************************************************************
	Set whether this Window will be clipped by its parent window(s).
*************************************************************************/
void Window::setClippedByParent(bool setting)
{
	// only react if setting has changed
	if (d_clippedByParent != setting)
	{
		d_clippedByParent = setting;
        WindowEventArgs args(this);
		onClippingChanged(args);
	}

}


/*************************************************************************
	Set the current text string for the Window.
*************************************************************************/
void Window::setText(const String& text)
{
	d_text = text;
    WindowEventArgs args(this);
	onTextChanged(args);
}


/*************************************************************************
	Set the current width of the Window.  Interpretation of the input
	value is dependant upon the current metrics system set for the Window.
*************************************************************************/
void Window::setWidth(float width)
{
	setSize(Size(width, getHeight()));
}


/*************************************************************************
	Set the current height of the Window.  Interpretation of the input
	value is dependant upon the current metrics system set for the Window.
*************************************************************************/
void Window::setHeight(float height)
{
	setSize(Size(getWidth(), height));
}


/*************************************************************************
	Set the current size of the Window.  Interpretation of the input value
	is dependant upon the current metrics system set for the Window.
*************************************************************************/
void Window::setSize(const Size& size)
{
	setSize(getMetricsMode(), size);
}


/*************************************************************************
	Set the current 'x' position of the Window.  Interpretation of the
	input value is dependant upon the current metrics system set for the
	Window.
*************************************************************************/
void Window::setXPosition(float x)
{
	setPosition(Point(x, getYPosition()));
}


/*************************************************************************
	Set the current 'y' position of the Window.  Interpretation of the
	input value is dependant upon the current metrics system set for the
	Window.
*************************************************************************/
void Window::setYPosition(float y)
{
	setPosition(Point(getXPosition(), y));
}


/*************************************************************************
	Set the current position of the Window.  Interpretation of the input
	value is dependant upon the current metrics system set for the Window.
*************************************************************************/
void Window::setPosition(const Point& position)
{
	setPosition(getMetricsMode(), position);
}


/*************************************************************************
	Set the current area for the Window, this allows for setting of
	position and size at the same time.  Interpretation of the input
	value is dependant upon the current metrics system set for the Window.	
*************************************************************************/
void Window::setAreaRect(const Rect& area)
{
	setRect(getMetricsMode(), area);
}


/*************************************************************************
	Set the font used by this Window.
*************************************************************************/
void Window::setFont(const Font* font)
{
	d_font = font;
    WindowEventArgs args(this);
	onFontChanged(args);
}


/*************************************************************************
	Set the font used by this Window.
*************************************************************************/
void Window::setFont(const String& name)
{
	if (name.empty())
	{
		setFont(NULL);
	}
	else
	{
		setFont(FontManager::getSingleton().getFont(name));
	}
	
}


/*************************************************************************
	Add the named Window as a child of this Window.  If the Window is
	already attached to a Window, it is detached before being added to
	this Window.	
*************************************************************************/
void Window::addChildWindow(const String& name)
{
	addChildWindow(WindowManager::getSingleton().getWindow(name));
}


/*************************************************************************
	Add the specified Window as a child of this Window.  If the Window
	is already attached to a Window, it is detached before being added
	to this Window.	
*************************************************************************/
void Window::addChildWindow(Window* window)
{
	addChild_impl(window);
    WindowEventArgs args(window);
	onChildAdded(args);
	window->onZChange_impl();
}


/*************************************************************************
	Remove the named Window from this windows child list.
*************************************************************************/
void Window::removeChildWindow(const String& name)
{
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getName() == name)
		{
			removeChildWindow(d_children[i]);
			return;
		}

	}

}


/*************************************************************************
	Remove the specified Window form this windows child list.
*************************************************************************/
void Window::removeChildWindow(Window* window)
{
	removeChild_impl(window);
    WindowEventArgs args(window);
	onChildRemoved(args);
	window->onZChange_impl();
}


/*************************************************************************
	Remove the first child Window with the specified ID.  If there is more
	than one attached Window objects with the specified ID, only the fist
	one encountered will be removed.	
*************************************************************************/
void Window::removeChildWindow(uint ID)
{
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getID() == ID)
		{
			removeChildWindow(d_children[i]);
			return;
		}

	}

}


/*************************************************************************
	Move the Window to the top of the z order.
*************************************************************************/
void Window::moveToFront()
{
	// if the window has no parent then we can have no siblings
	if (d_parent == NULL)
	{
		// perform initial activation if required.
		if (!isActive())
		{
            ActivationEventArgs args(this);
			args.otherWindow = NULL;
			onActivated(args);
		}

		return;
	}

	// bring parent window to front of it's siblings...
	d_parent->moveToFront();

	// get our sibling window which is currently active (if any)
	Window* activeWnd = NULL;

	uint idx = d_parent->getChildCount();

	while (idx-- > 0)
	{
		if (d_parent->d_children[idx]->isActive())
		{
			activeWnd = d_parent->d_children[idx];
			break;
		}

	}

	if (d_zOrderingEnabled)
	{
		// move us in front of sibling windows with the same 'always-on-top' setting as we have.
		Window* org_parent = d_parent;
		org_parent->removeChild_impl(this);
		org_parent->addChild_impl(this);
	}

	// notify ourselves that we have become active
	if (activeWnd != this)
	{
        ActivationEventArgs args(this);
		args.otherWindow = activeWnd;
		onActivated(args);
	}

	// notify previously active window that it is no longer active
	if ((activeWnd != NULL) && (activeWnd != this))
	{
        ActivationEventArgs args(activeWnd);
		args.otherWindow = this;
		activeWnd->onDeactivated(args);
	}

	// TODO: This could probably be moved above, is anyone relying on ordering of events?
	if (d_zOrderingEnabled)
	{
		onZChange_impl();
	}

}


/*************************************************************************
	Move the Window to the bottom of the Z order.
*************************************************************************/
void Window::moveToBack()
{
	// if the window is active, de-activate it.
	if (isActive())
	{
        ActivationEventArgs args(this);
		args.otherWindow = NULL;
		onDeactivated(args);
	}

	// if the window has no parent then we can have no siblings and have nothing more to do.
	if (d_parent == NULL)
	{
		return;
	}

	if (d_zOrderingEnabled)
	{
		// move us behind all sibling windows with the same 'always-on-top' setting as we have.
		Window* org_parent = d_parent;
		d_parent->removeChild_impl(this);

		ChildList::iterator pos = d_children.begin();

		if (isAlwaysOnTop())
		{
			while ((pos != d_children.end()) && (!(*pos)->isAlwaysOnTop()))
			{
				++pos;
			}

		}

		d_children.insert(pos, this);
		setParent(org_parent);

		onZChange_impl();
	}

	d_parent->moveToBack();
}


/*************************************************************************
	Captures input to this window
*************************************************************************/
bool Window::captureInput(void)
{
	// we can only capture if we are the active window
	if (!isActive()) {
		return false;
	}

	Window* current_capture = d_captureWindow;
	d_captureWindow = this;
    WindowEventArgs args(this);

	// inform any window which previously had capture that it doesn't anymore!
	if ((current_capture != NULL) && (current_capture != this) && (!d_restoreOldCapture)) {
		current_capture->onCaptureLost(args);
	}

	if (d_restoreOldCapture) {
		d_oldCapture = current_capture;
	}

	onCaptureGained(args);

	return true;
}


/*************************************************************************
	Releases input capture from this Window.  If this Window does not
	have inputs captured, nothing happens.
*************************************************************************/
void Window::releaseInput(void)
{
	// if we are not the window that has capture, do nothing
	if (!isCapturedByThis()) {
		return;
	}

	// restore old captured window if that mode is set
	if (d_restoreOldCapture) {
		d_captureWindow = d_oldCapture;

		// check for case when there was no previously captured window
		if (d_oldCapture != NULL) {
			d_oldCapture = NULL;
			d_captureWindow->moveToFront();
		}

	}
	else {
		d_captureWindow = NULL;
	}

    WindowEventArgs args(this);
	onCaptureLost(args);
}


/*************************************************************************
	Set whether this window will remember and restore the previous window
	that had inputs captured.
*************************************************************************/
void Window::setRestoreCapture(bool setting)
{
	d_restoreOldCapture = setting;

	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		d_children[i]->setRestoreCapture(setting);
	}

}


/*************************************************************************
	Set the current alpha value for this window.
*************************************************************************/
void Window::setAlpha(float alpha)
{
	d_alpha = alpha;
	WindowEventArgs args(this);
	onAlphaChanged(args);
}


/*************************************************************************
	Sets whether this Window will inherit alpha from its parent windows.
*************************************************************************/
void Window::setInheritsAlpha(bool setting)
{
	if (d_inheritsAlpha != setting)
	{
		// store old effective alpha so we can test if alpha value changes due to new setting.
		float oldAlpha = getEffectiveAlpha();

		// notify about the setting change.
		d_inheritsAlpha = setting;

		WindowEventArgs args(this);
		onInheritsAlphaChanged(args);

		// if effective alpha has changed fire notification about that too
		if (oldAlpha != getEffectiveAlpha())
		{
			args.handled = false;
			onAlphaChanged(args);
		}

	}

}


/*************************************************************************
	Signal the System object to redraw (at least) this Window on the next
	render cycle.
*************************************************************************/
void Window::requestRedraw(void) const
{
	System::getSingleton().signalRedraw();
}


/*************************************************************************
	Convert the given X co-ordinate from absolute to relative metrics.
*************************************************************************/
float Window::absoluteToRelativeX(float val) const
{
	return absoluteToRelativeX_impl(this, val);
}


/*************************************************************************
	Convert the given Y co-ordinate from absolute to relative metrics.
*************************************************************************/
float Window::absoluteToRelativeY(float val) const
{
	return absoluteToRelativeY_impl(this, val);
}


/*************************************************************************
	Convert the given position from absolute to relative metrics.
*************************************************************************/
Point Window::absoluteToRelative(const Point& pt) const
{
	return absoluteToRelative_impl(this, pt);
}


/*************************************************************************
	Convert the given size from absolute to relative metrics.
*************************************************************************/
Size Window::absoluteToRelative(const Size& sze) const
{
	return absoluteToRelative_impl(this, sze);
}


/*************************************************************************
	Convert the given area from absolute to relative metrics.
*************************************************************************/
Rect Window::absoluteToRelative(const Rect& rect) const
{
	return absoluteToRelative_impl(this, rect);
}


/*************************************************************************
	Convert the given X co-ordinate from relative to absolute metrics.
*************************************************************************/
float Window::relativeToAbsoluteX(float val) const
{
	return relativeToAbsoluteX_impl(this, val);
}


/*************************************************************************
	Convert the given Y co-ordinate from relative to absolute metrics.
*************************************************************************/
float Window::relativeToAbsoluteY(float val) const
{
	return relativeToAbsoluteY_impl(this, val);
}


/*************************************************************************
	Convert the given position from relative to absolute metrics.
*************************************************************************/
Point Window::relativeToAbsolute(const Point& pt) const
{
	return relativeToAbsolute_impl(this, pt);
}


/*************************************************************************
	Convert the given size from relative to absolute metrics.
*************************************************************************/
Size Window::relativeToAbsolute(const Size& sze) const
{
	return relativeToAbsolute_impl(this, sze);
}


/*************************************************************************
	Convert the given area from relative to absolute metrics.
*************************************************************************/
Rect Window::relativeToAbsolute(const Rect& rect) const
{
		return relativeToAbsolute_impl(this, rect);
}


/*************************************************************************
	Convert a window co-ordinate value, specified in whichever metrics
	mode is active, to a screen relative pixel co-ordinate.
*************************************************************************/
float Window::windowToScreenX(float x) const
{
	const Window* wnd = this;
	float baseX = 0;

	while (wnd != NULL)
	{
		baseX += wnd->d_abs_area.d_left;
		wnd = wnd->d_parent;
	}

	if (getMetricsMode() == Relative)
	{
		return baseX + relativeToAbsoluteX(x);
	}
	else
	{
		return baseX + x;
	}

}


/*************************************************************************
	Convert a window co-ordinate value, specified in whichever metrics
	mode is active, to a screen relative pixel co-ordinate.
*************************************************************************/
float Window::windowToScreenY(float y) const
{
	const Window* wnd = this;
	float baseY = 0;

	while (wnd != NULL)
	{
		baseY += wnd->d_abs_area.d_top;
		wnd = wnd->d_parent;
	}

	if (getMetricsMode() == Relative)
	{
		return baseY + relativeToAbsoluteY(y);
	}
	else
	{
		return baseY + y;
	}

}


/*************************************************************************
	Convert a window co-ordinate position, specified in whichever metrics
	mode is active, to a screen relative pixel co-ordinate position.
*************************************************************************/
Point Window::windowToScreen(const Point& pt) const
{
	const Window* wnd = this;
	Point base(0, 0);

	while (wnd != NULL)
	{
		base.d_x += wnd->d_abs_area.d_left;
		base.d_y += wnd->d_abs_area.d_top;
		wnd = wnd->d_parent;
	}

	if (getMetricsMode() == Relative)
	{
		return base + relativeToAbsolute(pt);
	}
	else
	{
		return base + pt;
	}

}


/*************************************************************************
	Convert a window size value, specified in whichever metrics mode is
	active, to a size in pixels.
*************************************************************************/
Size Window::windowToScreen(const Size& sze) const
{
	if (getMetricsMode() == Relative)
	{
		return Size(sze.d_width * d_abs_area.getWidth(), sze.d_height * d_abs_area.getHeight());
	}
	else
	{
		return sze;
	}

}


/*************************************************************************
	Convert a window area, specified in whichever metrics mode is
	active, to a screen area.
*************************************************************************/
Rect Window::windowToScreen(const Rect& rect) const
{
	const Window* wnd = this;
	Point base(0, 0);

	while (wnd != NULL)
	{
		base.d_x += wnd->d_abs_area.d_left;
		base.d_y += wnd->d_abs_area.d_top;
		wnd = wnd->d_parent;
	}

	if (getMetricsMode() == Relative)
	{
		return relativeToAbsolute(rect).offset(base);
	}
	else
	{
		Rect tmp(rect);
		return tmp.offset(base);
	}

}


/*************************************************************************
	Convert a screen relative pixel co-ordinate value to a window
	co-ordinate value, specified in whichever metrics mode is active.
*************************************************************************/
float Window::screenToWindowX(float x) const
{
	x -= windowToScreenX(0);

	if (getMetricsMode() == Relative)
	{
		x /= d_abs_area.getWidth();
	}

	return x;
}


/*************************************************************************
	Convert a screen relative pixel co-ordinate value to a window
	co-ordinate value, specified in whichever metrics mode is active.
*************************************************************************/
float Window::screenToWindowY(float y) const
{
	y -= windowToScreenY(0);

	if (getMetricsMode() == Relative)
	{
		y /= d_abs_area.getHeight();
	}

	return y;
}


/*************************************************************************
	Convert a screen relative pixel position to a window co-ordinate
	position, specified in whichever metrics mode is active.
*************************************************************************/
Point Window::screenToWindow(const Point& pt) const
{
	Point tmp(pt);

	tmp.d_x -= windowToScreenX(0);
	tmp.d_y -= windowToScreenY(0);

	if (getMetricsMode() == Relative)
	{
		tmp.d_x /= d_abs_area.getWidth();
		tmp.d_y /= d_abs_area.getHeight();
	}

	return tmp;
}


/*************************************************************************
	Convert a screen size to a window based size
*************************************************************************/
Size Window::screenToWindow(const Size& sze) const
{
	Size tmp(sze);

	if (getMetricsMode() == Relative)
	{
		tmp.d_width		/= d_abs_area.getWidth();
		tmp.d_height	/= d_abs_area.getHeight();
	}

	return tmp;
}


/*************************************************************************
	Convert a screen area to a window area, specified in whichever
	metrics mode is active.
*************************************************************************/
Rect Window::screenToWindow(const Rect& rect) const
{
	Rect tmp(rect);

	tmp.d_left		-= windowToScreenX(0);
	tmp.d_top		-= windowToScreenY(0);
	tmp.d_right		-= windowToScreenX(0);
	tmp.d_bottom	-= windowToScreenY(0);

	if (getMetricsMode() == Relative)
	{
		tmp.d_left		/= d_abs_area.getWidth();
		tmp.d_top		/= d_abs_area.getHeight();
		tmp.d_right		/= d_abs_area.getWidth();
		tmp.d_bottom	/= d_abs_area.getHeight();
	}

	return tmp;
}


/*************************************************************************
	Causes the Window object to render itself.
*************************************************************************/
void Window::render(void)
{
	// don't do anything if window is not visible
	if (!isVisible()) {
		return;
	}

	// signal rendering started
	WindowEventArgs args(this);
	onRenderingStarted(args);

	// perform drawing for 'this' Window
	Renderer* renderer = System::getSingleton().getRenderer();
	drawSelf(renderer->getCurrentZ());
	renderer->advanceZValue();

	// render any child windows
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		d_children[i]->render();
	}

	// signal rendering ended
	onRenderingEnded(args);
}


/*************************************************************************
	Set the parent window for this window object.
*************************************************************************/
void Window::setParent(Window* parent)
{
	d_parent = parent;
}


/*************************************************************************
	Return the pixel Width of the parent element.
	This always returns a valid number.
*************************************************************************/
float Window::getParentWidth(void) const
{
	if (d_parent == NULL)
	{
		return System::getSingleton().getRenderer()->getWidth();
	}

	return d_parent->d_abs_area.getWidth();
}


/*************************************************************************
	Return the pixel Height of the parent element.
	This always returns a valid number.
*************************************************************************/
float Window::getParentHeight(void) const
{
	if (d_parent == NULL)
	{
		return System::getSingleton().getRenderer()->getHeight();
	}

	return d_parent->d_abs_area.getHeight();
}


/*************************************************************************
	Return the pixel size of the parent element.
	This always returns a valid object.
*************************************************************************/
Size Window::getParentSize(void) const
{
	return getWindowSize_impl(d_parent);
}


/*************************************************************************
	Add standard Window events
*************************************************************************/
void Window::addStandardEvents(void)
{
	// window events
	addEvent(EventSized);					addEvent(EventMoved);					addEvent(EventTextChanged);
	addEvent(EventFontChanged);				addEvent(EventAlphaChanged);			addEvent(EventIDChanged);
	addEvent(EventActivated);				addEvent(EventDeactivated);				addEvent(EventShown);
	addEvent(EventHidden);					addEvent(EventEnabled);					addEvent(EventDisabled);
	addEvent(EventMetricsModeChanged);		addEvent(EventClippedByParentChanged);	addEvent(EventDestroyedByParentChanged);
	addEvent(EventInheritsAlphaChanged);	addEvent(EventAlwaysOnTopChanged);		addEvent(EventInputCaptureGained);
	addEvent(EventInputCaptureLost);		addEvent(EventRenderingStarted);		addEvent(EventRenderingEnded);
	addEvent(EventChildAdded);				addEvent(EventChildRemoved);			addEvent(EventDestructionStarted);
	addEvent(EventZOrderChanged);			addEvent(EventParentSized);

	// general input handling
	addEvent(EventMouseEnters);				addEvent(EventMouseLeaves);				addEvent(EventMouseMove);
	addEvent(EventMouseWheel);				addEvent(EventMouseButtonDown);			addEvent(EventMouseButtonUp);
	addEvent(EventMouseClick);				addEvent(EventMouseDoubleClick);		addEvent(EventMouseTripleClick);
	addEvent(EventKeyDown);					addEvent(EventKeyUp);					addEvent(EventCharacterKey);
}


/*************************************************************************
	Cleanup child windows
*************************************************************************/
void Window::cleanupChildren(void)
{
	while(getChildCount() != 0)
	{
		Window* wnd = d_children[0];

		// always remove child
		removeChildWindow(wnd);

		// destroy child if that is required
		if (wnd->isDestroyedByParent())
		{
			WindowManager::getSingleton().destroyWindow(wnd);
		}

	}

}


/*************************************************************************
	Add given window to child list at an appropriate position
*************************************************************************/
void Window::addChild_impl(Window* wnd)
{
	// if window is already attached, detach it first (will fire normal events)
	if (wnd->getParent() != NULL)
	{
		wnd->getParent()->removeChildWindow(wnd);
	}

	// calculate position where window should be added
	ChildList::reverse_iterator		position = d_children.rbegin();
	if (!wnd->isAlwaysOnTop())
	{
		position = d_children.rbegin();

		// find last non-topmost window
		while ((position != d_children.rend()) && ((*position)->isAlwaysOnTop()))
		{
			++position;
		}

	}

	// add window (just behind 'pos')
	d_children.insert(position.base(), wnd);
	wnd->setParent(this);

	// Force and update for the area Rects for 'wnd' so they're correct for it's new parent.
    WindowEventArgs args(this);
	wnd->onParentSized(args);
}


/*************************************************************************
	Remove given window from child list
*************************************************************************/
void Window::removeChild_impl(Window* wnd)
{
	if (!d_children.empty())
	{
		ChildList::iterator	position;
		position = std::find(d_children.begin(), d_children.end(), wnd);

		if (position != d_children.end())
		{
			d_children.erase(position);
			wnd->setParent(NULL);
		}

	}

}


/*************************************************************************
	Notify 'this' and all siblings of a ZOrder change event
*************************************************************************/
void Window::onZChange_impl(void)
{
	if (d_parent == NULL)
	{
        WindowEventArgs args(this);
		onZChanged(args);
	}
	else
	{
		uint child_count = d_parent->getChildCount();

		for (uint i = 0; i < child_count; ++i)
		{
            WindowEventArgs args(d_parent->d_children[i]);
			d_parent->d_children[i]->onZChanged(args);
		}

	}

}


/*************************************************************************
	
*************************************************************************/
Rect Window::absoluteToRelative_impl(const Window* window, const Rect& rect) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	Rect tmp;

	if (sz.d_width)
	{
		tmp.d_left	= PixelAligned(rect.d_left) / sz.d_width;
		tmp.d_right = PixelAligned(rect.d_right) / sz.d_width;
	}
	else
	{
		tmp.d_left = tmp.d_right = 0;
	}

	if (sz.d_height)
	{
		tmp.d_top		= PixelAligned(rect.d_top) / sz.d_height;
		tmp.d_bottom	= PixelAligned(rect.d_bottom) / sz.d_height;
	}
	else
	{
		tmp.d_top = tmp.d_bottom= 0;
	}

	return tmp;
}


/*************************************************************************

*************************************************************************/
Size Window::absoluteToRelative_impl(const Window* window, const Size& sz) const
{
	// get size object for whatever we are using as a base for the conversion
	Size wndsz = getWindowSize_impl(window);

	Size tmp;

	if (wndsz.d_width)
	{
		tmp.d_width = PixelAligned(sz.d_width) / wndsz.d_width;
	}
	else
	{
		tmp.d_width = 0;
	}

	if (wndsz.d_height)
	{
		tmp.d_height = PixelAligned(sz.d_height) / wndsz.d_height;
	}
	else
	{
		tmp.d_height = 0;
	}

	return tmp;
}


/*************************************************************************

*************************************************************************/
Point Window::absoluteToRelative_impl(const Window* window, const Point& pt) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	Point tmp;

	if (sz.d_width)
	{
		tmp.d_x = PixelAligned(pt.d_x) / sz.d_width;
	}
	else
	{
		tmp.d_x = 0;
	}

	if (sz.d_height)
	{
		tmp.d_y = PixelAligned(pt.d_y) / sz.d_height;
	}
	else
	{
		tmp.d_y = 0;
	}

	return tmp;
}


/*************************************************************************

*************************************************************************/
float Window::absoluteToRelativeX_impl(const Window* window, float x) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	if (sz.d_width)
	{
		return PixelAligned(x) / sz.d_width;
	}
	else
	{
		return 0;
	}
}


/*************************************************************************

*************************************************************************/
float Window::absoluteToRelativeY_impl(const Window* window, float y) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	if (sz.d_height)
	{
		return PixelAligned(y) / sz.d_height;
	}
	else
	{
		return 0;
	}
}


/*************************************************************************

*************************************************************************/
Rect Window::relativeToAbsolute_impl(const Window* window, const Rect& rect) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	return Rect(
		PixelAligned(rect.d_left * sz.d_width),
		PixelAligned(rect.d_top * sz.d_height),
		PixelAligned(rect.d_right * sz.d_width),
		PixelAligned(rect.d_bottom * sz.d_height)
		);
}


/*************************************************************************

*************************************************************************/
Size Window::relativeToAbsolute_impl(const Window* window, const Size& sz) const
{
	// get size object for whatever we are using as a base for the conversion
	Size wndsz = getWindowSize_impl(window);

	return Size(
		PixelAligned(sz.d_width * wndsz.d_width),
		PixelAligned(sz.d_height * wndsz.d_height)
		);
}


/*************************************************************************

*************************************************************************/
Point Window::relativeToAbsolute_impl(const Window* window, const Point& pt) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	return Point(
		PixelAligned(pt.d_x * sz.d_width),
		PixelAligned(pt.d_y * sz.d_height)
		);
}


/*************************************************************************

*************************************************************************/
float Window::relativeToAbsoluteX_impl(const Window* window, float x) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	return PixelAligned(x * sz.d_width);
}


/*************************************************************************

*************************************************************************/
float Window::relativeToAbsoluteY_impl(const Window* window, float y) const
{
	// get size object for whatever we are using as a base for the conversion
	Size sz = getWindowSize_impl(window);

	return PixelAligned(y * sz.d_height);
}


/*************************************************************************
	Return size of window.  If window is NULL return size of display.
*************************************************************************/
Size Window::getWindowSize_impl(const Window* window) const
{
	if (window == NULL)
	{
		return System::getSingleton().getRenderer()->getSize();
	}
	else
	{
		return window->d_abs_area.getSize();
	}

}


/*************************************************************************
	Return the current maximum size for this window.
*************************************************************************/
Size Window::getMaximumSize(void) const
{
	if (getMetricsMode() == Absolute)
	{
		return d_maxSize;
	}
	else
	{
		return absoluteToRelative_impl(NULL, d_maxSize);
	}

}


/*************************************************************************
	Return the current minimum size for this window.
*************************************************************************/
Size Window::getMinimumSize(void) const
{
	if (getMetricsMode() == Absolute)
	{
		return d_minSize;
	}
	else
	{
		return absoluteToRelative_impl(NULL, d_minSize);
	}

}


/*************************************************************************
	Set the minimum size for this window.
*************************************************************************/
void Window::setMinimumSize(const Size& sz)
{
	if (getMetricsMode() == Absolute)
	{
		d_minSize.d_width = PixelAligned(sz.d_width);
		d_minSize.d_height = PixelAligned(sz.d_height);
	}
	else
	{
		d_minSize = relativeToAbsolute_impl(NULL, sz);
	}

	// store old size.
	Rect old_sz(d_abs_area);

	// limit size as required
	d_abs_area.constrainSizeMin(d_minSize);

	// if size has changed, trigger notifications
	if (old_sz != d_abs_area)
	{
        WindowEventArgs args(this);
		onSized(args);
	}

}


/*************************************************************************
	Set the maximum size for this window.
*************************************************************************/
void Window::setMaximumSize(const Size& sz)
{
	if (getMetricsMode() == Absolute)
	{
		d_maxSize.d_width = PixelAligned(sz.d_width);
		d_maxSize.d_height = PixelAligned(sz.d_height);
	}
	else
	{
		d_maxSize = relativeToAbsolute_impl(NULL, sz);
	}

	// store old size.
	Rect old_sz(d_abs_area);

	// limit size as required
	d_abs_area.constrainSizeMax(d_maxSize);

	// if size has changed, trigger notifications
	if (old_sz != d_abs_area)
	{
        WindowEventArgs args(this);
		onSized(args);
	}

}


/*************************************************************************
	Return a pointer to the mouse cursor image to use when the mouse is
	within this window.
*************************************************************************/
const Image* Window::getMouseCursor(void) const
{
	if (d_mouseCursor != (const Image*)DefaultMouseCursor)
	{
		return d_mouseCursor;
	}
	else
	{
		return System::getSingleton().getDefaultMouseCursor();
	}

}


/*************************************************************************
	Set the mouse cursor image to be used when the mouse enters this
	window.	
*************************************************************************/
void Window::setMouseCursor(const String& imageset, const String& image_name)
{
	d_mouseCursor = &ImagesetManager::getSingleton().getImageset(imageset)->getImage(image_name);
}


/*************************************************************************
	Set the current ID for the Window.	
*************************************************************************/
void Window::setID(uint ID)
{
	if (d_ID != ID)
	{
		d_ID = ID;

		WindowEventArgs args(this);
		onIDChanged(args);
	}

}


/*************************************************************************
	set the current metrics mode employed by the Window	
*************************************************************************/
void Window::setMetricsMode(MetricsMode	mode)
{
	if (d_metricsMode != mode)
	{
		MetricsMode oldMode = d_metricsMode;
		d_metricsMode = mode;

		// only ever trigger the event if the mode is actually changed.
		if ((d_metricsMode != Inherited) || (oldMode != getMetricsMode()))
		{
			WindowEventArgs args(this);
			onMetricsChanged(args);
		}

	}

}


/*************************************************************************
	Set whether or not this Window will automatically be destroyed when
	its parent Window is destroyed.	
*************************************************************************/
void Window::setDestroyedByParent(bool setting)
{
	if (d_destroyedByParent != setting)
	{
		d_destroyedByParent = setting;

		WindowEventArgs args(this);
		onParentDestroyChanged(args);
	}

}


/*************************************************************************
	Return the inherited metrics mode.  This is either the metrics mode
	of our parent, or Relative if we have no parent.	
*************************************************************************/
MetricsMode Window::getInheritedMetricsMode(void) const
{
	return (d_parent == NULL) ? Relative : d_parent->getMetricsMode();
}


/*************************************************************************
	return the x position of the window using the specified metrics system.	
*************************************************************************/
float Window::getXPosition(MetricsMode mode) const
{
	// get proper mode to use for inherited.
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Absolute)
	{
		return d_abs_area.d_left;
	}
	else
	{
		return d_rel_area.d_left;
	}

}


/*************************************************************************
	return the y position of the window using the specified metrics system.	
*************************************************************************/
float Window::getYPosition(MetricsMode mode) const
{
	// get proper mode to use for inherited.
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Absolute)
	{
		return d_abs_area.d_top;
	}
	else
	{
		return d_rel_area.d_top;
	}

}


/*************************************************************************
	return the position of the window using the specified metrics system.	
*************************************************************************/
Point Window::getPosition(MetricsMode mode) const
{
	// get proper mode to use for inherited.
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Absolute)
	{
		return d_abs_area.getPosition();
	}
	else
	{
		return d_rel_area.getPosition();
	}

}


/*************************************************************************
	return the width of the Window using the specified metrics system.	
*************************************************************************/
float Window::getWidth(MetricsMode mode) const
{
	// get proper mode to use for inherited.
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Absolute)
	{
		return d_abs_area.getWidth();
	}
	else
	{
		return d_rel_area.getWidth();
	}

}


/*************************************************************************
	return the height of the Window using the specified metrics system.	
*************************************************************************/
float Window::getHeight(MetricsMode mode) const
{
	// get proper mode to use for inherited.
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Absolute)
	{
		return d_abs_area.getHeight();
	}
	else
	{
		return d_rel_area.getHeight();
	}

}


/*************************************************************************
	return the size of the Window using the specified metrics system.	
*************************************************************************/
Size Window::getSize(MetricsMode mode) const
{
	// get proper mode to use for inherited.
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Absolute)
	{
		return d_abs_area.getSize();
	}
	else
	{
		return d_rel_area.getSize();
	}

}


/*************************************************************************
	return a Rect object that describes the Window area using the
	specified metrics system.
*************************************************************************/
Rect Window::getRect(MetricsMode mode) const
{
	// get proper mode to use for inherited.
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Absolute)
	{
		return d_abs_area;
	}
	else
	{
		return d_rel_area;
	}

}


/*************************************************************************
	set the x position of the window using the specified metrics system.	
*************************************************************************/
void Window::setXPosition(MetricsMode mode, float x)
{
	setPosition(mode, Point(x, getYPosition(mode)));
}


/*************************************************************************
	set the y position of the window using the specified metrics system.	
*************************************************************************/
void Window::setYPosition(MetricsMode mode, float y)
{
	setPosition(mode, Point(getXPosition(mode), y));
}


/*************************************************************************
	set the position of the window using the specified metrics system.	
*************************************************************************/
void Window::setPosition(MetricsMode mode, const Point& position)
{
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Relative)
	{
		d_rel_area.setPosition(position);
		d_abs_area = relativeToAbsolute_impl(d_parent, d_rel_area);
		d_abs_area.constrainSize(d_maxSize, d_minSize);
	}
	else
	{
		Point intPos(PixelAligned(position.d_x), PixelAligned(position.d_y));

		d_abs_area.setPosition(intPos);
		d_rel_area.setPosition(absoluteToRelative_impl(d_parent, position));
	}

	WindowEventArgs args(this);
	onMoved(args);
}


/*************************************************************************
	set the width of the Window using the specified metrics system.	
*************************************************************************/
void Window::setWidth(MetricsMode mode, float width)
{
	setSize(mode, Size(width, getHeight(mode)));
}


/*************************************************************************
	set the height of the Window using the specified metrics system.	
*************************************************************************/
void Window::setHeight(MetricsMode mode, float height)
{
	setSize(mode, Size(getWidth(mode), height));
}


/*************************************************************************
	set the size of the Window using the specified metrics system.	
*************************************************************************/
void Window::setSize(MetricsMode mode, const Size& size)
{
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Relative)
	{
		d_rel_area.setSize(size);

		// update Rect for the other metrics system
        d_abs_area = relativeToAbsolute_impl(d_parent, d_rel_area);
		d_abs_area.constrainSize(d_maxSize, d_minSize);
	}
	else
	{
		Size intSize(PixelAligned(size.d_width), PixelAligned(size.d_height));

		d_abs_area.setSize(intSize);
		d_abs_area.constrainSize(d_maxSize, d_minSize);

		// update Rect for the other metrics system.
		d_rel_area.setSize(absoluteToRelative_impl(d_parent, size));
	}

	WindowEventArgs args(this);
	onSized(args);
}


/*************************************************************************
	set the Rect that describes the Window area using the specified
	metrics system.	
*************************************************************************/
void Window::setRect(MetricsMode mode, const Rect& area)
{
	if (mode == Inherited)
	{
		mode = getInheritedMetricsMode();
	}

	if (mode == Relative)
	{
		d_rel_area = area;

		d_abs_area = relativeToAbsolute_impl(d_parent, area);
		d_abs_area.constrainSize(d_maxSize, d_minSize);
	}
	else
	{
		Rect intArea(
			PixelAligned(area.d_left),
			PixelAligned(area.d_top),
			PixelAligned(area.d_right),
			PixelAligned(area.d_bottom)
			);

		d_abs_area = intArea;
		d_abs_area.constrainSize(d_maxSize, d_minSize);

		d_rel_area = absoluteToRelative_impl(d_parent, area);
	}

	WindowEventArgs args(this);
	onMoved(args);
	onSized(args);
}


/*************************************************************************
	Add standard CEGUI::Window properties.
*************************************************************************/
void Window::addStandardProperties(void)
{
	addProperty(&d_absHeightProperty);
	addProperty(&d_absMaxSizeProperty);
	addProperty(&d_absMinSizeProperty);
	addProperty(&d_absPositionProperty);
	addProperty(&d_absRectProperty);
	addProperty(&d_absSizeProperty);
	addProperty(&d_absWidthProperty);
	addProperty(&d_absXPosProperty);
	addProperty(&d_absYPosProperty);
	addProperty(&d_alphaProperty);
	addProperty(&d_alwaysOnTopProperty);
	addProperty(&d_clippedByParentProperty);
	addProperty(&d_destroyedByParentProperty);
	addProperty(&d_disabledProperty);
	addProperty(&d_fontProperty);
	addProperty(&d_heightProperty);
	addProperty(&d_IDProperty);
	addProperty(&d_inheritsAlphaProperty);
	addProperty(&d_metricsModeProperty);
	addProperty(&d_mouseCursorProperty);
	addProperty(&d_positionProperty);
	addProperty(&d_rectProperty);
	addProperty(&d_relHeightProperty);
	addProperty(&d_relMaxSizeProperty);
	addProperty(&d_relMinSizeProperty);
	addProperty(&d_relPositionProperty);
	addProperty(&d_relRectProperty);
	addProperty(&d_relSizeProperty);
	addProperty(&d_relWidthProperty);
	addProperty(&d_relXPosProperty);
	addProperty(&d_relYPosProperty);
	addProperty(&d_restoreOldCaptureProperty);
	addProperty(&d_sizeProperty);
	addProperty(&d_textProperty);
	addProperty(&d_visibleProperty);
	addProperty(&d_widthProperty);
	addProperty(&d_xPosProperty);
	addProperty(&d_yPosProperty);
	addProperty(&d_zOrderChangeProperty);
}


/*************************************************************************
	Return whether z-order changes are enabled.
*************************************************************************/
bool Window::isZOrderingEnabled(void) const
{
	return d_zOrderingEnabled;
}


/*************************************************************************
	Set whether z-order changes are enabled.
*************************************************************************/
void Window::setZOrderingEnabled(bool setting)
{
	if (d_zOrderingEnabled != setting)
	{
		d_zOrderingEnabled = setting;
	}

}


/*************************************************************************
	Cause window to update itself and any attached children
*************************************************************************/
void Window::update(float elapsed)
{
	// perform update for 'this' Window
	updateSelf(elapsed);

	// update child windows
	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		d_children[i]->update(elapsed);
	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Begin event triggers section

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

void Window::onSized(WindowEventArgs& e)
{
	// inform children their parent has been re-sized
	uint child_count = getChildCount();
	for (uint i = 0; i < child_count; ++i)
	{
		WindowEventArgs args(this);
		d_children[i]->onParentSized(args);
	}

	requestRedraw();

	fireEvent(EventSized, e, EventNamespace);
}


void Window::onMoved(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventMoved, e, EventNamespace);
}


void Window::onTextChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventTextChanged, e, EventNamespace);
}


void Window::onFontChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventFontChanged, e, EventNamespace);
}


void Window::onAlphaChanged(WindowEventArgs& e)
{
	// scan child list and call this method for all children that inherit alpha
	int child_count = getChildCount();

	for (int i = 0; i < child_count; ++i)
	{
		if (d_children[i]->inheritsAlpha())
		{
            WindowEventArgs args(d_children[i]);
			d_children[i]->onAlphaChanged(args);
		}

	}

	requestRedraw();
	fireEvent(EventAlphaChanged, e, EventNamespace);
}


void Window::onIDChanged(WindowEventArgs& e)
{
	fireEvent(EventIDChanged, e, EventNamespace);
}


void Window::onShown(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventShown, e, EventNamespace);
}


void Window::onHidden(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventHidden, e, EventNamespace);
}


void Window::onEnabled(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventEnabled, e, EventNamespace);
}


void Window::onDisabled(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventDisabled, e, EventNamespace);
}


void Window::onMetricsChanged(WindowEventArgs& e)
{
	fireEvent(EventMetricsModeChanged, e, EventNamespace);
}


void Window::onClippingChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventClippedByParentChanged, e, EventNamespace);
}


void Window::onParentDestroyChanged(WindowEventArgs& e)
{
	fireEvent(EventDestroyedByParentChanged, e, EventNamespace);
}


void Window::onInheritsAlphaChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventInheritsAlphaChanged, e, EventNamespace);
}


void Window::onAlwaysOnTopChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventAlwaysOnTopChanged, e, EventNamespace);
}


void Window::onCaptureGained(WindowEventArgs& e)
{
	fireEvent(EventInputCaptureGained, e, EventNamespace);
}


void Window::onCaptureLost(WindowEventArgs& e)
{
	// handle restore of previous capture window as required.
	if (d_restoreOldCapture && (d_oldCapture != NULL)) {
		d_oldCapture->onCaptureLost(e);
		d_oldCapture = NULL;
	}

	// handle case where mouse is now in a different window
	// (this is a bit of a hack that uses the mouse input injector to handle this for us).
	System::getSingleton().injectMouseMove(0, 0);

	fireEvent(EventInputCaptureLost, e, EventNamespace);
}


void Window::onRenderingStarted(WindowEventArgs& e)
{
	fireEvent(EventRenderingStarted, e, EventNamespace);
}


void Window::onRenderingEnded(WindowEventArgs& e)
{
	fireEvent(EventRenderingEnded, e, EventNamespace);
}


void Window::onZChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventZOrderChanged, e, EventNamespace);
}


void Window::onDestructionStarted(WindowEventArgs& e)
{
	fireEvent(EventDestructionStarted, e, EventNamespace);
}


void Window::onActivated(ActivationEventArgs& e)
{
	d_active = true;
	requestRedraw();
	fireEvent(EventActivated, e, EventNamespace);
}


void Window::onDeactivated(ActivationEventArgs& e)
{
	// first de-activate all children
	uint child_count = getChildCount();
	for (uint i = 0; i < child_count; ++i)
	{
		if (d_children[i]->isActive())
		{
			d_children[i]->onDeactivated(e);
		}

	}

	d_active = false;
	requestRedraw();
	fireEvent(EventDeactivated, e, EventNamespace);
}


void Window::onParentSized(WindowEventArgs& e)
{
	// synchronise area rects for new parent size
	if (getMetricsMode() == Relative)
	{
		d_abs_area = relativeToAbsolute_impl(d_parent, d_rel_area);

		// Check new absolute size and limit to currently set max/min values.  This does not affect relative co-ordinates
		// which must 'recover' after window is again sized so normal relativity can take over.
		d_abs_area.constrainSize(d_maxSize, d_minSize);

		// perform notifications
        WindowEventArgs args(this); 
		onMoved(args);
		onSized(args);

		// call for a redraw
		requestRedraw();
	}
	else
	{
		d_rel_area = absoluteToRelative_impl(d_parent, d_abs_area);
	}

	fireEvent(EventParentSized, e, EventNamespace);
}


void Window::onChildAdded(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventChildAdded, e, EventNamespace);
}


void Window::onChildRemoved(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventChildRemoved, e, EventNamespace);
}


void Window::onMouseEnters(MouseEventArgs& e)
{
	// set the mouse cursor
	MouseCursor::getSingleton().setImage(getMouseCursor());

	fireEvent(EventMouseEnters, e, EventNamespace);
}


void Window::onMouseLeaves(MouseEventArgs& e)
{
	fireEvent(EventMouseLeaves, e, EventNamespace);
}


void Window::onMouseMove(MouseEventArgs& e)
{
	fireEvent(EventMouseMove, e, EventNamespace);
}


void Window::onMouseWheel(MouseEventArgs& e)
{
	fireEvent(EventMouseWheel, e, EventNamespace);
}


void Window::onMouseButtonDown(MouseEventArgs& e)
{
	if (e.button == LeftButton)
	{
		moveToFront();
	}

	fireEvent(EventMouseButtonDown, e, EventNamespace);
}


void Window::onMouseButtonUp(MouseEventArgs& e)
{
	fireEvent(EventMouseButtonUp, e, EventNamespace);
}


void Window::onMouseClicked(MouseEventArgs& e)
{
	fireEvent(EventMouseClick, e, EventNamespace);
}


void Window::onMouseDoubleClicked(MouseEventArgs& e)
{
	fireEvent(EventMouseDoubleClick, e, EventNamespace);
}


void Window::onMouseTripleClicked(MouseEventArgs& e)
{
	fireEvent(EventMouseTripleClick, e, EventNamespace);
}


void Window::onKeyDown(KeyEventArgs& e)
{
	fireEvent(EventKeyDown, e, EventNamespace);
}


void Window::onKeyUp(KeyEventArgs& e)
{
	fireEvent(EventKeyUp, e, EventNamespace);
}


void Window::onCharacter(KeyEventArgs& e)
{
	fireEvent(EventCharacterKey, e, EventNamespace);
}

} // End of  CEGUI namespace section
