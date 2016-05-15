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
#include "NavigationStrategies.h"
#include "CEGUI/Window.h"

#include <algorithm>

using namespace CEGUI;
using namespace NavigationStrategiesPayloads;

Window* LinearNavigationStrategy::getWindow(Window* neighbour, const String& payload)
{
    std::vector<Window*>::const_iterator itor;
    // start at the beginning
    if (neighbour == nullptr)
        return *d_windows.begin();
    else
        itor = std::find(d_windows.begin(), d_windows.end(), neighbour);

    // no such neighbour window in here
    if (itor == d_windows.end())
        return nullptr;

    if (payload == NAVIGATE_PREVIOUS)
    {
        // first item. wrap to end
        if (itor == d_windows.begin())
            return *(d_windows.end() - 1);

        return *(itor - 1);
    }
    else if (payload == NAVIGATE_NEXT)
    {
        // last item. wrap to beginning
        if (itor == d_windows.end() - 1)
            return *d_windows.begin();

        return *(itor + 1);
    }

    // no payload handling, just return the same window
    return neighbour;
}

Window* MatrixNavigationStrategy::getWindow(Window* neighbour, const String& payload)
{
    size_t rows = d_windows.size();

    for (size_t row = 0; row < rows; ++row)
    {
        std::vector<Window*> column = d_windows.at(row);
        size_t cols = column.size();

        for (size_t col = 0; col < cols; ++col)
        {
            if (neighbour == column.at(col))
            {
                // compute the new window (wrapping)
                if (payload == NAVIGATE_RIGHT)
                    col = (col + 1) % cols;
                else if (payload == NAVIGATE_DOWN)
                    row = (row + 1) % rows;
                else if (payload == NAVIGATE_LEFT)
                {
                    if (col == 0)
                        col = cols - 1;
                    else
                        col --;
                }
                else if (payload == NAVIGATE_UP)
                {
                    if (row == 0)
                        row = rows - 1;
                    else
                        row --;
                }

                return d_windows.at(row).at(col);
            }
        }
    }

    // first button
    return d_windows.at(0).at(0);
}

CEGUI::Window* WindowChildrenNavigationStrategy::getWindow(CEGUI::Window* neighbour, const CEGUI::String& payload)
{
    if (d_targetWindow == nullptr)
        return nullptr;

    size_t child_count = d_targetWindow->getChildCount();
    if (child_count == 0)
        return nullptr;

    bool found = false;
    size_t index = 0;

    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_targetWindow->getChildAtIdx(i) == neighbour)
        {
            found = true;
            index = i;
            break;
        }
    }

    if (found)
    {
        if (payload == NAVIGATE_NEXT)
        {
            if (index >= child_count - 1)
                index = 0;
            else
                index ++;
        }

        if (payload == NAVIGATE_PREVIOUS)
        {
            if (index == 0)
                index = child_count - 1;
            else
                index --;
        }
    }

    Window* child = d_targetWindow->getChildAtIdx(index);

    // start a new search
    if (!child->canFocus())
    {
        // prevent overflow
        if (child != neighbour)
            return getWindow(child, payload);
        else
            return nullptr;
    }

    return child;
}
