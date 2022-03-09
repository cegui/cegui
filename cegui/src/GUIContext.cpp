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
#include "CEGUI/GlobalEventSet.h"

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
            Event::Subscriber(&GUIContext::areaChangedHandler, this))),
    d_fontRenderSizeChangeConnection(
        GlobalEventSet::getSingleton().subscribeEvent(
            "Font/RenderSizeChanged",
            Event::Subscriber(&GUIContext::fontRenderSizeChangedHandler, this)))
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
bool GUIContext::setActiveWindow(Window* window, bool moveToFront)
{
    if (window && (window->getGUIContextPtr() != this || !window->canFocus() || !window->isEffectiveVisible()))
        return false;

    // Enforce moving to front if requested, even for already active window
    const bool zOrderChanged = moveToFront && window && window->moveToFront();

    if (window == d_activeWindow)
        return zOrderChanged;

    Window* prevActiveWindow = d_activeWindow;
    const Window* commonAncestor = Window::getCommonAncestor(window, prevActiveWindow);

    // Deactivate branch from d_activeWindow to commonAncestor
    if (prevActiveWindow && prevActiveWindow != commonAncestor)
    {
        Window* curr = prevActiveWindow;
        do
        {
            releaseInputCapture(false, curr);

            ActivationEventArgs args(curr);
            args.otherWindow = window;
            curr->onDeactivated(args);

            curr = curr->getParent();
        }
        while (curr != commonAncestor);
    }

    d_activeWindow = window;

    if (window)
    {
        if (window == commonAncestor)
        {
            // The window became focused (active leaf) and may require redrawing
            window->invalidate();
        }
        else
        {
            // Activate branch from commonAncestor to window
            std::vector<Window*> reversedList;
            reversedList.reserve(16);
            Window* curr = window;
            do
            {
                reversedList.push_back(curr);
                curr = curr->getParent();
            }
            while (curr != commonAncestor);

            for (auto it = reversedList.rbegin(); it != reversedList.rend(); ++it)
            {
                ActivationEventArgs args((*it));
                args.otherWindow = prevActiveWindow;
                (*it)->onActivated(args);
            }
        }
    }

    return zOrderChanged;
}

//----------------------------------------------------------------------------//
bool GUIContext::isWindowActive(const Window* window) const
{
    return d_activeWindow && d_activeWindow->isInHierarchyOf(window);
}

//----------------------------------------------------------------------------//
void GUIContext::setModalWindow(Window* window)
{
    // TODO: need stack to show one modal dialog inside of another?
    if (!d_modalWindow)
    {
        setActiveWindow(d_modalWindow, true);
        if (d_activeWindow == window)
            d_modalWindow = window;
    }
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

        d_repeatPointerSource = CursorInputSource::NotSpecified;
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

    d_repeatPointerSource = CursorInputSource::NotSpecified;
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
        d_rootWindow->notifyScreenAreaChanged();
        positionTooltip();
    }

    return true;
}

//----------------------------------------------------------------------------//
bool GUIContext::fontRenderSizeChangedHandler(const EventArgs& args)
{
    const Font* font = static_cast<const FontEventArgs&>(args).font;
    return font && d_rootWindow && d_rootWindow->notifyFontRenderSizeChanged(*font);
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

    // Try to pass focus to the parent. Clear focus if failed.
    if (window == d_activeWindow)
    {
        setActiveWindow(window->getParent(), false);
        if (window == d_activeWindow)
            setActiveWindow(nullptr, false);
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
        if (d_tooltipDisplayTime > 0.f && d_tooltipTimer >= d_tooltipDisplayTime)
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
void GUIContext::updateInputAutoRepeating(float timeElapsed)
{
    if (!d_captureWindow || d_repeatPointerSource == CursorInputSource::NotSpecified)
        return;

    const float repeatRate = d_captureWindow->getAutoRepeatRate();

    // Stop auto-repeating if the window wants it no more
    if (!d_captureWindow->isCursorAutoRepeatEnabled() || repeatRate <= 0.f)
    {
        releaseInputCapture(true);
        return;
    }

    d_repeatElapsed += timeElapsed;

    if (!d_repeating)
    {
        if (d_repeatElapsed < d_captureWindow->getAutoRepeatDelay())
            return;

        d_repeating = true;
        d_repeatElapsed = repeatRate;
    }
    else if (d_repeatElapsed < repeatRate)
        return;

    CursorInputEventArgs ciea(d_captureWindow);
    ciea.position = d_captureWindow->getUnprojectedPosition(d_cursor.getPosition());
    ciea.source = d_repeatPointerSource;

    do
    {
        ciea.handled = 0;
        d_captureWindow->onCursorPressHold(ciea);
        d_repeatElapsed -= repeatRate;
    }
    while (d_repeatElapsed >= repeatRate);
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

    if (d_captureWindow)
        return (d_activeWindow && d_activeWindow->isInHierarchyOf(d_captureWindow)) ? d_activeWindow : d_captureWindow;

    return d_activeWindow ? d_activeWindow : d_modalWindow;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectInputEvent(const InputEvent& event)
{
    if (event.d_eventType == InputEventType::TextInputEventType)
    {
        if (Window* targetWindow = getInputTargetWindow())
        {
            TextEventArgs args(targetWindow);
            args.d_character = static_cast<const TextInputEvent&>(event).d_character;
            targetWindow->onCharacter(args);
            return args.handled != 0;
        }
    }
    else if (event.d_eventType == InputEventType::SemanticInputEventType)
    {
        auto& semantic_event = static_cast<const SemanticInputEvent&>(event);

        // Dispatch to a handler if we have one
        auto it = d_semanticEventHandlers.find(semantic_event.d_value);
        if (it != d_semanticEventHandlers.end())
            return (*(*it).second)(semantic_event);

        // Send a raw event
        if (Window* targetWindow = getInputTargetWindow())
        {
            SemanticEventArgs args(targetWindow);
            args.d_payload = semantic_event.d_payload;
            args.d_semanticValue = semantic_event.d_value;
            targetWindow->onSemanticInputEvent(args);
            if (args.handled)
                return true;
        }

        // Try to process an event in a window navigator
        if (d_windowNavigator)
        {
            // Activate a window. Treat as a handling action if there were changes in Z-order.
            auto newWnd = d_windowNavigator->getWindow(d_activeWindow, semantic_event.d_value);
            if (setActiveWindow(newWnd, d_moveToFrontOnActivateAllowed && newWnd && newWnd->isRiseOnCursorActivationEnabled()))
                return true;
        }
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
    updateInputAutoRepeating(timeElapsed);

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
    ciea.source = event.d_payload.source;

    // Activate a window. Treat as a handling action if there were changes in Z-order.
    if (ciea.source == CursorInputSource::Left)
        if (setActiveWindow(window, d_moveToFrontOnActivateAllowed && window->isRiseOnCursorActivationEnabled()))
            ++ciea.handled;

    window->onCursorPressHold(ciea);

    if (window->isCursorAutoRepeatEnabled())
    {
        if (d_repeatPointerSource == CursorInputSource::NotSpecified)
            captureInput(window);

        if (d_repeatPointerSource != ciea.source && d_captureWindow == window)
        {
            d_repeatPointerSource = ciea.source;
            d_repeatElapsed = 0.f;
            d_repeating = false;
        }
    }

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

    if (d_repeatPointerSource != CursorInputSource::NotSpecified)
        releaseInputCapture(true, window);

    CursorInputEventArgs ciea(window);
    ciea.position = ciea.window->getUnprojectedPosition(d_cursor.getPosition());
    ciea.source = event.d_payload.source;
    window->onCursorActivate(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleSelectWord(const SemanticInputEvent& event)
{
    auto window = getTargetWindow(d_cursor.getPosition(), false);
    if (!window)
        return false;

    CursorInputEventArgs ciea(window);
    ciea.position = window->getUnprojectedPosition(d_cursor.getPosition());
    ciea.source = event.d_payload.source;
    window->onSelectWord(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleSelectAll(const SemanticInputEvent& event)
{
    auto window = getTargetWindow(d_cursor.getPosition(), false);
    if (!window)
        return false;

    CursorInputEventArgs ciea(window);
    ciea.position = window->getUnprojectedPosition(d_cursor.getPosition());
    ciea.source = event.d_payload.source;
    window->onSelectAll(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleScrollEvent(const SemanticInputEvent& event)
{
    auto window = getTargetWindow(d_cursor.getPosition(), false);
    if (!window)
        return false;

    CursorInputEventArgs ciea(window);
    ciea.position = window->getUnprojectedPosition(d_cursor.getPosition());
    ciea.scroll = event.d_payload.single;
    window->onScroll(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorMoveEvent(const SemanticInputEvent& event)
{
    const glm::vec2 newPos(event.d_payload.array[0], event.d_payload.array[1]);
    const glm::vec2 moveDelta = newPos - d_cursor.getPosition();

    // No movement means no event
    if (moveDelta.x == 0 && moveDelta.y == 0)
        return false;

    // Move cursor to new position. Constraining is possible inside.
    d_cursor.setPosition(newPos);

    // Delay tooltip appearance
    if (!d_tooltipWindow)
        d_tooltipTimer = 0.f;
    else if (d_tooltipFollowsCursor)
        positionTooltip();

    // Update window under cursor and use it as an event receiver
    d_windowContainingCursorIsUpToDate = false;
    auto window = getWindowContainingCursor();
    if (!window)
        return false;

    CursorInputEventArgs ciea(window);
    ciea.moveDelta = moveDelta;
    ciea.state = d_cursorsState;
    ciea.position = window->getUnprojectedPosition(d_cursor.getPosition());
    window->onCursorMove(ciea);
    return ciea.handled != 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::handleCursorLeave(const SemanticInputEvent&)
{
    updateWindowContainingCursor_impl(nullptr);
    return true;
}

}
