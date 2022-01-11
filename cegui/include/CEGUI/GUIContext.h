/***********************************************************************
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
#include "CEGUI/InputEventReceiver.h"
#include "CEGUI/Cursor.h"
#include "CEGUI/InputEvent.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class WindowNavigator;

//! EventArgs class passed to subscribers for (most) GUIContext events.
class CEGUIEXPORT GUIContextEventArgs : public EventArgs
{
public:

    GUIContextEventArgs(GUIContext* context) : context(context) {}

    //! pointer to the GUIContext that triggered the event.
    GUIContext* context;
};

//! EventArgs class passed for GUIContext RenderTarget related events.
class CEGUIEXPORT GUIContextRenderTargetEventArgs : public GUIContextEventArgs
{
public:
    GUIContextRenderTargetEventArgs(GUIContext* context, RenderTarget* target) :
        GUIContextEventArgs(context),
        renderTarget(target)
    {}

    RenderTarget* renderTarget;
};

class CEGUIEXPORT GUIContext : public RenderingSurface,
                               public InputEventReceiver
{
public:
    /** Name of Event fired when the root window is changed to a different
     * Window.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the @e old root window (the new one is
     * obtained by calling GUIContext::getRootWindow).
     */
    static const String EventRootWindowChanged;
    /** Name of Event fired when the RenderTarget for the GUIContext is changed.
     * Handlers are passed a const GUIContextRenderTargetEventArgs struct, with
     * the renderTarget member set to the old RenderTarget.
     */
    static const String EventRenderTargetChanged;
    /** Event fired when the default font changes.
     * Handlers are passed a const reference to a generic EventArgs struct.
     */
    static const String EventDefaultFontChanged;
    /** Event fired when the tooltip is about to get activated.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip that is about to become
        * active.
        */
    static const String EventTooltipActive;
    /** Event fired when the tooltip has been deactivated.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip that has become inactive.
        */
    static const String EventTooltipInactive;
    /** Event fired when the tooltip changes target window but stays active.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip that has transitioned.
        */
    static const String EventTooltipTransition;

    GUIContext(RenderTarget& target);
    virtual ~GUIContext() override;

    Window* getRootWindow() const { return d_rootWindow; }
    void setRootWindow(Window* new_root);

    /*!
    \brief
        Activate the window giving it input focus.

    \param window
        Window to activate or nullptr to clear an active window.

    \return
        true if activation led to changes in Z-order
    */
    bool setActiveWindow(Window* window, bool moveToFront);

    //! Gets an active leaf window that owns an input focus when not overridden.
    Window* getActiveWindow() const { return d_activeWindow; }

    //! Checks if a window is in an active branch of the tree
    bool isWindowActive(const Window* window) const;

    //! \brief Controls whether the tooltip will follow the cursor
    void setMoveToFrontOnActivateAllowed(bool value) { d_moveToFrontOnActivateAllowed = value; }

    /*!
    \brief
        Set the modal state for this Window.

    \note
        This returns the previous modal window into normal state.

    \param state
        Boolean value defining if this Window should be the modal target.
        - true if this Window should be activated and set as the modal target.
        - false if the modal target should be cleared if this Window is
          currently the modal target.
    */
    void setModalWindow(Window* window);

    //! Return a pointer to the Window that is currently set as modal.
    Window* getModalWindow() const { return d_modalWindow; }

    Window* getInputCaptureWindow() const { return d_captureWindow; }
    bool captureInput(Window* window);
    void releaseInputCapture(bool allowRestoreOld = true, Window* exactWindow = nullptr);

    Window* getTargetWindow(const glm::vec2& pt, bool allow_disabled) const;
    Window* getWindowContainingCursor();

    const Sizef& getSurfaceSize() const { return d_surfaceSize; }

    //! call to indicate that some redrawing is required.
    void markAsDirty(std::uint32_t drawModeMask = DrawModeMaskAll) { d_dirtyDrawModeMask |= drawModeMask; }
    bool isDirty() const { return d_dirtyDrawModeMask != 0; }
    std::uint32_t getDirtyDrawModeMask() const { return d_dirtyDrawModeMask; }

    /*!
    \brief
        Retrieves Cursor used in this GUIContext

    \note
        Please note that each GUIContext has exactly one Cursor. The Cursor
        class holds position, as well as other properties. If you want to modify
        the Cursor (for example change its default image), you can retrieve
        a reference via this method and call a method on the reference
        (in our example that's setDefaultImage).
    */
    Cursor& getCursor() { return d_cursor; }
    const Cursor& getCursor() const { return d_cursor; }

    //! Tell the context to reconsider which window it thinks the cursor is in.
    void updateWindowContainingCursor() { d_windowContainingCursorIsUpToDate = false; }

    /*!
    \brief
        Set the default Tooltip to be used by specifying a Window type.

    \param tooltipType
        String holding the name of a Tooltip based Window type.
    */
    void setDefaultTooltipType(const String& tooltip_type) { d_defaultTooltipType = tooltip_type; }

    const String& getDefaultTooltipType() const { return d_defaultTooltipType; }
    Window* getTooltipObject(const String& type) const;
    Window* getOrCreateTooltipObject(const String& type);

    void setRenderTarget(RenderTarget& target);

    /*!
    \brief
        Set the default font to be used by the GUIContext

    \param name
        String object containing the name of the font to be used as the
        default for this GUIContext
    */
    void setDefaultFont(const String& name);

    /*!
    \brief
        Set the default font to be used by the GUIContext

    \param font
        Pointer to the font to be used as the default for this GUIContext.
    */
    void setDefaultFont(Font* font);

    /*!
    \brief
        Return a pointer to the default Font for the GUIContext

    \return
        Pointer to a Font object that is the default for this GUIContext.
    */
    Font* getDefaultFont() const;

    /*!
    \brief
        Function to inject time pulses into the receiver.

    \param timeElapsed
        float value indicating the amount of time passed, in seconds, since the last time this method was called.

    \return
        Currently, this method always returns true unless there is no root
        window or it is not effectively visible
    */
    bool injectTimePulse(float timeElapsed);

    // Implementation of InputEventReceiver interface
    bool injectInputEvent(const InputEvent& event) override;

    // public overrides
    void draw(std::uint32_t drawMode = DrawModeMaskAll) override;

    //! \brief Sets a window navigator to be used for navigating in this context
    void setWindowNavigator(WindowNavigator* navigator) { d_windowNavigator = navigator; }

    /*!
    \brief
        Return the number of seconds the cursor should hover stationary
        over the target window before the tooltip gets activated.
    */
    float getTooltipHoverTime() const { return d_tooltipHoverTime; }

    /*!
    \brief
        Set the number of seconds the cursor should hover stationary over
        the target window before the tooltip gets activated.
    */
    void setTooltipHoverTime(float seconds) { d_tooltipHoverTime = seconds; }

    /*!
    \brief
        Return the number of seconds the tooltip should be displayed for before it automatically
        de-activates itself. 0 indicates that the tooltip never timeout and auto-deactivates.
    */
    float getTooltipDisplayTime() const { return d_tooltipDisplayTime; }

    /*!
    \brief
        Set the number of seconds the tooltip should be displayed for before it automatically
        de-activates itself. 0 indicates that the tooltip should never timeout and auto-deactivate.
    */
    void setTooltipDisplayTime(float seconds) { d_tooltipDisplayTime = seconds; }

    //! \brief Controls whether the tooltip will follow the cursor
    void setTooltipFollowsCursor(bool value) { d_tooltipFollowsCursor = value; }

    //! \brief Immediately updates a tooltip position according to the cursor
    void positionTooltip();

    void onWindowDetached(Window* window);

protected:

    void drawWindowContentToTarget(std::uint32_t drawModeMask);

    //! returns the window used as input target
    Window* getInputTargetWindow() const;
    //! call some function for a chain of windows: (top, bottom]
    void notifyCursorTransition(Window* top, Window* bottom,
                               void (Window::*func)(CursorInputEventArgs&),
                               CursorInputEventArgs& args) const;

    void showTooltip(bool force);
    void hideTooltip(bool force);
    void updateTooltipState(float timeElapsed);

    void updateInputAutoRepeating(float timeElapsed);

    bool areaChangedHandler(const EventArgs& args);
    bool fontRenderSizeChangedHandler(const EventArgs& args);

    //! returns whether the window containing the cursor had changed.
    void updateWindowContainingCursor_impl(Window* windowWithCursor);

    // protected overrides
    void drawContent(std::uint32_t drawModeMask = DrawModeMaskAll) override;

    // Input event handlers
    void initializeSemanticEventHandlers();
    bool handleCopyRequest(const SemanticInputEvent& event);
    bool handleCutRequest(const SemanticInputEvent& event);
    bool handlePasteRequest(const SemanticInputEvent& event);
    bool handleScrollEvent(const SemanticInputEvent& event);
    bool handleCursorMoveEvent(const SemanticInputEvent& event);
    bool handleCursorLeave(const SemanticInputEvent& event);
    bool handleCursorActivateEvent(const SemanticInputEvent& event);
    bool handleCursorPressHoldEvent(const SemanticInputEvent& event);
    bool handleSelectWord(const SemanticInputEvent& event);
    bool handleSelectAll(const SemanticInputEvent& event);
    bool handleUndoRequest(const SemanticInputEvent& event);
    bool handleRedoRequest(const SemanticInputEvent& event);

    Window* d_rootWindow = nullptr;
    Window* d_windowContainingCursor = nullptr;
    Window* d_modalWindow = nullptr;
    Window* d_captureWindow = nullptr;
    Window* d_oldCaptureWindow = nullptr;
    Window* d_tooltipWindow = nullptr;
    Window* d_tooltipSource = nullptr;
    Window* d_activeWindow = nullptr;
    WindowNavigator* d_windowNavigator = nullptr;

    Font* d_defaultFont = nullptr;
    Cursor d_cursor;
    CursorsState d_cursorsState;

    String d_defaultTooltipType;
    std::map<String, Window*> d_tooltips;

    Sizef d_surfaceSize; //!< a cache of the target surface size, allows returning by ref.

    //! The mask of draw modes that must be redrawn
    std::uint32_t d_dirtyDrawModeMask = 0;

    float d_tooltipTimer = 0.f;
    float d_tooltipHoverTime = 0.4f;   //!< seconds cursor must stay stationary before tip shows
    float d_tooltipDisplayTime = 7.5f; //!< seconds that tip is shown for

    float d_repeatElapsed = 0.f;
    CursorInputSource d_repeatPointerSource = CursorInputSource::NotSpecified;

    Event::ScopedConnection d_areaChangedEventConnection;
    Event::ScopedConnection d_fontRenderSizeChangeConnection;
    std::vector<Event::ScopedConnection> d_tooltipEventConnections;
    std::map<SemanticValue, SlotFunctorBase<InputEvent>*> d_semanticEventHandlers;

    bool d_windowContainingCursorIsUpToDate = true;
    bool d_tooltipFollowsCursor = false;
    bool d_moveToFrontOnActivateAllowed = true;
    bool d_repeating = false;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
