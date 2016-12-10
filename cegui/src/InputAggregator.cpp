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
    if (button == MouseButton::LEFT_BUTTON)
        return CIS_Left;

    if (button == MouseButton::RIGHT_BUTTON)
        return CIS_Right;

    if (button == MouseButton::MIDDLE_BUTTON)
        return CIS_Middle;

    return CIS_None;
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
    d_mouseClickTrackers(new MouseClickTracker[static_cast<int>(MouseButton::COUNT)]),
    d_handleInKeyUp(true),
    d_mouseMovementScalingFactor(1.0f),
    d_pointerPosition(0.0f, 0.0f),
    d_keysPressed()
{
    // Initialise the array
    std::fill(std::begin(d_keyValuesMappings), std::end(d_keyValuesMappings), SV_NoValue);

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
int InputAggregator::getSemanticAction(Key::Scan scan_code, bool shift_down,
    bool alt_down, bool ctrl_down) const
{
    int value = d_keyValuesMappings[static_cast<signed char>(scan_code)];

    // handle combined keys
    if (ctrl_down && shift_down)
    {
        if (scan_code == Key::Scan::ARROW_LEFT)
            value = SV_SelectPreviousWord;
        else if (scan_code == Key::Scan::ARROW_RIGHT)
            value = SV_SelectNextWord;
        else if (scan_code == Key::Scan::END)
            value = SV_SelectToEndOfDocument;
        else if (scan_code == Key::Scan::HOME)
            value = SV_SelectToStartOfDocument;
        else if (scan_code == Key::Scan::Z)
            value = SV_Redo;
    }
    else if (ctrl_down)
    {
        if (scan_code == Key::Scan::ARROW_LEFT)
            value = SV_GoToPreviousWord;
        else if (scan_code == Key::Scan::ARROW_RIGHT)
            value = SV_GoToNextWord;
        else if (scan_code == Key::Scan::END)
            value = SV_GoToEndOfDocument;
        else if (scan_code == Key::Scan::HOME)
            value = SV_GoToStartOfDocument;
        else if (scan_code == Key::Scan::A)
            value = SV_SelectAll;
        else if (scan_code == Key::Scan::C)
            value = SV_Copy;
        else if (scan_code == Key::Scan::V)
            value = SV_Paste;
        else if (scan_code == Key::Scan::X)
            value = SV_Cut;
        else if (scan_code == Key::Scan::TAB)
            value = SV_NavigateToPrevious;
        else if (scan_code == Key::Scan::Z)
            value = SV_Undo;
        else if (scan_code == Key::Scan::Y)
            value = SV_Redo;
    }
    else if (shift_down)
    {
        if (scan_code == Key::Scan::ARROW_LEFT)
            value = SV_SelectPreviousCharacter;
        else if (scan_code == Key::Scan::ARROW_RIGHT)
            value = SV_SelectNextCharacter;
        else if (scan_code == Key::Scan::ARROW_UP)
            value = SV_SelectUp;
        else if (scan_code == Key::Scan::ARROW_DOWN)
            value = SV_SelectDown;
        else if (scan_code == Key::Scan::END)
            value = SV_SelectToEndOfLine;
        else if (scan_code == Key::Scan::HOME)
            value = SV_SelectToStartOfLine;
        else if (scan_code == Key::Scan::PAGE_UP)
            value = SV_SelectPreviousPage;
        else if (scan_code == Key::Scan::PAGE_DOWN)
            value = SV_SelectNextPage;
    }
    if (alt_down)
    {
        if(scan_code == Key::Scan::BACKSPACE)
            value = SV_Undo;
    }

    return value;
}

bool InputAggregator::handleScanCode(Key::Scan scan_code, bool shift_down,
     bool alt_down, bool ctrl_down)
{

    int value = getSemanticAction(scan_code, shift_down, alt_down,
        ctrl_down);

    if (value != SV_NoValue)
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

    SemanticInputEvent semantic_event(SV_CursorMove);
    semantic_event.d_payload.array[0] = x_pos;
    semantic_event.d_payload.array[1] = y_pos;

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseLeaves()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_PointerLeave);

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

    SemanticValue value = SV_CursorPressHold;
    if (isControlPressed())
        value = SV_SelectCumulative;
    else if (isShiftPressed())
        value = SV_SelectRange;

    SemanticInputEvent semantic_event(value);
    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}
bool InputAggregator::injectMouseButtonUp(MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_CursorActivate);
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

    SemanticInputEvent semantic_event(SV_VerticalScroll);
    semantic_event.d_payload.single = delta;

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseButtonClick(const MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_CursorActivate);

    if (isControlPressed())
        semantic_event.d_value = SV_SelectCumulative;

    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseButtonDoubleClick(const MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_SelectWord);
    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectMouseButtonTripleClick(const MouseButton button)
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_SelectAll);
    semantic_event.d_payload.source = convertToCursorInputSource(button);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectCopyRequest()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_Copy);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectCutRequest()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_Cut);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

bool InputAggregator::injectPasteRequest()
{
    if (d_inputReceiver == nullptr)
        return false;

    SemanticInputEvent semantic_event(SV_Paste);

    return d_inputReceiver->injectInputEvent(semantic_event);
}

void InputAggregator::initialise(bool handle_on_keyup /*= true*/)
{
    d_handleInKeyUp = handle_on_keyup;
    
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::BACKSPACE)] = SV_DeletePreviousCharacter;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::KEY_DELETE)] = SV_DeleteNextCharacter;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::NUMPAD_ENTER)] = SV_Confirm;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::RETURN)] = SV_Confirm;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::TAB)] = SV_NavigateToNext;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ARROW_LEFT)] = SV_GoToPreviousCharacter;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ARROW_RIGHT)] = SV_GoToNextCharacter;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ARROW_DOWN)] = SV_GoDown;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ARROW_UP)] = SV_GoUp;

    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::END)] = SV_GoToEndOfLine;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::HOME)] = SV_GoToStartOfLine;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::PAGE_DOWN)] = SV_GoToNextPage;
    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::PAGE_UP)] = SV_GoToPreviousPage;
}

bool InputAggregator::isShiftPressed()
{
    return d_keysPressed[static_cast<unsigned char>(Key::Scan::LEFT_SHIFT)] ||
        d_keysPressed[static_cast<unsigned char>(Key::Scan::RIGHT_SHIFT)];
}

bool InputAggregator::isAltPressed()
{
    return d_keysPressed[static_cast<unsigned char>(Key::Scan::LEFT_ALT)] ||
        d_keysPressed[static_cast<unsigned char>(Key::Scan::RIGHT_ALT)];
}

bool InputAggregator::isControlPressed()
{
    return d_keysPressed[static_cast<unsigned char>(Key::Scan::LEFT_CONTROL)] ||
        d_keysPressed[static_cast<unsigned char>(Key::Scan::RIGHT_CONTROL)];
}
//----------------------------------------------------------------------------//
void InputAggregator::setModifierKeys(bool shift_down, bool alt_down,
    bool ctrl_down)
{
    d_keysPressed[static_cast<unsigned char>(Key::Scan::LEFT_SHIFT)] = shift_down;
    d_keysPressed[static_cast<unsigned char>(Key::Scan::RIGHT_SHIFT)] = shift_down;

    d_keysPressed[static_cast<unsigned char>(Key::Scan::LEFT_ALT)] = alt_down;
    d_keysPressed[static_cast<unsigned char>(Key::Scan::RIGHT_ALT)] = alt_down;

    d_keysPressed[static_cast<unsigned char>(Key::Scan::LEFT_CONTROL)] = ctrl_down;
    d_keysPressed[static_cast<unsigned char>(Key::Scan::RIGHT_CONTROL)] = ctrl_down;
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
