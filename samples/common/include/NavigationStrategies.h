/***********************************************************************
    filename:   NavigationStrategies.h
    created:    25/08/2013
    author:     Timotei Dolean
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUINavigationStrategies_h_
#define _CEGUINavigationStrategies_h_

#include "CEGUI/String.h"
#include "CEGUI/WindowNavigator.h"
#include <vector>

namespace NavigationStrategiesPayloads
{
static const CEGUI::String NAVIGATE_LEFT = "left";
static const CEGUI::String NAVIGATE_RIGHT = "right";
static const CEGUI::String NAVIGATE_UP = "up";
static const CEGUI::String NAVIGATE_DOWN = "down";
static const CEGUI::String NAVIGATE_NEXT = "next";
static const CEGUI::String NAVIGATE_PREVIOUS = "previous";
}

class LinearNavigationStrategy : public CEGUI::NavigationStrategy
{
public:
    std::vector<CEGUI::Window*> d_windows;

    virtual CEGUI::Window* getWindow(CEGUI::Window* neighbour, const CEGUI::String& payload);
};

class MatrixNavigationStrategy : public CEGUI::NavigationStrategy
{
public:
    // matrix
    std::vector< std::vector<CEGUI::Window*> > d_windows;

    virtual CEGUI::Window* getWindow(CEGUI::Window* neighbour, const CEGUI::String& payload);
};

#endif
