/***********************************************************************
    created:    Sat Jul 19 2014
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
#include "CEGUI/widgets/ListWidget.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
ListWidgetItem& indexToWidgetItem(const ModelIndex& index)
{
    return *(static_cast<ListWidgetItem*>(index.d_modelData));
}

//----------------------------------------------------------------------------//
const String ListWidget::EventNamespace("ListWidget");
const String ListWidget::WidgetTypeName("CEGUI/ListWidget");

//----------------------------------------------------------------------------//
ListWidget::ListWidget(const String& type, const String& name) :
    ListView(type, name)
{
}

//----------------------------------------------------------------------------//
ListWidget::~ListWidget()
{
}

//----------------------------------------------------------------------------//
ListWidgetItem::ListWidgetItem()
{

}

//----------------------------------------------------------------------------//
ListWidgetItem::~ListWidgetItem()
{
}

//----------------------------------------------------------------------------//
bool ListWidgetItem::operator==(const ListWidgetItem& other) const
{
    return getText() == other.getText();
}

//----------------------------------------------------------------------------//
bool ListWidgetItem::operator<(const ListWidgetItem& other) const
{
    return getText() < other.getText();
}

//----------------------------------------------------------------------------//
bool ListWidget::isValidIndex(const ModelIndex& model_index) const
{
    return model_index.d_modelData != 0 && getChildId(model_index) != -1;
}

//----------------------------------------------------------------------------//
ModelIndex ListWidget::makeIndex(size_t child, const ModelIndex& parent_index)
{
    if (child >= d_widgetItems.size())
        return ModelIndex();

    return ModelIndex(&d_widgetItems[child]);
}

//----------------------------------------------------------------------------//
bool ListWidget::areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const
{
    return compareIndices(index1, index2) == 0;
}

//----------------------------------------------------------------------------//
int ListWidget::compareIndices(const ModelIndex& index1, const ModelIndex& index2) const
{
    if (!isValidIndex(index1) || !isValidIndex(index2))
        return false;

    return indexToWidgetItem(index1) < indexToWidgetItem(index2);
}

//----------------------------------------------------------------------------//
ModelIndex ListWidget::getParentIndex(const ModelIndex& model_index) const
{
    return getRootIndex();
}

//----------------------------------------------------------------------------//
int ListWidget::getChildId(const ModelIndex& model_index) const
{
    std::vector<ListWidgetItem>::const_iterator itor =
        std::find(d_widgetItems.begin(), d_widgetItems.end(), indexToWidgetItem(model_index));

    if (itor == d_widgetItems.end())
        return -1;

    return std::distance(d_widgetItems.begin(), itor);
}

//----------------------------------------------------------------------------//
ModelIndex ListWidget::getRootIndex() const
{
    return ModelIndex();
}

//----------------------------------------------------------------------------//
size_t ListWidget::getChildCount(const ModelIndex& model_index) const
{
    return d_widgetItems.size();
}

//----------------------------------------------------------------------------//
String ListWidget::getData(const ModelIndex& model_index, ItemDataRole role /*= IDR_Text*/)
{
    if (!isValidIndex(model_index))
        return "";

    return indexToWidgetItem(model_index).getText();
}

//----------------------------------------------------------------------------//
void ListWidget::addItem(String text)
{
    ListWidgetItem item;
    item.setText(text);

    addItem(item);
}

//----------------------------------------------------------------------------//
void ListWidget::addItem(const ListWidgetItem& item)
{
    d_widgetItems.push_back(item);
    notifyChildrenAdded(getRootIndex(), d_widgetItems.size() - 1, 1);
}

//----------------------------------------------------------------------------//
void ListWidget::initialiseComponents()
{
    ListView::initialiseComponents();
    setModel(this);
}
}
