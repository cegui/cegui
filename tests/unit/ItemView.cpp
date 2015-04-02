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

#include "CEGUI/views/ListView.h"
#include "CEGUI/WindowManager.h"
#include "ItemModelStub.h"

using namespace CEGUI;

//----------------------------------------------------------------------------//
struct ItemViewFixture
{
    ItemViewFixture()
    {
        view = static_cast<ListView*>(WindowManager::getSingleton().createWindow("TaharezLook/ListView", "lv"));
        view->setWindowRenderer("Core/ListView");
        view->setModel(&model);
    }

    ItemModelStub model;
    ItemView* view;
};

BOOST_FIXTURE_TEST_SUITE(ItemViewTestSuite, ItemViewFixture)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_SetsTheModel)
{
    BOOST_CHECK_EQUAL(&model, view->getModel());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_SameModel_DoesNotSetDirtyState)
{
    view->setIsDirty(false);

    view->setModel(&model);

    BOOST_CHECK(!view->isDirty());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_DifferentModel_SetsDirtyState)
{
    ItemModelStub model2;
    view->setIsDirty(false);

    view->setModel(&model2);

    BOOST_CHECK(view->isDirty());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_ModelHasNewChildren_SetsDirtyState)
{
    model.d_items.push_back("item");

    {
        view->setIsDirty(false);
        model.notifyChildrenAdded(model.getRootIndex(), 0, 1);

        BOOST_CHECK(view->isDirty());
    }

    {
        model.notifyChildrenWillBeRemoved(model.getRootIndex(), 0, 1);
        view->setIsDirty(false);
        model.notifyChildrenRemoved(model.getRootIndex(), 0, 1);

        BOOST_CHECK(view->isDirty());
    }
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_DifferentModel_UnhooksPreviousModelEvents)
{
    model.d_items.push_back("item");

    ItemModelStub model2;
    view->setModel(&model2);

    view->setIsDirty(false);
    model.notifyChildrenAdded(model.getRootIndex(), 0, 1);

    BOOST_CHECK(!view->isDirty());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_DifferentModel_RemovesSelection)
{
    model.d_items.push_back("item");
    view->setSelectedIndex(model.makeIndex(0, model.getRootIndex()));

    BOOST_CHECK_EQUAL(1, view->getIndexSelectionStates().size());

    ItemModelStub model2;
    view->setModel(&model2);
    BOOST_REQUIRE_EQUAL(0, view->getIndexSelectionStates().size());
}

BOOST_AUTO_TEST_CASE(ClearSelection)
{
    model.d_items.push_back("item");

    view->setSelectedIndex(model.makeIndex(0, model.getRootIndex()));

    view->clearSelections();

    BOOST_REQUIRE_EQUAL(0, view->getIndexSelectionStates().size());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_ReplacesSelection)
{
    model.d_items.push_back("item1");
    model.d_items.push_back("item2");
    view->setModel(&model);

    view->setSelectedIndex(model.makeIndex(0, model.getRootIndex()));
    BOOST_CHECK_EQUAL(1, view->getIndexSelectionStates().size());

    view->setSelectedIndex(model.makeIndex(1, model.getRootIndex()));
    BOOST_REQUIRE_EQUAL(1, view->getIndexSelectionStates().size());
    BOOST_REQUIRE_EQUAL("item2",
        *(static_cast<String*>(
            view->getIndexSelectionStates().at(0).d_selectedIndex.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetItemSelectionState_MultiSelectToggled_ModifiesSelectionAccordingly)
{
    model.d_items.push_back("item1");
    model.d_items.push_back("item2");
    view->setModel(&model);
    view->setMultiSelectEnabled(true);

    view->setIndexSelectionState(model.makeIndex(0, model.getRootIndex()), true);
    view->setIndexSelectionState(model.makeIndex(1, model.getRootIndex()), true);

    BOOST_REQUIRE_EQUAL(2, view->getIndexSelectionStates().size());
    BOOST_REQUIRE_EQUAL("item2",
        *(static_cast<String*>(
            view->getIndexSelectionStates().at(1).d_selectedIndex.d_modelData)));

    view->setMultiSelectEnabled(false);
    BOOST_REQUIRE_EQUAL(1, view->getIndexSelectionStates().size());
    BOOST_REQUIRE_EQUAL("item1",
        *(static_cast<String*>(
            view->getIndexSelectionStates().at(0).d_selectedIndex.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(EnsureItemIsVisible_ScrollsHorizontallyAndVertically)
{
    for (int i = 0; i < 10; ++i)
        model.d_items.push_back("1");
    model.d_items.push_back(String(100, 'x'));

    view->setSize(USize(cegui_absdim(10), cegui_absdim(10)));
    view->prepareForRender();

    view->getVertScrollbar()->setScrollPosition(view->getRenderedTotalHeight());
    view->getHorzScrollbar()->setScrollPosition(view->getRenderedMaxWidth());

    view->ensureIndexIsVisible(model.makeIndex(0, model.getRootIndex()));

    BOOST_REQUIRE_CLOSE(0.0f, view->getVertScrollbar()->getScrollPosition(), 1.0f);
    BOOST_REQUIRE_CLOSE(0.0f, view->getHorzScrollbar()->getScrollPosition(), 1.0f);
}

BOOST_AUTO_TEST_SUITE_END()
