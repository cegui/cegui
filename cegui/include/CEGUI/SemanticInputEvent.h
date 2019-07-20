/***********************************************************************
    created:    18/7/2013
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Holds the semantic input events used by the Input Aggregator
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
#ifndef _CEGUISemanticInputEvent_h_
#define _CEGUISemanticInputEvent_h_

#include "CEGUI/InputEvents.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Represents the source of an event that uses the cursor.
*/
enum class CursorInputSource : int
{
    NotSpecified,   //!< No specific cursor source
    Left,           //!< Left cursor source
    Right,          //!< Right cursor source
    Middle,         //!< Middle cursor source
};

/*!
\brief
    Holds the state of the cursors (held or not)
*/
class CursorsState
{
public:
    CursorsState() :
        d_state(0)
    {
    }

    unsigned int get() const { return d_state; }

    bool isHeld(CursorInputSource source)
    {
        return (d_state & (1 << static_cast<int>(source))) != 0;
    }

    void pointerHold(CursorInputSource source)
    {
        d_state |= (1 << static_cast<int>(source));
    }

    void pointerDeactivated(CursorInputSource source)
    {
        d_state &= ~(1 << static_cast<int>(source));
    }

private:
    unsigned int d_state;
};

/*!
\brief
    Represents the value of a semantic input event, generated from a specific
    operation or sequence of operations.
*/
enum class SemanticValue : int
{
    NoValue = 0x0000,
    CursorActivate,
    PointerDeactivate,
    CursorPressHold,
    CursorMove,
    PointerLeave,
    SelectRange,
    SelectCumulative,
    SelectWord,
    SelectAll,
    SelectPreviousCharacter,
    SelectNextCharacter,
    SelectPreviousWord,
    SelectNextWord,
    SelectToStartOfLine,
    SelectToEndOfLine,
    GoToPreviousCharacter,
    GoToNextCharacter,
    GoToPreviousWord,
    GoToNextWord,
    GoToStartOfLine,
    GoToEndOfLine,
    GoToStartOfDocument,
    GoToEndOfDocument,
    GoToNextPage,
    GoToPreviousPage,
    DeleteNextCharacter,
    DeletePreviousCharacter,
    Confirm,
    Back,
    Undo,
    Redo,
    Cut,
    Copy,
    Paste,
    HorizontalScroll,
    VerticalScroll,
    SelectToStartOfDocument,
    SelectToEndOfDocument,
    SelectToNextPage,
    SelectToPreviousPage,
    SelectNextPage,
    SelectPreviousPage,
    GoUp,
    GoDown,
    SelectUp,
    SelectDown,
    NavigateToNext,
    NavigateToPrevious,
    SpawnNewDialog,

    UserDefinedSemanticValue = 0x5000,   //!< This marks the beginning of user-defined semantic values.
};

/*!
\brief
    The type of the payload used in the semantic input events
*/
union SemanticPayload
{
    float array[2];
    float single;
    CursorInputSource source;
};

/*!
\brief
    Represents a semantic input event (e.g.: delete a previous character, confirm)
*/
class CEGUIEXPORT SemanticInputEvent : public InputEvent
{
public:
    SemanticInputEvent(SemanticValue value) :
        InputEvent(InputEventType::SemanticInputEventType),
        d_value(value),
        d_payload()
    {
    }

    SemanticValue d_value;                //!< The semantic value of this event
    SemanticPayload d_payload;  //!< Extra data associated to this event
};

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUISemanticInputEvent_h_
