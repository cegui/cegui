/***********************************************************************
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUI/Config.h"
#include "CEGUI/widgets/ListboxItem.h"
#if defined(CEGUI_USE_RAQM)
#include "CEGUI/RaqmTextData.h"
#endif
#include "CEGUI/System.h"
#include "CEGUI/ImageManager.h"
#if defined (CEGUI_USE_FRIBIDI)
    #include "CEGUI/FribidiVisualMapping.h"
#elif defined (CEGUI_USE_MINIBIDI)
    #include "CEGUI/MinibidiVisualMapping.h"
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const Colour	ListboxItem::DefaultSelectionColour	= 0xFF4444AA;

/*************************************************************************
	Base class constructor
*************************************************************************/
ListboxItem::ListboxItem(const String& text, unsigned int item_id, void* item_data, bool disabled, bool auto_delete) :
#if defined (CEGUI_USE_FRIBIDI)
    d_bidiVisualMapping(new FribidiVisualMapping),
#elif defined (CEGUI_USE_MINIBIDI)
    d_bidiVisualMapping(new MinibidiVisualMapping),
#elif defined(CEGUI_BIDI_SUPPORT)
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
#ifdef CEGUI_USE_RAQM
    d_raqmTextData(nullptr),
#endif 
	d_itemID(item_id),
	d_itemData(item_data),
    d_selected(false),
	d_disabled(disabled),
    d_autoDelete(auto_delete),
	d_owner(nullptr),
    d_selectCols(DefaultSelectionColour, DefaultSelectionColour, DefaultSelectionColour, DefaultSelectionColour),
	d_selectBrush(nullptr)
{
#ifdef CEGUI_USE_RAQM
    d_raqmTextData = new RaqmTextData();
#endif

    ListboxItem::setText(text);
}

//----------------------------------------------------------------------------//
ListboxItem::~ListboxItem(void)
{
#ifdef CEGUI_USE_RAQM
    delete d_raqmTextData;
#endif

#ifdef CEGUI_BIDI_SUPPORT
    delete d_bidiVisualMapping;
#endif
}

/*************************************************************************
	Set the selection highlighting brush image.
*************************************************************************/
void ListboxItem::setSelectionBrushImage(const String& name)
{
	setSelectionBrushImage(&ImageManager::getSingleton().get(name));
}

/*************************************************************************
	Return a colour value describing the colour specified by 'col' after
	having its alpha component modulated by the value 'alpha'.
*************************************************************************/
Colour ListboxItem::calculateModulatedAlphaColour(Colour col, float alpha) const
{
	Colour temp(col);
	temp.setAlpha(temp.getAlpha() * alpha);
	return temp;
}


/*************************************************************************
	Set the colours used for selection highlighting.	
*************************************************************************/
void ListboxItem::setSelectionColours(Colour top_left_colour, Colour top_right_colour, Colour bottom_left_colour, Colour bottom_right_colour)
{
	d_selectCols.d_top_left		= top_left_colour;
	d_selectCols.d_top_right	= top_right_colour;
	d_selectCols.d_bottom_left	= bottom_left_colour;
	d_selectCols.d_bottom_right	= bottom_right_colour;
}


void ListboxItem::setText( const String& text )
{
   d_textLogical = text;

#ifdef CEGUI_BIDI_SUPPORT
   d_bidiDataValid = false;
#endif

#ifdef CEGUI_USE_RAQM
   d_raqmTextNeedsUpdate = true;
#endif
}

//----------------------------------------------------------------------------//
const String& ListboxItem::getTextVisual() const
{
#if defined(CEGUI_BIDI_SUPPORT)
    // no bidi support
    if (!d_bidiVisualMapping)
        return d_textLogical;

    if (!d_bidiDataValid)
    {
        d_bidiVisualMapping->updateVisual(d_textLogical);
        d_bidiDataValid = true;
    }

    return d_bidiVisualMapping->getTextVisual();
#else
    return d_textLogical;
#endif
}

//----------------------------------------------------------------------------//
bool ListboxItem::handleFontRenderSizeChange(const Font* const /*font*/)
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
