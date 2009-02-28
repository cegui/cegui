/***********************************************************************
    filename:   CEGUIWindow.h
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Defines abstract base class for Window objects
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
#include "CEGUIUDim.h"
#include "CEGUIRenderCache.h"
#include "CEGUIWindowRenderer.h"
#include <vector>
#include <set>


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Enumerated type used when specifying vertical alignments.
 */
enum VerticalAlignment
{
    /**
     * Window's position specifies an offset of it's top edge from the top edge
     * of it's parent.
     */
    VA_TOP,
    /**
     * Window's position specifies an offset of it's vertical centre from the
     * vertical centre of it's parent.
     */
    VA_CENTRE,
    /**
     * Window's position specifies an offset of it's bottom edge from the
     * bottom edge of it's parent.
     */
    VA_BOTTOM
};

/*!
\brief
    Enumerated type used when specifying horizontal alignments.
 */
enum HorizontalAlignment
{
    /**
     * Window's position specifies an offset of it's left edge from the left
     * edge of it's parent.
     */
    HA_LEFT,
    /**
     * Window's position specifies an offset of it's horizontal centre from the
     * horizontal centre of it's parent.
     */
    HA_CENTRE,
    /**
     * Window's position specifies an offset of it's right edge from the right
     * edge of it's parent.
     */
    HA_RIGHT
};

/*!
\brief
    An abstract base class providing common functionality and specifying the
    required interface for derived classes.

    The Window base class is core UI object class that the the system knows
    about; for this reason, every other window, widget, or similar item within
    the system must be derived from Window.

    The base class provides the common functionality required by all UI objects,
    and specifies the minimal interface required to be implemented by derived
    classes.
*/
class CEGUIEXPORT Window : public PropertySet, public EventSet
{
public:
    /*************************************************************************
        Event name constants
    *************************************************************************/
    //!< Namespace for global events
    static const String EventNamespace;

    // generated internally by Window
	static const String EventWindowUpdated; //!< Event to signal the window is being updated. Used by lua system!
    //! Parent of this Window has been re-sized.
    static const String EventParentSized;
    //! Window size has changed
    static const String EventSized;
    //! Window position has changed
    static const String EventMoved;
    //! Text string for the Window has changed
    static const String EventTextChanged;
    //!Font object for the Window has been changed
    static const String EventFontChanged;
    //! Alpha blend value for the Window has changed
    static const String EventAlphaChanged;
    //! Client assigned ID code for the Window has changed
    static const String EventIDChanged;
    //! Window has been activated (has input focus)
    static const String EventActivated;
    //! Window has been deactivated (loses input focus)
    static const String EventDeactivated;
    //! Window has been made visible
    static const String EventShown;
    //! Window has been hidden from view
    static const String EventHidden;
    //! Window has been enabled (interaction is possible)
    static const String EventEnabled;
    //! Window has been disabled (interaction is no longer possible)
    static const String EventDisabled;
    //! Clipping by parent mode has been modified
    static const String EventClippedByParentChanged;
    //! Destruction by parent mode has been modified
    static const String EventDestroyedByParentChanged;
    //! Alpha inherited from parent mode has been modified.
    static const String EventInheritsAlphaChanged;
    //! Always on top mode has been modified
    static const String EventAlwaysOnTopChanged;
    //! Window has captured all inputs
    static const String EventInputCaptureGained;
    //! Window has lost it's capture on inputs
    static const String EventInputCaptureLost;
    //! Rendering of the Window has started
    static const String EventRenderingStarted;
    //! Rendering for the Window has finished
    static const String EventRenderingEnded;
    //! A child Window has been added
    static const String EventChildAdded;
    //! A child window has been removed
    static const String EventChildRemoved;
    //! Destruction of the Window is about to begin.
    static const String EventDestructionStarted;
    //! The z-order of the window has changed
    static const String EventZOrderChanged;
    //! A DragContainer has been dragged over this window.
    static const String EventDragDropItemEnters;
    //! A DragContainer has left this window.
    static const String EventDragDropItemLeaves;
    //! A DragContainer was dropped on this Window.
    static const String EventDragDropItemDropped;
    //! The vertical alignment of the window has changed.
    static const String EventVerticalAlignmentChanged;
    //! The vertical alignment of the window has changed.
    static const String EventHorizontalAlignmentChanged;
    //! The a new window renderer was attached.
    static const String EventWindowRendererAttached;
    //! The currently assigned window renderer was detached.
    static const String EventWindowRendererDetached;

    // generated externally (inputs)
    //! Mouse cursor has entered the Window.
    static const String EventMouseEnters;
    //! Mouse cursor has left the Window.
    static const String EventMouseLeaves;
    //! Mouse cursor was moved within the area of the Window.
    static const String EventMouseMove;
    //! Mouse wheel was scrolled within the Window.
    static const String EventMouseWheel;
    //! A mouse button was pressed down within the Window.
    static const String EventMouseButtonDown;
    //! A mouse button was released within the Window.
    static const String EventMouseButtonUp;
    //! A mouse button was clicked (down then up) within the Window.
    static const String EventMouseClick;
    //! A mouse button was double-clicked within the Window.
    static const String EventMouseDoubleClick;
    //! A mouse button was triple-clicked within the Window.
    static const String EventMouseTripleClick;
    //! A key on the keyboard was pressed.
    static const String EventKeyDown;
    //! A key on the keyboard was released.
    static const String EventKeyUp;
    //! A text character was typed on the keyboard.
    static const String EventCharacterKey;

    /*************************************************************************
        Child Widget name suffix constants
    *************************************************************************/
    //! Widget name suffix for automatically created tooltip widgets.
    static const String TooltipNameSuffix;
    //! Something that all generated widgets will have in their names.
    static const String AutoWidgetNameSuffix;


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
    const String& getType(void) const;

    /*!
    \brief
        return a String object holding the name of this Window.

    \return
        String object holding the unique Window name.
    */
    const String& getName(void) const  {return d_name;}

	/**
	 * Return a string to the window prefix
	 * \return String object holding the prefix of this window
	 */
	const String& getPrefix(void) const {return d_windowPrefix;}

    /*!
    \brief
        returns whether or not this Window is set to be destroyed when its
        parent window is destroyed.

    \return
        - true if the Window will be destroyed when its parent is destroyed.
        - false if the Window will remain when its parent is destroyed.
    */
    bool    isDestroyedByParent(void) const    {return d_destroyedByParent;}

    /*!
    \brief
        returns whether or not this Window is an always on top Window.  Also
        known as a top-most window.

    \return
        - true if this Window is always drawn on top of other normal windows.
        - false if the Window has normal z-order behaviour.
    */
    bool isAlwaysOnTop(void) const  {return d_alwaysOnTop;}

    /*!
    \brief
        return whether the Window is currently disabled

    \param localOnly
        States whether to only return the state set for this window, and not to
        factor in inherited state from ancestor windows.

    \return
        - true if the window is disabled.
        - false if the window is enabled.
    */
    bool isDisabled(bool localOnly = false) const;

    /*!
    \brief
        return true if the Window is currently visible.

        When true is returned from this function does not mean that the window
        is not completely obscured by other windows, just that the window will
        be processed when rendering, and is not explicitly marked as hidden.

    \param localOnly
        States whether to only return the state set for this window, and not to
        factor in inherited state from ancestor windows.

    \return
        - true if the window will be drawn.
        - false if the window is hidden and therefore ignored when rendering.
    */
    bool isVisible(bool localOnly = false) const;

    /*!
    \brief
        return true if this is the active Window.  An active window is a window
        that may receive user inputs.

        Mouse events are always sent to the window containing the mouse cursor
        regardless of what this function reports (unless a window has captured
        inputs).  The active state mainly determines where send other, for
        example keyboard, inputs.

    \return
        - true if the window is active and may be sent inputs by the system.
        - false if the window is inactive and will not be sent inputs.
    */
    bool isActive(void) const;

    /*!
    \brief
        return true if this Window is clipped so that its rendering will not
        pass outside of its parent Window area.

    \return
        - true if the window will be clipped by its parent Window.
        - false if the windows rendering may pass outside its parents area
    */
    bool isClippedByParent(void) const {return d_clippedByParent;}

    /*!
    \brief
        return the ID code currently assigned to this Window by client code.

    \return
        uint value equal to the currently assigned ID code for this Window.
    */
    uint getID(void) const {return d_ID;}

    /*!
    \brief
        return the number of child Window objects currently attached to this
        Window.

    \return
        size_t value equal to the number of Window objects directly attached
        to this Window as children.
    */
    size_t getChildCount(void) const  {return d_children.size();}

    /*!
    \brief
        returns whether a Window with the specified name is currently attached
        to this Window as a child.

    \param name
        String object containing the name of the Window to look for.

    \return
        - true if a Window named \a name is currently attached to this Window.
        - false if no such child Window is attached.
    */
    bool isChild(const String& name) const;

    /*!
    \brief
        returns whether at least one window with the given ID code is attached
        to this Window as a child.

    \note
        ID codes are client assigned and may or may not be unique, and as such,
        the return from this function will only have meaning to the client code.

    \param ID
        uint ID code to look for.

    \return
        - true if at least one child window was found with the ID code \a ID
        - false if no child window was found with the ID code \a ID.
    */
    bool isChild(uint ID) const;

    /*!
    \brief
        returns whether at least one window with the given ID code is attached
        to this Window or any of it's children as a child.

    \note
        ID codes are client assigned and may or may not be unique, and as such,
        the return from this function will only have meaning to the client code.

        WARNING! This function can be very expensive and should only be used
        when you have no other option available. If you decide to use it anyway,
        make sure the window hierarchy from the entry point is small.

    \param ID
        uint ID code to look for.

    \return
        - true if at least one child window was found with the ID code \a ID
        - false if no child window was found with the ID code \a ID.
    */
    bool isChildRecursive(uint ID) const;

    /*!
    \brief
        return true if the given Window is a child of this window.

    \param window
        Pointer to the Window object to look for.

    \return
        - true if Window object \a window is attached to this window as a child.
        - false if Window object \a window is not a child of this Window.
    */
    bool isChild(const Window* window) const;

    /*!
    \brief
        return a pointer to the child window with the specified name.

        This function will throw an exception if no child object with the given
        name is attached.  This decision was made (over returning NULL if no
        window was found) so that client code can assume that if the call
        returns it has a valid window pointer.  We provide the isChild()
        functions for checking if a given window is attached.

    \param name
        String object holding the name of the child window for which a pointer
        is to be returned.

    \return
        Pointer to the Window object attached to this window that has the name
        \a name.

    \exception UnknownObjectException
        thrown if no window named \a name is attached to this Window.
    */
    Window* getChild(const String& name) const;

    /*!
    \brief
        This is / was intended for internal use only (should have had protected
        visibility).  This function is deprecated - do not use this function at
        all, ever.  The only reason this function is still here is to maintain
        ABI compatibility for the 0.6.x series of releases.  This function will
        be gone in 0.7.0.
    */
	Window* recursiveChildSearch(const String& name) const;

    /*!
    \brief
        return a pointer to the first attached child window with the specified
        ID value.

        This function will throw an exception if no child object with the given
        ID is attached.  This decision was made (over returning NULL if no
        window was found) so that client code can assume that if the call
        returns it has a valid window pointer.  We provide the isChild()
        functions for checking if a given window is attached.

    \param ID
        uint value specifying the ID code of the window to return a pointer to.

    \return
        Pointer to the (first) Window object attached to this window that has
        the ID code \a ID.

    \exception UnknownObjectException
        thrown if no window with the ID code \a ID is attached to this Window.
    */
    Window* getChild(uint ID) const;

    /*!
    \brief
        return a pointer to the first attached child window with the specified
        name. Children are traversed recursively.

        Contrary to the non recursive version of this function, this one will
        not throw an exception, but return 0 in case no child was found.

    \note
        WARNING! This function can be very expensive and should only be used
        when you have no other option available. If you decide to use it anyway,
        make sure the window hierarchy from the entry point is small.

    \param name
        String object holding the name of the child window for which a pointer
        is to be returned.

    \return
        Pointer to the Window object attached to this window that has the name
        \a name.

        If no child is found with the name \a name, 0 is returned.
    */
    Window* getChildRecursive(const String& name) const;

    /*!
    \brief
        return a pointer to the first attached child window with the specified
        ID value. Children are traversed recursively.

        Contrary to the non recursive version of this function, this one will
        not throw an exception, but return 0 in case no child was found.

    \note
        WARNING! This function can be very expensive and should only be used
        when you have no other option available. If you decide to use it anyway,
        make sure the window hierarchy from the entry point is small.

    \param ID
        uint value specifying the ID code of the window to return a pointer to.

    \return
        Pointer to the (first) Window object attached to this window that has
        the ID code \a ID.
        If no child is found with the ID code \a ID, 0 is returned.
    */
    Window* getChildRecursive(uint ID) const;

    /*!
    \brief
        return a pointer to the child window that is attached to 'this' at the
        given index.

    \param idx
        Index of the child window whos pointer should be returned.  This value
        is not bounds checked, client code should ensure that this is less than
        the value returned by getChildCount().

    \return
        Pointer to the child window currently attached at index position \a idx
    */
    Window* getChildAtIdx(size_t idx) const {return d_children[idx];}

    /*!
    \brief
        return a pointer to the Window that currently has input focus starting
        with this Window.

    \return
        Pointer to the window that is active (has input focus) starting at this
        window.  The function will return 'this' if this Window is active
        and either no children are attached or if none of the attached children
        are active.  Returns NULL if this Window (and therefore all children)
        are not active.
    */
    Window* getActiveChild(void);
    const Window* getActiveChild(void) const;

    /*!
    \brief
        return true if the specified Window is some ancestor of this Window

    \param name
        String object holding the name of the Window to check for.

    \return
        - true if a Window named \a name is an ancestor (parent, or parent of
          parent, etc) of this Window.
        - false if a Window named \a name is in no way an ancestor of this
          window.
    */
    bool isAncestor(const String& name) const;

    /*!
    \brief
        return true if any Window with the given ID is some ancestor of this
        Window.

    \param ID
        uint value specifying the ID to look for.

    \return
        - true if an ancestor (parent, or parent of parent, etc) was found with
          the ID code \a ID.
        - false if no ancestor window has the ID code \a ID.
    */
    bool isAncestor(uint ID) const;

    /*!
    \brief
        return true if the specified Window is some ancestor of this Window.

    \param window
        Pointer to the Window object to look for.

    \return
        - true if \a window was found to be an ancestor (parent, or parent of
          parent, etc) of this Window.
        - false if \a window is not an ancestor of this window.
    */
    bool isAncestor(const Window* window) const;

    /*!
    \brief
        return the active Font object for the Window.

    \param useDefault
        Sepcifies whether to return the default font if this Window has no
        preferred font set.

    \return
        Pointer to the Font being used by this Window.  If the window has no
        assigned font, and \a useDefault is true, then the default system font
        is returned.
    */
    Font* getFont(bool useDefault = true) const;

    /*!
    \brief
        return the current text for the Window

    \return
        The String object that holds the current text for this Window.
    */
    const String& getText(void) const   {return d_text;}

    /*!
    \brief
        return true if the Window inherits alpha from its parent(s).

    \return
        - true if the Window inherits alpha from its parent(s)
        - false if the alpha for this Window is independant from its parents.
    */
    bool inheritsAlpha(void) const  {return d_inheritsAlpha;}

    /*!
    \brief
        return the current alpha value set for this Window

    \note
        The alpha value set for any given window may or may not be the final
        alpha value that is used when rendering.  All window objects, by
        default, inherit alpha from thier parent window(s) - this will blend
        child windows, relatively, down the line of inheritance.  This behaviour
        can be overridden via the setInheritsAlpha() method.  To return the true
        alpha value that will be applied when rendering, use the
        getEffectiveAlpha() method.

    \return
        the currently set alpha value for this Window.  The value returned Will
        be between 0.0f and 1.0f.
    */
    float getAlpha(void) const  {return d_alpha;}

    /*!
    \brief
        return the effective alpha value that will be used when rendering this
        window, taking into account inheritance of parent window(s) alpha.

    \return
        the effective alpha that will be applied to this Window when rendering.
        The value returned Will be between 0.0f and 1.0f.
    */
    float getEffectiveAlpha(void) const;

    /*!
    \brief
        return a Rect object describing the Window area in screen space.

    \return
        Rect object that describes the area covered by the Window.  The values
        in the returned Rect are in screen pixels.  The returned Rect is clipped
        as appropriate and depending upon the 'ClippedByParent' setting.

    \note
        This has now been made virtual to ease some customisations that require
        more specialised clipping requirements.
    */
    Rect getPixelRect(void) const;

    /*!
    \brief
        return a Rect object describing the Window area in screen space.

    \return
        Rect object that describes the area covered by the Window.  The values
        in the returned Rect are in screen pixels.  The returned Rect is clipped
        as appropriate and depending upon the 'ClippedByParent' setting.

    \note
        This has now been made virtual to ease some customisations that require
        more specialised clipping requirements.
    */
    virtual Rect getPixelRect_impl(void) const;

    /*!
    \brief
        return a Rect object describing the clipped inner area for this window.

    \return
        Rect object that describes, in appropriately clipped screen pixel
        co-ordinates, the window object's inner rect area.
    */
    Rect getInnerRect(void) const;

    /*!
    \brief
        return a Rect object describing the Window area unclipped, in screen
        space.

    \return
        Rect object that describes the area covered by the Window.  The values
        in the returned Rect are in screen pixels.  The returned rect is fully
        unclipped.
    */
    Rect getUnclippedPixelRect(void) const;

    /*!
    \brief
        Return a Rect object that describes, unclipped, the inner rectangle for
        this window.  The inner rectangle is typically an area that excludes
        some frame or other rendering that should not be touched by subsequent
        rendering.

    \return
        Rect object that describes, in unclipped screen pixel co-ordinates, the
        window object's inner rect area.
    */
    Rect getUnclippedInnerRect(void) const;

    /*!
    \brief
        Return a Rect object that describes, unclipped, the inner rectangle for
        this window.  The inner rectangle is typically an area that excludes
        some frame or other rendering that should not be touched by subsequent
        rendering.

    \return
        Rect object that describes, in unclipped screen pixel co-ordinates, the
        window object's inner rect area.
    */
    virtual Rect getUnclippedInnerRect_impl(void) const;

    /*!
    \brief
        return the Window that currently has inputs captured.

    \return
        Pointer to the Window object that currently has inputs captured, or NULL
        if no Window has captured input.
    */
    static Window* getCaptureWindow(void)   {return d_captureWindow;}

    /*!
    \brief
        return true if this Window has input captured.

    \return
        - true if this Window has captured inputs.
        - false if some other Window, or no Window, has captured inputs.
    */
    bool isCapturedByThis(void) const   {return getCaptureWindow() == this;}

    /*!
    \brief
        return true if an ancestor window has captured inputs.

    \return
        - true if input is captured by a Window that is some ancestor (parent,
          parent of parent, etc) of this Window.
        - false if no ancestor of this window has captured input.
    */
    bool isCapturedByAncestor(void) const
         {return isAncestor(getCaptureWindow());}

    /*!
    \brief
        return true if a child window has captured inputs.

    \return
        - true if input is captured by a Window that is a child of this Window.
        - false if no child of this window has not captured input.
    */
    bool isCapturedByChild(void) const  {return isChild(getCaptureWindow());}

    /*!
    \brief
        check if the given pixel position would hit this window.

    \param position
        Vector2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \return
        - true if \a position hits this Window.
        - false if \a position does not hit this window.
    */
    virtual bool isHit(const Vector2& position) const;

    /*!
    \brief
        return the child Window that is hit by the given pixel position

    \param position
        Vector2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \return
        Pointer to the child Window that was hit according to the location
        \a position, or 0 if no child of this window was hit.
    */
    Window* getChildAtPosition(const Vector2& position) const;

    /*!
    \brief
        return the child Window that is 'hit' by the given position, and is
        allowed to handle mouse events.

    \param position
        Vector2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \return
        Pointer to the child Window that was hit according to the location
        \a position, or 0 if no child of this window was hit.
    */
    Window* getTargetChildAtPosition(const Vector2& position) const;

    /*!
    \brief
        return the parent of this Window.

    \return
        Pointer to the Window object that is the parent of this Window.
        This value can be NULL, in which case the Window is a GUI sheet / root.
    */
    Window* getParent(void) const   {return d_parent;}

    /*!
    \brief
        Return a pointer to the mouse cursor image to use when the mouse cursor
        is within this window's area.

    \param useDefault
        Sepcifies whether to return the default mouse cursor image if this
        window specifies no preferred mouse cursor image.

    \return
        Pointer to the mouse cursor image that will be used when the mouse
        enters this window's area.  May return NULL indicating no cursor will
        be drawn for this window.
    */
    const Image* getMouseCursor(bool useDefault = true) const;

    /*!
    \brief
        Return the window size in pixels.

    \return
        Size object describing this windows size in pixels.
    */
    Size getPixelSize(void) const    { return d_pixelSize; }

    /*!
    \brief
        Return the user data set for this Window.

        Each Window can have some client assigned data attached to it, this data
        is not used by the GUI system in any way.  Interpretation of the data is
        entirely application specific.

    \return
        pointer to the user data that is currently set for this window.
    */
    void* getUserData(void) const   {return d_userData;}

    /*!
    \brief
        Return whether this window is set to restore old input capture when it
        loses input capture.

        This is only really useful for certain sub-components for widget
        writers.

    \return
        - true if the window will restore the previous capture window when it
          loses input capture.
        - false if the window will set the capture window to NULL when it loses
          input capture (this is the default behaviour).
    */
    bool restoresOldCapture(void) const     {return d_restoreOldCapture;}

    /*!
    \brief
        Return whether z-order changes are enabled or disabled for this Window.

    \note
        This is distinguished from the is/setRiseOnClickEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront or
        moveToBack member functions.  Whereas if z ordering is disabled the
        functions moveToFront and moveToBack are also precluded from affecting
        the Window z position.

    \return
        - true if z-order changes are enabled for this window.
          moveToFront/moveToBack work normally as expected.
        - false: z-order changes are disabled for this window.
          moveToFront/moveToBack are ignored for this window.
    */
    bool isZOrderingEnabled(void) const;

    /*!
    \brief
        Return whether this window will receive multi-click events or multiple
        'down' events instead.

    \return
        - true if the Window will receive double-click and triple-click events.
        - false if the Window will receive multiple mouse button down events
          instead of double/triple click events.
    */
    bool wantsMultiClickEvents(void) const;

    /*!
    \brief
        Return whether mouse button down event autorepeat is enabled for this
        window.

    \return
        - true if autorepeat of mouse button down events is enabled for this
          window.
        - false if autorepeat of mouse button down events is not enabled for
          this window.
    */
    bool isMouseAutoRepeatEnabled(void) const;

    /*!
    \brief
        Return the current auto-repeat delay setting for this window.

    \return
        float value indicating the delay, in seconds, defore the first repeat
        mouse button down event will be triggered when autorepeat is enabled.
    */
    float getAutoRepeatDelay(void) const;

    /*!
    \brief
        Return the current auto-repeat rate setting for this window.

    \return
        float value indicating the rate, in seconds, at which repeat mouse
        button down events will be generated after the initial delay has
        expired.
    */
    float getAutoRepeatRate(void) const;

    /*!
    \brief
        Return whether the window wants inputs passed to its attached
        child windows when the window has inputs captured.

    \return
        - true if System should pass captured input events to child windows.
        - false if System should pass captured input events to this window only.
    */
    bool distributesCapturedInputs(void) const;

    /*!
    \brief
        Return whether this Window is using the system default Tooltip for its
        Tooltip window.

    \return
        - true if the Window will use the system default tooltip.
        - false if the window has a custom Tooltip object.
    */
    bool isUsingDefaultTooltip(void) const;

    /*!
    \brief
        Return a pointer to the Tooltip object used by this Window.  The value
        returned may point to the system default Tooltip, a custom Window
        specific Tooltip, or be NULL.

    \return
        Pointer to a Tooltip based object, or NULL.
    */
    Tooltip* getTooltip(void) const;

    /*!
    \brief
        Return the custom tooltip type.

    \return
        String object holding the current custom tooltip window type, or an
        empty string if no custom tooltip is set.
     */
    String getTooltipType(void) const;

    /*!
    \brief
        Return the current tooltip text set for this Window.

    \return
        String object holding the current tooltip text set for this window.
     */
    const String& getTooltipText(void) const;

    /*!
    \brief
        Return whether this window inherits Tooltip text from its parent when
        its own tooltip text is not set.

    \return
        - true if the window inherits tooltip text from its parent when its own
          text is not set.
        - false if the window does not inherit tooltip text from its parent
          (and shows no tooltip when no text is set).
     */
    bool inheritsTooltipText(void) const;

    /*!
    \brief
        Return whether this window will rise to the top of the z-order when
        clicked with the left mouse button.

    \note
        This is distinguished from the is/setZOrderingEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront or
        moveToBack member functions.  Whereas if z ordering is disabled the
        functions moveToFront and moveToBack are also precluded from affecting
        the Window z position.

    \return
        - true if the window will come to the top of other windows when the left
          mouse button is pushed within its area.
        - false if the window does not change z-order position when the left
          mouse button is pushed within its area.
     */
    bool isRiseOnClickEnabled(void) const   { return d_riseOnClick; }

    /*!
    \brief
        Return whether this window was inherited from the given class name at
        some point in the inheritance hierarchy.

    \param class_name
        The class name that is to be checked.

    \return
        true if this window was inherited from \a class_name. false if not.
    */
    bool testClassName(const String& class_name) const
         {return testClassName_impl(class_name);}

    /*!
    \brief
        Get the vertical alignment.

        Returns the vertical alignment for the window.  This setting affects how
        the windows position is interpreted relative to its parent.

    \return
        One of the VerticalAlignment enumerated values.
     */
    VerticalAlignment getVerticalAlignment() const  {return d_vertAlign;}

    /*!
    \brief
        Get the horizontal alignment.

        Returns the horizontal alignment for the window.  This setting affects
        how the windows position is interpreted relative to its parent.

    \return
        One of the HorizontalAlignment enumerated values.
     */
    HorizontalAlignment getHorizontalAlignment() const  {return d_horzAlign;}

    /*!
    \brief
        Return the RenderCache object for this Window.

    \return
        Reference to the RenderCache object for this Window.
    */
    RenderCache& getRenderCache()   { return d_renderCache; }

    /*!
    \brief
        Get the name of the LookNFeel assigned to this window.

    \return
        String object holding the name of the look assigned to this window.
        Returns the empty string if no look is assigned.
    */
    const String& getLookNFeel() const;

    /*!
    \brief
        Get whether or not this Window is the modal target.

    \return
        Returns true if this Window is the modal target, otherwise false.
    */
    bool getModalState(void) const
         {return (System::getSingleton().getModalTarget() == this);}

    /*!
    \brief
        Returns a named user string.

    \param name
        String object holding the name of the string to be returned.

    \return
        String object holding the data stored for the requested user string.

    \exception UnknownObjectException
        thrown if a user string named \a name does not exist.
    */
    const String& getUserString(const String& name) const;

    /*!
    \brief
        Return whether a user string with the specified name exists.

    \param name
        String object holding the name of the string to be checked.

    \return
        - true if a user string named \a name exists.
        - false if no such user string exists.
    */
    bool isUserStringDefined(const String& name) const;

    /*!
    \brief
        Returns the active sibling window.

        This searches the immediate children of this window's parent, and
        returns a pointer to the active window.  The method will return this if
        we are the immediate child of our parent that is active.  If our parent
        is not active, or if no immediate child of our parent is active then 0
        is returned.  If this window has no parent, and this window is not
        active then 0 is returned, else this is returned.

    \return
        A pointer to the immediate child window attached to our parent that is
        currently active, or 0 if no immediate child of our parent is active.
    */
    Window* getActiveSibling();

    /*!
    \brief
        Return the pixel size of the parent element.  This always returns a
        valid object.

    \return
        Size object that describes the pixel dimensions of this Window objects
        parent
    */
    Size getParentPixelSize(void) const;

    /*!
    \brief
        Return the pixel Width of the parent element.  This always returns a
        valid number.

    \return
        float value that is equal to the pixel width of this Window objects
        parent
    */
    float getParentPixelWidth(void) const;

    /*!
    \brief
        Return the pixel Height of the parent element.  This always returns a
        valid number.

    \return
        float value that is equal to the pixel height of this Window objects
        parent
    */
    float getParentPixelHeight(void) const;

    /*!
    \brief
        Returns whether this window should ignore mouse event and pass them
        through to and other windows behind it. In effect making the window
        transparent to the mouse.

    \return
        true if mouse pass through is enabled.
        false if mouse pass through is not enabled.
    */
    bool isMousePassThroughEnabled(void) const  {return d_mousePassThroughEnabled;}

    /*!
    \brief
        Returns whether this window is an auto-child window.
        All auto-child windows have "__auto_" in their name, but this is faster.
    */
    bool isAutoWindow(void) const   {return d_autoWindow;}

    /*!
    \brief
        Returns whether this window is allowed to write XML.
    */
    bool isWritingXMLAllowed(void) const    {return d_allowWriteXML;}

    /*!
    \brief
        Helper method that returns an EventSet::Iterator object that can be used
        to iterate over the events currently added to the EventSet of this Window.
    \par
        This helper member is provided as an easy way to avoid some abiguity
        we have due to using multiple inheritence.  Ultimately it avoids the
        need to do things like this (which some people don't like!):
        \code
        // obtain an iterator for the EventSet
        EventSet::Iterator evt_iter = myWindow->EventSet::getIterator();

        // obtain an iterator for the PropertySet
        PropertySet::Iterator prp_iter = myWindow->PropertySet::getIterator();
        \endcode

    \note
        Iterating over events in the EventSet is of questionable use these days,
        since available Events are no longer added in one batch at creation time,
        but are added individually whenever an event is first subscribed.

    \return
        EventSet::Iterator object.
    */
    EventSet::Iterator getEventIterator() const;

    /*!
    \brief
        Helper method that returns a PropertySet::Iterator object that can be
        used to iterate over the events currently added to the PropertySet of
        this Window.
    \par
        This helper member is provided as an easy way to avoid some abiguity
        we have due to using multiple inheritence.  Ultimately it avoids the
        need to do things like this (which some people don't like!):
        \code
        // obtain an iterator for the EventSet
        EventSet:: Iterator evt_iter = myWindow->EventSet::getIterator();

        // obtain an iterator for the PropertySet
        PropertySet::Iterator prp_iter = myWindow->PropertySet::getIterator();
        \endcode

    \return
        PropertySet::Iterator object.
    */
    PropertySet::Iterator getPropertyIterator() const;

    /*!
    \brief
        Returns whether this Window object will receive events generated by
        the drag and drop support in the system.

    \return
        - true if the Window is enabled as a drag and drop target.
        - false if the window is not enabled as a drag and drop target.
    */
    bool isDragDropTarget() const;

    /*************************************************************************
        Manipulator functions
    *************************************************************************/
    /*!
    \brief
        Renames the window.

    \param new_name
        String object holding the new name for the window.

    \exception AlreadyExistsException
        thrown if a Window named \a new_name already exists in the system.
    */
    void rename(const String& new_name);

    /*!
    \brief
        Initialises the Window based object ready for use.

    \note
        This must be called for every window created.  Normally this is handled
        automatically by the WindowManager.

    \return
        Nothing
    */
    virtual void initialiseComponents(void) {}

    /*!
    \brief
        Set whether or not this Window will automatically be destroyed when its
        parent Window is destroyed.

    \param setting
        - true to have the Window auto-destroyed when its parent is destroyed
          (default behaviour)
        - false to have the Window remain after its parent is destroyed.

    \return
        Nothing
    */
    void setDestroyedByParent(bool setting);

    /*!
    \brief
        Set whether this window is always on top, or not.

    \param setting
        - true to have the Window appear on top of all other non always on top
          windows
        - false to allow the window to be covered by other normal windows.

    \return
        Nothing
    */
    void setAlwaysOnTop(bool setting);

    /*!
    \brief
        Set whether this window is enabled or disabled.  A disabled window
        normally can not be interacted with, and may have different rendering.

    \param setting
        - true to enable the Window
        - false to disable the Window.

    \return
        Nothing
    */
    void setEnabled(bool setting);

    /*!
    \brief
        enable the Window to allow interaction.

    \return
        Nothing
    */
    void enable(void)   {setEnabled(true);}

    /*!
    \brief
        disable the Window to prevent interaction.

    \return
        Nothing
    */
    void disable(void)  {setEnabled(false);}

    /*!
    \brief
        Set whether the Window is visible or hidden.

    \param setting
        - true to make the Window visible.
        - false to make the Window hidden.

    \note
        Hiding the active window will cause that window to become deactivated.
        Showing a window does not, however, automatically cause that window to
        become the active window (call Window::activate after making the window
        visible to activate it).

    \return
        Nothing
    */
    void setVisible(bool setting);

    /*!
    \brief
        show the Window.

    \note
        Showing a window does not automatically activate the window.  If you
        want the window to also become active you will need to call the
        Window::activate member also.

    \return
        Nothing
    */
    void show(void)     {setVisible(true);}

    /*!
    \brief
        hide the Window.
    \note
        If the window is the active window, it will become deactivated as a
        result of being hidden.

    \return
        Nothing
    */
    void hide(void)     {setVisible(false);}

    /*!
    \brief
        Activate the Window giving it input focus and bringing it to the top of
        all windows with the same always-on-top settig as this Window.

    \return
        Nothing
    */
    void activate(void);

    /*!
    \brief
        Deactivate the window.  No further inputs will be received by the window
        until it is re-activated either programmatically or by the user
        interacting with the gui.

    \return
        Nothing.
    */
    void deactivate(void);

    /*!
    \brief
        Set whether this Window will be clipped by its parent window(s).

    \param setting
        - true to have the Window clipped so that rendering is constrained to
          within the area of its parent(s).
        - false to have rendering constrained to the screen only.

    \return
        Nothing
    */
    void setClippedByParent(bool setting);

    /*!
    \brief
        Set the current ID for the Window.

    \param ID
        Client assigned ID code for this Window.  The GUI system assigns no
        meaning to any IDs, they are a device purely for client code usage.

    \return
        Nothing
    */
    void setID(uint ID);

	/**
	 * Sets the unique prefix for this window.
	 * \param prefix String object holding the prefix to be used on this window.
	 */
	void setPrefix(String prefix) { d_windowPrefix = prefix;}

    /*!
    \brief
        Set the current text string for the Window.

    \param text
        String object containing the text that is to be set as the Window text.

    \return
        Nothing
    */
    void setText(const String& text);

    /*!
    \brief
        Insert the text string \a text into the current text string for the
        Window object at the position specified by \a position.

    \param text
        String object holding the text that is to be inserted into the Window
        object's current text string.

    \param position
        The characted index position where the string \a text should be
        inserted.
    */
    void insertText(const String& text, const String::size_type position);

    /*!
    \brief
        Append the string \a text to the currect text string for the Window
        object.

    \param text
        String object holding the text that is to be appended to the Window
        object's current text string.
    */
    void appendText(const String& text);

    /*!
    \brief
        Set the font used by this Window.

    \param font
        Pointer to the Font object to be used by this Window.
        If \a font is NULL, the default font will be used.

    \return
        Nothing
    */
    void setFont(Font* font);

    /*!
    \brief
        Set the font used by this Window.

    \param name
        String object holding the name of the Font object to be used by this
        Window.  If \a name == "", the default font will be used.

    \return
        Nothing

    \exception UnknownObjectException
        thrown if the specified Font is unknown within the system.
    */
    void setFont(const String& name);

    /*!
    \brief
        Add the named Window as a child of this Window.  If the Window \a name
        is already attached to a Window, it is detached before being added to
        this Window.

    \param name
        String object holding the name of the Window to be added.

    \return
        Nothing.

    \exception UnknownObjectException
        thrown if no Window named \a name exists.
    \exception InvalidRequestException
        thrown if Window \a name is an ancestor of this Window, to prevent
        cyclic Window structures.
    */
    void addChildWindow(const String& name);

    /*!
    \brief
        Add the specified Window as a child of this Window.  If the Window
        \a window is already attached to a Window, it is detached before
        being added to this Window.

    \param window
        Pointer to the Window object to be added.

    \return
        Nothing

    \exception InvalidRequestException
        thrown if Window \a window is an ancestor of this Window, to prevent
        cyclic Window structures.
    */
    void addChildWindow(Window* window);

    /*!
    \brief
        Remove the named Window from this windows child list.

    \param name
        String object holding the name of the Window to be removed.  If the
        Window specified is not attached to this Window, nothing happens.

    \return
        Nothing.
    */
    void removeChildWindow(const String& name);

    /*!
    \brief
        Remove the specified Window form this windows child list.

    \param window
        Pointer to the Window object to be removed.  If the \a window is not
        attached to this Window, then nothing happens.

    \return
        Nothing.
    */
    void removeChildWindow(Window* window);

    /*!
    \brief
        Remove the first child Window with the specified ID.  If there is more
        than one attached Window objects with the specified ID, only the fist
        one encountered will be removed.

    \param ID
        ID number assigned to the Window to be removed.  If no Window with ID
        code \a ID is attached, nothing happens.

    \return
        Nothing.
    */
    void removeChildWindow(uint ID);

    /*!
    \brief
        Move the Window to the top of the z order.

        - If the Window is a non always-on-top window it is moved the the top of
          all other non always-on-top sibling windows, and the process repeated
          for all ancestors.
        - If the Window is an always-on-top window it is moved to the of of all
          sibling Windows, and the process repeated for all ancestors.

    \return
        Nothing
    */
    void moveToFront();

    /*!
    \brief
        Move the Window to the bottom of the Z order.

        - If the window is non always-on-top the Window is sent to the very
          bottom of its sibling windows and the process repeated for all
          ancestors.
        - If the window is always-on-top, the Window is sent to the bottom of
          all sibling always-on-top windows and the process repeated for all
          ancestors.

    \return
        Nothing
    */
    void moveToBack();

    /*!
    \brief
        Captures input to this window

    \return
        - true if input was successfully captured to this window.
        - false if input could not be captured to this window
          (maybe because the window is not active).
    */
    bool captureInput(void);

    /*!
    \brief
        Releases input capture from this Window.  If this Window does not have
        inputs captured, nothing happens.

    \return
        Nothing
    */
    void releaseInput(void);

    /*!
    \brief
        Set whether this window will remember and restore the previous window
        that had inputs captured.

    \param setting
        - true: The window will remember and restore the previous capture
          window.  The CaptureLost event is not fired on the previous window
          when this window steals input capture.  When this window releases
          capture, the old capture window is silently restored.

        - false: Input capture works as normal, each window losing capture is
          signalled via CaptureLost, and upon the final release of capture, no
          previous setting is restored (this is the default behaviour).

    \return
        Nothing
    */
    void setRestoreCapture(bool setting);

    /*!
    \brief
        Set the current alpha value for this window.

    \note
        The alpha value set for any given window may or may not be the final
        alpha value that is used when rendering.  All window objects, by
        default, inherit alpha from thier parent window(s) - this will blend
        child windows, relatively, down the line of inheritance.  This behaviour
        can be overridden via the setInheritsAlpha() method.  To return the true
        alpha value that will be applied when rendering, use the
        getEffectiveAlpha() method.

    \param alpha
        The new alpha value for the window.
        Value should be between 0.0f and 1.0f.

    \return
        Nothing
    */
    void setAlpha(float alpha);

    /*!
    \brief
        Sets whether this Window will inherit alpha from its parent windows.

    \param setting
        - true if the Window should use inherited alpha.
        - false if the Window should have an independant alpha value.

    \return
        Nothing
    */
    void setInheritsAlpha(bool setting);

    /*!
    \brief
        Signal the System object to redraw (at least) this Window on the next
        render cycle.

    \return
        Nothing
    */
    void requestRedraw(void) const;

    /*!
    \brief
        Set the mouse cursor image to be used when the mouse enters this window.

    \param image
        Pointer to the Image object to use as the mouse cursor image when the
        mouse enters the area for this Window.

    \return
        Nothing.
    */
    void setMouseCursor(const Image* image)     {d_mouseCursor = image;}

    /*!
    \brief
        Set the mouse cursor image to be used when the mouse enters this window.

    \param image
        One of the MouseCursorImage enumerated values.

    \return
        Nothing.
    */
    void setMouseCursor(MouseCursorImage image)
         {d_mouseCursor = (const Image*)image;}

    /*!
    \brief
        Set the mouse cursor image to be used when the mouse enters this window.

    \param imageset
        String object that contains the name of the Imageset that contains the
        image to be used.

    \param image_name
        String object that contains the name of the Image on \a imageset that
        is to be used.

    \return
        Nothing.

    \exception UnknownObjectException
        thrown if \a imageset is not known, or if \a imageset contains no Image
        named \a image_name.
    */
    void setMouseCursor(const String& imageset, const String& image_name);

    /*!
    \brief
        Set the user data set for this Window.

        Each Window can have some client assigned data attached to it, this data
        is not used by the GUI system in any way.  Interpretation of the data is
        entirely application specific.

    \param user_data
        pointer to the user data that is to be set for this window.

    \return
        Nothing.
    */
    void setUserData(void* user_data)   {d_userData = user_data;}

    /*!
    \brief
        Set whether z-order changes are enabled or disabled for this Window.

    \note
        This is distinguished from the is/setRiseOnClickEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront or
        moveToBack member functions.  Whereas if z ordering is disabled the
        functions moveToFront and moveToBack are also precluded from affecting
        the Window z position.

    \param setting
        - true if z-order changes are enabled for this window.
          moveToFront/moveToBack work normally as expected.
        - false: z-order changes are disabled for this window.
          moveToFront/moveToBack are ignored for this window.

    \return
        Nothing.
    */
    void    setZOrderingEnabled(bool setting);

    /*!
    \brief
        Set whether this window will receive multi-click events or multiple
        'down' events instead.

    \param setting
        - true if the Window will receive double-click and triple-click events.
        - false if the Window will receive multiple mouse button down events
          instead of double/triple click events.

    \return
        Nothing.
    */
    void setWantsMultiClickEvents(bool setting);

    /*!
    \brief
        Set whether mouse button down event autorepeat is enabled for this
        window.

    \param setting
        - true to enable autorepeat of mouse button down events.
        - false to disable autorepeat of mouse button down events.

    \return
        Nothing.
    */
    void setMouseAutoRepeatEnabled(bool setting);

    /*!
    \brief
        Set the current auto-repeat delay setting for this window.

    \param delay
        float value indicating the delay, in seconds, defore the first repeat
        mouse button down event should be triggered when autorepeat is enabled.

    \return
        Nothing.
    */
    void setAutoRepeatDelay(float delay);

    /*!
    \brief
        Set the current auto-repeat rate setting for this window.

    \param rate
        float value indicating the rate, in seconds, at which repeat mouse
        button down events should be generated after the initial delay has
        expired.

    \return
        Nothing.
    */
    void setAutoRepeatRate(float rate);

    /*!
    \brief
        Set whether the window wants inputs passed to its attached
        child windows when the window has inputs captured.

    \param setting
        - true if System should pass captured input events to child windows.
        - false if System should pass captured input events to this window only.
    */
    void setDistributesCapturedInputs(bool setting);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemEnters(DragContainer* item);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemLeaves(DragContainer* item);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemDropped(DragContainer* item);

    /*!
    \brief
        Internal destroy method which actually just adds the window and any
        parent destructed child windows to the dead pool.

        This is virtual to allow for specialised cleanup which may be required
        in some advanced cases.  If you override this for the above reason, you
        MUST call this base class version.

    \note
        You never have to call this method yourself, use WindowManager to
        destroy your Window objects (which will call this for you).
    */
    virtual void destroy(void);

    /*!
    \brief
        Set the custom Tooltip object for this Window.  This value may be 0 to
        indicate that the Window should use the system default Tooltip object.

    \param tooltip
        Pointer to a valid Tooltip based object which should be used as the
        tooltip for this Window, or 0 to indicate that the Window should use the
        system default Tooltip object.  Note that when passing a pointer to a
        Tooltip object, ownership of the Tooltip does not pass to this Window
        object.

    \return
        Nothing.
    */
    void setTooltip(Tooltip* tooltip);

    /*!
    \brief
        Set the custom Tooltip to be used by this Window by specifying a Window
        type.

        The Window will internally attempt to create an instance of the
        specified window type (which must be derived from the base Tooltip
        class).  If the Tooltip creation fails, the error is logged and the
        Window will revert to using either the existing custom Tooltip or the
        system default Tooltip.

    \param tooltipType
        String object holding the name of the Tooltip based Window type which
        should be used as the Tooltip for this Window.

    \return
        Nothing.
    */
    void setTooltipType(const String& tooltipType);

    /*!
    \brief
        Set the tooltip text for this window.

    \param tip
        String object holding the text to be displayed in the tooltip for this
        Window.

    \return
        Nothing.
    */
    void setTooltipText(const String& tip);

    /*!
    \brief
        Set whether this window inherits Tooltip text from its parent when its
        own tooltip text is not set.

    \param setting
        - true if the window should inherit tooltip text from its parent when
          its own text is not set.
        - false if the window should not inherit tooltip text from its parent
          (and so show no tooltip when no text is set).

    \return
        Nothing.
     */
    void setInheritsTooltipText(bool setting);

    /*!
    \brief
        Set whether this window will rise to the top of the z-order when clicked
        with the left mouse button.

    \note
        This is distinguished from the is/setZOrderingEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront or
        moveToBack member functions.  Whereas if z ordering is disabled the
        functions moveToFront and moveToBack are also precluded from affecting
        the Window z position.

    \param setting
        - true if the window should come to the top of other windows when the
          left mouse button is pushed within its area.
        - false if the window should not change z-order position when the left
          mouse button is pushed within its area.

    \return
        Nothing.
     */
    void setRiseOnClickEnabled(bool setting)    { d_riseOnClick = setting; }

    /*!
    \brief
        Set the vertical alignment.

        Modifies the vertical alignment for the window.  This setting affects
        how the windows position is interpreted relative to its parent.

    \param alignment
        One of the VerticalAlignment enumerated values.

    \return
        Nothing.
     */
    void setVerticalAlignment(const VerticalAlignment alignment);

    /*!
    \brief
        Set the horizontal alignment.

        Modifies the horizontal alignment for the window.  This setting affects
        how the windows position is interpreted relative to its parent.

    \param alignment
        One of the HorizontalAlignment enumerated values.

    \return
        Nothing.
     */
    void setHorizontalAlignment(const HorizontalAlignment alignment);

    /*!
    \brief
        Set the LookNFeel that shoule be used for this window.

    \param look
        String object holding the name of the look to be assigned to the window.

    \return
        Nothing.

    \exception UnknownObjectException
        thrown if the look'n'feel specified by \a look does not exist.

    \note
        Once a look'n'feel has been assigned it is locked - as in cannot be changed.
    */
    virtual void setLookNFeel(const String& look);

    /*!
    \brief
        Set the modal state for this Window.

    \param state
        Boolean value defining if this Window should be the modal target.
        - true if this Window should be activated and set as the modal target.
        - false if the modal target should be cleared if this Window is
          currently the modal target.

    \return
        Nothing.
    */
    void setModalState(bool state);

    /*!
    \brief
        method called to perform extended laying out of attached child windows.

        The system may call this at various times (like when it is resized for
        example), and it may be invoked directly where required.

    \return
        Nothing.
    */
    virtual void performChildWindowLayout();

    /*!
    \brief
       Sets the value a named user string, creating it as required.

    \param name
        String object holding the name of the string to be returned.

    \param value
        String object holding the value to be assigned to the user string.

    \return
        Nothing.
    */
    void setUserString(const String& name, const String& value);

    /*!
    \brief
        Set the window area.

        Sets the area occupied by this window.  The defined area is offset from
        the top-left corner of this windows parent window or from the top-left
        corner of the display if this window has no parent (i.e. it is the root
        window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param xpos
        UDim describing the new x co-ordinate (left edge) of the window area.

    \param ypos
        UDim describing the new y co-ordinate (top-edge) of the window area.

    \param width
        UDim describing the new width of the window area.

    \param height
        UDim describing the new height of the window area.
     */
    void setArea(const UDim& xpos, const UDim& ypos, const UDim& width, const UDim& height);

    /*!
    \brief
        Set the window area.

        Sets the area occupied by this window.  The defined area is offset from
        the top-left corner of this windows parent window or from the top-left
        corner of the display if this window has no parent (i.e. it is the root
        window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param pos
        UVector2 describing the new position (top-left corner) of the window
        area.

    \param size
        UVector2 describing the new size of the window area.
     */
    void setArea(const UVector2& pos, const UVector2& size);

    /*!
    \brief
        Set the window area.

        Sets the area occupied by this window.  The defined area is offset from
        the top-left corner of this windows parent window or from the top-left
        corner of the display if this window has no parent (i.e. it is the root
        window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param area
        URect describing the new area rectangle of the window area.
     */
    void setArea(const URect& area);

    /*!
    \brief
        Set the window's position.

        Sets the position of the area occupied by this window.  The position is
        offset from the top-left corner of this windows parent window or from
        the top-left corner of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param pos
        UVector2 describing the new position (top-left corner) of the window
        area.
     */
    void setPosition(const UVector2& pos);

    /*!
    \brief
        Set the window's X position.

        Sets the x position (left edge) of the area occupied by this window.
        The position is offset from the left edge of this windows parent window
        or from the left edge of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param x
        UDim describing the new x position of the window area.
     */
    void setXPosition(const UDim& x);

    /*!
    \brief
        Set the window's Y position.

        Sets the y position (top edge) of the area occupied by this window.
        The position is offset from the top edge of this windows parent window
        or from the top edge of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param y
        UDim describing the new y position of the window area.
     */
    void setYPosition(const UDim& y);

    /*!
    \brief
        Set the window's size.

        Sets the size of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param size
        UVector2 describing the new size of the window area.
     */
    void setSize(const UVector2& size);

    /*!
    \brief
        Set the window's width.

        Sets the width of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param width
        UDim describing the new width of the window area.
     */
    void setWidth(const UDim& width);

    /*!
    \brief
        Set the window's height.

        Sets the height of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param height
        UDim describing the new height of the window area.
     */
    void setHeight(const UDim& height);

    /*!
    \brief
        Set the window's maximum size.

        Sets the maximum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param size
        UVector2 describing the new maximum size of the window area.
     */
    void setMaxSize(const UVector2& size);

    /*!
    \brief
        Set the window's minimum size.

        Sets the minimum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param size
        UVector2 describing the new minimum size of the window area.
     */
    void setMinSize(const UVector2& size);

    /*!
    \brief
        Return the windows area.

        Returns the area occupied by this window.  The defined area is offset
        from the top-left corner of this windows parent window or from the
        top-left corner of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        URect describing the rectangle of the window area.
     */
    const URect& getArea() const;

    /*!
    \brief
        Get the window's position.

        Gets the position of the area occupied by this window.  The position is
        offset from the top-left corner of this windows parent window or from
        the top-left corner of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UVector2 describing the position (top-left corner) of the window area.
     */
    const UVector2& getPosition() const;

    /*!
    \brief
        Get the window's X position.

        Gets the x position (left edge) of the area occupied by this window.
        The position is offset from the left edge of this windows parent window
        or from the left edge of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UDim describing the x position of the window area.
     */
    const UDim& getXPosition() const;

    /*!
    \brief
        Get the window's Y position.

        Gets the y position (top edge) of the area occupied by this window.
        The position is offset from the top edge of this windows parent window
        or from the top edge of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UDim describing the y position of the window area.
     */
    const UDim& getYPosition() const;

    /*!
    \brief
        Get the window's size.

        Gets the size of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UVector2 describing the size of the window area.
     */
    UVector2 getSize() const;

    /*!
    \brief
        Get the window's width.

        Gets the width of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UDim describing the width of the window area.
     */
    UDim getWidth() const;

    /*!
    \brief
        Get the window's height.

        Gets the height of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UDim describing the height of the window area.
     */
    UDim getHeight() const;

    /*!
    \brief
        Get the window's maximum size.

        Gets the maximum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UVector2 describing the maximum size of the window area.
     */
    const UVector2& getMaxSize() const;

    /*!
    \brief
        Get the window's minimum size.

        Gets the minimum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UVector2 describing the minimum size of the window area.
     */
    const UVector2& getMinSize() const;

    /*!
    \brief
        Causes the Window object to render itself and all of it's attached
        children

    \return
        Nothing
    */
    void render(void);

    /*!
    \brief
        Cause window to update itself and any attached children.  Client code
        does not need to call this method; to ensure full, and proper updates,
        call the injectTimePulse methodname method provided by the System class.

    \note
        The update order is such that 'this' window is updated prior to any
        child windows, this is so that child windows that access the parent in
        their update code get the correct updated state.

    \param elapsed
        float value indicating the number of seconds passed since the last
        update.

    \return
        Nothing.
    */
    void update(float elapsed);

    /*!
    \brief
        Writes an xml representation of this window object to \a out_stream.

    \param xml_stream
        Stream where xml data should be output.

    \return
        Nothing.
    */
    virtual void writeXMLToStream(XMLSerializer& xml_stream) const;

    /*!
    \brief
        Sets the internal 'initialising' flag to true.
        This can be use to optimize initialisation of some widgets, and is called
        automatically by the layout XML handler when it has created a window.
        That is just after the window has been created, but before any children or
        properties are read.
    */
    virtual void beginInitialisation(void)     {d_initialising=true;}

    /*!
    \brief
        Sets the internal 'initialising' flag to false.
        This is called automatically by the layout XML handler when it is done
        creating a window. That is after all properties and children have been
        loaded and just before the next sibling gets created.
    */
    virtual void endInitialisation(void)       {d_initialising=false;}

    /*!
    \brief
        Sets whether this window should ignore mouse events and pass them
        through to any windows behind it. In effect making the window
        transparent to the mouse.

    \param setting
        true if mouse pass through is enabled.
        false if mouse pass through is not enabled.
    */
    void setMousePassThroughEnabled(bool setting)   {d_mousePassThroughEnabled = setting;}

    /*!
    \brief
        Assign the WindowRenderer to specify the Look'N'Feel specification
        to be used.

    \param name
        The factory name of the WindowRenderer to use.

    \note
        Once a window renderer has been assigned it is locked - as in cannot be changed.
    */
    void setWindowRenderer(const String& name);

    /*!
    \brief
        Get the currently assigned WindowRenderer. (Look'N'Feel specification).

    \return
        A pointer to the assigned window renderer object.
        0 if no window renderer is assigned.
    */
    WindowRenderer* getWindowRenderer(void) const;

    /*!
    \brief
        Get the factory name of the currently assigned WindowRenderer.
        (Look'N'Feel specification).

    \return
        The factory name of the currently assigned WindowRenderer.
        If no WindowRenderer is assigned an empty string is returned.
    */
    String getWindowRendererName(void) const;

    /*!
    \brief
        Sets whether this window is allowed to write XML
    */
    void setWritingXMLAllowed(bool allow)   {d_allowWriteXML = allow;}

    /*!
    \brief
        Recursively inform all children that the screen area has changed, and needs to be re-cached
    */
    void notifyScreenAreaChanged(void);

    /*!
    \brief
        Changes the widget's falagard type, thus changing its look'n'feel and optionally its
        renderer in the process.

    \param type
        New look'n'feel of the widget

    \param type
        New renderer of the widget
    */
    void setFalagardType(const String& type, const String& rendererType = "");

    /*!
    \brief
        Specifies whether this Window object will receive events generated by
        the drag and drop support in the system.

    \param setting
        - true to enable the Window as a drag and drop target.
        - false to disable the Window as a drag and drop target.
    */
    void setDragDropTarget(bool setting);

protected:
    /*************************************************************************
        System object can trigger events directly
    *************************************************************************/
    friend class System;

    /*************************************************************************
        Event trigger methods
    *************************************************************************/
    /*!
    \brief
        Handler called when the window's size changes.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onSized(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's position changes.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onMoved(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's text is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onTextChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's font is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onFontChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's alpha blend value is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onAlphaChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's client assigned ID is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onIDChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is shown (made visible).

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onShown(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is hidden.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onHidden(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is enabled.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onEnabled(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window is disabled.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onDisabled(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's setting for being clipped by it's
        parent is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onClippingChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's setting for being destroyed
        automatically be it's parent is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onParentDestroyChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's setting for inheriting alpha-blending
        is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onInheritsAlphaChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the window's always-on-top setting is changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onAlwaysOnTopChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window gains capture of mouse inputs.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onCaptureGained(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window loses capture of mouse inputs.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onCaptureLost(WindowEventArgs& e);

    /*!
    \brief
        Handler called when rendering for this window has started.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onRenderingStarted(WindowEventArgs& e);

    /*!
    \brief
        Handler called when rendering for this window has ended.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onRenderingEnded(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the z-order position of this window has changed.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onZChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window's destruction sequence has begun.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that triggered the event.  For this event the trigger window is always
        'this'.
    */
    virtual void onDestructionStarted(WindowEventArgs& e);

    /*!
    \brief
        Handler called when this window has become the active window.

    \param e
        ActivationEventArgs class whose 'otherWindow' field is set to the window
        that previously was active, or NULL for none.
    */
    virtual void onActivated(ActivationEventArgs& e);

    /*!
    \brief
        Handler called when this window has lost input focus and has been
        deactivated.

    \param e
        ActivationEventArgs object whose 'otherWindow' field is set to the
        window that has now become active, or NULL for none.
    */
    virtual void onDeactivated(ActivationEventArgs& e);

    /*!
    \brief
        Handler called when this window's parent window has been resized.  If
        this window is the root / GUI Sheet window, this call will be made when
        the display size changes.

    \param e
        WindowEventArgs object whose 'window' pointer field is set the the
        window that caused the event; this is typically either this window's
        parent window, or NULL to indicate the screen size has changed.
    */
    virtual void onParentSized(WindowEventArgs& e);

    /*!
    \brief
        Handler called when a child window is added to this window.

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the window
        that has been added.
    */
    virtual void onChildAdded(WindowEventArgs& e);

    /*!
    \brief
        Handler called when a child window is removed from this window.

    \param e
        WindowEventArgs object whose 'window' pointer field is set the window
        that has been removed.
    */
    virtual void onChildRemoved(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the mouse cursor has entered this window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseEnters(MouseEventArgs& e);

    /*!
    \brief
        Handler called when the mouse cursor has left this window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseLeaves(MouseEventArgs& e);

    /*!
    \brief
        Handler called when the mouse cursor has been moved within this window's
        area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseMove(MouseEventArgs& e);

    /*!
    \brief
        Handler called when the mouse wheel (z-axis) position changes within
        this window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseWheel(MouseEventArgs& e);

    /*!
    \brief
        Handler called when a mouse button has been depressed within this
        window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseButtonDown(MouseEventArgs& e);

    /*!
    \brief
        Handler called when a mouse button has been released within this
        window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseButtonUp(MouseEventArgs& e);

    /*!
    \brief
        Handler called when a mouse button has been clicked (that is depressed
        and then released, within a specified time) within this window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseClicked(MouseEventArgs& e);

    /*!
    \brief
        Handler called when a mouse button has been double-clicked within this
        window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseDoubleClicked(MouseEventArgs& e);

    /*!
    \brief
        Handler called when a mouse button has been triple-clicked within this
        window's area.

    \param e
        MouseEventArgs object.  All fields are valid.
    */
    virtual void onMouseTripleClicked(MouseEventArgs& e);

    /*!
    \brief
        Handler called when a key as been depressed while this window has input
        focus.

    \param e
        KeyEventArgs object whose 'scancode' field is set to the Key::Scan value
        representing the key that was pressed, and whose 'sysKeys' field
        represents the combination of SystemKey that were active when the event
        was generated.
    */
    virtual void onKeyDown(KeyEventArgs& e);

    /*!
    \brief
        Handler called when a key as been released while this window has input
        focus.

    \param e
        KeyEventArgs object whose 'scancode' field is set to the Key::Scan value
        representing the key that was released, and whose 'sysKeys' field
        represents the combination of SystemKey that were active when the event
        was generated.  All other fields should be considered as 'junk'.
    */
    virtual void onKeyUp(KeyEventArgs& e);

    /*!
    \brief
        Handler called when a character-key has been pressed while this window
        has input focus.

    \param e
        KeyEventArgs object whose 'codepoint' field is set to the Unicode code
        point (encoded as utf32) for the character typed, and whose 'sysKeys'
        field represents the combination of SystemKey that were active when the
        event was generated.  All other fields should be considered as 'junk'.
    */
    virtual void onCharacter(KeyEventArgs& e);

    /*!
    \brief
        Handler called when a DragContainer is dragged over this window.

    \param e
        DragDropEventArgs object initialised as follows:
        - window field is normaly set to point to 'this' window.
        - dragDropItem is a pointer to a DragContainer window that triggered
          the event.
    */
    virtual void onDragDropItemEnters(DragDropEventArgs& e);

    /*!
    \brief
        Handler called when a DragContainer is dragged over this window.

    \param e
        DragDropEventArgs object initialised as follows:
        - window field is normaly set to point to 'this' window.
        - dragDropItem is a pointer to a DragContainer window that triggered
          the event.
    */
    virtual void onDragDropItemLeaves(DragDropEventArgs& e);

    /*!
    \brief
        Handler called when a DragContainer is dragged over this window.

    \param e
        DragDropEventArgs object initialised as follows:
        - window field is normaly set to point to 'this' window.
        - dragDropItem is a pointer to a DragContainer window that triggered
          the event.
    */
    virtual void onDragDropItemDropped(DragDropEventArgs& e);

    /*!
    \brief
        Handler called when the vertical alignment setting for the window is
        changed.

    \param e
        WindowEventArgs object initialised as follows:
        - window field is set to point to the Window object whos alignment has
          changed (typically 'this').
    */
    virtual void onVerticalAlignmentChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the horizontal alignment setting for the window is
        changed.

    \param e
        WindowEventArgs object initialised as follows:
        - window field is set to point to the Window object whos alignment has
          changed (typically 'this').
    */
    virtual void onHorizontalAlignmentChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when a new window renderer object is attached.

    \param e
        WindowEventArgs object initialised as follows:
        - window field is set to point to the Window object that just got a new
          window renderer attached. (typically 'this').
    */
    virtual void onWindowRendererAttached(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the currently attached window renderer object is detached.

    \param e
        WindowEventArgs object initialised as follows:
        - window field is set to point to the Window object that just got lost its
          window renderer. (typically 'this').
    */
    virtual void onWindowRendererDetached(WindowEventArgs& e);

    /*************************************************************************
        Implementation Functions
    *************************************************************************/
    /*!
    \brief
        Perform actual update processing for this Window.

    \param elapsed
        float value indicating the number of seconds elapsed since the last
        update call.

    \return
        Nothing.
    */
    virtual void updateSelf(float elapsed);

    /*!
    \brief
        Perform the actual rendering for this Window.

    \param z
        float value specifying the base Z co-ordinate that should be used when
        rendering

    \return
        Nothing
    */
    virtual void drawSelf(float z);

    /*!
    \brief
        Update the rendering cache.

        Populates the Window's RenderCache with imagery to be sent to the
        renderer.
    */
    virtual void populateRenderCache()  {}

    /*!
    \brief
        Return whether this window was inherited from the given class name at
        some point in the inheritance hierarchy.

    \param class_name
        The class name that is to be checked.

    \return
        true if this window was inherited from \a class_name. false if not.
    */
    virtual bool testClassName_impl(const String& class_name) const
    {
        if (class_name=="Window")   return true;
        return false;
    }

    /*!
    \brief
        Set the parent window for this window object.

    \param parent
        Pointer to a Window object that is to be assigned as the parent to this
        Window.

    \return
        Nothing
    */
    void setParent(Window* parent);

    Size getSize_impl(const Window* window) const;

    /*!
    \brief
        Fires off a repeated mouse button down event for this window.
    */
    void generateAutoRepeatEvent(MouseButton button);

    /*!
    \brief
        Function used in checking if a WindowRenderer is valid for this window.

    \return
        Returns true if the given WindowRenderer class name is valid for this window.
        False if not.
    */
    virtual bool validateWindowRenderer(const String& name) const;

    /*!
    \brief
        Adds a property to the XML ban list
    */
    void banPropertyFromXML(const Property* property);

    /*!
    \brief
        Returns whether a property is banned from XML
    */
    bool isPropertyBannedFromXML(const Property* property) const;

    /*!
    \brief
        Returns whether a property is at it's default value.
        This function is different from Property::isDefatult as it takes the assigned look'n'feel
        (if the is one) into account.
    */
    bool isPropertyAtDefault(const Property* property) const;

    /*!
    \brief
        Recursively inform all children that the clipping has changed and screen rects
        needs to be recached.
    */
    void notifyClippingChanged(void);

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    // child stuff
    typedef std::vector<Window*> ChildList;
    //! The list of child Window objects attached to this.
    ChildList d_children;

    //! Child window objects arranged in rendering order.
    ChildList d_drawList;

    // general data
    //! Window that has captured inputs
    static Window* d_captureWindow;

    //! The Window that previously had capture (used for restoreOldCapture mode)
    Window* d_oldCapture;

    //! Holds pointer to the parent window.
    Window* d_parent;

    //! Holds pointer to the Window objects current Font.
    Font* d_font;

    //! Holds the text / label / caption for this Window.
    String d_text;

    //! User ID assigned to this Window
    uint d_ID;

    //! Alpha transparency setting for the Window
    float d_alpha;

    //! This Window objects area as defined by a URect.
    URect d_area;

    //! Current constrained pixel size of the window.
    Size d_pixelSize;

    //! Holds pointer to the Window objects current mouse cursor image.
    const Image* d_mouseCursor;

    // user data
    typedef std::map<String, String, String::FastLessCompare>   UserStringMap;

    //! Holds a collection of named user string values.
    UserStringMap d_userStrings;

    //! Holds pointer to some user assigned data.
    void* d_userData;

    // positional alignments
    //! Specifies the base for horizontal alignment.
    HorizontalAlignment d_horzAlign;

    //! Specifies the base for vertical alignment.
    VerticalAlignment d_vertAlign;

    // maximum and minimum sizes
    //! current minimum size for the window.
    UVector2 d_minSize;

    //! current maximum size for the window.
    UVector2 d_maxSize;

    // settings
    //! true when Window is enabled
    bool d_enabled;

    /*!
    \brief
        true when Window is visible (that is it will be rendered, but may be
        obscured so no necesarily really visible)
    */
    bool d_visible;

    //! true when Window is the active Window (receiving inputs).
    bool d_active;

    //! true when Window will be clipped by parent Window area Rect.
    bool d_clippedByParent;

    //! true when Window will be auto-destroyed by parent.
    bool d_destroyedByParent;

    //! true if Window will be drawn on top of all other Windows
    bool d_alwaysOnTop;

    //! true if the Window inherits alpha from the parent Window
    bool d_inheritsAlpha;

    /*!
    \brief
        true if the Window restores capture to the previous window when it
        releases capture.
    */
    bool d_restoreOldCapture;

    //! true if the Window responds to z-order change requests.
    bool d_zOrderingEnabled;

    //! true if the Window wishes to hear about multi-click mouse events.
    bool d_wantsMultiClicks;

    /*!
    \brief
        true if unhandled captured inputs should be distributed to child
        windows.
    */
    bool d_distCapturedInputs;

    /*!
    \brief
        True if the window should come to the front of the z order in response
        to a left mouse button down event.
    */
    bool d_riseOnClick;

    // mouse button autorepeat data
    /*!
    \brief
        true if button will auto-repeat mouse button down events while mouse
        button is held down.
    */
    bool d_autoRepeat;

    //! seconds before first repeat event is fired
    float d_repeatDelay;

    //! secons between further repeats after delay has expired.
    float d_repeatRate;

    //! implements repeating - is true after delay has elapsed,
    bool d_repeating;

    //! implements repeating - tracks time elapsed.
    float d_repeatElapsed;

    /*!
    \brief
        Button we're tracking (implication of this is that we only support one
        button at a time).
    */
    MouseButton d_repeatButton;

    //! true if window will receive drag and drop related notifications
    bool d_dragDropTarget;

    // Tooltip stuff
    //! Text string used as tip for this window.
    String d_tooltipText;
    //! Possible custom Tooltip for this window.
    Tooltip* d_customTip;
    //! true if this Window created the custom Tooltip.
    bool d_weOwnTip;
    /*!
    \brief
        true if the Window inherits tooltip text from its parent (when none set
        for itself).
    */
    bool d_inheritsTipText;

    // rendering
    //! Object which acts as a cache for Images to be drawn by this Window.
    RenderCache d_renderCache;
    //! true if window image cache needs to be regenerated.
    mutable bool d_needsRedraw;

    // Look'N'Feel stuff
    //! Name of the Look assigned to this window (if any).
    String d_lookName;
    //! The WindowRenderer module that implements the Look'N'Feel specification
    WindowRenderer* d_windowRenderer;

    //! true when this window is currently being initialised (creating children etc)
    bool d_initialising;
    //! true when this window is being destroyed.
    bool d_destructionStarted;

    // Event pass through
    /*!
    \brief
        true if this window can never be "hit" by the cursor.
        false for normal mouse event handling.
    */
    bool d_mousePassThroughEnabled;

    //! true when this window is an auto-window (it's name contains __auto_)
    bool d_autoWindow;

    /*!
    \brief
        std::set used to determine whether a window should write a property to XML or not.
        if the property name is present the property will not be written
    */
    typedef std::set<String, String::FastLessCompare> BannedXMLPropertySet;
    BannedXMLPropertySet d_bannedXMLProperties;

    //! true if this window is allowed to write XML, false if not
    bool d_allowWriteXML;

    //! current unclipped screen rect in pixels
    mutable Rect d_screenUnclippedRect;
    mutable bool d_screenUnclippedRectValid;
    //! current unclipped inner screen rect in pixels
    mutable Rect d_screenUnclippedInnerRect;
    mutable bool d_screenUnclippedInnerRectValid;
    //! current fully clipped screen rect in pixels
    mutable Rect d_screenRect;
    mutable bool d_screenRectValid;
    //! current fully clipped inner screen rect in pixels
    mutable Rect d_screenInnerRect;
    mutable bool d_screenInnerRectValid;

protected:
    /*************************************************************************
        Properties for Window base class
    *************************************************************************/
    static  WindowProperties::Alpha             d_alphaProperty;
    static  WindowProperties::AlwaysOnTop       d_alwaysOnTopProperty;
    static  WindowProperties::ClippedByParent   d_clippedByParentProperty;
    static  WindowProperties::DestroyedByParent d_destroyedByParentProperty;
    static  WindowProperties::Disabled          d_disabledProperty;
    static  WindowProperties::Font              d_fontProperty;
    static  WindowProperties::ID                d_IDProperty;
    static  WindowProperties::InheritsAlpha     d_inheritsAlphaProperty;
    static  WindowProperties::MouseCursorImage  d_mouseCursorProperty;
    static  WindowProperties::RestoreOldCapture d_restoreOldCaptureProperty;
    static  WindowProperties::Text              d_textProperty;
    static  WindowProperties::Visible           d_visibleProperty;
    static  WindowProperties::ZOrderChangeEnabled   d_zOrderChangeProperty;
    static  WindowProperties::WantsMultiClickEvents d_wantsMultiClicksProperty;
    static  WindowProperties::MouseButtonDownAutoRepeat d_autoRepeatProperty;
    static  WindowProperties::AutoRepeatDelay   d_autoRepeatDelayProperty;
    static  WindowProperties::AutoRepeatRate    d_autoRepeatRateProperty;
    static  WindowProperties::DistributeCapturedInputs d_distInputsProperty;
    static  WindowProperties::CustomTooltipType d_tooltipTypeProperty;
    static  WindowProperties::Tooltip           d_tooltipProperty;
    static  WindowProperties::InheritsTooltipText d_inheritsTooltipProperty;
    static  WindowProperties::RiseOnClick       d_riseOnClickProperty;
    static  WindowProperties::VerticalAlignment   d_vertAlignProperty;
    static  WindowProperties::HorizontalAlignment d_horzAlignProperty;
    static  WindowProperties::UnifiedAreaRect   d_unifiedAreaRectProperty;
    static  WindowProperties::UnifiedPosition   d_unifiedPositionProperty;
    static  WindowProperties::UnifiedXPosition  d_unifiedXPositionProperty;
    static  WindowProperties::UnifiedYPosition  d_unifiedYPositionProperty;
    static  WindowProperties::UnifiedSize       d_unifiedSizeProperty;
    static  WindowProperties::UnifiedWidth      d_unifiedWidthProperty;
    static  WindowProperties::UnifiedHeight     d_unifiedHeightProperty;
    static  WindowProperties::UnifiedMinSize    d_unifiedMinSizeProperty;
    static  WindowProperties::UnifiedMaxSize    d_unifiedMaxSizeProperty;
    static  WindowProperties::MousePassThroughEnabled   d_mousePassThroughEnabledProperty;
    static  WindowProperties::WindowRenderer    d_windowRendererProperty;
    static  WindowProperties::LookNFeel         d_lookNFeelProperty;
    static  WindowProperties::DragDropTarget    d_dragDropTargetProperty;

    /*************************************************************************
        implementation functions
    *************************************************************************/
    /*!
    \brief
        Cleanup child windows
    */
    virtual void cleanupChildren(void);

    /*!
    \brief
        Add given window to child list at an appropriate position
    */
    virtual void addChild_impl(Window* wnd);

    /*!
    \brief
        Remove given window from child list
    */
    virtual void removeChild_impl(Window* wnd);

    /*!
    \brief
        Notify 'this' and all siblings of a ZOrder change event
    */
    virtual void onZChange_impl(void);

    /*!
    \brief
        Add standard CEGUI::Window properties.
    */
    void    addStandardProperties(void);

    /*!
    \brief
        Implements move to front behavior.

    \return
        Should return true if some action was taken, or false if there was
        nothing to be done.
    */
    virtual bool moveToFront_impl(bool wasClicked);

    /*!
    \brief
        Implementation of rise on click functionality.

    \deprecated
        This function is redundant and will be removed for the 0.7.0 release.
        To achieve what this function was supposed to do - but never actually
        did - call Window::moveToFront_impl passing true as the parameter.

    \return
        true if we did something, false if there was nothing to do.
    */
    bool doRiseOnClick(void);

    /*!
    \brief
        Implementation method to modify window area while correctly applying
        min / max size processing, and firing any appropriate events.

    /note
        This is the implementation function for setting size and position.
        In order to simplify area management, from this point on, all
        modifications to window size and position (area rect) should come
        through here.

    /param pos
        UVector2 object describing the new area position.

    /param size
        UVector2 object describing the new area size.

    /param topLeftSizing
        - true to indicate the the operation is a sizing operation on the top
          and/or left edges of the area, and so window movement should be
          inhibited if size is at max or min.
        - false to indicate the operation is not a strict sizing operation on
          the top and/or left edges and that the window position may change as
          required

    /param fireEvents
        - true if events should be fired as normal.
        - false to inhibit firing of events (required, for example, if you need
          to call this from the onSize/onMove handlers).
     */
    void setArea_impl(const UVector2& pos, const UVector2& size, bool topLeftSizing = false, bool fireEvents = true);

    /*!
    \brief
        Add the given window to the drawing list at an appropriate position for
        it's settings and the required direction.  Basically, when \a at_back
        is false, the window will appear in front of all other windows with the
        same 'always on top' setting.  When \a at_back is true, the window will
        appear behind all other windows wih the same 'always on top' setting.

    \param wnd
        Window object to be added to the drawing list.

    \param at_back
        Indicates whether the window should be placed at the back of other
        windows in the same group. If this is false, the window is placed in
        front of other windows in the group.

    \return
        Nothing.
    */
    void addWindowToDrawList(Window& wnd, bool at_back = false);

    /*!
    \brief
        Removes the window from the drawing list.  If the window is not attached
        to the drawing list then nothing happens.

    \param wnd
        Window object to be removed from the drawing list.

    \return
        Nothing.
    */
    void removeWindowFromDrawList(const Window& wnd);

    /*!
    \brief
        Return whether the window is at the top of the Z-Order.  This will
        correctly take into account 'Always on top' windows as needed.

    \return
        - true if the Window is at the top of the z-order in relation to sibling
          windows with the same 'always on top' setting.
        - false if the Window is not at the top of the z-order in relation to
          sibling windows with the same 'always on top' setting.
    */
    bool isTopOfZOrder() const;

    virtual int writePropertiesXML(XMLSerializer& xml_stream) const;
    virtual int writeChildWindowsXML(XMLSerializer& xml_stream) const;
    virtual bool writeAutoChildWindowXML(XMLSerializer& xml_stream) const;

    /*************************************************************************
        May not copy or assign Window objects
    *************************************************************************/
    Window(const Window&) : PropertySet(), EventSet() {}
    Window& operator=(const Window&) {return *this;}

    /*************************************************************************
        Private implementation Data
    *************************************************************************/
    /*!
    \brief
        String holding the type name for the Window
        (is also the name of the WindowFactory that created us)
    */
    const String    d_type;

    //! The name of the window (GUI system unique).
    String    d_name;

    //! Type name of the window as defined in a Falagard mapping.
    String    d_falagardType;

	//! The prefix used on this window (if any) when created instanced windows.
	String    d_windowPrefix;

    friend class WindowManager;
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIWindow_h_
