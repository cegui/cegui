/***********************************************************************
 *    filename:   ListView.cpp
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
static const String ITEM1 = "ITEM 1";
static const String ITEM2 = "ITEM 2";
static const String ITEM3 = "ITEM 3";

//----------------------------------------------------------------------------//
struct ListViewFixture
{
    ListViewFixture() : view("TaharezLook/ListView", "lv")
    {
        view.setModel(&model);
    }

    ListView view;
    ItemModelStub model;

};

BOOST_FIXTURE_TEST_SUITE(ListViewTestSuite, ListViewFixture)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_NoItems_ReturnsInvalidIndex)
{
    ModelIndex index = view.indexAt(Vector2f(0, 0));

    BOOST_REQUIRE(index.d_modelData == 0);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSingleObject_ReturnsCorrectIndex)
{
    model.d_items.push_back(ITEM1);

    ModelIndex index = view.indexAt(Vector2f(0, view.getFont()->getFontHeight() / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(ITEM1, *(static_cast<String*>(index.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSingleObjectListWithOffset_ReturnsCorrectIndex)
{
    float x_offset = 500;
    float y_offset = 354;
    view.setPosition(UVector2(cegui_absdim(x_offset), cegui_absdim(y_offset)));
    model.d_items.push_back(ITEM1);

    ModelIndex index = view.indexAt(Vector2f(
        x_offset + 0,
        y_offset + view.getFont()->getFontHeight() / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(ITEM1, *(static_cast<String*>(index.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionOutsideSingleObject_ReturnsInvalidIndex)
{
    model.d_items.push_back(ITEM1);

    ModelIndex index = view.indexAt(Vector2f(0, view.getFont()->getFontHeight() * 2));

    BOOST_REQUIRE(index.d_modelData == 0);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSecondObject_ReturnsCorrectIndex)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);

    ModelIndex index = view.indexAt(Vector2f(0, view.getFont()->getFontHeight() * 2));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(ITEM2, *(static_cast<String*>(index.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_InitialSelection_SelectsFirstObject)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);

    bool selected = view.setSelectedItem(ModelIndex(&model.d_items.at(0)));
    view.prepareForRender();

    BOOST_REQUIRE(selected);
    BOOST_REQUIRE(view.getRenderingState()->getItems().at(0).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_SecondSelection_SelectsSecondObject)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);
    view.setSelectedItem(ModelIndex(&model.d_items.at(0)));
    view.prepareForRender();

    bool selected = view.setSelectedItem(ModelIndex(&model.d_items.at(1)));
    view.prepareForRender();

    BOOST_REQUIRE(selected);
    BOOST_REQUIRE(!view.getRenderingState()->getItems().at(0).d_isSelected);
    BOOST_REQUIRE(view.getRenderingState()->getItems().at(1).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemAdded_ProperSelectionIsPersisted)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);
    view.setSelectedItem(ModelIndex(&model.d_items.at(1)));
    view.prepareForRender();

    model.d_items.insert(model.d_items.begin() + 1, 1, ITEM3);
    model.notifyChildrenAdded(model.getRootIndex(), 1, 1);

    view.prepareForRender();

    BOOST_REQUIRE(!view.getRenderingState()->getItems().at(0).d_isSelected);
    BOOST_REQUIRE(!view.getRenderingState()->getItems().at(1).d_isSelected);
    BOOST_REQUIRE(view.getRenderingState()->getItems().at(2).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemRemoved_NothingIsSelected)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);
    model.d_items.push_back(ITEM3);
    view.setSelectedItem(ModelIndex(&model.d_items.at(1)));
    view.prepareForRender();

    model.d_items.erase(model.d_items.begin() + 1);
    model.notifyChildrenRemoved(model.getRootIndex(), 1, 1);

    view.prepareForRender();

    BOOST_REQUIRE(view.getRenderingState()->getItems().size() == 2);
    BOOST_REQUIRE(!view.getRenderingState()->getItems().at(0).d_isSelected);
    BOOST_REQUIRE(!view.getRenderingState()->getItems().at(1).d_isSelected);
}

BOOST_AUTO_TEST_SUITE_END()
