/***********************************************************************
    created:    Thu Jul 7 2005
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
#include "CEGUI/WindowRendererSets/Core/MultiLineEditbox.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/Image.h"
#include "CEGUI/Font.h"
#include "CEGUI/TplWindowRendererProperty.h"

 //!!!FIXME TEXT: UTF-8 selection / caret positioning is broken, sometimes it breaks a multibyte codepoint in the middle

namespace CEGUI
{
const String FalagardMultiLineEditbox::TypeName("Core/MultiLineEditbox");
const String FalagardMultiLineEditbox::UnselectedTextColourPropertyName("NormalTextColour");
const String FalagardMultiLineEditbox::SelectedTextColourPropertyName("SelectedTextColour");
const String FalagardMultiLineEditbox::ActiveSelectionColourPropertyName("ActiveSelectionColour");
const String FalagardMultiLineEditbox::InactiveSelectionColourPropertyName("InactiveSelectionColour");
const float FalagardMultiLineEditbox::DefaultCaretBlinkTimeout(0.66f);

//----------------------------------------------------------------------------//
FalagardMultiLineEditbox::FalagardMultiLineEditbox(const String& type) :
    MultiLineEditboxWindowRenderer(type)
{
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardMultiLineEditbox,bool,
        "BlinkCaret", "Property to get/set whether the Editbox caret should blink.  "
        "Value is either \"true\" or \"false\".",
        &FalagardMultiLineEditbox::setCaretBlinkEnabled,&FalagardMultiLineEditbox::isCaretBlinkEnabled,
        false);
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardMultiLineEditbox,float,
        "BlinkCaretTimeout", "Property to get/set the caret blink timeout / speed.  "
        "Value is a float value indicating the timeout in seconds.",
        &FalagardMultiLineEditbox::setCaretBlinkTimeout,&FalagardMultiLineEditbox::getCaretBlinkTimeout,
        0.66f);
}

//----------------------------------------------------------------------------//
Rectf FalagardMultiLineEditbox::getTextRenderArea() const
{
    MultiLineEditbox* w = static_cast<MultiLineEditbox*>(d_window);
    const WidgetLookFeel& wlf = getLookNFeel();
    const bool v_visible = w->getVertScrollbar()->isVisible();
    const bool h_visible = w->getHorzScrollbar()->isVisible();

    // if either of the scrollbars are visible, we might want to use another text rendering area
    if (v_visible || h_visible)
    {
        String areaName("TextArea");

        if (h_visible)
            areaName += "H";
        if (v_visible)
            areaName += "V";
        areaName += "Scroll";

        if (wlf.isNamedAreaPresent(areaName))
            return wlf.getNamedArea(areaName).getArea().getPixelRect(*w);
    }

    // default to plain TextArea
    return wlf.getNamedArea("TextArea").getArea().getPixelRect(*w);
}

//----------------------------------------------------------------------------//
void FalagardMultiLineEditbox::cacheEditboxBaseImagery()
{
    MultiLineEditbox* w = static_cast<MultiLineEditbox*>(d_window);

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

    getLookNFeel().getStateImagery(state).render(*w);
}

//----------------------------------------------------------------------------//
void FalagardMultiLineEditbox::cacheCaretImagery(const Rectf& textArea)
{
    MultiLineEditbox* w = static_cast<MultiLineEditbox*>(d_window);
    const Font* font = w->getActualFont();
    if (!font)
        return;

    const size_t caretLine = w->getLineNumberFromIndex(w->getCaretIndex());

    const auto& lines = w->getFormattedLines();

    if (caretLine >= lines.size())
        return;

    // Calculate pixel offsets to where caret should be drawn
    const size_t caretLineIdx = w->getCaretIndex() - lines[caretLine].d_startIdx;
    const float ypos = caretLine * font->getLineSpacing();
    const float xpos = font->getTextAdvance(w->getText().substr(lines[caretLine].d_startIdx, caretLineIdx));

    const ImagerySection& caretImagery = getLookNFeel().getImagerySection("Caret");

    // Calculate final destination area for caret
    Rectf caretArea;
    caretArea.left(textArea.left() + xpos - w->getHorzScrollbar()->getScrollPosition());
    caretArea.top(textArea.top() + ypos - w->getVertScrollbar()->getScrollPosition());
    caretArea.setWidth(caretImagery.getBoundingRect(*w).getSize().d_width);
    caretArea.setHeight(font->getLineSpacing());

    // Create the render geometry for the caret image
    caretImagery.render(*w, caretArea, nullptr, &textArea);
}

//----------------------------------------------------------------------------//
void FalagardMultiLineEditbox::createRenderGeometry()
{
    // Create the render geometry for the general frame and stuff before we handle the text itself
    cacheEditboxBaseImagery();

    const Rectf textArea = getTextRenderArea();

    // Create the render geometry for the edit box text
    cacheTextLines(textArea);

    // Create the render geometry for the caret
    auto w = static_cast<const MultiLineEditbox*>(d_window);
    if (w->hasInputFocus() && !w->isReadOnly() && (!d_blinkCaret || d_showCaret))
        cacheCaretImagery(textArea);
}

//----------------------------------------------------------------------------//
void FalagardMultiLineEditbox::cacheTextLines(const Rectf& destArea)
{
    MultiLineEditbox* w = static_cast<MultiLineEditbox*>(d_window);

    const Font* font = w->getActualFont();
    if (!font)
        return;

    const float vertScrollPos = w->getVertScrollbar()->getScrollPosition();

    // Calculate final colours to use
    const ColourRect normalTextCol = getOptionalColour(UnselectedTextColourPropertyName);
    const ColourRect selectTextCol = getOptionalColour(SelectedTextColourPropertyName);
    const ColourRect selectBrushCol = getOptionalColour(
        w->hasInputFocus() ? ActiveSelectionColourPropertyName : InactiveSelectionColourPropertyName);

    const auto& lines = w->getFormattedLines();
    const size_t numLines = lines.size();

    // Calculate the range of visible lines
    const size_t sidx = static_cast<size_t>(vertScrollPos / font->getLineSpacing());
    const size_t eidx = std::min(numLines, 1 + sidx + static_cast<size_t>(destArea.getHeight() / font->getLineSpacing()));

    Rectf drawArea(destArea);
    drawArea.offset(-glm::vec2(w->getHorzScrollbar()->getScrollPosition(), vertScrollPos));
    drawArea.d_min.y += font->getLineSpacing() * static_cast<float>(sidx);

    const size_t selStart = w->getSelectionStart();
    const size_t selEnd = w->getSelectionEnd();

    // Text is already formatted, we just grab the lines and
    // create the render geometry for them with the required alignment.
    for (size_t i = sidx; i < eidx; ++i)
    {
        const auto& currLine = lines[i];
        const size_t lineStart = currLine.d_startIdx;
        const size_t lineLength = currLine.d_length;
        const String lineText(w->getTextVisual().substr(lineStart, lineLength));

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
        if (!lineText.isUtf8StringValid())
        {
            // This line is invalid, skip to the next one
            drawArea.d_min.y += font->getLineSpacing();
            continue;
        }
#endif

        // Offset the font little down so that it's centered within its own spacing
        Rectf lineRect = drawArea;
        const float oldTop = lineRect.top();
        lineRect.d_min.y += (font->getLineSpacing() - font->getFontHeight()) * 0.5f;

        // If it is a simple 'no selection area' case
        if (!w->getSelectionBrushImage() ||
            selStart >= selEnd ||
            lineStart >= selEnd ||
            lineStart + lineLength <= selStart)
        {
            // Create geometry buffers for the text and add to the Window
            font->createTextRenderGeometry(w->getGeometryBuffers(), lineText,
                lineRect.getPosition(), &destArea, true, normalTextCol, w->getDefaultParagraphDirection());
        }
        else
        {
            // Render text with selection area
            size_t sectIdx = 0;
            float selStartOffset = 0.f;

            // Create the render geometry for any text prior to selected region of line
            if (lineStart < selStart)
            {
                const size_t sectLen = selStart - lineStart;
                String sect = lineText.substr(0, sectLen);
                sectIdx = sectLen;

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
                if (!sect.isUtf8StringValid())
                {
                    // The section string is invalid, use the entire line instead
                    sect = lineText;
                    sectIdx = lineText.size();
                    w->setCaretIndex(0);
                    w->setSelectionLength(0);
                }
#endif          
                // Get the pixel offset to the beginning of the selection area highlight.
                selStartOffset = font->getTextAdvance(sect);

                font->createTextRenderGeometry(w->getGeometryBuffers(), sect,
                    lineRect.getPosition(), &destArea, true, normalTextCol,
                    w->getDefaultParagraphDirection());

                // Set position ready for next portion of text
                lineRect.d_min.x += selStartOffset;
            }

            // Get selected text section
            const size_t sectLen = std::min(selEnd - lineStart, lineLength) - sectIdx;
            const String sect = lineText.substr(sectIdx, sectLen);
            sectIdx += sectLen;

            // Get the extent to use as the width of the selection area highlight
            const float selAreaWidth = font->getTextAdvance(sect);

            const float textTop = lineRect.top();
            lineRect.top(oldTop);

            // Calculate area for the selection brush on this line
            lineRect.left(drawArea.left() + selStartOffset);
            lineRect.setWidth(selAreaWidth);
            lineRect.setHeight(font->getLineSpacing());

            // Create the render geometry for the selection area brush for this line
            ImageRenderSettings renderSettings(lineRect, &destArea, true, selectBrushCol);
            w->getSelectionBrushImage()->createRenderGeometry(w->getGeometryBuffers(), renderSettings);

            lineRect.top(textTop);

            // Create the render geometry for the selected text
            font->createTextRenderGeometry(w->getGeometryBuffers(), sect,
                lineRect.getPosition(), &destArea, true, selectTextCol, w->getDefaultParagraphDirection());

            // Create the render geometry for any text beyond selected region of line
            if (sectIdx < lineLength)
            {
                lineRect.d_min.x += selAreaWidth;
                const String sect = lineText.substr(sectIdx, lineLength - sectIdx);
                font->createTextRenderGeometry(w->getGeometryBuffers(), sect,
                    lineRect.getPosition(), &destArea, true, normalTextCol, w->getDefaultParagraphDirection());
            }
        }

        // Update master position for next line in paragraph
        drawArea.d_min.y += font->getLineSpacing();
    }
}

//----------------------------------------------------------------------------//
ColourRect FalagardMultiLineEditbox::getOptionalColour(const String& propertyName) const
{
    ColourRect rect(0);
    if (d_window->isPropertyPresent(propertyName))
        rect = d_window->getProperty<ColourRect>(propertyName);
    return rect;
}

//----------------------------------------------------------------------------//
void FalagardMultiLineEditbox::update(float elapsed)
{
    WindowRenderer::update(elapsed);

    // Only do the update if we absolutely have to
    auto w = static_cast<const MultiLineEditbox*>(d_window);
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
bool FalagardMultiLineEditbox::handleFontRenderSizeChange(const Font* const font)
{
    const bool res = WindowRenderer::handleFontRenderSizeChange(font);

    if (d_window->getActualFont() == font)
    {
        d_window->invalidate();
        static_cast<MultiLineEditbox*>(d_window)->formatText(true);
        return true;
    }

    return res;
}

}
