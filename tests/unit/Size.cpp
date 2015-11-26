/***********************************************************************
 *    created:    19/08/2012
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

#include "CEGUI/Size.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Size)

BOOST_AUTO_TEST_CASE(ConstructionAssignment)
{
    CEGUI::Sizef a(0, 0);
    CEGUI::Sizef b(0, 0);

    BOOST_CHECK_EQUAL(a, b);
    BOOST_CHECK_EQUAL(a * 1, b);
    BOOST_CHECK_EQUAL(a * 0, b);
}

BOOST_AUTO_TEST_CASE(Clamping)
{
    CEGUI::Sizef a;

    a = CEGUI::Sizef(1, 1);
    a.clamp(CEGUI::Sizef(1, 1), CEGUI::Sizef(2, 2));
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(1, 1));

    a = CEGUI::Sizef(1, 1);
    a.clamp(CEGUI::Sizef(0, 0), CEGUI::Sizef(1, 1));
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(1, 1));

    a = CEGUI::Sizef(1, 1);
    a.clamp(CEGUI::Sizef(0, 0), CEGUI::Sizef(0.5f, 0.5f));
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(0.5f, 0.5f));

    a = CEGUI::Sizef(1, 1);
    a.clamp(CEGUI::Sizef(2, 2), CEGUI::Sizef(2, 2));
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(2, 2));
}

BOOST_AUTO_TEST_CASE(AspectRatio)
{
    CEGUI::Sizef a;

    a = CEGUI::Sizef(1, 1);
    a.scaleToAspect(CEGUI::AM_IGNORE, 100.0f);
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(1, 1));

    a = CEGUI::Sizef(1, 1);
    a.scaleToAspect(CEGUI::AM_SHRINK, 1.0f / 2.0f);
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(0.5f, 1.0f));

    a = CEGUI::Sizef(1, 1);
    a.scaleToAspect(CEGUI::AM_EXPAND, 1.0f / 2.0f);
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(1.0f, 2.0f));

    a = CEGUI::Sizef(1, 1);
    a.scaleToAspect(CEGUI::AM_SHRINK, 2.0f / 1.0f);
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(1.0f, 0.5f));

    a = CEGUI::Sizef(1, 1);
    a.scaleToAspect(CEGUI::AM_EXPAND, 2.0f / 1.0f);
    BOOST_CHECK_EQUAL(a, CEGUI::Sizef(2.0f, 1.0f));
}

BOOST_AUTO_TEST_SUITE_END()
