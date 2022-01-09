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
#include "CEGUI/System.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/RenderingWindow.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Window.h"
#include "CEGUI/WindowNavigator.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String GUIContext::EventRootWindowChanged("RootWindowChanged");
const String GUIContext::EventRenderTargetChanged("RenderTargetChanged");
const String GUIContext::EventDefaultFontChanged("DefaultFontChanged");
const String GUIContext::EventTooltipActive("TooltipActive");
const String GUIContext::EventTooltipInactive("TooltipInactive");
const String GUIContext::EventTooltipTransition("TooltipTransition");

//----------------------------------------------------------------------------//
GUIContext::GUIContext(RenderTarget& target) :
    RenderingSurface(target),
    d_cursor(*this),
    d_surfaceSize(target.getArea().getSize()),
    d_areaChangedEventConnection(
        target.subscribeEvent(
            RenderTarget::EventAreaChanged,
            Event::Subscriber(&GUIContext::areaChangedHandler, this)))
{
    d_cursor.resetPositionToDefault();
    initializeSemanticEventHandlers();
}

//----------------------------------------------------------------------------//
GUIContext::~GUIContext()
{
    if (d_rootWindow)
        d_rootWindow->attachToGUIContext(nullptr);

    for (auto record : d_tooltips)
        if (WindowManager::getSingleton().isAlive(record.second))
            WindowManager::getSingleton().destroyWindow(record.second);

    for (auto it = d_semanticEventHandlers.begin(); it != d_semanticEventHandlers.end(); ++it)
        delete it->second;
}

//----------------------------------------------------------------------------//
void GUIContext::setRootWindow(Window* new_root)
{
    if (d_rootWindow == new_root)
        return;

    // Remember previous root for the event
    WindowEventArgs args(d_rootWindow);

    // NB: all tracked windows will be cleared by onWindowDetached
    if (d_rootWindow)
        d_rootWindow->attachToGUIContext(nullptr);

    d_rootWindow = new_root;
	d_windowContainingCursorIsUpToDate = false;

    if (d_rootWindow)
        d_rootWindow->attachToGUIContext(this);

    markAsDirty();

    fireEvent(EventRootWindowChanged, args);
}

//----------------------------------------------------------------------------//
bool GUIContext::captureInput(Window* window)
{
    // We can only capture if we are the active window (LEAVE THIS ALONE!)
    if (!window || !window->isActive())
        return false;

    if (d_captureWindow == window)
        return true;

    WindowEventArgs args(window);

    if (d_captureWindow)
    {
        if (window->restoresOldCapture())
            d_oldCaptureWindow = d_captureWindow;

        d_captureWindow->onCaptureLost(args);
        args.handled = 0;

        d_windowContainingCursorIsUpToDate = false;
    }

    d_captureWindow = window;
    window->onCaptureGained(args);

    return true;
}

//----------------------------------------------------------------------------//
void GUIContext::releaseInputCapture(bool allowRestoreOld, Window* exactWindow)
{
    // Nothing captured
    if (!d_captureWindow)
        return;

    // Exact window specified and it is not the capturing one
    if (exactWindow && d_captureWindow != exactWindow)
        return;

    WindowEventArgs args(d_captureWindow);
    d_captureWindow->onCaptureLost(args);

    d_windowContainingCursorIsUpToDate = false;

    if (allowRestoreOld && d_oldCaptureWindow && d_captureWindow->restoresOldCapture())
    {
        d_captureWindow = d_oldCaptureWindow;
        d_captureWindow->moveToFront();

        args.handled = 0;
        d_captureWindow->onCaptureGained(args);
    }
    else
    {
        d_captureWindow = nullptr;
    }

    d_oldCaptureWindow = nullptr;
}

//----------------------------------------------------------------------------//
void GUIContext::setModalWindow(Window* window)
{
    d_modalWindow = window;
    if (d_modalWindow)
        d_modalWindow->activate();
}

//----------------------------------------------------------------------------//
Window* GUIContext::getWindowContainingCursor()
{
    if (!d_windowContainingCursorIsUpToDate)
        updateWindowContainingCursor_impl(getTargetWindow(d_cursor.getPosition(), true));

    return d_windowContainingCursor;
}

//----------------------------------------------------------------------------//
void GUIContext::draw(std::uint32_t drawModeMask)
{
    // Cursor is always dirty because it must be redrawn each frame
    const bool drawCursor = (drawModeMask & DrawModeFlagMouseCursor);
    
    drawModeMask &= d_dirtyDrawModeMask;

    drawWindowContentToTarget(drawModeMask);

    if (drawCursor)
        drawModeMask |= DrawModeFlagMouseCursor;

    RenderingSurface::draw(drawModeMask);
}

//----------------------------------------------------------------------------//
void GUIContext::drawContent(std::uint32_t drawModeMask)
{
    if (!drawModeMask)
        return;

    RenderingSurface::drawContent(drawModeMask);

    if (drawModeMask & DrawModeFlagMouseCursor)
        d_cursor.draw(DrawModeFlagMouseCursor);
}

//----------------------------------------------------------------------------//
void GUIContext::drawWindowContentToTarget(std::uint32_t drawModeMask)
{
    if (!drawModeMask)
        return;

    if (d_rootWindow)
    {
        // Draw the window hierarchy to surfaces
        if (RenderingSurface* rs = d_rootWindow->getTargetRenderingSurface())
        {
            rs->clearGeometry();

            if (rs->isRenderingWindow())
                static_cast<RenderingWindow*>(rs)->getOwner().clearGeometry();

            d_rootWindow->draw(drawModeMask);
        }
    }
    else
    {
        clearGeometry();
    }

    // Mark all rendered modes as not dirty (cursor is always redrawn anyway)
    d_dirtyDrawModeMask &= (~drawModeMask);
}

//----------------------------------------------------------------------------//
bool GUIContext::areaChangedHandler(const EventArgs&)
{
    d_surfaceSize = d_target->getArea().getSize();
    d_cursor.notifyTargetSizeChanged(d_surfaceSize);

    if (d_rootWindow)
    {
        d_rootWindow->notifyScreenAreaChanged(true);
        positionTooltip();
    }

    return true;
}

//----------------------------------------------------------------------------//
void GUIContext::onWindowDetached(Window* window)
{
    if (!window)
        return;

    if (window == d_tooltipSource)
    {
        hideTooltip(true);
        d_tooltipSource = nullptr;
    }

    // Handle external deletion of a tooltip but don't delete it if only detached but alive
    auto itTooltip = d_tooltips.find(window->getType());
    if (itTooltip != d_tooltips.cend() && window == itTooltip->second)
    {
        // TODO: can use Window::d_destructionStarted for faster check?
        if (!WindowManager::getSingleton().isAlive(window))
            d_tooltips.erase(itTooltip);
    }

    if (window == d_windowContainingCursor)
    {
        d_windowContainingCursor = nullptr;
        d_windowContainingCursorIsUpToDate = false;
    }

    if (window == d_rootWindow)
        d_rootWindow = nullptr;

    if (window == d_modalWindow)
        d_modalWindow = nullptr;

    if (window == d_oldCaptureWindow)
        d_oldCaptureWindow = nullptr;

    releaseInputCapture(true, window);
}

//----------------------------------------------------------------------------//
void GUIContext::updateWindowContainingCursor_impl(Window* windowWithCursor)
{
    d_windowContainingCursorIsUpToDate = true;

    if (windowWithCursor == d_windowContainingCursor)
        return;

    Window* oldWindow = d_windowContainingCursor;
    d_windowContainingCursor = windowWithCursor;

    // Change the tooltip source only if the cursor is not over the tooltip itself
    if (d_tooltipSource != windowWithCursor &&
        (!windowWithCursor || !windowWithCursor->isInHierarchyOf(d_tooltipWindow)))
    {
        d_tooltipSource = windowWithCursor;
        d_tooltipTimer = 0.f;

        // If the tooltip is already visible, update it immediately
        if (d_tooltipWindow)
        {
            hideTooltip(true);
            showTooltip(true);
        }
    }

    // For 'area' version of events
    Window* root = Window::getCommonAncestor(oldWindow, windowWithCursor);

    if (oldWindow)
    {
        // Inform the previous window that the cursor has left it
        CursorInputEventArgs ciea(oldWindow);
        ciea.position = oldWindow->getUnprojectedPosition(d_cursor.getPosition());
        oldWindow->onCursorLeaves(ciea);
        notifyCursorTransition(root, oldWindow, &Window::onCursorLeavesArea, ciea);
    }

    // Set the new cursor
    d_cursor.setImage((windowWithCursor && windowWithCursor->getCursor()) ?
        windowWithCursor->getCursor() :
        d_cursor.getDefaultImage());

    if (windowWithCursor)
    {
        // Inform window containing cursor that cursor has entered it
        CursorInputEventArgs ciea(windowWithCursor);
        ciea.position = windowWithCursor->getUnprojectedPosition(d_cursor.getPosition());
        windowWithCursor->onCursorEnters(ciea);
        notifyCursorTransition(root, windowWithCursor, &Window::onCursorEntersArea, ciea);
    }
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
Window* GUIContext::getTooltipObject(const String& type) const
{
    auto it = d_tooltips.find(type);
    return (it == d_tooltips.cend()) ? nullptr : it->second;
}

//----------------------------------------------------------------------------//
Window* GUIContext::getOrCreateTooltipObject(const String& type)
{
    if (type.empty())
        return nullptr;

    if (auto tooltip = getTooltipObject(type))
        return tooltip;

    if (WindowManager::getSingleton().isLocked())
        return nullptr;

    Window* tooltip = WindowManager::getSingleton().createWindow(
        type, "__auto_tooltip__" + type);

    if (!tooltip)
        return nullptr;

    tooltip->setAutoWindow(true);
    tooltip->setWritingXMLAllowed(false);
    tooltip->setClippedByParent(false);
    tooltip->setDestroyedByParent(false);
    tooltip->setAlwaysOnTop(true);
    tooltip->setCursorPassThroughEnabled(true);
    tooltip->hide(true);

    d_tooltips.emplace(type, tooltip);
    return tooltip;
}

//----------------------------------------------------------------------------//
void GUIContext::showTooltip(bool force)
{
    if (!d_tooltipSource)
        return;

    const bool wasShown = !!d_tooltipWindow;

    if (!d_tooltipWindow)
    {
        const String& tooltipType = !d_tooltipSource->getTooltipType().empty() ?
            d_tooltipSource->getTooltipType() :
            d_defaultTooltipType;

        d_tooltipWindow = getOrCreateTooltipObject(tooltipType);

        // Avoid trying again every frame if there is no certain tooltip type registered
        if (!d_tooltipWindow)
        {
            d_tooltipSource = nullptr;
            return;
        }
    }

    d_tooltipEventConnections.clear();

    d_rootWindow->addChild(d_tooltipWindow);

    d_tooltipWindow->setText(d_tooltipSource->getTooltipTextIncludingInheritance());
    positionTooltip();

    d_tooltipEventConnections.push_back(d_tooltipSource->subscribeEvent(
        Window::EventTooltipTextChanged, [this](const EventArgs& args)
    {
        if (d_tooltipWindow && d_tooltipSource)
            d_tooltipWindow->setText(d_tooltipSource->getTooltipTextIncludingInheritance());
    }));

    d_tooltipEventConnections.push_back(d_tooltipSource->subscribeEvent(
        Window::EventSized, [this](const EventArgs& args)
    {
        positionTooltip();
    }));

    d_tooltipEventConnections.push_back(d_cursor.subscribeEvent(
        Cursor::EventImageChanged, [this](const EventArgs& args)
    {
        positionTooltip();
    }));

    WindowEventArgs args(d_tooltipWindow);
    if (wasShown)
    {
        fireEvent(EventTooltipTransition, args, EventNamespace);
    }
    else
    {
        d_tooltipWindow->show(force);
        fireEvent(EventTooltipActive, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void GUIContext::hideTooltip(bool force)
{
    if (!d_tooltipWindow)
        return;

    // Prevent repeated hiding
    auto tooltipWnd = d_tooltipWindow;
    d_tooltipWindow = nullptr;

    d_tooltipEventConnections.clear();

    // Wait until the optional tooltip window hide animation is finished. Subscribe before hide()!
    d_tooltipEventConnections.push_back(tooltipWnd->subscribeEvent(
        Window::EventHidden, [this](const EventArgs& args)
    {
        d_tooltipEventConnections.clear();

        if (auto wnd = static_cast<const WindowEventArgs&>(args).window)
        {
            // Context fields will be cleared in removeChild -> onWindowDetached
            if (wnd->getParent())
                wnd->getParent()->removeChild(wnd);

            // NB: resetting a text is important for triggering auto-sizing for certain tooltip widgets.
            // If we had kept the text, it may match the next one and EventTextChanged wouldn't happen.
            wnd->setText(String::GetEmpty());
        }
    }));

    tooltipWnd->hide(force);

    WindowEventArgs args(tooltipWnd);
    fireEvent(EventTooltipInactive, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void GUIContext::positionTooltip()
{
    if (!d_tooltipWindow)
        return;

    glm::vec2 pos = d_cursor.getPosition();
    if (auto cursorImage = d_cursor.getImage())
    {
        pos.x += cursorImage->getRenderedSize().d_width;
        pos.y += cursorImage->getRenderedSize().d_height;
    }

    const Rectf tipRect(pos, d_tooltipWindow->getUnclippedOuterRect().get().getSize());

    // if the tooltip would be off more at the right side of the screen,
    // reposition to the other side of the cursor.
    if (d_surfaceSize.d_width - tipRect.right() < tipRect.left() - tipRect.getWidth())
        pos.x = d_cursor.getPosition().x - tipRect.getWidth() - 5;

    // if the tooltip would be off more at the bottom side of the screen,
    // reposition to the other side of the cursor.
    if (d_surfaceSize.d_height - tipRect.bottom() < tipRect.top() - tipRect.getHeight())
        pos.y = d_cursor.getPosition().y - tipRect.getHeight() - 5;

    // prevent being cut off at edges
    pos.x = std::max(0.0f, std::min(pos.x, d_surfaceSize.d_width - tipRect.getWidth()));
    pos.y = std::max(0.0f, std::min(pos.y, d_surfaceSize.d_height - tipRect.getHeight()));

    // set final position of tooltip window.
    d_tooltipWindow->setPosition(UVector2(cegui_absdim(pos.x), cegui_absdim(pos.y)));
}

//----------------------------------------------------------------------------//
void GUIContext::updateTooltipState(float timeElapsed)
{
    // TODO: allow creating nonstandard tooltips without specifying the text?
    const bool needTooltip = d_tooltipSource &&
        d_tooltipSource->isTooltipEnabled() &&
        !d_tooltipSource->getTooltipTextIncludingInheritance().empty();

    if (!needTooltip)
    {
        hideTooltip(true);
        d_tooltipSource = nullptr;
        return;
    }

    d_tooltipTimer += timeElapsed;
    if (d_tooltipWindow)
    {
        // Don't update while hidden, e.g. by holding the mouse button
        if (!d_tooltipWindow->isVisible())
            return;

        // TODO: add an option to calculate d_tooltipDisplayTime from text? avgTextReadingTime(lang, text)
        if (d_tooltipTimer >= d_tooltipDisplayTime)
        {
            hideTooltip(false);
            d_tooltipTimer = 0.f;
            d_tooltipSource = nullptr;
        }
        else
        {
            // Update the tooltip if its type has changed on the fly
            const String& tooltipType = !d_tooltipSource->getTooltipType().empty() ?
                d_tooltipSource->getTooltipType() :
                d_defaultTooltipType;

            if (d_tooltipWindow->getType() != tooltipType)
            {
                hideTooltip(true);
                showTooltip(true);
            }
        }
    }
    else if (d_tooltipTimer >= d_tooltipHoverTime)
    {
        d_tooltipTimer = 0.f;
        showTooltip(false);
    }
}

//----------------------------------------------------------------------------//
Window* GUIContext::getTargetWindow(const glm::vec2& pt, bool allow_disabled) const
{
    // if there is no GUI sheet visible, then there is nowhere to send input
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return nullptr;

    Window* destWnd;
    if (d_captureWindow)
    {
        destWnd = d_captureWindow;
        if (destWnd->distributesCapturedInputs())
            if (auto childWnd = destWnd->getTargetChildAtPosition(pt, allow_disabled))
                destWnd = childWnd;
    }
    else
    {
        destWnd = d_rootWindow->getTargetChildAtPosition(pt, allow_disabled);
        if (!destWnd)
            destWnd = d_rootWindow;
    }

    // modal target overrules
    if (destWnd->isInHierarchyOf(d_modalWindow))
        destWnd = d_modalWindow;

    return destWnd;
}

//----------------------------------------------------------------------------//
Window* GUIContext::getInputTargetWindow() const
{
    // if no active sheet, there is no target window.
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
        auto& semantic_event = static_cast<const SemanticInputEvent&>(event);

        if (d_windowNavigator)
            d_windowNavigator->handleSemanticEvent(semantic_event);

        return handleSemanticInputEvent(semantic_event);
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectTimePulse(float timeElapsed)
{
    // If no visible active sheet, input can't be handled
    if (!d_rootWindow || !d_rootWindow->isEffectiveVisible())
        return false;

    // Ensure window containing cursor is now valid
    getWindowContainingCursor();

    updateTooltipState(timeElapsed);

    // Pass to sheet for distribution. This input is then /always/ considered handled.
    d_rootWindow->update(timeElapsed);
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

    d_areaChangedEventConnection.disconnect();
    d_areaChangedEventConnection = d_target->subscribeEvent(
            RenderTarget::EventAreaChanged,
            Event::Subscriber(&GUIContext::areaChangedHandler, this));

    EventArgs area_args;
    areaChangedHandler(area_args);

    GUIContextRenderTargetEventArgs change_args(this, old_target);
    fireEvent(EventRenderTargetChanged, change_args, EventNamespace);
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultFont(const String& name)
{
    setDefaultFont(name.empty() ? nullptr  : &FontManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultFont(Font* font)
{
    if (d_defaultFont == font)
        return;

    d_defaultFont = font;

    if (d_rootWindow)
        d_rootWindow->notifyDefaultFontChanged();

    EventArgs args;
    fireEvent(EventDefaultFontChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
Font* GUIContext::getDefaultFont() const
{
    if (d_defaultFont)
        return d_defaultFont;

    // if no explicit default, return the first font we can get from the font manager
    const auto& registeredFonts = FontManager::getSingleton().getRegisteredFonts();
    auto iter = registeredFonts.cbegin();
    return (iter != registeredFonts.end()) ? iter->second : nullptr;
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
    if (d_windowNavigator)
        targetWindow = d_windowNavigator->getCurrentFocusedWindow();

    if (targetWindow)
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
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::SelectWord,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleSelectWord, this)));
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::SelectAll,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleSelectAll, this)));
    d_semanticEventHandlers.insert(std::make_pair(SemanticValue::CursorMove,
        new InputEventHandlerSlot<GUIContext, SemanticInputEvent>(
            &GUIContext::handleCursorMoveEvent, this)));
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorPressHoldEvent(const SemanticInputEvent& event)
{
    auto window = getTargetWindow(d_cursor.getPosition(), false);
    if (!window)
        return false;

    if (d_tooltipWindow)
        d_tooltipWindow->hide(true);

    CursorInputEventArgs ciea(window);
    ciea.position = ciea.window->getUnprojectedPosition(d_cursor.getPosition());
    ciea.moveDelta = glm::vec2(0.f, 0.f);
    ciea.source = event.d_payload.source;

    if (d_windowNavigator)
        d_windowNavigator->setCurrentFocusedWindow(window);

    window->onCursorPressHold(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorActivateEvent(const SemanticInputEvent& event)
{
    auto window = getTargetWindow(d_cursor.getPosition(), false);
    if (!window)
        return false;

    if (d_tooltipWindow)
    {
        d_tooltipWindow->show(true);
        d_tooltipTimer = 0.f;
    }

    CursorInputEventArgs ciea(window);
    ciea.position = ciea.window->getUnprojectedPosition(d_cursor.getPosition());
    ciea.moveDelta = glm::vec2(0.f, 0.f);
    ciea.source = event.d_payload.source;

    if (d_windowNavigator)
        d_windowNavigator->setCurrentFocusedWindow(window);

    window->onCursorActivate(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleSelectWord(const SemanticInputEvent& event)
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

    if (d_windowNavigator)
        d_windowNavigator->setCurrentFocusedWindow(ciea.window);

    ciea.window->onSelectWord(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleSelectAll(const SemanticInputEvent& event)
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

    if (d_windowNavigator)
        d_windowNavigator->setCurrentFocusedWindow(ciea.window);

    ciea.window->onSelectAll(ciea);
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

    d_windowContainingCursorIsUpToDate = false;

    // input can't be handled if there is no window to handle it.
    if (!getWindowContainingCursor())
        return false;

	// resetting the timer is an old behaviour, let's keep it for now
    d_tooltipTimer = 0.f;
    if (d_tooltipFollowsCursor)
        positionTooltip();

    // make cursor position sane for this target window
    ciea.position = getWindowContainingCursor()->getUnprojectedPosition(ciea.position);
    // inform window about the input.
    ciea.window = getWindowContainingCursor();
    ciea.handled = 0;
    ciea.window->onCursorMove(ciea);

    // return whether window handled the input.
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorLeave(const SemanticInputEvent&)
{
    updateWindowContainingCursor_impl(nullptr);
    return true;
}

}
