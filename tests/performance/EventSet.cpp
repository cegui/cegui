/***********************************************************************
 *    created:    Sat Aug 16 2014
 *    author:     Martin Preisler
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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

#include "PerformanceTest.h"
#include "CEGUI/EventSet.h"
#include "CEGUI/GlobalEventSet.h"
#include "CEGUI/EventArgs.h"
#include <sstream>

static const CEGUI::String EVENT_NAME("ExplicitlyAddedTestEvent");

class EventSetPerformanceTest : public PerformanceTest
{
public:
    EventSetPerformanceTest(CEGUI::String const& test_name, CEGUI::EventSet& set)
        : PerformanceTest(test_name), d_eventSet(set)
    {
    }

    virtual void doTest()
    {
        CEGUI::EventArgs args;
        for (unsigned int i = 0; i < 1000000; ++i)
        {
            d_eventSet.fireEvent(EVENT_NAME, args);
        }
    }

    CEGUI::EventSet& d_eventSet;
};

BOOST_AUTO_TEST_SUITE(EventSetPerformance)

BOOST_AUTO_TEST_CASE(OneEventTest)
{
    CEGUI::EventSet set;
    set.addEvent(EVENT_NAME);
    // at this point, the EventSet should contain just one event with eventName as it's name

    EventSetPerformanceTest test("1000000x event lookup (1 event)", set);
    test.execute();
}

BOOST_AUTO_TEST_CASE(OneHundredEventsTest)
{
    CEGUI::EventSet set;
    // a total of 1000 events (realistic scenario test)
    for (unsigned int i = 0; i < 100; ++i)
    {
        std::stringstream s;
        s << "Event" << i;
        set.addEvent(s.str());
    }

    EventSetPerformanceTest test("1000000x event lookup (100 events)", set);
    test.execute();
}

BOOST_AUTO_TEST_CASE(TenThousandEventsTest)
{
    CEGUI::EventSet set;
    // a total of 10000 events (synthetic test)
    for (unsigned int i = 0; i < 10000; ++i)
    {
        std::stringstream s;
        s << "Event" << i;
        set.addEvent(s.str());
    }

    EventSetPerformanceTest test("1000000x event lookup (10000 events)", set);
    test.execute();
}
BOOST_AUTO_TEST_SUITE_END()
