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

#include "CEGUI/WindowManager.h"
#include "CEGUI/widgets/ItemListbox.h"

class ItemListboxPerformanceTest : public BaseListPerformanceTest<CEGUI::ItemListbox>
{
public:
    ItemListboxPerformanceTest(CEGUI::String const& windowType, CEGUI::String const& renderer)
        : BaseListPerformanceTest<CEGUI::ItemListbox>(windowType, renderer)
    {
    }

    virtual void clearItems()
    {
        d_window->resetList();
    }

    CEGUI::ItemEntry* createItem(CEGUI::String id)
    {
        CEGUI::ItemEntry* item = static_cast<CEGUI::ItemEntry*>(
            CEGUI::WindowManager::getSingleton().createWindow(
            "TaharezLook/ListboxItem", id));
        item->setText(id);
        return item;
    }

    virtual void addItems(size_t count)
    {
        // prevent adding already existing items.
        size_t offset = d_window->getItemCount();
        for (size_t i = 0; i < count; ++i)
        {
            d_window->addItem(createItem(
                CEGUI::PropertyHelper<CEGUI::uint>::toString(offset + i)));
        }
    }

    virtual void addItems(size_t count, size_t at_position)
    {
        // prevent adding already existing items.
        size_t offset = d_window->getItemCount();
        const CEGUI::ItemEntry* item_at_pos = d_window->getItemFromIndex(at_position);
        for (size_t i = 0; i < count; ++i)
        {
            d_window->insertItem(
                createItem(CEGUI::PropertyHelper<CEGUI::uint>::toString(offset + i)),
                item_at_pos);
        }
    }

    virtual void deleteFirstItems(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            d_window->removeItem(d_window->getItemFromIndex(0));
        }
    }

    virtual void deleteLastItems(size_t count)
    {
        size_t child_count = d_window->getItemCount();

        for (size_t i = 0; i < count; ++i, --child_count)
        {
            d_window->removeItem(d_window->getItemFromIndex(child_count - 1));
        }
    }

    virtual void sortItems()
    {
        d_window->setSortMode(CEGUI::ItemListBase::UserSort);
        d_window->setSortMode(CEGUI::ItemListBase::Ascending);
    }
};

BOOST_AUTO_TEST_SUITE(ItemListboxPerformance)

BOOST_AUTO_TEST_CASE(Test)
{
    ItemListboxPerformanceTest item_listbox_performance_test(
        "TaharezLook/ItemListbox", "Core/ItemListbox");
    item_listbox_performance_test.execute();
}

BOOST_AUTO_TEST_SUITE_END()
