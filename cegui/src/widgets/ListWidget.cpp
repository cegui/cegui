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
void ListWidget::initialiseComponents()
{
    ListView::initialiseComponents();
    setModel(&d_itemModel);
}

//----------------------------------------------------------------------------//
void ListWidget::setSelectedItem(size_t item_index, bool state)
{
    if (item_index > d_itemModel.getChildCount(d_itemModel.getRootIndex()))
    {
        CEGUI_THROW(InvalidRequestException(
            "the value passed in the 'item_index' parameter is out of range for this ListWidget."));
    }

    setItemSelectionState(
        d_itemModel.makeIndex(item_index, d_itemModel.getRootIndex()), state);
}

//----------------------------------------------------------------------------//
StandardItem* ListWidget::getFirstSelectedItem()
{
    if (d_indexSelectionStates.empty())
        return 0;

    return d_itemModel.getItemForIndex(d_indexSelectionStates.front().d_selectedIndex);
}

//----------------------------------------------------------------------------//
StandardItemModel* ListWidget::getModel()
{
    return static_cast<StandardItemModel*>(&d_itemModel);
}

//----------------------------------------------------------------------------//
void ListWidget::addItem(const String& text)
{
    StandardItem item(text);
    addItem(item);
}

//----------------------------------------------------------------------------//
void ListWidget::addItem(const StandardItem& item)
{
    d_itemModel.addItem(item);
}
}
