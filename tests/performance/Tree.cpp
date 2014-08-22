/***********************************************************************
 *    created:    Sat Aug 16 2014
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
#include "PerformanceTest.h"

#include <boost/test/unit_test.hpp>

#include "CEGUI/PropertyHelper.h"
// Easy way to access the tree's items - tree doesn't provide individual access
// to already added items by index :(
#define protected public
#include "CEGUI/widgets/Tree.h"
#include "CEGUI/widgets/TreeItem.h"

class TreePerformanceTest : public WindowPerformanceTest<CEGUI::Tree>
{
public:
    TreePerformanceTest(CEGUI::String const& windowType, CEGUI::String const& renderer)
        : WindowPerformanceTest<CEGUI::Tree>(windowType, renderer)
    {
    }

    virtual void doTest()
    {
        size_t root_items_count = 200;
        size_t id = 0;
        // 200 items with 50 children each
        for (size_t parent = 0; parent < root_items_count; ++parent, ++id)
        {
            CEGUI::TreeItem* item = new CEGUI::TreeItem(
                CEGUI::PropertyHelper<CEGUI::uint>::toString(id));
            d_window->addItem(item);

            for (size_t child = 0; child < 50; ++child, ++id)
            {
                item->addItem(new CEGUI::TreeItem(
                    CEGUI::PropertyHelper<CEGUI::uint>::toString(id)));
                d_window->render();
            }
        }
        d_window->render();

        for (size_t i = 0; i < root_items_count; ++i)
        {
            d_window->d_listItems.at(i)->toggleIsOpen();
        }
        d_window->render();

        for (size_t i = 0; i < 30; ++i)
        {
            d_window->removeItem(d_window->d_listItems.at(i));
        }
        d_window->render();

        d_window->setSortingEnabled(false);
        d_window->setSortingEnabled(true);
    }
};

BOOST_AUTO_TEST_SUITE(TreePerformance)

BOOST_AUTO_TEST_CASE(Test)
{
    TreePerformanceTest tree_performance_test(
        "TaharezLook/Tree", "Core/Tree");
    tree_performance_test.execute();
}

BOOST_AUTO_TEST_SUITE_END()
