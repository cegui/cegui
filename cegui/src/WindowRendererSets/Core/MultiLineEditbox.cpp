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
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/TplWindowRendererProperty.h"

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
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardMultiLineEditbox, Image*,
        "SelectionBrushImage", "Property to get/set the selection brush image for the editbox.  Value should be \"[imageset_name]/[image_name]\".",
        &FalagardMultiLineEditbox::setSelectionBrushImage, &FalagardMultiLineEditbox::getSelectionBrushImage, nullptr
    );
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
Rectf FalagardMultiLineEditbox::getCaretRect() const
{
    auto w = static_cast<MultiLineEditbox*>(d_window);

    Rectf caretGlyphRect;
    if (!w->getRenderedText().getTextIndexBounds(w->getCaretIndex(), caretGlyphRect))
        return {};

    caretGlyphRect.setWidth(getLookNFeel().getImagerySection("Caret").getBoundingRect(*w).getWidth());
    return caretGlyphRect;
}

//----------------------------------------------------------------------------//
void FalagardMultiLineEditbox::createRenderGeometry()
{
    // NB: this may affect the text area and therefore is done first
    auto w = static_cast<MultiLineEditbox*>(d_window);
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
        caretRect.offset(textArea.left() - w->getHorzScrollbar()->getScrollPosition(),
            textArea.top() - w->getVertScrollbar()->getScrollPosition());
        getLookNFeel().getImagerySection("Caret").render(*d_window, caretRect, nullptr, &textArea);
    }
}

//----------------------------------------------------------------------------//
void FalagardMultiLineEditbox::renderBaseImagery() const
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
void FalagardMultiLineEditbox::createRenderGeometryForText(const Rectf& textArea)
{
    MultiLineEditbox* w = static_cast<MultiLineEditbox*>(d_window);
    const auto& renderedText = w->getRenderedText();
    if (renderedText.empty())
        return;

    // Calculate the range of visible lines
    const glm::vec2 pos(
        textArea.left() - w->getHorzScrollbar()->getScrollPosition(),
        textArea.top() - w->getVertScrollbar()->getScrollPosition());

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
void FalagardMultiLineEditbox::setSelectionBrushImage(const Image* image)
{
    if (d_selectionBrush == image)
        return;

    d_selectionBrush = image;
    d_window->invalidate();
}

}
