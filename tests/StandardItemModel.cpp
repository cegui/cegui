/***********************************************************************
 *    created:    Wed Jul 30 2014
 *    author:     Timotei Dolean <timotei21@gmail.com>
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

#include "CEGUI/CEGUI.h"

using namespace CEGUI;

BOOST_AUTO_TEST_SUITE(StandardItemModelTestSuite)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(Insert_NullPosition_InsertsAtBeginning)
{
    StandardItemModel model;
    model.addItem("i1");

    model.insertItem(new StandardItem("i2"), 0);

    BOOST_REQUIRE_EQUAL(2, model.getChildCount(model.getRootIndex()));
    BOOST_REQUIRE_EQUAL("i2", model.getData(model.makeIndex(0, model.getRootIndex()), IDR_Text));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(Insert_NonNullPosition_InsertsBefore)
{
    StandardItemModel model;
    model.addItem("i1");
    model.addItem("i2");

    model.insertItem(new StandardItem("i3"), model.getItemForIndex(model.makeIndex(1, model.getRootIndex())));

    BOOST_REQUIRE_EQUAL(3, model.getChildCount(model.getRootIndex()));
    BOOST_REQUIRE_EQUAL("i3", model.getData(model.makeIndex(1, model.getRootIndex()), IDR_Text));
}

BOOST_AUTO_TEST_SUITE_END()
