/***********************************************************************
 *    filename:   TreeView.cpp
 *    created:    Fri Jun 13 2014
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
#include "InventoryModel.h"

using namespace CEGUI;

//----------------------------------------------------------------------------//
struct TreeViewFixture
{
    TreeViewFixture()
    {
        view = static_cast<TreeView*>(WindowManager::getSingleton().createWindow("TaharezLook/TreeView", "tv"));
        view->setModel(&model);
        font_height = view->getFont()->getFontHeight();
    }

    TreeView* view;
    InventoryModel model;
    float font_height;
};

BOOST_FIXTURE_TEST_SUITE(TreeViewTestSuite, TreeViewFixture)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_NoItems_ReturnsInvalidIndex)
{
    ModelIndex index = view->indexAt(Vector2f(1, 0));

    BOOST_REQUIRE(index.d_modelData == 0);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideObject_ReturnsCorrectIndex)
{
    // we do this on purpose: we need to skip the root from selection & related.
    // if it has a name then we need not to render it.
    model.getInventoryRoot().d_name = "Root";
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(Vector2f(1, font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(
        model.getInventoryRoot().d_items.at(0),
        static_cast<InventoryItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideObjectTreeWithOffset_ReturnsCorrectIndex)
{
    float x_offset = 500;
    float y_offset = 354;
    view->setPosition(UVector2(cegui_absdim(x_offset), cegui_absdim(y_offset)));
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(Vector2f(
        x_offset + 1,
        y_offset + font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(
        model.getInventoryRoot().d_items.at(0),
        static_cast<InventoryItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSingleObjectTreeWithScrollbar_ReturnsCorrectIndex)
{
    for (int i = 0; i < 50; ++i)
        model.addRandomItemWithChildren(model.getRootIndex(), 0);

    view->setSize(USize(cegui_absdim(200), cegui_absdim(font_height * 10)));
    view->prepareForRender();
    view->getVertScrollbar()->setUnitIntervalScrollPosition(1.0f);

    ModelIndex index = view->indexAt(Vector2f(50, 9 * font_height + font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(
        model.getInventoryRoot().d_items.at(49),
        static_cast<InventoryItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionOutsideObject_ReturnsInvalidIndex)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(Vector2f(1, font_height * 3));

    BOOST_REQUIRE(index.d_modelData == 0);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSecondObject_ReturnsCorrectIndex)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(Vector2f(1, font_height  + font_height / 2));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(
        model.getInventoryRoot().d_items.at(1),
        static_cast<InventoryItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_InitialSelection_SelectsFirstObject)
{
    model.load();

    bool selected = view->setSelectedItem(model.makeIndex(0, model.getRootIndex()));
    view->prepareForRender();

    BOOST_REQUIRE(selected);
    BOOST_REQUIRE(view->getRootItemState().d_renderedChildren.at(0).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_SecondSelection_SelectsSecondObject)
{
    model.load();
    view->setSelectedItem(model.makeIndex(0, model.getRootIndex()));
    view->prepareForRender();

    bool selected = view->setSelectedItem(model.makeIndex(1, model.getRootIndex()));
    view->prepareForRender();

    BOOST_REQUIRE(selected);

    const std::vector<TreeViewItemRenderingState>& children = view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0).d_isSelected);
    BOOST_REQUIRE(children.at(1).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemAdded_ProperSelectionIsPersisted)
{
    model.load();
    view->setSelectedItem(model.makeIndex(1, model.getRootIndex()));
    view->prepareForRender();

    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState>& children = view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0).d_isSelected);
    BOOST_REQUIRE(!children.at(1).d_isSelected);
    BOOST_REQUIRE(children.at(2).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemAdded_ProperChildSelectionIsPersisted)
{
    model.load();
    // first child
    view->setSelectedItem(model.makeIndex(0, model.makeIndex(1, model.getRootIndex())));
    view->prepareForRender();

    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState>& children = view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0).d_isSelected);
    BOOST_REQUIRE(!children.at(1).d_isSelected);
    BOOST_REQUIRE(!children.at(2).d_isSelected);
    BOOST_REQUIRE(children.at(2).d_renderedChildren.at(0).d_isSelected);
    BOOST_REQUIRE(!children.at(3).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemRemoved_NothingIsSelected)
{
    model.load();
    size_t initial_size = model.getInventoryRoot().d_items.size();
    view->setSelectedItem(model.makeIndex(1, model.getRootIndex()));
    view->prepareForRender();

    model.removeItem(model.makeIndex(1, model.getRootIndex()));

    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState>& children = view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE_EQUAL(initial_size - 1, view->getRootItemState().d_renderedChildren.size());
    BOOST_REQUIRE(!children.at(0).d_isSelected);
    BOOST_REQUIRE(!children.at(1).d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(InitialState_NoSubtreesOpenedAndCountIsComputedRight)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState>& children = view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0).d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(1, children.at(0).d_totalChildCount);
    BOOST_REQUIRE(!children.at(1).d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(1, children.at(1).d_totalChildCount);
}


BOOST_AUTO_TEST_SUITE_END()
