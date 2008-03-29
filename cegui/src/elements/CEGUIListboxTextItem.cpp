/***********************************************************************
	filename: 	CEGUIListboxTextItem.cpp
	created:	12/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of List box text items
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
#include "elements/CEGUIListboxTextItem.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIWindow.h"
#include "CEGUIImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const colour	ListboxTextItem::DefaultTextColour		= 0xFFFFFFFF;


/*************************************************************************
	Constructor
*************************************************************************/
ListboxTextItem::ListboxTextItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete) :
	ListboxItem(text, item_id, item_data, disabled, auto_delete),
	d_textCols(DefaultTextColour, DefaultTextColour, DefaultTextColour, DefaultTextColour),
	d_font(0)
{
}


/*************************************************************************
	Return a pointer to the font being used by this ListboxTextItem
*************************************************************************/
Font* ListboxTextItem::getFont(void) const
{
	// prefer out own font
	if (d_font)
	{
		return d_font;
	}
	// try our owner window's font setting (may be null if owner uses no existant default font)
	else if (d_owner)
	{
		return d_owner->getFont();
	}
	// no owner, just use the default (which may be NULL anyway)
	else
	{
		return System::getSingleton().getDefaultFont();
	}

}


/*************************************************************************
	Set the font to be used by this ListboxTextItem
*************************************************************************/
void ListboxTextItem::setFont(const String& font_name)
{
	setFont(FontManager::getSingleton().getFont(font_name));
}

/*************************************************************************
	Return the rendered pixel size of this list box item.
*************************************************************************/
Size ListboxTextItem::getPixelSize(void) const
{
	Size tmp(0,0);

	Font* fnt = getFont();

	if (fnt)
	{
		tmp.d_height = PixelAligned(fnt->getLineSpacing());
		tmp.d_width = PixelAligned(fnt->getTextExtent(d_itemText));
	}

	return tmp;
}


/*************************************************************************
	Draw the list box item in its current state.
*************************************************************************/
void ListboxTextItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
{
	if (d_selected && (d_selectBrush != 0))
	{
		d_selectBrush->draw(clipper, position.d_z, clipper, getModulateAlphaColourRect(d_selectCols, alpha));
	}

	Font* fnt = getFont();

	if (fnt)
	{
        Vector3 finalPos(position);
        finalPos.d_y += PixelAligned((fnt->getLineSpacing() - fnt->getFontHeight()) * 0.5f);
		fnt->drawText(d_itemText, finalPos, clipper, getModulateAlphaColourRect(d_textCols, alpha));
	}

}

void ListboxTextItem::draw(RenderCache& cache,const Rect& targetRect, float zBase, float alpha, const Rect* clipper) const
{
    if (d_selected && d_selectBrush != 0)
    {
        cache.cacheImage(*d_selectBrush, targetRect, zBase, getModulateAlphaColourRect(d_selectCols, alpha), clipper);
    }

    Font* font = getFont();

    if (font)
    {
        Rect finalPos(targetRect);
        finalPos.d_top += PixelAligned((font->getLineSpacing() - font->getFontHeight()) * 0.5f);
        cache.cacheText(d_itemText, font, LeftAligned, finalPos, zBase, getModulateAlphaColourRect(d_textCols, alpha), clipper);
    }
}


/*************************************************************************
	Set the colours used for text rendering.	
*************************************************************************/
void ListboxTextItem::setTextColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour)
{
	d_textCols.d_top_left		= top_left_colour;
	d_textCols.d_top_right		= top_right_colour;
	d_textCols.d_bottom_left	= bottom_left_colour;
	d_textCols.d_bottom_right	= bottom_right_colour;
}


} // End of  CEGUI namespace section
