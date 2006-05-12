/***********************************************************************
	filename: 	CEGUIListboxItem.cpp
	created:	12/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of base class for list items
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
#include "elements/CEGUIListboxItem.h"
#include "CEGUISystem.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const colour	ListboxItem::DefaultSelectionColour	= 0xFF4444AA;

/*************************************************************************
	Base class constructor
*************************************************************************/
ListboxItem::ListboxItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete) :
	d_itemText(text),
	d_itemID(item_id),
	d_itemData(item_data),
    d_selected(false),
	d_disabled(disabled),
    d_autoDelete(auto_delete),
	d_owner(0),
    d_selectCols(DefaultSelectionColour, DefaultSelectionColour, DefaultSelectionColour, DefaultSelectionColour),
	d_selectBrush(0)
{
}


/*************************************************************************
	Set the selection highlighting brush image.
*************************************************************************/
void ListboxItem::setSelectionBrushImage(const String& imageset, const String& image)
{
	setSelectionBrushImage(&ImagesetManager::getSingleton().getImageset(imageset)->getImage(image));
}


/*************************************************************************
	Return a ColourRect object describing the colours in 'cols' after
	having their alpha component modulated by the value 'alpha'.
*************************************************************************/
ColourRect ListboxItem::getModulateAlphaColourRect(const ColourRect& cols, float alpha) const
{
	return ColourRect
		(
			calculateModulatedAlphaColour(cols.d_top_left, alpha),
			calculateModulatedAlphaColour(cols.d_top_right, alpha),
			calculateModulatedAlphaColour(cols.d_bottom_left, alpha),
			calculateModulatedAlphaColour(cols.d_bottom_right, alpha)
		);
}


/*************************************************************************
	Return a colour value describing the colour specified by 'col' after
	having its alpha component modulated by the value 'alpha'.
*************************************************************************/
colour ListboxItem::calculateModulatedAlphaColour(colour col, float alpha) const
{
	colour temp(col);
	temp.setAlpha(temp.getAlpha() * alpha);
	return temp;
}


/*************************************************************************
	Set the colours used for selection highlighting.	
*************************************************************************/
void ListboxItem::setSelectionColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour)
{
	d_selectCols.d_top_left		= top_left_colour;
	d_selectCols.d_top_right	= top_right_colour;
	d_selectCols.d_bottom_left	= bottom_left_colour;
	d_selectCols.d_bottom_right	= bottom_right_colour;
}

} // End of  CEGUI namespace section
