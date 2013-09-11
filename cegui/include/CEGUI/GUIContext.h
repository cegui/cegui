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
#include "CEGUI/InputEventReceiver.h"
#include "CEGUI/SemanticInputEvent.h"
#include "CEGUI/PointerIndicator.h"
#include "CEGUI/SystemKeys.h"
#include "CEGUI/WindowNavigator.h"

#include <map>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
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

    Window* getWindowContainingPointer() const;

    const Sizef& getSurfaceSize() const;

    const SystemKeys& getSystemKeys() const;

    //! call to indicate that some redrawing is required.
    void markAsDirty();
    bool isDirty() const;

    PointerIndicator& getPointerIndicator();
    const PointerIndicator& getPointerIndicator() const;

    //! Tell the context to reconsider which window it thinks the pointer is in.
    void updateWindowContainingPointer();

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
    bool injectInputEvent(const InputEvent& event);

    bool injectUndoRequest();
    bool injectRedoRequest();

    // public overrides
    void draw();

    /*!
    \brief
        Sets a window navigator to be used for navigating in this context
    */
    void setWindowNavigator(WindowNavigator* navigator);

protected:
    void updateRootWindowAreaRects() const;
    void drawWindowContentToTarget();
    void renderWindowHierarchyToSurfaces();

    void createDefaultTooltipWindowInstance() const;
    void destroyDefaultTooltipWindowInstance();

    //! notify windows in a hierarchy using default font, when font changes.
    void notifyDefaultFontChanged(Window* hierarchy_root) const;

    Window* getTargetWindow(const Vector2f& pt, const bool allow_disabled) const;
    Window* getKeyboardTargetWindow() const;
    Window* getCommonAncestor(Window* w1, Window* w2) const;
    //! call some function for a chain of windows: (top, bottom]
    void notifyPointerTransition(Window* top, Window* bottom,
                               void (Window::*func)(PointerEventArgs&),
                               PointerEventArgs& args) const;

    bool areaChangedHandler(const EventArgs& args);
    bool windowDestroyedHandler(const EventArgs& args);

    //! returns whether the window containing the pointer had changed.
    bool updateWindowContainingPointer_impl() const;
    void resetWindowContainingPointer();

    // event trigger functions.
    virtual void onRootWindowChanged(WindowEventArgs& args);
    virtual void onRenderTargetChanged(GUIContextRenderTargetEventArgs& args);
    virtual void onDefaultFontChanged(EventArgs& args);

    // protected overrides
    void drawContent();

    // Input event handlers
    void initializeSemanticEventHandlers();

    bool handleTextInputEvent(const TextInputEvent& event);
    bool handleSemanticInputEvent(const SemanticInputEvent& event);
    bool handleCopyRequest(const SemanticInputEvent& event);
    bool handleCutRequest(const SemanticInputEvent& event);
    bool handlePasteRequest(const SemanticInputEvent& event);
    bool handleScrollEvent(const SemanticInputEvent& event);
    bool handlePointerMoveEvent(const SemanticInputEvent& event);
    bool handlePointerMove_impl(PointerEventArgs& pa);
    bool handlePointerLeave(const SemanticInputEvent& event);
    bool handlePointerActivateEvent(const SemanticInputEvent& event);
    bool handlePointerPressHoldEvent(const SemanticInputEvent& event);

    Window* d_rootWindow;
    bool d_isDirty;
    PointerIndicator d_pointerIndicator;

    mutable Tooltip* d_defaultTooltipObject;
    mutable bool d_weCreatedTooltipObject;
    String d_defaultTooltipType;

    Font* d_defaultFont;

    //! a cache of the target surface size, allows returning by ref.
    Sizef d_surfaceSize;

    mutable Window* d_windowContainingPointer;
    mutable bool    d_windowContainingPointerIsUpToDate;
    Window* d_modalWindow;
    Window* d_captureWindow;

    PointersState d_pointersState;
    SystemKeys d_systemKeys;

    Event::ScopedConnection d_areaChangedEventConnection;
    Event::ScopedConnection d_windowDestroyedEventConnection;
    std::map<int, SlotFunctorBase<InputEvent>*> d_semanticEventHandlers;

    //! the window navigator (if any) used to navigate the GUI
    WindowNavigator* d_windowNavigator;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

