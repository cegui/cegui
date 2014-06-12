/***********************************************************************
    filename:   ListView.cpp
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
#include "CEGUI/ImageManager.h"
#include "CEGUI/CoordConverter.h"


namespace CEGUI
{
typedef std::vector<ModelIndexSelectionState> SelectionStatesVector;

//----------------------------------------------------------------------------//
const String ListView::EventNamespace("ListView");
const String ListView::WidgetTypeName("CEGUI/ListView");

//----------------------------------------------------------------------------//
ListViewItemRenderingState::ListViewItemRenderingState() : d_isSelected(false)
{
}

//----------------------------------------------------------------------------//
ListView::ListView(const String& type, const String& name) :
    ItemView(type, name)
{
    addListViewProperties();
}

//----------------------------------------------------------------------------//
ListView::~ListView()
{
}

//----------------------------------------------------------------------------//
void ListView::addListViewProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(ListView, Image*,
        "SelectionBrushImage",
        "Property to get/set the selection brush image for the list view. Value should be \"set:[imageset name] image:[image name]\".",
        &ListView::setSelectionBrushImage, &ListView::getSelectionBrushImage, 0
        );
}

//----------------------------------------------------------------------------//
void ListView::prepareForRender()
{
    if (d_itemModel == 0 || !isDirty())
        return;

    ModelIndex root_index = d_itemModel->getRootIndex();
    size_t child_count = d_itemModel->getChildCount(root_index);

    std::vector<ListViewItemRenderingState> items;

    for (size_t child = 0; child < child_count; ++child)
    {
        ListViewItemRenderingState item;
        ModelIndex index = d_itemModel->makeIndex(child, root_index);

        String text = d_itemModel->getData(index);

        RenderedString rendered_string =
            getRenderedStringParser().parse(text, getFont(), &d_textColourRect);
        item.d_string = rendered_string;

        item.d_size = Sizef(
            rendered_string.getHorizontalExtent(this),
            rendered_string.getVerticalExtent(this));

        item.d_isSelected = isIndexSelected(index);
        items.push_back(item);
    }

    d_items = items;
    setIsDirty(false);
}

//----------------------------------------------------------------------------//
void ListView::onPointerPressHold(PointerEventArgs& e)
{
    Window::onPointerPressHold(e);

    if (e.source != PS_Left)
        return;

    ModelIndex index = indexAt(e.position);
    setSelectedItem(index);

    ++e.handled;
}

//----------------------------------------------------------------------------//
ModelIndex ListView::indexAt(const Vector2f& position)
{
    if (d_itemModel == 0)
        return ModelIndex();

    //TODO: add prepareForLayout() as a cheaper operation alternative?
    prepareForRender();

    Vector2f window_position = CoordConverter::screenToWindow(*this, position);
    size_t index;
    float cur_height = 0;
    for (index = 0; index < d_items.size(); ++index)
    {
        Sizef size = d_items.at(index).d_size;
        float next_height = cur_height + size.d_height;

        if (window_position.d_y >= cur_height &&
            window_position.d_y <= next_height)
        {
            return ModelIndex(d_itemModel->makeIndex(index, d_itemModel->getRootIndex()));
        }

        cur_height = next_height;
    }

    return ModelIndex();
}

//----------------------------------------------------------------------------//
bool ListView::setSelectedItem(const ModelIndex& index)
{
    if (d_itemModel == 0 ||
        !d_itemModel->isValidIndex(index))
        return false;

    if (isIndexSelected(index))
        return true;

    ModelIndexSelectionState selection_state;
    selection_state.d_selectedIndex = index;
    selection_state.d_childId = d_itemModel->getChildId(index);
    selection_state.d_parentIndex = d_itemModel->getParentIndex(index);

    //TODO: take into account multiple & cumulative selection
    SelectionStatesVector selection_states;
    selection_states.push_back(selection_state);
    d_indexSelectionStates = selection_states;

    invalidateView(false);

    return true;
}

//----------------------------------------------------------------------------//
bool ListView::isIndexSelected(const ModelIndex& index) const
{
    for (SelectionStatesVector::const_iterator itor = d_indexSelectionStates.begin();
        itor != d_indexSelectionStates.end(); ++itor)
    {
        if (d_itemModel->areIndicesEqual(index, (*itor).d_selectedIndex))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
void ListView::setSelectionBrushImage(const String& name)
{
    setSelectionBrushImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void ListView::setSelectionBrushImage(const Image* image)
{
    d_selectionBrush = image;
    invalidateView(false);
}

//----------------------------------------------------------------------------//
const Image* ListView::getSelectionBrushImage(void) const
{
    return d_selectionBrush;
}

//----------------------------------------------------------------------------//
const std::vector<ListViewItemRenderingState>& ListView::getItems() const
{
    return d_items;
}

//----------------------------------------------------------------------------//
bool ListView::onChildrenAdded(const EventArgs& args)
{
    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    for (SelectionStatesVector::iterator itor = d_indexSelectionStates.begin();
        itor != d_indexSelectionStates.end(); ++itor)
    {
        ModelIndexSelectionState& state = *itor;

        if (state.d_childId >= model_args.d_startId)
        {
            state.d_childId += model_args.d_count;
            state.d_selectedIndex = d_itemModel->makeIndex(state.d_childId, state.d_parentIndex);
        }
    }

    return ItemView::onChildrenAdded(args);
}

//----------------------------------------------------------------------------//
bool ListView::onChildrenRemoved(const EventArgs& args)
{
    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    SelectionStatesVector::iterator itor = d_indexSelectionStates.begin();
    while (itor != d_indexSelectionStates.end())
    {
        ModelIndexSelectionState& state = *itor;

        if (state.d_childId >= model_args.d_startId &&
            state.d_childId <= model_args.d_startId + model_args.d_count)
        {
            itor = d_indexSelectionStates.erase(itor);
        }
        else
        {
            ++itor;
        }
    }

    return ItemView::onChildrenRemoved(args);
}

}
