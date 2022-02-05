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
#include "CEGUI/text/Font.h"
#include "CEGUI/text/BidiVisualMapping.h"
#include "CEGUI/TplWindowRendererProperty.h"
#include <stdio.h>

 //!!!DBG TMP!
#include "CEGUI/ImageManager.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String FalagardEditbox::TypeName("Core/Editbox");
const String FalagardEditbox::UnselectedTextColourPropertyName("NormalTextColour");
const String FalagardEditbox::SelectedTextColourPropertyName("SelectedTextColour");
const String FalagardEditbox::ActiveSelectionColourPropertyName("ActiveSelectionColour");
const String FalagardEditbox::InactiveSelectionColourPropertyName("InactiveSelectionColour");
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

    String visualText;
    setupVisualString(visualText);

    Editbox* const w = static_cast<Editbox*>(d_window);
    d_renderedText.renderText(visualText, nullptr, w->getActualFont(), w->getDefaultParagraphDirection());

    const Rectf textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*d_window));

    d_renderedText.setHorizontalFormatting(d_textFormatting);
    d_renderedText.setWordWrappingEnabled(false);
    //d_renderedText.updateDynamicObjectExtents(w); // no parsing implies no dynamic objects
    d_renderedText.updateFormatting(textArea.getWidth());

    const float textExtent = d_renderedText.getExtents().d_height;
    const ImagerySection& caretImagery = wlf.getImagerySection("Caret");
    const float caretWidth = caretImagery.getBoundingRect(*d_window, textArea).getWidth();
    const float extentToCaretVisual = d_renderedText.getCodepointBounds(w->getCaretIndex()).left();

    //!!!TODO TEXT:
    // - BIDI caret side choosing
    // - draw caret after the text if request caretIndex > text length / glyph count
    // - if getGlyphIndex can't find one, return nearest?

    float extentToCaretLogical = extentToCaretVisual;
    switch (d_textFormatting)
    {
        case HorizontalTextFormatting::CentreAligned:
            extentToCaretLogical = (textExtent - caretWidth) / 2.f;
            break;
        case HorizontalTextFormatting::RightAligned:
            extentToCaretLogical = extentToCaretVisual - caretWidth;
            break;
    }

    // Update text offset if caret is to the left or to the right of the box
    if ((d_textOffset + extentToCaretLogical) < 0)
        d_textOffset = -extentToCaretLogical;
    else if ((d_textOffset + extentToCaretLogical) >= (textArea.getWidth() - caretWidth))
        d_textOffset = textArea.getWidth() - extentToCaretLogical - caretWidth;

    const float textOffsetVisual = getTextOffsetVisual(textArea, textExtent);

    createRenderGeometryForText(wlf, visualText, textArea, textOffsetVisual);

    // Create the render geometry for the caret
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
    const String& text, const Rectf& textArea, float textOffset)
{
    if (d_renderedText.empty())
        return;

    // Scroll text to the visible part and center it vertically inside the area
    Rectf textPartRect = textArea;
    textPartRect.offset(glm::vec2(textOffset, (textArea.getHeight() - d_renderedText.getExtents().d_height) * 0.5f));

    const ColourRect normalTextCol = getOptionalColour(UnselectedTextColourPropertyName);

    Editbox* const w = static_cast<Editbox*>(d_window);
    const size_t selStart = w->getSelectionStart();
    const size_t selEnd = w->getSelectionEnd();
    SelectionInfo* selection = nullptr;
    SelectionInfo selectionInfo;
    if (selStart < selEnd)
    {
        //const auto& selectBrushImagery = wlf.getStateImagery(w->hasInputFocus() ? "ActiveSelection" : "InactiveSelection");
        //!!!DBG TMP!
        if (ImageManager::getSingleton().isDefined("TaharezLook/GenericBrush"))
            selectionInfo.bgBrush = &ImageManager::getSingleton().get("TaharezLook/GenericBrush");

        selectionInfo.bgColours = getOptionalColour(
            w->hasInputFocus() ? ActiveSelectionColourPropertyName : InactiveSelectionColourPropertyName);
        selectionInfo.textColours = getOptionalColour(SelectedTextColourPropertyName);
        selectionInfo.start = selStart;
        selectionInfo.end = selEnd;
        selection = &selectionInfo;
    }

    d_renderedText.createRenderGeometry(w->getGeometryBuffers(), textPartRect.getPosition(), &normalTextCol, &textArea, selection);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderCaret(const ImagerySection& imagery,
                                  const Rectf& textArea,
                                  float textOffset,
                                  float extent_to_caret) const
{
    Rectf caretRect(textArea);
    caretRect.d_min.x += extent_to_caret + textOffset;
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
