/***********************************************************************
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Implementation for a view that displays a list of model items.
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
#include "CEGUI/views/ListView.h"
#include "CEGUI/CoordConverter.h"
#include <algorithm>

namespace CEGUI
{
typedef std::vector<ListViewItemRenderingState> ViewItemsVector;

//----------------------------------------------------------------------------//
static bool listViewItemPointerLess(
    const ListViewItemRenderingState* item1,
    const ListViewItemRenderingState* item2)
{
    return *item1 < *item2;
}

//----------------------------------------------------------------------------//
static bool listViewItemPointerGreater(
    const ListViewItemRenderingState* item1,
    const ListViewItemRenderingState* item2)
{
    return *item1 > *item2;
}

//----------------------------------------------------------------------------//
const String ListView::EventNamespace("ListView");
const String ListView::WidgetTypeName("CEGUI/ListView");

//----------------------------------------------------------------------------//
ListViewItemRenderingState::ListViewItemRenderingState(ListView* list_view) :
    d_isSelected(false),
    d_attachedListView(list_view)
{
}

bool ListViewItemRenderingState::operator<(ListViewItemRenderingState const& other) const
{
    return d_attachedListView->getModel()->compareIndices(d_index, other.d_index) < 0;
}

//----------------------------------------------------------------------------//
bool ListViewItemRenderingState::operator>(const ListViewItemRenderingState& other) const
{
    return d_attachedListView->getModel()->compareIndices(d_index, other.d_index) > 0;
}

//----------------------------------------------------------------------------//
ListView::ListView(const String& type, const String& name) :
    ItemView(type, name)
{
}

//----------------------------------------------------------------------------//
ListView::~ListView()
{
}

//----------------------------------------------------------------------------//
void ListView::prepareForRender()
{
    ItemView::prepareForRender();
    if (d_itemModel == nullptr || !isDirty())
        return;

    if (d_needsFullRender)
    {
        d_renderedMaxWidth = d_renderedTotalHeight = 0;
        d_items.clear();
    }

    ModelIndex root_index = d_itemModel->getRootIndex();
    size_t child_count = d_itemModel->getChildCount(root_index);

    for (size_t child = 0; child < child_count; ++child)
    {
        ModelIndex index = d_itemModel->makeIndex(child, root_index);

        if (d_needsFullRender)
        {
            ListViewItemRenderingState state = ListViewItemRenderingState(this);
            updateItem(state, index, d_renderedMaxWidth, d_renderedTotalHeight);
            d_items.push_back(state);
        }
        else
        {
            ListViewItemRenderingState& item = d_items.at(child);
            d_renderedTotalHeight -= item.d_size.d_height;

            updateItem(item, index, d_renderedMaxWidth, d_renderedTotalHeight);
        }
    }

    updateScrollbars();
    setIsDirty(false);
    resortListView();
    d_needsFullRender = false;
}

//----------------------------------------------------------------------------//
ModelIndex ListView::indexAt(const glm::vec2& position)
{
    if (d_itemModel == nullptr)
        return ModelIndex();

    //TODO: add prepareForLayout() as a cheaper operation alternative?
    prepareForRender();

    glm::vec2 window_position = CoordConverter::screenToWindow(*this, position);
    Rectf render_area(getViewRenderer()->getViewRenderArea());

    if (!render_area.isPointInRectf(window_position))
        return ModelIndex();

    float cur_height = render_area.d_min.y - getVertScrollbar()->getScrollPosition();
    //TODO: start only on the visible area
    for (size_t index = 0; index < d_sortedItems.size(); ++index)
    {
        ListViewItemRenderingState* item = d_sortedItems.at(index);
        Sizef size = item->d_size;
        float next_height = cur_height + size.d_height;

        if (window_position.y >= cur_height &&
            window_position.y <= next_height)
        {
            return item->d_index;
        }

        cur_height = next_height;
    }

    return ModelIndex();
}

//----------------------------------------------------------------------------//
const std::vector<ListViewItemRenderingState*>& ListView::getItems() const
{
    return d_sortedItems;
}

//----------------------------------------------------------------------------//
void ListView::resortListView()
{
    d_sortedItems.clear();

    for (ViewItemsVector::iterator itor = d_items.begin();
        itor != d_items.end(); ++itor)
    {
        d_sortedItems.push_back(&(*itor));
    }

    if (d_sortMode == ViewSortMode::NoSorting)
        return;

    sort(d_sortedItems.begin(), d_sortedItems.end(),
        d_sortMode == ViewSortMode::Ascending ? &listViewItemPointerLess : &listViewItemPointerGreater);
}

//----------------------------------------------------------------------------//
void ListView::resortView()
{
    resortListView();
    invalidateView(false);
}

//----------------------------------------------------------------------------//
void ListView::updateItem(ListViewItemRenderingState &item, ModelIndex index,
    float& max_width, float& total_height)
{
    String text = d_itemModel->getData(index);

    RenderedString rendered_string =
        getRenderedStringParser().parse(text, getFont(), &d_textColourRect);
    item.d_string = rendered_string;
    item.d_index = index;
    item.d_text = text;
    item.d_icon = d_itemModel->getData(index, ItemDataRole::Icon);

    item.d_size = Sizef(
        rendered_string.getHorizontalExtent(this),
        rendered_string.getVerticalExtent(this));

    max_width = std::max(item.d_size.d_width, max_width);

    total_height += item.d_size.d_height;

    item.d_isSelected = isIndexSelected(index);
}

//----------------------------------------------------------------------------//
bool ListView::onChildrenAdded(const EventArgs& args)
{
    ItemView::onChildrenAdded(args);
    const ModelEventArgs& margs = static_cast<const ModelEventArgs&>(args);

    if (!d_itemModel->areIndicesEqual(margs.d_parentIndex, d_itemModel->getRootIndex()))
        return true;

    ViewItemsVector items;
    for (size_t i = 0; i < margs.d_count; ++i)
    {
        ListViewItemRenderingState item(this);

        updateItem(item,
            d_itemModel->makeIndex(margs.d_startId + i, margs.d_parentIndex),
            d_renderedMaxWidth, d_renderedTotalHeight);

        items.push_back(item);
    }

    d_items.insert(d_items.begin() + margs.d_startId, items.begin(), items.end());

    //TODO: insert in the right place directly!
    resortListView();
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
bool ListView::onChildrenRemoved(const EventArgs& args)
{
    ItemView::onChildrenRemoved(args);
    const ModelEventArgs& margs = static_cast<const ModelEventArgs&>(args);

    if (!d_itemModel->areIndicesEqual(margs.d_parentIndex, d_itemModel->getRootIndex()))
        return true;

    ViewItemsVector::iterator begin = d_items.begin() + margs.d_startId;
    ViewItemsVector::iterator end = begin + margs.d_count;

    for (ViewItemsVector::iterator itor = begin; itor < end; ++itor)
    {
        d_renderedTotalHeight -= (*itor).d_size.d_height;
    }

    d_items.erase(begin, end);

    resortListView();
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
Rectf ListView::getIndexRect(const ModelIndex& index)
{
    int child_id = d_itemModel->getChildId(index);
    if (child_id == -1)
    {
        return Rectf(0, 0, 0, 0);
    }

    glm::vec2 pos(0, 0);

    for (size_t i = 0; i < static_cast<size_t>(child_id); ++i)
    {
        pos.y += d_items.at(i).d_size.d_height;
    }

    return Rectf(pos, d_items.at(static_cast<size_t>(child_id)).d_size);
}
}
