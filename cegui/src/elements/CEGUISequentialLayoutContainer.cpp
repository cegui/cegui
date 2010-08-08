/***********************************************************************
    filename:   CEGUISequentialLayoutContainer.cpp
    created:    02/8/2010
    author:     Martin Preisler

    purpose:    Implements the Layout Container base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "elements/CEGUISequentialLayoutContainer.h"
#include "CEGUIWindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String SequentialLayoutContainer::EventNamespace("SequentialLayoutContainer");
const String SequentialLayoutContainer::EventChildWindowOrderChanged("ChildWindowOrderChanged");

//----------------------------------------------------------------------------//
SequentialLayoutContainer::SequentialLayoutContainer(const String& type,
                                                     const String& name):
    LayoutContainer(type, name)
{}

//----------------------------------------------------------------------------//
SequentialLayoutContainer::~SequentialLayoutContainer(void)
{}

//----------------------------------------------------------------------------//
size_t SequentialLayoutContainer::getPositionOfChildWindow(Window* wnd) const
{
    return getIdxOfChildWindow(wnd);
}

//----------------------------------------------------------------------------//
size_t SequentialLayoutContainer::getPositionOfChildWindow(const String& wnd) const
{
    return getPositionOfChildWindow(
                WindowManager::getSingleton().getWindow(wnd));
}

//----------------------------------------------------------------------------//
Window* SequentialLayoutContainer::getChildWindowAtPosition(size_t position) const
{
    return getChildAtIdx(position);
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::swapChildWindowPositions(size_t wnd1,
                                                         size_t wnd2)
{
    if (wnd1 < d_children.size() && wnd2 < d_children.size())
    {
        std::swap(d_children[wnd1], d_children[wnd2]);

        WindowEventArgs args(this);
        onChildWindowOrderChanged(args);
    }
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::swapChildWindows(Window* wnd1, Window* wnd2)
{
    if (isChild(wnd1) && isChild(wnd2))
    {
        swapChildWindowPositions(getPositionOfChildWindow(wnd1),
                                 getPositionOfChildWindow(wnd2));
    }
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::swapChildWindows(const String& wnd1,
                                                 Window* wnd2)
{
    swapChildWindows(WindowManager::getSingleton().getWindow(wnd1), wnd2);
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::swapChildWindows(Window* wnd1,
                                                 const String& wnd2)
{
    swapChildWindows(wnd1, WindowManager::getSingleton().getWindow(wnd2));
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::swapChildWindows(const String& wnd1,
                                                 const String& wnd2)
{
    swapChildWindows(WindowManager::getSingleton().getWindow(wnd1),
                     WindowManager::getSingleton().getWindow(wnd2));
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::moveChildWindowToPosition(Window* wnd,
                                                          size_t position)
{
    if (!isChild(wnd))
        return;

    position = std::min(position, d_children.size() - 1);

    const size_t oldPosition = getPositionOfChildWindow(wnd);

    if (oldPosition == position)
    {
        return;
    }

    // we get the iterator of the old position
    ChildList::iterator it = d_children.begin();
    std::advance(it, oldPosition);

    // we are the window from it's old position
    d_children.erase(it);

    // if the window comes before the point we want to insert to,
    // we have to decrement the position
    if (oldPosition < position)
    {
        --position;
    }

    // find iterator of the new position
    it = d_children.begin();
    std::advance(it, position);
    // and insert the window there
    d_children.insert(it, wnd);

    WindowEventArgs args(this);
    onChildWindowOrderChanged(args);
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::moveChildWindowToPosition(const String& wnd,
                                                          size_t position)
{
    moveChildWindowToPosition(WindowManager::getSingleton().getWindow(wnd),
                              position);
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::moveChildWindow(Window* window, int delta)
{
    const size_t oldPosition = getPositionOfChildWindow(window);
    int newPosition = oldPosition + delta;
    newPosition = std::max(newPosition, 0);
    // this is handled by moveChildWindowToPosition itself
    //newPosition = std::min(newPosition, (int)(d_children.size() - 1));

    moveChildWindowToPosition(window, newPosition);
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::addChildWindowToPosition(Window* window,
                                                         size_t position)
{
    addChildWindow(window);

    moveChildWindowToPosition(window, position);
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::addChildWindowToPosition(const String& window,
                                                         size_t position)
{
    addChildWindowToPosition(WindowManager::getSingleton().getWindow(window),
                             position);
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::removeChildWindowFromPosition(size_t position)
{
    removeChildWindow(getChildWindowAtPosition(position));
}

//----------------------------------------------------------------------------//
void SequentialLayoutContainer::onChildWindowOrderChanged(WindowEventArgs& e)
{
    markNeedsLayouting();

    fireEvent(EventChildWindowOrderChanged, e, EventNamespace);
}

} // End of  CEGUI namespace section

