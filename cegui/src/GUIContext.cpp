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
#include "CEGUI/GUIContext.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/RenderingWindow.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Window.h"
#include "CEGUI/widgets/Tooltip.h"
#include "CEGUI/SimpleTimer.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4355)
#endif

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String GUIContext::EventRootWindowChanged("RootWindowChanged");
const String GUIContext::EventRenderTargetChanged("RenderTargetChanged");
const String GUIContext::EventDefaultFontChanged("DefaultFontChanged");

//----------------------------------------------------------------------------//
GUIContext::GUIContext(RenderTarget& target) :
    RenderingSurface(target),
    d_rootWindow(nullptr),
    d_isDirty(false),
    d_defaultTooltipObject(nullptr),
    d_weCreatedTooltipObject(false),
    d_defaultFont(nullptr),
    d_surfaceSize(target.getArea().getSize()),
    d_modalWindow(nullptr),
    d_captureWindow(nullptr),
    d_areaChangedEventConnection(
        target.subscribeEvent(
            RenderTarget::EventAreaChanged,
            Event::Subscriber(&GUIContext::areaChangedHandler, this))),
    d_windowDestroyedEventConnection(
        WindowManager::getSingleton().subscribeEvent(
            WindowManager::EventWindowDestroyed,
            Event::Subscriber(&GUIContext::windowDestroyedHandler, this))),
    d_semanticEventHandlers(),
    d_windowNavigator(nullptr)
{
    resetWindowContainingCursor();
    initializeSemanticEventHandlers();
}

//----------------------------------------------------------------------------//
void GUIContext::resetWindowContainingCursor()
{
    d_windowContainingCursor = nullptr;
    d_windowContainingCursorIsUpToDate = true;
}

//----------------------------------------------------------------------------//
GUIContext::~GUIContext()
{
    destroyDefaultTooltipWindowInstance();
    deleteSemanticEventHandlers();

    if (d_rootWindow)
        d_rootWindow->setGUIContext(nullptr);
}

//----------------------------------------------------------------------------//
Window* GUIContext::getRootWindow() const
{
    return d_rootWindow;
}

//----------------------------------------------------------------------------//
void GUIContext::setRootWindow(Window* new_root)
{
    if (d_rootWindow == new_root)
        return;

    if (d_rootWindow)
        d_rootWindow->setGUIContext(nullptr);

    WindowEventArgs args(d_rootWindow);

    d_rootWindow = new_root;

    if (d_rootWindow)
    {
        d_rootWindow->setGUIContext(this);
        d_rootWindow->syncTargetSurface();
    }

    onRootWindowChanged(args);
}

//----------------------------------------------------------------------------//
void GUIContext::updateRootWindowAreaRects() const
{
    ElementEventArgs args(nullptr);
    d_rootWindow->onParentSized(args);
}

//----------------------------------------------------------------------------//
Window* GUIContext::getInputCaptureWindow() const
{
    return d_captureWindow;
}

//----------------------------------------------------------------------------//
void GUIContext::setInputCaptureWindow(Window* window)
{
    d_captureWindow = window;
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultTooltipObject(Tooltip* tooltip)
{
    destroyDefaultTooltipWindowInstance();

    d_defaultTooltipObject = tooltip;

    if (d_defaultTooltipObject)
        d_defaultTooltipObject->setWritingXMLAllowed(false);
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultTooltipType(const String& tooltip_type)
{
    destroyDefaultTooltipWindowInstance();

    d_defaultTooltipType = tooltip_type;
}

//----------------------------------------------------------------------------//
void GUIContext::destroyDefaultTooltipWindowInstance()
{
    if (d_defaultTooltipObject && d_weCreatedTooltipObject)
    {
        WindowManager::getSingleton().destroyWindow(d_defaultTooltipObject);
        d_defaultTooltipObject = nullptr;
    }

    d_weCreatedTooltipObject = false;
}

//----------------------------------------------------------------------------//
Tooltip* GUIContext::getDefaultTooltipObject() const
{
    if (!d_defaultTooltipObject && !d_defaultTooltipType.empty())
        createDefaultTooltipWindowInstance();

    return d_defaultTooltipObject;
}

//----------------------------------------------------------------------------//
void GUIContext::createDefaultTooltipWindowInstance() const
{
    WindowManager& winmgr(WindowManager::getSingleton());

    if (winmgr.isLocked())
        return;

    d_defaultTooltipObject = dynamic_cast<Tooltip*>(
        winmgr.createWindow(d_defaultTooltipType,
                            "CEGUI::System::default__auto_tooltip__"));
    d_defaultTooltipObject->setAutoWindow(true);

    if (d_defaultTooltipObject)
    {
        d_defaultTooltipObject->setWritingXMLAllowed(false);
        d_weCreatedTooltipObject = true;
    }
}

//----------------------------------------------------------------------------//
void GUIContext::setModalWindow(Window* window)
{
    d_modalWindow = window;
}

//----------------------------------------------------------------------------//
Window* GUIContext::getModalWindow() const
{
    return d_modalWindow;
}

//----------------------------------------------------------------------------//
Window* GUIContext::getWindowContainingCursor() const
{
    if (!d_windowContainingCursorIsUpToDate)
    {
        updateWindowContainingCursor_impl();
        d_windowContainingCursorIsUpToDate = true;
    }

    return d_windowContainingCursor;
}

//----------------------------------------------------------------------------//
const Sizef& GUIContext::getSurfaceSize() const
{
    return d_surfaceSize;
}

//----------------------------------------------------------------------------//
void GUIContext::markAsDirty()
{
    d_isDirty = true;
}

//----------------------------------------------------------------------------//
bool GUIContext::isDirty() const
{
    return d_isDirty;
}

//----------------------------------------------------------------------------//
void GUIContext::draw()
{
    if (d_isDirty)
        drawWindowContentToTarget();

    RenderingSurface::draw();
}

//----------------------------------------------------------------------------//
void GUIContext::drawContent()
{
    RenderingSurface::drawContent();

    d_cursor.draw();
}

//----------------------------------------------------------------------------//
void GUIContext::drawWindowContentToTarget()
{
    if (d_rootWindow)
        renderWindowHierarchyToSurfaces();
    else
        clearGeometry();

    d_isDirty = false;
}

//----------------------------------------------------------------------------//
void GUIContext::renderWindowHierarchyToSurfaces()
{
    RenderingSurface& rs = d_rootWindow->getTargetRenderingSurface();
    rs.clearGeometry();

    if (rs.isRenderingWindow())
        static_cast<RenderingWindow&>(rs).getOwner().clearGeometry();

    d_rootWindow->draw();
}

//----------------------------------------------------------------------------//
Cursor& GUIContext::getCursor()
{
    return const_cast<Cursor&>(
        static_cast<const GUIContext*>(this)->getCursor());
}

//----------------------------------------------------------------------------//
const Cursor& GUIContext::getCursor() const
{
    return d_cursor;
}

//----------------------------------------------------------------------------//
bool GUIContext::areaChangedHandler(const EventArgs&)
{
    d_surfaceSize = d_target->getArea().getSize();
    d_cursor.notifyDisplaySizeChanged(d_surfaceSize);

    if (d_rootWindow)
        updateRootWindowAreaRects();

    return true;
}

//----------------------------------------------------------------------------//
bool GUIContext::windowDestroyedHandler(const EventArgs& args)
{
    const Window* const window =
        static_cast<const WindowEventArgs&>(args).window;

    if (window == d_rootWindow)
        d_rootWindow = nullptr;

    if (window == getWindowContainingCursor())
        resetWindowContainingCursor();

    if (window == d_modalWindow)
        d_modalWindow = nullptr;

    if (window == d_captureWindow)
        d_captureWindow = nullptr;

    if (window == d_defaultTooltipObject)
    {
        d_defaultTooltipObject = nullptr;
        d_weCreatedTooltipObject = false;
    }

    return true;
}

//----------------------------------------------------------------------------//
void GUIContext::onRootWindowChanged(WindowEventArgs& args)
{
    if (d_rootWindow)
        updateRootWindowAreaRects();

    markAsDirty();

    fireEvent(EventRootWindowChanged, args);
}

//----------------------------------------------------------------------------//
void GUIContext::updateWindowContainingCursor()
{
    d_windowContainingCursorIsUpToDate = false;
}

//----------------------------------------------------------------------------//
bool GUIContext::updateWindowContainingCursor_impl() const
{
    CursorInputEventArgs ciea(nullptr);
    const glm::vec2 cursor_pos(d_cursor.getPosition());

    Window* const window_with_cursor = getTargetWindow(cursor_pos, true);

    // exit if window containing cursor has not changed.
    if (window_with_cursor == d_windowContainingCursor)
        return false;

    ciea.scroll = 0;
    ciea.source = CursorInputSource::NotSpecified;

    Window* oldWindow = d_windowContainingCursor;
    d_windowContainingCursor = window_with_cursor;

    // inform previous window the cursor has left it
    if (oldWindow)
    {
        ciea.window = oldWindow;
        ciea.position = oldWindow->getUnprojectedPosition(cursor_pos);
        oldWindow->onCursorLeaves(ciea);
    }

    // inform window containing cursor that cursor has entered it
    if (d_windowContainingCursor)
    {
        ciea.handled = 0;
        ciea.window = d_windowContainingCursor;
        ciea.position = d_windowContainingCursor->getUnprojectedPosition(cursor_pos);
        d_windowContainingCursor->onCursorEnters(ciea);
    }

    // do the 'area' version of the events
    Window* root = getCommonAncestor(oldWindow, d_windowContainingCursor);

    if (oldWindow)
        notifyCursorTransition(root, oldWindow, &Window::onCursorLeavesArea, ciea);

    if (d_windowContainingCursor)
        notifyCursorTransition(root, d_windowContainingCursor, &Window::onCursorEntersArea, ciea);

    return true;
}

//----------------------------------------------------------------------------//
Window* GUIContext::getCommonAncestor(Window* w1, Window* w2) const
{
    if (!w2)
        return w2;

    if (w1 == w2)
        return w1;

    // make sure w1 is always further up
    if (w1 && w1->isAncestor(w2))
        return w2;

    while (w1)
    {
        if (w2->isAncestor(w1))
            break;

        w1 = w1->getParent();
    }

    return w1;
}

//----------------------------------------------------------------------------//
void GUIContext::notifyCursorTransition(Window* top, Window* bottom,
                                    void (Window::*func)(CursorInputEventArgs&),
                                    CursorInputEventArgs& args) const
{
    if (top == bottom)
        return;

    Window* const parent = bottom->getParent();

    if (parent && parent != top)
        notifyCursorTransition(top, parent, func, args);

    args.handled = 0;
    args.window = bottom;

    (bottom->*func)(args);
}

//----------------------------------------------------------------------------//
Window* GUIContext::getTargetWindow(const glm::vec2& pt,
                                    const bool allow_disabled) const
{
    // if there is no GUI sheet visible, then there is nowhere to send input
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return nullptr;

    Window* dest_window = d_captureWindow;

    if (!dest_window)
    {
        dest_window = d_rootWindow->
            getTargetChildAtPosition(pt, allow_disabled);

        if (!dest_window)
            dest_window = d_rootWindow;
    }
    else
    {
        if (dest_window->distributesCapturedInputs())
        {
            Window* child_window = dest_window->
                getTargetChildAtPosition(pt, allow_disabled);

            if (child_window)
                dest_window = child_window;
        }
    }

    // modal target overrules
    if (d_modalWindow && dest_window != d_modalWindow)
        if (!dest_window->isAncestor(d_modalWindow))
            dest_window = d_modalWindow;

    return dest_window;
}

//----------------------------------------------------------------------------//
Window* GUIContext::getInputTargetWindow() const
{
    // if no active sheet, there is no target widow.
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return nullptr;

    // handle normal non-modal situations
    if (!d_modalWindow)
        return d_rootWindow->getActiveChild();

    // handle possible modal window.
    Window* const target = d_modalWindow->getActiveChild();
    return target ? target : d_modalWindow;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectInputEvent(const InputEvent& event)
{
    if (event.d_eventType == InputEventType::TextInputEventType)
        return handleTextInputEvent(static_cast<const TextInputEvent&>(event));

    if (event.d_eventType == InputEventType::SemanticInputEventType)
    {
        const SemanticInputEvent& semantic_event =
            static_cast<const SemanticInputEvent&>(event);

        if (d_windowNavigator != nullptr)
            d_windowNavigator->handleSemanticEvent(semantic_event);

        return handleSemanticInputEvent(semantic_event);
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectTimePulse(float timeElapsed)
{
    // if no visible active sheet, input can't be handled
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return false;

    // ensure window containing cursor is now valid
    getWindowContainingCursor();

    // else pass to sheet for distribution.
    d_rootWindow->update(timeElapsed);
    // this input is then /always/ considered handled.
    return true;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCopyRequest(const SemanticInputEvent&)
{
    Window* source = getInputTargetWindow();
    return source ? source->performCopy(*System::getSingleton().getClipboard()) : false;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCutRequest(const SemanticInputEvent&)
{
    Window* source = getInputTargetWindow();
    return source ? source->performCut(*System::getSingleton().getClipboard()) : false;
}

//----------------------------------------------------------------------------//
bool GUIContext::handlePasteRequest(const SemanticInputEvent&)
{
    Window* target = getInputTargetWindow();
    return target ? target->performPaste(*System::getSingleton().getClipboard()) : false;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleUndoRequest(const SemanticInputEvent&)
{
    Window* target = getInputTargetWindow();
    return target ? target->performUndo() : false;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleRedoRequest(const SemanticInputEvent&)
{
    Window* target = getInputTargetWindow();
    return target ? target->performRedo() : false;
}

//----------------------------------------------------------------------------//
void GUIContext::setRenderTarget(RenderTarget& target)
{
    if (d_target == &target)
        return;

    RenderTarget* const old_target = d_target;
    d_target = &target;

    if (d_rootWindow)
        d_rootWindow->syncTargetSurface();

    d_areaChangedEventConnection.disconnect();
    d_areaChangedEventConnection = d_target->subscribeEvent(
            RenderTarget::EventAreaChanged,
            Event::Subscriber(&GUIContext::areaChangedHandler, this));

    EventArgs area_args;
    areaChangedHandler(area_args);

    GUIContextRenderTargetEventArgs change_args(this, old_target);
    onRenderTargetChanged(change_args);
}

//----------------------------------------------------------------------------//
void GUIContext::onRenderTargetChanged(GUIContextRenderTargetEventArgs& args)
{
    fireEvent(EventRenderTargetChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultFont(const String& name)
{
    if (name.empty())
        setDefaultFont(nullptr);
    else
        setDefaultFont(&FontManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultFont(Font* font)
{
    d_defaultFont = font;

    EventArgs args;
    onDefaultFontChanged(args);
}

//----------------------------------------------------------------------------//
Font* GUIContext::getDefaultFont() const
{
    if (d_defaultFont)
        return d_defaultFont;

    // if no explicit default, we will return the first font we can get from
    // the font manager
    const FontManager::FontRegistry& registeredFonts = FontManager::getSingleton().getRegisteredFonts();

    FontManager::FontRegistry::const_iterator iter = registeredFonts.begin();

    return (iter != registeredFonts.end()) ? iter->second : 0;
}

//----------------------------------------------------------------------------//
void GUIContext::onDefaultFontChanged(EventArgs& args)
{
    if (d_rootWindow)
        notifyDefaultFontChanged(d_rootWindow);

    fireEvent(EventDefaultFontChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void GUIContext::notifyDefaultFontChanged(Window* hierarchy_root) const
{
    WindowEventArgs evt_args(hierarchy_root);

    if (!hierarchy_root->getFont(false))
        hierarchy_root->onFontChanged(evt_args);

    for (size_t i = 0; i < hierarchy_root->getChildCount(); ++i)
        notifyDefaultFontChanged(hierarchy_root->getChildAtIdx(i));
}

//----------------------------------------------------------------------------//
bool GUIContext::handleTextInputEvent(const TextInputEvent& event)
{
    TextEventArgs args(getInputTargetWindow());

    // if there's no destination window, input can't be handled.
    if (!args.window)
        return false;

    args.d_character = event.d_character;

    args.window->onCharacter(args);
    return args.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleSemanticInputEvent(const SemanticInputEvent& event)
{
    // dispatch to a handler if we have one
    std::map<SemanticValue, SlotFunctorBase<InputEvent>*>::const_iterator itor =
        d_semanticEventHandlers.find(event.d_value);
    if (itor != d_semanticEventHandlers.end())
    {
        return (*(*itor).second)(event);
    }

    Window* targetWindow = getInputTargetWindow();
    // window navigator's window takes precedence
    if (d_windowNavigator != nullptr)
        targetWindow = d_windowNavigator->getCurrentFocusedWindow();

    if (targetWindow != nullptr)
    {
        SemanticEventArgs args(targetWindow);

        args.d_payload = event.d_payload;
        args.d_semanticValue = event.d_value;

        args.window->onSemanticInputEvent(args);

        return args.handled != 0;
    }

    return false;
}

//----------------------------------------------------------------------------//
void GUIContext::initializeSemanticEventHandlers()
{
    d_semanticEventHandlers.insert(
        std::make_pair(SemanticValue::Undo,
                       new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
                           &GUIContext::handleUndoRequest, this)));
    d_semanticEventHandlers.insert(
        std::make_pair(SemanticValue::Redo,
                       new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
                           &GUIContext::handleRedoRequest, this)));

    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::Cut,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleCutRequest, this)));
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::Copy,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleCopyRequest, this)));
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::Paste,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handlePasteRequest, this)));
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::VerticalScroll,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleScrollEvent, this)));

    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::CursorActivate,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
        &GUIContext::handleCursorActivateEvent, this)));
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::CursorPressHold,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
        &GUIContext::handleCursorPressHoldEvent, this)));
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::CursorMove,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleCursorMoveEvent, this)));
}

//----------------------------------------------------------------------------//
void GUIContext::deleteSemanticEventHandlers()
{
    for (std::map<SemanticValue, SlotFunctorBase<InputEvent>*>::iterator itor = d_semanticEventHandlers.begin();
        itor != d_semanticEventHandlers.end(); ++itor)
    {
        delete itor->second;
    }
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorActivateEvent(const SemanticInputEvent& event)
{
    CursorInputEventArgs ciea(nullptr);
    ciea.position = d_cursor.getPosition();
    ciea.moveDelta = glm::vec2(0, 0);
    ciea.source = event.d_payload.source;
    ciea.scroll = 0;
    ciea.window = getTargetWindow(ciea.position, false);
    // make cursor position sane for this target window
    if (ciea.window)
        ciea.position = ciea.window->getUnprojectedPosition(ciea.position);

    // if there is no target window, input can not be handled.
    if (!ciea.window)
        return false;

    if (d_windowNavigator != nullptr)
        d_windowNavigator->setCurrentFocusedWindow(ciea.window);

    ciea.window->onCursorActivate(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorPressHoldEvent(const SemanticInputEvent& event)
{
    CursorInputEventArgs ciea(nullptr);
    ciea.position = d_cursor.getPosition();
    ciea.moveDelta = glm::vec2(0, 0);
    ciea.source = event.d_payload.source;
    ciea.scroll = 0;
    ciea.window = getTargetWindow(ciea.position, false);
    // make cursor position sane for this target window
    if (ciea.window)
        ciea.position = ciea.window->getUnprojectedPosition(ciea.position);

    if (d_windowNavigator != nullptr)
        d_windowNavigator->setCurrentFocusedWindow(ciea.window);

    ciea.window->onCursorPressHold(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleScrollEvent(const SemanticInputEvent& event)
{
    CursorInputEventArgs ciea(nullptr);
    ciea.position = d_cursor.getPosition();
    ciea.moveDelta = glm::vec2(0, 0);
    ciea.source = CursorInputSource::NotSpecified;
    ciea.scroll = event.d_payload.single;
    ciea.window = getTargetWindow(ciea.position, false);
    // make cursor position sane for this target window
    if (ciea.window)
        ciea.position = ciea.window->getUnprojectedPosition(ciea.position);

    // if there is no target window, input can not be handled.
    if (!ciea.window)
        return false;

    ciea.window->onScroll(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorMove_impl(CursorInputEventArgs& pa)
{
    updateWindowContainingCursor();

    // input can't be handled if there is no window to handle it.
    if (!getWindowContainingCursor())
        return false;

    // make cursor position sane for this target window
    pa.position = getWindowContainingCursor()->getUnprojectedPosition(pa.position);
    // inform window about the input.
    pa.window = getWindowContainingCursor();
    pa.handled = 0;
    pa.window->onCursorMove(pa);

    // return whether window handled the input.
    return pa.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorMoveEvent(const SemanticInputEvent& event)
{
    const glm::vec2 new_position(
        event.d_payload.array[0],
        event.d_payload.array[1]);

    // setup cursor movement event args object.
    CursorInputEventArgs ciea(nullptr);
    ciea.moveDelta = new_position - d_cursor.getPosition();

    // no movement means no event
    if ((ciea.moveDelta.x == 0) && (ciea.moveDelta.y == 0))
        return false;

    ciea.scroll = 0;
    ciea.source = CursorInputSource::NotSpecified;
    ciea.state = d_cursorsState;

    // move cursor to new position
    d_cursor.setPosition(new_position);
    // update position in args (since actual position may be constrained)
    ciea.position = d_cursor.getPosition();

    return handleCursorMove_impl(ciea);
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorLeave(const SemanticInputEvent&)
{
    if (!getWindowContainingCursor())
        return false;

    CursorInputEventArgs ciea(nullptr);
    ciea.position = getWindowContainingCursor()->getUnprojectedPosition(
        d_cursor.getPosition());
    ciea.moveDelta = glm::vec2(0, 0);
    ciea.source = CursorInputSource::NotSpecified;
    ciea.scroll = 0;
    ciea.window = getWindowContainingCursor();

    getWindowContainingCursor()->onCursorLeaves(ciea);
    resetWindowContainingCursor();

    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
void GUIContext::setWindowNavigator(WindowNavigator* navigator)
{
    d_windowNavigator = navigator;
}

//----------------------------------------------------------------------------//
#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

}

