/***********************************************************************
    created:    25/4/2004
    author:     Paul D Turner

    purpose:    Implementation of common Titlebar parts.
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
#include "CEGUI/widgets/Titlebar.h"
#include "CEGUI/widgets/FrameWindow.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
const String Titlebar::EventNamespace("Titlebar");
const String Titlebar::WidgetTypeName("CEGUI/Titlebar");

//----------------------------------------------------------------------------//
Titlebar::Titlebar(const String& type, const String& name)
    : Window(type, name)
{
    addTitlebarProperties();
    setAlwaysOnTop(true);
    setCursorInputPropagationEnabled(true);
}

//----------------------------------------------------------------------------//
void Titlebar::setDraggingEnabled(bool setting)
{
    if (d_dragEnabled == setting)
        return;

    d_dragEnabled = setting;

    // stop dragging now if the setting has been disabled.
    if (!d_dragEnabled && d_dragging)
        releaseInput();

    WindowEventArgs args(this);
    onDraggingEnabledChanged(args);
}

//----------------------------------------------------------------------------//
void Titlebar::onCursorMove(CursorMoveEventArgs& e)
{
    Window::onCursorMove(e);

    if (d_dragging && d_parent)
    {
        // Move the window. Titlebar objects should only be attached to FrameWindow derived classes.
        if (auto frameWnd = dynamic_cast<FrameWindow*>(d_parent))
        {
            const glm::vec2 delta = e.d_localPos - d_dragPoint;
            frameWnd->setPosition(frameWnd->getPosition() + UVector2(cegui_absdim(delta.x), cegui_absdim(delta.y)));
        }

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void Titlebar::onMouseButtonDown(MouseButtonEventArgs& e)
{
    Window::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left)
    {
        // Sizing border events are propagated to the owning FrameWindow
        auto frameWnd = dynamic_cast<FrameWindow*>(d_parent);
        if (frameWnd && frameWnd->isSizingEnabled())
        {
            if (frameWnd->getSizingBorderAtPoint(e.d_localPos) != FrameWindow::SizingLocation::Invalid)
                return;
        }

        if (d_parent && d_dragEnabled)
        {
            // we want all cursor inputs from now on
            if (captureInput())
            {
                // initialise the dragging state
                d_dragging = true;
                d_dragPoint = e.d_localPos;

                // store old constraint area
                d_oldCursorArea = d_guiContext->getCursorConstraintArea();

                // setup new constraint area to be the intersection of the old area and our grand-parent's clipped inner-area
                const Rectf oldAreaAbs = CoordConverter::asAbsolute(d_oldCursorArea, d_guiContext->getSurfaceSize());
                Rectf constrainArea;
                if (auto grandParent = static_cast<Window*>(d_parent->getParentElement()))
                {
                    constrainArea = grandParent->getInnerRectClipper().getIntersection(oldAreaAbs);
                }
                else
                {
                    const Rectf screen(glm::vec2(0.f, 0.f), getRootContainerSize());
                    constrainArea = screen.getIntersection(oldAreaAbs);
                }

                d_guiContext->setCursorConstraintArea(URect(
                    UDim(0.f, constrainArea.d_min.x), UDim(0.f, constrainArea.d_min.y),
                    UDim(0.f, constrainArea.d_max.x), UDim(0.f, constrainArea.d_max.y)));
            }
        }

        // Consume click anyway, preventing unwanted pass-through
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void Titlebar::onMouseButtonUp(MouseButtonEventArgs& e)
{
    Window::onMouseButtonUp(e);

    if (e.d_button == MouseButton::Left)
    {
        releaseInput();
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void Titlebar::onDoubleClick(MouseButtonEventArgs& e)
{
    Window::onDoubleClick(e);

    if (e.d_button == MouseButton::Left && !isDisabled())
    {
        // Our parent must be a FrameWindow or subclass for rolling up to work
        if (auto frameWnd = dynamic_cast<FrameWindow*>(d_parent))
        {
            frameWnd->toggleRollup();
            ++e.handled;
        }
    }
}

//----------------------------------------------------------------------------//
void Titlebar::onCaptureLost(WindowEventArgs& e)
{
    // Base class processing
    Window::onCaptureLost(e);

    // when we lose out hold on the cursor inputs, we are no longer dragging.
    d_dragging = false;

    // restore old constraint area
    getGUIContext().setCursorConstraintArea(d_oldCursorArea);
}

//----------------------------------------------------------------------------//
void Titlebar::onFontChanged(WindowEventArgs& e)
{
    Window::onFontChanged(e);

    if (d_parent && !getParent()->isInitializing())
        getParent()->performChildLayout(false, false);
}

//----------------------------------------------------------------------------//
void Titlebar::addTitlebarProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Titlebar, bool,
        "DraggingEnabled", "Property to get/set the state of the dragging enabled setting for the Titlebar.  Value is either \"true\" or \"false\".",
        &Titlebar::setDraggingEnabled, &Titlebar::isDraggingEnabled, true
    );
}

}
