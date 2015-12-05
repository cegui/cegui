/***********************************************************************
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

#include "CEGUI/EventSet.h"
#include "CEGUI/GlobalEventSet.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/Exceptions.h"

#include <functional>

#include <boost/test/unit_test.hpp>

//#define CEGUI_TEST_BOOST_BIND
#ifdef CEGUI_TEST_BOOST_BIND
#   include <boost/bind.hpp>
#endif

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

void freeFunctionSubscriberVoid(const CEGUI::EventArgs& args)
{
    g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;
}

bool freeFunctionSubscriberNoArgs()
{
    g_GlobalEventValue = 12;

    return true;
}

void freeFunctionSubscriberVoidNoArgs()
{
    g_GlobalEventValue = 13;
}

bool freeFunctionSubscriberOtherArgs(const CEGUI::EventArgs& args, const int arg)
{
    g_GlobalEventValue = arg;

    return true;
}

void freeFunctionSubscriberFirstOtherArgs(const int arg, const CEGUI::EventArgs& args)
{
    g_GlobalEventValue = arg;
}

bool freeFunctionSubscriberJustOtherArgs(const int arg)
{
    g_GlobalEventValue = arg;

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

class FunctorSubscriberConst
{
public:
    bool operator()(const CEGUI::EventArgs& args) const
    {
        g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;

        return true;
    }
};

class FunctorSubscriberVoid
{
public:
    void operator()(const CEGUI::EventArgs& args)
    {
        g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;
    }
};

class FunctorSubscriberVoidConst
{
public:
    void operator()(const CEGUI::EventArgs& args) const
    {
        g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;
    }
};

class FunctorSubscriberNoArgs
{
public:
    bool operator()()
    {
        g_GlobalEventValue = 24;

        return true;
    }
};

class FunctorSubscriberNoArgsConst
{
public:
    bool operator()() const
    {
        g_GlobalEventValue = 25;

        return true;
    }
};

class FunctorSubscriberVoidNoArgs
{
public:
    void operator()()
    {
        g_GlobalEventValue = 26;
    }
};

class FunctorSubscriberVoidNoArgsConst
{
public:
    void operator()() const
    {
        g_GlobalEventValue = 27;
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

    void memberMethodVoid(const CEGUI::EventArgs& args)
    {
        g_GlobalEventValue = static_cast<const TestEventArgs&>(args).d_targetValue;
    }

    // always sets the value to 32
    bool memberMethodNoArgs()
    {
        g_GlobalEventValue = 32;

        return true;
    }

    // always sets the value to 33
    void memberMethodVoidNoArgs()
    {
        g_GlobalEventValue = 33;
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
        args.d_targetValue = 10;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 10);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &freeFunctionSubscriberVoid);
        args.d_targetValue = 11;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 11);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &freeFunctionSubscriberNoArgs);
        args.d_targetValue = 12;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 12);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &freeFunctionSubscriberVoidNoArgs);
        args.d_targetValue = 13;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 13);
        connection->disconnect();
    }
    // C++11 only!
#   if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, std::bind(&freeFunctionSubscriberOtherArgs, std::placeholders::_1, 14));
        args.d_targetValue = 14;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 14);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, std::bind<void>(&freeFunctionSubscriberFirstOtherArgs, 15, std::placeholders::_1));
        args.d_targetValue = 15;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 15);
        connection->disconnect();
    }
    {
        // TODO: why do we need to tell std::bind about the return type when boost::bind can figure it out?
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, std::bind<bool>(&freeFunctionSubscriberJustOtherArgs, 16));
        args.d_targetValue = 16;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 16);
        connection->disconnect();
    }
#   endif

#   ifdef CEGUI_TEST_BOOST_BIND
    // broken on g++ 5.1.1
#   ifndef __GNUC__
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, boost::bind(&freeFunctionSubscriberOtherArgs, _1, 17));
        args.d_targetValue = 17;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 17);
        connection->disconnect();
    }
#   endif
    // broken on g++ 5.1.1
#   ifndef __GNUC__
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, boost::bind(&freeFunctionSubscriberFirstOtherArgs, 18, _1));
        args.d_targetValue = 18;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 18);
        connection->disconnect();
    }
#   endif
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, boost::bind(&freeFunctionSubscriberJustOtherArgs, 19));
        args.d_targetValue = 19;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 19);
        connection->disconnect();
    }
#   endif

    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriber());
        args.d_targetValue = 20;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 20);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriberConst());
        args.d_targetValue = 21;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 21);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriberVoid());
        args.d_targetValue = 22;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 22);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriberVoidConst());
        args.d_targetValue = 23;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 23);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriberNoArgs());
        args.d_targetValue = 24;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 24);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriberNoArgsConst());
        args.d_targetValue = 25;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 25);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriberVoidNoArgs());
        args.d_targetValue = 26;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 26);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, FunctorSubscriberVoidNoArgsConst());
        args.d_targetValue = 27;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 27);
        connection->disconnect();
    }

    {
        MemberMethodSubscriber instance;
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &MemberMethodSubscriber::memberMethod, &instance);
        args.d_targetValue = 30;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 30);
        connection->disconnect();
    }
    {
        MemberMethodSubscriber instance;
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &MemberMethodSubscriber::memberMethodVoid, &instance);
        args.d_targetValue = 31;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 31);
        connection->disconnect();
    }
    {
        MemberMethodSubscriber instance;
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &MemberMethodSubscriber::memberMethodNoArgs, &instance);
        args.d_targetValue = 32;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 32);
        connection->disconnect();
    }
    {
        MemberMethodSubscriber instance;
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, &MemberMethodSubscriber::memberMethodVoidNoArgs, &instance);
        args.d_targetValue = 33;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 33);
        connection->disconnect();
    }

    // C++11 only!
#   if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, [](const CEGUI::EventArgs&) { g_GlobalEventValue = 40; return true; });
        args.d_targetValue = 40;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 40);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, [](const CEGUI::EventArgs&) { g_GlobalEventValue = 41; });
        args.d_targetValue = 41;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 41);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, []() { g_GlobalEventValue = 42; return true; });
        args.d_targetValue = 42;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 42);
        connection->disconnect();
    }
    {
        CEGUI::Event::Connection connection = set.subscribeEvent(eventName, []() { g_GlobalEventValue = 43; });
        args.d_targetValue = 43;
        set.fireEvent(eventName, args);
        BOOST_CHECK_EQUAL(g_GlobalEventValue, 43);
        connection->disconnect();
    }
#endif
}
BOOST_AUTO_TEST_SUITE_END()
