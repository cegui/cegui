/***********************************************************************
    created:    12/6/2004
    author:        Paul D Turner
    
    purpose:    Implementation of base class for list items
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
#include "CEGUI/widgets/ListboxItem.h"
#include "CEGUI/ImageManager.h"

namespace CEGUI
{
const Colour ListboxItem::DefaultSelectionColour = 0xFF4444AA;

//----------------------------------------------------------------------------//
ListboxItem::ListboxItem(const String& text, unsigned int item_id, void* item_data, bool disabled, bool auto_delete)
    : d_textLogical(text)
    , d_itemID(item_id)
    , d_itemData(item_data)
    , d_disabled(disabled)
    , d_autoDelete(auto_delete)
    , d_selectCols(DefaultSelectionColour, DefaultSelectionColour, DefaultSelectionColour, DefaultSelectionColour)
{
}

//----------------------------------------------------------------------------//
void ListboxItem::setSelectionBrushImage(const String& name)
{
    setSelectionBrushImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
Colour ListboxItem::calculateModulatedAlphaColour(Colour col, float alpha) const
{
    Colour temp(col);
    temp.setAlpha(temp.getAlpha() * alpha);
    return temp;
}

//----------------------------------------------------------------------------//
void ListboxItem::setSelectionColours(Colour top_left_colour, Colour top_right_colour, Colour bottom_left_colour, Colour bottom_right_colour)
{
    d_selectCols.d_top_left        = top_left_colour;
    d_selectCols.d_top_right    = top_right_colour;
    d_selectCols.d_bottom_left    = bottom_left_colour;
    d_selectCols.d_bottom_right    = bottom_right_colour;
}

}
