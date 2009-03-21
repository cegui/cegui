/***********************************************************************
    filename:   FalEditbox.cpp
    created:    Sat Jun 25 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "FalEditbox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUICoordConverter.h"
#include "CEGUIFont.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const utf8 FalagardEditbox::TypeName[] = "Falagard/Editbox";
const String FalagardEditbox::UnselectedTextColourPropertyName("NormalTextColour");
const String FalagardEditbox::SelectedTextColourPropertyName("SelectedTextColour");

FalagardEditbox::FalagardEditbox(const String& type) :
        EditboxWindowRenderer(type),
        d_lastTextOffset(0)
{
}

void FalagardEditbox::render()
{
    Editbox* w = (Editbox*)d_window;
    const StateImagery* imagery;

    // draw container etc
    // get WidgetLookFeel for the assigned look.
    const WidgetLookFeel& wlf = getLookNFeel();
    // try and get imagery for the approprite state.
    imagery = &wlf.getStateImagery(w->isDisabled() ? "Disabled" : (w->isReadOnly() ? "ReadOnly" : "Enabled"));

    // peform the rendering operation for the container.
    imagery->render(*w);

    // get destination area for text
    const Rect textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*w));

    //
    // Required preliminary work for text rendering operations
    //
    Font* font = w->getFont();

    // no font == no more rendering
    if (!font)
        return;

    // This will point to the final string to be used for rendering.  Useful because it means we
    // do not have to have duplicate code or be copying getText() for handling masked/unmasked text.
    String* editText;

    // Create a 'masked' version of the string if needed.
    String maskedText, windowText;
    if (w->isTextMasked())
    {
        maskedText.insert(0, w->getText().length(), w->getMaskCodePoint());
        editText = &maskedText;
    }
    // text not masked to editText will be the windows getText() String.
    else
    {
#ifdef CEGUI_BIDI_SUPPORT
        windowText = w->getTextVisual();
#else
        windowText = w->getText();
#endif
        editText = &windowText;
    }

    // calculate best position to render text to ensure carat is always visible
    float textOffset;
    size_t cartIndex = w->getCaratIndex();

#ifdef CEGUI_BIDI_SUPPORT
    // the char before the cart bidi type
    bool currCharIsRtl = false;
    if ((editText->size() > 0) && (cartIndex > 0))
    {
        size_t curCartIndex = w->getCaratIndex();
        TextUtils::BidiCharType charBeforeCartType = TextUtils::getBidiCharType((*editText)[curCartIndex - 1]);
        // for neutral chars you decide by the char after
        for (; TextUtils::bctNeutral == charBeforeCartType && (editText->size() > curCartIndex) ; curCartIndex++)
        {
            charBeforeCartType = TextUtils::getBidiCharType((*editText)[curCartIndex - 1]);

        }

        currCharIsRtl  = (TextUtils::bctRightToLeft == charBeforeCartType);
    }

    bool isFirstChar = cartIndex == 0;

    // the pos is by the char before
    if (!isFirstChar)
    {
        cartIndex--;
    }

    // we need to find the cart pos by the logical to visual map
    if (w->getV2lMapping().size() > cartIndex)
    {
        cartIndex = w->getL2vMapping()[cartIndex];
    }

    // for non RTL char - the cart pos is after the char
    if (!currCharIsRtl)
    {
        cartIndex++;
    }

    // if first char is not rtl - we need to stand at the start of the line
    if (isFirstChar)
    {
        bool firstCharRtl = (editText->size() > 0) && (TextUtils::bctRightToLeft == TextUtils::getBidiCharType((*editText)[0]));
        if (!firstCharRtl)
        {
            cartIndex--;
        }
    }
#endif

    float extentToCarat = font->getTextExtent(editText->substr(0, cartIndex));

    // get carat imagery
    const ImagerySection& caratImagery = wlf.getImagerySection("Carat");
    // store carat width
    float caratWidth = caratImagery.getBoundingRect(*w, textArea).getWidth();

    // if box is inactive
    if (!w->hasInputFocus())
    {
        textOffset = d_lastTextOffset;
    }
    // if carat is to the left of the box
    else if ((d_lastTextOffset + extentToCarat) < 0)
    {
        textOffset = -extentToCarat;
    }
    // if carat is off to the right.
    else if ((d_lastTextOffset + extentToCarat) >= (textArea.getWidth() - caratWidth))
    {
        textOffset = textArea.getWidth() - extentToCarat - caratWidth;
    }
    // else carat is already within the box
    else
    {
        textOffset = d_lastTextOffset;
    }

    ColourRect colours;
    float alpha_comp = w->getEffectiveAlpha();

    //
    // Draw label text
    //
    // setup initial rect for text formatting
    Rect text_part_rect(textArea);
    // allow for scroll position
    text_part_rect.d_left += textOffset;
    // centre text vertically within the defined text area
    text_part_rect.d_top += (textArea.getHeight() - font->getFontHeight()) * 0.5f;

    // get unhighlighted text colour (saves accessing property twice)
    colour unselectedColour(getUnselectedTextColour());

    // see if the editbox is active or inactive.
    bool active = (!w->isReadOnly()) && w->hasInputFocus();

#ifdef CEGUI_BIDI_SUPPORT
    if (w->getSelectionLength() == 0)
    {
        // no highlighted text - we can draw the whole thing
        colours.setColours(unselectedColour);
        colours.modulateAlpha(alpha_comp);
        font->drawText(w->getGeometryBuffer(), *editText, text_part_rect, &textArea, LeftAligned, colours);

        // adjust rect for next section
        text_part_rect.d_left += font->getTextExtent(*editText);

    }
    else
    {
        // there is highlighted text - because of the BiDi support - the highlighted
        // can be in some cases nonconsecutive.
        // So - we need to draw it char by char (I guess we can optimize it more
        // but this is not that big performance hit because it only happens if
        // we have highlighted text - not that common...)
        for (size_t i = 0 ; i < editText->size() ; i++)
        {
            // get the char
            String currChar = editText->substr(i, 1);
            size_t realPos = 0;

            // get he visual pos of the char
            if (w->getV2lMapping().size() > i)
            {
                realPos = w->getV2lMapping()[i];
            }

            // check if it is in the highlighted region
            bool highlighted = realPos >= w->getSelectionStartIndex() && realPos < w->getSelectionStartIndex() + w->getSelectionLength();

            float charAdvance = font->getGlyphData(currChar[0])->getAdvance(1.0f);

            if (highlighted)
            {
                colours.setColours(getSelectedTextColour());
                colours.modulateAlpha(alpha_comp);

                {

                    // calculate area for selection imagery.
                    Rect hlarea(textArea);
                    hlarea.d_left = text_part_rect.d_left ;
                    hlarea.d_right = text_part_rect.d_left + charAdvance ;

                    // render the selection imagery.
                    wlf.getStateImagery(active ? "ActiveSelection" : "InactiveSelection").render(*w, hlarea, 0, &textArea);
                }

            }
            else
            {
                colours.setColours(unselectedColour);
                colours.modulateAlpha(alpha_comp);
            }
            font->drawText(w->getGeometryBuffer(), currChar, text_part_rect, &textArea, LeftAligned, colours);

            // adjust rect for next section
            text_part_rect.d_left += charAdvance;

        }
    }
#else
    //
    // Render selection imagery.
    //
    if (w->getSelectionLength() != 0)
    {
        // calculate required start and end offsets of selection imagery.
        float selStartOffset = font->getTextExtent(editText->substr(0, w->getSelectionStartIndex()));
        float selEndOffset   = font->getTextExtent(editText->substr(0, w->getSelectionEndIndex()));

        // calculate area for selection imagery.
        Rect hlarea(textArea);
        hlarea.d_left += textOffset + selStartOffset;
        hlarea.d_right = hlarea.d_left + (selEndOffset - selStartOffset);

        // render the selection imagery.
        wlf.getStateImagery(active ? "ActiveSelection" : "InactiveSelection").render(*w, hlarea, 0, &textArea);
    }

    // draw pre-highlight text
    String sect = editText->substr(0, w->getSelectionStartIndex());
    colours.setColours(unselectedColour);
    colours.modulateAlpha(alpha_comp);
    font->drawText(w->getGeometryBuffer(), sect, text_part_rect, &textArea, LeftAligned, colours);

    // adjust rect for next section
    text_part_rect.d_left += font->getTextExtent(sect);

    // draw highlight text
    sect = editText->substr(w->getSelectionStartIndex(), w->getSelectionLength());
    colours.setColours(getSelectedTextColour());
    colours.modulateAlpha(alpha_comp);
    font->drawText(w->getGeometryBuffer(), sect, text_part_rect, &textArea, LeftAligned, colours);

    // adjust rect for next section
    text_part_rect.d_left += font->getTextExtent(sect);

    // draw post-highlight text
    sect = editText->substr(w->getSelectionEndIndex());
    colours.setColours(unselectedColour);
    colours.modulateAlpha(alpha_comp);
    font->drawText(w->getGeometryBuffer(), sect, text_part_rect, &textArea, LeftAligned, colours);

    // remember this for next time.
    d_lastTextOffset = textOffset;
#endif

    //
    // Render carat
    //
    if (active)
    {
        Rect caratRect(textArea);
        caratRect.d_left += extentToCarat + textOffset;

        caratImagery.render(*w, caratRect, 0, &textArea);
    }
}

size_t FalagardEditbox::getTextIndexFromPosition(const Point& pt) const
{
    Editbox* w = (Editbox*)d_window;
    //
    // calculate final window position to be checked
    //
    float wndx = CoordConverter::screenToWindowX(*w, pt.d_x);

    wndx -= d_lastTextOffset;

    //
    // Return the proper index
    //
    if (w->isTextMasked())
    {
#ifdef CEGUI_BIDI_SUPPORT
        return w->getFont()->getCharAtPixel(String(w->getTextVisual().length(), w->getMaskCodePoint()), wndx);
#else
        return w->getFont()->getCharAtPixel(String(w->getText().length(), w->getMaskCodePoint()), wndx);
#endif
    }
    else
    {
#ifdef CEGUI_BIDI_SUPPORT
        return w->getFont()->getCharAtPixel(w->getTextVisual(), wndx);
#else
        return w->getFont()->getCharAtPixel(w->getText(), wndx);
#endif
    }
}

colour FalagardEditbox::getOptionalPropertyColour(const String& propertyName) const
{
    if (d_window->isPropertyPresent(propertyName))
        return PropertyHelper::stringToColour(d_window->getProperty(propertyName));
    else
        return colour(0, 0, 0);
}

colour FalagardEditbox::getUnselectedTextColour() const
{
    return getOptionalPropertyColour(UnselectedTextColourPropertyName);
}

colour FalagardEditbox::getSelectedTextColour() const
{
    return getOptionalPropertyColour(SelectedTextColourPropertyName);
}

} // End of  CEGUI namespace section
