/***********************************************************************
    created:    Sat Jun 25 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/WindowRendererSets/Core/Editbox.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/Font.h"
#ifdef CEGUI_BIDI_SUPPORT
#include "CEGUI/FontGlyph.h" // FIXME: for getAdvance() in old code. Maybe now should be solved differently!
#endif
#include "CEGUI/BidiVisualMapping.h"
#include "CEGUI/TplWindowRendererProperty.h"
#include <stdio.h>

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String FalagardEditbox::TypeName("Core/Editbox");
const String FalagardEditbox::UnselectedTextColourPropertyName( "NormalTextColour" );
const String FalagardEditbox::SelectedTextColourPropertyName( "SelectedTextColour" );
const String FalagardEditbox::ActiveSelectionColourPropertyName( "ActiveSelectionColour" );
const String FalagardEditbox::InactiveSelectionColourPropertyName( "InactiveSelectionColour" );
const float FalagardEditbox::DefaultCaretBlinkTimeout(0.66f);

//----------------------------------------------------------------------------//
FalagardEditbox::FalagardEditbox(const String& type) :
    EditboxWindowRenderer(type)
{
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardEditbox, bool,
        "BlinkCaret", "Property to get/set whether the Editbox caret should blink.  "
        "Value is either \"true\" or \"false\".",
        &FalagardEditbox::setCaretBlinkEnabled, &FalagardEditbox::isCaretBlinkEnabled,
        false);
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardEditbox, float,
        "BlinkCaretTimeout", "Property to get/set the caret blink timeout / speed.  "
        "Value is a float value indicating the timeout in seconds.",
        &FalagardEditbox::setCaretBlinkTimeout, &FalagardEditbox::getCaretBlinkTimeout,
        DefaultCaretBlinkTimeout);
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardEditbox, HorizontalTextFormatting,
        "TextFormatting", "Property to get/set the horizontal formatting mode. "
        "Value is one of: LeftAligned, RightAligned or HorzCentred",
        &FalagardEditbox::setTextFormatting, &FalagardEditbox::getTextFormatting,
        HorizontalTextFormatting::LeftAligned);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::createRenderGeometry()
{
    const WidgetLookFeel& wlf = getLookNFeel();

    renderBaseImagery(wlf);

    const Font* font = d_window->getActualFont();
    if (!font)
        return;

    String visualText;
    setupVisualString(visualText);

    // NB: this mess really should be calculated before drawing both text and caret
    const Rectf textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*d_window));
    const float textExtent = font->getTextExtent(visualText);
    const size_t caret_index = getCaretIndex(visualText);
    const ImagerySection& caretImagery = wlf.getImagerySection("Caret");
    const float caret_width = caretImagery.getBoundingRect(*d_window, textArea).getWidth();
    const float extentToCaretVisual = font->getTextAdvance(visualText.substr(0, caret_index));
    const float extentToCaretLogical = getExtentToCaretLogical(extentToCaretVisual, textExtent, caret_width);

    // Update text offset if caret is to the left or to the right of the box
    if ((d_textOffset + extentToCaretLogical) < 0)
        d_textOffset = -extentToCaretLogical;
    else if ((d_textOffset + extentToCaretLogical) >= (textArea.getWidth() - caret_width))
        d_textOffset = textArea.getWidth() - extentToCaretLogical - caret_width;

    const float textOffsetVisual = getTextOffsetVisual(textArea, textExtent);

    createRenderGeometryForText(wlf, visualText, textArea, textOffsetVisual);

    // Create the render geometry for the caret
    auto w = static_cast<const Editbox*>(d_window);
    if (w->hasInputFocus() && !w->isReadOnly() && (!d_blinkCaret || d_showCaret))
        renderCaret(caretImagery, textArea, textOffsetVisual, extentToCaretVisual);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderBaseImagery(const WidgetLookFeel& wlf) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    String state;

    if (w->isEffectiveDisabled())
        state = "Disabled";
    else
    {
        if (w->isReadOnly())
            state = "ReadOnly";
        else
            state = "Enabled";

        if (w->isFocused())
            state += "Focused";
    }

    wlf.getStateImagery(state).render(*w);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setupVisualString(String& visual) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    if (w->isTextMaskingEnabled())
    {
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII) 
        visual.assign(w->getText().length(), static_cast<String::value_type>(w->getTextMaskingCodepoint()));
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) 
        visual.assign(w->getText().length(), static_cast<char32_t>(w->getTextMaskingCodepoint()));
#endif
    }
    else
    {
        visual.assign(w->getTextVisual());
    }
}

//----------------------------------------------------------------------------//
size_t FalagardEditbox::getCaretIndex(const String& visual_text) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    size_t caretIndex = w->getCaretIndex();

#ifdef CEGUI_BIDI_SUPPORT
    // the char before the caret bidi type
    bool currCharIsRtl = false;
    if (!visual_text.empty() && caretIndex > 0)
    {
        size_t curCaretIndex = w->getCaretIndex();
        auto charBeforeCaretType = w->getBidiVisualMapping()->getBidiCharType(visual_text[curCaretIndex - 1]);
        // for neutral chars you decide by the char after
        for (; BidiCharType::NEUTRAL == charBeforeCaretType && (visual_text.size() > curCaretIndex); curCaretIndex++)
            charBeforeCaretType = w->getBidiVisualMapping()->getBidiCharType(visual_text[curCaretIndex - 1]);

        currCharIsRtl = (BidiCharType::RIGHT_TO_LEFT == charBeforeCaretType);
    }

    const bool isFirstChar = (caretIndex == 0);

    // the pos is by the char before
    if (!isFirstChar)
        --caretIndex;

    // we need to find the caret pos by the logical to visual map
    if (w->getBidiVisualMapping()->getV2lMapping().size() > caretIndex)
        caretIndex = w->getBidiVisualMapping()->getL2vMapping()[caretIndex];

    // for non RTL char - the caret pos is after the char
    if (!currCharIsRtl)
        ++caretIndex;

    // if first char is not rtl - we need to stand at the start of the line
    if (isFirstChar)
    {
        const bool firstCharRtl = !visual_text.empty() &&
            (BidiCharType::RIGHT_TO_LEFT == w->getBidiVisualMapping()->getBidiCharType(visual_text[0]));
        if (!firstCharRtl)
            --caretIndex;
    }
#else
    CEGUI_UNUSED(visual_text);
#endif

    return caretIndex;
}

//----------------------------------------------------------------------------//
float FalagardEditbox::getExtentToCaretLogical(float extentToCaretVisual,
    float textExtent, float caretWidth) const
{
    switch (d_textFormatting)
    {
        case HorizontalTextFormatting::LeftAligned:
            return extentToCaretVisual;
        case HorizontalTextFormatting::CentreAligned:
            return (textExtent - caretWidth) / 2.f;
        case HorizontalTextFormatting::RightAligned:
            return textExtent - extentToCaretVisual - caretWidth;
        default:
            throw InvalidRequestException("Invalid horizontal text formatting.");
    }
}

//----------------------------------------------------------------------------//
float FalagardEditbox::getTextOffsetVisual(const Rectf& textArea, float textExtent) const
{
    switch (d_textFormatting)
    {
        case HorizontalTextFormatting::LeftAligned:
            return d_textOffset;
        case HorizontalTextFormatting::CentreAligned:
            return (textArea.getWidth() - textExtent) / 2.f;
        case HorizontalTextFormatting::RightAligned:
            return textArea.getWidth() - d_textOffset - textExtent;
        default:
            throw InvalidRequestException("Invalid horizontal text formatting.");
    } 
}

//----------------------------------------------------------------------------//
void FalagardEditbox::createRenderGeometryForText(const WidgetLookFeel& wlf,
    const String& text, const Rectf& textArea, float text_offset)
{
    Editbox* const w = static_cast<Editbox*>(d_window);

    const Font* const font = w->getActualFont();
    if (!font)
        return;

    const bool active = w->hasInputFocus();

    // setup initial rect for text formatting
    Rectf textPartRect(textArea);
    // allow for scroll position
    textPartRect.d_min.x += text_offset;
    // centre text vertically within the defined text area
    textPartRect.d_min.y += (textArea.getHeight() - font->getFontHeight()) * 0.5f;

    const ColourRect normalTextCol = getOptionalColour(UnselectedTextColourPropertyName);
    const ColourRect selectTextCol = getOptionalColour(SelectedTextColourPropertyName);

#ifdef CEGUI_BIDI_SUPPORT

    if (w->getSelectionLength() == 0)
    {
        // No highlighted text - we can draw the whole thing
        font->createTextRenderGeometry(w->getGeometryBuffers(),
            text, textPartRect.d_min.x,
            textPartRect.getPosition(),
            &textArea, true, normalTextCol, w->getDefaultParagraphDirection());
    }
    else
    {
        // There is highlighted text - because of the Bidi support - the
        // highlighted area can be in some cases nonconsecutive.
        // So - we need to draw it char by char (I guess we can optimize it more
        // but this is not that big performance hit because it only happens if
        // we have highlighted text - not that common...)
        for (size_t i = 0 ; i < text.size() ; i++)
        {
            // get the char
            String currChar = text.substr(i, 1);
            size_t realPos = 0;

            // get he visual pos of the char
            if (w->getBidiVisualMapping()->getV2lMapping().size() > i)
            {
                realPos = w->getBidiVisualMapping()->getV2lMapping()[i];
            }

            // check if it is in the highlighted region
            bool highlighted =
                realPos >= w->getSelectionStart() &&
                realPos < w->getSelectionStart() + w->getSelectionLength();

            float charAdvance = font->getGlyphForCodepoint(currChar[0])->getAdvance();

            if (highlighted)
            {
                // calculate area for selection imagery.
                Rectf hlarea(textArea);
                hlarea.d_min.x = textPartRect.d_min.x ;
                hlarea.d_max.x = textPartRect.d_min.x + charAdvance;

                // render the selection imagery.
                wlf.getStateImagery(active ? "ActiveSelection" : "InactiveSelection").
                    render(*w, hlarea, 0, &textArea);
            }

            font->createTextRenderGeometry(w->getGeometryBuffers(),
                currChar, textPartRect.d_min.x, textPartRect.getPosition(),
                &textArea, true, highlighted ? selectTextCol : normalTextCol, w->getDefaultParagraphDirection());

            // adjust rect for next section
            textPartRect.d_min.x += charAdvance;
        }
    }

#else // no CEGUI_BIDI_SUPPORT

    if (w->getSelectionLength() != 0)
    {
        // calculate required start and end offsets of selection imagery.
        float selStartOffset =
            font->getTextAdvance(text.substr(0, w->getSelectionStart()));
        float selEndOffset =
            font->getTextAdvance(text.substr(0, w->getSelectionEnd()));

        // calculate area for selection imagery.
        Rectf hlarea(textArea);
        hlarea.d_min.x += text_offset + selStartOffset;
        hlarea.d_max.x = hlarea.d_min.x + (selEndOffset - selStartOffset);

        // create render geometry for the selection imagery.
        const String& stateName = active ? "ActiveSelection" : "InactiveSelection";
        wlf.getStateImagery(stateName).render(*w, hlarea, nullptr, &textArea);
    }

    // create render geometry for pre-highlight text
    String sect = text.substr(0, w->getSelectionStart());
    colours = unselectedColours;

    auto preHighlightTextGeomBuffers = font->createTextRenderGeometry(
        sect, textPartRect.d_min.x,
        textPartRect.getPosition(),
        &textArea, true, colours, w->getDefaultParagraphDirection());

    w->appendGeometryBuffers(preHighlightTextGeomBuffers);

    // create render geometry for highlight text
    sect = text.substr(w->getSelectionStart(), w->getSelectionLength());
    setColourRectToSelectedTextColour(colours);

    auto highlitTextGeomBuffers = font->createTextRenderGeometry(
        sect, textPartRect.d_min.x, textPartRect.getPosition(),
        &textArea, true, colours, w->getDefaultParagraphDirection());

    w->appendGeometryBuffers(highlitTextGeomBuffers);

    // create render geometry for  post-highlight text
    sect = text.substr(w->getSelectionEnd());
    colours = unselectedColours;

    auto postHighlitTextGeomBuffers = font->createTextRenderGeometry(
        sect, textPartRect.d_min.x, textPartRect.getPosition(),
        &textArea, true, colours, w->getDefaultParagraphDirection());

    w->appendGeometryBuffers(postHighlitTextGeomBuffers);

#endif
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderCaret(const ImagerySection& imagery,
                                  const Rectf& textArea,
                                  float text_offset,
                                  float extent_to_caret) const
{
    Rectf caretRect(textArea);
    caretRect.d_min.x += extent_to_caret + text_offset;
    imagery.render(*d_window, caretRect, nullptr, &textArea);
}

//----------------------------------------------------------------------------//
size_t FalagardEditbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
    Editbox* w = static_cast<Editbox*>(d_window);
    const Font* font = w->getActualFont();
    if (!font)
        return w->getText().length();

    String visualText;
    setupVisualString(visualText);

    const Rectf textArea(getLookNFeel().getNamedArea("TextArea").getArea().getPixelRect(*d_window));
    const float textExtent = font->getTextExtent(visualText);
    const float wndx = CoordConverter::screenToWindowX(*w, pt.x) - getTextOffsetVisual(textArea, textExtent);
    return w->getActualFont()->getCharAtPixel(visualText, wndx);
}

//----------------------------------------------------------------------------//
ColourRect FalagardEditbox::getOptionalColour(const String& propertyName) const
{
    ColourRect rect(0);
    if (d_window->isPropertyPresent(propertyName))
        rect = d_window->getProperty<ColourRect>(propertyName);
    return rect;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::update(float elapsed)
{
    WindowRenderer::update(elapsed);

    // Only do the update if we absolutely have to
    auto w = static_cast<const Editbox*>(d_window);
    if (d_blinkCaret && !w->isReadOnly() && w->hasInputFocus())
    {
        d_caretBlinkElapsed += elapsed;
        if (d_caretBlinkElapsed > d_caretBlinkTimeout)
        {
            d_caretBlinkElapsed = 0.f;
            d_showCaret = !d_showCaret;

            // State changed, so need a redraw
            d_window->invalidate();
        }
    }
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setTextFormatting(const HorizontalTextFormatting format)
{
    if (isUnsupportedFormat(format))
        throw InvalidRequestException(
            "currently only HorizontalTextFormatting::LeftAligned, "
            "HorizontalTextFormatting::RightAligned and "
            "HorizontalTextFormatting::CentreAligned are accepted for Editbox formatting");

    d_textFormatting = format;
    d_window->invalidate();
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::isUnsupportedFormat(const HorizontalTextFormatting format) const
{
    return !(format == HorizontalTextFormatting::LeftAligned ||
             format == HorizontalTextFormatting::RightAligned ||
             format == HorizontalTextFormatting::CentreAligned);
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::handleFontRenderSizeChange(const Font* const font)
{
    const bool res = WindowRenderer::handleFontRenderSizeChange(font);

    if (d_window->getActualFont() == font)
    {
        d_window->invalidate();
        return true;
    }

    return res;
}

}
