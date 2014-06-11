/***********************************************************************
    filename:   Model.cpp
    created:    Tue May 27 2014
    author:     Timotei Dolean <timotei21@gmail.com>
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
#include "Model.h"
#include "CEGUI/PropertyHelper.h"
#include <iterator>
#include <algorithm>

using namespace CEGUI;

template<typename T>
static ModelIndex makeValidIndex(size_t id, std::vector<T>& vector)
{
    if (id >= 0 && id < vector.size())
        return ModelIndex(&vector.at(id));

    return ModelIndex();
}

//----------------------------------------------------------------------------//
InventoryItem InventoryItem::make(const CEGUI::String& name, float weight)
{
    InventoryItem item;

    item.d_name = name;
    item.d_weight = weight;

    return item;
}

//----------------------------------------------------------------------------//
bool InventoryItem::operator==(const InventoryItem& other)
{
    if (d_weight != other.d_weight) return false;
    if (d_name != other.d_name) return false;
    if (d_items.size() != other.d_items.size()) return false;

    for (size_t i = 0; i < d_items.size(); ++i)
    {
        if (d_items.at(i) != other.d_items.at(i))
            return false;
    }
    return true;
}

//----------------------------------------------------------------------------//
bool InventoryItem::operator!=(const InventoryItem& other)
{
    return !(*this == other);
}
//----------------------------------------------------------------------------//
void InventoryModel::load()
{
    d_inventoryRoot = InventoryItem::make("Inventory", 0.0f);

    InventoryItem prev_matryoshka;
    // matryoshka D to A
    bool has_child = false;
    for (char chr = 'D'; chr >= 'A'; --chr)
    {
        InventoryItem matryoshka = InventoryItem::make("Matryoshka " + String(1, chr), 1.0f);

        if (has_child)
            matryoshka.d_items.push_back(prev_matryoshka);

        prev_matryoshka = matryoshka;
        has_child = true;
    }

    InventoryItem beans = InventoryItem::make("Beans!", 0.1f);
    InventoryItem beans_can = InventoryItem::make("Beans can", 1.0f);
    beans_can.d_items.push_back(beans);

    InventoryItem backpack = InventoryItem::make("Trip backpack", 2.0f);
    backpack.d_items.push_back(prev_matryoshka);
    backpack.d_items.push_back(beans_can);

    d_inventoryRoot.d_items.push_back(backpack);

    InventoryItem bow = InventoryItem::make("Bow", 23.451f);
    for (int i = 0; i < 25; ++i)
    {
        InventoryItem arrow = InventoryItem::make(
            "arrow " + PropertyHelper<int>::toString(i), 0.2f);
        bow.d_items.push_back(arrow);
    }
    d_inventoryRoot.d_items.push_back(bow);

    // generate *many* items :D
    for (int i = 1960; i < 2000; i += 2)
    {
        InventoryItem almanach = InventoryItem::make(
            "Almanach " + PropertyHelper<int>::toString(i), 0.34f);
        d_inventoryRoot.d_items.push_back(almanach);
    }
}

//----------------------------------------------------------------------------//
bool InventoryModel::isValidIndex(const ModelIndex& model_index) const
{
    //TODO: check if we still have a reference to model_index.userdata?
    return model_index.d_modelData != 0;
}

//----------------------------------------------------------------------------//
CEGUI::ModelIndex InventoryModel::makeIndex(size_t child, const ModelIndex& parent_index)
{
    if (parent_index.d_modelData == 0)
        return ModelIndex();

    InventoryItem* item = static_cast<InventoryItem*>(parent_index.d_modelData);
    return makeValidIndex(child, item->d_items);
}

//----------------------------------------------------------------------------//
CEGUI::ModelIndex InventoryModel::getParentIndex(const ModelIndex& model_index)
{
    if (model_index.d_modelData == &d_inventoryRoot)
        return ModelIndex();

    return getRootIndex();
}

//----------------------------------------------------------------------------//
CEGUI::ModelIndex InventoryModel::getRootIndex()
{
    return ModelIndex(&d_inventoryRoot);
}

//----------------------------------------------------------------------------//
size_t InventoryModel::getChildCount(const ModelIndex& model_index)
{
    if (model_index.d_modelData == 0)
        return d_inventoryRoot.d_items.size();

    return static_cast<InventoryItem*>(model_index.d_modelData)->d_items.size();
}

//----------------------------------------------------------------------------//
CEGUI::String InventoryModel::getData(const ModelIndex& model_index, ItemDataRole role /*= IDR_Text*/)
{
    if (model_index.d_modelData == 0)
        return "";

    InventoryItem* item = static_cast<InventoryItem*>(model_index.d_modelData);
    if (role == CEGUI::IDR_Text)
        return item->d_name;

    return "";
}

//----------------------------------------------------------------------------//
void InventoryModel::clear()
{
    size_t items_count = d_inventoryRoot.d_items.size();
    d_inventoryRoot.d_items.clear();
    notifyChildrenRemoved(getRootIndex(), 0, items_count);
}

//----------------------------------------------------------------------------//
void InventoryModel::addItem(InventoryItem& new_item)
{
    d_inventoryRoot.d_items.insert(d_inventoryRoot.d_items.begin(), new_item);

    //TODO: see how we specify that we added items starting *before* or *after* that start index
    notifyChildrenAdded(getRootIndex(), 0, 1);
}

//----------------------------------------------------------------------------//
bool InventoryModel::areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2)
{
    return index1.d_modelData == index2.d_modelData;
}

//----------------------------------------------------------------------------//
int InventoryModel::getChildId(const ModelIndex& model_index)
{
    ModelIndex parent_index = getParentIndex(model_index);
    InventoryItem* parent_item = static_cast<InventoryItem*>(parent_index.d_modelData);
    InventoryItem* child_item = static_cast<InventoryItem*>(model_index.d_modelData);

    std::vector<InventoryItem>::iterator itor = std::find(
        parent_item->d_items.begin(), parent_item->d_items.end(), *child_item);

    if (itor == parent_item->d_items.end())
        return -1;

    return std::distance(parent_item->d_items.begin(), itor);
}

//----------------------------------------------------------------------------//
void InventoryModel::removeItem(const ModelIndex& index)
{
    ModelIndex parent_index = getParentIndex(index);
    InventoryItem* parent_item = static_cast<InventoryItem*>(parent_index.d_modelData);
    InventoryItem* child_item = static_cast<InventoryItem*>(index.d_modelData);

    std::vector<InventoryItem>::iterator itor = std::find(
        parent_item->d_items.begin(), parent_item->d_items.end(), *child_item);

    if (itor != parent_item->d_items.end())
    {
        size_t child_id = std::distance(parent_item->d_items.begin(), itor);
        parent_item->d_items.erase(itor);
        notifyChildrenRemoved(parent_index, child_id, 1);
    }
}
