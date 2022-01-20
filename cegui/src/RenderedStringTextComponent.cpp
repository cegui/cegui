/***********************************************************************
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
#include "CEGUI/RenderedStringTextComponent.h"
#include "CEGUI/Font.h"
#include "CEGUI/TextUtils.h"
#include "CEGUI/Window.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
RenderedStringTextComponent::RenderedStringTextComponent(const String& text, const Font* font)
    : d_text(text)
    , d_font(font)
{
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::setText(const String& text)
{
    d_text = text;

    // Count the number of spaces in this component.
    // NB: here I'm not counting tabs since those are really intended to be
    // something other than just a bigger space.
    // TODO: There are other space characters!
    d_spaceCount = std::count(d_text.cbegin(), d_text.cend(), ' ');
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::setSelection(const Window* refWnd,
                                               const float start, const float end)
{
    if (start >= end)
    {
        d_selectionStart = 0;
        d_selectionLength = 0;
        return;
    }

    const Font* font = getEffectiveFont(refWnd);
    d_selectionStart = font->getCharAtPixel(d_text, start);
    d_selectionLength = font->getCharAtPixel(d_text, end) - d_selectionStart;
}

//----------------------------------------------------------------------------//
const Font* RenderedStringTextComponent::getEffectiveFont(const Window* window) const
{
    return d_font ? d_font : window ? window->getActualFont() : nullptr;
}

//----------------------------------------------------------------------------//
void RenderedStringTextComponent::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const Window* refWnd, const glm::vec2& position, const ColourRect* modColours,
    const Rectf* clipRect, float verticalSpace, float spaceExtra) const
{
    const Font* font = getEffectiveFont(refWnd);
    if (!font)
        return;

    glm::vec2 penPosition = position + d_padding.getPosition();
    switch (d_verticalFormatting)
    {
        case VerticalImageFormatting::BottomAligned:
            penPosition.y += verticalSpace - getPixelSize(refWnd).d_height;
            break;
        case VerticalImageFormatting::CentreAligned:
            penPosition.y += (verticalSpace - getPixelSize(refWnd).d_height) / 2.f;
            break;
        // Otherwise default to TopAligned
        // TODO TEXT: Stretched
    }

    // Apply modulative colours if needed
    ColourRect finalColours(d_colours);
    if (modColours)
        finalColours *= *modColours;

    // Render selection background before text
    if (d_selectionImage && d_selectionLength)
    {
        const float selStartExtent = (d_selectionStart > 0) ? font->getTextExtent(d_text.substr(0, d_selectionStart)) : 0;
        const float selEndExtent = font->getTextExtent(d_text.substr(0, d_selectionStart + d_selectionLength));
        const Rectf selRect(position.x + selStartExtent, position.y, position.x + selEndExtent, position.y + verticalSpace);
        ImageRenderSettings imgRenderSettings(selRect, clipRect, ColourRect(0xFF002FFF));
        d_selectionImage->createRenderGeometry(out, imgRenderSettings);
    }

    // Create the geometry for rendering for the given text
    font->createTextRenderGeometry(out, d_text, penPosition, clipRect, finalColours, d_defaultParagraphDir, spaceExtra);
}

//----------------------------------------------------------------------------//
Sizef RenderedStringTextComponent::getPixelSize(const Window* refWnd) const
{
    Sizef psz(d_padding.d_min.x + d_padding.d_max.x,
              d_padding.d_min.y + d_padding.d_max.y);

    if (const Font* font = getEffectiveFont(refWnd))
    {
        psz.d_width += font->getTextExtent(d_text);
        psz.d_height += font->getFontHeight();
    }

    return psz;
}

//----------------------------------------------------------------------------//
RenderedStringComponentPtr RenderedStringTextComponent::split(
    const Window* refWnd, float split_point, bool first_component, bool& was_word_split)
{
    const Font* font = getEffectiveFont(refWnd);

    // This is checked, but should never fail, since if we had no font our
    // extent would be 0 and we would never cause a split to be needed here.
    if (!font)
        throw InvalidRequestException("unable to split with no font set.");

    was_word_split = false;

    // create 'left' side of split and clone our basic configuration
    auto lhs = std::make_unique<RenderedStringTextComponent>();
    lhs->d_padding = d_padding;
    lhs->d_verticalFormatting = d_verticalFormatting;
    lhs->d_font = d_font;
    lhs->d_colours = d_colours;

    // calculate the 'best' place to split the text
    size_t left_len = 0;
    float left_extent = 0.0f;

    while (left_len < d_text.length())
    {
        auto word_start = d_text.find_first_not_of(TextUtils::DefaultWrapDelimiters, left_len);
        if (word_start == String::npos)
            word_start = left_len;

        auto word_end = d_text.find_first_of(TextUtils::DefaultWrapDelimiters, word_start);
        if (word_end == String::npos)
            word_end = d_text.length();

        const size_t token_len = word_end - left_len;

        // exit loop if no more valid tokens.
        if (token_len == 0)
            break;

        const float token_extent =  font->getTextExtent(d_text.substr(left_len, token_len));

        // does the next token extend past the split point?
        if (left_extent + token_extent > split_point)
        {
            // if it was the first token, split the token itself
            if (first_component && left_len == 0)
            {
                was_word_split = true;
                left_len =
                    std::max(static_cast<size_t>(1),
                             font->getCharAtPixel(
                                 d_text.substr(0, token_len), split_point));
            }
            
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
    size_t rhs_start = d_text.find_first_not_of(TextUtils::DefaultWrapDelimiters, left_len);
    if (rhs_start == String::npos)
        rhs_start = left_len;

    // split the selection
    if (d_selectionLength)
    {
        const size_t sel_end = d_selectionStart + d_selectionLength - 1;
        lhs->d_selectionStart = d_selectionStart;
        lhs->d_selectionLength = sel_end < left_len ? d_selectionLength : left_len - d_selectionStart;

        d_selectionStart = 0;
        if (sel_end >= left_len)
            d_selectionLength -= rhs_start;
        else
            d_selectionLength = 0;
    }

    setText(d_text.substr(rhs_start));

    return lhs;
}

//----------------------------------------------------------------------------//
RenderedStringComponentPtr RenderedStringTextComponent::clone() const
{
    return std::make_unique<RenderedStringTextComponent>(*this);
}

}
