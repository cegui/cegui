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
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/TplWindowRendererProperty.h"
#include "CEGUI/widgets/MultiLineEditbox.h"
#include "CEGUI/widgets/Scrollbar.h"


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
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardEditbox, Image*,
        "SelectionBrushImage", "Property to get/set the selection brush image for the editbox.  Value should be \"[imageset_name]/[image_name]\".",
        &FalagardEditbox::setSelectionBrushImage, &FalagardEditbox::getSelectionBrushImage, nullptr);
}

//----------------------------------------------------------------------------//
Rectf FalagardEditbox::getTextRenderArea() const
{
    const WidgetLookFeel& wlf = getLookNFeel();

    if (auto w = dynamic_cast<MultiLineEditbox*>(d_window))
    {
        // If either of the scrollbars are visible, we might want to use another text rendering area
        const bool v_visible = w->getVertScrollbar()->isVisible();
        const bool h_visible = w->getHorzScrollbar()->isVisible();
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
    }

    // Default to plain TextArea
    return wlf.getNamedArea("TextArea").getArea().getPixelRect(*d_window);
}

//----------------------------------------------------------------------------//
Rectf FalagardEditbox::getCaretRect() const
{
    auto w = static_cast<EditboxBase*>(d_window);

    Rectf caretGlyphRect;
    bool isRightToLeft;
    if (!w->getRenderedText().getTextIndexBounds(w->getCaretIndex(), caretGlyphRect, &isRightToLeft))
        return {};

    if (isRightToLeft)
        caretGlyphRect.d_min.x = caretGlyphRect.d_max.x - getCaretWidth();
    else
        caretGlyphRect.d_max.x = caretGlyphRect.d_min.x + getCaretWidth();

    return caretGlyphRect;
}

//----------------------------------------------------------------------------//
float FalagardEditbox::getCaretWidth() const
{
    return getLookNFeel().getImagerySection("Caret").getBoundingRect(*d_window).getWidth();
}

//----------------------------------------------------------------------------//
void FalagardEditbox::createRenderGeometry()
{
    // NB: this may affect the text area and therefore is done first
    auto w = static_cast<EditboxBase*>(d_window);
    const auto& renderedText = w->getRenderedText();

    // Create the render geometry for the general frame and stuff before we handle the text itself
    renderBaseImagery();

    // Create the render geometry for the edit box text
    const Rectf textArea = getTextRenderArea();
    createRenderGeometryForText(textArea);

    // Create the render geometry for the caret
    if (!w->isReadOnly() && (!d_blinkCaret || d_showCaret) && w->hasInputFocus())
    {
        Rectf caretRect = getCaretRect();
        caretRect.offset(textArea.d_min - w->getTextOffset());
        getLookNFeel().getImagerySection("Caret").render(*d_window, caretRect, nullptr, &textArea);
    }
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderBaseImagery() const
{
    auto w = static_cast<EditboxBase*>(d_window);

    const auto& lnf = getLookNFeel();

    String state;

    if (w->isEffectiveDisabled())
        state = "Disabled";
    else
    {
        if (w->isReadOnly())
            state = "ReadOnly";
        else
            state = "Enabled";

        if (w->isFocused() && lnf.isStateImageryPresent(state + "Focused"))
            state += "Focused";
    }

    lnf.getStateImagery(state).render(*w);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::createRenderGeometryForText(const Rectf& textArea)
{
    auto w = static_cast<EditboxBase*>(d_window);
    const auto& renderedText = w->getRenderedText();
    if (renderedText.empty())
        return;

    const glm::vec2 pos = textArea.d_min - w->getTextOffset();
    const ColourRect normalTextCol = getOptionalColour(UnselectedTextColourPropertyName);

    const size_t selStart = w->getSelectionStart();
    const size_t selEnd = w->getSelectionEnd();
    SelectionInfo* selection = nullptr;
    SelectionInfo selectionInfo;
    if (selStart < selEnd)
    {
        selectionInfo.bgBrush = d_selectionBrush;
        selectionInfo.bgColours = getOptionalColour(
            w->isActive() ? ActiveSelectionColourPropertyName : InactiveSelectionColourPropertyName);
        selectionInfo.textColours = getOptionalColour(SelectedTextColourPropertyName);
        selectionInfo.start = selStart;
        selectionInfo.end = selEnd;
        selection = &selectionInfo;
    }

    renderedText.createRenderGeometry(w->getGeometryBuffers(), pos, &normalTextCol, &textArea, selection);
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
void FalagardEditbox::setSelectionBrushImage(const Image* image)
{
    if (d_selectionBrush == image)
        return;

    d_selectionBrush = image;
    d_window->invalidate();
}

}
