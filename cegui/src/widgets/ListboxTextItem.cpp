/***********************************************************************
    created:    12/6/2004
    author:        Paul D Turner
    
    purpose:    Implementation of List box text items
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
#include "CEGUI/text/Font.h"
#include "CEGUI/Window.h"
#include "CEGUI/text/TextParser.h"

namespace CEGUI
{
const Colour ListboxTextItem::DefaultTextColour = 0xFFFFFFFF;

//----------------------------------------------------------------------------//
ListboxTextItem::ListboxTextItem(const String& text, unsigned int item_id, void* item_data, bool disabled, bool auto_delete) :
    ListboxItem(text, item_id, item_data, disabled, auto_delete),
    d_textCols(DefaultTextColour, DefaultTextColour, DefaultTextColour, DefaultTextColour)
{
}

//----------------------------------------------------------------------------//
Font* ListboxTextItem::getFont() const
{
    return d_font ? d_font : d_owner ? d_owner->getEffectiveFont() : nullptr;
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
    d_renderedTextValid = false;
}

//----------------------------------------------------------------------------//
Sizef ListboxTextItem::getPixelSize() const
{
    if (d_textLogical.empty() || !getFont())
        return Sizef(0, 0);

    if (!d_renderedTextValid)
        parseTextString();

    return d_renderedText.getExtents();
}

//----------------------------------------------------------------------------//
void ListboxTextItem::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const Rectf& targetRect, float alpha, const Rectf* clipper) const
{
    // Draw selection brush
    if (d_selected && d_selectBrush)
    {
        ImageRenderSettings imgRenderSettings(targetRect, clipper, d_selectCols, alpha);
        d_selectBrush->createRenderGeometry(out, imgRenderSettings);
    }

    // Draw text
    if (const Font* font = getFont())
    {
        if (!d_renderedTextValid)
            parseTextString();

        d_renderedText.createRenderGeometry(out, targetRect.getPosition(), &d_textCols, clipper);
    }
}

//----------------------------------------------------------------------------//
void ListboxTextItem::parseTextString() const
{
    d_renderedText.renderText(d_textLogical, d_textParser, getFont(), DefaultParagraphDirection::LeftToRight);
    d_renderedText.setHorizontalFormatting(HorizontalTextFormatting::LeftAligned);
    d_renderedText.setWordWrapEnabled(false);
    d_renderedText.updateDynamicObjectExtents(d_owner);
    d_renderedText.updateFormatting(d_owner->getPixelSize().d_width);

    d_renderedTextValid = true;
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setText(const String& text)
{
    ListboxItem::setText(text);
    d_renderedTextValid = false;
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setTextColours(Colour top_left_colour,
                                     Colour top_right_colour,
                                     Colour bottom_left_colour,
                                     Colour bottom_right_colour)
{
    d_textCols.d_top_left = top_left_colour;
    d_textCols.d_top_right = top_right_colour;
    d_textCols.d_bottom_left = bottom_left_colour;
    d_textCols.d_bottom_right = bottom_right_colour;
    d_renderedTextValid = false;
}

//----------------------------------------------------------------------------//
// FIXME: now this silently resets a custom parser!
void ListboxTextItem::setTextParsingEnabled(bool enable)
{
    setCustomTextParser(enable ? CEGUI::System::getSingleton().getDefaultTextParser() : nullptr);
}

//----------------------------------------------------------------------------//
bool ListboxTextItem::isTextParsingEnabled() const
{
    return !!d_textParser;
}

//----------------------------------------------------------------------------//
void ListboxTextItem::setCustomTextParser(CEGUI::TextParser* parser)
{
    if (d_textParser == parser)
        return;

    d_textParser = parser;
    d_renderedTextValid = false;
}

//----------------------------------------------------------------------------//
bool ListboxTextItem::handleFontRenderSizeChange(const Font* const font)
{
    if (font == getFont())
    {
        d_renderedTextValid = false;
        return true;
    }

    return false;
}

}
