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
void InventoryModel::load()
{
    InventoryItem prev_matryoshka;
    // matryoshka D to A
    bool has_child = false;
    for (char chr = 'D'; chr >= 'A'; --chr)
    {
        InventoryItem matryoshka = InventoryItem::make("Matryoshka " + chr, 1.0f);

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

    d_inventoryItems.push_back(backpack);

    InventoryItem bow = InventoryItem::make("Bow", 23.451f);
    for (int i = 0; i < 25; ++i)
    {
        InventoryItem arrow = InventoryItem::make(
            "arrow " + PropertyHelper<int>::toString(i), 0.2f);
        bow.d_items.push_back(arrow);
    }
    d_inventoryItems.push_back(bow);

    // generate *many* items :D
    for (int i = 1960; i < 2000; i += 2)
    {
        InventoryItem almanach = InventoryItem::make(
            "Almanach " + PropertyHelper<int>::toString(i), 0.34f);
        d_inventoryItems.push_back(almanach);
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
    // root item
    if (parent_index.d_modelData == 0)
    {
        return makeValidIndex(child, d_inventoryItems);
    }

    InventoryItem* item = static_cast<InventoryItem*>(parent_index.d_modelData);

    return makeValidIndex(child, item->d_items);
}

//----------------------------------------------------------------------------//
CEGUI::ModelIndex InventoryModel::getParentIndex(const ModelIndex& model_index)
{
    return CEGUI::ModelIndex();
}

//----------------------------------------------------------------------------//
CEGUI::ModelIndex InventoryModel::getRootIndex()
{
    return ModelIndex();
}

//----------------------------------------------------------------------------//
size_t InventoryModel::getChildCount(const ModelIndex& model_index)
{
    if (model_index.d_modelData == 0)
        return d_inventoryItems.size();

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
    d_inventoryItems.clear();
}

//----------------------------------------------------------------------------//
void InventoryModel::addItem(InventoryItem& new_item)
{
    d_inventoryItems.insert(d_inventoryItems.begin(), new_item);
}
