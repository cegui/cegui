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
#include "CEGUI/Exceptions.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
StandardItem::StandardItem() : GenericItem(), d_id(0)
{
}

//----------------------------------------------------------------------------//
StandardItem::StandardItem(const String& text, unsigned int id) : GenericItem(text), d_id(id)
{
}

//----------------------------------------------------------------------------//
StandardItem::StandardItem(const String& text, const String& icon, unsigned int id /*= 0*/) :
GenericItem(text, icon), d_id(id)
{
}

//----------------------------------------------------------------------------//
StandardItemModel::StandardItemModel() :
GenericItemModel<StandardItem>(new StandardItem)
{
}

//----------------------------------------------------------------------------//
void StandardItemModel::updateItemText(StandardItem* item, const String& new_text)
{
    ModelIndex parent_index = getParentIndex(getIndexForItem(item));

    notifyChildrenDataWillChange(parent_index, 0, 1);

    item->setText(new_text);

    notifyChildrenDataChanged(parent_index, 0, 1);
}
}
