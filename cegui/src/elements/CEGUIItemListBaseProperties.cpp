/***********************************************************************
	filename: 	CEGUIItemListBaseProperties.cpp
	created:	5/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "elements/CEGUIItemListBaseProperties.h"
#include "elements/CEGUIItemListBase.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ItemListBaseProperties namespace section
namespace ItemListBaseProperties
{
    String AutoResizeEnabled::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::boolToString(static_cast<const ItemListBase*>(receiver)->isAutoResizeEnabled());
    }

    void AutoResizeEnabled::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ItemListBase*>(receiver)->setAutoResizeEnabled(PropertyHelper::stringToBool(value));
    }

    String SortEnabled::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::boolToString(static_cast<const ItemListBase*>(receiver)->isSortEnabled());
    }

    void SortEnabled::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ItemListBase*>(receiver)->setSortEnabled(PropertyHelper::stringToBool(value));
    }

    String SortMode::get(const PropertyReceiver* receiver) const
    {
        const char* out = "Ascending";
        ItemListBase::SortMode sm = static_cast<const ItemListBase*>(receiver)->getSortMode();
        if (sm == ItemListBase::Descending)
            out = "Descending";
        else if (sm == ItemListBase::UserSort)
            out = "UserSort";
        return String(out);
    }

    void SortMode::set(PropertyReceiver* receiver, const String& value)
    {
        ItemListBase::SortMode sm = ItemListBase::Ascending;
        if (value == "Descending")
            sm = ItemListBase::Descending;
        else if (value == "UserSort")
            sm = ItemListBase::UserSort;
        static_cast<ItemListBase*>(receiver)->setSortMode(sm);
    }

} // End of  ItemListBaseProperties namespace section


} // End of  CEGUI namespace section
