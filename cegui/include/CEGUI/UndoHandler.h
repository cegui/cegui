/***********************************************************************
	created:	15/07/2013
	author:		Erihel <erihel@gmail.com>
	
	purpose:	Class for handling undo/redo of windows
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
#ifndef _CEGUIUndoHandler_h_
#define _CEGUIUndoHandler_h_

#include "String.h"
#include <deque>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class Window;

/*!
\brief
	Handler class for undo support

*/
class CEGUIEXPORT UndoHandler
{
public:
    UndoHandler(Window *window);

    /*!
    \brief
        Undo action types (insert, delete)
    */
    enum class UndoActionType : int
    {
        Insert = 1,
        Delete = 2
    };

    /*!
    \brief
        Struct used to store information about undo actions
    */
    struct UndoAction
    {
        UndoAction() : d_type(UndoActionType::Insert), d_startIdx(0) {}

        UndoActionType  d_type;     //!< Undo type
        size_t          d_startIdx; //!< Starting index for this line.
        String          d_text;
    };

    typedef std::deque<UndoAction>   UndoList;   //!< Type for collection of UndoActions.

    /*!
    \brief
        Perform undo

    \param cursor
        - new cursor position
    */
    bool undo(size_t &cursor);

    /*!
    \brief
        Perform redo

        \param cursor
        - new cursor position
    */
    bool redo(size_t &cursor);

    /*!
    \brief
        Add a single record to undo history
    */
    void addUndoHistory(UndoAction &action);

    /*!
    \brief
        Clear undo history
    */
    void clearUndoHistory();

    /*!
    \brief
        Set maximum number of undo steps to store.

    \param limit
        - size of undo list
    */
    void setUndoLimit(int limit);

    /*!
    \brief
        Get count of undo actions in queue
    */
    size_t count();

    /*!
    \brief
        Checks if undo action is possible
    */
    bool canUndo();

    /*!
    \brief
        Checks if redo action is possible
    */
    bool canRedo();

    /*!
    \brief
        Gets the action type of the last performed
        action added to the undo list.
    */
    UndoAction getLastAction();

private:
    UndoList    d_undoList;         //!< Holds the undo history
    size_t      d_undoLimit;        //!< Maximum numer of undo entries
    int         d_undoPosition;     //!< Position in undo list
    Window*     d_attachedWindow;   //!< Pointer to window that we will handle
    bool        d_lastUndo;         //!< True if last function called was undo
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif	// end of guard _CEGUIUndoHandler_h_
