/***********************************************************************
    filename:   Model.h
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

#include "CEGUI/ItemModel.h"
#include "CEGUI/String.h"
#include <vector>

/*
    This is a sample Model in order to show how different views render it.
    So, we have multiple nested levels and columns for some of them, in the
    following way, so we can demo many different cases of a model:


    InventoryObject     INT     STR     AGI
        ObjectPart1
            Material1   WEIGHT  UNIT_PRICE
            Material2   WEIGHT  UNIT_PRICE
            ...
        ObjectPart2
        ...
    InventoryObject
        ...
*/

// just a marker class so we can use dynamic_cast :) on ModelIndex's user data void*
// TIP: Instead of doing a dynamic cast, we could have a nice data structure with
// a boolean-flag that tells us what kind of data we stored for a ModelIndex.
class Model
{
public:
    virtual ~Model() { }
};

class Material : public Model
{
public:
    CEGUI::String d_materialName;
    float d_weight;
    float d_unitPrice;

    static Material make(const CEGUI::String& name, float weight, float unit_price);
};

class ObjectPart : public Model
{
public:
    CEGUI::String d_partName;
    std::vector<Material> d_materials;

    static ObjectPart make(const CEGUI::String& name);
};

class InventoryObject : public Model
{
public:
    int d_intelligence;
    int d_strength;
    int d_agility;
    CEGUI::String d_name;
    std::vector<ObjectPart> d_parts;

    static InventoryObject make(const CEGUI::String& name, int intelligence,
        int strength, int agility);
};


class InventoryModel : public CEGUI::ItemModel
{
public:
    // simulate loading the model
    void load();

    // implementation of ItemModel
    virtual bool isValidIndex(const CEGUI::ModelIndex& model_index) const;
    virtual CEGUI::ModelIndex makeIndex(int child, const CEGUI::ModelIndex& parent_index);
    virtual CEGUI::ModelIndex getParentIndex(const CEGUI::ModelIndex& model_index);
    virtual int getChildCount(const CEGUI::ModelIndex& model_index);
    virtual CEGUI::String getData(const CEGUI::ModelIndex& model_index, CEGUI::ItemDataRole role = CEGUI::IDR_Text);

private:
    std::vector<InventoryObject> d_inventoryObjects;
};

#endif  // end of guard_Sample_Model_View_Model_h_
