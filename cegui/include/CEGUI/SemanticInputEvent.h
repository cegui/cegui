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
enum CursorInputSource
{
    CIS_None,        //!< No specific cursor source
    CIS_Left,        //!< Left cursor source
    CIS_Right,       //!< Right cursor source
    CIS_Middle,      //!< Middle cursor source
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
        return (d_state & (1 << source)) != 0;
    }

    void pointerHold(CursorInputSource source)
    {
        d_state |= (1 << source);
    }

    void pointerDeactivated(CursorInputSource source)
    {
        d_state &= ~(1 << source);
    }

private:
    unsigned int d_state;
};

/*!
\brief
    Represents the value of a semantic input event, generated from a specific
    operation or sequence of operations.
*/
enum SemanticValue
{
    SV_NoValue = 0x0000,
    SV_CursorActivate,
    SV_PointerDeactivate,
    SV_CursorPressHold,
    SV_CursorMove,
    SV_PointerLeave,
    SV_SelectRange,
    SV_SelectCumulative,
    SV_SelectWord,
    SV_SelectAll,
    SV_SelectPreviousCharacter,
    SV_SelectNextCharacter,
    SV_SelectPreviousWord,
    SV_SelectNextWord,
    SV_SelectToStartOfLine,
    SV_SelectToEndOfLine,
    SV_GoToPreviousCharacter,
    SV_GoToNextCharacter,
    SV_GoToPreviousWord,
    SV_GoToNextWord,
    SV_GoToStartOfLine,
    SV_GoToEndOfLine,
    SV_GoToStartOfDocument,
    SV_GoToEndOfDocument,
    SV_GoToNextPage,
    SV_GoToPreviousPage,
    SV_DeleteNextCharacter,
    SV_DeletePreviousCharacter,
    SV_Confirm,
    SV_Back,
    SV_Undo,
    SV_Redo,
    SV_Cut,
    SV_Copy,
    SV_Paste,
    SV_HorizontalScroll,
    SV_VerticalScroll,
    SV_SelectToStartOfDocument,
    SV_SelectToEndOfDocument,
    SV_SelectToNextPage,
    SV_SelectToPreviousPage,
    SV_SelectNextPage,
    SV_SelectPreviousPage,
    SV_GoUp,
    SV_GoDown,
    SV_SelectUp,
    SV_SelectDown,
    SV_NavigateToNext,
    SV_NavigateToPrevious,

    SV_UserDefinedSemanticValue = 0x5000,   //!< This marks the beginning of user-defined semantic values.
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
    SemanticInputEvent(int value) :
        InputEvent(InputEventType::SemanticInputEventType),
        d_value(value),
        d_payload()
    {
    }

    int d_value;                //!< The semantic value of this event
    SemanticPayload d_payload;  //!< Extra data associated to this event
};

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUISemanticInputEvent_h_
