/***********************************************************************
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
#include "CEGUI/Rectf.h"
#include "CEGUI/SemanticInputEvent.h"
#include "CEGUI/SimpleTimer.h"
#include "CEGUI/System.h"

#include <iterator>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4351)
#   pragma warning(disable : 4355)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

static CursorInputSource convertToCursorInputSource(MouseButton button)
{
    if (button == MouseButton::Left)
        return CursorInputSource::Left;

    if (button == MouseButton::Right)
        return CursorInputSource::Right;

    if (button == MouseButton::Middle)
        return CursorInputSource::Middle;

    return CursorInputSource::NotSpecified;
}

/*!
\brief
    Implementation structure used in tracking up & down mouse button inputs in
    order to generate click, double-click, and triple-click events.
*/
struct MouseClickTracker
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
const String InputAggregator::EventMouseMoveScalingFactorChanged("MouseMoveScalingFactorChanged");

//----------------------------------------------------------------------------//
const float InputAggregator::DefaultMouseButtonClickTimeout = 0.0f;
const float InputAggregator::DefaultMouseButtonMultiClickTimeout = 0.3333f;
const Sizef InputAggregator::DefaultMouseButtonMultiClickTolerance(0.01f, 0.01f);

//----------------------------------------------------------------------------//
InputAggregator::InputAggregator(InputEventReceiver* input_receiver) :
    d_displaySizeChangedConnection(
        System::getSingletonPtr()->subscribeEvent(System::EventDisplaySizeChanged,
            Event::Subscriber(&InputAggregator::onDisplaySizeChanged, this))),
    d_inputReceiver(input_receiver),
    d_mouseButtonClickTimeout(DefaultMouseButtonClickTimeout),
    d_mouseButtonMultiClickTimeout(DefaultMouseButtonMultiClickTimeout),
    d_mouseButtonMultiClickTolerance(DefaultMouseButtonMultiClickTolerance),
    d_generateMouseClickEvents(true),
    d_mouseClickTrackers(new MouseClickTracker[static_cast<int>(MouseButton::Count)]),
    d_handleInKeyUp(true),
    d_mouseMovementScalingFactor(1.0f),
    d_pointerPosition(0.0f, 0.0f),
    d_keysPressed()
{
    // Initialise the array
    std::fill(std::begin(d_keyValuesMappings), std::end(d_keyValuesMappings), SemanticValue::NoValue);

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
void InputAggregator::setMouseMoveScalingFactor(float factor)
{
    d_mouseMovementScalingFactor = factor;

    InputAggregatorEventArgs args(this);
    onMouseMoveScalingFactorChanged(args);
}

//----------------------------------------------------------------------------//
float InputAggregator::getMouseMoveScalingFactor() const
{
    return d_mouseMovementScalingFactor;
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

//----------------------------------------------------------------------------//
SemanticValue InputAggregator::getSemanticAction(Key::Scan scan_code, bool shift_down,
    bool alt_down, bool ctrl_down) const
{
    SemanticValue value = d_keyValuesMappings[static_cast<unsigned char>(scan_code)];

    // handle combined keys
    if (ctrl_down && shift_down)
    {
        if (scan_code == Key::Scan::ArrowLeft)
            value = SemanticValue::SelectPreviousWord;
        else if (scan_code == Key::Scan::ArrowRight)
            value = SemanticValue::SelectNextWord;
        else if (scan_code == Key::Scan::End)
            value = SemanticValue::SelectToEndOfDocument;
        else if (scan_code == Key::Scan::Home)
            value = SemanticValue::SelectToStartOfDocument;
        else if (scan_code == Key::Scan::Z)
            value = SemanticValue::Redo;
    }
    else if (ctrl_down)
    {
        if (scan_code == Key::Scan::ArrowLeft)
            value = SemanticValue::GoToPreviousWord;
        else if (scan_code == Key::Scan::ArrowRight)
            value = SemanticValue::GoToNextWord;
        else if (scan_code == Key::Scan::End)
            value = SemanticValue::GoToEndOfDocument;
        else if (scan_code == Key::Scan::Home)
            value = SemanticValue::GoToStartOfDocument;
        else if (scan_code == Key::Scan::A)
            value = SemanticValue::SelectAll;
        else if (scan_code == Key::Scan::C)
            value = SemanticValue::Copy;
        else if (scan_code == Key::Scan::V)
            value = SemanticValue::Paste;
        else if (scan_code == Key::Scan::X)
            value = SemanticValue::Cut;
        else if (scan_code == Key::Scan::Tab)
            value = SemanticValue::NavigateToPrevious;
        else if (scan_code == Key::Scan::Z)
            value = SemanticValue::Undo;
        else if (scan_code == Key::Scan::Y)
            value = SemanticValue::Redo;
    }
    else if (shift_down)
    {
        if (scan_code == Key::Scan::ArrowLeft)
            value = SemanticValue::SelectPreviousCharacter;
        else if (scan_code == Key::Scan::ArrowRight)
            value = SemanticValue::SelectNextCharacter;
        else if (scan_code == Key::Scan::ArrowUp)
            value = SemanticValue::SelectUp;
        else if (scan_code == Key::Scan::ArrowDown)
            value = SemanticValue::SelectDown;
        else if (scan_code == Key::Scan::End)
            value = SemanticValue::SelectToEndOfLine;
        else if (scan_code == Key::Scan::Home)
            value = SemanticValue::SelectToStartOfLine;
        else if (scan_code == Key::Scan::PageUp)
            value = SemanticValue::SelectPreviousPage;
        else if (scan_code == Key::Scan::PageDown)
            value = SemanticValue::SelectNextPage;
    }
    if (alt_down)
    {
        if(scan_code == Key::Scan::Backspace)
            value = SemanticValue::Undo;
    }

    return value;
}

bool InputAggregator::handleScanCode(Key::Scan scan_code, bool shift_down,
     bool alt_down, bool ctrl_down)
{

    SemanticValue value = getSemanticAction(scan_code, shift_down, alt_down,
        ctrl_down);

    if (value != SemanticValue::NoValue)
    {
        SemanticInputEvent semantic_event(value);
        return d_inputReceiver->injectInputEvent(semantic_event);
    }

    return false;
}
//----------------------------------------------------------------------------//
void InputAggregator::onMouseMoveScalingFactorChanged(InputAggregatorEventArgs& args)
{
    fireEvent(EventMouseMoveScalingFactorChanged, args);
}

bool InputAggregator::injectMouseMove(float delta_x, float delta_y)
{
    return injectMousePosition(
        delta_x + d_pointerPosition.x * d_mouseMovementScalingFactor,
        delta_y + d_pointerPosition.y * d_mouseMovementScalingFactor);
}

bool InputAggregator::injectMousePosition(float x_pos, float y_pos)
{
    if (d_inputReceiver == nullptr)
        return false;

    d_pointerPosition = glm::vec2(x_pos, y_pos);

    SemanticInputEvent semantic_event(SemanticValue::CursorMove);
    semantic_event.d_payload.array[0] = x_pos;
    semantic_event.d_payload.array[1] = y_pos;

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseLeaves()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::PointerLeave);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseButtonDown(MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    //
    // Handling for multi-click generation
    //
    MouseClickTracker& tkr = d_mouseClickTrackers[static_cast<int>(button)];

    tkr.d_click_count++;

    // TODO: re-add the check for different windows?
    // if multi-click requirements are not met
    if (((d_mouseButtonMultiClickTimeout > 0) && (tkr.d_timer.elapsedTime() > d_mouseButtonMultiClickTimeout)) ||
        (!tkr.d_click_area.isPointInRectf(d_pointerPosition)) ||
        (tkr.d_click_count > 3))
    {
        // reset to single down event.
        tkr.d_click_count = 1;

        // build new allowable area for multi-clicks
        tkr.d_click_area.setPosition(d_pointerPosition);
        tkr.d_click_area.setSize(d_mouseButtonMultiClickAbsoluteTolerance);
        tkr.d_click_area.offset(-0.5f * glm::vec2(
            d_mouseButtonMultiClickAbsoluteTolerance.d_width,
            d_mouseButtonMultiClickAbsoluteTolerance.d_height));
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

    SemanticValue value = SemanticValue::CursorPressHold;
    if (isControlPressed())
        value = SemanticValue::SelectCumulative;
    else if (isShiftPressed())
        value = SemanticValue::SelectRange;

    SemanticInputEvent semantic_event(value);
    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}
bool InputAggregator::injectMouseButtonUp(MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::CursorActivate);
    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectKeyDown(Key::Scan scan_code)
{
    if (d_inputReceiver == nullptr)
        return false;
    
    d_keysPressed[static_cast<unsigned char>(scan_code)] = true;

    if (d_handleInKeyUp)
        return true;

    return handleScanCode(scan_code, isShiftPressed(), isAltPressed(),
        isControlPressed());
}

bool InputAggregator::injectKeyUp(Key::Scan scan_code)
{
    if (d_inputReceiver == nullptr)
        return false;
    
    d_keysPressed[static_cast<unsigned char>(scan_code)] = false;

    if (!d_handleInKeyUp)
        return true;
    
    return handleScanCode(scan_code, isShiftPressed(), isAltPressed(),
        isControlPressed());
}

bool InputAggregator::injectChar(char32_t code_point)
{
    if (d_inputReceiver == nullptr)
        return false;

    TextInputEvent text_event;
    text_event.d_character = code_point;

    return d_inputReceiver->injectInputEvent(text_event);
}

bool InputAggregator::injectMouseWheelChange(float delta)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::VerticalScroll);
    semantic_event.d_payload.single = delta;

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseButtonClick(const MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::CursorActivate);

    if (isControlPressed())
        semantic_event.d_value = SemanticValue::SelectCumulative;

    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseButtonDoubleClick(const MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::SelectWord);
    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseButtonTripleClick(const MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::SelectAll);
    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectCopyRequest()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::Copy);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectCutRequest()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::Cut);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectPasteRequest()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SemanticValue::Paste);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

void InputAggregator::initialise(bool handle_on_keyup /*= true*/)
{
    d_handleInKeyUp = handle_on_keyup;
    
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Backspace)] = SemanticValue::DeletePreviousCharacter;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::DeleteKey)] = SemanticValue::DeleteNextCharacter;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::NumpadEnter)] = SemanticValue::Confirm;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Return)] = SemanticValue::Confirm;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Tab)] = SemanticValue::NavigateToNext;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowLeft)] = SemanticValue::GoToPreviousCharacter;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowRight)] = SemanticValue::GoToNextCharacter;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowDown)] = SemanticValue::GoDown;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowUp)] = SemanticValue::GoUp;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::End)] = SemanticValue::GoToEndOfLine;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Home)] = SemanticValue::GoToStartOfLine;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::PageDown)] = SemanticValue::GoToNextPage;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::PageUp)] = SemanticValue::GoToPreviousPage;
}

bool InputAggregator::isShiftPressed()
{
    return d_keysPressed[static_cast<unsigned char>(Key::Scan::LeftShift)] ||
        d_keysPressed[static_cast<unsigned char>(Key::Scan::RightShift)];
}

bool InputAggregator::isAltPressed()
{
    return d_keysPressed[static_cast<unsigned char>(Key::Scan::LeftAlt)] ||
        d_keysPressed[static_cast<unsigned char>(Key::Scan::RightAlt)];
}

bool InputAggregator::isControlPressed()
{
    return d_keysPressed[static_cast<unsigned char>(Key::Scan::LeftControl)] ||
        d_keysPressed[static_cast<unsigned char>(Key::Scan::RightControl)];
}
//----------------------------------------------------------------------------//
void InputAggregator::setModifierKeys(bool shift_down, bool alt_down,
    bool ctrl_down)
{
    d_keysPressed[static_cast<unsigned char>(Key::Scan::LeftShift)] = shift_down;
    d_keysPressed[static_cast<unsigned char>(Key::Scan::RightShift)] = shift_down;

    d_keysPressed[static_cast<unsigned char>(Key::Scan::LeftAlt)] = alt_down;
    d_keysPressed[static_cast<unsigned char>(Key::Scan::RightAlt)] = alt_down;

    d_keysPressed[static_cast<unsigned char>(Key::Scan::LeftControl)] = ctrl_down;
    d_keysPressed[static_cast<unsigned char>(Key::Scan::RightControl)] = ctrl_down;
}
//----------------------------------------------------------------------------//
void InputAggregator::recomputeMultiClickAbsoluteTolerance()
{
    const Sizef& display_size = System::getSingleton().getRenderer()->getDisplaySize();
    d_mouseButtonMultiClickAbsoluteTolerance = Sizef(
        d_mouseButtonMultiClickTolerance.d_width * display_size.d_width,
        d_mouseButtonMultiClickTolerance.d_height * display_size.d_height);
}

bool InputAggregator::onDisplaySizeChanged(const EventArgs&)
{
    recomputeMultiClickAbsoluteTolerance();
    return true;
}

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

} // End of  CEGUI namespace section
