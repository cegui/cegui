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
#include "CEGUI/CoordConverter.h"
#include "CEGUI/InputAggregator.h"
#include "CEGUI/Rect.h"
#include "CEGUI/SemanticInputEvent.h"
#include "CEGUI/SimpleTimer.h"
#include "CEGUI/System.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4351)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Implementation structure used in tracking up & down mouse button inputs in
    order to generate click, double-click, and triple-click events.
*/
struct MouseClickTracker :
    public AllocatedObject<MouseClickTracker>
{
    MouseClickTracker() :
        d_click_count(0),
        d_click_area(0, 0, 0, 0)
    {}

    //! Timer used to track clicks for this button.
    SimpleTimer d_timer;
    //! count of clicks made so far.
    int d_click_count;
    //! area used to detect multi-clicks
    Rectf d_click_area;
};

//----------------------------------------------------------------------------//
const String InputAggregator::EventMouseButtonClickTimeoutChanged("MouseButtonClickTimeoutChanged" );
const String InputAggregator::EventMouseButtonMultiClickTimeoutChanged("MouseButtonMultiClickTimeoutChanged" );
const String InputAggregator::EventMouseButtonMultiClickToleranceChanged("MouseButtonMultiClickToleranceChanged" );

//----------------------------------------------------------------------------//
const float InputAggregator::DefaultMouseButtonClickTimeout = 0.0f;
const float InputAggregator::DefaultMouseButtonMultiClickTimeout = 0.3333f;
const Sizef InputAggregator::DefaultMouseButtonMultiClickTolerance(0.01f, 0.01f);

//----------------------------------------------------------------------------//
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

//----------------------------------------------------------------------------//
InputAggregator::InputAggregator(InputEventReceiver* input_receiver) :
    d_inputReceiver(input_receiver),
    d_mouseButtonClickTimeout(DefaultMouseButtonClickTimeout),
    d_mouseButtonMultiClickTimeout(DefaultMouseButtonMultiClickTimeout),
    d_mouseButtonMultiClickTolerance(DefaultMouseButtonMultiClickTolerance),
    d_generateMouseClickEvents(true),
    d_mouseClickTrackers(new MouseClickTracker[MouseButtonCount]),
    d_pointerPosition(0.0f, 0.0f),
    d_displaySizeChangedConnection(
        System::getSingletonPtr()->subscribeEvent(System::EventDisplaySizeChanged,
            Event::Subscriber(&InputAggregator::onDisplaySizeChanged, this))),
    d_keysPressed()
{
    initializeSimpleKeyMappings();
    // initial absolute tolerance
    recomputeMultiClickAbsoluteTolerance();
}

//----------------------------------------------------------------------------//
InputAggregator::~InputAggregator()
{
    d_displaySizeChangedConnection->disconnect();
    delete[] d_mouseClickTrackers;
}

//----------------------------------------------------------------------------//
void InputAggregator::setMouseClickEventGenerationEnabled(const bool enable)
{
    d_generateMouseClickEvents = enable;
}

//----------------------------------------------------------------------------//
bool InputAggregator::isMouseClickEventGenerationEnabled() const
{
    return d_generateMouseClickEvents;
}

//----------------------------------------------------------------------------//
void InputAggregator::setMouseButtonClickTimeout(float seconds)
{
    d_mouseButtonClickTimeout = seconds;

    InputAggregatorEventArgs args(this);
    onMouseButtonClickTimeoutChanged(args);
}

//----------------------------------------------------------------------------//
float InputAggregator::getMouseButtonClickTimeout() const
{
    return d_mouseButtonClickTimeout;
}

//----------------------------------------------------------------------------//
void InputAggregator::setMouseButtonMultiClickTimeout(float seconds)
{
    d_mouseButtonMultiClickTimeout = seconds;

    InputAggregatorEventArgs args(this);
    onMouseButtonMultiClickTimeoutChanged(args);
}

//----------------------------------------------------------------------------//
float InputAggregator::getMouseButtonMultiClickTimeout() const
{
    return d_mouseButtonMultiClickTimeout;
}

//----------------------------------------------------------------------------//
void InputAggregator::setMouseButtonMultiClickTolerance(const Sizef& sz)
{
    d_mouseButtonMultiClickTolerance = sz;

    InputAggregatorEventArgs args(this);
    onMouseButtonMultiClickToleranceChanged(args);
}

//----------------------------------------------------------------------------//
const Sizef& InputAggregator::getMouseButtonMultiClickTolerance() const
{
    return d_mouseButtonMultiClickTolerance;
}

//----------------------------------------------------------------------------//
void InputAggregator::onMouseButtonClickTimeoutChanged(InputAggregatorEventArgs& args)
{
    fireEvent(EventMouseButtonClickTimeoutChanged, args);
}

//----------------------------------------------------------------------------//
void InputAggregator::onMouseButtonMultiClickTimeoutChanged(InputAggregatorEventArgs& args)
{
    fireEvent(EventMouseButtonMultiClickTimeoutChanged, args);
}

//----------------------------------------------------------------------------//
void InputAggregator::onMouseButtonMultiClickToleranceChanged(InputAggregatorEventArgs& args)
{
    fireEvent(EventMouseButtonMultiClickToleranceChanged, args);
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
    if (d_inputReceiver == 0)
        return false;

    d_pointerPosition = Vector2f(x_pos, y_pos);

    SemanticInputEvent semantic_event(SV_PointerMove);
    semantic_event.d_payload.array[0] = x_pos;
    semantic_event.d_payload.array[1] = y_pos;

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseLeaves()
{
    return true;
}

bool InputAggregator::injectMouseButtonDown(MouseButton button)
{
    if (d_inputReceiver == 0)
        return false;

    //
    // Handling for multi-click generation
    //
    MouseClickTracker& tkr = d_mouseClickTrackers[button];

    tkr.d_click_count++;

    // TODO: re-add the check for different windows?
    // if multi-click requirements are not met
    if (((d_mouseButtonMultiClickTimeout > 0) && (tkr.d_timer.elapsed() > d_mouseButtonMultiClickTimeout)) ||
        (!tkr.d_click_area.isPointInRect(d_pointerPosition)) ||
        (tkr.d_click_count > 3))
    {
        // reset to single down event.
        tkr.d_click_count = 1;

        // build new allowable area for multi-clicks
        tkr.d_click_area.setPosition(d_pointerPosition);
        tkr.d_click_area.setSize(d_mouseButtonMultiClickAbsoluteTolerance);
        tkr.d_click_area.offset(Vector2f(
            -(d_mouseButtonMultiClickAbsoluteTolerance.d_width / 2),
            -(d_mouseButtonMultiClickAbsoluteTolerance.d_height / 2)));
    }

    // reset timer for this tracker.
    tkr.d_timer.restart();

    if (d_generateMouseClickEvents)
    {
        switch (tkr.d_click_count)
        {
        case 2:
            return injectMouseButtonDoubleClick(button);

        case 3:
            return injectMouseButtonTripleClick(button);
        }
    }

    SemanticValue value = SV_PointerPressHold;
    if (isControlPressed())
        value = SV_SelectCumulative;
    else if (isShiftPressed())
        value = SV_SelectMultipleItems;

    SemanticInputEvent semantic_event(value);
    semantic_event.d_payload.source = convertToPointerSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}
bool InputAggregator::injectMouseButtonUp(MouseButton button)
{
    if (d_inputReceiver == 0) 
        return false;

    SemanticInputEvent semantic_event(SV_PointerActivate);
    semantic_event.d_payload.source = convertToPointerSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectKeyDown(Key::Scan scan_code)
{
    d_keysPressed[scan_code] = true;
    return true;
}

bool InputAggregator::injectKeyUp(Key::Scan scan_code)
{
    if (d_inputReceiver == 0)
        return false;

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

        d_keysPressed[scan_code] = false;

        return d_inputReceiver->injectInputEvent(semantic_event);
    }

    return false;
}

bool InputAggregator::injectChar(String::value_type code_point)
{
    if (d_inputReceiver == 0)
        return false;

    TextInputEvent text_event;
    text_event.d_character = code_point;

    return d_inputReceiver->injectInputEvent(text_event);
}

bool InputAggregator::injectMouseWheelChange(float delta)
{
    if (d_inputReceiver == 0)
        return false;

    SemanticInputEvent semantic_event(SV_VerticalScroll);
    semantic_event.d_payload.single = delta;

    return d_inputReceiver->injectInputEvent(semantic_event);;
}

bool InputAggregator::injectMouseButtonClick(const MouseButton button)
{
    if (d_inputReceiver == 0)
        return false;

    SemanticInputEvent semantic_event(SV_PointerActivate);

    if (isControlPressed())
        semantic_event.d_value = SV_SelectCumulative;

    semantic_event.d_payload.source = convertToPointerSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);;
}

bool InputAggregator::injectMouseButtonDoubleClick(const MouseButton button)
{
    if (d_inputReceiver == 0)
        return false;

    SemanticInputEvent semantic_event(SV_SelectWord);
    semantic_event.d_payload.source = convertToPointerSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);;
}

bool InputAggregator::injectMouseButtonTripleClick(const MouseButton button)
{
    if (d_inputReceiver == 0)
        return false;

    SemanticInputEvent semantic_event(SV_SelectAll);
    semantic_event.d_payload.source = convertToPointerSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectCopyRequest()
{
    if (d_inputReceiver == 0)
        return false;

    SemanticInputEvent semantic_event(SV_Copy);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectCutRequest()
{
    if (d_inputReceiver == 0)
        return false;

    SemanticInputEvent semantic_event(SV_Cut);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectPasteRequest()
{
    if (d_inputReceiver == 0)
        return false;

    SemanticInputEvent semantic_event(SV_Paste);

    return d_inputReceiver->injectInputEvent(semantic_event);
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

//----------------------------------------------------------------------------//
void InputAggregator::recomputeMultiClickAbsoluteTolerance()
{
    const Sizef& display_size = System::getSingleton().getRenderer()->getDisplaySize();
    d_mouseButtonMultiClickAbsoluteTolerance = Sizef(
        d_mouseButtonMultiClickTolerance.d_width * display_size.d_width,
        d_mouseButtonMultiClickTolerance.d_height * display_size.d_height);
}

bool InputAggregator::onDisplaySizeChanged(const EventArgs& args)
{
    recomputeMultiClickAbsoluteTolerance();
    return true;
}

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

} // End of  CEGUI namespace section
