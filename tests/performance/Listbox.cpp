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

#include "CEGUI/widgets/Listbox.h"
#include "CEGUI/widgets/ListboxTextItem.h"

class ListBoxPerformanceTest : public BaseListPerformanceTest<CEGUI::Listbox>
{
public:
    ListBoxPerformanceTest(CEGUI::String const& windowType, CEGUI::String const& renderer)
        : BaseListPerformanceTest<CEGUI::Listbox>(windowType, renderer)
    {
    }

    virtual void clearItems()
    {
        d_window->resetList();
    }

    virtual void addItems(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            d_window->addItem(new CEGUI::ListboxTextItem(
                CEGUI::PropertyHelper<CEGUI::uint>::toString(i)));
        }
    }

    virtual void addItems(size_t count, size_t at_position)
    {
        CEGUI::ListboxItem* item_at_pos = d_window->getListboxItemFromIndex(at_position);
        for (size_t i = 0; i < count; ++i)
        {
            d_window->insertItem(new CEGUI::ListboxTextItem(
                CEGUI::PropertyHelper<CEGUI::uint>::toString(i)), item_at_pos);
        }
    }

    virtual void deleteFirstItems(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            d_window->removeItem(d_window->getListboxItemFromIndex(0));
        }
    }

    virtual void deleteLastItems(size_t count)
    {
        size_t child_count = d_window->getItemCount();

        for (size_t i = 0; i < count; ++i, --child_count)
        {
            d_window->removeItem(d_window->getListboxItemFromIndex(child_count - 1));
        }
    }

    virtual void sortItems()
    {
        d_window->setSortingEnabled(false);
        d_window->setSortingEnabled(true);
    }
};

BOOST_AUTO_TEST_SUITE(ListBoxPerformance)

BOOST_AUTO_TEST_CASE(Test)
{
    ListBoxPerformanceTest list_box_performance_test(
        "TaharezLook/Listbox", "Core/Listbox");
    list_box_performance_test.execute();
}

BOOST_AUTO_TEST_SUITE_END()
