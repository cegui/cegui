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
//----------------------------------------------------------------------------//
const String ListView::EventNamespace("ListView");
const String ListView::WidgetTypeName("CEGUI/ListView");

//----------------------------------------------------------------------------//
BasicRenderedStringParser ListView::d_stringParser;

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
    if (d_itemModel == 0 || !d_renderingState.isDirty())
        return;

    ModelIndex root_index = d_itemModel->getRootIndex();
    size_t child_count = d_itemModel->getChildCount(root_index);

    std::vector<ListViewItemRenderingState> items;

    // TODO: migrate ListboxTextItem colorness
    ColourRect colour_rect(Colour(1, 1, 1));

    for (size_t child = 0; child < child_count; ++child)
    {
        ListViewItemRenderingState item;
        ModelIndex index = d_itemModel->makeIndex(child, root_index);

        String text = d_itemModel->getData(index);

        // TODO: migrate the ListboxTextItem string rendering
        RenderedString rendered_string =
            d_stringParser.parse(text, getFont(), &colour_rect);
        item.d_string = rendered_string;

        item.d_size = Sizef(
            rendered_string.getHorizontalExtent(this),
            rendered_string.getVerticalExtent(this));

        item.d_isSelected = isIndexSelected(index);
        items.push_back(item);
    }

    d_renderingState.setItems(items);
    d_renderingState.setIsDirty(false);
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
    for (index = 0; index < d_renderingState.getItems().size(); ++index)
    {
        Sizef size = d_renderingState.getItems().at(index).d_size;
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

    //TODO: take into account multiple & cumulative selection
    std::vector<ModelIndex> selected_indices;
    selected_indices.push_back(index);
    d_renderingState.setSelectedIndices(selected_indices);

    invalidateView(false);

    return true;
}

//----------------------------------------------------------------------------//
bool ListView::isIndexSelected(const ModelIndex& index) const
{
    const std::vector<ModelIndex>& indices = d_renderingState.getSelectedIndices();
    std::vector<ModelIndex>::const_iterator found_index = std::find(
        indices.begin(),
        indices.end(),
        index);

    return found_index != indices.end();
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
ListViewRenderingState* ListView::getRenderingState()
{
    return &d_renderingState;
}

//----------------------------------------------------------------------------//
const std::vector<ListViewItemRenderingState>& ListViewRenderingState::getItems() const
{
    return d_items;
}

//----------------------------------------------------------------------------//
void ListViewRenderingState::setItems(const std::vector<ListViewItemRenderingState>& val)
{
    d_items = val;
}

//----------------------------------------------------------------------------//
const std::vector<ModelIndex>& ListViewRenderingState::getSelectedIndices() const
{
    return d_selectedIndices;
}

//----------------------------------------------------------------------------//
void ListViewRenderingState::setSelectedIndices(const std::vector<ModelIndex>& val)
{
    d_selectedIndices = val;
}
}
