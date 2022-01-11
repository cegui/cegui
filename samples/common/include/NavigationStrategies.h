/***********************************************************************
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

#include "CEGUI/WindowNavigator.h"
#include <vector>

// Simple navigators with hardcoded events. Used for demostrational purposes.

//! \brief A strategy that navigates between a specified window's children
class WindowChildrenNavigator : public CEGUI::WindowNavigator
{
public:

    CEGUI::Window* d_targetWindow = nullptr;

    virtual CEGUI::Window* getWindow(CEGUI::Window* neighbour, const CEGUI::SemanticValue& event) override;
};

//! \brief A linear strategy that navigates in the windows vector
class LinearNavigator : public CEGUI::WindowNavigator
{
public:

    std::vector<CEGUI::Window*> d_windows;

    virtual CEGUI::Window* getWindow(CEGUI::Window* neighbour, const CEGUI::SemanticValue& event) override;
};

//! \brief A matrix navigation strategy which navigates in the windows matrix.
class MatrixNavigator : public CEGUI::WindowNavigator
{
public:

    std::vector<std::vector<CEGUI::Window*>> d_windows;

    virtual CEGUI::Window* getWindow(CEGUI::Window* neighbour, const CEGUI::SemanticValue& event) override;
};

#endif
