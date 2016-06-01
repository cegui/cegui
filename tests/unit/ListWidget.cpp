/***********************************************************************
 *    created:    Sun May 25 2014
 *    author:     Timotei Dolean <timotei21@gmail.com>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
 *
 *    Permission is hereby granted, free of charge, to any person obtaining
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

#include "CEGUI/widgets/ListWidget.h"
#include "CEGUI/WindowManager.h"

using namespace CEGUI;

struct ListWidgetFixture
{
    ListWidgetFixture()
    {
        view = static_cast<ListWidget*>(
            WindowManager::getSingleton().createWindow("TaharezLook/ListWidget", "lw"));
        view->setWindowRenderer("Core/ListView");
    }

    ListWidget* view;
};

BOOST_FIXTURE_TEST_SUITE(ListWidgetTestSuite, ListWidgetFixture)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(GetFirstSelectedItem_NoSelection_ReturnsNull)
{
    BOOST_REQUIRE_EQUAL(static_cast<StandardItem*>(nullptr), view->getFirstSelectedItem());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(GetFirstSelectedItem_OneItem_ReturnsSelection)
{
    view->addItem("item1");

    view->setIndexSelectionState(static_cast<size_t>(0), true);

    BOOST_REQUIRE_EQUAL(view->getItemAtIndex(0), view->getFirstSelectedItem());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(GetFirstSelectedItem_MultipleSelectedItemsUnordered_ReturnsFirst)
{
    view->addItem("item1");
    view->addItem("item2");
    view->addItem("item3");
    view->setMultiSelectEnabled(true);

    view->setIndexSelectionState(2, true);
    view->setIndexSelectionState(1, true);

    BOOST_REQUIRE_EQUAL(2, view->getSelectedItemsCount());
    BOOST_REQUIRE_EQUAL(view->getItemAtIndex(1), view->getFirstSelectedItem());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(GetNextSelectedItem_OneItem_NullStartItem_ReturnsFirstSelectedItem)
{
    view->addItem("item1");

    view->setIndexSelectionState(static_cast<size_t>(0), true);

    BOOST_REQUIRE_EQUAL(view->getItemAtIndex(0), view->getNextSelectedItem(nullptr));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(GetNextSelectedItem_HappyFlow_ReturnsExpectedItem)
{
    view->addItem("item1");
    view->addItem("item2");
    view->addItem("item3");
    view->setMultiSelectEnabled(true);

    view->setIndexSelectionState(2, true);
    view->setIndexSelectionState(1, true);

    BOOST_REQUIRE_EQUAL(2, view->getSelectedItemsCount());
    BOOST_REQUIRE_EQUAL(view->getItemAtIndex(1),
        view->getNextSelectedItem(view->getItemAtIndex(0)));
    BOOST_REQUIRE_EQUAL(view->getItemAtIndex(2),
        view->getNextSelectedItem(view->getItemAtIndex(1)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(RemoveItem_IsRemoved)
{
    view->addItem("item1");
    view->addItem("item2");

    view->prepareForRender();

    BOOST_REQUIRE_EQUAL(2, view->getItems().size());

    view->removeItem(view->getItemAtIndex(0));
    view->prepareForRender();

    BOOST_REQUIRE_EQUAL(1, view->getItems().size());
    BOOST_REQUIRE_EQUAL("item2", view->getItems().at(0)->d_text);
}


//----------------------------------------------------------------------------//
struct OnListSelectionQueriesFirstSelectedItemTestFixture : public ListWidgetFixture
{
    OnListSelectionQueriesFirstSelectedItemTestFixture()
    {
        view->subscribeEvent(ListWidget::EventSelectionChanged,
            Event::Subscriber(
                &OnListSelectionQueriesFirstSelectedItemTestFixture::onSelectionChanged,
                this));
    }

    bool onSelectionChanged(const EventArgs&)
    {
        d_firstSelectedItem = view->getFirstSelectedItem();
        return true;
    }

    StandardItem* d_firstSelectedItem;
};

BOOST_FIXTURE_TEST_CASE(
    ClearList_ViewWithSelectionChangedHandler_SelectedItemEqualsToNull,
    OnListSelectionQueriesFirstSelectedItemTestFixture)
{
    view->addItem("ichi");
    view->addItem("ni");
    view->addItem("san");

    view->setIndexSelectionState(1, true);

    view->clearList();

    BOOST_REQUIRE_EQUAL(static_cast<StandardItem*>(nullptr), d_firstSelectedItem);
}

BOOST_AUTO_TEST_SUITE_END()
