/***********************************************************************
    created:    Wed May 21 2014
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
#include "CEGUI/views/ItemModel.h"

#include <ostream>

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ItemModel::EventChildrenWillBeAdded("ChildrenWillBeAdded");
const String ItemModel::EventChildrenAdded("ChildrenAdded");
const String ItemModel::EventChildrenWillBeRemoved("ChildrenWillBeRemoved");
const String ItemModel::EventChildrenRemoved("ChildrenRemoved");
const String ItemModel::EventChildrenDataWillChange;
const String ItemModel::EventChildrenDataChanged("ChildrenDataChanged");

//----------------------------------------------------------------------------//
std::ostream& operator<< (std::ostream& os, const ModelIndex& arg)
{
    return os << "CEGUI::ModelIndex(" << arg.d_modelData << ")";
}

//----------------------------------------------------------------------------//
ModelEventArgs::ModelEventArgs(ItemModel* item_model, ModelIndex parent_index, size_t start_id, size_t count /*= 1*/) :
    d_itemModel(item_model),
    d_parentIndex(parent_index),
    d_startId(start_id),
    d_count(count)
{
}

//----------------------------------------------------------------------------//
ModelIndex::ModelIndex(void* model_data /*= 0*/) :
    d_modelData(model_data)
{
}

//----------------------------------------------------------------------------//
ItemModel::~ItemModel()
{
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenWillBeAdded(ModelIndex parent_index, size_t start_id, size_t count)
{
    ModelEventArgs args(this, parent_index, start_id, count);
    fireEvent(EventChildrenWillBeAdded, args);
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenAdded(ModelIndex parent_index, size_t start_id, size_t count)
{
    ModelEventArgs args(this, parent_index, start_id, count);
    fireEvent(EventChildrenAdded, args);
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenWillBeRemoved(ModelIndex parent_index, size_t start_id, size_t count)
{
    ModelEventArgs args(this, parent_index, start_id, count);
    fireEvent(EventChildrenWillBeRemoved, args);
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenRemoved(ModelIndex parent_index, size_t start_id, size_t count)
{
    ModelEventArgs args(this, parent_index, start_id, count);
    fireEvent(EventChildrenRemoved, args);
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenDataWillChange(ModelIndex parent_index, size_t start_id, size_t count)
{
    ModelEventArgs args(this, parent_index, start_id, count);
    fireEvent(EventChildrenDataWillChange, args);
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenDataChanged(ModelIndex parent_index, size_t start_id, size_t count)
{
    ModelEventArgs args(this, parent_index, start_id, count);
    fireEvent(EventChildrenDataChanged, args);
}

//----------------------------------------------------------------------------//
bool ItemModel::areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const
{
    return compareIndices(index1, index2) == 0;
}
}
