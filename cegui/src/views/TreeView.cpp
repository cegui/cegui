/***********************************************************************
    filename:   TreeView.cpp
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
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String TreeView::EventNamespace("TreeView");
const String TreeView::WidgetTypeName("CEGUI/TreeView");

//----------------------------------------------------------------------------//
TreeViewItemRenderingState::TreeViewItemRenderingState() :
    d_size(0, 0),
    d_isSelected(false),
    d_childId(0)
{

}

//----------------------------------------------------------------------------//
TreeView::TreeView(const String& type, const String& name) :
    ItemView(type, name)
{

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
void TreeView::prepareForRender()
{
    ItemView::prepareForRender();
    if (d_itemModel == 0 || !isDirty())
        return;

    ModelIndex root_index = d_itemModel->getRootIndex();
    d_renderedMaxWidth = 0;
    d_renderedTotalHeight = 0;

    d_rootItemState = computeRenderingStateForIndex(root_index, true,
        d_renderedMaxWidth, d_renderedTotalHeight);

    updateScrollbars();
    setIsDirty(false);
}

//----------------------------------------------------------------------------//
TreeViewItemRenderingState TreeView::computeRenderingStateForIndex(
    const ModelIndex& index, bool isRoot, float& rendered_max_width,
    float& rendered_total_height)
{
    if (d_itemModel == 0)
        return TreeViewItemRenderingState();

    TreeViewItemRenderingState state;
    if (!isRoot)
    {
        String text = d_itemModel->getData(index);
        RenderedString rendered_string = getRenderedStringParser().parse(
            text, getFont(), &d_textColourRect);
        state.d_string = rendered_string;
        state.d_size = Sizef(
            rendered_string.getHorizontalExtent(this),
            rendered_string.getVerticalExtent(this));

        rendered_max_width = ceguimax(rendered_max_width, state.d_size.d_width);
        rendered_total_height += state.d_size.d_height;

        state.d_isSelected = isIndexSelected(index);
    }

    size_t child_count = d_itemModel->getChildCount(index);
    for (size_t child = 0; child < child_count; ++child)
    {
        ModelIndex child_index = d_itemModel->makeIndex(child, index);
        TreeViewItemRenderingState child_state =
            computeRenderingStateForIndex(child_index, false, rendered_max_width,
                rendered_total_height);
        child_state.d_parentIndex = index;
        child_state.d_childId = child;
        state.d_children.push_back(child_state);
    }

    return state;
}

//----------------------------------------------------------------------------//
ModelIndex TreeView::indexAt(const Vector2f& position)
{
    if (d_itemModel == 0)
        return ModelIndex();

    //TODO: add prepareForLayout() as a cheaper operation alternative?
    prepareForRender();

    Vector2f window_position = CoordConverter::screenToWindow(*this, position);
    Rectf render_area(getViewRenderer()->getViewRenderArea());
    if (!render_area.isPointInRect(window_position))
        return ModelIndex();

    float cur_height = render_area.d_min.d_y - getVertScrollbar()->getScrollPosition();

    return indexAtRecursive(d_rootItemState, cur_height, window_position);
}

//----------------------------------------------------------------------------//
ModelIndex TreeView::indexAtRecursive(TreeViewItemRenderingState& item,
    float& cur_height, const Vector2f& window_position)
{
    float next_height = cur_height + item.d_size.d_height;

    if (window_position.d_y >= cur_height &&
        window_position.d_y <= next_height)
    {
        return ModelIndex(d_itemModel->makeIndex(item.d_childId, item.d_parentIndex));
    }

    cur_height = next_height;

    for (size_t i = 0; i < item.d_children.size(); ++i)
    {
        ModelIndex index = indexAtRecursive(item.d_children.at(i),
            cur_height, window_position);
        if (index.d_modelData != 0)
            return index;
    }

    return ModelIndex();
}

}
