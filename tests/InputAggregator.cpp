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

#include "CEGUI/CEGUI.h"

using namespace CEGUI;

class MockInputEventReceiver : public InputEventReceiver
{
public:
    void injectInputEvent(const InputEvent* event)
    {

    }
};

struct InputAggregatorFixture
{
    InputAggregatorFixture()
    {
        d_inputEventReceiver = new MockInputEventReceiver;
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

BOOST_AUTO_TEST_CASE(MovementEvent1)
{
}

BOOST_AUTO_TEST_SUITE_END()
