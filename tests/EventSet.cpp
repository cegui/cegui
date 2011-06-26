/***********************************************************************
 *    filename:   EventSet.cpp
 *    created:    11/6/2011
 *    author:     Martin Preisler
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

#include "CEGUIEventSet.h"
#include "CEGUIGlobalEventSet.h"
#include "CEGUIEventArgs.h"
#include "CEGUIExceptions.h"

#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

BOOST_AUTO_TEST_SUITE(EventSet)

BOOST_AUTO_TEST_CASE(AddingAndRemovingEvents)
{
    CEGUI::EventSet set;
    
    const CEGUI::String eventName("ExplicitlyAddedTestEvent");
    
    set.addEvent(eventName);
    BOOST_CHECK(set.isEventPresent(eventName));
    BOOST_CHECK_THROW(set.addEvent(eventName), CEGUI::AlreadyExistsException);
    set.removeEvent(eventName);
    BOOST_CHECK(!set.isEventPresent(eventName));
    set.addEvent(eventName);
    // at this point, the EventSet should contain just one event with eventName as it's name
}

// we keep setting this value to various things to confirm event subscription
// is working
static int g_GlobalEventValue = 0;

/**
 * \brief Custom EventArgs subclass made just for testing
 */
class TestEventArgs : public CEGUI::EventArgs
{
public:
    TestEventArgs():
    d_targetValue(0)
    {}
    
    int d_targetValue;
};

// CEGUI can subscribe events to 3 things, free functions, functor classes
// or member methods, we will test all 3 of these, mostly to make sure it
// all compiles and is still supported. We do test that these subscribers
// have actually run as well.

bool freeFunctionSubscriber(const CEGUI::EventArgs& args)
{
    g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;
    
    return true;
}

class FunctorSubscriber
{
public:
    bool operator()(const CEGUI::EventArgs& args)
    {
        g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;
        
        return true;
    }
};

class MemberMethodSubscriber
{
public:
    bool memberMethod(const CEGUI::EventArgs& args)
    {
        g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;
        
        return true;
    }
};

BOOST_AUTO_TEST_CASE(Subscribing)
{
    CEGUI::EventSet set;
    
    const CEGUI::String eventName("ExplicitlyAddedTestEvent");
    set.addEvent(eventName);
    // at this point, the EventSet should contain just one event with eventName as it's name
    
    TestEventArgs args;
    
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &freeFunctionSubscriber);
        args.d_targetValue = 1;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 1);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriber());
        args.d_targetValue = 2;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 2);
        connection->disconnect();
    }
    {
        MemberMethodSubscriber instance;
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &MemberMethodSubscriber::memberMethod, &instance);
        args.d_targetValue = 3;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 3);
        connection->disconnect();
    }
}
/*
BOOST_AUTO_TEST_CASE(Performance)
{
    CEGUI::EventSet set;
    
    const CEGUI::String eventName("ExplicitlyAddedTestEvent");
    set.addEvent(eventName);
    // at this point, the EventSet should contain just one event with eventName as it's name
    
    TestEventArgs args;
    // we measure event firing lookup overhead with one event first
    
    {
        boost::timer timer;
        for (unsigned int i = 0; i < 1000000; ++i)
        {
            set.fireEvent(eventName, args);
        }
        
        BOOST_TEST_MESSAGE("Time taken, 1000000x event lookup (1 event): " << timer.elapsed());
    }
    
    // now lets add events to a total of 100 and try again (realistic scenario test)
    for (unsigned int i = 1; i < 100; ++i)
    {
        std::stringstream s;
        s << "Event" << i;
        set.addEvent(s.str());
    }
    
    {
        boost::timer timer;
        for (unsigned int i = 0; i < 1000000; ++i)
        {
            set.fireEvent(eventName, args);
        }
        
        BOOST_TEST_MESSAGE("Time taken, 1000000x event lookup (100 events): " << timer.elapsed());
    }
    
    // now lets add events to a total of 10000 and try again (synthetic test)
    for (unsigned int i = 100; i < 10000; ++i)
    {
        std::stringstream s;
        s << "Event" << i;
        set.addEvent(s.str());
    }
    
    {
        boost::timer timer;
        for (unsigned int i = 0; i < 1000000; ++i)
        {
            set.fireEvent(eventName, args);
        }
        
        BOOST_TEST_MESSAGE("Time taken, 1000000x event lookup (10000 events): " << timer.elapsed());
    }
}
*/
BOOST_AUTO_TEST_SUITE_END()
