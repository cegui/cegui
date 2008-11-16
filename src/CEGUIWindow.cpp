/***********************************************************************
	filename: 	CEGUIWindow.cpp
	created:	21/2/2004
	author:		Paul D Turner

	purpose:	Implements the Window base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIMouseCursor.h"
#include "CEGUICoordConverter.h"
#include "CEGUIWindowRendererManager.h"
#include "elements/CEGUITooltip.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "falagard/CEGUIFalWidgetComponent.h"
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
WindowProperties::Alpha				Window::d_alphaProperty;
WindowProperties::AlwaysOnTop		Window::d_alwaysOnTopProperty;
WindowProperties::ClippedByParent	Window::d_clippedByParentProperty;
WindowProperties::DestroyedByParent	Window::d_destroyedByParentProperty;
WindowProperties::Disabled			Window::d_disabledProperty;
WindowProperties::Font				Window::d_fontProperty;
WindowProperties::ID				Window::d_IDProperty;
WindowProperties::InheritsAlpha		Window::d_inheritsAlphaProperty;
WindowProperties::MouseCursorImage	Window::d_mouseCursorProperty;
WindowProperties::RestoreOldCapture	Window::d_restoreOldCaptureProperty;
WindowProperties::Text				Window::d_textProperty;
WindowProperties::Visible			Window::d_visibleProperty;
WindowProperties::ZOrderChangeEnabled	Window::d_zOrderChangeProperty;
WindowProperties::WantsMultiClickEvents Window::d_wantsMultiClicksProperty;
WindowProperties::MouseButtonDownAutoRepeat Window::d_autoRepeatProperty;
WindowProperties::AutoRepeatDelay   Window::d_autoRepeatDelayProperty;
WindowProperties::AutoRepeatRate    Window::d_autoRepeatRateProperty;
WindowProperties::DistributeCapturedInputs Window::d_distInputsProperty;
WindowProperties::CustomTooltipType Window::d_tooltipTypeProperty;
WindowProperties::Tooltip           Window::d_tooltipProperty;
WindowProperties::InheritsTooltipText Window::d_inheritsTooltipProperty;
WindowProperties::RiseOnClick       Window::d_riseOnClickProperty;
WindowProperties::VerticalAlignment   Window::d_vertAlignProperty;
WindowProperties::HorizontalAlignment Window::d_horzAlignProperty;
WindowProperties::UnifiedAreaRect	Window::d_unifiedAreaRectProperty;
WindowProperties::UnifiedPosition	Window::d_unifiedPositionProperty;
WindowProperties::UnifiedXPosition	Window::d_unifiedXPositionProperty;
WindowProperties::UnifiedYPosition	Window::d_unifiedYPositionProperty;
WindowProperties::UnifiedSize		Window::d_unifiedSizeProperty;
WindowProperties::UnifiedWidth		Window::d_unifiedWidthProperty;
WindowProperties::UnifiedHeight		Window::d_unifiedHeightProperty;
WindowProperties::UnifiedMinSize	Window::d_unifiedMinSizeProperty;
WindowProperties::UnifiedMaxSize	Window::d_unifiedMaxSizeProperty;
WindowProperties::MousePassThroughEnabled   Window::d_mousePassThroughEnabledProperty;
WindowProperties::WindowRenderer    Window::d_windowRendererProperty;
WindowProperties::LookNFeel         Window::d_lookNFeelProperty;
WindowProperties::DragDropTarget    Window::d_dragDropTargetProperty;

/*************************************************************************
	static data definitions
*************************************************************************/
Window*	Window::d_captureWindow		= 0;


/*************************************************************************
	Event name constants
*************************************************************************/
const String Window::EventWindowUpdated ( "WindowUpdate" );
const String Window::EventParentSized( "ParentSized" );
const String Window::EventSized( "Sized" );
const String Window::EventMoved( "Moved" );
const String Window::EventTextChanged( "TextChanged" );
const String Window::EventFontChanged( "FontChanged" );
const String Window::EventAlphaChanged( "AlphaChanged" );
const String Window::EventIDChanged( "IDChanged" );
const String Window::EventActivated( "Activated" );
const String Window::EventDeactivated( "Deactivated" );
const String Window::EventShown( "Shown" );
const String Window::EventHidden( "Hidden" );
const String Window::EventEnabled( "Enabled" );
const String Window::EventDisabled( "Disabled" );
const String Window::EventClippedByParentChanged( "ClippingChanged" );
const String Window::EventDestroyedByParentChanged( "DestroyedByParentChanged" );
const String Window::EventInheritsAlphaChanged( "InheritAlphaChanged" );
const String Window::EventAlwaysOnTopChanged( "AlwaysOnTopChanged" );
const String Window::EventInputCaptureGained( "CaptureGained" );
const String Window::EventInputCaptureLost( "CaptureLost" );
const String Window::EventRenderingStarted( "StartRender" );
const String Window::EventRenderingEnded( "EndRender" );
const String Window::EventChildAdded( "AddedChild" );
const String Window::EventChildRemoved( "RemovedChild" );
const String Window::EventDestructionStarted( "DestructStart" );
const String Window::EventZOrderChanged( "ZChanged" );
const String Window::EventDragDropItemEnters("DragDropItemEnters");
const String Window::EventDragDropItemLeaves("DragDropItemLeaves");
const String Window::EventDragDropItemDropped("DragDropItemDropped");
const String Window::EventVerticalAlignmentChanged("VerticalAlignmentChanged");
const String Window::EventHorizontalAlignmentChanged("HorizontalAlignmentChanged");
const String Window::EventWindowRendererAttached("WindowRendererAttached");
const String Window::EventWindowRendererDetached("WindowRendererDetached");
const String Window::EventMouseEnters( "MouseEnter" );
const String Window::EventMouseLeaves( "MouseLeave" );
const String Window::EventMouseMove( "MouseMove" );
const String Window::EventMouseWheel( "MouseWheel" );
const String Window::EventMouseButtonDown( "MouseButtonDown" );
const String Window::EventMouseButtonUp( "MouseButtonUp" );
const String Window::EventMouseClick( "MouseClick" );
const String Window::EventMouseDoubleClick( "MouseDoubleClick" );
const String Window::EventMouseTripleClick( "MouseTripleClick" );
const String Window::EventKeyDown( "KeyDown" );
const String Window::EventKeyUp( "KeyUp" );
const String Window::EventCharacterKey( "CharacterKey" );

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String Window::TooltipNameSuffix( "__auto_tooltip__" );
const String Window::AutoWidgetNameSuffix( "__auto_" );


/*************************************************************************
	Constructor
*************************************************************************/
Window::Window(const String& type, const String& name) :
	d_type(type),
	d_name(name)
{
	// basic set-up
	d_parent		= 0;
	d_font			= 0;
	d_ID			= 0;
	d_alpha			= 1.0f;
	d_mouseCursor	= (const Image*)DefaultMouseCursor;
	d_userData		= 0;
	d_needsRedraw   = true;

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
    d_wantsMultiClicks  = true;
    d_distCapturedInputs = false;
    d_riseOnClick       = true;
    d_dragDropTarget    = true;

    // initialise mouse button auto-repeat state
    d_repeatButton = NoButton;
    d_autoRepeat   = false;
    d_repeating    = false;
    d_repeatDelay  = 0.3f;
    d_repeatRate   = 0.06f;

    // Tooltip setup
    d_customTip = 0;
    d_weOwnTip = false;
    d_inheritsTipText = false;

    // set initial min/max sizes.  These should normally be re-set in derived classes to something appropriate.
    d_minSize = UVector2(cegui_reldim(0), cegui_reldim(0));
    d_maxSize = UVector2(cegui_reldim(1), cegui_reldim(1));

    // set initial window area.
    d_area = URect(cegui_reldim(0), cegui_reldim(0), cegui_reldim(0), cegui_reldim(0));
    d_pixelSize = Size(0, 0);

    // set initial alignments
    d_horzAlign = HA_LEFT;
    d_vertAlign = VA_TOP;

    // initialisation flags
    d_initialising = false;
    d_destructionStarted = false;

    // event pass through
    d_mousePassThroughEnabled = false;

    // WindowRenderer
    d_windowRenderer = 0;

    // auto window flag
    d_autoWindow = (d_name.rfind(AutoWidgetNameSuffix) != String::npos);

    // allow writing XML flag
    d_allowWriteXML = true;

    // screen rect caching
    d_screenUnclippedRectValid = false;
    d_screenUnclippedInnerRectValid = false;
    d_screenRectValid = false;
    d_screenInnerRectValid = false;

    d_screenUnclippedRect = Rect(0,0,0,0);
    d_screenUnclippedInnerRect = Rect(0,0,0,0);
    d_screenRect = Rect(0,0,0,0);
    d_screenInnerRect = Rect(0,0,0,0);

	// add properties
	addStandardProperties();
}

/*************************************************************************
	Destructor
*************************************************************************/
Window::~Window(void)
{
    // cleanup events actually happened earlier.
}


/*************************************************************************
	return type of this window.
*************************************************************************/
const String& Window::getType(void) const
{
    return d_falagardType.empty() ? d_type : d_falagardType;
}


/*************************************************************************
	return true if the Window is currently disabled
*************************************************************************/
bool Window::isDisabled(bool localOnly) const
{
	bool parDisabled = ((d_parent == 0) || localOnly) ? false : d_parent->isDisabled();

	return (!d_enabled) || parDisabled;
}


/*************************************************************************
	return true if the Window is currently visible.
*************************************************************************/
bool Window::isVisible(bool localOnly) const
{
	bool parVisible = ((d_parent == 0) || localOnly) ? true : d_parent->isVisible();

	return d_visible && parVisible;
}


/*************************************************************************
	return true if this is the active Window
	(the window that receives inputs)
*************************************************************************/
bool Window::isActive(void) const
{
	bool parActive = (d_parent == 0) ? true : d_parent->isActive();

	return d_active && parActive;
}


/*************************************************************************
	returns whether a Window with the specified name is currently
	attached to this Window as a child.
*************************************************************************/
bool Window::isChild(const String& name) const
{
    size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        const String childName(d_children[i]->getName());

        if (childName == name ||
            childName == d_children[i]->d_windowPrefix + name)

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
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getID() == ID)
		{
			return true;
		}

	}

	return false;
}

/*************************************************************************
	returns whether at least one window with the given ID code is
	attached as a child to us or any of our children.
*************************************************************************/
bool Window::isChildRecursive(uint ID) const
{
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getID() == ID || d_children[i]->isChildRecursive(ID))
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
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
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
    size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        const String childName = d_children[i]->getName();
        // check if 'name' or 'prefix + name' is attached.
        if (childName == name || childName == d_children[i]->d_windowPrefix + name)
            return d_children[i];
    }

    throw UnknownObjectException("Window::getChild - The Window object named '"
        + name +"' is not attached to Window '" + d_name + "'.");
}

/***********************************************************************
	returns a pointer to a child window... searches recursively for it
	Use getChild() only. This function should only be used by getChild
	to find any child windows within it. This function does not throw any
	exceptions it will return NULL if nothing is found **WARNING**
************************************************************************/
Window* Window::recursiveChildSearch( const String& name ) const
{
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
	{
		String childName = d_children[i]->getName();
		//We need to check if the current name is available or if the name + prefix is available.. Hopefully not both
		if(childName == name || childName == d_children[i]->d_windowPrefix + name)
		{
			return d_children[i];
		}

	} // for (size_t i = 0; i < child_count; ++i)

	for(size_t i=0;i<child_count;i++)
	{
		Window* temp = d_children[i]->recursiveChildSearch(name);
		if(temp)
			return temp;
	} // for(size_t i=0;i<child_count;i++)

	return 0;
}


/*************************************************************************
	return a pointer to the first attached child window with the
	specified ID.
*************************************************************************/
Window* Window::getChild(uint ID) const
{
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
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

//----------------------------------------------------------------------------//
Window* Window::getChildRecursive(const String& name) const
{
    size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        const String childName = d_children[i]->getName();
        // check if 'name' or 'prefix + name' is attached
        if (childName == name || childName == d_children[i]->d_windowPrefix + name)
            return d_children[i];

        Window* tmp = d_children[i]->getChildRecursive(name);
        if (tmp)
            return tmp;
    }

    return 0;
}

/*************************************************************************
	return a pointer to the first attached child window with the
	specified ID. Recursive version.
*************************************************************************/
Window* Window::getChildRecursive(uint ID) const
{
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
	{
		if (d_children[i]->getID() == ID)
		{
			return d_children[i];
		}

		Window* tmp = d_children[i]->getChildRecursive(ID);
		if (tmp != 0)
        {
            return tmp;
        }
	}

	return 0;
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
		return 0;
	}

	size_t pos = getChildCount();

	while (pos-- > 0)
	{
		// don't need full backward scan for activeness as we already know 'this' is active
		// NB: This uses the draw-ordered child list, as that should be quicker in most cases.
		if (d_drawList[pos]->d_active)
			return d_drawList[pos]->getActiveChild();
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
	if (!d_parent)
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
	if (!d_parent)
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
	if (!d_parent)
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
Font* Window::getFont(bool useDefault) const
{
	if (!d_font)
	{
		return useDefault ? System::getSingleton().getDefaultFont() : 0;
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
	if ((d_parent == 0) || (!inheritsAlpha()))
	{
		return d_alpha;
	}

	return d_alpha * d_parent->getEffectiveAlpha();
}


/*************************************************************************
    return a Rect object describing the Window area in screen space.
*************************************************************************/
Rect Window::getPixelRect(void) const
{
    if (!d_screenRectValid)
    {
        d_screenRect = (d_windowRenderer != 0) ?
            d_windowRenderer->getPixelRect()
            : getPixelRect_impl();
        d_screenRectValid = true;
    }

    return d_screenRect;
}


/*************************************************************************
	return a Rect object describing the Window area in screen space.
*************************************************************************/
Rect Window::getPixelRect_impl(void) const
{
	// clip to parent?
	if (isClippedByParent() && (d_parent != 0))
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
    if (!d_screenInnerRectValid)
    {
        // clip to parent?
	    if (isClippedByParent() && (d_parent != 0))
	    {
		    d_screenInnerRect = getUnclippedInnerRect().getIntersection(d_parent->getInnerRect());
	    }
	    // else, clip to screen
	    else
	    {
		    d_screenInnerRect = getUnclippedInnerRect().getIntersection(
		        System::getSingleton().getRenderer()->getRect());
	    }
        d_screenInnerRectValid = true;
    }

    return d_screenInnerRect;
}


/*************************************************************************
	return a Rect object describing the Window area unclipped, in
	screen space.
*************************************************************************/
Rect Window::getUnclippedPixelRect(void) const
{
    if (!d_screenUnclippedRectValid)
    {
        Rect localArea(0, 0, d_pixelSize.d_width, d_pixelSize.d_height);
        d_screenUnclippedRect = CoordConverter::windowToScreen(*this, localArea);
        d_screenUnclippedRectValid = true;
    }

    return d_screenUnclippedRect;
}


/*************************************************************************
	Return a Rect object that describes, unclipped, the inner rectangle
	for this window.  The inner rectangle is typically an area that
	excludes some frame or other rendering that should not be touched by
	subsequent rendering.
*************************************************************************/
Rect Window::getUnclippedInnerRect(void) const
{
    if (!d_screenUnclippedInnerRectValid)
    {
        d_screenUnclippedInnerRect = getUnclippedInnerRect_impl();
        d_screenUnclippedInnerRectValid = true;
    }

    return d_screenUnclippedInnerRect;
}


/*************************************************************************
	Return a Rect object that describes, unclipped, the inner rectangle
	for this window.  The inner rectangle is typically an area that
	excludes some frame or other rendering that should not be touched by
	subsequent rendering.
*************************************************************************/
Rect Window::getUnclippedInnerRect_impl(void) const
{
    if (d_windowRenderer != 0)
    {
        return d_windowRenderer->getUnclippedInnerRect();
    }
    return getUnclippedPixelRect();
}


/*************************************************************************
	check if the given position would hit this window.
*************************************************************************/
bool Window::isHit(const Vector2& position) const
{
    // cannot be hit if we are disabled.
    if (isDisabled())
        return false;

    Rect clipped_area(getPixelRect());

    if (clipped_area.getWidth() == 0)
        return false;

    return clipped_area.isPointInRect(position);
}

/*************************************************************************
	return the child Window that is 'hit' by the given position
*************************************************************************/
Window* Window::getChildAtPosition(const Vector2& position) const
{
    ChildList::const_reverse_iterator	child, end;

    end = d_drawList.rend();

    for (child = d_drawList.rbegin(); child != end; ++child)
    {
        if ((*child)->isVisible())
        {
            // recursively scan children of this child windows...
            Window* wnd = (*child)->getChildAtPosition(position);

            // return window pointer if we found a 'hit' down the chain somewhere
            if (wnd)
                return wnd;
            // see if this child is hit and return it's pointer if it is
            else if ((*child)->isHit(position))
                return (*child);
        }
    }

    // nothing hit
    return 0;
}

/*************************************************************************
    return the child Window that is 'hit' by the given position, and
    does not have mouse pass through enabled.
*************************************************************************/
Window* Window::getTargetChildAtPosition(const Vector2& position) const
{
    ChildList::const_reverse_iterator   child, end;

    end = d_drawList.rend();

    for (child = d_drawList.rbegin(); child != end; ++child)
    {
        if ((*child)->isVisible())
        {
            // recursively scan children of this child windows...
            Window* wnd = (*child)->getTargetChildAtPosition(position);

            // return window pointer if we found a 'hit' down the chain somewhere
            if (wnd)
                return wnd;
            // see if this child is hit and return it's pointer if it is
            else if (!(*child)->isMousePassThroughEnabled() && (*child)->isHit(position))
                return (*child);
        }
    }

    // nothing hit
    return 0;
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
		if (d_parent)
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

        if (d_enabled)
        {
            // check to see if the window is actually enabled (which depends upon all ancestor windows being enabled)
            // we do this so that events we fire give an accurate indication of the state of a window.
            if ((d_parent && !d_parent->isDisabled()) || !d_parent)
                onEnabled(args);
        }
        else
        {
            onDisabled(args);
        }

        System::getSingleton().updateWindowContainingMouse();
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

        System::getSingleton().updateWindowContainingMouse();
	}

}


/*************************************************************************
	Activate the Window giving it input focus and bringing it to the top
	of all non always-on-top Windows.
*************************************************************************/
void Window::activate(void)
{
    // exit if the window is not visible, since a hidden window may not be the
    // active window.
    if (!isVisible())
        return;

	// force complete release of input capture.
	// NB: This is not done via releaseCapture() because that has
	// different behaviour depending on the restoreOldCapture setting.
	if ((d_captureWindow != 0) && (d_captureWindow != this))
	{
		Window* tmpCapture = d_captureWindow;
		d_captureWindow = 0;

		WindowEventArgs args(0);
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
	args.otherWindow = 0;
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
	Set the font used by this Window.
*************************************************************************/
void Window::setFont(Font* font)
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
		setFont(0);
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
	// dont add ourselves as a child
	// and dont add null windows
	if (window == this || window == 0)
	{
		return;
	}
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
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
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
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
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
    moveToFront_impl(false);
}


/*************************************************************************
	Implementation of move to front
*************************************************************************/
bool Window::moveToFront_impl(bool wasClicked)
{
    bool took_action = false;

	// if the window has no parent then we can have no siblings
	if (!d_parent)
	{
		// perform initial activation if required.
		if (!isActive())
		{
            took_action = true;
            ActivationEventArgs args(this);
			args.otherWindow = 0;
			onActivated(args);
		}

		return took_action;
	}

	// bring parent window to front of it's siblings
    took_action = wasClicked ? d_parent->doRiseOnClick() :
                               d_parent->moveToFront_impl(false);

    // get immediate child of parent that is currently active (if any)
    Window* activeWnd = getActiveSibling();

    // if a change in active window has occurred
    if (activeWnd != this)
    {
        took_action = true;

        // notify ourselves that we have become active
        ActivationEventArgs args(this);
        args.otherWindow = activeWnd;
        onActivated(args);

        // notify any previously active window that it is no longer active
        if (activeWnd)
        {
            args.window = activeWnd;
            args.otherWindow = this;
            args.handled = false;
            activeWnd->onDeactivated(args);
        }
    }

    // bring us to the front of our siblings
    if (d_zOrderingEnabled &&
        (!wasClicked || d_riseOnClick) &&
        !isTopOfZOrder())
    {
        took_action = true;

        // remove us from our parent's draw list
        d_parent->removeWindowFromDrawList(*this);
        // re-attach ourselves to our parent's draw list which will move us in front of
        // sibling windows with the same 'always-on-top' setting as we have.
        d_parent->addWindowToDrawList(*this);
        // notify relevant windows about the z-order change.
        onZChange_impl();
    }

    return took_action;
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
		args.otherWindow = 0;
		onDeactivated(args);
	}

    // we only need to proceed if we have a parent (otherwise we have no siblings)
    if (d_parent)
    {
        if (d_zOrderingEnabled)
        {
            // remove us from our parent's draw list
            d_parent->removeWindowFromDrawList(*this);
            // re-attach ourselves to our parent's draw list which will move us in behind
            // sibling windows with the same 'always-on-top' setting as we have.
            d_parent->addWindowToDrawList(*this, true);
            // notify relevant windows about the z-order change.
            onZChange_impl();
        }

        d_parent->moveToBack();
    }
}


/*************************************************************************
	Captures input to this window
*************************************************************************/
bool Window::captureInput(void)
{
    // we can only capture if we are the active window (LEAVE THIS ALONE!)
    if (!isActive())
        return false;

    if (d_captureWindow != this)
    {
        Window* current_capture = d_captureWindow;
        d_captureWindow = this;
        WindowEventArgs args(this);

        // inform any window which previously had capture that it doesn't anymore!
        if ((current_capture != 0) && (current_capture != this) && (!d_restoreOldCapture))
            current_capture->onCaptureLost(args);

        if (d_restoreOldCapture)
            d_oldCapture = current_capture;

        onCaptureGained(args);
    }

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
		if (d_oldCapture)
		{
			d_oldCapture = 0;
			d_captureWindow->moveToFront();
		}

	}
	else
	{
		d_captureWindow = 0;
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

	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
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
    d_needsRedraw = true;
	System::getSingleton().signalRedraw();
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
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
	{
		d_drawList[i]->render();
	}

	// signal rendering ended
	onRenderingEnded(args);
}


/*************************************************************************
    Perform the actual rendering for this Window.
*************************************************************************/
void Window::drawSelf(float z)
{
    if (d_needsRedraw)
    {
        // dispose of already cached imagery.
        d_renderCache.clearCachedImagery();
        // get derived class or WindowRenderer module to re-populate cache.
        if (d_windowRenderer != 0)
        {
            d_windowRenderer->render();
        }
        else
        {
            populateRenderCache();
        }
        // mark ourselves as no longer needed a redraw.
        d_needsRedraw = false;
    }

    // if render cache contains imagery.
    if (d_renderCache.hasCachedImagery())
    {
		Point absPos(getUnclippedPixelRect().getPosition());
        // calculate clipping area for this window
        Rect clipper(getPixelRect());
        // If window is not totally clipped.
        if (clipper.getWidth())
        {
            // send cached imagery to the renderer.
            d_renderCache.render(absPos, z, clipper);
        }
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
float Window::getParentPixelWidth(void) const
{
    return d_parent ?
           d_parent->d_pixelSize.d_width :
           System::getSingleton().getRenderer()->getWidth();
}


/*************************************************************************
	Return the pixel Height of the parent element.
	This always returns a valid number.
*************************************************************************/
float Window::getParentPixelHeight(void) const
{
    return d_parent ?
           d_parent->d_pixelSize.d_height:
           System::getSingleton().getRenderer()->getHeight();
}


/*************************************************************************
	Return the pixel size of the parent element.
	This always returns a valid object.
*************************************************************************/
Size Window::getParentPixelSize(void) const
{
	return getSize_impl(d_parent);
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
	if (wnd->getParent())
		wnd->getParent()->removeChildWindow(wnd);

    addWindowToDrawList(*wnd);

    // add window to child list
    d_children.push_back(wnd);

	// set the parent window
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
    // remove from draw list
    removeWindowFromDrawList(*wnd);

    // if window has children
    if (!d_children.empty())
    {
        // find this window in the child list
        ChildList::iterator	position = std::find(d_children.begin(), d_children.end(), wnd);

        // if the window was found in the child list
        if (position != d_children.end())
        {
            // remove window from child list
            d_children.erase(position);
            // reset windows parent so it's no longer this window.
            wnd->setParent(0);
        }
    }
}


/*************************************************************************
	Notify 'this' and all siblings of a ZOrder change event
*************************************************************************/
void Window::onZChange_impl(void)
{
	if (!d_parent)
	{
        WindowEventArgs args(this);
		onZChanged(args);
	}
	else
	{
		size_t child_count = d_parent->getChildCount();

		for (size_t i = 0; i < child_count; ++i)
		{
            WindowEventArgs args(d_parent->d_children[i]);
			d_parent->d_children[i]->onZChanged(args);
		}

	}

    System::getSingleton().updateWindowContainingMouse();
}


/*************************************************************************
	Return size of window.  If window is NULL return size of display.
*************************************************************************/
Size Window::getSize_impl(const Window* window) const
{
    return window ?
           window->d_pixelSize :
           System::getSingleton().getRenderer()->getSize();
}


/*************************************************************************
	Return a pointer to the mouse cursor image to use when the mouse is
	within this window.
*************************************************************************/
const Image* Window::getMouseCursor(bool useDefault) const
{
	if (d_mouseCursor != (const Image*)DefaultMouseCursor)
	{
		return d_mouseCursor;
	}
	else
	{
		return useDefault ? System::getSingleton().getDefaultMouseCursor() : 0;
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
    Helper method to fire off a mouse button down event.
*************************************************************************/
void Window::generateAutoRepeatEvent(MouseButton button)
{
    MouseEventArgs ma(this);
    ma.position = MouseCursor::getSingleton().getPosition();
    ma.moveDelta = Vector2(0.0f, 0.0f);
    ma.button = button;
    ma.sysKeys = System::getSingleton().getSystemKeys();
    ma.wheelChange = 0;
    onMouseButtonDown(ma);
}


/*************************************************************************
	Add standard CEGUI::Window properties.
*************************************************************************/
void Window::addStandardProperties(void)
{
	addProperty(&d_alphaProperty);
	addProperty(&d_alwaysOnTopProperty);
	addProperty(&d_clippedByParentProperty);
	addProperty(&d_destroyedByParentProperty);
	addProperty(&d_disabledProperty);
	addProperty(&d_fontProperty);
	addProperty(&d_IDProperty);
	addProperty(&d_inheritsAlphaProperty);
	addProperty(&d_mouseCursorProperty);
	addProperty(&d_restoreOldCaptureProperty);
	addProperty(&d_textProperty);
	addProperty(&d_visibleProperty);
	addProperty(&d_zOrderChangeProperty);
    addProperty(&d_wantsMultiClicksProperty);
    addProperty(&d_autoRepeatProperty);
    addProperty(&d_autoRepeatDelayProperty);
    addProperty(&d_autoRepeatRateProperty);
    addProperty(&d_distInputsProperty);
    addProperty(&d_tooltipTypeProperty);
    addProperty(&d_tooltipProperty);
    addProperty(&d_inheritsTooltipProperty);
    addProperty(&d_riseOnClickProperty);
    addProperty(&d_vertAlignProperty);
    addProperty(&d_horzAlignProperty);
    addProperty(&d_unifiedAreaRectProperty);
    addProperty(&d_unifiedPositionProperty);
    addProperty(&d_unifiedXPositionProperty);
    addProperty(&d_unifiedYPositionProperty);
    addProperty(&d_unifiedSizeProperty);
    addProperty(&d_unifiedWidthProperty);
    addProperty(&d_unifiedHeightProperty);
    addProperty(&d_unifiedMinSizeProperty);
    addProperty(&d_unifiedMaxSizeProperty);
    addProperty(&d_mousePassThroughEnabledProperty);
    addProperty(&d_windowRendererProperty);
    addProperty(&d_lookNFeelProperty);
    addProperty(&d_dragDropTargetProperty);

    // we ban some of these properties from xml for auto windows by default
    if (isAutoWindow())
    {
        banPropertyFromXML(&d_destroyedByParentProperty);
        banPropertyFromXML(&d_vertAlignProperty);
        banPropertyFromXML(&d_horzAlignProperty);
        banPropertyFromXML(&d_unifiedAreaRectProperty);
        banPropertyFromXML(&d_unifiedPositionProperty);
        banPropertyFromXML(&d_unifiedXPositionProperty);
        banPropertyFromXML(&d_unifiedYPositionProperty);
        banPropertyFromXML(&d_unifiedSizeProperty);
        banPropertyFromXML(&d_unifiedWidthProperty);
        banPropertyFromXML(&d_unifiedHeightProperty);
        banPropertyFromXML(&d_unifiedMinSizeProperty);
        banPropertyFromXML(&d_unifiedMaxSizeProperty);
        banPropertyFromXML(&d_windowRendererProperty);
        banPropertyFromXML(&d_lookNFeelProperty);
    }
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
    Return whether this window will receive multi-click events or
    multiple 'down' events instead.
*************************************************************************/
bool Window::wantsMultiClickEvents(void) const
{
    return d_wantsMultiClicks;
}


/*************************************************************************
    Set whether this window will receive multi-click events or
    multiple 'down' events instead.
*************************************************************************/
void Window::setWantsMultiClickEvents(bool setting)
{
    if (d_wantsMultiClicks != setting)
    {
        d_wantsMultiClicks = setting;

        // TODO: Maybe add a 'setting changed' event for this?
    }

}


/*************************************************************************
    Return whether mouse button down event autorepeat is enabled for
    this window.
*************************************************************************/
bool Window::isMouseAutoRepeatEnabled(void) const
{
    return d_autoRepeat;
}


/*************************************************************************
    Return the current auto-repeat delay setting for this window.
*************************************************************************/
float Window::getAutoRepeatDelay(void) const
{
    return d_repeatDelay;
}


/*************************************************************************
    Return the current auto-repeat rate setting for this window.
*************************************************************************/
float Window::getAutoRepeatRate(void) const
{
    return d_repeatRate;
}


/*************************************************************************
    Set whether mouse button down event autorepeat is enabled for this
    window.
*************************************************************************/
void Window::setMouseAutoRepeatEnabled(bool setting)
{
    if (d_autoRepeat != setting)
    {
        d_autoRepeat = setting;
        d_repeatButton = NoButton;

        // FIXME: There is a potential issue here if this setting is
        // FIXME: changed _while_ the mouse is auto-repeating, and
        // FIXME: the 'captured' state of input could get messed up.
        // FIXME: The alternative is to always release here, but that
        // FIXME: has a load of side effects too - so for now nothing
        // FIXME: is done.  This whole aspect of the system needs a
        // FIXME: review an reworking - though such a change was
        // FIXME: beyond the scope of the bug-fix that originated this
        // FIXME: comment block.  PDT - 30/10/06

        // TODO: Maybe add a 'setting changed' event for this?
    }

}


/*************************************************************************
    Set the current auto-repeat delay setting for this window.
*************************************************************************/
void Window::setAutoRepeatDelay(float delay)
{
    if (d_repeatDelay != delay)
    {
        d_repeatDelay = delay;

        // TODO: Maybe add a 'setting changed' event for this?
    }

}


/*************************************************************************
    Set the current auto-repeat rate setting for this window.
*************************************************************************/
void Window::setAutoRepeatRate(float rate)
{
    if (d_repeatRate != rate)
    {
        d_repeatRate = rate;

        // TODO: Maybe add a 'setting changed' event for this?
    }

}


/*************************************************************************
	Cause window to update itself and any attached children
*************************************************************************/
void Window::update(float elapsed)
{
	// perform update for 'this' Window
	updateSelf(elapsed);

	UpdateEventArgs e(this,elapsed);
	fireEvent(EventWindowUpdated,e,EventNamespace);

	// update child windows
	for (size_t i = 0; i < getChildCount(); ++i)
		d_children[i]->update(elapsed);
}


/*************************************************************************
    Perform actual update processing for this Window.
*************************************************************************/
void Window::updateSelf(float elapsed)
{
    // Mouse button autorepeat processing.
    if (d_autoRepeat && d_repeatButton != NoButton)
    {
        d_repeatElapsed += elapsed;

        if (d_repeating)
        {
            if (d_repeatElapsed > d_repeatRate)
            {
                d_repeatElapsed -= d_repeatRate;
                // trigger the repeated event
                generateAutoRepeatEvent(d_repeatButton);
            }
        }
        else
        {
            if (d_repeatElapsed > d_repeatDelay)
            {
                d_repeatElapsed = 0;
                d_repeating = true;
                // trigger the repeated event
                generateAutoRepeatEvent(d_repeatButton);
            }
        }
    }
}

bool Window::distributesCapturedInputs(void) const
{
    return d_distCapturedInputs;
}

void Window::setDistributesCapturedInputs(bool setting)
{
    if (d_distCapturedInputs != setting)
    {
        d_distCapturedInputs = setting;

        // TODO: Maybe add a 'setting changed' event for this?
    }
}

void Window::notifyDragDropItemEnters(DragContainer* item)
{
    if (item)
    {
        DragDropEventArgs args(this);
        args.dragDropItem = item;
        onDragDropItemEnters(args);
    }
}

void Window::notifyDragDropItemLeaves(DragContainer* item)
{
    if (item)
    {
        DragDropEventArgs args(this);
        args.dragDropItem = item;
        onDragDropItemLeaves(args);
    }
}

void Window::notifyDragDropItemDropped(DragContainer* item)
{
    if (item)
    {
        DragDropEventArgs args(this);
        args.dragDropItem = item;
        onDragDropItemDropped(args);
    }
}

void Window::destroy(void)
{
    // because we know that people do not read the API ref properly,
    // here is some protection to ensure that WindowManager does the
    // destruction and not anyone else.
    WindowManager& wmgr = WindowManager::getSingleton();

    if (wmgr.isWindowPresent(this->getName()))
    {
        wmgr.destroyWindow(this);

        // now return, the rest of what we need to do will happen
        // once WindowManager re-calls this method.
        return;
    }

    releaseInput();

    // let go of the tooltip if we have it
    Tooltip* tip = getTooltip();
    if (tip && tip->getTargetWindow()==this)
        tip->setTargetWindow(0);

    // ensure custom tooltip is cleaned up
    setTooltip(static_cast<Tooltip*>(0));

    // free any assigned WindowRenderer
    if (d_windowRenderer != 0)
    {
        d_windowRenderer->onDetach();
        WindowRendererManager::getSingleton().destroyWindowRenderer(d_windowRenderer);
        d_windowRenderer = 0;
    }

    // signal our imminent destruction
    WindowEventArgs args(this);
    onDestructionStarted(args);

    // double check we are detached from parent
    if (d_parent)
    {
        d_parent->removeChildWindow(this);
    }

    cleanupChildren();
}

bool Window::isUsingDefaultTooltip(void) const
{
    return d_customTip == 0;
}

Tooltip* Window::getTooltip(void) const
{
    return isUsingDefaultTooltip() ? System::getSingleton().getDefaultTooltip() : d_customTip;
}

void Window::setTooltip(Tooltip* tooltip)
{
    // destroy current custom tooltip if one exists and we created it
    if (d_customTip && d_weOwnTip)
        WindowManager::getSingleton().destroyWindow(d_customTip);

    // set new custom tooltip
    d_weOwnTip = false;
    d_customTip = tooltip;
}

void Window::setTooltipType(const String& tooltipType)
{
    // destroy current custom tooltip if one exists and we created it
    if (d_customTip && d_weOwnTip)
        WindowManager::getSingleton().destroyWindow(d_customTip);

    if (tooltipType.empty())
    {
        d_customTip = 0;
        d_weOwnTip = false;
    }
    else
    {
        try
        {
            d_customTip = static_cast<Tooltip*>(WindowManager::getSingleton().createWindow(tooltipType, getName() + TooltipNameSuffix));
            d_weOwnTip = true;
        }
        catch (UnknownObjectException&)
        {
            d_customTip = 0;
            d_weOwnTip = false;
        }
    }
}

String Window::getTooltipType(void) const
{
    return isUsingDefaultTooltip() ? String("") : d_customTip->getType();
}

void Window::setTooltipText(const String& tip)
{
    d_tooltipText = tip;

    Tooltip* tooltip = getTooltip();

    if (tooltip && tooltip->getTargetWindow() == this)
    {
        tooltip->setText(tip);
    }
}

const String& Window::getTooltipText(void) const
{
    if (d_inheritsTipText && d_parent && d_tooltipText.empty())
    {
        return d_parent->getTooltipText();
    }
    else
    {
        return d_tooltipText;
    }
}

bool Window::inheritsTooltipText(void) const
{
    return d_inheritsTipText;
}

void Window::setInheritsTooltipText(bool setting)
{
    if (d_inheritsTipText != setting)
    {
        d_inheritsTipText = setting;

        // TODO: Maybe add a 'setting changed' event for this?
    }
}

bool Window::doRiseOnClick(void)
{
    return moveToFront_impl(true);
}

void Window::setArea_impl(const UVector2& pos, const UVector2& size, bool topLeftSizing, bool fireEvents)
{
    // we make sure the screen areas are recached when this is called as we need it in most cases
    d_screenUnclippedRectValid = false;
    d_screenUnclippedInnerRectValid = false;
    d_screenRectValid = false;
    d_screenInnerRectValid = false;

    // notes of what we did
    bool moved = false, sized;

    // save original size so we can work out how to behave later on
    Size oldSize(d_pixelSize);

    // calculate pixel sizes for everything, so we have a common format for comparisons.
    Vector2 absMax(d_maxSize.asAbsolute(System::getSingleton().getRenderer()->getSize()));
    Vector2 absMin(d_minSize.asAbsolute(System::getSingleton().getRenderer()->getSize()));
    d_pixelSize = size.asAbsolute(getParentPixelSize()).asSize();

    // limit new pixel size to: minSize <= newSize <= maxSize
    if (d_pixelSize.d_width < absMin.d_x)
        d_pixelSize.d_width = absMin.d_x;
    else if (d_pixelSize.d_width > absMax.d_x)
        d_pixelSize.d_width = absMax.d_x;
    if (d_pixelSize.d_height < absMin.d_y)
        d_pixelSize.d_height = absMin.d_y;
    else if (d_pixelSize.d_height > absMax.d_y)
        d_pixelSize.d_height = absMax.d_y;

    d_area.setSize(size);
    sized = (d_pixelSize != oldSize);

    // If this is a top/left edge sizing op, only modify position if the size actually changed.
    // If it is not a sizing op, then position may always change.
    if (!topLeftSizing || sized)
    {
        // only update position if a change has occurred.
        if (pos != d_area.d_min)
        {
            d_area.setPosition(pos);
            moved = true;
        }
    }

    // fire events as required
    if (fireEvents)
    {
        WindowEventArgs args(this);

        if (moved)
        {
            onMoved(args);
            // reset handled so 'sized' event can re-use (since  wo do not care about it)
            args.handled = false;
        }

        if (sized)
        {
            onSized(args);
        }
    }

    if (moved || sized)
        System::getSingleton().updateWindowContainingMouse();
}

void Window::setArea(const UDim& xpos, const UDim& ypos, const UDim& width, const UDim& height)
{
    setArea(UVector2(xpos, ypos), UVector2(width, height));
}

void Window::setArea(const UVector2& pos, const UVector2& size)
{
    setArea_impl(pos, size);
}

void Window::setArea(const URect& area)
{
    setArea(area.d_min, area.getSize());
}

void Window::setPosition(const UVector2& pos)
{
    setArea_impl(pos, d_area.getSize());
}

void Window::setXPosition(const UDim& x)
{
    setArea_impl(UVector2(x, d_area.d_min.d_y), d_area.getSize());
}

void Window::setYPosition(const UDim& y)
{
    setArea_impl(UVector2(d_area.d_min.d_x, y), d_area.getSize());
}

void Window::setSize(const UVector2& size)
{
    setArea_impl(d_area.getPosition(), size);
}

void Window::setWidth(const UDim& width)
{
    setArea_impl(d_area.getPosition(), UVector2(width, d_area.getSize().d_y));
}

void Window::setHeight(const UDim& height)
{
    setArea_impl(d_area.getPosition(), UVector2(d_area.getSize().d_x, height));
}

void Window::setMaxSize(const UVector2& size)
{
    d_maxSize = size;

    // set window area back on itself to cause new maximum size to be applied if required.
    setArea(d_area);
}

void Window::setMinSize(const UVector2& size)
{
    d_minSize = size;

    // set window area back on itself to cause new minimum size to be applied if required.
    setArea(d_area);
}

const URect& Window::getArea() const
{
    return d_area;
}

const UVector2& Window::getPosition() const
{
    return d_area.d_min;
}

const UDim& Window::getXPosition() const
{
    return d_area.d_min.d_x;
}

const UDim& Window::getYPosition() const
{
    return d_area.d_min.d_y;
}

UVector2 Window::getSize() const
{
    return d_area.getSize();
}

UDim Window::getWidth() const
{
    return d_area.getSize().d_x;
}

UDim Window::getHeight() const
{
    return d_area.getSize().d_y;
}

const UVector2& Window::getMaxSize() const
{
    return d_maxSize;
}

const UVector2& Window::getMinSize() const
{
    return d_minSize;
}

void Window::setVerticalAlignment(const VerticalAlignment alignment)
{
    if (d_vertAlign != alignment)
    {
        d_vertAlign = alignment;

        WindowEventArgs args(this);
        onVerticalAlignmentChanged(args);
    }
}

void Window::setHorizontalAlignment(const HorizontalAlignment alignment)
{
    if (d_horzAlign != alignment)
    {
        d_horzAlign = alignment;

        WindowEventArgs args(this);
        onHorizontalAlignmentChanged(args);
    }
}

const String& Window::getLookNFeel() const
{
    return d_lookName;
}

void Window::setLookNFeel(const String& look)
{
    if (d_windowRenderer == 0)
    {
        throw NullObjectException("Window::setLookNFeel - There must be a window renderer assigned to the window '"+d_name+"' to set its look'n'feel");
    }

    WidgetLookManager& wlMgr = WidgetLookManager::getSingleton();
    if (!d_lookName.empty())
    {
        // Allow reset of look and feel
        // NOTE: If you want to prevent this, replace the following lines with the commented exception one
        d_windowRenderer->onLookNFeelUnassigned();
        const WidgetLookFeel& wlf = wlMgr.getWidgetLook(d_lookName);
        wlf.cleanUpWidget(*this);
        //throw AlreadyExistsException("Window::setLookNFeel - There is already a look'n'feel assigned to the window '"+d_name+"'");
    }
    d_lookName = look;
    Logger::getSingleton().logEvent("Assigning LookNFeel '" + look +"' to window '" + d_name + "'.", Informative);

    // Work to initialise the look and feel...
    const WidgetLookFeel& wlf = wlMgr.getWidgetLook(look);
    // Add property definitions, apply property initialisers and create child widgets.
    wlf.initialiseWidget(*this);
    // do the necessary binding to the stuff added by the look and feel
    initialiseComponents();
    // let the window renderer know about this
    d_windowRenderer->onLookNFeelAssigned();

    requestRedraw();
}

void Window::setModalState(bool state)
{
	bool already_modal = getModalState();

    // do nothing is state is'nt changing
	if (state != already_modal)
	{
	    // if going modal
	    if (state)
	    {
		    activate();
		    System::getSingleton().setModalTarget(this);
	    }
	    // clear the modal target
	    else
	    {
		    System::getSingleton().setModalTarget(0);
	    }
	}
}

void Window::performChildWindowLayout()
{
    if (!d_lookName.empty())
    {
        // here we just grab the look and feel and get it to layout its defined children
        try
        {
            const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
            // get look'n'feel to layout any child windows it created.
            wlf.layoutChildWidgets(*this);
        }
        catch (UnknownObjectException&)
        {
            Logger::getSingleton().logEvent("Window::performChildWindowLayout - assigned widget look was not found.", Errors);
        }
        if (d_windowRenderer != 0)
        {
            d_windowRenderer->performChildWindowLayout();
        }
    }
}

const String& Window::getUserString(const String& name) const
{
    UserStringMap::const_iterator iter = d_userStrings.find(name);

    if (iter != d_userStrings.end())
    {
        return (*iter).second;
    }
    else
    {
        throw UnknownObjectException("Window::getUserString - a user string named '" + name + "' has not been set for this Window.");
    }
}

bool Window::isUserStringDefined(const String& name) const
{
     return d_userStrings.find(name) != d_userStrings.end();
}

void Window::setUserString(const String& name, const String& value)
{
    d_userStrings[name] = value;
}

void Window::writeXMLToStream(XMLSerializer& xml_stream) const
{
    // just stop now if we are'nt allowed to write XML
    if (!d_allowWriteXML)
    {
        return;
    }

    // output opening Window tag
    xml_stream.openTag("Window")
        .attribute("Type", getType());
    // write name if not auto-generated
    if (getName().compare(0, WindowManager::GeneratedWindowNameBase.length(), WindowManager::GeneratedWindowNameBase) != 0)
    {
        xml_stream.attribute("Name", getName());
    }
    // write out properties.
    writePropertiesXML(xml_stream);
    // write out attached child windows.
    writeChildWindowsXML(xml_stream);
    // now ouput closing Window tag
    xml_stream.closeTag();
}

int Window::writePropertiesXML(XMLSerializer& xml_stream) const
{
    int propertiesWritten = 0;
    PropertySet::Iterator iter =  PropertySet::getIterator();

    while(!iter.isAtEnd())
    {
        // first we check to make sure the property is'nt banned from XML
        if (!isPropertyBannedFromXML(iter.getCurrentValue()))
        {
            try
            {
                // only write property if it's not at the default state
                if (!isPropertyAtDefault(iter.getCurrentValue()))
                {
                    iter.getCurrentValue()->writeXMLToStream(this, xml_stream);
                    ++propertiesWritten;
                }
            }
            catch (InvalidRequestException&)
            {
                // This catches error(s) from the MultiLineColumnList for example
                Logger::getSingleton().logEvent("Window::writePropertiesXML - property receiving failed. Continuing...", Errors);
            }
        }

        ++iter;
    }

    return propertiesWritten;
}

int Window::writeChildWindowsXML(XMLSerializer& xml_stream) const
{
    int windowsWritten = 0;

    for (uint i = 0; i < getChildCount(); ++i)
    {
        Window* child = d_children[i];

        // conditional to ensure that auto created windows are handled seperately.
        if (!child->isAutoWindow())
        {
            child->writeXMLToStream(xml_stream);
            ++windowsWritten;
        }
        // this is one of those auto created windows so we do some special handling
        else if (child->writeAutoChildWindowXML(xml_stream))
        {
            ++windowsWritten;
        }
    }

    return windowsWritten;
}

bool Window::writeAutoChildWindowXML(XMLSerializer& xml_stream) const
{

    // just stop now if we are'nt allowed to write XML
    if (!d_allowWriteXML)
    {
        return false;
    }
    // we temporarily output to this string stream to see if have to do the tag at all. // Make sure this stream does UTF-8
    std::ostringstream ss;
    XMLSerializer xml(ss);
    xml.openTag("AutoWindow");
    // Create the XML Child Tree
    // write out properties.
    writePropertiesXML(xml);
    // write out attached child windows.
    writeChildWindowsXML(xml);
    xml.closeTag();
    if (xml.getTagCount() <= 1)
    {
        return false;
    }
    // output opening AutoWindow tag
    xml_stream.openTag("AutoWindow");
    // extract the name suffix
    String suffix(getName(), getParent()->getName().length(), String::npos);
    // write name suffix attribute
    xml_stream.attribute("NameSuffix", suffix);
    // Inefficient : do the XML serialization again
    // write out properties.
    writePropertiesXML(xml_stream);
    // write out attached child windows.
    writeChildWindowsXML(xml_stream);
    xml_stream.closeTag();
    return true;
}

void Window::addWindowToDrawList(Window& wnd, bool at_back)
{
    // add behind other windows in same group
    if (at_back)
    {
         // calculate position where window should be added for drawing
        ChildList::iterator pos = d_drawList.begin();
        if (wnd.isAlwaysOnTop())
        {
            // find first topmost window
            while ((pos != d_drawList.end()) && (!(*pos)->isAlwaysOnTop()))
                ++pos;
        }
        // add window to draw list
        d_drawList.insert(pos, &wnd);
    }
    // add in front of other windows in group
    else
    {
        // calculate position where window should be added for drawing
        ChildList::reverse_iterator	position = d_drawList.rbegin();
        if (!wnd.isAlwaysOnTop())
        {
            // find last non-topmost window
            while ((position != d_drawList.rend()) && ((*position)->isAlwaysOnTop()))
                ++position;
        }
        // add window to draw list
        d_drawList.insert(position.base(), &wnd);
    }
}

void Window::removeWindowFromDrawList(const Window& wnd)
{
    // if draw list is not empty
    if (!d_drawList.empty())
    {
        // attempt to find the window in the draw list
        ChildList::iterator	position = std::find(d_drawList.begin(), d_drawList.end(), &wnd);

        // remove the window if it was found in the draw list
        if (position != d_drawList.end())
            d_drawList.erase(position);
    }
}

Window* Window::getActiveSibling()
{
    // initialise with this if we are active, else 0
    Window* activeWnd = isActive() ? this : 0;

    // if active window not already known, and we have a parent window
    if (!activeWnd && d_parent)
    {
        // scan backwards through the draw list, as this will
        // usually result in the fastest result.
        size_t idx = d_parent->getChildCount();
        while (idx-- > 0)
        {
            // if this child is active
            if (d_parent->d_drawList[idx]->isActive())
            {
                // set the return value
                activeWnd = d_parent->d_drawList[idx];
                // exit loop early, as we have found what we needed
                break;
            }
        }
    }

    // return whatever we discovered
    return activeWnd;
}

void Window::rename(const String& new_name)
{
    WindowManager& winMgr = WindowManager::getSingleton();
    /*
     * Client code should never call this, but again, since we know people do
     * not read and stick to the API reference, here is some built-in protection
     * which ensures that things are handled via the WindowManager anyway.
     */
    if (winMgr.isWindowPresent(d_name))
    {
        winMgr.renameWindow(this, new_name);
        // now we return, since the work was already done when WindowManager
        // re-called this function in the proper manner.
        return;
    }

    if (winMgr.isWindowPresent(new_name))
        throw AlreadyExistsException("Window::rename - Failed to rename Window: " +
                d_name + " as: " + new_name + ".  A Window named:" +
                new_name + "' already exists within the system.");

    // rename Falagard created child windows
    if (!d_lookName.empty())
    {
        const WidgetLookFeel& wlf =
                WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

        // get WidgetLookFeel to rename the children it created
        wlf.renameChildren(*this, new_name);
    }

    // how to detect other auto created windows.
    const String autoPrefix(d_name + AutoWidgetNameSuffix);
    // length of current name
    const size_t oldNameLength = d_name.length();

    // now rename all remaining auto-created windows attached
    for (size_t i = 0; i < getChildCount(); ++i)
    {
        // is this an auto created window that we created?
        if (!d_children[i]->d_name.compare(0, autoPrefix.length(), autoPrefix))
        {
            winMgr.renameWindow(d_children[i],
                                new_name +
                                d_children[i]->d_name.substr(oldNameLength));
        }
    }

    // log this under informative level
    Logger::getSingleton().logEvent("Renamed window: " + d_name +
                                    " as: " + new_name,
                                    Informative);

    // finally, set our new name
    d_name = new_name;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Begin event triggers section

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

void Window::onSized(WindowEventArgs& e)
{
    /*d_screenUnclippedRectValid = false;
    d_screenUnclippedInnerRectValid = false;
    d_screenRectValid = false;
    d_screenInnerRectValid = false;*/

	// inform children their parent has been re-sized
	size_t child_count = getChildCount();
	for (size_t i = 0; i < child_count; ++i)
	{
		WindowEventArgs args(this);
		d_children[i]->onParentSized(args);
	}

	performChildWindowLayout();

	requestRedraw();

	fireEvent(EventSized, e, EventNamespace);
}


void Window::onMoved(WindowEventArgs& e)
{
    /*d_screenUnclippedRectValid = false;
    d_screenUnclippedInnerRectValid = false;
    d_screenRectValid = false;
    d_screenInnerRectValid = false;*/

    // inform children their parent has been moved
	const size_t child_count = getChildCount();
	for (size_t i = 0; i < child_count; ++i)
	{
		d_children[i]->notifyScreenAreaChanged();
	}

    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
	fireEvent(EventMoved, e, EventNamespace);
}


void Window::onTextChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(EventTextChanged, e, EventNamespace);
}


void Window::onFontChanged(WindowEventArgs& e)
{
    // This was added to enable the Falagard FontDim to work
    // properly.  A better, more selective, solution would
    // probably be to do something funky with events ;)
    performChildWindowLayout();

	requestRedraw();
	fireEvent(EventFontChanged, e, EventNamespace);
}


void Window::onAlphaChanged(WindowEventArgs& e)
{
	// scan child list and call this method for all children that inherit alpha
	size_t child_count = getChildCount();

	for (size_t i = 0; i < child_count; ++i)
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
    // first deactivate window if it is the active window.
    if (isActive())
        deactivate();

	requestRedraw();
	fireEvent(EventHidden, e, EventNamespace);
}


void Window::onEnabled(WindowEventArgs& e)
{
    // signal all non-disabled children that they are now enabled (via inherited state)
    size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->d_enabled)
        {
            WindowEventArgs args(d_children[i]);
            d_children[i]->onEnabled(args);
        }
    }

    requestRedraw();
    fireEvent(EventEnabled, e, EventNamespace);
}


void Window::onDisabled(WindowEventArgs& e)
{
    // signal all non-disabled children that they are now disabled (via inherited state)
    size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->d_enabled)
        {
            WindowEventArgs args(d_children[i]);
            d_children[i]->onDisabled(args);
        }
    }

	requestRedraw();
	fireEvent(EventDisabled, e, EventNamespace);
}


void Window::onClippingChanged(WindowEventArgs& e)
{
	requestRedraw();
	notifyClippingChanged();
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
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
	fireEvent(EventAlwaysOnTopChanged, e, EventNamespace);
}


void Window::onCaptureGained(WindowEventArgs& e)
{
	fireEvent(EventInputCaptureGained, e, EventNamespace);
}


void Window::onCaptureLost(WindowEventArgs& e)
{
    // reset auto-repeat state
    d_repeatButton = NoButton;

	// handle restore of previous capture window as required.
	if (d_restoreOldCapture && (d_oldCapture != 0)) {
		d_oldCapture->onCaptureLost(e);
		d_oldCapture = 0;
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
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
	fireEvent(EventZOrderChanged, e, EventNamespace);
}


void Window::onDestructionStarted(WindowEventArgs& e)
{
    d_destructionStarted = true;
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
	size_t child_count = getChildCount();
	for (size_t i = 0; i < child_count; ++i)
	{
		if (d_children[i]->isActive())
		{
			// make sure the child gets itself as the .window member
			ActivationEventArgs child_e(d_children[i]);
			child_e.otherWindow = e.otherWindow;
			d_children[i]->onDeactivated(child_e);
		}

	}

	d_active = false;
	requestRedraw();
	fireEvent(EventDeactivated, e, EventNamespace);
}


void Window::onParentSized(WindowEventArgs& e)
{
    // set window area back on itself to cause minimum and maximum size
    // constraints to be applied as required.  (fire no events though)
    setArea_impl(d_area.getPosition(), d_area.getSize(), false, false);

    bool moved = ((d_area.d_min.d_x.d_scale != 0) || (d_area.d_min.d_y.d_scale != 0));
    bool sized = ((d_area.d_max.d_x.d_scale != 0) || (d_area.d_max.d_y.d_scale != 0));

    // now see if events should be fired.
    if (moved)
    {
        WindowEventArgs args(this);
        onMoved(args);
    }

    if (sized)
    {
        WindowEventArgs args(this);
        onSized(args);
    }

    // if we were not moved or sized, do child layout anyway!
    if (!(moved || sized))
        performChildWindowLayout();

    fireEvent(EventParentSized, e, EventNamespace);
}


void Window::onChildAdded(WindowEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
	fireEvent(EventChildAdded, e, EventNamespace);
}


void Window::onChildRemoved(WindowEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
	fireEvent(EventChildRemoved, e, EventNamespace);
}


void Window::onMouseEnters(MouseEventArgs& e)
{
	// set the mouse cursor
	MouseCursor::getSingleton().setImage(getMouseCursor());

    // perform tooltip control
    Tooltip* tip = getTooltip();
    if (tip)
    {
        tip->setTargetWindow(this);
    }

    fireEvent(EventMouseEnters, e, EventNamespace);
}


void Window::onMouseLeaves(MouseEventArgs& e)
{
    // perform tooltip control
    Tooltip* tip = getTooltip();
    if (tip)
    {
        tip->setTargetWindow(0);
    }

    fireEvent(EventMouseLeaves, e, EventNamespace);
}


void Window::onMouseMove(MouseEventArgs& e)
{
    // perform tooltip control
    Tooltip* tip = getTooltip();
    if (tip)
    {
        tip->resetTimer();
    }

    fireEvent(EventMouseMove, e, EventNamespace);
}


void Window::onMouseWheel(MouseEventArgs& e)
{
	fireEvent(EventMouseWheel, e, EventNamespace);
}


void Window::onMouseButtonDown(MouseEventArgs& e)
{
    // perform tooltip control
    Tooltip* tip = getTooltip();
    if (tip)
    {
        tip->setTargetWindow(0);
    }

    if (e.button == LeftButton)
    {
        e.handled |= doRiseOnClick();
    }

    // if auto repeat is enabled and we are not currently tracking
    // the button that was just pushed (need this button check because
    // it could be us that generated this event via auto-repeat).
    if (d_autoRepeat)
    {
        if (d_repeatButton == NoButton)
            captureInput();

        if ((d_repeatButton != e.button) && isCapturedByThis())
        {
            d_repeatButton = e.button;
            d_repeatElapsed = 0;
            d_repeating = false;
        }
    }

    fireEvent(EventMouseButtonDown, e, EventNamespace);
}


void Window::onMouseButtonUp(MouseEventArgs& e)
{
    // reset auto-repeat state
    if (d_autoRepeat && d_repeatButton != NoButton)
    {
        releaseInput();
        d_repeatButton = NoButton;
    }

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

void Window::onDragDropItemEnters(DragDropEventArgs& e)
{
    fireEvent(EventDragDropItemEnters, e, EventNamespace);
}

void Window::onDragDropItemLeaves(DragDropEventArgs& e)
{
    fireEvent(EventDragDropItemLeaves, e, EventNamespace);
}

void Window::onDragDropItemDropped(DragDropEventArgs& e)
{
    fireEvent(EventDragDropItemDropped, e, EventNamespace);
}

void Window::onVerticalAlignmentChanged(WindowEventArgs& e)
{
    fireEvent(EventVerticalAlignmentChanged, e, EventNamespace);
}

void Window::onHorizontalAlignmentChanged(WindowEventArgs& e)
{
    fireEvent(EventHorizontalAlignmentChanged, e, EventNamespace);
}

void Window::setWindowRenderer(const String& name)
{
    WindowRendererManager& wrm = WindowRendererManager::getSingleton();
    if (d_windowRenderer != 0)
    {
        // Allow reset of renderer
        // NOTE: If you want to prevent it, replace the following lines with the commented exception one
        if (d_windowRenderer->getName() == name)
        {
            return;
        }
        WindowEventArgs e(this);
        onWindowRendererDetached(e);
        wrm.destroyWindowRenderer(d_windowRenderer);
        //throw AlreadyExistsException("Window::setWindowRenderer - There is already a window renderer assigned to the window '"+d_name+"'");
    }

    if (!name.empty())
    {
        Logger::getSingleton().logEvent("Assigning the window renderer '"+name+"' to the window '"+d_name+"'", Informative);
        d_windowRenderer = wrm.createWindowRenderer(name);
        WindowEventArgs e(this);
        onWindowRendererAttached(e);
    }
    else
    {
        //d_windowRenderer = 0;
        throw InvalidRequestException("Window::setWindowRenderer - Tried to assign a 'null' window renderer");
    }
}

WindowRenderer* Window::getWindowRenderer(void) const
{
    return d_windowRenderer;
}

void Window::onWindowRendererAttached(WindowEventArgs& e)
{
    if (!validateWindowRenderer(d_windowRenderer->getClass()))
    {
        throw InvalidRequestException("The window renderer '"+d_windowRenderer->getName()+
        "' is not compatible with this widget ("+getType()+")");
    }
    if (!testClassName(d_windowRenderer->getClass()))
    {
        throw InvalidRequestException("The window renderer '"+d_windowRenderer->getName()+
        "' is not compatible with this widget ("+getType()+"). It requires a '"+d_windowRenderer->getClass()+"' based window type");
    }
    d_windowRenderer->d_window = this;
    d_windowRenderer->onAttach();
    fireEvent(EventWindowRendererAttached, e, EventNamespace);
}

void Window::onWindowRendererDetached(WindowEventArgs& e)
{
    d_windowRenderer->onDetach();
    d_windowRenderer->d_window = 0;
    fireEvent(EventWindowRendererDetached, e, EventNamespace);
}

bool Window::validateWindowRenderer(const String& name) const
{
    return true;
}

String Window::getWindowRendererName(void) const
{
    if (d_windowRenderer)
    {
        return d_windowRenderer->getName();
    }
    return String();
}

void Window::banPropertyFromXML(const Property* property)
{
    // check if the insertion failed
    if (!d_bannedXMLProperties.insert(property->getName()).second)
    {
        // just log the incidence
        AlreadyExistsException("Window::banPropertyFromXML - The property '"+property->getName()+"' is already banned in window '"+d_name+"'");
    }
}

bool Window::isPropertyBannedFromXML(const Property* property) const
{
    BannedXMLPropertySet::const_iterator i = d_bannedXMLProperties.find(property->getName());
    return (i != d_bannedXMLProperties.end());
}

bool Window::isPropertyAtDefault(const Property* property) const
{
    // if we have a looknfeel we examine it for defaults
    if (!d_lookName.empty())
    {
        // if we're an autowindow, we check our parent's looknfeel's Child section which
        // we came from as we might be initialised there
        if (d_autoWindow && getParent() && !getParent()->getLookNFeel().empty())
        {
            const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(getParent()->getLookNFeel());

            // find our name suffix
            String suffix(getName(), getParent()->getName().length(), String::npos);

            // find the widget component if possible
            const WidgetComponent* wc = wlf.findWidgetComponent(suffix);
            if (wc != 0)
            {
                const PropertyInitialiser* propinit = wc->findPropertyInitialiser(property->getName());
                if (propinit != 0)
                {
                    return (getProperty(property->getName()) == propinit->getInitialiserValue());
                }
            }
        }

        // if the looknfeel has a new default for this property we compare against that
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        const PropertyInitialiser* propinit = wlf.findPropertyInitialiser(property->getName());
        if (propinit != 0)
        {
            return (getProperty(property->getName()) == propinit->getInitialiserValue());
        }
    }
    // we dont have a looknfeel with a new value for this property so we rely on the hardcoded default
    return property->isDefault(this);
}

void Window::notifyClippingChanged(void)
{
    d_screenRectValid = false;
    d_screenInnerRectValid = false;
    // make sure everything gets redrawn now that clipping has changed.
    // NB: this way may not be not ideal, but invalidating the clipped rects
    // only did not have the desired effect.
    d_screenUnclippedInnerRectValid = false;

    // inform children that their clipped screen areas must be updated
    const size_t num = d_children.size();
    for (size_t i=0; i<num; ++i)
    {
        if (d_children[i]->isClippedByParent())
        {
            d_children[i]->notifyClippingChanged();
        }
    }
}

void Window::notifyScreenAreaChanged()
{
    d_screenUnclippedRectValid = false;
    d_screenUnclippedInnerRectValid = false;
    d_screenRectValid = false;
    d_screenInnerRectValid = false;

    // inform children that their screen area must be updated
	const size_t child_count = getChildCount();
	for (size_t i = 0; i < child_count; ++i)
	{
		d_children[i]->notifyScreenAreaChanged();
	}
}

EventSet::Iterator Window::getEventIterator() const
{
    return EventSet::getIterator();
}

PropertySet::Iterator Window::getPropertyIterator() const
{
    return PropertySet::getIterator();
}

bool Window::isDragDropTarget() const
{
    return d_dragDropTarget;
}

void Window::setDragDropTarget(bool setting)
{
    d_dragDropTarget = setting;
}

//-----------------------------------------------------------------------
void Window::setFalagardType(const String& type, const String& rendererType)
{
    // Retrieve the new widget look
    const String separator("/");
    String::size_type pos = type.find(separator);
    String newLook(type, 0, pos);

    // Check if old one is the same. If so, ignore since we don't need to do anything (type
    // is already assigned)
    pos = d_falagardType.find(separator);
    String oldLook(d_falagardType, 0, pos);
    if(oldLook == newLook)
        return;

    // Obtain widget kind
    String widget(d_falagardType, pos + 1);

    // Build new type (look/widget)
    d_falagardType = newLook + separator + widget;

    // Set new renderer
    if(rendererType.length() > 0)
        setWindowRenderer(rendererType);

    // Apply the new look to the widget
    setLookNFeel(type);
}

bool Window::isTopOfZOrder() const
{
    // if not attached, then always on top!
    if (!d_parent)
        return true;

    // get position of window at top of z-order in same group as this window
    ChildList::reverse_iterator pos = d_parent->d_drawList.rbegin();
    if (!d_alwaysOnTop)
    {
        // find last non-topmost window
        while ((pos != d_parent->d_drawList.rend()) && (*pos)->isAlwaysOnTop())
            ++pos;
    }

    // return whether the window at the top of the z order is us
    return *pos == this;
}

//----------------------------------------------------------------------------//
void Window::insertText(const String& text, const String::size_type position)
{
    d_text.insert(position, text);

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//
void Window::appendText(const String& text)
{
    d_text.append(text);

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
