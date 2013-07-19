/***********************************************************************
    filename:   InputAggregator.cpp
    created:    10/7/2013
    author:     Timotei Dolean <timotei21@gmail.com>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/InputAggregator.h"
#include "CEGUI/SemanticInputEvent.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    InputAggregator::InputAggregator(InputEventReceiver* input_receiver) :
        d_inputReceiver(input_receiver),
        d_pointerPosition(0.0f, 0.0f)
    {
    }

    bool InputAggregator::injectTimePulse(float timeElapsed)
    {
        return true;
    }

    bool InputAggregator::injectMouseMove(float delta_x, float delta_y)
    {
        return injectMousePosition(delta_x + d_pointerPosition.d_x,
            delta_y + d_pointerPosition.d_y);
    }

    bool InputAggregator::injectMousePosition(float x_pos, float y_pos)
    {
        d_pointerPosition = Vector2f(x_pos, y_pos);

        SemanticInputEvent semantic_event(PointerMove);
        semantic_event.d_payload.array[0] = x_pos;
        semantic_event.d_payload.array[1] = y_pos;

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectMouseLeaves()
    {
        return true;
    }

    bool InputAggregator::injectMouseButtonDown(MouseButton button)
    {
        SemanticInputEvent semantic_event(PointerHold);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }
    bool InputAggregator::injectMouseButtonUp(MouseButton button)
    {
        SemanticInputEvent semantic_event(PointerActivate);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectKeyDown(Key::Scan scan_code)
    {
        /* Ignored for now */
        return true;
    }

    bool InputAggregator::injectKeyUp(Key::Scan scan_code)
    {

        return true;
    }

    bool InputAggregator::injectChar(String::value_type code_point)
    {
        TextInputEvent text_event;
        text_event.d_character = code_point;

        d_inputReceiver->injectInputEvent(&text_event);
        return true;
    }

    bool InputAggregator::injectMouseWheelChange(float delta)
    {
        SemanticInputEvent semantic_event(VerticalScroll);
        semantic_event.d_payload.single = delta;

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectMouseButtonClick(const MouseButton button)
    {
        SemanticInputEvent semantic_event(PointerActivate);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectMouseButtonDoubleClick(const MouseButton button)
    {
        SemanticInputEvent semantic_event(SelectWord);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectMouseButtonTripleClick(const MouseButton button)
    {
        SemanticInputEvent semantic_event(SelectAll);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectCopyRequest()
    {
        SemanticInputEvent semantic_event(Copy);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectCutRequest()
    {
        SemanticInputEvent semantic_event(Cut);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }

    bool InputAggregator::injectPasteRequest()
    {
        SemanticInputEvent semantic_event(Paste);

        d_inputReceiver->injectInputEvent(&semantic_event);

        return true;
    }
} // End of  CEGUI namespace section
