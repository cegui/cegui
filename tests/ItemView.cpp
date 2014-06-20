/***********************************************************************
 *    filename:   ItemView.cpp
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

#include "CEGUI/CEGUI.h"
#include "ItemModelStub.h"

using namespace CEGUI;

//----------------------------------------------------------------------------//
class TestItemView : public ItemView
{
public:
    TestItemView(const String& type, const String& name) : ItemView(type, name) {}

    //----------------------------------------------------------------------------//
    virtual ModelIndex indexAt(const Vector2f& position)
    {
        return ModelIndex();
    }
};

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_SUITE(ItemViewTestSuite)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_SetsTheModel)
{
    ItemModelStub stub;
    TestItemView testItemView("DefaultWindow", "id01");

    testItemView.setModel(&stub);

    BOOST_CHECK_EQUAL(&stub, testItemView.getModel());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_SameModel_DoesNotSetDirtyState)
{
    ItemModelStub stub;
    TestItemView testItemView("DefaultWindow", "id01");
    testItemView.setModel(&stub);
    testItemView.setIsDirty(false);

    testItemView.setModel(&stub);

    BOOST_CHECK(!testItemView.isDirty());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_DifferentModel_SetsDirtyState)
{
    ItemModelStub stub, stub2;
    TestItemView testItemView("DefaultWindow", "id01");
    testItemView.setModel(&stub);
    testItemView.setIsDirty(false);

    testItemView.setModel(&stub2);

    BOOST_CHECK(testItemView.isDirty());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_ModelHasNewChildren_SetsDirtyState)
{
    ItemModelStub stub;
    TestItemView test_item_view("DefaultWindow", "id01");
    stub.d_items.push_back("item");
    test_item_view.setModel(&stub);

    {
        test_item_view.setIsDirty(false);
        stub.notifyChildrenAdded(stub.getRootIndex(), 0, 1);

        BOOST_CHECK(test_item_view.isDirty());
    }

    {
        test_item_view.setIsDirty(false);
        stub.notifyChildrenRemoved(stub.getRootIndex(), 0, 1);

        BOOST_CHECK(test_item_view.isDirty());
    }
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_DifferentModel_UnhooksPreviousModelEvents)
{
    ItemModelStub stub1, stub2;
    TestItemView test_item_view("DefaultWindow", "id01");
    stub1.d_items.push_back("item");
    test_item_view.setModel(&stub1);

    test_item_view.setModel(&stub2);

    test_item_view.setIsDirty(false);
    stub1.notifyChildrenAdded(stub1.getRootIndex(), 0, 1);

    BOOST_CHECK(!test_item_view.isDirty());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetModel_DifferentModel_RemovesSelection)
{
    ItemModelStub stub1, stub2;
    TestItemView test_item_view("DefaultWindow", "id0");
    stub1.d_items.push_back("item");
    test_item_view.setModel(&stub1);
    test_item_view.setSelectedItem(stub1.makeIndex(0, stub1.getRootIndex()));

    BOOST_CHECK_EQUAL(1, test_item_view.getIndexSelectionStates().size());

    test_item_view.setModel(&stub2);
    BOOST_REQUIRE_EQUAL(0, test_item_view.getIndexSelectionStates().size());
}

BOOST_AUTO_TEST_SUITE_END()
