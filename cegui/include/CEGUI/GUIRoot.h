/***********************************************************************
    filename:   GUIRoot.h
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
#ifndef _CEGUIGUIRoot_h_
#define _CEGUIGUIRoot_h_

#include "CEGUI/RenderingSurface.h"
#include "CEGUI/InjectedInputReceiver.h"
#include "CEGUI/MouseCursor.h"

namespace CEGUI
{
class CEGUIEXPORT GUIRoot : public RenderingSurface,
                                   InjectedInputReceiver
{
public:
    GUIRoot(RenderTarget& target);
    ~GUIRoot();

    Window* getRootWindow() const;
    void setRootWindow(Window* new_root);

    Sizef getSurfaceSize() const;

    //! call to indicate that some redrawing is required.
    void markAsDirty();
    bool isDirty() const;

    void drawContent();

    MouseCursor& getMouseCursor();
    const MouseCursor& getMouseCursor() const;

    // Implementation of InjectedInputReceiver interface
    bool injectMouseMove(float delta_x, float delta_y);
    bool injectMouseLeaves(void);
    bool injectMouseButtonDown(MouseButton button);
    bool injectMouseButtonUp(MouseButton button);
    bool injectKeyDown(uint key_code);
    bool injectKeyUp(uint key_code);
    bool injectChar(String::value_type code_point);
    bool injectMouseWheelChange(float delta);
    bool injectMousePosition(float x_pos, float y_pos);
    bool injectTimePulse(float timeElapsed);
    bool injectMouseButtonClick(const MouseButton button);
    bool injectMouseButtonDoubleClick(const MouseButton button);
    bool injectMouseButtonTripleClick(const MouseButton button);
    bool injectCopyRequest();
    bool injectCutRequest();
    bool injectPasteRequest();

protected:
    //! notify the root that the size of it's surface has changed
    void notifySurfaceSizeChanged(const Sizef& new_size);
    //! notify the root that some window has been destroyed.
    void notifyWindowDestroyed(const Window* window);

    void updateRootWindowAreaRects() const;
    void drawWindowContentToTarget();
    void renderWindowHierarchyToSurfaces();

    Window* d_rootWindow;
    bool d_isDirty;
    MouseCursor* d_mouseCursor;
};

}

#endif

