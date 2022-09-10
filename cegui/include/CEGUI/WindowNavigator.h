/***********************************************************************
    created:    12/08/2013
    author:     Timotei Dolean

    purpose:    Implements the ability to navigate in the GUI
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
#ifndef _CEGUIWindowNavigator_h_
#define _CEGUIWindowNavigator_h_

#include "CEGUI/InputEvent.h"

namespace CEGUI
{

/*!
\brief
    Provides a strategy for navigating the GUI with input events.
    Inherit it to implement different strategies and event mappings.

    For a brief tutorial on how to use the GUI navigation please refer
    to the @ref gui_navigation_tutorial
*/
class CEGUIEXPORT WindowNavigator
{
public:

    virtual ~WindowNavigator() = default;

    /*!
    \brief
        Returns a window based on its neighbour and a certain payload

    \param neighbour
        The neighbour window relative to which the new window is requested

    \param event
        A semantic event type to process
    */
    virtual Window* getWindow(Window* neighbour, Key::Scan key, bool down, ModifierKeys modifiers) = 0;
};

}

#endif
