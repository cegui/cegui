/************************************************************************
	filename: 	CEGUIWindow.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the Window base class
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
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	static data definitions
*************************************************************************/
Window*	Window::d_captureWindow		= NULL;


/*************************************************************************
	Event name constants
*************************************************************************/
const utf8	Window::ParentSized[]					= "ParentSized";
const utf8	Window::SizedEvent[]					= "Sized";
const utf8	Window::MovedEvent[]					= "Moved";
const utf8	Window::TextChangedEvent[]				= "TextChanged";
const utf8	Window::FontChangedEvent[]				= "FontChanged";
const utf8	Window::AlphaChangedEvent[]				= "AlphaChanged";
const utf8	Window::IDChangedEvent[]				= "IDChanged";
const utf8	Window::ActivatedEvent[]				= "Activated";
const utf8	Window::DeactivatedEvent[]				= "Deactivated";
const utf8	Window::ShownEvent[]					= "Shown";
const utf8	Window::HiddenEvent[]					= "Hidden";
const utf8	Window::EnabledEvent[]					= "Enabled";
const utf8	Window::DisabledEvent[]					= "Disabled";
const utf8	Window::MetricsChangedEvent[]			= "MetricsChanged";
const utf8	Window::ClippingChangedEvent[]			= "ClippingChanged";
const utf8	Window::ParentDestroyChangedEvent[]		= "DestroyedByParentChanged";
const utf8	Window::InheritsAlphaChangedEvent[]		= "InheritAlphaChanged";
const utf8	Window::AlwaysOnTopChangedEvent[]		= "AlwaysOnTopChanged";
const utf8	Window::CaptureGainedEvent[]			= "CaptureGained";
const utf8	Window::CaptureLostEvent[]				= "CaptureLost";
const utf8	Window::RenderingStartedEvent[]			= "StartRender";
const utf8	Window::RenderingEndedEvent[]			= "EndRender";
const utf8	Window::ChildAddedEvent[]				= "AddedChild";
const utf8	Window::ChildRemovedEvent[]				= "RemovedChild";
const utf8	Window::DestructionStartedEvent[]		= "DestructStart";
const utf8	Window::ZChangedEvent[]					= "ZChanged";
const utf8	Window::MouseEntersEvent[]				= "MouseEnter";
const utf8	Window::MouseLeavesEvent[]				= "MouseLeave";
const utf8	Window::MouseMoveEvent[]				= "MouseMove";
const utf8	Window::MouseWheelEvent[]				= "MouseWheel";
const utf8	Window::MouseButtonDownEvent[]			= "MouseButtonDown";
const utf8	Window::MouseButtonUpEvent[]			= "MouseButtonUp";
const utf8	Window::MouseClickEvent[]				= "MouseClick";
const utf8	Window::MouseDoubleClickEvent[]			= "MouseDoubleClick";
const utf8	Window::MouseTripleClickEvent[]			= "MouseTripleClick";
const utf8	Window::KeyDownEvent[]					= "KeyDown";
const utf8	Window::KeyUpEvent[]					= "KeyUp";
const utf8	Window::CharacterEvent[]				= "CharacterKey";
	
	
/*************************************************************************
	Constructor
*************************************************************************/
Window::Window(const String& type, const String& name) :
	d_type(type),
	d_name(name)
{
	addStandardEvents();
}

/*************************************************************************
	Destructor
*************************************************************************/
Window::~Window(void)
{
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

	return (!d_enabled) && parDisabled;
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

	throw UnknownObjectException((utf8*)"The Window object named '" + name +"' is not attached to Window '" + d_name + "'.");
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
	throw UnknownObjectException((utf8*)"The Window with the requested ID is not attached to Window '" + d_name + "'.");
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
		return getUnclippedPixelRect().getIntersection(d_parent->getPixelRect());
	}
	// else, clip to screen
	else
	{
		return getUnclippedPixelRect().getIntersection(System::getSingleton().getRenderer()->getRect());
	}

}


/*************************************************************************
	return a Rect object describing the Window area unclipped, in
	screen space.	
*************************************************************************/
Rect Window::getUnclippedPixelRect(void) const
{
	return windowToScreen(d_abs_area);
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
		if (d_parent != NULL)
		{
			return d_parent->getMetricsMode();
		}

		return Relative;
	}

	return d_metricsMode;
}


/*************************************************************************
	return the x position of the window.  Interpretation of return value
	depends upon the metric type in use by this window.
*************************************************************************/
float Window::getXPos(void) const
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
float Window::getYPos(void) const
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
Point Window::getPos(void) const
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

		// move us infront of sibling windows with the same 'always-on-top' setting as we have.
		Window* parent = getParent();

		if (parent != NULL)
		{
			parent->removeChild_impl(this);
			parent->addChild_impl(this);

			onZChange_impl();
		}

		onAlwaysOnTopChanged(EventArgs());
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
		d_enabled ? onEnabled(EventArgs()) : onDisabled(EventArgs());
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
		d_visible ? onShown(EventArgs()) : onHidden(EventArgs());
	}

}


/*************************************************************************
	Activate the Window giving it input focus and bringing it to the top
	of all non always-on-top Windows.
*************************************************************************/
void Window::activate(void)
{
	moveToFront();
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
		onClippingChanged(EventArgs());
	}

}


/*************************************************************************
	Set the current text string for the Window.
*************************************************************************/
void Window::setText(const String& text)
{
	d_text = text;
	onTextChanged(EventArgs());
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
	if (getMetricsMode() == Relative)
	{
		d_rel_area.setSize(size);
		d_abs_area.setSize(relativeToAbsolute(size));
	}
	else
	{
		d_abs_area.setSize(size);
		d_rel_area.setSize(absoluteToRelative(size));
	}

	onSized(EventArgs());
}


/*************************************************************************
	Set the current 'x' position of the Window.  Interpretation of the
	input value is dependant upon the current metrics system set for the
	Window.
*************************************************************************/
void Window::setXPos(float x)
{
	setPosition(Point(x, getYPos()));
}


/*************************************************************************
	Set the current 'y' position of the Window.  Interpretation of the
	input value is dependant upon the current metrics system set for the
	Window.
*************************************************************************/
void Window::setYPos(float y)
{
	setPosition(Point(getXPos(), y));
}


/*************************************************************************
	Set the current position of the Window.  Interpretation of the input
	value is dependant upon the current metrics system set for the Window.
*************************************************************************/
void Window::setPosition(const Point& position)
{
	if (getMetricsMode() == Relative)
	{
		d_rel_area.setPosition(position);
		d_abs_area.setPosition(relativeToAbsolute(position));
	}
	else
	{
		d_abs_area.setPosition(position);
		d_rel_area.setPosition(absoluteToRelative(position));
	}

	onMoved(EventArgs());
}


/*************************************************************************
	Set the current area for the Window, this allows for setting of
	position and size at the same time.  Interpretation of the input
	value is dependant upon the current metrics system set for the Window.	
*************************************************************************/
void Window::setAreaRect(const Rect& area)
{
	if (getMetricsMode() == Relative)
	{
		d_rel_area = area;
		d_abs_area = relativeToAbsolute(area);
	}
	else
	{
		d_abs_area = area;
		d_rel_area = absoluteToRelative(area);
	}

	onMoved(EventArgs());
	onSized(EventArgs());
}


/*************************************************************************
	Set the font used by this Window.
*************************************************************************/
void Window::setFont(const Font* font)
{
	d_font = font;
	onFontChanged(EventArgs());
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
	onChildAdded(WindowEventArgs(window));
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
	onChildRemoved(WindowEventArgs(window));
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
	// if the window has no parent then we can have no siblings and have nothing more to do.
	Window* parent = getParent();
	if (parent == NULL)
	{
		return;
	}

	// get our sibling window which is currently active (if any)
	Window* activeWnd = NULL;

	uint idx = getChildCount();

	while (idx-- > 0)
	{
		if (parent->d_children[idx]->isActive())
		{
			activeWnd = parent->d_children[idx];
			break;
		}

	}

	// move us infront of sibling windows with the same 'always-on-top' setting as we have.
	parent->removeChild_impl(this);
	parent->addChild_impl(this);

	// notify ourselves that we have become active
	if (activeWnd != this)
	{
		onActivated(WindowEventArgs(activeWnd));
	}

	// notify previously active window that it is no longer active
	if ((activeWnd != NULL) && (activeWnd != this))
	{
		activeWnd->onDeactivated(WindowEventArgs(this));
	}

	onZChange_impl();

	// bring parent window to front of it's siblings...
	parent->moveToFront();
}


/*************************************************************************
	Move the Window to the bottom of the Z order.
*************************************************************************/
void Window::moveToBack()
{
	Window* parent = getParent();

	// if the window has no parent then we can have no siblings and have nothing more to do.
	if (parent == NULL)
	{
		return;
	}

	// if the window is active, de-activate it.
	onDeactivated(WindowEventArgs(NULL));

	// move us behind all sibling windows with the same 'always-on-top' setting as we have.
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
	setParent(parent);

	onZChange_impl();

	parent->moveToBack();
}


/*************************************************************************
	Captures input to this window
*************************************************************************/
void Window::captureInput(void)
{
	// we can only capture if we are the active window
	if (!isActive()) {
		return;
	}

	Window* current_capture = d_captureWindow;
	d_captureWindow = this;

	// inform any window which previously had capture that it doesn't anymore!
	if ((current_capture != NULL) && (current_capture != this) && (!d_restoreOldCapture)) {
		current_capture->onCaptureLost(EventArgs());
	}

	if (d_restoreOldCapture) {
		d_oldCapture = current_capture;
	}

	onCaptureGained(EventArgs());
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

	onCaptureLost(EventArgs());
}


/*************************************************************************
	Set whether this window will remember and restore the previous window
	that had inputs captured.
*************************************************************************/
void Window::setRestoreCaptue(bool setting)
{
	d_restoreOldCapture = setting;

	uint child_count = getChildCount();

	for (uint i = 0; i < child_count; ++i)
	{
		d_children[i]->setRestoreCaptue(setting);
	}

}


/*************************************************************************
	Set the current alpha value for this window.
*************************************************************************/
void Window::setAlpha(float alpha)
{
	d_alpha = alpha;
	onAlphaChanged(EventArgs());
}


/*************************************************************************
	Sets whether this Window will inherit alpha from its parent windows.
*************************************************************************/
void Window::setInheritsAlpha(bool setting)
{
	d_inheritsAlpha = setting;
	onInheritsAlphaChanged(EventArgs());
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
	return val / getParentWidth();
}


/*************************************************************************
	Convert the given Y co-ordinate from absolute to relative metrics.
*************************************************************************/
float Window::absoluteToRelativeY(float val) const
{
	return val / getParentHeight();
}


/*************************************************************************
	Convert the given position from absolute to relative metrics.
*************************************************************************/
Point Window::absoluteToRelative(const Point& pt) const
{
	Size parSze(getParentSize());

	return Point(pt.d_x / parSze.d_width, pt.d_y / parSze.d_height);
}


/*************************************************************************
	Convert the given size from absolute to relative metrics.
*************************************************************************/
Size Window::absoluteToRelative(const Size& sze) const
{
	Size parSze(getParentSize());

	return Size(sze.d_width / parSze.d_width, sze.d_height / parSze.d_height);
}


/*************************************************************************
	Convert the given area from absolute to relative metrics.
*************************************************************************/
Rect Window::absoluteToRelative(const Rect& rect) const
{
	Size parSze(getParentSize());

	return Rect(rect.d_left / parSze.d_width, rect.d_top / parSze.d_height, rect.d_right / parSze.d_width, rect.d_bottom / parSze.d_height);
}


/*************************************************************************
	Convert the given X co-ordinate from relative to absolute metrics.
*************************************************************************/
float Window::relativeToAbsoluteX(float val) const
{
	return val * getParentWidth();
}


/*************************************************************************
	Convert the given Y co-ordinate from relative to absolute metrics.
*************************************************************************/
float Window::relativeToAbsoluteY(float val) const
{
	return val * getParentHeight();
}


/*************************************************************************
	Convert the given position from relative to absolute metrics.
*************************************************************************/
Point Window::relativeToAbsolute(const Point& pt) const
{
	Size parSze(getParentSize());

	return Point(pt.d_x * parSze.d_width, pt.d_y * parSze.d_height);
}


/*************************************************************************
	Convert the given size from relative to absolute metrics.
*************************************************************************/
Size Window::relativeToAbsolute(const Size& sze) const
{
	Size parSze(getParentSize());

	return Size(sze.d_width * parSze.d_width, sze.d_height * parSze.d_height);
}


/*************************************************************************
	Convert the given area from relative to absolute metrics.
*************************************************************************/
Rect Window::relativeToAbsolute(const Rect& rect) const
{
	Size parSze(getParentSize());

	return Rect(rect.d_left * parSze.d_width, rect.d_top * parSze.d_height, rect.d_right * parSze.d_width, rect.d_bottom * parSze.d_height);
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
		wnd = wnd->getParent();
	}

	if (getMetricsMode() == Relative)
	{
		x = relativeToAbsoluteX(x);
	}

	return baseX + x;
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
		wnd = wnd->getParent();
	}

	if (getMetricsMode() == Relative)
	{
		y = relativeToAbsoluteX(y);
	}

	return baseY + y;
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
		wnd = wnd->getParent();
	}

	Point tmp;

	if (getMetricsMode() == Relative)
	{
		tmp = relativeToAbsolute(pt);
	}
	else
	{
		tmp = pt;
	}

	return base += tmp;
}


/*************************************************************************
	Convert a window size value, specified in whichever metrics mode is
	active, to a size in pixels.
*************************************************************************/
Size Window::windowToScreen(const Size& sze) const
{
	Size tmp(sze);

	if (getMetricsMode() == Relative)
	{
		tmp.d_width		*= d_abs_area.getWidth();
		tmp.d_height	*= d_abs_area.getHeight();
	}

	return tmp;
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
		wnd = wnd->getParent();
	}

	Rect tmp;

	if (getMetricsMode() == Relative)
	{
		tmp = relativeToAbsolute(rect);
	}
	else
	{
		tmp = rect;
	}

	return tmp.offset(base);
}


/*************************************************************************
	Convert a screen relative pixel co-ordinate value to a window
	co-ordinate value, specified in whichever metrics mode is active.
*************************************************************************/
float Window::screenToWindowX(float x) const
{
	if (d_parent != NULL)
	{
		x -= windowToScreenX(0);
	}

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
	if (d_parent != NULL)
	{
		y -= windowToScreenY(0);
	}

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

	if (d_parent != NULL)
	{
		tmp.d_x -= windowToScreenX(0);
		tmp.d_y -= windowToScreenY(0);
	}

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

	if (d_parent != NULL)
	{
		tmp.d_left		-= windowToScreenX(0);
		tmp.d_top		-= windowToScreenY(0);
		tmp.d_right		-= windowToScreenX(0);
		tmp.d_bottom	-= windowToScreenY(0);
	}

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
	if (d_parent == NULL)
	{
		return System::getSingleton().getRenderer()->getSize();
	}

	return Size(d_parent->d_abs_area.getWidth(), d_parent->d_abs_area.getHeight());
}


/*************************************************************************
	Add standard Window events
*************************************************************************/
void Window::addStandardEvents(void)
{
	// window events
	addEvent(SizedEvent);					addEvent(MovedEvent);					addEvent(TextChangedEvent);
	addEvent(FontChangedEvent);				addEvent(AlphaChangedEvent);			addEvent(IDChangedEvent);
	addEvent(ActivatedEvent);				addEvent(DeactivatedEvent);				addEvent(ShownEvent);
	addEvent(HiddenEvent);					addEvent(EnabledEvent);					addEvent(DisabledEvent);
	addEvent(MetricsChangedEvent);			addEvent(ClippingChangedEvent);			addEvent(ParentDestroyChangedEvent);
	addEvent(InheritsAlphaChangedEvent);	addEvent(AlwaysOnTopChangedEvent);		addEvent(CaptureGainedEvent);
	addEvent(CaptureLostEvent);				addEvent(RenderingStartedEvent);		addEvent(RenderingEndedEvent);
	addEvent(ChildAddedEvent);				addEvent(ChildRemovedEvent);			addEvent(DestructionStartedEvent);
	addEvent(ZChangedEvent);				addEvent(ParentSized);

	// general input handling
	addEvent(MouseEntersEvent);				addEvent(MouseLeavesEvent);				addEvent(MouseMoveEvent);
	addEvent(MouseWheelEvent);				addEvent(MouseButtonDownEvent);			addEvent(MouseButtonUpEvent);
	addEvent(MouseClickEvent);				addEvent(MouseDoubleClickEvent);		addEvent(MouseTripleClickEvent);
	addEvent(KeyDownEvent);					addEvent(KeyUpEvent);					addEvent(CharacterEvent);
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
	ChildList::reverse_iterator		position;
	if (wnd->isAlwaysOnTop())
	{
		position = d_children.rend();
	}
	else
	{
		// find last non-topmost window
		while ((position != d_children.rend()) && ((*position)->isAlwaysOnTop()))
		{
			++position;
		}

	}

	// add window (just behind 'pos')
	d_children.insert(position.base(), wnd);
	wnd->setParent(this);
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
		onZChanged(EventArgs());
	}
	else
	{
		EventArgs	nullArgs;

		uint child_count = getChildCount();

		for (uint i = 0; i < child_count; ++i)
		{
			d_parent->d_children[i]->onZChanged(nullArgs);
		}

	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Begin event triggers section

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

void Window::onSized(const EventArgs& e)
{
	// inform children their parent has been re-sized
	uint child_count = getChildCount();
	for (uint i = 0; i < child_count; ++i)
	{
		WindowEventArgs args(this);
		d_children[i]->onParentSized(args);
	}

	requestRedraw();

	fireEvent(SizedEvent, e);
}


void Window::onMoved(const EventArgs& e)
{
	requestRedraw();
	fireEvent(MovedEvent, e);
}


void Window::onTextChanged(const EventArgs& e)
{
	requestRedraw();
	fireEvent(TextChangedEvent, e);
}


void Window::onFontChanged(const EventArgs& e)
{
	requestRedraw();
	fireEvent(FontChangedEvent, e);
}


void Window::onAlphaChanged(const EventArgs& e)
{
	requestRedraw();
	fireEvent(AlphaChangedEvent, e);
}


void Window::onIDChanged(const EventArgs& e)
{
	fireEvent(IDChangedEvent, e);
}


void Window::onShown(const EventArgs& e)
{
	requestRedraw();
	fireEvent(ShownEvent, e);
}


void Window::onHidden(const EventArgs& e)
{
	requestRedraw();
	fireEvent(HiddenEvent, e);
}


void Window::onEnabled(const EventArgs& e)
{
	requestRedraw();
	fireEvent(EnabledEvent, e);
}


void Window::onDisabled(const EventArgs& e)
{
	requestRedraw();
	fireEvent(DisabledEvent, e);
}


void Window::onMetricsChanged(const EventArgs& e)
{
	fireEvent(MetricsChangedEvent, e);
}


void Window::onClippingChanged(const EventArgs& e)
{
	requestRedraw();
	fireEvent(ClippingChangedEvent, e);
}


void Window::onParentDestroyChanged(const EventArgs& e)
{
	fireEvent(ParentDestroyChangedEvent, e);
}


void Window::onInheritsAlphaChanged(const EventArgs& e)
{
	requestRedraw();
	fireEvent(InheritsAlphaChangedEvent, e);
}


void Window::onAlwaysOnTopChanged(const EventArgs& e)
{
	requestRedraw();
	fireEvent(AlwaysOnTopChangedEvent, e);
}


void Window::onCaptureGained(const EventArgs& e)
{
	fireEvent(CaptureGainedEvent, e);
}


void Window::onCaptureLost(const EventArgs& e)
{
	if (d_restoreOldCapture && (d_oldCapture != NULL)) {
		d_oldCapture->onCaptureLost(e);
		d_oldCapture = NULL;
	}

	fireEvent(CaptureLostEvent, e);
}


void Window::onRenderingStarted(const EventArgs& e)
{
	fireEvent(RenderingStartedEvent, e);
}


void Window::onRenderingEnded(const EventArgs& e)
{
	fireEvent(RenderingEndedEvent, e);
}


void Window::onZChanged(const EventArgs& e)
{
	requestRedraw();
	fireEvent(ZChangedEvent, e);
}


void Window::onDestructionStarted(const EventArgs& e)
{
	fireEvent(DestructionStartedEvent, e);
}


void Window::onActivated(const WindowEventArgs& e)
{
	d_active = true;
	requestRedraw();
	fireEvent(ActivatedEvent, e);
}


void Window::onDeactivated(const WindowEventArgs& e)
{
	d_active = false;
	requestRedraw();
	fireEvent(DeactivatedEvent, e);
}


void Window::onParentSized(const WindowEventArgs& e)
{
	// re-calculate relative rect
	d_rel_area = absoluteToRelative(d_abs_area);

	requestRedraw();
	fireEvent(ParentSized, e);
}


void Window::onChildAdded(const WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(ChildAddedEvent, e);
}


void Window::onChildRemoved(const WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(ChildRemovedEvent, e);
}


void Window::onMouseEnters(const MouseEventArgs& e)
{
	fireEvent(MouseEntersEvent, e);
}


void Window::onMouseLeaves(const MouseEventArgs& e)
{
	fireEvent(MouseLeavesEvent, e);
}


void Window::onMouseMove(const MouseEventArgs& e)
{
	fireEvent(MouseMoveEvent, e);
}


void Window::onMouseWheel(const MouseEventArgs& e)
{
	fireEvent(MouseWheelEvent, e);
}


void Window::onMouseButtonDown(const MouseEventArgs& e)
{
	fireEvent(MouseButtonDownEvent, e);
}


void Window::onMouseButtonUp(const MouseEventArgs& e)
{
	fireEvent(MouseButtonUpEvent, e);
}


void Window::onMouseClicked(const MouseEventArgs& e)
{
	fireEvent(MouseClickEvent, e);
}


void Window::onMouseDoubleClicked(const MouseEventArgs& e)
{
	fireEvent(MouseDoubleClickEvent, e);
}


void Window::onMouseTripleClicked(const MouseEventArgs& e)
{
	fireEvent(MouseTripleClickEvent, e);
}


void Window::onKeyDown(const KeyEventArgs& e)
{
	fireEvent(KeyDownEvent, e);
}


void Window::onKeyUp(const KeyEventArgs& e)
{
	fireEvent(KeyUpEvent, e);
}


void Window::onCharacter(const KeyEventArgs& e)
{
	fireEvent(CharacterEvent, e);
}


} // End of  CEGUI namespace section
