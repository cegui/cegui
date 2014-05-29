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
#include <string>

using namespace CEGUI;


//----------------------------------------------------------------------------//
Material Material::make(const CEGUI::String& name, float weight, float unit_price)
{
    Material mat;

    mat.d_materialName = name;
    mat.d_weight = weight;
    mat.d_unitPrice = unit_price;

    return mat;
}

//----------------------------------------------------------------------------//
ObjectPart ObjectPart::make(const CEGUI::String& name)
{
    ObjectPart part;

    part.d_partName = name;

    return part;
}

//----------------------------------------------------------------------------//
InventoryObject InventoryObject::make(const CEGUI::String& name, int intelligence, int strength, int agility)
{
    InventoryObject obj;

    obj.d_name = name;
    obj.d_intelligence = intelligence;
    obj.d_strength = strength;
    obj.d_agility = agility;

    return obj;
}

//----------------------------------------------------------------------------//
void InventoryModel::load()
{
    // materials
    Material wood_material = Material::make("wood", 1.2f, 0.4f);
    Material iron_material = Material::make("iron", 3.0f, 2.456f);
    Material onyx_material = Material::make("onyx", 0.5f, 200.0f);
    Material ruby_material = Material::make("ruby", 0.6f, 120.0f);
    Material glue_material = Material::make("glue", 0.1f, 0.1f);
    Material wire_material = Material::make("wire", 0.1f, 0.2f);
    Material magic_dust_material = Material::make("magic dust", 0.001f, 25.0f);

    // object parts
    ObjectPart wand_rod_part = ObjectPart::make("rod");
    wand_rod_part.d_materials.push_back(wood_material);
    wand_rod_part.d_materials.push_back(wire_material);

    ObjectPart lightning_wand_head = ObjectPart::make("lightning head");
    lightning_wand_head.d_materials.push_back(onyx_material);
    lightning_wand_head.d_materials.push_back(magic_dust_material);

    ObjectPart fiery_wand_head = ObjectPart::make("fiery head");
    fiery_wand_head.d_materials.push_back(ruby_material);
    fiery_wand_head.d_materials.push_back(magic_dust_material);

    ObjectPart sword_handle_part = ObjectPart::make("sword handle");
    sword_handle_part.d_materials.push_back(wood_material);
    sword_handle_part.d_materials.push_back(wire_material);

    ObjectPart sword_blade_part = ObjectPart::make("sword blade");
    sword_blade_part.d_materials.push_back(iron_material);

    // inventory objects
    InventoryObject lightning_wand = InventoryObject::make("Lightning Wand", 20, 1, 1);
    lightning_wand.d_parts.push_back(wand_rod_part);
    lightning_wand.d_parts.push_back(lightning_wand_head);
    d_inventoryObjects.push_back(lightning_wand);

    InventoryObject fiery_wand = InventoryObject::make("Fiery Wand", 23, 0, 4);
    fiery_wand.d_parts.push_back(wand_rod_part);
    fiery_wand.d_parts.push_back(fiery_wand_head);
    d_inventoryObjects.push_back(fiery_wand);

    InventoryObject justice_sword = InventoryObject::make("Justice sword", 1, 25, 3);
    justice_sword.d_parts.push_back(sword_handle_part);
    justice_sword.d_parts.push_back(sword_blade_part);
    d_inventoryObjects.push_back(justice_sword);
}

bool InventoryModel::isValidIndex(const ModelIndex& model_index) const
{
    //TODO: check if we still have a reference to model_index.userdata?
    return model_index.d_modelData != 0;
}

CEGUI::ModelIndex InventoryModel::makeIndex(size_t child, const ModelIndex& parent_index)
{
    // root item
    if (parent_index.d_modelData == 0)
    {
        //TODO: find a better way of validating all these conditions without
        // so much duplication(s). Maybe we can't remove it if we have this
        // level of depth (3 in our case)
        return child >= 0 && child <= d_inventoryObjects.size()
            ? ModelIndex(&d_inventoryObjects.at(child))
            : ModelIndex();
    }

    Model* model = static_cast<Model*>(parent_index.d_modelData);

    InventoryObject* object = dynamic_cast<InventoryObject*>(model);
    if (object != 0)
    {
        return child >= 0 && child <= object->d_parts.size()
            ? ModelIndex(&object->d_parts.at(child))
            : ModelIndex();
    }

    ObjectPart* part = dynamic_cast<ObjectPart*>(model);
    if (part != 0)
    {
        return child >= 0 && child <= part->d_materials.size()
            ? ModelIndex(&part->d_materials.at(child))
            : ModelIndex();
    }

    return ModelIndex();
}

CEGUI::ModelIndex InventoryModel::getParentIndex(const ModelIndex& model_index)
{
    return CEGUI::ModelIndex();
}

int InventoryModel::getChildCount(const ModelIndex& model_index)
{
    if (model_index.d_modelData == 0)
        return d_inventoryObjects.size();

    Model* model = static_cast<Model*>(model_index.d_modelData);
    InventoryObject* object = dynamic_cast<InventoryObject*>(model);
    if (object != 0)
        return object->d_parts.size();

    ObjectPart* part = dynamic_cast<ObjectPart*>(model);
    if (part != 0)
        return part->d_materials.size();

    return 0;
}

CEGUI::String InventoryModel::getData(const ModelIndex& model_index, ItemDataRole role /*= IDR_Text*/)
{
    Model* model = static_cast<Model*>(model_index.d_modelData);
    InventoryObject* object = dynamic_cast<InventoryObject*>(model);
    if (object != 0)
    {
        if (role == CEGUI::IDR_Text)
            return object->d_name;
    }

    ObjectPart* part = dynamic_cast<ObjectPart*>(model);
    if (part != 0)
    {
        if (role == CEGUI::IDR_Text)
            return part->d_partName;
    }

    Material* material = dynamic_cast<Material*>(model);
    if (material != 0)
    {
        if (role == CEGUI::IDR_Text)
            return material->d_materialName;
    }

    return "";
}
