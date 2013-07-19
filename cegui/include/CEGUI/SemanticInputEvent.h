/***********************************************************************
    filename:   SemanticInputEvent.h
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
    Represents the value of a semantic input event, generated from a specific
    operation or sequence of operations.
*/
enum SemanticValue
{
    SV_NoValue                         = 0x0000,
    SV_PointerActivate                 = 0x0001,
    SV_PointerDeactivate               = 0x0002,
    SV_PointerHold                     = 0x0003,
    SV_PointerMove                     = 0x0004,
    SV_SelectMultipleItems             = 0x0010,
    SV_SelectCumulative                = 0x0011,
    SV_SelectWord                      = 0x0012,
    SV_SelectAll                       = 0x0013,
    SV_SelectLeftCharacter             = 0x0014,
    SV_SelectRightCharacter            = 0x0015,
    SV_GoToPreviousCharacter           = 0x0020,
    SV_GoToNextCharacter               = 0x0021,
    SV_GoToPreviousWord                = 0x0022,
    SV_GoToNextWord                    = 0x0023,
    SV_GoToStartOfLine                 = 0x0024,
    SV_GoToEndOfLine                   = 0x0025,
    SV_DeleteLastCharacter             = 0x0030,
    SV_DeletePreviousCharacter         = 0x0031,
    SV_Confirm                         = 0x0050,
    SV_Back                            = 0x0051,
    SV_Undo                            = 0x0052,
    SV_Redo                            = 0x0053,
    SV_Cut                             = 0x0054,
    SV_Copy                            = 0x0055,
    SV_Paste                           = 0x0056,
    SV_HorizontalScroll                = 0x0060,
    SV_VerticalScroll                  = 0x0061,

    SV_UserDefinedSemanticValue        = 0x5000,   //!< This marks the beginning of user-defined semantic values.
};

/*!
\brief
    Represents a semantic input event (e.g.: delete a previous character, confirm)
*/
class CEGUIEXPORT SemanticInputEvent : public InputEvent
{
public:
    SemanticInputEvent(SemanticValue value) :
        InputEvent(IET_SemanticInputEventType),
        d_value(value),
        d_payload()
    {
    }

    SemanticValue d_value;            //!< The semantic value of this event
    union {
        float array[2];
        float single;
    } d_payload;                                //!< Extra data associated to this event
};

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUISemanticInputEvent_h_
