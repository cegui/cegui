/***********************************************************************
 *    created:    Fri Sep 19 2014
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
#include "CEGUI/PropertySet.h"
#include <sstream>

static const CEGUI::String PROPERTY_NAME("ExplicitlyAddedTestProperty");

class PropertySetStringPerformanceTest : public PerformanceTest
{
public:
    PropertySetStringPerformanceTest(const CEGUI::String& test_name, CEGUI::PropertySet& set):
        PerformanceTest(test_name),
        d_propertySet(set)
    {}

    virtual void doTest()
    {
        for (unsigned int i = 0; i < 1000000; ++i)
        {
            d_propertySet.setProperty(PROPERTY_NAME, "{ { 1, 0 }, {0.5, 100} }");
        }
    }

    CEGUI::PropertySet& d_propertySet;
};

class PropertySetTypedPerformanceTest : public PerformanceTest
{
public:
    PropertySetTypedPerformanceTest(const CEGUI::String& test_name, CEGUI::PropertySet& set):
        PerformanceTest(test_name),
        d_propertySet(set)
    {}

    virtual void doTest()
    {
        for (unsigned int i = 0; i < 1000000; ++i)
        {
            d_propertySet.setProperty<CEGUI::UVector2>(PROPERTY_NAME, CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.5, 100)));
        }
    }

    CEGUI::PropertySet& d_propertySet;
};

class TestingPropertySet : public CEGUI::PropertySet
{
    public:
        TestingPropertySet()
        {
            const CEGUI::String propertyOrigin("abc");

            CEGUI_DEFINE_PROPERTY(TestingPropertySet, CEGUI::UVector2,
                PROPERTY_NAME, "Doc",
                &TestingPropertySet::setTestingProperty,
                &TestingPropertySet::getTestingProperty,
                CEGUI::UVector2()
            );
        }

        void setTestingProperty(const CEGUI::UVector2& v)
        {
            d_testingProperty = v;
        }

        const CEGUI::UVector2& getTestingProperty() const
        {
            return d_testingProperty;
        }

    private:
        CEGUI::UVector2 d_testingProperty;
};

BOOST_AUTO_TEST_SUITE(PropertySetPerformance)

BOOST_AUTO_TEST_CASE(StringSetTest)
{
    TestingPropertySet set;

    PropertySetStringPerformanceTest test("PropertySet String set test", set);
    test.execute();
}

BOOST_AUTO_TEST_CASE(TypedSetTest)
{
    TestingPropertySet set;

    PropertySetTypedPerformanceTest test("PropertySet typed set test", set);
    test.execute();
}

BOOST_AUTO_TEST_SUITE_END()
