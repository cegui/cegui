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
#include "CEGUI/CoordConverter.h"
#include "CEGUI/GUIContext.h"

namespace CEGUI
{
const String MultiLineEditbox::EventNamespace("MultiLineEditbox");
const String MultiLineEditbox::WidgetTypeName("CEGUI/MultiLineEditbox");
const String MultiLineEditbox::VertScrollbarName("__auto_vscrollbar__");
const String MultiLineEditbox::HorzScrollbarName("__auto_hscrollbar__");

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
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    const float caretWidth = wr->getCaretWidth();

    Rectf textArea = wr->getTextRenderArea();
    d_renderedText.updateFormatting(textArea.getWidth() - caretWidth);

    Scrollbar* const vertScrollbar = getVertScrollbar();
    Scrollbar* const horzScrollbar = getHorzScrollbar();

    // Update vertical scrollbar state
    {
        const bool show = d_forceVertScroll || d_renderedText.getExtents().d_height > textArea.getHeight();
        if (vertScrollbar->isVisible() != show)
        {
            vertScrollbar->setVisible(show);
            textArea = wr->getTextRenderArea();
            d_renderedText.updateFormatting(textArea.getWidth() - caretWidth);
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
            d_renderedText.updateFormatting(textArea.getWidth() - caretWidth);
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
            d_renderedText.updateFormatting(textArea.getWidth() - caretWidth);
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
float MultiLineEditbox::getTextOffsetX() const
{
    return getHorzScrollbar()->getScrollPosition();
}

//----------------------------------------------------------------------------//
float MultiLineEditbox::getTextOffsetY() const
{
    return getVertScrollbar()->getScrollPosition();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setTextOffsetX(float value)
{
    getHorzScrollbar()->setScrollPosition(value);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setTextOffsetY(float value)
{
    getVertScrollbar()->setScrollPosition(value);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::ensureCaretIsVisible()
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    updateRenderedText();

    const Rectf caretRect = wr->getCaretRect();

    const float caretOffsetX = caretRect.left();
    const float textOffsetX = getTextOffsetX();
    if (caretOffsetX < textOffsetX)
        setTextOffsetX(caretOffsetX);
    else if (caretOffsetX > textOffsetX + d_renderedText.getAreaWidth())
        setTextOffsetX(caretOffsetX - d_renderedText.getAreaWidth());

    const Rectf textArea = wr->getTextRenderArea();
    const float textOffsetY = getTextOffsetY();
    if (caretRect.top() < textOffsetY)
        setTextOffsetY(caretRect.top());
    else if (caretRect.bottom() > textOffsetY + textArea.getHeight())
        setTextOffsetY(caretRect.bottom() - textArea.getHeight());
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handleLineUp(bool select)
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    if (d_desiredCaretOffsetXDirty)
        d_desiredCaretOffsetX = wr->getCaretRect().left();

    updateRenderedText();
    handleCaretMovement(d_renderedText.lineUpTextIndex(d_caretPos, d_desiredCaretOffsetX), select);

    d_desiredCaretOffsetXDirty = false;
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handleLineDown(bool select)
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    if (d_desiredCaretOffsetXDirty)
        d_desiredCaretOffsetX = wr->getCaretRect().left();

    updateRenderedText();
    handleCaretMovement(d_renderedText.lineDownTextIndex(d_caretPos, d_desiredCaretOffsetX), select);

    d_desiredCaretOffsetXDirty = false;
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handlePageUp(bool select)
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    const float pageHeight = wr->getTextRenderArea().getHeight();

    if (d_desiredCaretOffsetXDirty)
        d_desiredCaretOffsetX = wr->getCaretRect().left();

    updateRenderedText();
    handleCaretMovement(d_renderedText.pageUpTextIndex(d_caretPos, d_desiredCaretOffsetX, pageHeight), select);

    d_desiredCaretOffsetXDirty = false;
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handlePageDown(bool select)
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    const float pageHeight = wr->getTextRenderArea().getHeight();

    if (d_desiredCaretOffsetXDirty)
        d_desiredCaretOffsetX = wr->getCaretRect().left();

    updateRenderedText();
    handleCaretMovement(d_renderedText.pageDownTextIndex(d_caretPos, d_desiredCaretOffsetX, pageHeight), select);

    d_desiredCaretOffsetXDirty = false;
}

//----------------------------------------------------------------------------//
bool MultiLineEditbox::handleScrollChange(const EventArgs&)
{
    // Simply trigger a redraw of the MultiLineEditbox
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
void MultiLineEditbox::processKeyEvent(KeyEventArgs& e, bool down)
{
    if (d_guiContext->isInputSemantic(SemanticValue::Confirm, e, down))
        insertString("\x0a");
    else if (d_guiContext->isInputSemantic(SemanticValue::GoUp, e, down))
        handleLineUp(false);
    else if (d_guiContext->isInputSemantic(SemanticValue::SelectUp, e, down))
        handleLineUp(true);
    else if (d_guiContext->isInputSemantic(SemanticValue::GoDown, e, down))
        handleLineDown(false);
    else if (d_guiContext->isInputSemantic(SemanticValue::SelectDown, e, down))
        handleLineDown(true);
    else if (d_guiContext->isInputSemantic(SemanticValue::GoToPreviousPage, e, down))
        handlePageUp(false);
    else if (d_guiContext->isInputSemantic(SemanticValue::SelectToPreviousPage, e, down))
        handlePageUp(true);
    else if (d_guiContext->isInputSemantic(SemanticValue::GoToNextPage, e, down))
        handlePageDown(false);
    else if (d_guiContext->isInputSemantic(SemanticValue::SelectToNextPage, e, down))
        handlePageDown(true);
    else
    {
        EditboxBase::processKeyEvent(e, down);
        return;
    }

    ++e.handled;
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::onScroll(ScrollEventArgs& e)
{
    Window::onScroll(e);

    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

    if (vertScrollbar->isEffectiveVisible() && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.d_delta);
    else if (horzScrollbar->isEffectiveVisible() && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.d_delta);

    ++e.handled;
}

//------------------------------------------------------------------------//
void MultiLineEditbox::onTextChanged(WindowEventArgs& e)
{
    d_desiredCaretOffsetXDirty = true;
    EditboxBase::onTextChanged(e);
}

//------------------------------------------------------------------------//
void MultiLineEditbox::onCaretMoved(WindowEventArgs& e)
{
    d_desiredCaretOffsetXDirty = true;
    EditboxBase::onCaretMoved(e);
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
