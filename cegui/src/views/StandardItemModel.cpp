/***********************************************************************
    created:    Wed May 23 2014
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
#include "CEGUI/views/StandardItemModel.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
StandardItem& indexToItem(const ModelIndex& index)
{
    return *(static_cast<StandardItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
StandardItem::StandardItem()
{
}

//----------------------------------------------------------------------------//
StandardItem::~StandardItem()
{
}

//----------------------------------------------------------------------------//
bool StandardItem::operator==(const StandardItem& other) const
{
    return getText() == other.getText();
}

//----------------------------------------------------------------------------//
bool StandardItem::operator<(const StandardItem& other) const
{
    return getText() < other.getText();
}

//----------------------------------------------------------------------------//
bool StandardItemModel::isValidIndex(const ModelIndex& model_index) const
{
    return model_index.d_modelData != 0 && getChildId(model_index) != -1;
}

//----------------------------------------------------------------------------//
ModelIndex StandardItemModel::makeIndex(size_t child, const ModelIndex& parent_index)
{
    if (child >= d_items.size())
        return ModelIndex();

    return ModelIndex(&d_items[child]);
}

//----------------------------------------------------------------------------//
bool StandardItemModel::areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const
{
    return compareIndices(index1, index2) == 0;
}

//----------------------------------------------------------------------------//
int StandardItemModel::compareIndices(const ModelIndex& index1, const ModelIndex& index2) const
{
    if (!isValidIndex(index1) || !isValidIndex(index2))
        return false;

    if (indexToItem(index1) < indexToItem(index2))
        return -1;

    return indexToItem(index1) == indexToItem(index2) ? 0 : 1;
}

//----------------------------------------------------------------------------//
ModelIndex StandardItemModel::getParentIndex(const ModelIndex& model_index) const
{
    return getRootIndex();
}

//----------------------------------------------------------------------------//
int StandardItemModel::getChildId(const ModelIndex& model_index) const
{
    std::vector<StandardItem>::const_iterator itor =
        std::find(d_items.begin(), d_items.end(), indexToItem(model_index));

    if (itor == d_items.end())
        return -1;

    return std::distance(d_items.begin(), itor);
}

//----------------------------------------------------------------------------//
ModelIndex StandardItemModel::getRootIndex() const
{
    return ModelIndex();
}

//----------------------------------------------------------------------------//
size_t StandardItemModel::getChildCount(const ModelIndex& model_index) const
{
    return d_items.size();
}

//----------------------------------------------------------------------------//
String StandardItemModel::getData(const ModelIndex& model_index, ItemDataRole role /*= IDR_Text*/)
{
    if (!isValidIndex(model_index))
        return "";

    return indexToItem(model_index).getText();
}

//----------------------------------------------------------------------------//
void StandardItemModel::addItem(String text)
{
    StandardItem item;
    item.setText(text);

    addItem(item);
}

//----------------------------------------------------------------------------//
void StandardItemModel::addItem(const StandardItem& item)
{
    d_items.push_back(item);
    notifyChildrenAdded(getRootIndex(), d_items.size() - 1, 1);
}

}
