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

#include "CEGUI/PropertySet.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(PropertySet)

class TestPropertySet : public CEGUI::PropertySet
{
public:
    TestPropertySet():
        d_memberValue(0)
    {
        defineProperty();
    }

    void setMemberValue(int value)
    {
        d_memberValue = value;
    }

    int getMemberValue() const
    {
        return d_memberValue;
    }

    void defineProperty()
    {
        const CEGUI::String propertyOrigin = "TestPropertySet";

        CEGUI_DEFINE_PROPERTY(TestPropertySet, int, "MemberValue", "", &TestPropertySet::setMemberValue, &TestPropertySet::getMemberValue, 0);
    }

private:
    int d_memberValue;
};

BOOST_AUTO_TEST_CASE(Definition)
{
    TestPropertySet set;

    BOOST_REQUIRE(set.isPropertyPresent("MemberValue"));

    // non-existing retrieval must throw
    BOOST_CHECK_THROW(set.getProperty("NonExistant"), CEGUI::UnknownObjectException);
    // non-existing set must throw
    BOOST_CHECK_THROW(set.setProperty("NonExistant", "Whoo"), CEGUI::UnknownObjectException);
    // duplicate definition must throw
    BOOST_CHECK_THROW(set.defineProperty(), CEGUI::AlreadyExistsException);
}

BOOST_AUTO_TEST_CASE(SettingGetting)
{
    TestPropertySet set;

    BOOST_CHECK(!set.isPropertyPresent("BogusValue"));

    BOOST_REQUIRE(set.isPropertyPresent("MemberValue"));

    // Value retrieval
    BOOST_CHECK_EQUAL(set.getProperty<int>("MemberValue"), 0);
    BOOST_CHECK_EQUAL(set.getProperty<float>("MemberValue"), 0); // this should fallback to string and still work!
    BOOST_CHECK_EQUAL(set.getProperty("MemberValue"), "0");

    // Value setting
    BOOST_CHECK_NO_THROW(set.setProperty<int>("MemberValue", 10));
    BOOST_CHECK_EQUAL(set.getProperty<int>("MemberValue"), 10);
}

BOOST_AUTO_TEST_SUITE_END()
