/***********************************************************************
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
#include "InventoryModel.h"
#include "CEGUI/PropertyHelper.h"
#include <iterator>
#include <algorithm>

using namespace CEGUI;

//----------------------------------------------------------------------------//
InventoryItem::InventoryItem(String name) : GenericItem(name)
{
}

//----------------------------------------------------------------------------//
InventoryItem* InventoryItem::make(const String& name, float weight, GenericItem* parent)
{
    InventoryItem* item = new InventoryItem(name);

    item->d_weight = weight;
    item->d_parent = parent;

    return item;
}

//----------------------------------------------------------------------------//
bool InventoryItem::operator==(const InventoryItem& other)
{
    if (!GenericItem::operator==(other))
        return false;

    return d_weight == other.d_weight;
}

//----------------------------------------------------------------------------//
InventoryModel::InventoryModel() :
GenericItemModel<InventoryItem>(InventoryItem::make("Inventory", 0.0f)),
d_randomItemsCount(0)
{
}

//----------------------------------------------------------------------------//
void InventoryModel::load()
{
    InventoryItem* backpack = InventoryItem::make("Trip backpack", 2.0f, d_root);
    InventoryItem* prev_matryoshka = nullptr;

    // matryoshka Z to A
    for (char chr = 'Z'; chr >= 'A'; --chr)
    {
        InventoryItem* matryoshka = InventoryItem::make("Matryoshka " + String(1, chr), 1.0f, backpack);

        matryoshka->setIcon("DriveIcons/DriveStack");

        if (prev_matryoshka != nullptr)
        {
            prev_matryoshka->setParent(matryoshka);
            matryoshka->addItem(prev_matryoshka);
        }

        prev_matryoshka = matryoshka;
    }

    backpack->addItem(InventoryItem::make("Gepäckaufbewahrungsschein: Wiener Neustadt", 1.0f, backpack));
    backpack->addItem(InventoryItem::make("Gepäckaufbewahrungsschein: Frankfurt am Main", 1.0f, backpack));
    backpack->addItem(InventoryItem::make("Gepäckaufbewahrungsschein: Sankt Johann im Pongau", 1.0f, backpack));
    backpack->addItem(InventoryItem::make("Gepäckaufbewahrungsschein: Seekirchen am Wallersee", 1.0f, backpack));

    InventoryItem* beans_can = InventoryItem::make("Beans can", 1.0f, backpack);
    beans_can->setIcon("DriveIcons/GlobalDrive");
    InventoryItem* beans = InventoryItem::make("Beans!", 0.1f, beans_can);
    beans_can->addItem(beans);

    backpack->addItem(prev_matryoshka);
    backpack->addItem(beans_can);

    d_root->addItem(backpack);

    InventoryItem* bow = InventoryItem::make("Bow", 23.451f, d_root);
    for (std::int32_t i = 25; i >= 0; --i)
    {
        InventoryItem* arrow = InventoryItem::make(
            "arrow " + PropertyHelper<std::int32_t>::toString(i), 0.2f, bow);
        bow->addItem(arrow);
    }
    d_root->addItem(bow);

    // generate *many* items :D
    for (std::int32_t i = 2000; i >= 1960; i -= 2)
    {
        InventoryItem* almanach = InventoryItem::make(
            "Almanach " + PropertyHelper<std::int32_t>::toString(i), 0.34f, d_root);

        if (i % 2 == 0)
            almanach->setIcon("DriveIcons/Lime");

        d_root->addItem(almanach);
    }
}

//----------------------------------------------------------------------------//
void InventoryModel::addRandomItemWithChildren(const ModelIndex& parent, size_t position,
    size_t child_count)
{
    InventoryItem* new_item = InventoryItem::make(
        "New random item #" + PropertyHelper<std::int32_t>::toString(d_randomItemsCount),
        0.3f, static_cast<InventoryItem*>(parent.d_modelData));

    for (size_t i = 0; i < child_count; ++i)
    {
        InventoryItem* new_subitem = InventoryItem::make(
            "New sub item #" + PropertyHelper<std::int32_t>::toString(d_randomItemsCount),
            1.3f, new_item);

        new_item->addItem(new_subitem);
    }

    addItemAtPosition(new_item, parent, position);
    d_randomItemsCount++;
}

//----------------------------------------------------------------------------//
void InventoryModel::updateItemName(const ModelIndex& index, const String& newName)
{
    ModelIndex parent_index = getParentIndex(index);

    notifyChildrenDataWillChange(parent_index, 0, 1);

    InventoryItem* item = static_cast<InventoryItem*>(index.d_modelData);
    item->setText(newName);

    notifyChildrenDataChanged(parent_index, 0, 1);
}

//----------------------------------------------------------------------------//
std::ostream& operator<<(std::ostream& output, const InventoryItem& item)
{
    return output << item.getText() << " [" << item.d_weight << "]";
}
