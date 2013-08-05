/***********************************************************************
    filename:   GUIContext.cpp
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
const String GUIContext::EventMouseMoveScalingFactorChanged("MouseMoveScalingFactorChanged");
const String GUIContext::EventRenderTargetChanged("RenderTargetChanged");
const String GUIContext::EventDefaultFontChanged("DefaultFontChanged");

//----------------------------------------------------------------------------//
GUIContext::GUIContext(RenderTarget& target) :
    RenderingSurface(target),
    d_rootWindow(0),
    d_isDirty(false),
    d_pointerMovementScalingFactor(1.0f),
    d_defaultTooltipObject(0),
    d_weCreatedTooltipObject(false),
    d_defaultFont(0),
    d_surfaceSize(target.getArea().getSize()),
    d_modalWindow(0),
    d_captureWindow(0),
    d_areaChangedEventConnection(
        target.subscribeEvent(
            RenderTarget::EventAreaChanged,
            Event::Subscriber(&GUIContext::areaChangedHandler, this))),
    d_windowDestroyedEventConnection(
        WindowManager::getSingleton().subscribeEvent(
            WindowManager::EventWindowDestroyed,
            Event::Subscriber(&GUIContext::windowDestroyedHandler, this))),
    d_semanticEventHandlers()
{
    resetWindowContainingPointer();
    initializeSemanticEventHandlers();
}

//----------------------------------------------------------------------------//
void GUIContext::resetWindowContainingPointer()
{
    d_windowContainingPointer = 0;
    d_windowContainingPointerIsUpToDate = true;
}

//----------------------------------------------------------------------------//
GUIContext::~GUIContext()
{
    destroyDefaultTooltipWindowInstance();

    if (d_rootWindow)
        d_rootWindow->setGUIContext(0);
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
        d_rootWindow->setGUIContext(0);

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
    ElementEventArgs args(0);
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
        d_defaultTooltipObject = 0;
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
Window* GUIContext::getWindowContainingPointer() const
{
    if (!d_windowContainingPointerIsUpToDate)
    {
        updateWindowContainingPointer_impl();
        d_windowContainingPointerIsUpToDate = true;
    }

    return d_windowContainingPointer;
}

//----------------------------------------------------------------------------//
const Sizef& GUIContext::getSurfaceSize() const
{
    return d_surfaceSize;
}

//----------------------------------------------------------------------------//
const SystemKeys& GUIContext::getSystemKeys() const
{
    return d_systemKeys;
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

    d_pointerIndicator.draw();
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

    d_rootWindow->render();
}

//----------------------------------------------------------------------------//
PointerIndicator& GUIContext::getPointerIndicator()
{
    return const_cast<PointerIndicator&>(
        static_cast<const GUIContext*>(this)->getPointerIndicator());
}

//----------------------------------------------------------------------------//
const PointerIndicator& GUIContext::getPointerIndicator() const
{
    return d_pointerIndicator;
}

//----------------------------------------------------------------------------//
void GUIContext::setMouseMoveScalingFactor(float factor)
{
    d_pointerMovementScalingFactor = factor;

    GUIContextEventArgs args(this);
    onMouseMoveScalingFactorChanged(args);
}

//----------------------------------------------------------------------------//
float GUIContext::getMouseMoveScalingFactor() const
{
    return d_pointerMovementScalingFactor;
}

//----------------------------------------------------------------------------//
bool GUIContext::areaChangedHandler(const EventArgs&)
{
    d_surfaceSize = d_target->getArea().getSize();
    d_pointerIndicator.notifyDisplaySizeChanged(d_surfaceSize);

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
        d_rootWindow = 0;

    if (window == getWindowContainingPointer())
        resetWindowContainingPointer();

    if (window == d_modalWindow)
        d_modalWindow = 0;

    if (window == d_captureWindow)
        d_captureWindow = 0;

    if (window == d_defaultTooltipObject)
    {
        d_defaultTooltipObject = 0;
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
void GUIContext::onMouseMoveScalingFactorChanged(GUIContextEventArgs& args)
{
    fireEvent(EventMouseMoveScalingFactorChanged, args);
}

//----------------------------------------------------------------------------//
void GUIContext::updateWindowContainingPointer()
{
    d_windowContainingPointerIsUpToDate = false;
}

//----------------------------------------------------------------------------//
bool GUIContext::updateWindowContainingPointer_impl() const
{
    PointerEventArgs pa(0);
    const Vector2f mouse_pos(d_pointerIndicator.getPosition());

    Window* const curr_wnd_with_pointer = getTargetWindow(mouse_pos, true);

    // exit if window containing pointer has not changed.
    if (curr_wnd_with_pointer == d_windowContainingPointer)
        return false;

    pa.scroll = 0;
    pa.source = PS_None;

    Window* oldWindow = d_windowContainingPointer;
    d_windowContainingPointer = curr_wnd_with_pointer;

    // inform previous window the pointer has left it
    if (oldWindow)
    {
        pa.window = oldWindow;
        pa.position = oldWindow->getUnprojectedPosition(mouse_pos);
        oldWindow->onPointerLeaves(pa);
    }

    // inform window containing pointer that pointer has entered it
    if (d_windowContainingPointer)
    {
        pa.handled = 0;
        pa.window = d_windowContainingPointer;
        pa.position = d_windowContainingPointer->getUnprojectedPosition(mouse_pos);
        d_windowContainingPointer->onPointerEnters(pa);
    }

    // do the 'area' version of the events
    Window* root = getCommonAncestor(oldWindow, d_windowContainingPointer);

    if (oldWindow)
        notifyPointerTransition(root, oldWindow, &Window::onPointerLeavesArea, pa);

    if (d_windowContainingPointer)
        notifyPointerTransition(root, d_windowContainingPointer, &Window::onPointerEntersArea, pa);

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
void GUIContext::notifyPointerTransition(Window* top, Window* bottom,
                                    void (Window::*func)(PointerEventArgs&),
                                    PointerEventArgs& args) const
{
    if (top == bottom)
        return;

    Window* const parent = bottom->getParent();

    if (parent && parent != top)
        notifyPointerTransition(top, parent, func, args);

    args.handled = 0;
    args.window = bottom;

    (bottom->*func)(args);
}

//----------------------------------------------------------------------------//
Window* GUIContext::getTargetWindow(const Vector2f& pt,
                                 const bool allow_disabled) const
{
    // if there is no GUI sheet visible, then there is nowhere to send input
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return 0;

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
Window* GUIContext::getKeyboardTargetWindow() const
{
    // if no active sheet, there is no target widow.
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return 0;

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
    if (event.d_eventType == IET_TextInputEventType)
        return handleTextInputEvent(static_cast<const TextInputEvent&>(event));

    if (event.d_eventType == IET_SemanticInputEventType)
        return handleSemanticInputEvent(static_cast<const SemanticInputEvent&>(event));

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectKeyDown(Key::Scan scan_code)
{
    d_systemKeys.keyPressed(scan_code);

    KeyEventArgs args(getKeyboardTargetWindow());

    // if there's no destination window, input can't be handled.
    if (!args.window)
        return false;

    args.scancode = scan_code;
    args.sysKeys = d_systemKeys.get();

    args.window->onKeyDown(args);
    return args.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectKeyUp(Key::Scan scan_code)
{
    d_systemKeys.keyReleased(scan_code);

    KeyEventArgs args(getKeyboardTargetWindow());

    // if there's no destination window, input can't be handled.
    if (!args.window)
        return false;

    args.scancode = scan_code;
    args.sysKeys = d_systemKeys.get();

    args.window->onKeyUp(args);
    return args.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectTimePulse(float timeElapsed)
{
    // if no visible active sheet, input can't be handled
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return false;

    // ensure window containing mouse is now valid
    getWindowContainingPointer();

    // else pass to sheet for distribution.
    d_rootWindow->update(timeElapsed);
    // this input is then /always/ considered handled.
    return true;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCopyRequest(const SemanticInputEvent& event)
{
    Window* source = getKeyboardTargetWindow();
    return source ? source->performCopy(*System::getSingleton().getClipboard()) : false;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCutRequest(const SemanticInputEvent& event)
{
    Window* source = getKeyboardTargetWindow();
    return source ? source->performCut(*System::getSingleton().getClipboard()) : false;
}

//----------------------------------------------------------------------------//
bool GUIContext::handlePasteRequest(const SemanticInputEvent& event)
{
    Window* target = getKeyboardTargetWindow();
    return target ? target->performPaste(*System::getSingleton().getClipboard()) : false;
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
        setDefaultFont(0);
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
    FontManager::FontIterator iter = FontManager::getSingleton().getIterator();

    return (!iter.isAtEnd()) ? *iter : 0;
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
    TextEventArgs args(getKeyboardTargetWindow());

    // if there's no destination window, input can't be handled.
    if (!args.window)
        return false;

    args.character = event.d_character;

    args.window->onCharacter(args);
    return args.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleSemanticInputEvent(const SemanticInputEvent& event)
{
    // dispatch to a handler if we have one
    std::map<int, SlotFunctorBase<InputEvent>*>::const_iterator itor =
        d_semanticEventHandlers.find(event.d_value);
    if (itor != d_semanticEventHandlers.end())
    {
        return (*(*itor).second)(event);
    }

    SemanticEventArgs args(getTargetWindow(d_pointerIndicator.getPosition(), false));

    args.d_payload = event.d_payload;
    args.d_semanticValue = event.d_value;

    args.window->onSemanticInputEvent(args);

    return args.handled != 0;
}

//----------------------------------------------------------------------------//
void GUIContext::initializeSemanticEventHandlers()
{
    d_semanticEventHandlers.insert(std::make_pair(SV_Cut,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleCutRequest, this)));
    d_semanticEventHandlers.insert(std::make_pair(SV_Copy,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleCopyRequest, this)));
    d_semanticEventHandlers.insert(std::make_pair(SV_Paste,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handlePasteRequest, this)));
    d_semanticEventHandlers.insert(std::make_pair(SV_VerticalScroll,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleScrollEvent, this)));

    d_semanticEventHandlers.insert(std::make_pair(SV_PointerActivate,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
        &GUIContext::handlePointerActivateEvent, this)));
    d_semanticEventHandlers.insert(std::make_pair(SV_PointerPressHold,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
        &GUIContext::handlePointerPressHoldEvent, this)));
    d_semanticEventHandlers.insert(std::make_pair(SV_PointerMove,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handlePointerMoveEvent, this)));
}

bool GUIContext::handlePointerActivateEvent(const SemanticInputEvent& event)
{
    PointerEventArgs pa(0);
    pa.position = d_pointerIndicator.getPosition();
    pa.moveDelta = Vector2f(0.0f, 0.0f);
    pa.source = event.d_payload.source;
    pa.scroll = 0;
    pa.window = getTargetWindow(pa.position, false);
    // make pointer position sane for this target window
    if (pa.window)
        pa.position = pa.window->getUnprojectedPosition(pa.position);

    // if there is no target window, input can not be handled.
    if (!pa.window)
        return false;

    pa.window->onPointerActivate(pa);
    return pa.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handlePointerPressHoldEvent(const SemanticInputEvent& event)
{
    PointerEventArgs pa(0);
    pa.position = d_pointerIndicator.getPosition();
    pa.moveDelta = Vector2f(0.0f, 0.0f);
    pa.source = event.d_payload.source;
    pa.scroll = 0;
    pa.window = getTargetWindow(pa.position, false);
    // make mouse position sane for this target window
    if (pa.window)
        pa.position = pa.window->getUnprojectedPosition(pa.position);

    pa.window->onPointerPressHold(pa);
    return pa.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleScrollEvent(const SemanticInputEvent& event)
{
    PointerEventArgs pa(0);
    pa.position = d_pointerIndicator.getPosition();
    pa.moveDelta = Vector2f(0.0f, 0.0f);
    pa.source = PS_None;
    pa.scroll = event.d_payload.single;
    pa.window = getTargetWindow(pa.position, false);
    // make pointer position sane for this target window
    if (pa.window)
        pa.position = pa.window->getUnprojectedPosition(pa.position);

    // if there is no target window, input can not be handled.
    if (!pa.window)
        return false;

    pa.window->onScroll(pa);
    return pa.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handlePointerMove_impl(PointerEventArgs& pa)
{
    updateWindowContainingPointer();

    // input can't be handled if there is no window to handle it.
    if (!getWindowContainingPointer())
        return false;

    // make pointer position sane for this target window
    pa.position = getWindowContainingPointer()->getUnprojectedPosition(pa.position);
    // inform window about the input.
    pa.window = getWindowContainingPointer();
    pa.handled = 0;
    pa.window->onPointerMove(pa);

    // return whether window handled the input.
    return pa.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handlePointerMoveEvent(const SemanticInputEvent& event)
{
    const Vector2f new_position(event.d_payload.array[0],
        event.d_payload.array[1]);

    // setup pointer movement event args object.
    PointerEventArgs pa(0);
    pa.moveDelta = new_position - d_pointerIndicator.getPosition();

    // no movement means no event
    if ((pa.moveDelta.d_x == 0) && (pa.moveDelta.d_y == 0))
        return false;

    pa.scroll = 0;
    pa.source = PS_None;
    pa.pointerState = d_pointersState;

    // move pointer cursor to new position
    d_pointerIndicator.setPosition(new_position);
    // update position in args (since actual position may be constrained)
    pa.position = d_pointerIndicator.getPosition();

    return handlePointerMove_impl(pa);
}

//----------------------------------------------------------------------------//
bool GUIContext::handlePointerLeave(const SemanticInputEvent& event)
{
    if (!getWindowContainingPointer())
        return false;

    PointerEventArgs pa(0);
    pa.position = getWindowContainingPointer()->getUnprojectedPosition(
        d_pointerIndicator.getPosition());
    pa.moveDelta = Vector2f(0.0f, 0.0f);
    pa.source = PS_None;
    pa.scroll = 0;
    pa.window = getWindowContainingPointer();

    getWindowContainingPointer()->onPointerLeaves(pa);
    resetWindowContainingPointer();

    return pa.handled != 0;
}

//----------------------------------------------------------------------------//
#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

}

