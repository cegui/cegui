/***********************************************************************
 *    created:    Fri Aug 15 2014
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

#include "PerformanceTest.h"

#include "CEGUI/views/StandardItemModel.h"
#include "CEGUI/views/ListView.h"
#include "CEGUI/Window.h"

#include <iostream>

using namespace CEGUI;

class ListViewPerformanceTest : public BaseListPerformanceTest<ListView>
{
public:
    ListViewPerformanceTest(String windowType, String renderer)
        : BaseListPerformanceTest<ListView>(windowType, renderer)
    {
        d_window->setModel(&d_model);
    }

    void addItems(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            d_model.addItem(PropertyHelper<std::uint32_t>::toString(i));
        }
    }

    void addItems(size_t count, size_t at_position)
    {
        for (size_t i = 0; i < count; ++i)
        {
            d_model.addItemAtPosition(
                new StandardItem(PropertyHelper<std::uint32_t>::toString(i)), at_position + i + 1);
        }
    }

    void deleteFirstItems(size_t count)
    {
        ModelIndex root_index = d_model.getRootIndex();
        for (size_t i = 0; i < count; ++i)
        {
            d_model.removeItem(d_model.makeIndex(0, root_index));
        }
    }

    void deleteLastItems(size_t count)
    {
        ModelIndex root_index = d_model.getRootIndex();
        size_t child_count = d_model.getChildCount(d_model.getRootIndex());

        for (size_t i = 0; i < count; ++i, --child_count)
        {
            d_model.removeItem(d_model.makeIndex(child_count - 1, root_index));
        }
    }

    virtual void clearItems()
    {
        d_model.clear(true);
    }

    virtual void sortItems()
    {
        d_window->setSortMode(VSM_Ascending);
    }

    StandardItemModel d_model;
};

BOOST_AUTO_TEST_SUITE(ListViewPerformance)

BOOST_AUTO_TEST_CASE(Test)
{
    ListViewPerformanceTest listview_test("TaharezLook/ListView", "Core/ListView");
    listview_test.execute();
}

BOOST_AUTO_TEST_SUITE_END()
