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

static PointerSource convertToPointerSource(MouseButton button)
{
    if (button == LeftButton)
        return PS_Left;

    if (button == RightButton)
        return PS_Right;

    if (button == MiddleButton)
        return PS_Middle;

    return PS_None;
}

InputAggregator::InputAggregator(InputEventReceiver* input_receiver) :
    d_inputReceiver(input_receiver),
    d_pointerPosition(0.0f, 0.0f),
    d_keysPressed()
{
    initializeSimpleKeyMappings();
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

    SemanticInputEvent semantic_event(SV_PointerMove);
    semantic_event.d_payload.array[0] = x_pos;
    semantic_event.d_payload.array[1] = y_pos;

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectMouseLeaves()
{
    return true;
}

bool InputAggregator::injectMouseButtonDown(MouseButton button)
{
    SemanticValue value = SV_PointerHold;
    if (isControlPressed())
        value = SV_SelectCumulative;
    else if (isShiftPressed())
        value = SV_SelectMultipleItems;

    SemanticInputEvent semantic_event(value);
    semantic_event.d_payload.source = convertToPointerSource(button);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}
bool InputAggregator::injectMouseButtonUp(MouseButton button)
{
    SemanticInputEvent semantic_event(SV_PointerActivate);
    semantic_event.d_payload.source = convertToPointerSource(button);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectKeyDown(Key::Scan scan_code)
{
    d_keysPressed[scan_code] = true;
    return true;
}

bool InputAggregator::injectKeyUp(Key::Scan scan_code)
{
    SemanticValue value = d_keyValuesMappings[scan_code];

    // handle combined keys
    if (isControlPressed())
    {
        if (scan_code == Key::ArrowLeft)
            value = SV_GoToPreviousWord;
        else if (scan_code == Key::ArrowRight)
            value = SV_GoToNextWord;
    }
    else if (isShiftPressed())
    {
        if (scan_code == Key::ArrowLeft)
            value = SV_SelectPreviousCharacter;
        else if (scan_code == Key::ArrowRight)
            value = SV_SelectNextCharacter;
    }

    if (value != SV_NoValue)
    {
        SemanticInputEvent semantic_event(value);

        d_inputReceiver->injectInputEvent(semantic_event);

        d_keysPressed[scan_code] = false;
    }
    return true;
}

bool InputAggregator::injectChar(String::value_type code_point)
{
    TextInputEvent text_event;
    text_event.d_character = code_point;

    d_inputReceiver->injectInputEvent(text_event);
    return true;
}

bool InputAggregator::injectMouseWheelChange(float delta)
{
    SemanticInputEvent semantic_event(SV_VerticalScroll);
    semantic_event.d_payload.single = delta;

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectMouseButtonClick(const MouseButton button)
{
    SemanticInputEvent semantic_event(SV_PointerActivate);

    if (isControlPressed())
        semantic_event.d_value = SV_SelectCumulative;

    semantic_event.d_payload.source = convertToPointerSource(button);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectMouseButtonDoubleClick(const MouseButton button)
{
    SemanticInputEvent semantic_event(SV_SelectWord);
    semantic_event.d_payload.source = convertToPointerSource(button);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectMouseButtonTripleClick(const MouseButton button)
{
    SemanticInputEvent semantic_event(SV_SelectAll);
    semantic_event.d_payload.source = convertToPointerSource(button);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectCopyRequest()
{
    SemanticInputEvent semantic_event(SV_Copy);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectCutRequest()
{
    SemanticInputEvent semantic_event(SV_Cut);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

bool InputAggregator::injectPasteRequest()
{
    SemanticInputEvent semantic_event(SV_Paste);

    d_inputReceiver->injectInputEvent(semantic_event);

    return true;
}

void InputAggregator::initializeSimpleKeyMappings()
{
    // Initialize the array
    memset(d_keyValuesMappings, SV_NoValue, sizeof(SemanticValue) * 0xFF);

    d_keyValuesMappings[Key::Backspace] = SV_DeletePreviousCharacter;
    d_keyValuesMappings[Key::Delete] = SV_DeleteNextCharacter;

    d_keyValuesMappings[Key::NumpadEnter] = SV_Confirm;
    d_keyValuesMappings[Key::Tab] = SV_Confirm;
    d_keyValuesMappings[Key::Return] = SV_Confirm;

    d_keyValuesMappings[Key::ArrowLeft] = SV_GoToPreviousCharacter;
    d_keyValuesMappings[Key::ArrowRight] = SV_GoToNextCharacter;
    d_keyValuesMappings[Key::End] = SV_GoToEndOfLine;
    d_keyValuesMappings[Key::Home] = SV_GoToStartOfLine;
}

bool InputAggregator::isShiftPressed()
{
    return d_keysPressed[Key::LeftShift] || d_keysPressed[Key::RightShift];
}

bool InputAggregator::isAltPressed()
{
    return d_keysPressed[Key::LeftAlt] || d_keysPressed[Key::RightAlt];
}

bool InputAggregator::isControlPressed()
{
    return d_keysPressed[Key::LeftControl] || d_keysPressed[Key::RightControl];
}

} // End of  CEGUI namespace section
