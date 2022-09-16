/***********************************************************************
    created:    Fri Jun 06 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Implementation of the base class for all item model-based views.
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
#include "CEGUI/views/TreeView.h"
#include "CEGUI/widgets/Scrollbar.h"
#include <algorithm> // sort

//TODO: handle event for up/down and left/right keys (open/close subtree)

namespace CEGUI
{

//----------------------------------------------------------------------------//
static bool treeViewItemPointerLess(
    const TreeViewItemRenderingState* item1,
    const TreeViewItemRenderingState* item2)
{
    return *item1 < *item2;
}

//----------------------------------------------------------------------------//
static bool treeViewItemPointerGreater(
    const TreeViewItemRenderingState* item1,
    const TreeViewItemRenderingState* item2)
{
    return *item1 > *item2;
}

//----------------------------------------------------------------------------//
TreeViewWindowRenderer::TreeViewWindowRenderer(const String& type) :
    ItemViewWindowRenderer(type)
{
}
static const float DefaultSubtreeExpanderMargin = 5.0f;
//----------------------------------------------------------------------------//
const String TreeView::EventNamespace("TreeView");
const String TreeView::WidgetTypeName("CEGUI/TreeView");
const String TreeView::EventSubtreeExpanded("SubtreeExpanded");
const String TreeView::EventSubtreeCollapsed("SubtreeCollapsed");

//----------------------------------------------------------------------------//
TreeViewItemRenderingState::TreeViewItemRenderingState(TreeView* attached_tree_view) :
    d_totalChildCount(0),
    d_size(0, 0),
    d_isSelected(false),
    d_childId(0),
    d_subtreeIsExpanded(false),
    d_nestedLevel(0),
    d_attachedTreeView(attached_tree_view)
{
}

//----------------------------------------------------------------------------//
void TreeViewItemRenderingState::sortChildren()
{
    d_renderedChildren.clear();

    for (auto& item : d_children)
    {
        d_renderedChildren.push_back(&item);
        item.sortChildren();
    }

    if (d_attachedTreeView->getSortMode() == ViewSortMode::NoSorting)
        return;

    sort(d_renderedChildren.begin(), d_renderedChildren.end(),
        d_attachedTreeView->getSortMode() == ViewSortMode::Ascending
        ? &treeViewItemPointerLess : &treeViewItemPointerGreater);
}

//----------------------------------------------------------------------------//
bool TreeViewItemRenderingState::operator<(const TreeViewItemRenderingState& other) const
{
    ItemModel* model = d_attachedTreeView->getModel();

    return model->compareIndices(
        model->makeIndex(d_childId, d_parentIndex),
        model->makeIndex(other.d_childId, other.d_parentIndex)
        ) < 0;
}

//----------------------------------------------------------------------------//
bool TreeViewItemRenderingState::operator>(const TreeViewItemRenderingState& other) const
{
    ItemModel* model = d_attachedTreeView->getModel();

    return model->compareIndices(
        model->makeIndex(d_childId, d_parentIndex),
        model->makeIndex(other.d_childId, other.d_parentIndex)
        ) > 0;
}

//----------------------------------------------------------------------------//
TreeView::TreeView(const String& type, const String& name) :
    ItemView(type, name),
    d_rootItemState(this),
    d_subtreeExpanderMargin(DefaultSubtreeExpanderMargin)
{
    addTreeViewProperties();
}

//----------------------------------------------------------------------------//
void TreeView::addTreeViewProperties()
{
    const String& propertyOrigin = "TreeView";

    CEGUI_DEFINE_PROPERTY(TreeView, float,
        "SubtreeExpanderMargin", "Property to access the margin around the subtree expander. Value is a float. Default is 5.0f",
        &TreeView::setSubtreeExpanderMargin, &TreeView::getSubtreeExpanderMargin,
        DefaultSubtreeExpanderMargin
        )
}

//----------------------------------------------------------------------------//
TreeView::~TreeView()
{
}

//----------------------------------------------------------------------------//
const TreeViewItemRenderingState& TreeView::getRootItemState() const
{
    return d_rootItemState;
}

//----------------------------------------------------------------------------//
float TreeView::getSubtreeExpanderMargin() const
{
    return d_subtreeExpanderMargin;
}

//----------------------------------------------------------------------------//
void TreeView::setSubtreeExpanderMargin(float value)
{
    d_subtreeExpanderMargin = value;
}

//----------------------------------------------------------------------------//
void TreeView::prepareForRender()
{
    ItemView::prepareForRender();
    //TODO: better way of ignoring the null item model? E.g.: warn? Throw an exception?
    if (d_itemModel == nullptr || !isDirty())
        return;

    if (d_needsFullRender)
    {
        ModelIndex root_index = d_itemModel->getRootIndex();
        d_renderedMaxWidth = 0;
        d_renderedTotalHeight = 0;

        d_rootItemState = TreeViewItemRenderingState(this);
        // root item isn't a proper item so it does not have a nested level.
        d_rootItemState.d_nestedLevel = -1;
        d_rootItemState.d_subtreeIsExpanded = true;

        computeRenderedChildrenForItem(d_rootItemState, root_index,
            d_renderedMaxWidth, d_renderedTotalHeight);
    }
    else
    {
        updateRenderingStateForItem(d_rootItemState,
            d_renderedMaxWidth, d_renderedTotalHeight);
    }

    updateScrollbars();
    setIsDirty(false);
    d_needsFullRender = false;
}

//----------------------------------------------------------------------------//
bool TreeView::handleSelection(const glm::vec2& localPos, bool should_select,
    bool is_cumulative, bool is_range)
{
    return handleSelection(
        indexAtWithAction(localPos, &TreeView::handleSelectionAction),
        should_select, is_cumulative, is_range);
}

//----------------------------------------------------------------------------//
TreeViewItemRenderingState TreeView::computeRenderingStateForIndex(
    const ModelIndex& parent_index, size_t child_id, size_t nested_level,
    float& rendered_max_width, float& rendered_total_height)
{
    ModelIndex index = d_itemModel->makeIndex(child_id, parent_index);
    TreeViewItemRenderingState state(this);
    state.d_nestedLevel = static_cast<int>(nested_level);
    state.d_parentIndex = parent_index;
    state.d_childId = child_id;

    fillRenderingState(state, index, rendered_max_width, rendered_total_height);

    computeRenderedChildrenForItem(state, index, rendered_max_width,
        rendered_total_height);

    return state;
}

//----------------------------------------------------------------------------//
void TreeView::computeRenderedChildrenForItem(TreeViewItemRenderingState &item,
    const ModelIndex& index, float& rendered_max_width, float& rendered_total_height)
{
    size_t child_count = d_itemModel->getChildCount(index);
    item.d_totalChildCount = child_count;

    if (!item.d_subtreeIsExpanded)
        return;

    for (size_t child = 0; child < child_count; ++child)
    {
        item.d_children.push_back(
            computeRenderingStateForIndex(index, child, item.d_nestedLevel + 1,
            rendered_max_width, rendered_total_height));
    }

    item.sortChildren();
}

//----------------------------------------------------------------------------//
void TreeView::updateRenderingStateForItem(TreeViewItemRenderingState& item,
    float& rendered_max_width, float& rendered_total_height)
{
    // subtract the previous height
    rendered_total_height -= item.d_size.d_height;

    fillRenderingState(item,
        d_itemModel->makeIndex(item.d_childId, item.d_parentIndex),
        rendered_max_width, rendered_total_height);

    for (auto& child : item.d_children)
    {
        child.d_nestedLevel = item.d_nestedLevel + 1;
        updateRenderingStateForItem(child, rendered_max_width, rendered_total_height);
    }
}

//----------------------------------------------------------------------------//
void TreeView::fillRenderingState(TreeViewItemRenderingState& item,
    const ModelIndex& index, float& rendered_max_width, float& rendered_total_height)
{
    item.d_text = d_itemModel->getData(index);
    item.d_icon = d_itemModel->getData(index, ItemDataRole::Icon);

    // The root item is never rendered in a tree, so we don't waste time and also keep its extents empty
    if (&item != &d_rootItemState)
    {
        item.d_renderedText.renderText(item.d_text, getTextParser(), getEffectiveFont(), DefaultParagraphDirection::LeftToRight);
        item.d_renderedText.setHorizontalFormatting(HorizontalTextFormatting::LeftAligned);
        item.d_renderedText.setWordWrapEnabled(false);
        item.d_renderedText.updateDynamicObjectExtents(this);
        item.d_renderedText.updateFormatting(getPixelSize().d_width);
        item.d_size = item.d_renderedText.getExtents();
    }

    const float indent = getViewRenderer()->getSubtreeExpanderXIndent(item.d_nestedLevel) +
        getViewRenderer()->getSubtreeExpanderSize().d_width;
    rendered_max_width = std::max(rendered_max_width, item.d_size.d_width + indent);
    rendered_total_height += item.d_size.d_height;

    item.d_isSelected = isIndexSelected(index);
}

//----------------------------------------------------------------------------//
ModelIndex TreeView::indexAtLocal(const glm::vec2& localPos)
{
    return indexAtWithAction(localPos, &TreeView::noopAction);
}

//----------------------------------------------------------------------------//
ModelIndex TreeView::indexAtWithAction(const glm::vec2& localPos, TreeViewItemAction action)
{
    if (!d_itemModel)
        return ModelIndex();

    //TODO: add prepareForLayout() as a cheaper operation alternative?
    prepareForRender();

    Rectf render_area(getViewRenderer()->getViewRenderArea());
    if (!render_area.isPointInRectf(localPos))
        return ModelIndex();

    float cur_height = render_area.d_min.y - getVertScrollbar()->getScrollPosition();
    bool handled = false;
    return indexAtRecursive(d_rootItemState, cur_height, localPos, handled, action);
}

//----------------------------------------------------------------------------//
ModelIndex TreeView::indexAtRecursive(TreeViewItemRenderingState& item,
    float& cur_height, const glm::vec2& window_position, bool& handled,
    TreeViewItemAction action)
{
    float next_height = cur_height + item.d_size.d_height;

    if (window_position.y >= cur_height &&
        window_position.y <= next_height)
    {
        handled = true;

        float expander_width = getViewRenderer()->getSubtreeExpanderSize().d_width;
        float base_x = getViewRenderer()->getSubtreeExpanderXIndent(item.d_nestedLevel);
        base_x -= getHorzScrollbar()->getScrollPosition();
        if (window_position.x >= base_x &&
            window_position.x <= base_x + expander_width)
        {
            (this->*action)(item, true);
            return ModelIndex();
        }

        (this->*action)(item, false);
        return ModelIndex(d_itemModel->makeIndex(item.d_childId, item.d_parentIndex));
    }

    cur_height = next_height;

    for (size_t i = 0; i < item.d_renderedChildren.size(); ++i)
    {
        ModelIndex index = indexAtRecursive(*item.d_renderedChildren.at(i),
            cur_height, window_position, handled, action);
        if (handled)
            return index;
    }

    return ModelIndex();
}

//----------------------------------------------------------------------------//
TreeViewWindowRenderer* TreeView::getViewRenderer()
{
    return static_cast<TreeViewWindowRenderer*>(ItemView::getViewRenderer());
}

//----------------------------------------------------------------------------//
void TreeView::toggleSubtree(TreeViewItemRenderingState& item)
{
    if (d_itemModel == nullptr)
        return;

    item.d_subtreeIsExpanded = !item.d_subtreeIsExpanded;

    ItemViewEventArgs args(this,
        d_itemModel->makeIndex(item.d_childId, item.d_parentIndex));

    if (item.d_subtreeIsExpanded)
    {
        computeRenderedChildrenForItem(item,
            d_itemModel->makeIndex(item.d_childId, item.d_parentIndex),
            d_renderedMaxWidth, d_renderedTotalHeight);
        onSubtreeExpanded(args);
    }
    else
    {
        clearItemRenderedChildren(item, d_renderedTotalHeight);
        onSubtreeCollapsed(args);
    }

    updateScrollbars();
    // we need just a simple invalidation. No need to redo the render state
    // as we modified it ourself directly.
    invalidate(false);
}

//----------------------------------------------------------------------------//
void TreeView::clearItemRenderedChildren(TreeViewItemRenderingState& item, float& renderedTotalHeight)
{
    for (auto& child : item.d_children)
    {
        clearItemRenderedChildren(child, renderedTotalHeight);
        d_renderedTotalHeight -= item.d_size.d_height;
    }

    item.d_children.clear();
    item.sortChildren();
}

//----------------------------------------------------------------------------//
void TreeView::handleSelectionAction(TreeViewItemRenderingState& item, bool toggles_expander)
{
    if (!toggles_expander)
        return;

    toggleSubtree(item);
}

//----------------------------------------------------------------------------//
bool TreeView::onChildrenRemoved(const EventArgs& args)
{
    ItemView::onChildrenRemoved(args);

    const ModelEventArgs& margs = static_cast<const ModelEventArgs&>(args);
    TreeViewItemRenderingState* item = getTreeViewItemForIndex(margs.d_parentIndex);

    if (item == nullptr)
        return true;

    item->d_totalChildCount -= margs.d_count;

    if (!item->d_subtreeIsExpanded)
        return true;

    auto begin = item->d_children.begin() + margs.d_startId;
    auto end = begin + margs.d_count;

    // update existing child ids
    for (auto itor = begin; itor != item->d_children.end(); ++itor)
    {
        (*itor).d_childId -= margs.d_count;

        if (itor < end)
            d_renderedTotalHeight -= (*itor).d_size.d_height;
    }

    item->d_children.erase(begin, end);

    item->sortChildren();
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
bool TreeView::onChildrenAdded(const EventArgs& args)
{
    ItemView::onChildrenAdded(args);

    const ModelEventArgs& margs = static_cast<const ModelEventArgs&>(args);
    TreeViewItemRenderingState* item = getTreeViewItemForIndex(margs.d_parentIndex);

    if (item == nullptr)
        return true;

    item->d_totalChildCount += margs.d_count;

    if (!item->d_subtreeIsExpanded)
        return true;

    std::vector<TreeViewItemRenderingState> states;
    for (size_t id = margs.d_startId; id < margs.d_startId + margs.d_count; ++id)
    {
        states.push_back(computeRenderingStateForIndex(margs.d_parentIndex, id,
            item->d_nestedLevel + 1, d_renderedMaxWidth, d_renderedTotalHeight));
    }

    // update existing child ids
    for (auto itor = item->d_children.begin() + margs.d_startId; itor != item->d_children.end(); ++itor)
        (*itor).d_childId += margs.d_count;

    item->d_children.insert(
        item->d_children.begin() + margs.d_startId,
        std::make_move_iterator(states.begin()), std::make_move_iterator(states.end()));

    item->sortChildren();
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
void TreeView::onSubtreeExpanded(ItemViewEventArgs& args)
{
    fireEvent(EventSubtreeExpanded, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void TreeView::onSubtreeCollapsed(ItemViewEventArgs& args)
{
    fireEvent(EventSubtreeCollapsed, args, EventNamespace);
}

//----------------------------------------------------------------------------//
TreeViewItemRenderingState* TreeView::getTreeViewItemForIndex(const ModelIndex& index)
{
    std::vector<int> ids_stack;
    ModelIndex root_index = d_itemModel->getRootIndex();
    ModelIndex temp_index = index;

    // we create a stack of child ids which will allow us to drill back
    // in the right hierarchy.
    do
    {
        int id = d_itemModel->getChildId(temp_index);
        if (id == -1)
            break;

        ids_stack.push_back(id);
        temp_index = d_itemModel->getParentIndex(temp_index);
    } while (d_itemModel->isValidIndex(temp_index) &&
        !d_itemModel->areIndicesEqual(temp_index, root_index));

    TreeViewItemRenderingState* item = &d_rootItemState;
    while(!ids_stack.empty())
    {
        int child_id = ids_stack.back();
        ids_stack.pop_back();

        if (static_cast<size_t>(child_id) >= item->d_children.size())
            return nullptr;

        item = &item->d_children.at(child_id);
    }

    return item;
}

//----------------------------------------------------------------------------//
void TreeView::resortView()
{
    d_rootItemState.sortChildren();
    invalidateView(false);
}

//----------------------------------------------------------------------------//
void TreeView::expandAllSubtrees()
{
    expandSubtreeRecursive(d_rootItemState);
}

//----------------------------------------------------------------------------//
void TreeView::expandSubtreeRecursive(TreeViewItemRenderingState& item)
{
    if (!item.d_subtreeIsExpanded)
        toggleSubtree(item);

    if (item.d_totalChildCount == 0)
        return;

    for (auto& child : item.d_children)
        expandSubtreeRecursive(child);
}

//----------------------------------------------------------------------------//
Rectf TreeView::getIndexRect(const ModelIndex&)
{
    //TODO: implement for tree view. What do we do for indices in closed subtrees?
    throw InvalidRequestException("Not implemented for tree view yet.");
}

}
