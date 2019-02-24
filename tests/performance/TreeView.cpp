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
#include "CEGUI/views/TreeView.h"
#include "CEGUI/Window.h"

#include <iostream>

using namespace CEGUI;


class TreePerformanceTest : public WindowPerformanceTest<TreeView>
{
public:
    TreePerformanceTest(String windowType, String renderer)
        : WindowPerformanceTest<TreeView>(windowType, renderer)
    {
        d_window->setModel(&d_model);
    }

    virtual void doTest()
    {
        size_t id = 0;
        // 200 items with 50 children each
        for (size_t parent = 0; parent < 200; ++parent, ++id)
        {
            StandardItem* item = new StandardItem(
                PropertyHelper<std::uint32_t>::toString(id));
            d_model.addItem(item);

            ModelIndex index = d_model.getIndexForItem(item);
            for (size_t child = 0; child < 50; ++child, ++id)
            {
                d_model.addItemAtPosition(
                    new StandardItem(PropertyHelper<std::uint32_t>::toString(child)),
                    index, 0);
                d_window->draw();
            }
        }
        d_window->draw();

        d_window->expandAllSubtrees();
        d_window->draw();

        ModelIndex root = d_model.getRootIndex();
        for (size_t i = 0; i < 30; ++i)
        {
            d_model.removeItem(d_model.makeIndex(i, root));
        }
        d_window->draw();

        d_window->setSortMode(VSM_Ascending);
    }

    StandardItemModel d_model;
};

BOOST_AUTO_TEST_SUITE(TreeViewPerformance)

BOOST_AUTO_TEST_CASE(Test)
{
    TreePerformanceTest treeview_performance_test(
        "TaharezLook/TreeView", "Core/TreeView");
    treeview_performance_test.execute();
}

BOOST_AUTO_TEST_SUITE_END()
