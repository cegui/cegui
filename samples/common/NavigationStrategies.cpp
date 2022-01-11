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

//----------------------------------------------------------------------------//
Window* WindowChildrenNavigator::getWindow(Window* neighbour, const SemanticValue& event)
{
    if (event != SemanticValue::GoDown && event != SemanticValue::GoUp)
        return neighbour;

    if (!d_targetWindow)
        return nullptr;

    size_t child_count = d_targetWindow->getChildCount();
    if (child_count == 0)
        return nullptr;

    bool found = false;
    size_t index = 0;

    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_targetWindow->getChildAtIndex(i) == neighbour)
        {
            found = true;
            index = i;
            break;
        }
    }

    if (found)
    {
        if (event == SemanticValue::GoDown)
        {
            if (index >= child_count - 1)
                index = 0;
            else
                index++;
        }

        if (event == SemanticValue::GoUp)
        {
            if (index == 0)
                index = child_count - 1;
            else
                index--;
        }
    }

    Window* child = d_targetWindow->getChildAtIndex(index);

    // start a new search, prevent overflow
    if (!child->canFocus())
        return (child != neighbour) ? getWindow(child, event) : nullptr;

    return child;
}

//----------------------------------------------------------------------------//
Window* LinearNavigator::getWindow(Window* neighbour, const SemanticValue& event)
{
    if (event != SemanticValue::NavigateToPrevious && event != SemanticValue::NavigateToNext)
        return neighbour;

    if (d_windows.empty())
        return nullptr;

    auto it = std::find(d_windows.begin(), d_windows.end(), neighbour);
    if (it == d_windows.end())
        return (event == SemanticValue::NavigateToNext) ? d_windows.front() : d_windows.back();

    if (event == SemanticValue::NavigateToPrevious)
    {
        do
        {
            if (it == d_windows.begin())
                it = d_windows.end();
            --it;
        }
        while ((*it) != neighbour && !(*it)->canFocus());
    }
    else if (event == SemanticValue::NavigateToNext)
    {
        do
        {
            ++it;
            if (it == d_windows.end())
                it = d_windows.begin();
        }
        while ((*it) != neighbour && !(*it)->canFocus());
    }

    return *it;
}

//----------------------------------------------------------------------------//
Window* MatrixNavigator::getWindow(Window* neighbour, const SemanticValue& event)
{
    if (event != SemanticValue::GoToNextCharacter &&
        event != SemanticValue::GoDown &&
        event != SemanticValue::GoToPreviousCharacter &&
        event != SemanticValue::GoUp)
    {
        return neighbour;
    }

    const size_t rows = d_windows.size();
    for (size_t row = 0; row < rows; ++row)
    {
        const auto& column = d_windows.at(row);
        const size_t cols = column.size();
        for (size_t col = 0; col < cols; ++col)
        {
            if (neighbour == column.at(col))
            {
                // compute the new window (wrapping)
                if (event == SemanticValue::GoToNextCharacter)
                    col = (col + 1) % cols;
                else if (event == SemanticValue::GoDown)
                    row = (row + 1) % rows;
                else if (event == SemanticValue::GoToPreviousCharacter)
                {
                    if (col == 0)
                        col = cols - 1;
                    else
                        --col;
                }
                else if (event == SemanticValue::GoUp)
                {
                    if (row == 0)
                        row = rows - 1;
                    else
                        --row;
                }

                return d_windows.at(row).at(col);
            }
        }
    }

    // first window
    return d_windows.at(0).at(0);
}
