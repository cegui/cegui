/***********************************************************************
    filename:   CEGUIRenderedStringTextComponent.cpp
    created:    25/05/2009
    author:     Paul Turner
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIRenderedStringTextComponent.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUISystem.h"
#include "CEGUIExceptions.h"
#include "CEGUITextUtils.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
RenderedStringTextComponent::RenderedStringTextComponent() :
    d_font(0),
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF)
{
}

//----------------------------------------------------------------------------//
RenderedStringTextComponent::RenderedStringTextComponent(const String& text) :
    d_text(text),
    d_font(0),
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF)
{
}

//----------------------------------------------------------------------------//
RenderedStringTextComponent::RenderedStringTextComponent(
        const String& text, const String& font_name) :
    d_text(text),
    d_font(font_name.empty() ? 0 : &FontManager::getSingleton().get(font_name)),
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF)
{
}

//----------------------------------------------------------------------------//
RenderedStringTextComponent::RenderedStringTextComponent(const String& text,
                                                         Font* font) :
    d_text(text),
    d_font(font),
    d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF)
{
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::setText(const String& text)
{
    d_text = text;
}

//----------------------------------------------------------------------------//
const String& RenderedStringTextComponent::getText() const
{
    return d_text;
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::setFont(Font* font)
{
    d_font = font;
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::setFont(const String& font_name)
{
    d_font =
        font_name.empty() ? 0 : &FontManager::getSingleton().get(font_name);
}

//----------------------------------------------------------------------------//
Font* RenderedStringTextComponent::getFont() const
{
    return d_font;
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::setColours(const ColourRect& cr)
{
    d_colours = cr;
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::setColours(const Colour& c)
{
    d_colours.setColours(c);
}

//----------------------------------------------------------------------------//
const ColourRect& RenderedStringTextComponent::getColours() const
{
    return d_colours;
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::draw(GeometryBuffer& buffer,
                                       const Vector2<>& position,
                                       const ColourRect* mod_colours,
                                       const Rect* clip_rect,
                                       const float vertical_space,
                                       const float space_extra) const
{
    Font* fnt = d_font ? d_font : System::getSingleton().getDefaultFont();

    if (!fnt)
        return;

    Vector2<> final_pos(position);
    float y_scale = 1.0f;

    // handle formatting options
    switch (d_verticalFormatting)
    {
    case VF_BOTTOM_ALIGNED:
        final_pos.d_y += vertical_space - getPixelSize().d_height;
        break;

    case VF_CENTRE_ALIGNED:
        final_pos.d_y += (vertical_space - getPixelSize().d_height) / 2 ;
        break;

    case VF_STRETCHED:
        y_scale = vertical_space / getPixelSize().d_height;
        break;

    case VF_TOP_ALIGNED:
        // nothing additional to do for this formatting option.
        break;

    default:
        CEGUI_THROW(InvalidRequestException("RenderedStringTextComponent::draw: "
                "unknown VerticalFormatting option specified."));
    }

    // apply padding to position:
    final_pos += d_padding.getPosition();

    // apply modulative colours if needed.
    ColourRect final_cols(d_colours);
    if (mod_colours)
        final_cols *= *mod_colours;

    // draw the text string.
    fnt->drawText(buffer, d_text, final_pos, clip_rect, final_cols,
                  space_extra, 1.0f, y_scale);
}

//----------------------------------------------------------------------------//
Size RenderedStringTextComponent::getPixelSize() const
{
    Font* fnt = d_font ? d_font : System::getSingleton().getDefaultFont();

    Size psz(d_padding.d_left + d_padding.d_right,
             d_padding.d_top + d_padding.d_bottom);

    if (fnt)
    {
        psz.d_width += fnt->getTextExtent(d_text);
        psz.d_height += fnt->getFontHeight();
    }

    return psz;
}

//----------------------------------------------------------------------------//
bool RenderedStringTextComponent::canSplit() const
{
    return d_text.length() > 1;
}

//----------------------------------------------------------------------------//
RenderedStringTextComponent* RenderedStringTextComponent::split(
    float split_point, bool first_component)
{
    Font* fnt = d_font ? d_font : System::getSingleton().getDefaultFont();

    // This is checked, but should never fail, since if we had no font our
    // extent would be 0 and we would never cause a split to be needed here.
    if (!fnt)
        CEGUI_THROW(InvalidRequestException(
            "RenderedStringTextComponent::split: "
            "unable to split with no font set."));

    // create 'left' side of split and clone our basic configuration
    RenderedStringTextComponent* lhs = CEGUI_NEW_AO RenderedStringTextComponent();
    lhs->d_padding = d_padding;
    lhs->d_verticalFormatting = d_verticalFormatting;
    lhs->d_font = d_font;
    lhs->d_colours = d_colours;

    // calculate the 'best' place to split the text
    size_t left_len = 0;
    float left_extent = 0.0f;

    while (left_len < d_text.length())
    {
        size_t token_len = getNextTokenLength(d_text, left_len);
        // exit loop if no more valid tokens.
        if (token_len == 0)
            break;

        const float token_extent = 
            fnt->getTextExtent(d_text.substr(left_len, token_len));

        // does the next token extend past the split point?
        if (left_extent + token_extent > split_point)
        {
            // if it was the first token, split the token itself
            if (first_component && left_len == 0)
                left_len =
                    ceguimax(static_cast<size_t>(1),
                             fnt->getCharAtPixel(
                                d_text.substr(0, token_len), split_point));
            
            // left_len is now the character index at which to split the line
            break;
        }
        
        // add this token to the left side
        left_len += token_len;
        left_extent += token_extent;
    }
    
    // perform the split.
    lhs->d_text = d_text.substr(0, left_len);

    // here we're trimming leading delimiters from the substring range 
    size_t rhs_start =
        d_text.find_first_not_of(TextUtils::DefaultWrapDelimiters, left_len);
    if (rhs_start == String::npos)
        rhs_start = left_len;
    
    d_text = d_text.substr(rhs_start);

    return lhs;
}

//----------------------------------------------------------------------------//
size_t RenderedStringTextComponent::getNextTokenLength(const String& text,
                                                       size_t start_idx)
{
    String::size_type word_start =
        text.find_first_not_of(TextUtils::DefaultWrapDelimiters, start_idx);

    if (word_start == String::npos)
        word_start = start_idx;

    String::size_type word_end =
        text.find_first_of(TextUtils::DefaultWrapDelimiters, word_start);

    if (word_end == String::npos)
        word_end = text.length();

    return word_end - start_idx;
}

//----------------------------------------------------------------------------//
RenderedStringTextComponent* RenderedStringTextComponent::clone() const
{
    RenderedStringTextComponent* c = CEGUI_NEW_AO RenderedStringTextComponent(*this);
    return c;
}

//----------------------------------------------------------------------------//
size_t RenderedStringTextComponent::getSpaceCount() const
{
    // TODO: The value calculated here is a good candidate for caching.

    size_t space_count = 0;

    // Count the number of spaces in this component.
    // NB: here I'm not countng tabs since those are really intended to be
    // something other than just a bigger space.
    const size_t char_count = d_text.length();
    for (size_t c = 0; c < char_count; ++c)
        if (d_text[c] == ' ') // TODO: There are other space characters!
            ++space_count;

    return space_count;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
