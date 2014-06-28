/***********************************************************************
 *    created:    01/11/2012
 *    author:     Martin Preisler
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/PropertyHelper.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(PropertyHelper)

BOOST_AUTO_TEST_CASE(Integer)
{
    // --- toString
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<int>::toString(0), "0");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<int>::toString(123), "123");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<int>::toString(-123), "-123");

    // --- fromString
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<int>::fromString("0"), 0);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<int>::fromString("123"), 123);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<int>::fromString("-123"), -123);
}

BOOST_AUTO_TEST_CASE(Float)
{
    // --- toString
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::toString(0), "0");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::toString(123), "123");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::toString(-123), "-123");

    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::toString(123.1f), "123.1");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::toString(-123.1f), "-123.1");

    // --- fromString
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::fromString("0"), 0);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::fromString("123"), 123);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::fromString("-123"), -123);

    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::fromString("123.1"), 123.1f);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<float>::fromString("-123.1"), -123.1f);
}

BOOST_AUTO_TEST_CASE(Double)
{
    // --- toString
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::toString(0), "0");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::toString(123), "123");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::toString(-123), "-123");

    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::toString(123.1), "123.1");
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::toString(-123.1), "-123.1");

    // --- fromString
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::fromString("0"), 0);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::fromString("123"), 123);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::fromString("-123"), -123);

    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::fromString("123.1"), 123.1);
    BOOST_CHECK_EQUAL(CEGUI::PropertyHelper<double>::fromString("-123.1"), -123.1);
}

BOOST_AUTO_TEST_SUITE_END()
