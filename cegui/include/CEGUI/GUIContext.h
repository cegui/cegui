/***********************************************************************
    filename:   GUIContext.h
    created:    Mon Jan 12 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIGUIContext_h_
#define _CEGUIGUIContext_h_

#include "CEGUI/RenderingSurface.h"
#include "CEGUI/InjectedInputReceiver.h"
#include "CEGUI/MouseCursor.h"
#include "CEGUI/SystemKeys.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
struct MouseClickTracker;

//! EventArgs class passed to subscribers for (most) GUIContext events.
class CEGUIEXPORT GUIContextEventArgs : public EventArgs
{
public:
    GUIContextEventArgs(GUIContext* context):
        context(context)
    {}

    //! pointer to the GUIContext that triggered the event.
    GUIContext* context;
};

class CEGUIEXPORT GUIContext : public RenderingSurface,
                               public InjectedInputReceiver,
                               public AllocatedObject<GUIContext>
{
public:
    static const float DefaultMouseButtonClickTimeout;
    static const float DefaultMouseButtonMultiClickTimeout;
    static const Sizef DefaultMouseButtonMultiClickTolerance;

    /** Name of Event fired when the root window is changed to a different
     * Window.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the @e old root window (the new one is
     * obtained by calling GUIContext::getRootWindow).
     */
    static const String EventRootWindowChanged;
    /** Name of Event fired when the mouse movement scaling factor is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseMoveScalingFactorChanged;
    /** Name of Event fired when the mouse click timeout is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseButtonClickTimeoutChanged;
    /** Name of Event fired when the mouse multi-click timeout is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseButtonMultiClickTimeoutChanged;
    /** Name of Event fired when the mouse multi-click movement tolerance area
     * size is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseButtonMultiClickToleranceChanged;

    GUIContext(RenderTarget& target);
    ~GUIContext();

    Window* getRootWindow() const;
    void setRootWindow(Window* new_root);

    /*!
    \brief
        Internal function to directly set the current modal window.

    \note
        This function is called internally by Window, and should not be called
        by client code.  Doing so will likely not give the expected results.
    */
    void setModalWindow(Window* window);

    //! Return a pointer to the Window that is currently set as modal.
    Window* getModalWindow() const;

    Window* getWindowContainingMouse() const;

    const Sizef& getSurfaceSize() const;

    const SystemKeys& getSystemKeys() const;

    //! call to indicate that some redrawing is required.
    void markAsDirty();
    bool isDirty() const;

    MouseCursor& getMouseCursor();
    const MouseCursor& getMouseCursor() const;

    void setMouseMoveScalingFactor(float factor);
    float getMouseMoveScalingFactor() const;

    void setMouseButtonClickTimeout(float seconds);
    float getMouseButtonClickTimeout() const;

    void setMouseButtonMultiClickTimeout(float seconds);
    float getMouseButtonMultiClickTimeout() const;

    void setMouseButtonMultiClickTolerance(const Sizef& sz);
    const Sizef& getMouseButtonMultiClickTolerance() const;

    /*!
    \brief
        Set whether automatic mouse button click and multi-click (i.e.
        double-click and treble-click) event generation will occur.

    \param enable
        - true to have mouse button click and multi-click events automatically
        generated by the system from the basic button up and down event
        injections.
        - false if no automatic generation of events should occur.  In this
        instance the user may wish to use the additional event injectors to
        manually inform the system of such events.
    */
    void setMouseClickEventGenerationEnabled(const bool enable);

    /*!
    \brief
        Return whether automatic mouse button click and multi-click (i.e.
        double-click and treble-click) event generation is enabled.

    \return
        - true if mouse button click and multi-click events will be
        automatically generated by the system from the basic button up and down
        event injections.
        - false if no automatic generation of events will occur.  In this
        instance the user may wish to use the additional event injectors to
        manually inform the system of such events.
    */
    bool isMouseClickEventGenerationEnabled() const;
    //! returns whether the window containing the mouse had changed.
    bool updateWindowContainingMouse();

    Window* getInputCaptureWindow() const;
    void setInputCaptureWindow(Window* window);

    /*!
    \brief
        Set the default Tooltip object for this GUIContext. This value may be 0
        to indicate that no default Tooltip object will be available.

    \param tooltip
        Pointer to a valid Tooltip based object which should be used as the
        default tooltip for the GUIContext, or 0 to indicate that no default
        Tooltip is required.

    \note
        When passing a pointer to a Tooltip object, ownership of the Tooltip
        does not pass to the GUIContext.
    */
    void setDefaultTooltipObject(Tooltip* tooltip);

    /*!
    \brief
        Set the default Tooltip to be used by specifying a Window type.

        The GUIContext will internally attempt to create an instance of the
        specified window type (which must be derived from the base Tooltip
        class).  If the Tooltip creation fails, the error is logged and no
        default Tooltip will be available on the GUIContext.

    \param tooltipType
        String holding the name of a Tooltip based Window type.
    */
    void setDefaultTooltipType(const String& tooltip_type);

    //! Returns a pointer to the context's default tooltip object.  May return 0.
    Tooltip* getDefaultTooltipObject() const;

    // Implementation of InjectedInputReceiver interface
    bool injectMouseMove(float delta_x, float delta_y);
    bool injectMouseLeaves(void);
    bool injectMouseButtonDown(MouseButton button);
    bool injectMouseButtonUp(MouseButton button);
    bool injectKeyDown(Key::Scan scan_code);
    bool injectKeyUp(Key::Scan scan_code);
    bool injectChar(String::value_type code_point);
    bool injectMouseWheelChange(float delta);
    bool injectMousePosition(float x_pos, float y_pos);
    bool injectTimePulse(float timeElapsed);
    bool injectMouseButtonClick(const MouseButton button);
    bool injectMouseButtonDoubleClick(const MouseButton button);
    bool injectMouseButtonTripleClick(const MouseButton button);
    bool injectCopyRequest();
    bool injectCutRequest();
    bool injectPasteRequest();

    // public overrides
    void draw();

protected:
    void updateRootWindowAreaRects() const;
    void drawWindowContentToTarget();
    void renderWindowHierarchyToSurfaces();

    void createDefaultTooltipWindowInstance() const;
    void destroyDefaultTooltipWindowInstance();

    bool mouseMoveInjection_impl(MouseEventArgs& ma);
    Window* getTargetWindow(const Vector2f& pt, const bool allow_disabled) const;
    Window* getKeyboardTargetWindow() const;
    Window* getCommonAncestor(Window* w1, Window* w2);
    //! call some function for a chain of windows: (top, bottom]
    void notifyMouseTransition(Window* top, Window* bottom,
                               void (Window::*func)(MouseEventArgs&),
                               MouseEventArgs& args);

    bool areaChangedHandler(const EventArgs& args);
    bool windowDestroyedHandler(const EventArgs& args);

    // event trigger functions.
    virtual void onRootWindowChanged(WindowEventArgs& args);
    virtual void onMouseMoveScalingFactorChanged(GUIContextEventArgs& args);
    virtual void onMouseButtonClickTimeoutChanged(GUIContextEventArgs& args);
    virtual void onMouseButtonMultiClickTimeoutChanged(GUIContextEventArgs& args);
    virtual void onMouseButtonMultiClickToleranceChanged(GUIContextEventArgs& args);

    // protected overrides
    void drawContent();

    Window* d_rootWindow;
    bool d_isDirty;
    MouseCursor d_mouseCursor;
    //! Scaling factor applied to injected mouse move deltas.
    float d_mouseMovementScalingFactor;
    //! should mouse click/multi-click events be automatically generated.
    bool d_generateMouseClickEvents;
    //! Timeout used to when detecting a single-click.
    float d_mouseButtonClickTimeout;
    //! Timeout used when detecting multi-click events.
    float d_mouseButtonMultiClickTimeout;
    //! Movement tolerance used when detecting multi-click events.
    Sizef d_mouseButtonMultiClickTolerance;

    mutable Tooltip* d_defaultTooltipObject;
    mutable bool d_weCreatedTooltipObject;
    String d_defaultTooltipType;

    //! a cache of the target surface size, allows returning by ref.
    Sizef d_surfaceSize;

    Window* d_windowContainingMouse;
    Window* d_modalWindow;
    Window* d_captureWindow;

    SystemKeys d_systemKeys;
    MouseClickTracker* d_mouseClickTrackers;

    Event::ScopedConnection d_areaChangedEventConnection;
    Event::ScopedConnection d_windowDestroyedEventConnection;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

