/***********************************************************************
 *    created:    10/7/2013
 *    author:     Timotei Dolean <timotei21@gmail.com>
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

#include <boost/test/unit_test.hpp>

#include <vector>
#include <iostream>
#include <map>

#include "CEGUI/CEGUI.h"
#include "CEGUI/PropertyHelper.h"

namespace std
{
ostream& operator<< (ostream& os, const CEGUI::SemanticValue& value)
{
    return os << static_cast<int>(value);
}
}

using namespace CEGUI;

class MockInputEventReceiver : public InputEventReceiver
{
public:
    CEGUI::String d_text;
    float d_totalScroll;
    glm::vec2 d_cursorPosition;
    std::vector<SemanticValue> d_semanticValues;

    MockInputEventReceiver() :
        d_text(""),
        d_totalScroll(0),
        d_cursorPosition(0.0f, 0.0f)
    {}

    ~MockInputEventReceiver()
    {
        for (InputEventTypeHandlerMap::const_iterator itor = d_handlersMap.begin();
            itor != d_handlersMap.end(); ++ itor)
        {
            delete itor->second;
        }

        for (SemanticEventHandlerMap::const_iterator itor = d_semanticEventsHandlersMap.begin();
            itor != d_semanticEventsHandlersMap.end(); ++itor)
        {
            delete itor->second;
        }
    }

    bool injectInputEvent(const InputEvent& event) override
    {
        InputEventTypeHandlerMap::const_iterator itor = d_handlersMap.find(event.d_eventType);
        if (itor != d_handlersMap.end())
        {
            return (*(*itor).second)(event);
        }
        else
        {
            std::cout << "No event handler for event type: " <<
               static_cast<int>(event.d_eventType) << std::endl;
            return false;
        }
    }

    bool handleTextEvent(const TextInputEvent& event)
    {
        d_text += event.d_character;
        return true;
    }

    bool handleMovementEvent(const SemanticInputEvent& event)
    {
        d_cursorPosition = glm::vec2(event.d_payload.array[0],
            event.d_payload.array[1]);
        return true;
    }

    bool handleScrollEvent(const SemanticInputEvent& event)
    {
        d_totalScroll += event.d_payload.single;
        return true;
    }

    bool handleSemanticEvent(const SemanticInputEvent& event)
    {
        SemanticEventHandlerMap::const_iterator itor =
            d_semanticEventsHandlersMap.find(event.d_value);
        if (itor != d_semanticEventsHandlersMap.end())
        {
            return (*(*itor).second)(event);
        }
        else
        {
            d_semanticValues.push_back(event.d_value);
        }

        return true;
    }

    void initializeEventHandlers()
    {
        d_handlersMap.insert(std::make_pair(InputEventType::TextInputEventType,
            new InputEventHandlerSlot<MockInputEventReceiver, TextInputEvent>(
                &MockInputEventReceiver::handleTextEvent, this)));

        d_handlersMap.insert(std::make_pair(InputEventType::SemanticInputEventType,
            new InputEventHandlerSlot<MockInputEventReceiver, SemanticInputEvent>(
                &MockInputEventReceiver::handleSemanticEvent, this)));
    }

    void initializeSemanticEventHandlers()
    {
        d_semanticEventsHandlersMap.insert(std::make_pair(SemanticValue::VerticalScroll,
            new InputEventHandlerSlot<MockInputEventReceiver, SemanticInputEvent>(
                &MockInputEventReceiver::handleScrollEvent, this)));

        d_semanticEventsHandlersMap.insert(std::make_pair(SemanticValue::CursorMove,
            new InputEventHandlerSlot<MockInputEventReceiver, SemanticInputEvent>(
                &MockInputEventReceiver::handleMovementEvent, this)));
    }

private:
    typedef std::map<SemanticValue, SlotFunctorBase<InputEvent>*> SemanticEventHandlerMap;
    SemanticEventHandlerMap d_semanticEventsHandlersMap;
    typedef std::map<InputEventType, SlotFunctorBase<InputEvent>*> InputEventTypeHandlerMap;
    InputEventTypeHandlerMap d_handlersMap;
};

struct InputAggregatorFixture
{
    InputAggregatorFixture()
    {
        d_inputEventReceiver = new MockInputEventReceiver;
        d_inputEventReceiver->initializeEventHandlers();
        d_inputEventReceiver->initializeSemanticEventHandlers();
        d_inputAggregator = new InputAggregator(d_inputEventReceiver);
        d_inputAggregator->initialise();
    }

    ~InputAggregatorFixture()
    {
        delete d_inputEventReceiver;
        delete d_inputAggregator;
    }

    InputAggregator* d_inputAggregator;
    MockInputEventReceiver* d_inputEventReceiver;
};

//----------------------------------------------------------------------------//

BOOST_FIXTURE_TEST_SUITE(InputAggregator, InputAggregatorFixture)

BOOST_AUTO_TEST_CASE(TextEventOneChar)
{
    d_inputAggregator->injectChar('a');

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_text, "a");
}

BOOST_AUTO_TEST_CASE(TextEventMultipleChars)
{
    d_inputAggregator->injectChar('a');
    d_inputAggregator->injectChar('b');
    d_inputAggregator->injectChar('c');

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_text, "abc");
}

BOOST_AUTO_TEST_CASE(MovementEventNoDelta)
{
    d_inputAggregator->injectMouseMove(0, 0);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 0);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, 0);
}

BOOST_AUTO_TEST_CASE(MovementEventSingleDelta)
{
    d_inputAggregator->injectMouseMove(0, 0);
    d_inputAggregator->injectMouseMove(3, 5);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 3);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, 5);
}

BOOST_AUTO_TEST_CASE(MovementEventMultipleDeltas)
{
    d_inputAggregator->injectMouseMove(0, 0);
    d_inputAggregator->injectMouseMove(3, 5);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 3);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, 5);

    d_inputAggregator->injectMouseMove(3, -3);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 6);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, 2);
}

BOOST_AUTO_TEST_CASE(MovementEventZeroPosition)
{
    d_inputAggregator->injectMousePosition(0, 0);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 0);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, 0);
}

BOOST_AUTO_TEST_CASE(MovementEventNonZeroPosition)
{
    d_inputAggregator->injectMousePosition(30, 40);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 30);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, 40);
}

BOOST_AUTO_TEST_CASE(MovementEventMultiplePositions)
{
    d_inputAggregator->injectMousePosition(3, 5);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 3);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, 5);

    d_inputAggregator->injectMousePosition(3, -3);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.x, 3);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_cursorPosition.y, -3);
}

BOOST_AUTO_TEST_CASE(ScrollEventNoDelta)
{
    d_inputAggregator->injectMouseWheelChange(0);

    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_totalScroll, 0);
}

BOOST_AUTO_TEST_CASE(ScrollEventMultipleDelta)
{
    d_inputAggregator->injectMouseWheelChange(1);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_totalScroll, 1);

    d_inputAggregator->injectMouseWheelChange(3);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_totalScroll, 4);

    d_inputAggregator->injectMouseWheelChange(5);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_totalScroll, 9);

    d_inputAggregator->injectMouseWheelChange(-2);
    BOOST_REQUIRE_EQUAL(d_inputEventReceiver->d_totalScroll, 7);
}

BOOST_AUTO_TEST_CASE(CutRequestToCut)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::Cut);

    d_inputAggregator->injectCutRequest();

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(CopyRequestToCopy)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::Copy);

    d_inputAggregator->injectCopyRequest();

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(PasteRequestToPaste)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::Paste);

    d_inputAggregator->injectPasteRequest();

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseButtonDownToCursorHold)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::CursorPressHold);

    d_inputAggregator->injectMouseButtonDown(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseButtonDownAndControlToSelectCumulative)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::SelectCumulative);

    d_inputAggregator->injectKeyDown(Key::Scan::LeftControl);
    d_inputAggregator->injectMouseButtonDown(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseButtonDownAndShiftToSelectMultiple)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::SelectRange);

    d_inputAggregator->injectKeyDown(Key::Scan::LeftShift);
    d_inputAggregator->injectMouseButtonDown(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseButtonUpToCursorActivate)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::CursorActivate);

    d_inputAggregator->injectMouseButtonUp(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseButtonDownAndUpCombined)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::CursorPressHold);
    expected_values.push_back(SemanticValue::CursorActivate);

    d_inputAggregator->injectMouseButtonDown(MouseButton::Left);
    d_inputAggregator->injectMouseButtonUp(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseClickToCursorActivate)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::CursorActivate);

    d_inputAggregator->injectMouseButtonClick(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseClickAndControlToSelectCumulative)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::SelectCumulative);

    d_inputAggregator->injectKeyDown(Key::Scan::LeftControl);
    d_inputAggregator->injectMouseButtonClick(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseClickAndShiftToSelectMultipleItems)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::CursorActivate);

    d_inputAggregator->injectKeyDown(Key::Scan::LeftShift);
    d_inputAggregator->injectMouseButtonClick(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseDoubleClickToSelectWord)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::SelectWord);

    d_inputAggregator->injectMouseButtonDoubleClick(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(MouseTripleClickToSelectAll)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::SelectAll);

    d_inputAggregator->injectMouseButtonTripleClick(MouseButton::Left);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(KeyDownBackspaceToDeletePreviousCharacter)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::DeletePreviousCharacter);

    d_inputAggregator->injectKeyDown(Key::Scan::Backspace);
    d_inputAggregator->injectKeyUp(Key::Scan::Backspace);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(KeyDownDeleteToDeleteNextCharacter)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::DeleteNextCharacter);

    d_inputAggregator->injectKeyDown(Key::Scan::DeleteKey);
    d_inputAggregator->injectKeyUp(Key::Scan::DeleteKey);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(KeyDownLeftArrowToGoToPreviousCharacter)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::GoToPreviousCharacter);

    d_inputAggregator->injectKeyDown(Key::Scan::ArrowLeft);
    d_inputAggregator->injectKeyUp(Key::Scan::ArrowLeft);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(KeyDownLeftArrowAndShiftToSelectPreviousCharacter)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::SelectPreviousCharacter);

    d_inputAggregator->injectKeyDown(Key::Scan::LeftShift);
    d_inputAggregator->injectKeyDown(Key::Scan::ArrowLeft);
    d_inputAggregator->injectKeyUp(Key::Scan::ArrowLeft);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_CASE(KeyDownLeftArrowAndControlToGoToPreviousWord)
{
    std::vector<SemanticValue> expected_values;
    expected_values.push_back(SemanticValue::GoToPreviousWord);

    d_inputAggregator->injectKeyDown(Key::Scan::LeftControl);
    d_inputAggregator->injectKeyDown(Key::Scan::ArrowLeft);
    d_inputAggregator->injectKeyUp(Key::Scan::ArrowLeft);

    BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_values.begin(), expected_values.end(),
        d_inputEventReceiver->d_semanticValues.begin(),
        d_inputEventReceiver->d_semanticValues.end());
}

BOOST_AUTO_TEST_SUITE_END()
