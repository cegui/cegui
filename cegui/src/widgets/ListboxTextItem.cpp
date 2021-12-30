/***********************************************************************
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
#include "CEGUI/widgets/ListboxTextItem.h"
#include "CEGUI/System.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Font.h"
#include "CEGUI/Window.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/RenderedStringParser.h"

namespace CEGUI
{
const Colour ListboxTextItem::DefaultTextColour = 0xFFFFFFFF;

//----------------------------------------------------------------------------//
ListboxTextItem::ListboxTextItem(const String& text, unsigned int item_id, void* item_data, bool disabled, bool auto_delete) :
	ListboxItem(text, item_id, item_data, disabled, auto_delete),
	d_textCols(DefaultTextColour, DefaultTextColour, DefaultTextColour, DefaultTextColour),
    d_renderedStringParser(&CEGUI::System::getSingleton().getDefaultRenderedStringParser())
{
}

//----------------------------------------------------------------------------//
const Font* ListboxTextItem::getFont() const
{
	// prefer out own font
	if (d_font)
		return d_font;
	// try our owner window's font setting (may be null if owner uses no existant default font)
	if (d_owner)
		return d_owner->getActualFont();
    // no owner, so the default font is ambiguous (it could be of any context)
    return nullptr;  
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setFont(const String& font_name)
{
	setFont(&FontManager::getSingleton().get(font_name));
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setFont(Font* font)
{
    d_font = font;
    d_renderedStringValid = false;
}

//----------------------------------------------------------------------------//
Sizef ListboxTextItem::getPixelSize() const
{
    const Font* fnt = getFont();

    if (!fnt)
        return Sizef(0, 0);

    if (!d_renderedStringValid)
        parseTextString();

    Sizef sz(0.0f, 0.0f);

    for (size_t i = 0; i < d_renderedString.getLineCount(); ++i)
    {
        const Sizef line_sz(d_renderedString.getPixelSize(d_owner, i));
        sz.d_height += line_sz.d_height;

        if (line_sz.d_width > sz.d_width)
            sz.d_width = line_sz.d_width;
    }

    return sz;
}

//----------------------------------------------------------------------------//
std::vector<GeometryBuffer*> ListboxTextItem::createRenderGeometry(
    const Rectf& targetRect, float alpha, const Rectf* clipper) const
{
    std::vector<GeometryBuffer*> geomBuffers;

    if (d_selected && d_selectBrush != nullptr)
    {
        ImageRenderSettings imgRenderSettings(
            targetRect, clipper, true,
            d_selectCols, alpha);

        std::vector<GeometryBuffer*> brushGeomBuffers =
            d_selectBrush->createRenderGeometry(imgRenderSettings);

        geomBuffers.insert(geomBuffers.end(), brushGeomBuffers.begin(),
            brushGeomBuffers.end());
    }

    const Font* font = getFont();

    if (!font)
        return geomBuffers;

    glm::vec2 draw_pos(targetRect.getPosition());

    draw_pos.y += CoordConverter::alignToPixels(
        (font->getLineSpacing() - font->getFontHeight()) * 0.5f);

    if (!d_renderedStringValid)
        parseTextString();

    const ColourRect final_colours(ColourRect(0xFFFFFFFF));

    for (size_t i = 0; i < d_renderedString.getLineCount(); ++i)
    {
        std::vector<GeometryBuffer*> stringGeomBuffers = d_renderedString.createRenderGeometry(
            d_owner, i, draw_pos, &final_colours, clipper, 0.0f);

        geomBuffers.insert(geomBuffers.end(), stringGeomBuffers.begin(),
            stringGeomBuffers.end());

        draw_pos.y += d_renderedString.getPixelSize(d_owner, i).d_height;
    }

    return geomBuffers;
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setTextColours(Colour top_left_colour,
                                     Colour top_right_colour,
                                     Colour bottom_left_colour,
                                     Colour bottom_right_colour)
{
	d_textCols.d_top_left		= top_left_colour;
	d_textCols.d_top_right		= top_right_colour;
	d_textCols.d_bottom_left	= bottom_left_colour;
	d_textCols.d_bottom_right	= bottom_right_colour;

    d_renderedStringValid = false;
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setText(const String& text)
{
    ListboxItem::setText(text);
    d_renderedStringValid = false;
}

//----------------------------------------------------------------------------//
void ListboxTextItem::parseTextString() const
{
    d_renderedString = d_renderedStringParser->parse(getTextVisual(), getFont(), &d_textCols);
    d_renderedStringValid = true;
}

//----------------------------------------------------------------------------//
// FIXME: now this silently resets a custom parser!
void ListboxTextItem::setTextParsingEnabled(bool enable)
{
    const auto prevParser = d_renderedStringParser;
    d_renderedStringParser = enable ?
        &CEGUI::System::getSingleton().getBasicRenderedStringParser() :
        &CEGUI::System::getSingleton().getDefaultRenderedStringParser();
    d_renderedStringValid &= (prevParser != d_renderedStringParser);
}

//----------------------------------------------------------------------------//
bool ListboxTextItem::isTextParsingEnabled() const
{
    return d_renderedStringParser != &CEGUI::System::getSingleton().getDefaultRenderedStringParser();
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setCustomRenderedStringParser(CEGUI::RenderedStringParser* parser)
{
    const auto prevParser = d_renderedStringParser;
    d_renderedStringParser = parser ? parser : &CEGUI::System::getSingleton().getDefaultRenderedStringParser();
    d_renderedStringValid &= (prevParser != d_renderedStringParser);
}

//----------------------------------------------------------------------------//
bool ListboxTextItem::handleFontRenderSizeChange(const Font* const font)
{
    return getFont() == font;
}

}
