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

#include "CEGUI/String.h"

#include <boost/test/unit_test.hpp>

// it's not worth it to test std::string, is it?
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE

BOOST_AUTO_TEST_SUITE(String)

BOOST_AUTO_TEST_CASE(ConstructionAssignment)
{
    CEGUI::String a;

    BOOST_CHECK_EQUAL(a, "");
    BOOST_CHECK_EQUAL(a.length(), 0);
    a = "abc";
    BOOST_CHECK_EQUAL(a, "abc");
    BOOST_CHECK_EQUAL(a.length(), 3);
}

BOOST_AUTO_TEST_CASE(Operators)
{
    CEGUI::String a = "ABC";
    CEGUI::String b = "BCD";

    BOOST_CHECK_EQUAL(a, a);
    BOOST_CHECK_EQUAL(b, b);

    BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
