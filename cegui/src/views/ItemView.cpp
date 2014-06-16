/***********************************************************************
    filename:   ItemView.cpp
    created:    Sat May 24 2014
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
#include "CEGUI/views/ItemView.h"
#include "CEGUI/ImageManager.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const Colour ItemView::DefaultTextColour = 0xFFFFFFFF;

//----------------------------------------------------------------------------//
ItemView::ItemView(const String& type, const String& name) :
    Window(type, name),
    d_itemModel(0),
    d_textColourRect(ColourRect(DefaultTextColour)),
    d_isDirty(true),
    d_eventChildrenAddedConnection(0),
    d_eventChildrenRemovedConnection(0)
{
    addItemViewProperties();
}

//----------------------------------------------------------------------------//
ItemView::~ItemView()
{
    disconnectModelEvents();
}

//----------------------------------------------------------------------------//
void ItemView::addItemViewProperties()
{
    const String& propertyOrigin = "ItemView";

    CEGUI_DEFINE_PROPERTY(ItemView, Image*,
        "SelectionBrushImage",
        "Property to get/set the selection brush image for the list view. Value should be \"set:[imageset name] image:[image name]\".",
        &ItemView::setSelectionBrushImage, &ItemView::getSelectionBrushImage, 0
        );
}

//----------------------------------------------------------------------------//
void ItemView::setModel(ItemModel* item_model)
{
    if (item_model == d_itemModel)
        return;

    if (d_itemModel != 0)
    {
        disconnectModelEvents();
    }

    d_itemModel = item_model;

    connectToModelEvents(d_itemModel);
    invalidateView(true);
}

//----------------------------------------------------------------------------//
void ItemView::connectToModelEvents(ItemModel* d_itemModel)
{
    d_eventChildrenAddedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenAdded,
        &ItemView::onChildrenAdded, this);
    d_eventChildrenRemovedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenRemoved,
        &ItemView::onChildrenRemoved, this);
    d_eventChildrenDataChangedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenDataChanged,
        &ItemView::onChildrenDataChanged, this);
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenAdded(const EventArgs& args)
{
    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    for (SelectionStatesVector::iterator itor = d_indexSelectionStates.begin();
        itor != d_indexSelectionStates.end(); ++itor)
    {
        ModelIndexSelectionState& state = *itor;

        if (state.d_childId >= model_args.d_startId &&
            d_itemModel->areIndicesEqual(state.d_parentIndex, model_args.d_parentIndex))
        {
            state.d_childId += model_args.d_count;
            state.d_selectedIndex = d_itemModel->makeIndex(state.d_childId, state.d_parentIndex);
        }
    }

    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenRemoved(const EventArgs& args)
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

    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenDataChanged(const EventArgs& args)
{
    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
void ItemView::onPointerPressHold(PointerEventArgs& e)
{
    Window::onPointerPressHold(e);

    if (e.source != PS_Left)
        return;

    ModelIndex index = indexAt(e.position);
    setSelectedItem(index);

    ++e.handled;
}

//----------------------------------------------------------------------------//
void ItemView::disconnectModelEvents()
{
    if (d_eventChildrenAddedConnection != 0)
        d_eventChildrenAddedConnection->disconnect();

    if (d_eventChildrenRemovedConnection != 0)
        d_eventChildrenRemovedConnection->disconnect();

    if (d_eventChildrenDataChangedConnection != 0)
        d_eventChildrenDataChangedConnection->disconnect();
}

//----------------------------------------------------------------------------//
void ItemView::invalidateView(bool recursive)
{
    //TODO: allow invalidation only of certain parts (e.g.: items/indices)
    setIsDirty(true);
    invalidate(recursive);
}

//----------------------------------------------------------------------------//
ItemModel* ItemView::getModel() const
{
    return d_itemModel;
}

//----------------------------------------------------------------------------//
void ItemView::prepareForRender()
{
}

//----------------------------------------------------------------------------//
const ColourRect& ItemView::getTextColourRect() const
{
    return d_textColourRect;
}

//----------------------------------------------------------------------------//
void ItemView::setTextColourRect(const ColourRect& colour_rect)
{
    d_textColourRect = colour_rect;
}

//----------------------------------------------------------------------------//
const std::vector<ModelIndexSelectionState>& ItemView::getIndexSelectionStates() const
{
    return d_indexSelectionStates;
}

//----------------------------------------------------------------------------//
bool ItemView::isDirty() const
{
    return d_isDirty;
}

//----------------------------------------------------------------------------//
void ItemView::setIsDirty(bool value)
{
    d_isDirty = value;
}

//----------------------------------------------------------------------------//
bool ItemView::isIndexSelected(const ModelIndex& index) const
{
    for (SelectionStatesVector::const_iterator
        itor = d_indexSelectionStates.begin();
        itor != d_indexSelectionStates.end(); ++itor)
    {
        if (d_itemModel->areIndicesEqual(index, (*itor).d_selectedIndex))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
bool ItemView::setSelectedItem(const ModelIndex& index)
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
    d_indexSelectionStates.clear();
    d_indexSelectionStates.push_back(selection_state);

    invalidateView(false);

    return true;
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionBrushImage(const String& name)
{
    setSelectionBrushImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionBrushImage(const Image* image)
{
    d_selectionBrush = image;
    invalidateView(false);
}

//----------------------------------------------------------------------------//
const Image* ItemView::getSelectionBrushImage(void) const
{
    return d_selectionBrush;
}

}
