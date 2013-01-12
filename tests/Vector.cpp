/***********************************************************************
 *    filename:   Vector.cpp
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

BOOST_AUTO_TEST_CASE(Constants)
{
    BOOST_CHECK_EQUAL(CEGUI::Vector2f::zero(), CEGUI::Vector2f(0, 0));
    BOOST_CHECK_EQUAL(CEGUI::Vector2f::one(), CEGUI::Vector2f(1, 1));
    
    BOOST_CHECK_EQUAL(CEGUI::UVector2::zero(), CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
    BOOST_CHECK_EQUAL(CEGUI::UVector2::one(), CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
}

BOOST_AUTO_TEST_SUITE_END()
