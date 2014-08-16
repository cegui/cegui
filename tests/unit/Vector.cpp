/***********************************************************************
 *    created:    5/7/2011
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

#include "CEGUI/Vector.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Vector)

BOOST_AUTO_TEST_CASE(ConstructionAssignment2)
{
    CEGUI::Vector2f a(1, 0);
    CEGUI::Vector2f b(0, 1);
    CEGUI::Vector2f c(2, 2);
    CEGUI::Vector2f d(2, 1);
    CEGUI::Vector2f e(1, 2);

    BOOST_CHECK_EQUAL(a + b, CEGUI::Vector2f(1, 1));
    BOOST_CHECK_EQUAL(a - b, CEGUI::Vector2f(1, -1));

    BOOST_CHECK_EQUAL(a * c, CEGUI::Vector2f(2, 0));
    BOOST_CHECK_EQUAL(b * c, CEGUI::Vector2f(0, 2));
    BOOST_CHECK_EQUAL(c * d, CEGUI::Vector2f(4, 2));
    BOOST_CHECK_EQUAL(c * e, CEGUI::Vector2f(2, 4));

    BOOST_CHECK_EQUAL(a * 2, CEGUI::Vector2f(2, 0));
    BOOST_CHECK_EQUAL(b * 2, CEGUI::Vector2f(0, 2));
}

BOOST_AUTO_TEST_CASE(ConstructionAssignment3)
{
    CEGUI::Vector3f a(1, 0, 0);
    CEGUI::Vector3f b(0, 1, 0);
    CEGUI::Vector3f c(0, 0, 1);

    BOOST_CHECK_EQUAL(a + b, CEGUI::Vector3f(1, 1, 0));
    BOOST_CHECK_EQUAL(a - b, CEGUI::Vector3f(1, -1, 0));

    BOOST_CHECK_EQUAL(a * 2, CEGUI::Vector3f(2, 0, 0));
    BOOST_CHECK_EQUAL(b * 2, CEGUI::Vector3f(0, 2, 0));
    BOOST_CHECK_EQUAL(c * 2, CEGUI::Vector3f(0, 0, 2));

    BOOST_CHECK_EQUAL(CEGUI::Vector3f::one(), CEGUI::Vector3f::one_x() + CEGUI::Vector3f::one_y() + CEGUI::Vector3f::one_z());
}

BOOST_AUTO_TEST_CASE(Constants2)
{
    BOOST_CHECK_EQUAL(CEGUI::Vector2f::zero(), CEGUI::Vector2f(0, 0));
    BOOST_CHECK_EQUAL(CEGUI::Vector2f::one(), CEGUI::Vector2f(1, 1));
    BOOST_CHECK_EQUAL(CEGUI::Vector2f::one_x(), CEGUI::Vector2f(1, 0));
    BOOST_CHECK_EQUAL(CEGUI::Vector2f::one_y(), CEGUI::Vector2f(0, 1));

    BOOST_CHECK_EQUAL(CEGUI::UVector2::zero(), CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
    BOOST_CHECK_EQUAL(CEGUI::UVector2::one(), CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
    BOOST_CHECK_EQUAL(CEGUI::UVector2::one_x(), CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0, 0)));
    BOOST_CHECK_EQUAL(CEGUI::UVector2::one_y(), CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(1, 0)));

    // test if it works with the UDim zero and relative
    BOOST_CHECK_EQUAL(CEGUI::UVector2::zero(), CEGUI::UVector2(CEGUI::UDim::zero(), CEGUI::UDim::zero()));
    BOOST_CHECK_EQUAL(CEGUI::UVector2::one(), CEGUI::UVector2(CEGUI::UDim::relative(), CEGUI::UDim::relative()));
    BOOST_CHECK_EQUAL(CEGUI::UVector2::one_x(), CEGUI::UVector2(CEGUI::UDim::relative(), CEGUI::UDim::zero()));
    BOOST_CHECK_EQUAL(CEGUI::UVector2::one_y(), CEGUI::UVector2(CEGUI::UDim::zero(), CEGUI::UDim::relative()));
}

BOOST_AUTO_TEST_CASE(Constants3)
{
    BOOST_CHECK_EQUAL(CEGUI::Vector3f::zero(), CEGUI::Vector3f(0, 0, 0));
    BOOST_CHECK_EQUAL(CEGUI::Vector3f::one(), CEGUI::Vector3f(1, 1, 1));

    BOOST_CHECK_EQUAL(CEGUI::Vector3f::one_x(), CEGUI::Vector3f(1, 0, 0));
    BOOST_CHECK_EQUAL(CEGUI::Vector3f::one_y(), CEGUI::Vector3f(0, 1, 0));
    BOOST_CHECK_EQUAL(CEGUI::Vector3f::one_z(), CEGUI::Vector3f(0, 0, 1));
}

BOOST_AUTO_TEST_SUITE_END()
