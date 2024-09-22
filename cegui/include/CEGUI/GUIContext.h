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
#include "CEGUI/InjectedInputReceiver.h"
#include "CEGUI/URect.h"
#include <chrono>

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
    GUIContext* context = nullptr;
};

class CEGUIEXPORT GUIContext : public RenderingSurface, public InjectedInputReceiver
{
public:
    /** Name of Event fired when the root window is changed to a different Window.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the @e old root window (the new one is
     * obtained by calling GUIContext::getRootWindow).
     */
    static const String EventRootWindowChanged;
    /** Name of Event fired when the cursor image is changed.
     * Handlers are passed a const GUIContextEventArgs reference
     */
    static const String EventCursorImageChanged;
    /** Name of Event fired when the default cursor image is changed.
     * Handlers are passed a const GUIContextEventArgs reference
     */
    static const String EventDefaultCursorImageChanged;
    /** Event fired when the default font changes.
     * Handlers are passed a const GUIContextEventArgs reference
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

    //! A rect that is used to unset cursor constraints
    static const URect NoCursorConstraint;

    static constexpr float DEFAULT_CLICK_DISTANCE = 5.f;
    static constexpr float DEFAULT_CLICK_TIMEOUT = 0.25f;

    GUIContext(RenderTarget& target);
    virtual ~GUIContext() override;

    void destroyTooltips();

    void draw(std::uint32_t drawMode = DrawModeMaskAll) override;

    void setRenderTarget(RenderTarget& target);

    //! call to indicate that some redrawing is required.
    void markAsDirty(std::uint32_t drawModeMask = DrawModeMaskAll) { d_dirtyDrawModeMask |= drawModeMask; }
    bool isDirty() const { return d_dirtyDrawModeMask != 0; }
    std::uint32_t getDirtyDrawModeMask() const { return d_dirtyDrawModeMask; }

    /*!
    \brief
        Function to inject time pulses into the context.

    \param timeElapsed
        float value indicating the amount of time passed, in seconds, since the last time this method was called.

    \return
        Currently, this method always returns true unless there is no root window or it is not visible
    */
    bool injectTimePulse(float timeElapsed);

    // Implementation of InjectedInputReceiver interface
    bool injectMousePosition(float x, float y) override;
    bool injectMouseMove(float dx, float dy) override;
    bool injectMouseLeaves() override;
    bool injectMouseWheelChange(float delta) override;
    bool injectMouseButtonDown(MouseButton button) override;
    bool injectMouseButtonUp(MouseButton button) override;
    bool injectMouseButtonClick(MouseButton button) override;
    bool injectMouseButtonDoubleClick(MouseButton button) override;
    bool injectMouseButtonTripleClick(MouseButton button) override;
    bool injectKeyDown(Key::Scan scanCode) override;
    bool injectKeyUp(Key::Scan scanCode) override;
    bool injectChar(char32_t codePoint) override;

    /*!
    \brief
        Set automatic mouse button click event generation mode. Click is generated when the same window
        receives down and up events for the same mouse button within time and cursor offset threshold.

    \param maxClicksToGenerate
        - 0 to disable auto-generation of clicks. Typically accompanies explicit injectMouseButtonClick calls.
        - 1 to generate only single click events (onClick).
        - 2 or 3 to also generate double (onDoubleClick) and triple clicks (onTripleClick).
        - higher values trigger onClick, d_generatedClickEventOrder can be examined to know the order of the click generated.
    */
    void setMouseClickEventGeneration(int maxClicksToGenerate) { d_clickLimit = maxClicksToGenerate; }
    //! \brief Returns automatic mouse button click event generation mode. See setMouseClickEventGeneration.
    int getMouseClickEventGeneration() const { return d_clickLimit; }

    void setMouseButtonMultiClickTimeout(float seconds) { d_multiClickTimeout = seconds; }
    float getMouseButtonMultiClickTimeout() const { return d_multiClickTimeout; }
    void setMouseButtonMultiClickTolerance(float pixels) { d_multiClickDistance = pixels; }
    float getMouseButtonMultiClickTolerance() const { return d_multiClickDistance; }

    // TODO StringAtom!
    void initDefaultInputSemantics();
    void registerInputSemantic(const String& value, Key::Scan scanCode, ModifierKeyRule modifiers = {});
    void registerInputSemantic(const String& value, MouseButton button, ModifierKeyRule modifiers = {}, int clickOrder = 0, MouseButtons buttons = {});
    void unregisterInputSemantic(const String& value);
    void unregisterAllInputSemantics();
    bool isInputSemantic(const String& value, const KeyEventArgs& args) const;
    bool isInputSemantic(const String& value, const MouseButtonEventArgs& args) const;

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

    //! \brief Controls rising windows to frontmost Z-order on activation
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

    \return
        true if the \a window became modal for this context
    */
    bool setModalWindow(Window* window);

    //! Return a pointer to the Window that is currently set as modal.
    Window* getModalWindow() const { return d_modalWindow; }

    Window* getInputCaptureWindow() const { return d_captureWindow; }
    bool captureInput(Window* window);
    void releaseInputCapture(bool allowRestoreOld = true, Window* exactWindow = nullptr);

    Window* getWindowContainingCursor();
    //! Tell the context to reconsider which window it thinks the cursor is in.
    void updateWindowContainingCursor() { d_windowContainingCursorIsUpToDate = false; }

    //! Return currently pressed modifier keys
    ModifierKeys getModifierKeys() const { return d_modifierKeys; }
    //! Return currently pressed mouse buttons
    MouseButtons getMouseButtons() const { return d_mouseButtons; }

    void setCursorImage(const Image* image);
    const Image* getCursorImage() const { return d_cursorImage; }

    /*!
    \brief
        Set the image to be used as the default cursor.

    \param image
        Pointer to an image object that is to be used as the default cursor
        indicator. To have no indicator rendered by default, you can specify nullptr here.
    */
    void setDefaultCursorImage(const Image* image);
    void setDefaultCursorImage(const String& name);
    const Image* getDefaultCursorImage() const { return d_defaultCursorImage; }

    void setCursorVisible(bool visible) { d_cursorVisible = visible; }
    bool isCursorVisible() const { return d_cursorVisible; }

    /*!
    \brief
        Set an explicit size for the cursor image to be drawn at.
        Set any dimension to 0 to use an original image size for it.
    */
    void setCursorSize(float w, float h);
    const Sizef& getCursorSize() const { return d_cursorSize; }

    /*!
    \brief
        Set the current cursor position

    \param position
        Point object describing the new location for the cursor. This will
        be clipped to within the renderer screen area.
    */
    void setCursorPosition(const glm::vec2& position);
    const glm::vec2& getCursorPosition() const { return d_cursorPosition; }

    /*!
    \brief
        Set the area that the cursor is constrained to.

    \param area
        Pointer to a URect object that describes the area of the display that
        the cursor is allowed to occupy. The given area will be clipped to the
        current Renderer screen area - it is never possible for the cursor to
        leave this area. If this parameter is NULL, the constraint is set to
        the size of the current Renderer screen area.
    */
    void setCursorConstraintArea(const URect& area = NoCursorConstraint);
    const URect& getCursorConstraintArea() const { return d_cursorConstraints; }

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
        Set the default Tooltip to be used by specifying a Window type.

    \param tooltipType
        String holding the name of a Tooltip based Window type.
    */
    void setDefaultTooltipType(const String& tooltip_type) { d_defaultTooltipType = tooltip_type; }

    const String& getDefaultTooltipType() const { return d_defaultTooltipType; }
    Window* getTooltipObject(const String& type) const;
    Window* getOrCreateTooltipObject(const String& type);

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

    //! \brief Sets a window navigator to be used for navigating in this context
    void setWindowNavigator(WindowNavigator* navigator) { d_windowNavigator = navigator; }

    const Sizef& getSurfaceSize() const { return d_surfaceSize; }

    void onWindowDetached(Window* window);

protected:

    Window* getInputTargetWindow() const;
    void updateInputAutoRepeating(float timeElapsed);
    bool sendScrollEvent(float delta, Window* window);

    void drawWindowContentToTarget(std::uint32_t drawModeMask);
    void drawContent(std::uint32_t drawModeMask = DrawModeMaskAll) override;

    //! call some function for a chain of windows: (top, bottom]
    void notifyCursorTransition(Window* top, Window* bottom, void (Window::* func)(CursorInputEventArgs&), CursorInputEventArgs& args) const;
    bool checkClickLocality() const;
    void resetClickTracker();

    void showTooltip(bool force);
    void hideTooltip(bool force);
    void updateTooltipState(float timeElapsed);

    bool areaChangedHandler(const EventArgs& args);
    bool fontRenderSizeChangedHandler(const EventArgs& args);

    //! returns whether the window containing the cursor had changed.
    void updateWindowContainingCursorInternal(Window* windowWithCursor);
    Window* getCursorTargetWindow(const glm::vec2& pt, bool allow_disabled) const;

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
    const Image* d_cursorImage = nullptr;
    const Image* d_defaultCursorImage = nullptr;
    std::vector<GeometryBuffer*> d_cursorGeometry;

    String d_defaultTooltipType;
    std::map<String, Window*> d_tooltips;

    MouseButtons d_mouseButtons;
    ModifierKeys d_modifierKeys;
    std::vector<KeySemanticMapping> d_keySemantics;
    std::vector<MouseButtonSemanticMapping> d_mouseSemantics;

    float d_multiClickDistance = DEFAULT_CLICK_DISTANCE;
    float d_multiClickTimeout = DEFAULT_CLICK_TIMEOUT;   //!< Zero means no timeout
    int d_clickLimit = 3;                                //!< Use 0 to disable click events, 1 to enable only single clicks

    struct ClickTracker
    {
        Window* firstWindow = nullptr;
        Window* lastWindow = nullptr;
        glm::vec2 downPos;
        std::chrono::time_point<std::chrono::steady_clock> downTime;
        int clickCount = 0;
        MouseButton button = MouseButton::Invalid;
    };
    ClickTracker d_clickTracker;

    Sizef d_surfaceSize; //!< a cache of the target surface size, allows returning by ref.
    Sizef d_cursorSize;
    glm::vec2 d_cursorPosition;
    URect d_cursorConstraints;

    std::uint32_t d_dirtyDrawModeMask = 0; //!< the mask of draw modes that must be redrawn

    float d_tooltipTimer = 0.f;
    float d_tooltipHoverTime = 0.4f;   //!< seconds cursor must stay stationary before tip shows
    float d_tooltipDisplayTime = 7.5f; //!< seconds that tip is shown for

    Event::ScopedConnection d_areaChangedEventConnection;
    Event::ScopedConnection d_fontRenderSizeChangeConnection;
    std::vector<Event::ScopedConnection> d_tooltipEventConnections;

    float d_autoRepeatElapsed = 0.f;
    MouseButton d_autoRepeatMouseButton = MouseButton::Invalid;
    bool d_autoRepeating = false;

    bool d_cursorVisible = true;
    bool d_cursorDirty = false;
    bool d_windowContainingCursorIsUpToDate = true;
    bool d_tooltipFollowsCursor = false;
    bool d_moveToFrontOnActivateAllowed = true;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
