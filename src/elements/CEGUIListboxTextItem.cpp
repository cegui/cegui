/************************************************************************
	filename: 	CEGUIListboxTextItem.cpp
	created:	12/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of List box text items
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
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
	d_font(NULL)
{
}


/*************************************************************************
	Return a pointer to the font being used by this ListboxTextItem
*************************************************************************/
const Font* ListboxTextItem::getFont(void) const
{
	// prefer out own font
	if (d_font != NULL)
	{
		return d_font;
	}
	// try our owner window's font setting (may be null if owner uses no existant default font)
	else if (d_owner != NULL)
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

	const Font* fnt = getFont();

	if (fnt != NULL)
	{
		tmp.d_height	= fnt->getLineSpacing();
		tmp.d_width		= fnt->getTextExtent(d_itemText);
	}

	return tmp;
}


/*************************************************************************
	Draw the list box item in its current state.
*************************************************************************/
void ListboxTextItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
{
	if (d_selected && (d_selectBrush != NULL))
	{
		d_selectBrush->draw(clipper, position.d_z, clipper, getModulateAlphaColourRect(d_selectCols, alpha));
	}

	const Font* fnt = getFont();

	if (fnt != NULL)
	{
		fnt->drawText(d_itemText, position, clipper, getModulateAlphaColourRect(d_textCols, alpha));
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
