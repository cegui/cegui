/***********************************************************************
    created:    Sat Aug 09 2014
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
#include "CEGUI/views/GenericItemModel.h"
#include "CEGUI/Exceptions.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
GenericItem::GenericItem() : d_text(""), d_parent(nullptr)
{
}

//----------------------------------------------------------------------------//
GenericItem::GenericItem(const String& text) : d_text(text), d_parent(nullptr)
{
}

//----------------------------------------------------------------------------//
GenericItem::GenericItem(const String& text, const String& icon) :
d_text(text), d_icon(icon), d_parent(nullptr)
{
}

//----------------------------------------------------------------------------//
GenericItem::~GenericItem()
{
    while (!d_children.empty())
    {
        GenericItem* item = d_children.back();
        d_children.pop_back();
        delete item;
    }
}

//----------------------------------------------------------------------------//
bool GenericItem::operator==(const GenericItem& other) const
{
    if (d_text != other.d_text)
        return false;

    if (d_icon != other.d_icon)
        return false;

    if (d_children.size() != other.d_children.size()) return false;

    for (size_t i = 0; i < d_children.size(); ++i)
    {
        if (d_children.at(i) != other.d_children.at(i))
            return false;
    }

    return true;
}

//----------------------------------------------------------------------------//
bool GenericItem::operator!=(const GenericItem& other) const
{
    return !(*this == other);
}

//----------------------------------------------------------------------------//
bool GenericItem::operator<(const GenericItem& other) const
{
    return d_text < other.d_text;
}

//----------------------------------------------------------------------------//
void GenericItem::addItem(GenericItem* child)
{
    d_children.push_back(child);
    child->setParent(this);
}

}
