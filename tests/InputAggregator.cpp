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
    std::string text;

    MockInputEventReceiver()  {}

    ~MockInputEventReceiver()
    {
        for(HandlersMap::const_iterator itor = d_handlersMap.begin();
            itor != d_handlersMap.end(); ++ itor)
        {
            delete (*itor).second;
        }

        d_handlersMap.clear();
    }

    void injectInputEvent(const InputEvent* event)
    {
        HandlersMap::const_iterator itor = d_handlersMap.find(event->eventType);
        if (itor != d_handlersMap.end())
        {
            (*itor).second->handle(event);
        }
        else
        {
            std::cout << "No event handler for event type: " << event->eventType << std::endl;
        }
    }

    void handleTextEvent(const TextInputEvent* event)
    {
        text += event->character;
    }

    void initializeEventHandlers() 
    {
        d_handlersMap.insert(std::make_pair(TextInputEventType, 
            new InputEventHandlerImpl<TextInputEvent, MockInputEventReceiver>(
                &MockInputEventReceiver::handleTextEvent, this)));
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

    BOOST_CHECK_EQUAL(d_inputEventReceiver->text, "a");
}

BOOST_AUTO_TEST_CASE(TextEventMultipleChars)
{
    d_inputAggregator->injectChar('a');
    d_inputAggregator->injectChar('b');
    d_inputAggregator->injectChar('c');

    BOOST_CHECK_EQUAL(d_inputEventReceiver->text, "abc");
}

BOOST_AUTO_TEST_SUITE_END()
