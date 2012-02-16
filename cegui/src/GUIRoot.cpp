/***********************************************************************
    filename:   GUIRoot.cpp
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
#include "CEGUI/GUIRoot.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/RenderingWindow.h"
#include "CEGUI/Window.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
GUIRoot::GUIRoot(RenderTarget& target) :
    RenderingSurface(target),
    d_rootWindow(0),
    d_isDirty(false)
{
}

//----------------------------------------------------------------------------//
GUIRoot::~GUIRoot()
{
}

//----------------------------------------------------------------------------//
Window* GUIRoot::getRootWindow() const
{
    return d_rootWindow;
}

//----------------------------------------------------------------------------//
void GUIRoot::setRootWindow(Window* new_root)
{
    d_rootWindow = new_root;

    if (d_rootWindow)
        updateRootWindowAreaRects();
}

//----------------------------------------------------------------------------//
void GUIRoot::updateRootWindowAreaRects() const
{
    ElementEventArgs args(0);
    d_rootWindow->onParentSized(args);
}

//----------------------------------------------------------------------------//
Sizef GUIRoot::getSurfaceSize() const
{
    return d_target.getArea().getSize();
}

//----------------------------------------------------------------------------//
void GUIRoot::markAsDirty()
{
    d_isDirty = true;
}

//----------------------------------------------------------------------------//
bool GUIRoot::isDirty() const
{
    return d_isDirty;
}

//----------------------------------------------------------------------------//
void GUIRoot::drawContent()
{
    if (d_isDirty)
        drawWindowContentToTarget();

    draw();

    d_mouseCursor.draw();
}

//----------------------------------------------------------------------------//
void GUIRoot::drawWindowContentToTarget()
{
    if (d_rootWindow)
        renderWindowHierarchyToSurfaces();
    else
        clearGeometry();

    d_isDirty = false;
}

//----------------------------------------------------------------------------//
void GUIRoot::renderWindowHierarchyToSurfaces()
{
    RenderingSurface& rs = d_rootWindow->getTargetRenderingSurface();
    rs.clearGeometry();

    if (rs.isRenderingWindow())
        static_cast<RenderingWindow&>(rs).getOwner().clearGeometry();

    d_rootWindow->render();
}

//----------------------------------------------------------------------------//
MouseCursor& GUIRoot::getMouseCursor()
{
    return const_cast<MouseCursor&>(
        static_cast<const GUIRoot*>(this)->getMouseCursor());
}

//----------------------------------------------------------------------------//
const MouseCursor& GUIRoot::getMouseCursor() const
{
    return d_mouseCursor;
}

//----------------------------------------------------------------------------//
void GUIRoot::notifySurfaceSizeChanged(const Sizef& new_size)
{
    if (d_rootWindow)
        updateRootWindowAreaRects();

    d_mouseCursor.notifyDisplaySizeChanged(new_size);
}

//----------------------------------------------------------------------------//
void GUIRoot::notifyWindowDestroyed(const Window* window)
{
    if (window == d_rootWindow)
        d_rootWindow = 0;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseMove(float delta_x, float delta_y)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseLeaves(void)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseButtonDown(MouseButton button)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseButtonUp(MouseButton button)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectKeyDown(uint key_code)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectKeyUp(uint key_code)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectChar(String::value_type code_point)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseWheelChange(float delta)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMousePosition(float x_pos, float y_pos)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectTimePulse(float timeElapsed)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseButtonClick(const MouseButton button)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseButtonDoubleClick(const MouseButton button)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectMouseButtonTripleClick(const MouseButton button)
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectCopyRequest()
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectCutRequest()
{
    return false;
}

//----------------------------------------------------------------------------//
bool GUIRoot::injectPasteRequest()
{
    return false;
}

//----------------------------------------------------------------------------//

}

