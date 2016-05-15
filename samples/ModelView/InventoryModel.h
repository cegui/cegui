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
#ifndef _Sample_Model_View_Model_h_
#define _Sample_Model_View_Model_h_

#include "CEGUI/views/GenericItemModel.h"
#include "CEGUI/String.h"
#include <vector>

/*
    This is a sample Model in order to show how different views render it.
    So, we have multiple nested levels and columns (properties) for some of them,
    in the following way.

    Inventory Item (backpack)                           weight
        Inventory Item (can)                            ...
            Inventory Item (beans)
        Inventory Item (matryoshka doll)
            Inventory Item (matryoshka doll)
                Inventory Item (matryoshka doll)
                    Inventory Item (matryoshka doll)
                        ...
        ...
    ...
*/

class InventoryItem : public CEGUI::GenericItem
{
public:
    InventoryItem(CEGUI::String name);
    float d_weight;

    bool operator==(const InventoryItem& other);

    static InventoryItem* make(const CEGUI::String& name, float weight,
        GenericItem* parent = nullptr);
    friend std::ostream& operator<< (std::ostream& output, const InventoryItem& item);
};

class InventoryModel : public CEGUI::GenericItemModel<InventoryItem>
{
public:
    InventoryModel();

    // simulate loading the model
    void load();

    void addRandomItemWithChildren(const CEGUI::ModelIndex& parent, size_t position,
        size_t child_count = 1);
    void updateItemName(const CEGUI::ModelIndex& index, const CEGUI::String& newName);

private:
    std::int32_t d_randomItemsCount;
};

#endif
