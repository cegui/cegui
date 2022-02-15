/***********************************************************************
    created:    30/6/2004
    author:        Paul D Turner

    purpose:    Implementation of the Multi-line edit box base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/MultiLineEditbox.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
const String MultiLineEditbox::EventNamespace("MultiLineEditbox");
const String MultiLineEditbox::WidgetTypeName("CEGUI/MultiLineEditbox");
const String MultiLineEditbox::VertScrollbarName("__auto_vscrollbar__");
const String MultiLineEditbox::HorzScrollbarName("__auto_hscrollbar__");

//----------------------------------------------------------------------------//
MultiLineEditboxWindowRenderer::MultiLineEditboxWindowRenderer(const String& name) :
    WindowRenderer(name, MultiLineEditbox::EventNamespace)
{
}

//----------------------------------------------------------------------------//
MultiLineEditbox::MultiLineEditbox(const String& type, const String& name)
    : EditboxBase(type, name)
{
    d_renderedText.setWordWrapEnabled(true);
    addMultiLineEditboxProperties();
}

//----------------------------------------------------------------------------//
MultiLineEditbox::~MultiLineEditbox() = default;

//----------------------------------------------------------------------------//
void MultiLineEditbox::initialiseComponents()
{
    getVertScrollbar()->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&MultiLineEditbox::handleScrollChange, this));
    getHorzScrollbar()->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&MultiLineEditbox::handleScrollChange, this));

    Window::initialiseComponents();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::updateFormatting()
{
    auto wr = static_cast<const MultiLineEditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    Scrollbar* const vertScrollbar = getVertScrollbar();
    Scrollbar* const horzScrollbar = getHorzScrollbar();

    Rectf textArea = wr->getTextRenderArea();
    d_renderedText.updateFormatting(textArea.getWidth());

    // Update vertical scrollbar state
    {
        const bool show = d_forceVertScroll || d_renderedText.getExtents().d_height > textArea.getHeight();
        if (vertScrollbar->isVisible() != show)
        {
            vertScrollbar->setVisible(show);
            textArea = wr->getTextRenderArea();
            d_renderedText.updateFormatting(textArea.getWidth());
        }
    }

    // Update horizontal scrollbar state
    const bool wasVisibleHorz = horzScrollbar->isVisible();
    {
        const bool show = d_forceHorzScroll || d_renderedText.getExtents().d_width > textArea.getWidth();
        if (wasVisibleHorz != show)
        {
            horzScrollbar->setVisible(show);
            textArea = wr->getTextRenderArea();
            d_renderedText.updateFormatting(textArea.getWidth());
        }
    }

    // Change in a horizontal scrollbar state might affect viewable area,
    // so update vertical scrollbar state again
    if (wasVisibleHorz != horzScrollbar->isVisible())
    {
        const bool show = d_forceVertScroll || d_renderedText.getExtents().d_height > textArea.getHeight();
        if (vertScrollbar->isVisible() != show)
        {
            vertScrollbar->setVisible(show);
            textArea = wr->getTextRenderArea();
            d_renderedText.updateFormatting(textArea.getWidth());
        }
    }

    vertScrollbar->setDocumentSize(d_renderedText.getExtents().d_height);
    vertScrollbar->setPageSize(textArea.getHeight());
    vertScrollbar->setStepSize(std::max(1.0f, textArea.getHeight() / 10.0f));
    vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition());

    horzScrollbar->setDocumentSize(d_renderedText.getExtents().d_width);
    horzScrollbar->setPageSize(textArea.getWidth());
    horzScrollbar->setStepSize(std::max(1.0f, textArea.getWidth() / 10.0f));
    horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition());
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::ensureCaretIsVisible()
{
    auto wr = static_cast<const MultiLineEditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    updateRenderedText();

    //!!!TODO TEXT: get caret rect from renderer? The same code as in geometry generation!
    Rectf caretGlyphRect;
    if (!d_renderedText.getTextIndexBounds(d_caretPos, caretGlyphRect))
        return;

    const Rectf textArea = wr->getTextRenderArea();

    const float caretWidth = 5.f; //!!!FIXME TEXT!
    const float areaWidth = textArea.getWidth() - caretWidth;
    const float caretOffsetX = caretGlyphRect.left();

    Scrollbar* vertScrollbar = getVertScrollbar();
    if (caretGlyphRect.top() < vertScrollbar->getScrollPosition())
        vertScrollbar->setScrollPosition(caretGlyphRect.top());
    else if (caretGlyphRect.bottom() > vertScrollbar->getScrollPosition() + textArea.getHeight())
        vertScrollbar->setScrollPosition(caretGlyphRect.bottom() - textArea.getHeight());

    Scrollbar* horzScrollbar = getHorzScrollbar();
    if (caretOffsetX < horzScrollbar->getScrollPosition())
        horzScrollbar->setScrollPosition(caretOffsetX);
    else if (caretOffsetX > horzScrollbar->getScrollPosition() + areaWidth)
        horzScrollbar->setScrollPosition(caretOffsetX - areaWidth);
}

//----------------------------------------------------------------------------//
size_t MultiLineEditbox::getTextIndexFromPosition(const glm::vec2& pt)
{
    const auto& text = getText();
    if (text.empty())
        return 0;

    auto wr = static_cast<const MultiLineEditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return 0;

    updateRenderedText();

    //???FIXME TEXT: move to renderer? Should not rely on the same calculations in different places!
    glm::vec2 localPt = CoordConverter::screenToWindow(*this, pt) - wr->getTextRenderArea().d_min;
    localPt.x += getHorzScrollbar()->getScrollPosition();
    localPt.y += getVertScrollbar()->getScrollPosition();

    return d_renderedText.getTextIndexAtPoint(localPt);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handleLineUp(bool select)
{
    //size_t caretLine = getLineNumberFromIndex(d_caretPos);
    //if (caretLine > 0)
    {
        //float caretPixelOffset = getActualFont()->getTextAdvance(getText().substr(d_lines[caretLine].d_startIdx, d_caretPos - d_lines[caretLine].d_startIdx));
        //--caretLine;
        //size_t newLineIndex = getActualFont()->getCharAtPixel(getText().substr(d_lines[caretLine].d_startIdx, d_lines[caretLine].d_length), caretPixelOffset);
        //setCaretIndex(d_lines[caretLine].d_startIdx + newLineIndex);
    }

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handleLineDown(bool select)
{
    //size_t caretLine = getLineNumberFromIndex(d_caretPos);
    //if ((d_renderedText.getLineCount() > 1) && (caretLine < (d_renderedText.getLineCount() - 1)))
    {
        //float caretPixelOffset = getActualFont()->getTextAdvance(getText().substr(d_lines[caretLine].d_startIdx, d_caretPos - d_lines[caretLine].d_startIdx));
        //++caretLine;
        //size_t newLineIndex = getActualFont()->getCharAtPixel(getText().substr(d_lines[caretLine].d_startIdx, d_lines[caretLine].d_length), caretPixelOffset);
        //setCaretIndex(d_lines[caretLine].d_startIdx + newLineIndex);
    }

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handlePageUp(bool select)
{
    auto wr = static_cast<const MultiLineEditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    //size_t caretLine = getLineNumberFromIndex(d_caretPos);
    //size_t nbLine = static_cast<size_t>(wr->getTextRenderArea().getHeight() / getActualFont()->getLineSpacing());
    //size_t newline = 0;
    //if (nbLine < caretLine)
    //    newline = caretLine - nbLine;

    //setCaretIndex(d_lines[newline].d_startIdx);

    if (select)
        setSelection(d_caretPos, d_selectionEnd);
    else
        clearSelection();

    ensureCaretIsVisible();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handlePageDown(bool select)
{
    auto wr = static_cast<const MultiLineEditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    //size_t caretLine = getLineNumberFromIndex(d_caretPos);
    //size_t nbLine =  static_cast<size_t>(wr->getTextRenderArea().getHeight() / getActualFont()->getLineSpacing());
    //size_t newline = caretLine + nbLine;
    //if (!d_lines.empty())
    //    newline = std::min(newline, d_renderedText.getLineCount() - 1);

    //setCaretIndex(d_lines[newline].d_startIdx + d_lines[newline].d_length - 1);

    if (select)
        setSelection(d_selectionStart, d_caretPos);
    else
        clearSelection();

    ensureCaretIsVisible();
}

//----------------------------------------------------------------------------//
bool MultiLineEditbox::validateWindowRenderer(const WindowRenderer* renderer) const
{
    return dynamic_cast<const MultiLineEditboxWindowRenderer*>(renderer) != nullptr;
}

//----------------------------------------------------------------------------//
bool MultiLineEditbox::handleScrollChange(const EventArgs&)
{
    // simply trigger a redraw of the MultiLineEditbox
    invalidate();
    return true;
}

//----------------------------------------------------------------------------//
Scrollbar* MultiLineEditbox::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(VertScrollbarName));
}

//----------------------------------------------------------------------------//
Scrollbar* MultiLineEditbox::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(HorzScrollbarName));
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setShowVertScrollbar(bool setting)
{
    if (d_forceVertScroll == setting)
        return;

    d_forceVertScroll = setting;
    d_formattingDirty = true;
    invalidate();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setTextFormatting(HorizontalTextFormatting format)
{
    bool wordWrap = false;
    EditboxBase::setTextFormatting(decomposeHorizontalFormatting(format, &wordWrap));
    if (wordWrap)
        setWordWrapEnabled(true);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setLastJustifiedLineFormatting(HorizontalTextFormatting format)
{
    if (d_renderedText.getLastJustifiedLineFormatting() == format)
        return;

    d_renderedText.setLastJustifiedLineFormatting(decomposeHorizontalFormatting(format));
    d_formattingDirty = true;
    invalidate();
}

//------------------------------------------------------------------------//
void MultiLineEditbox::setWordWrapEnabled(bool wrap)
{
    if (d_renderedText.isWordWrapEnabled() == wrap)
        return;

    d_renderedText.setWordWrapEnabled(wrap);
    d_formattingDirty = true;
    invalidate();
}

//------------------------------------------------------------------------//
bool MultiLineEditbox::isWordWrapEnabled() const
{
    return d_renderedText.isWordWrapEnabled();
}

//------------------------------------------------------------------------//
bool MultiLineEditbox::processSemanticInputEvent(const SemanticEventArgs& e)
{
    switch (e.d_semanticValue)
    {
        case SemanticValue::Confirm:
            insertString("\x0a");
            return true;

        case SemanticValue::GoUp:
            handleLineUp(false);
            return true;

        case SemanticValue::SelectUp:
            handleLineUp(true);
            return true;

        case SemanticValue::GoDown:
            handleLineDown(false);
            return true;

        case SemanticValue::SelectDown:
            handleLineDown(true);
            return true;

        case SemanticValue::GoToPreviousPage:
            handlePageUp(false);
            return true;

        case SemanticValue::GoToNextPage:
            handlePageDown(false);
            return true;
    }

    return EditboxBase::processSemanticInputEvent(e);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::onScroll(CursorInputEventArgs& e)
{
    Window::onScroll(e);

    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

    if (vertScrollbar->isEffectiveVisible() && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.scroll);
    else if (horzScrollbar->isEffectiveVisible() && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.scroll);

    ++e.handled;
}

//------------------------------------------------------------------------//
void MultiLineEditbox::addMultiLineEditboxProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(MultiLineEditbox, bool,
        "WordWrap", "Property to get/set the word-wrap setting of the edit box. Value is either \"true\" or \"false\".",
        &MultiLineEditbox::setWordWrapEnabled, &MultiLineEditbox::isWordWrapEnabled, true
    );
    CEGUI_DEFINE_PROPERTY(MultiLineEditbox, bool,
        "ForceVertScrollbar", "Property to get/set the 'always show' setting for the vertical scroll bar of the list box."
        "Value is either \"true\" or \"false\".",
        &MultiLineEditbox::setShowVertScrollbar, &MultiLineEditbox::isVertScrollbarAlwaysShown, false
    );
}

}
