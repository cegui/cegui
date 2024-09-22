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
#include "CEGUI/Renderer.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/RenderingWindow.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Window.h"
#include "CEGUI/WindowNavigator.h"
#include "CEGUI/System.h"
#include "CEGUI/GlobalEventSet.h"
#include "CEGUI/CoordConverter.h"
#include <algorithm>

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String GUIContext::EventRootWindowChanged("RootWindowChanged");
const String GUIContext::EventCursorImageChanged("CursorImageChanged");
const String GUIContext::EventDefaultCursorImageChanged("DefaultCursorImageChanged");
const String GUIContext::EventDefaultFontChanged("DefaultFontChanged");
const String GUIContext::EventTooltipActive("TooltipActive");
const String GUIContext::EventTooltipInactive("TooltipInactive");
const String GUIContext::EventTooltipTransition("TooltipTransition");
const URect GUIContext::NoCursorConstraint(cegui_reldim(0.f), cegui_reldim(0.f), cegui_reldim(1.f), cegui_reldim(1.f));

//----------------------------------------------------------------------------//
GUIContext::GUIContext(RenderTarget& target) :
    RenderingSurface(target),
    d_surfaceSize(target.getArea().getSize()),
    d_cursorSize(0.f, 0.f),
    d_cursorPosition(0.f, 0.f),
    d_cursorConstraints(NoCursorConstraint),
    d_areaChangedEventConnection(
        target.subscribeEvent(
            RenderTarget::EventAreaChanged,
            Event::Subscriber(&GUIContext::areaChangedHandler, this))),
    d_fontRenderSizeChangeConnection(
        GlobalEventSet::getSingleton().subscribeEvent(
            "Font/RenderSizeChanged",
            Event::Subscriber(&GUIContext::fontRenderSizeChangedHandler, this)))
{
}

//----------------------------------------------------------------------------//
GUIContext::~GUIContext()
{
    if (d_rootWindow)
        d_rootWindow->attachToGUIContext(nullptr);

    destroyTooltips();

    for (auto currentBuffer : d_cursorGeometry)
        System::getSingleton().getRenderer()->destroyGeometryBuffer(*currentBuffer);
}

void GUIContext::destroyTooltips()
{
    for(auto record : d_tooltips)
        if(WindowManager::getSingleton().isAlive(record.second))
            WindowManager::getSingleton().destroyWindow(record.second);

    d_tooltips.clear();
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
// TODO: need stack to show one modal dialog inside of another?
bool GUIContext::setModalWindow(Window* window)
{
    if (window)
    {
        setActiveWindow(window, true);
        if (d_activeWindow == window)
            d_modalWindow = window;
    }
    else
    {
        d_modalWindow = nullptr;
    }

    return d_modalWindow == window;
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

        d_autoRepeatMouseButton = MouseButton::Invalid;
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

    d_autoRepeatMouseButton = MouseButton::Invalid;
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
        updateWindowContainingCursorInternal(getCursorTargetWindow(d_cursorPosition, true));

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

    if ((drawModeMask & DrawModeFlagMouseCursor) && d_cursorVisible && d_cursorImage)
    {
        // Update cursor geometry
        if (d_cursorDirty)
        {
            d_cursorDirty = false;
            for (auto currentBuffer : d_cursorGeometry)
                System::getSingleton().getRenderer()->destroyGeometryBuffer(*currentBuffer);
            d_cursorGeometry.clear();

            const auto imgOffset = d_cursorImage->getRenderedOffset();
            const auto imgSize = d_cursorImage->getRenderedSize();

            const Sizef cursorSize(
                (d_cursorSize.d_width > 0.0f) ? d_cursorSize.d_width : imgSize.d_width,
                (d_cursorSize.d_height > 0.0f) ? d_cursorSize.d_height : imgSize.d_height);

            const glm::vec2 cursorOffset(
                cursorSize.d_width / imgSize.d_width * imgOffset.x - imgOffset.x,
                cursorSize.d_height / imgSize.d_height * imgOffset.y - imgOffset.y);

            ImageRenderSettings imgRenderSettings(Rectf(cursorOffset, cursorSize));
            d_cursorImage->createRenderGeometry(d_cursorGeometry, imgRenderSettings);

            const Rectf clippingArea(glm::vec2(0, 0), d_surfaceSize);
            for (auto currentBuffer : d_cursorGeometry)
            {
                currentBuffer->setClippingRegion(clippingArea);
                currentBuffer->setTranslation(glm::vec3(d_cursorPosition, 0));
            }
        }

        System::getSingleton().getRenderer()->uploadBuffers(d_cursorGeometry);
        for (auto currentBuffer : d_cursorGeometry)
            currentBuffer->draw(drawModeMask);
    }
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
    d_cursorDirty = true;

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

    if(d_clickTracker.firstWindow == window || d_clickTracker.lastWindow == window)
        resetClickTracker();
}

//----------------------------------------------------------------------------//
void GUIContext::updateWindowContainingCursorInternal(Window* windowWithCursor)
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
        CursorInputEventArgs args(oldWindow, d_cursorPosition);
        oldWindow->onCursorLeaves(args);
        notifyCursorTransition(root, oldWindow, &Window::onCursorLeavesArea, args);
    }

    // Set the new cursor
    setCursorImage((windowWithCursor && windowWithCursor->getCursor()) ?
        windowWithCursor->getCursor() :
        d_defaultCursorImage);

    if (windowWithCursor)
    {
        // Inform window containing cursor that cursor has entered it
        CursorInputEventArgs args(windowWithCursor, d_cursorPosition);
        windowWithCursor->onCursorEnters(args);
        notifyCursorTransition(root, windowWithCursor, &Window::onCursorEntersArea, args);
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

    glm::vec2 pos = d_cursorPosition;
    if (auto parent = d_tooltipWindow->getParent())
    {
        pos -= parent->getUnclippedInnerRect().get().d_min;
    }
    if (d_cursorImage)
    {
        pos.x += d_cursorImage->getRenderedSize().d_width;
        pos.y += d_cursorImage->getRenderedSize().d_height;
    }

    const Rectf tipRect(pos, d_tooltipWindow->getUnclippedOuterRect().get().getSize());

    // if the tooltip would be off more at the right side of the screen,
    // reposition to the other side of the cursor.
    if (d_surfaceSize.d_width - tipRect.right() < tipRect.left() - tipRect.getWidth())
        pos.x = d_cursorPosition.x - tipRect.getWidth() - 5;

    // if the tooltip would be off more at the bottom side of the screen,
    // reposition to the other side of the cursor.
    if (d_surfaceSize.d_height - tipRect.bottom() < tipRect.top() - tipRect.getHeight())
        pos.y = d_cursorPosition.y - tipRect.getHeight() - 5;

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
    if (!d_captureWindow || d_autoRepeatMouseButton == MouseButton::Invalid)
        return;

    const float repeatRate = d_captureWindow->getAutoRepeatRate();

    // Stop auto-repeating if the window wants it no more
    if (!d_captureWindow->isCursorAutoRepeatEnabled() || repeatRate <= 0.f)
    {
        releaseInputCapture();
        return;
    }

    d_autoRepeatElapsed += timeElapsed;

    if (!d_autoRepeating)
    {
        // Delay before the first repeated event
        if (d_autoRepeatElapsed < d_captureWindow->getAutoRepeatDelay())
            return;

        d_autoRepeating = true;
        d_autoRepeatElapsed = repeatRate;
    }
    else if (d_autoRepeatElapsed < repeatRate)
        return;

    // Send events according to elapsed time
    MouseButtonEventArgs args(d_captureWindow, d_cursorPosition, d_mouseButtons, d_modifierKeys, d_autoRepeatMouseButton);
    args.d_generatedClickEventOrder = d_clickTracker.clickCount;
    do
    {
        args.handled = 0;
        d_captureWindow->onMouseButtonDown(args);
        d_autoRepeatElapsed -= repeatRate;
    }
    while (d_autoRepeatElapsed >= repeatRate);
}

//----------------------------------------------------------------------------//
Window* GUIContext::getCursorTargetWindow(const glm::vec2& pt, bool allow_disabled) const
{
    // If there is no GUI sheet visible, then there is nowhere to send input
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

    // Modal target overrules
    if (d_modalWindow && !destWnd->isInHierarchyOf(d_modalWindow))
        destWnd = d_modalWindow;

    return destWnd;
}

//----------------------------------------------------------------------------//
Window* GUIContext::getInputTargetWindow() const
{
    // if no active sheet, there is no target window.
    if (!d_rootWindow)
        return nullptr;

    if (d_captureWindow)
        return (d_activeWindow && d_activeWindow->isInHierarchyOf(d_captureWindow)) ? d_activeWindow : d_captureWindow;

    if (d_modalWindow)
        return (d_activeWindow && d_activeWindow->isInHierarchyOf(d_modalWindow)) ? d_activeWindow : d_modalWindow;

    return d_activeWindow;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectTimePulse(float timeElapsed)
{
    if (!d_rootWindow)
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
bool GUIContext::injectMousePosition(float x, float y)
{
    return injectMouseMove(x - d_cursorPosition.x, y - d_cursorPosition.y);
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseMove(float dx, float dy)
{
    // Move cursor to new position. Constraining is possible inside.
    const auto oldPos = d_cursorPosition;
    setCursorPosition(d_cursorPosition + glm::vec2(dx, dy));
    const auto delta = d_cursorPosition - oldPos;

    // No movement means no event
    if (delta.x == 0.f && delta.y == 0.f)
        return false;

    // Delay tooltip appearance, follow the cursor if already shown
    if (!d_tooltipWindow)
        d_tooltipTimer = 0.f;
    else if (d_tooltipFollowsCursor)
        positionTooltip();

    // Update window under cursor and use it as an event receiver
    d_windowContainingCursorIsUpToDate = false;
    auto window = getWindowContainingCursor();
    while (window)
    {
        CursorMoveEventArgs args(window, d_cursorPosition, d_mouseButtons, d_modifierKeys, delta);
        window->onCursorMove(args);
        if (args.handled && !window->isCursorInputPropagationEnabled())
            return true;

        if (window == d_modalWindow)
            break;

        window = window->getParent();
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseLeaves()
{
    updateWindowContainingCursorInternal(nullptr);
    return true;
}

//----------------------------------------------------------------------------//
bool GUIContext::sendScrollEvent(float delta, Window* window)
{
    while (window)
    {
        ScrollEventArgs args(window, d_cursorPosition, d_mouseButtons, d_modifierKeys, delta);
        window->onScroll(args);
        if (args.handled)
            return true;

        if (window == d_modalWindow)
            break;

        window = window->getParent();
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseWheelChange(float delta)
{
    // Wheel events are sent to the widget under the mouse cursor, but if that
    // widget does not handle the event they are sent to the focus widget
    auto cursorTargetWnd = getCursorTargetWindow(d_cursorPosition, false);
    if (sendScrollEvent(delta, cursorTargetWnd))
        return true;

    auto inputTargetWnd = getInputTargetWindow();
    return (cursorTargetWnd != inputTargetWnd) && sendScrollEvent(delta, inputTargetWnd);
}

//----------------------------------------------------------------------------//
// Check spatial and temporal locality of mouse button events. Used for click event generation.
bool GUIContext::checkClickLocality() const
{
    // Single clicks are not cancelled by timeout and by offset for better UX
    if (d_clickTracker.clickCount < 2)
        return true;

    if (std::abs(d_clickTracker.downPos.x - d_cursorPosition.x) > d_multiClickDistance ||
        std::abs(d_clickTracker.downPos.y - d_cursorPosition.y) > d_multiClickDistance)
    {
        return false;
    }

    if (d_multiClickTimeout > 0.f)
    {
        const auto secElapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - d_clickTracker.downTime).count();
        if (secElapsed > d_clickTracker.clickCount * d_multiClickTimeout)
            return false;
    }

    return true;
}

//----------------------------------------------------------------------------//
void GUIContext::resetClickTracker()
{
    d_clickTracker.firstWindow = nullptr;
    d_clickTracker.lastWindow = nullptr;
    d_clickTracker.button = MouseButton::Invalid;
    d_clickTracker.clickCount = 0;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseButtonDown(MouseButton button)
{
    d_mouseButtons += button;

    if (d_tooltipWindow)
        d_tooltipWindow->hide(true);

    bool consumed = false;
    auto window = getCursorTargetWindow(d_cursorPosition, false);
    while (window)
    {
        if (!window->isCursorPassThroughEnabled())
        {
            // Track mouse activity for click event generation
            if (d_clickLimit)
            {
                if (!consumed)
                {
                    // A click must be generated when mouse down and up events visually happen
                    // within the same window. But these events are not necessarily handled by
                    // the same window. One of our parents might have handled a down event, but
                    // an up event is handled by ourselves. This can confuse the click generator.
                    // To generate clicks as user expects, we must track a chain of windows
                    // who received mouse down event, no matter if they handled it or not.
                    auto clickWnd = d_clickTracker.firstWindow;
                    while (clickWnd)
                    {
                        if (clickWnd == window)
                        {
                            d_clickTracker.firstWindow = window;
                            break;
                        }

                        if (clickWnd == d_clickTracker.lastWindow)
                            break;

                        clickWnd = clickWnd->getParent();
                    }

                    // Do this before checkClickLocality() because it depends on the current click count
                    ++d_clickTracker.clickCount;
                    if (d_clickTracker.clickCount > d_clickLimit)
                        d_clickTracker.clickCount = 1;

                    // Detect breaking of the multi-click sequence
                    if (d_clickTracker.button != button || d_clickTracker.firstWindow != window || !checkClickLocality())
                    {
                        d_clickTracker.firstWindow = window;
                        d_clickTracker.button = button;
                        d_clickTracker.clickCount = 1;
                    }

                    if (d_clickTracker.clickCount == 1)
                    {
                        d_clickTracker.downPos = d_cursorPosition;
                        d_clickTracker.downTime = std::chrono::steady_clock::now();
                    }
                }

                d_clickTracker.lastWindow = window;
            }

            MouseButtonEventArgs args(window, d_cursorPosition, d_mouseButtons, d_modifierKeys, button);
            args.d_generatedClickEventOrder = d_clickTracker.clickCount;

            // Activate a window with left click. Treat input as handled if the window changed its Z-order in response.
            if (button == MouseButton::Left)
                if (setActiveWindow(window, d_moveToFrontOnActivateAllowed && window->isRiseOnCursorActivationEnabled()))
                    ++args.handled;

            window->onMouseButtonDown(args);

            if (args.handled)
            {
                // Start mouse down event auto-repeating if needed
                if (window->isCursorAutoRepeatEnabled())
                {
                    if (d_autoRepeatMouseButton == MouseButton::Invalid)
                        captureInput(window);

                    if (d_autoRepeatMouseButton != button && d_captureWindow == window)
                    {
                        d_autoRepeatMouseButton = button;
                        d_autoRepeatElapsed = 0.f;
                        d_autoRepeating = false;
                    }
                }

                return true;
            }

            // Consume input within non-input-transparent CEGUI geometry even when the window doesn't handle the event
            consumed = true;
        }

        if (window == d_modalWindow || !window->isCursorInputPropagationEnabled())
            break;

        window = window->getParent();
    }

    return consumed;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseButtonUp(MouseButton button)
{
    d_mouseButtons -= button;

    if (d_tooltipWindow)
    {
        d_tooltipWindow->show(true);
        d_tooltipTimer = 0.f;
    }

    // Stop auto-repeating and release capture caused by it
    if (d_autoRepeatMouseButton != MouseButton::Invalid)
        releaseInputCapture();

    bool consumed = false;
    bool clickSent = false;
    Window* windowUnderCursor = nullptr;
    auto window = getCursorTargetWindow(d_cursorPosition, false);
    while (window)
    {
        if (!window->isCursorPassThroughEnabled())
        {
            // Check most of click generation conditions once
            if (!consumed)
            {
                if (!d_clickLimit || d_clickTracker.button != button || !checkClickLocality())
                {
                    resetClickTracker();
                }
                else
                {
                    
                    windowUnderCursor = d_rootWindow->getTargetChildAtPosition(d_cursorPosition);
                    if (!windowUnderCursor)
                        resetClickTracker();
                }
            }

            MouseButtonEventArgs args(window, d_cursorPosition, d_mouseButtons, d_modifierKeys, button);

            // Send click events
            // NB: clicks are generated only for mouse up happening over the same window
            if (d_clickTracker.clickCount &&
                !clickSent &&
                windowUnderCursor && windowUnderCursor->isInHierarchyOf(window) &&
                window->isInChain(d_clickTracker.firstWindow, d_clickTracker.lastWindow))
            {
                clickSent = true;
                args.d_generatedClickEventOrder = d_clickTracker.clickCount;

                // First process multi-clicks that have dedicated event handlers
                switch (d_clickTracker.clickCount)
                {
                    case 2: window->onDoubleClick(args); break;
                    case 3: window->onTripleClick(args); break;
                }

                // Then process single clicks. Windows that do not handle multi-clicks are taken a chance to process them as single clicks too.
                if (!args.handled)
                {
                    window->onClick(args);

                    // If a multi-click was processed as single, reset tracker to generate subsequent multi-clicks accordingly
                    if (d_clickTracker.clickCount > 1)
                    {
                        d_clickTracker.clickCount = 1;
                        d_clickTracker.downTime = std::chrono::steady_clock::now() - std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<float>(d_multiClickTimeout));
                    }
                }
            }

            // Finally send mouse up event. Handling click also means handling mouse up, so we pass the same args.
            window->onMouseButtonUp(args);

            if (args.handled)
            {
                // Reset clicks if mouse up event was handled by another window
                if (!clickSent)
                    resetClickTracker();

                return true;
            }

            // Consume input within non-input-transparent CEGUI geometry even when the window doesn't handle the event
            consumed = true;
        }

        if (window == d_modalWindow || !window->isCursorInputPropagationEnabled())
            break;

        window = window->getParent();
    }

    // Reset clicks if mouse up event was not handled
    resetClickTracker();

    return consumed;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseButtonClick(MouseButton button)
{
    auto window = getCursorTargetWindow(d_cursorPosition, false);
    while (window)
    {
        MouseButtonEventArgs args(window, d_cursorPosition, d_mouseButtons, d_modifierKeys, button);
        window->onClick(args);
        if (args.handled)
            return true;

        if (window == d_modalWindow || !window->isCursorInputPropagationEnabled())
            break;

        window = window->getParent();
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseButtonDoubleClick(MouseButton button)
{
    auto window = getCursorTargetWindow(d_cursorPosition, false);
    while (window)
    {
        MouseButtonEventArgs args(window, d_cursorPosition, d_mouseButtons, d_modifierKeys, button);
        window->onDoubleClick(args);
        if (args.handled)
            return true;

        if (window == d_modalWindow || !window->isCursorInputPropagationEnabled())
            break;

        window = window->getParent();
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectMouseButtonTripleClick(MouseButton button)
{
    auto window = getCursorTargetWindow(d_cursorPosition, false);
    while (window)
    {
        MouseButtonEventArgs args(window, d_cursorPosition, d_mouseButtons, d_modifierKeys, button);
        window->onTripleClick(args);
        if (args.handled)
            return true;

        if (window == d_modalWindow || !window->isCursorInputPropagationEnabled())
            break;

        window = window->getParent();
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectKeyDown(Key::Scan scanCode)
{
    d_modifierKeys += ModifierFromScanCode(scanCode);

    auto window = getInputTargetWindow();
    while (window)
    {
        KeyEventArgs args(window, scanCode, d_modifierKeys);
        window->onKeyDown(args);
        if (args.handled)
            return true;

        if (window == d_modalWindow)
            break;

        window = window->getParent();
    }

    // Try to process an event in a window navigator
    if (d_windowNavigator)
    {
        // Activate a window. Treat input as handled if the window changed its Z-order in response.
        auto newWnd = d_windowNavigator->getWindow(d_activeWindow, scanCode, true, d_modifierKeys);
        if (setActiveWindow(newWnd, d_moveToFrontOnActivateAllowed && newWnd && newWnd->isRiseOnCursorActivationEnabled()))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectKeyUp(Key::Scan scanCode)
{
    d_modifierKeys -= ModifierFromScanCode(scanCode);

    auto window = getInputTargetWindow();
    while (window)
    {
        KeyEventArgs args(window, scanCode, d_modifierKeys);
        window->onKeyUp(args);
        if (args.handled)
            return true;

        if (window == d_modalWindow)
            break;

        window = window->getParent();
    }

    // Try to process an event in a window navigator
    if (d_windowNavigator)
    {
        // Activate a window. Treat input as handled if the window changed its Z-order in response.
        auto newWnd = d_windowNavigator->getWindow(d_activeWindow, scanCode, false, d_modifierKeys);
        if (setActiveWindow(newWnd, d_moveToFrontOnActivateAllowed && newWnd && newWnd->isRiseOnCursorActivationEnabled()))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::injectChar(char32_t codePoint)
{
    auto window = getInputTargetWindow();
    while (window)
    {
        TextEventArgs args(window, codePoint);
        window->onCharacter(args);
        if (args.handled)
            return true;

        if (window == d_modalWindow)
            break;

        window = window->getParent();
    }

    return false;
}

//----------------------------------------------------------------------------//
void GUIContext::initDefaultInputSemantics()
{
    d_keySemantics.clear();
    d_keySemantics.push_back({ SemanticValue::GoDown, Key::Scan::ArrowDown, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoUp, Key::Scan::ArrowUp, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoToPreviousCharacter, Key::Scan::ArrowLeft, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoToNextCharacter, Key::Scan::ArrowRight, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::NavigateToNext, Key::Scan::Tab, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::NavigateToPrevious, Key::Scan::Tab, ModifierKeyRule().ctrl() });
    d_keySemantics.push_back({ SemanticValue::DeletePreviousCharacter, Key::Scan::Backspace, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::DeleteNextCharacter, Key::Scan::DeleteKey, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::Confirm, Key::Scan::NumpadEnter, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::Confirm, Key::Scan::Return, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoToEndOfLine, Key::Scan::End, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoToStartOfLine, Key::Scan::Home, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoToNextPage, Key::Scan::PageDown, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoToPreviousPage, Key::Scan::PageUp, ModifierKeyRule::None() });
    d_keySemantics.push_back({ SemanticValue::GoToPreviousWord, Key::Scan::ArrowLeft, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::GoToNextWord, Key::Scan::ArrowRight, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::GoToEndOfDocument, Key::Scan::End, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::GoToStartOfDocument, Key::Scan::Home, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::SelectAll, Key::Scan::A, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::Undo, Key::Scan::Backspace, ModifierKeyRule::OnlyAlt() });
    d_keySemantics.push_back({ SemanticValue::Undo, Key::Scan::Z, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::Redo, Key::Scan::Y, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::Copy, Key::Scan::C, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::Paste, Key::Scan::V, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::Cut, Key::Scan::X, ModifierKeyRule::OnlyCtrl() });
    d_keySemantics.push_back({ SemanticValue::SelectPreviousCharacter, Key::Scan::ArrowLeft, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectNextCharacter, Key::Scan::ArrowRight, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectUp, Key::Scan::ArrowUp, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectDown, Key::Scan::ArrowDown, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectToEndOfLine, Key::Scan::End, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectToStartOfLine, Key::Scan::Home, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectToPreviousPage, Key::Scan::PageUp, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectToNextPage, Key::Scan::PageDown, ModifierKeyRule::OnlyShift() });
    d_keySemantics.push_back({ SemanticValue::SelectPreviousWord, Key::Scan::ArrowLeft, ModifierKeyRule::OnlyCtrlShift() });
    d_keySemantics.push_back({ SemanticValue::SelectNextWord, Key::Scan::ArrowRight, ModifierKeyRule::OnlyCtrlShift() });
    d_keySemantics.push_back({ SemanticValue::SelectToEndOfDocument, Key::Scan::End, ModifierKeyRule::OnlyCtrlShift() });
    d_keySemantics.push_back({ SemanticValue::SelectToStartOfDocument, Key::Scan::Home, ModifierKeyRule::OnlyCtrlShift() });
    std::sort(d_keySemantics.begin(), d_keySemantics.end());

    d_mouseSemantics.clear();
    d_mouseSemantics.push_back({ SemanticValue::SelectCumulative, MouseButton::Left, ModifierKeyRule::OnlyCtrl() });
    d_mouseSemantics.push_back({ SemanticValue::SelectRange, MouseButton::Left, ModifierKeyRule::OnlyShift() });
    d_mouseSemantics.push_back({ SemanticValue::SelectWord, MouseButton::Left, ModifierKeyRule::OnlyShift(), {}, 2 });
    d_mouseSemantics.push_back({ SemanticValue::SelectAll, MouseButton::Left, ModifierKeyRule::OnlyShift(), {}, 3 });
    std::sort(d_mouseSemantics.begin(), d_mouseSemantics.end());
}

//----------------------------------------------------------------------------//
void GUIContext::registerInputSemantic(const String& value, Key::Scan scanCode, ModifierKeyRule modifiers)
{
    KeySemanticMapping rec{ value, scanCode, modifiers };
    d_keySemantics.insert(std::upper_bound(d_keySemantics.begin(), d_keySemantics.end(), rec), std::move(rec));
}

//----------------------------------------------------------------------------//
void GUIContext::registerInputSemantic(const String& value, MouseButton button, ModifierKeyRule modifiers, int clickOrder, MouseButtons buttons)
{
    MouseButtonSemanticMapping rec{ value, button, modifiers, buttons, clickOrder };
    d_mouseSemantics.insert(std::upper_bound(d_mouseSemantics.begin(), d_mouseSemantics.end(), rec), std::move(rec));
}

//----------------------------------------------------------------------------//
void GUIContext::unregisterInputSemantic(const String& value)
{
    auto keyPair = std::equal_range(d_keySemantics.begin(), d_keySemantics.end(), value, KeySemanticMappingComp{});
    d_keySemantics.erase(keyPair.first, keyPair.second);
    auto mousePair = std::equal_range(d_mouseSemantics.begin(), d_mouseSemantics.end(), value, MouseButtonSemanticMappingComp{});
    d_mouseSemantics.erase(mousePair.first, mousePair.second);
}

//----------------------------------------------------------------------------//
void GUIContext::unregisterAllInputSemantics()
{
    d_keySemantics.clear();
    d_mouseSemantics.clear();
}

//----------------------------------------------------------------------------//
bool GUIContext::isInputSemantic(const String& value, const KeyEventArgs& args) const
{
    auto pair = std::equal_range(d_keySemantics.cbegin(), d_keySemantics.cend(), value, KeySemanticMappingComp{});
    for (auto it = pair.first; it != pair.second; ++it)
    {
        const auto& rule = *it;
        if (rule.scanCode == args.d_key && rule.modifiers.match(args.d_modifiers))
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------//
bool GUIContext::isInputSemantic(const String& value, const MouseButtonEventArgs& args) const
{
    auto pair = std::equal_range(d_mouseSemantics.cbegin(), d_mouseSemantics.cend(), value, MouseButtonSemanticMappingComp{});
    for (auto it = pair.first; it != pair.second; ++it)
    {
        const auto& rule = *it;
        if (rule.button == args.d_button &&
            rule.buttons == args.d_buttons &&
            rule.clickOrder == args.d_generatedClickEventOrder &&
            rule.modifiers.match(args.d_modifiers))
        {
            return true;
        }
    }
    return false;
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
}

//----------------------------------------------------------------------------//
void GUIContext::setCursorImage(const Image* image)
{
    if (image == d_cursorImage)
        return;

    d_cursorImage = image;
    d_cursorDirty = true;

    // Tooltip tries not to occlude the cursor
    positionTooltip();

    GUIContextEventArgs args(this);
    fireEvent(EventCursorImageChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultCursorImage(const Image* image)
{
    if (image == d_defaultCursorImage)
        return;

    // update the current image if it is the default image
    if (d_cursorImage == d_defaultCursorImage)
    {
        auto window = getWindowContainingCursor();
        if (!window || !window->getCursor())
        {
            setCursorImage(image);
        }
    }

    d_defaultCursorImage = image;
    d_cursorDirty = true;

    GUIContextEventArgs args(this);
    fireEvent(EventDefaultCursorImageChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void GUIContext::setDefaultCursorImage(const String& name)
{
    // TODO: remove this method to reduce dependencies, use ImageManager in the calling code!
    setDefaultCursorImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void GUIContext::setCursorSize(float w, float h)
{
    d_cursorSize.d_width = w;
    d_cursorSize.d_height = h;
    d_cursorDirty = true;
}

//----------------------------------------------------------------------------//
void GUIContext::setCursorPosition(const glm::vec2& position)
{
    d_cursorPosition = CoordConverter::asAbsolute(d_cursorConstraints, d_surfaceSize).clampPointToRect(position);
    for (auto currentBuffer : d_cursorGeometry)
        currentBuffer->setTranslation(glm::vec3(d_cursorPosition, 0));
}

//----------------------------------------------------------------------------//
void GUIContext::setCursorConstraintArea(const URect& area)
{
    d_cursorConstraints = area;
    d_cursorPosition = CoordConverter::asAbsolute(d_cursorConstraints, d_surfaceSize).clampPointToRect(d_cursorPosition);
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

    GUIContextEventArgs args(this);
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

}
