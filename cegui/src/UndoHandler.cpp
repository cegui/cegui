/***********************************************************************
	created:	15/07/2013
	author:		Erihel

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
#include "CEGUI/UndoHandler.h"
#include "CEGUI/Window.h"

namespace CEGUI
{

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
UndoHandler::UndoHandler(Window *window) :
    d_undoLimit(30),
    d_undoPosition(-1),
    d_lastUndo(true)
{
    d_attachedWindow = window;
}

/*************************************************************************
    Undo/Redo
*************************************************************************/
bool UndoHandler::undo(size_t &cursor)
{
    if (!d_attachedWindow)
        return false;

    if (canUndo())
    {
        if (static_cast<size_t>(d_undoPosition) < d_undoList.size())
        {
            UndoAction &action = d_undoList[d_undoPosition--];

            String newText = d_attachedWindow->getText();
            if (action.d_type == UndoActionType::Insert)
            {
                newText.erase(action.d_startIdx, action.d_text.length());
                cursor = action.d_startIdx;
            }
            else
            {
                newText.insert(action.d_startIdx, action.d_text);
                cursor = action.d_startIdx + action.d_text.length();
            }
            d_attachedWindow->setText(newText);
            d_lastUndo = true;
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------//
bool UndoHandler::redo(size_t &cursor)
{
    if (!d_attachedWindow)
        return false;

    if (canRedo())
    {
        UndoAction &action = d_undoList[++d_undoPosition];

        String newText = d_attachedWindow->getText();
        if (action.d_type == UndoActionType::Insert)
        {
            newText.insert(action.d_startIdx, action.d_text);
            cursor = action.d_startIdx + action.d_text.length();
        }
        else
        {
            newText.erase(action.d_startIdx, action.d_text.length());
            cursor = action.d_startIdx;
        }
        d_attachedWindow->setText(newText);
        d_lastUndo = false;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------//
void UndoHandler::setUndoLimit(int limit)
{
    if (limit < 0)
    {
        throw InvalidRequestException("Invalid undo limit. Limit cannot be less then zero.");
    }

    d_undoLimit = limit;

    while (d_undoList.size() > d_undoLimit)
        d_undoList.pop_front();

    d_undoPosition = static_cast<int>(d_undoList.size()) - 1;
}

//----------------------------------------------------------------------------//
void UndoHandler::addUndoHistory(UndoAction &action)
{
    // discard all actions that are after our current position in undo list
    while (d_undoList.size() > 0 && d_undoPosition < static_cast<int>(d_undoList.size()) - 1)
        d_undoList.pop_back();

    if (d_undoList.size() > d_undoLimit)
        d_undoList.pop_front();

    d_undoList.push_back(action);
    d_undoPosition = static_cast<int>(d_undoList.size()) - 1;
}

//----------------------------------------------------------------------------//
void UndoHandler::clearUndoHistory()
{
    d_undoList.clear();
    d_undoPosition = -1;
}

//----------------------------------------------------------------------------//
size_t UndoHandler::count()
{
    return d_undoList.size();
}

//----------------------------------------------------------------------------//
bool UndoHandler::canUndo()
{
    return (d_undoList.size() > 0 && d_undoPosition >= 0);
}

//----------------------------------------------------------------------------//
bool UndoHandler::canRedo()
{
    return (d_undoList.size() > 0 && d_undoPosition < static_cast<int>(d_undoList.size()) - 1);
}

//----------------------------------------------------------------------------//
UndoHandler::UndoAction UndoHandler::getLastAction()
{
    UndoAction lastAction;

    if (d_lastUndo)
    {
        if (d_undoPosition == static_cast<int>(d_undoList.size()) - 1)
            lastAction = d_undoList[d_undoPosition];
        else
            lastAction = d_undoList[d_undoPosition + 1];
    }
    else
    {
        if (d_undoPosition <= 0)
            lastAction = d_undoList[0];
        else
            lastAction = d_undoList[d_undoPosition];
    }
    return lastAction;
}

} // End of  CEGUI namespace section

