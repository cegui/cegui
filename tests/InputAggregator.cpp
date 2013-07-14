/***********************************************************************
 *    filename:   InputAggregator.cpp
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

using namespace CEGUI;

struct InputEventHandler
{ 
    virtual void handle(const InputEvent* event) = 0;
};

template <typename TInput, typename TClass>
struct InputEventHandlerImpl : public InputEventHandler
{
    typedef void(TClass::*HandlerFunctionType)(const TInput*);

    InputEventHandlerImpl(HandlerFunctionType handler_func, TClass* obj)
        : d_handlerFunc(handler_func)
        , d_obj(obj)
    {}

    void handle(const InputEvent* event)
    {
        (d_obj->*d_handlerFunc)((const TInput*)event);
    }

private:
    HandlerFunctionType d_handlerFunc;
    TClass* d_obj;
};

class MockInputEventReceiver : public InputEventReceiver
{
public:
    std::string d_text;
    float d_totalScroll;
    Vector2f d_pointerPosition;
    Vector2f d_pointerTotalDelta;

    MockInputEventReceiver()
        : d_text("")
        , d_pointerPosition(0.0f, 0.0f)
        , d_pointerTotalDelta(0.0f, 0.0f)
        , d_totalScroll(0)
    {}

    ~MockInputEventReceiver()
    {
        for (HandlersMap::const_iterator itor = d_handlersMap.begin();
            itor != d_handlersMap.end(); ++ itor)
        {
            delete (*itor).second;
        }

        d_handlersMap.clear();
    }

    void injectInputEvent(const InputEvent* event)
    {
        HandlersMap::const_iterator itor = d_handlersMap.find(event->d_eventType);
        if (itor != d_handlersMap.end())
        {
            (*itor).second->handle(event);
        }
        else
        {
            std::cout << "No event handler for event type: " << event->d_eventType << std::endl;
        }
    }

    void handleTextEvent(const TextInputEvent* event)
    {
        d_text += event->d_character;
    }

    void handleMovementEvent(const MovementInputEvent* event)
    {
        d_pointerPosition = event->d_position;
        d_pointerTotalDelta += event->d_delta;
    }

    void handleScrollEvent(const ScrollInputEvent* event)
    {
        d_totalScroll += event->d_delta;
    }

    void initializeEventHandlers() 
    {
        d_handlersMap.insert(std::make_pair(TextInputEventType, 
            new InputEventHandlerImpl<TextInputEvent, MockInputEventReceiver>(
                &MockInputEventReceiver::handleTextEvent, this)));

        d_handlersMap.insert(std::make_pair(MovementInputEventType, 
            new InputEventHandlerImpl<MovementInputEvent, MockInputEventReceiver>(
                &MockInputEventReceiver::handleMovementEvent, this)));

        d_handlersMap.insert(std::make_pair(ScrollInputEventType, 
            new InputEventHandlerImpl<ScrollInputEvent, MockInputEventReceiver>(
            &MockInputEventReceiver::handleScrollEvent, this)));
    }

private:
    typedef std::map<int, InputEventHandler*> HandlersMap;
    HandlersMap d_handlersMap;
};

struct InputAggregatorFixture
{
    InputAggregatorFixture()
    {
        d_inputEventReceiver = new MockInputEventReceiver;
        d_inputEventReceiver->initializeEventHandlers();
        d_inputAggregator = new InputAggregator(d_inputEventReceiver);
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

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_text, "a");
}

BOOST_AUTO_TEST_CASE(TextEventMultipleChars)
{
    d_inputAggregator->injectChar('a');
    d_inputAggregator->injectChar('b');
    d_inputAggregator->injectChar('c');

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_text, "abc");
}

BOOST_AUTO_TEST_CASE(MovementEventNoDelta)
{
    d_inputAggregator->injectMouseMove(0, 0);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition, 
        d_inputEventReceiver->d_pointerTotalDelta);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 0);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, 0);
}

BOOST_AUTO_TEST_CASE(MovementEventSingleDelta)
{
    d_inputAggregator->injectMouseMove(0, 0);
    d_inputAggregator->injectMouseMove(3, 5);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition, 
        d_inputEventReceiver->d_pointerTotalDelta);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 3);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, 5);
}

BOOST_AUTO_TEST_CASE(MovementEventMultipleDeltas)
{
    d_inputAggregator->injectMouseMove(0, 0);
    d_inputAggregator->injectMouseMove(3, 5);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition, 
        d_inputEventReceiver->d_pointerTotalDelta);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 3);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, 5);

    d_inputAggregator->injectMouseMove(3, -3);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 6);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, 2);
}

BOOST_AUTO_TEST_CASE(MovementEventZeroPosition)
{
	d_inputAggregator->injectMousePosition(0, 0);

	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition, 
		d_inputEventReceiver->d_pointerTotalDelta);

	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 0);
	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, 0);
}

BOOST_AUTO_TEST_CASE(MovementEventNonZeroPosition)
{
	d_inputAggregator->injectMousePosition(30, 40);

	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition, 
		d_inputEventReceiver->d_pointerTotalDelta);

	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 30);
	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, 40);
}

BOOST_AUTO_TEST_CASE(MovementEventMultiplePositions)
{
	d_inputAggregator->injectMousePosition(3, 5);

	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition, 
		d_inputEventReceiver->d_pointerTotalDelta);

	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 3);
	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, 5);

	d_inputAggregator->injectMousePosition(3, -3);

	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_x, 3);
	BOOST_CHECK_EQUAL(d_inputEventReceiver->d_pointerPosition.d_y, -3);
}

BOOST_AUTO_TEST_CASE(ScrollEventNoDelta)
{
    d_inputAggregator->injectMouseWheelChange(0);

    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_totalScroll, 0);
}

BOOST_AUTO_TEST_CASE(ScrollEventMultipleDelta)
{
    d_inputAggregator->injectMouseWheelChange(1);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_totalScroll, 1);

    d_inputAggregator->injectMouseWheelChange(3);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_totalScroll, 4);

    d_inputAggregator->injectMouseWheelChange(5);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_totalScroll, 9);

    d_inputAggregator->injectMouseWheelChange(-2);
    BOOST_CHECK_EQUAL(d_inputEventReceiver->d_totalScroll, 7);
}

BOOST_AUTO_TEST_SUITE_END()
