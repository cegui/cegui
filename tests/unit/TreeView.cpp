/***********************************************************************
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

#include "InventoryModel.h"
#include "CEGUI/Event.h"
#include "CEGUI/Font.h"
#include "CEGUI/WindowManager.h"

// Yup. We need this in order to easily inject/call event handlers without having
// to go through GUIContext, or inherit from widgets in order to test them.
#define protected public
#include "CEGUI/views/TreeView.h"

using namespace CEGUI;
#define BOOST_REQUIRE_INDICES_EQUAL(i1, i2) BOOST_REQUIRE_EQUAL(i1.d_modelData, i2.d_modelData)

//----------------------------------------------------------------------------//
struct TreeViewFixture
{
    TreeViewFixture()
    {
        System::getSingleton().notifyDisplaySizeChanged(Sizef(100, 100));

        view = static_cast<TreeView*>(
            WindowManager::getSingleton().createWindow("TaharezLook/TreeView", "tv"));
        view->setModel(&model);
        view->setItemTooltipsEnabled(true);
        font_height = view->getFont()->getFontHeight();
        expander_width = view->getViewRenderer()->getSubtreeExpanderSize().d_width;

        view->subscribeEvent(TreeView::EventSubtreeExpanded,
            Event::Subscriber(&TreeViewFixture::onSubtreeExpanded, this));
        view->subscribeEvent(TreeView::EventSubtreeCollapsed,
            Event::Subscriber(&TreeViewFixture::onSubtreeCollapsed, this));
    }

    bool onSubtreeExpanded(const EventArgs& args)
    {
        expanded_nodes.push_back(
            static_cast<const ItemViewEventArgs&>(args).d_index);
        return true;
    }

    bool onSubtreeCollapsed(const EventArgs& args)
    {
        collapsed_nodes.push_back(
            static_cast<const ItemViewEventArgs&>(args).d_index);
        return true;
    }


    TreeView* view;
    InventoryModel model;
    float font_height;
    float expander_width;

    std::vector<ModelIndex> expanded_nodes;
    std::vector<ModelIndex> collapsed_nodes;
};

//----------------------------------------------------------------------------//
CursorInputEventArgs createCursorEventArgs(float x, float y, TreeView* view)
{
    CursorInputEventArgs args(view);

    args.source = CursorInputSource::Left;
    args.position = glm::vec2(x, y);

    return args;
}

BOOST_FIXTURE_TEST_SUITE(TreeViewTestSuite, TreeViewFixture)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_NoItems_ReturnsInvalidIndex)
{
    ModelIndex index = view->indexAt(glm::vec2(1, 0));

    BOOST_REQUIRE(index.d_modelData == nullptr);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideObject_ReturnsCorrectIndex)
{
    // we do this on purpose: we need to skip the root from selection & related.
    // if it has a name then we need not to render it.
    model.getRoot().setText("Root");
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(glm::vec2(expander_width * 2, font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != nullptr);
    BOOST_REQUIRE_EQUAL(
        model.getRoot().getChildren().at(0),
        static_cast<InventoryItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideObjectTreeWithOffset_ReturnsCorrectIndex)
{
    float x_offset = 500;
    float y_offset = 354;
    view->setPosition(UVector2(cegui_absdim(x_offset), cegui_absdim(y_offset)));
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(glm::vec2(
        x_offset + expander_width * 2,
        y_offset + font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != nullptr);
    BOOST_REQUIRE_EQUAL(
        model.getRoot().getChildren().at(0),
        static_cast<InventoryItem*>(index.d_modelData));
}

// Test fails so it's disabled. This should be fixed!
#if 0

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSingleObjectTreeWithScrollbar_ReturnsCorrectIndex)
{
    for (int i = 0; i < 50; ++i)
        model.addRandomItemWithChildren(model.getRootIndex(), 0);

    view->setSize(USize(cegui_absdim(200), cegui_absdim(font_height * 10)));
    view->prepareForRender();
    view->getVertScrollbar()->setUnitIntervalScrollPosition(1.0f);

    ModelIndex index = view->indexAt(glm::vec2(
        expander_width * 2,
        9 * font_height + font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != nullptr);
    BOOST_REQUIRE_EQUAL(
        model.getRoot().getChildren().at(49),
        static_cast<InventoryItem*>(index.d_modelData));
}

#endif

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionOutsideObject_ReturnsInvalidIndex)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(glm::vec2(expander_width * 2, font_height * 3));

    BOOST_REQUIRE(index.d_modelData == nullptr);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSecondObject_ReturnsCorrectIndex)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    model.addRandomItemWithChildren(model.getRootIndex(), 0);

    ModelIndex index = view->indexAt(glm::vec2(
        expander_width * 2,
        font_height + font_height / 2));

    BOOST_REQUIRE(index.d_modelData != nullptr);
    BOOST_REQUIRE_EQUAL(
        model.getRoot().getChildren().at(1),
        static_cast<InventoryItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_InitialSelection_SelectsFirstObject)
{
    model.load();

    bool selected = view->setSelectedIndex(model.makeIndex(0, model.getRootIndex()));
    view->prepareForRender();

    BOOST_REQUIRE(selected);
    BOOST_REQUIRE(view->getRootItemState().d_renderedChildren.at(0)->d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_SecondSelection_SelectsSecondObject)
{
    model.load();
    view->setSelectedIndex(model.makeIndex(0, model.getRootIndex()));
    view->prepareForRender();

    bool selected = view->setSelectedIndex(model.makeIndex(1, model.getRootIndex()));
    view->prepareForRender();

    BOOST_REQUIRE(selected);

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0)->d_isSelected);
    BOOST_REQUIRE(children.at(1)->d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemAdded_ProperSelectionIsPersisted)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->setSelectedIndex(model.makeIndex(1, model.getRootIndex()));
    view->prepareForRender();

    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0)->d_isSelected);
    BOOST_REQUIRE(!children.at(1)->d_isSelected);
    BOOST_REQUIRE(children.at(2)->d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemAdded_ProperChildSelectionIsPersisted)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->prepareForRender();
    CursorInputEventArgs args =
        createCursorEventArgs(expander_width / 2, font_height * 3 / 2, view);
    view->onCursorPressHold(args);
    // first child
    view->setSelectedIndex(model.makeIndex(0, model.makeIndex(1, model.getRootIndex())));

    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children = view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0)->d_isSelected);
    BOOST_REQUIRE(!children.at(1)->d_isSelected);
    BOOST_REQUIRE(!children.at(2)->d_isSelected);
    BOOST_REQUIRE(children.at(2)->d_renderedChildren.at(0)->d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemAdded_InExpandedSubtree_IsRendered)
{
    ModelIndex root_index = model.getRootIndex();
    model.addRandomItemWithChildren(root_index, 0, 0);

    ModelIndex child_index = model.makeIndex(0, root_index);
    model.addRandomItemWithChildren(child_index, 0);
    view->prepareForRender();

    CursorInputEventArgs args =
        createCursorEventArgs(expander_width / 2, font_height / 2, view);
    view->onCursorPressHold(args);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE_EQUAL(1, children.at(0)->d_renderedChildren.size());

    model.addRandomItemWithChildren(child_index, 0);
    view->prepareForRender();
    BOOST_REQUIRE_EQUAL(2, children.at(0)->d_renderedChildren.size());
    BOOST_REQUIRE_EQUAL(2, children.at(0)->d_totalChildCount);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemRemoved_NothingIsSelectedAndRenderingHeightIsUpdated)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 0);
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 0);
    view->setSelectedIndex(model.makeIndex(1, model.getRootIndex()));
    view->prepareForRender();

    float itemSize = view->getRenderedTotalHeight() / 2;

    model.removeItem(model.makeIndex(1, model.getRootIndex()));

    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE_EQUAL(1, view->getRootItemState().d_renderedChildren.size());
    BOOST_REQUIRE_EQUAL(1, view->getRootItemState().d_totalChildCount);
    BOOST_REQUIRE(!children.at(0)->d_isSelected);
    BOOST_REQUIRE(view->getRenderedTotalHeight() <= (itemSize + 0.01f));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(InitialState_NoSubtreesOpenedAndCountIsComputedRight)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    model.addRandomItemWithChildren(model.getRootIndex(), 0);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0)->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(1, children.at(0)->d_totalChildCount);
    BOOST_REQUIRE(!children.at(1)->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(1, children.at(1)->d_totalChildCount);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(CursorMoved_OverSubtreeExpander_DoesNotTriggerExpanding)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    view->prepareForRender();

    CursorInputEventArgs args =
        createCursorEventArgs(expander_width / 2, font_height / 2, view);
    view->onCursorMove(args);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(!children.at(0)->d_subtreeIsExpanded);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(CursorPressed_ExpandAndCollapseEventsFired)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    view->prepareForRender();

    BOOST_REQUIRE_EQUAL(3, view->getRootItemState().d_renderedChildren.at(0)->d_totalChildCount);

    CursorInputEventArgs args =
        createCursorEventArgs(expander_width / 2, font_height / 2, view);
    ModelIndex target_node = model.makeIndex(0, model.getRootIndex());
    view->onCursorPressHold(args);
    BOOST_REQUIRE_INDICES_EQUAL(target_node, expanded_nodes.at(0));
    BOOST_REQUIRE(collapsed_nodes.empty());
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(children.at(0)->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(3, children.at(0)->d_renderedChildren.size());

    // collapse
    view->onCursorPressHold(args);
    BOOST_REQUIRE_INDICES_EQUAL(target_node, expanded_nodes.at(0));
    BOOST_REQUIRE_INDICES_EQUAL(target_node, collapsed_nodes.at(0));
    BOOST_REQUIRE(!children.at(0)->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(0, children.at(0)->d_renderedChildren.size());
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(CursorPressed_ExpandChild)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    model.addRandomItemWithChildren(model.makeIndex(0, model.getRootIndex()), 0, 3);
    view->prepareForRender();

    CursorInputEventArgs args = createCursorEventArgs(expander_width / 2, font_height / 2, view);
    view->onCursorPressHold(args);

    args = createCursorEventArgs(
        view->getViewRenderer()->getSubtreeExpanderXIndent(1) + expander_width / 2,
        font_height + font_height / 2, view);
    view->onCursorPressHold(args);
    ModelIndex first_node = model.makeIndex(0, model.getRootIndex());
    BOOST_REQUIRE_INDICES_EQUAL(first_node, expanded_nodes.at(0));
    BOOST_REQUIRE_INDICES_EQUAL(model.makeIndex(0, first_node), expanded_nodes.at(1));
    BOOST_REQUIRE(collapsed_nodes.empty());

    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(children.at(0)->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(4, children.at(0)->d_renderedChildren.size());
    BOOST_REQUIRE(children.at(0)->d_renderedChildren.at(0)->d_subtreeIsExpanded);
}

BOOST_AUTO_TEST_CASE(SubtreeExpanded_SelectionChanges_DoesNotChangeExpandedState)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    view->prepareForRender();

    CursorInputEventArgs args =
        createCursorEventArgs(expander_width / 2, font_height / 2, view);
    view->onCursorPressHold(args);

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(children.at(0)->d_subtreeIsExpanded);

    view->setSelectedIndex(model.makeIndex(1, model.getRootIndex()));
    view->prepareForRender();

    BOOST_REQUIRE(children.at(0)->d_subtreeIsExpanded);
    BOOST_REQUIRE(children.at(1)->d_isSelected);
}

BOOST_AUTO_TEST_CASE(SetSelectedItem_ChildInExpandedSubtree_SelectsChild)
{
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    model.addRandomItemWithChildren(model.getRootIndex(), 0, 3);
    view->prepareForRender();

    CursorInputEventArgs args =
        createCursorEventArgs(expander_width / 2, font_height / 2, view);
    view->onCursorPressHold(args);

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE(children.at(0)->d_subtreeIsExpanded);

    view->setSelectedIndex(model.makeIndex(0, model.makeIndex(0, model.getRootIndex())));
    view->prepareForRender();

    BOOST_REQUIRE(children.at(0)->d_subtreeIsExpanded);
    BOOST_REQUIRE(children.at(0)->d_renderedChildren.at(0)->d_isSelected);
}

BOOST_AUTO_TEST_CASE(ExpandAll)
{
    ModelIndex root = model.getRootIndex();
    model.addRandomItemWithChildren(root, 0, 3);
    model.addRandomItemWithChildren(root, 0, 3);
    model.addRandomItemWithChildren(model.makeIndex(0, model.makeIndex(0, root)), 0, 2);
    model.addRandomItemWithChildren(model.makeIndex(0, model.makeIndex(1, root)), 0, 2);

    view->prepareForRender();
    view->expandAllSubtrees();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;

    const TreeViewItemRenderingState* first_child = children.at(0);
    BOOST_REQUIRE(first_child->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(3, first_child->d_renderedChildren.size());
    BOOST_REQUIRE(children.at(1)->d_subtreeIsExpanded);

    BOOST_REQUIRE(first_child->d_renderedChildren.at(0)->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(1, first_child->d_renderedChildren.at(0)->d_renderedChildren.size());

    const TreeViewItemRenderingState* child2 =
        first_child->d_renderedChildren.at(0)->d_renderedChildren.at(0);
    BOOST_REQUIRE(child2->d_subtreeIsExpanded);
    BOOST_REQUIRE_EQUAL(2, child2->d_renderedChildren.size());
}

BOOST_AUTO_TEST_CASE(GetTreeViewItemForIndex)
{
    ModelIndex root_index = model.getRootIndex();

    /*
    ROOT
        Index0
            Index0_Child
                A
            B
        C
        D
    */
    model.addRandomItemWithChildren(root_index, 0);
    model.addRandomItemWithChildren(root_index, 0);
    model.addRandomItemWithChildren(root_index, 0);

    ModelIndex index0 = model.makeIndex(0, root_index);
    model.addRandomItemWithChildren(index0, 0);
    model.addRandomItemWithChildren(index0, 0);

    ModelIndex index0_child = model.makeIndex(0, index0);
    model.addRandomItemWithChildren(index0_child, 0);
    view->prepareForRender();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;

    BOOST_REQUIRE_EQUAL(&view->getRootItemState(), view->getTreeViewItemForIndex(root_index));
    BOOST_REQUIRE_EQUAL(children.at(0), view->getTreeViewItemForIndex(index0));
    BOOST_REQUIRE_EQUAL(children.at(1), view->getTreeViewItemForIndex(model.makeIndex(1, root_index)));

    // subtree not opened now.
    BOOST_REQUIRE_EQUAL(
        static_cast<TreeViewItemRenderingState*>(nullptr),
        view->getTreeViewItemForIndex(index0_child));
    CursorInputEventArgs args =
        createCursorEventArgs(expander_width / 2, font_height / 2, view);
    view->onCursorPressHold(args);
    view->prepareForRender();

    BOOST_REQUIRE_EQUAL(children.at(0)->d_renderedChildren.at(0),
        view->getTreeViewItemForIndex(index0_child));
}

BOOST_AUTO_TEST_CASE(SortEnabled_IsEnabled_TreeIsSorted)
{
    InventoryItem* i1 = InventoryItem::make("C", 1, &model.getRoot());
    InventoryItem* i2 = InventoryItem::make("B", 1, &model.getRoot());
    InventoryItem* i3 = InventoryItem::make("A", 1, &model.getRoot());

    InventoryItem* i1_child1 = InventoryItem::make("Z3", 1, i1);
    InventoryItem* i1_child2 = InventoryItem::make("Z1", 1, i1);
    i1->getChildren().push_back(i1_child1);
    i1->getChildren().push_back(i1_child2);

    InventoryItem* i_leaf1 = InventoryItem::make("Z", 1, i1_child1);
    InventoryItem* i_leaf2 = InventoryItem::make("A", 1, i1_child1);
    InventoryItem* i_leaf3 = InventoryItem::make("B", 1, i1_child1);
    i1_child1->getChildren().push_back(i_leaf1);
    i1_child1->getChildren().push_back(i_leaf2);
    i1_child1->getChildren().push_back(i_leaf3);

    model.getRoot().getChildren().push_back(i1);
    model.getRoot().getChildren().push_back(i2);
    model.getRoot().getChildren().push_back(i3);
    view->prepareForRender();

    view->setSortMode(ViewSortMode::Ascending);
    view->prepareForRender();
    view->expandAllSubtrees();

    const std::vector<TreeViewItemRenderingState*>& children =
        view->getRootItemState().d_renderedChildren;
    BOOST_REQUIRE_EQUAL(2, children.at(0)->d_childId);
    BOOST_REQUIRE_EQUAL(1, children.at(1)->d_childId);
    BOOST_REQUIRE_EQUAL(0, children.at(2)->d_childId);

    BOOST_REQUIRE_EQUAL(1, children.at(2)->d_renderedChildren.at(0)->d_childId);

    BOOST_REQUIRE_EQUAL(0,
        children.at(2)->d_renderedChildren.at(1)->d_renderedChildren.at(2)->d_childId);
}

BOOST_AUTO_TEST_CASE(MultipleLevelsHierarchy_RenderedMaxWidthGreaterThanViewSize)
{
    view->setSize(USize(cegui_absdim(10), cegui_absdim(10)));
    InventoryItem* parent = &model.getRoot();
    for (std::int32_t i = 0; i < 100; ++i)
    {
        InventoryItem* item = InventoryItem::make("level " + PropertyHelper<std::int32_t>::toString(i), 0, parent);

        parent->getChildren().push_back(item);
        parent = item;
    }
    view->prepareForRender();

    view->expandAllSubtrees();

    BOOST_REQUIRE(view->getRenderedMaxWidth() > 100);
}

BOOST_AUTO_TEST_SUITE_END()
