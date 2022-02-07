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
#include "CEGUI/text/TextUtils.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/Clipboard.h"
#include "CEGUI/UndoHandler.h"

namespace CEGUI
{
const String MultiLineEditbox::EventNamespace("MultiLineEditbox");
const String MultiLineEditbox::WidgetTypeName("CEGUI/MultiLineEditbox");
const String MultiLineEditbox::VertScrollbarName("__auto_vscrollbar__");
const String MultiLineEditbox::HorzScrollbarName("__auto_hscrollbar__");
const String MultiLineEditbox::EventVertScrollbarModeChanged("VertScrollbarModeChanged");
const String MultiLineEditbox::EventHorzScrollbarModeChanged("HorzScrollbarModeChanged");

//----------------------------------------------------------------------------//
MultiLineEditboxWindowRenderer::MultiLineEditboxWindowRenderer(const String& name) :
    WindowRenderer(name, MultiLineEditbox::EventNamespace)
{
}

//----------------------------------------------------------------------------//
MultiLineEditbox::MultiLineEditbox(const String& type, const String& name)
    : EditboxBase(type, name)
{
    d_renderedText.setWordWrappingEnabled(d_wordWrap);
    addMultiLineEditboxProperties();
}

//----------------------------------------------------------------------------//
MultiLineEditbox::~MultiLineEditbox() = default;

//----------------------------------------------------------------------------//
void MultiLineEditbox::initialiseComponents()
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

    vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&MultiLineEditbox::handleScrollChange, this));
    horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&MultiLineEditbox::handleScrollChange, this));

    Window::initialiseComponents();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setCaretIndex(size_t caretPos)
{
    // Make sure new position is valid
    const auto textLen = getText().length();
    if (caretPos > textLen)
        caretPos = textLen;

    if (d_caretPos == caretPos)
        return;

    d_caretPos = caretPos;
    ensureCaretIsVisible();

    WindowEventArgs args(this);
    onCaretMoved(args);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::ensureCaretIsVisible()
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

    // calculate the location of the caret
    size_t caretLine = getLineNumberFromIndex(d_caretPos);

    if (caretLine >= d_lines.size())
        return;

    Rectf textArea(getTextRenderArea());

    size_t caretLineIdx = d_caretPos - d_lines[caretLine].d_startIdx;

    const Font* fnt = getActualFont();
    String caretLineSubstr = getText().substr(d_lines[caretLine].d_startIdx, caretLineIdx);
#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    float xpos = fnt->getTextAdvance(caretLineSubstr);
#else
    float xpos = caretLineSubstr.isUtf8StringValid() ? fnt->getTextAdvance(caretLineSubstr) : 0;
#endif
    xpos -= horzScrollbar->getScrollPosition();

    float ypos = caretLine * fnt->getLineSpacing() - vertScrollbar->getScrollPosition();

    // if caret is above window, scroll up
    if (ypos < 0)
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + ypos);
    // if caret is below the window, scroll down
    else if ((ypos += fnt->getLineSpacing()) > textArea.getHeight())
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + (ypos - textArea.getHeight()) + fnt->getLineSpacing());

    // if caret is left of the window, scroll left
    if (xpos < 0)
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + xpos - 50);
    // if caret is right of the window, scroll right
    else if (xpos > textArea.getWidth())
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + (xpos - textArea.getWidth()) + 50);
}

//----------------------------------------------------------------------------//
size_t MultiLineEditbox::getNextTokenLength(const String& text, size_t start_idx) const
{
    String::size_type pos = text.find_first_of(TextUtils::DefaultWrapDelimiters, start_idx);

    // handle case where no more whitespace exists (so this is last token)
    if (pos == String::npos)
        return (text.length() - start_idx);

    // handle 'delimiter' token cases
    if ((pos - start_idx) == 0)
        return 1;

    return (pos - start_idx);
}

//----------------------------------------------------------------------------//
size_t MultiLineEditbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
    if (d_lines.empty())
        return 0;

    // calculate final window position to be checked
    glm::vec2 wndPt = CoordConverter::screenToWindow(*this, pt);

    const Rectf textArea = getTextRenderArea();
    wndPt -= glm::vec2(textArea.d_min.x, textArea.d_min.y);

    // factor in scroll bar values
    wndPt.x += getHorzScrollbar()->getScrollPosition();
    wndPt.y += getVertScrollbar()->getScrollPosition();

    size_t lineNumber = static_cast<size_t>(
        std::max(0.0f, wndPt.y) / getActualFont()->getLineSpacing());

    if (lineNumber >= d_lines.size())
        lineNumber = d_lines.size() - 1;

    const String lineText(getText().substr(d_lines[lineNumber].d_startIdx, d_lines[lineNumber].d_length));

    size_t lineIdx = getActualFont()->getCharAtPixel(lineText, wndPt.x);

    if (lineIdx >= lineText.length() - 1)
        lineIdx = lineText.length() - 1;

    return d_lines[lineNumber].d_startIdx + lineIdx;
}

//----------------------------------------------------------------------------//
size_t MultiLineEditbox::getLineNumberFromIndex(size_t index) const
{
    size_t lineCount = d_lines.size();

    if (!lineCount)
        return 0;

    if (index >= getText().length() - 1)
        return lineCount - 1;

    size_t indexCount = 0;
    for (size_t caretLine = 0; caretLine < lineCount; ++caretLine)
    {
        indexCount += d_lines[caretLine].d_length;
        if (index < indexCount)
            return caretLine;
    }

    throw InvalidRequestException(
        "Unable to identify a line from the given, invalid, index.");
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handleLineUp(bool select)
{
    size_t caretLine = getLineNumberFromIndex(d_caretPos);
    if (caretLine > 0)
    {
        float caretPixelOffset = getActualFont()->getTextAdvance(getText().substr(d_lines[caretLine].d_startIdx, d_caretPos - d_lines[caretLine].d_startIdx));
        --caretLine;
        size_t newLineIndex = getActualFont()->getCharAtPixel(getText().substr(d_lines[caretLine].d_startIdx, d_lines[caretLine].d_length), caretPixelOffset);
        setCaretIndex(d_lines[caretLine].d_startIdx + newLineIndex);
    }

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handleLineDown(bool select)
{
    size_t caretLine = getLineNumberFromIndex(d_caretPos);
    if ((d_lines.size() > 1) && (caretLine < (d_lines.size() - 1)))
    {
        float caretPixelOffset = getActualFont()->getTextAdvance(getText().substr(d_lines[caretLine].d_startIdx, d_caretPos - d_lines[caretLine].d_startIdx));
        ++caretLine;
        size_t newLineIndex = getActualFont()->getCharAtPixel(getText().substr(d_lines[caretLine].d_startIdx, d_lines[caretLine].d_length), caretPixelOffset);
        setCaretIndex(d_lines[caretLine].d_startIdx + newLineIndex);
    }

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handlePageUp(bool select)
{
    size_t caretLine = getLineNumberFromIndex(d_caretPos);
    size_t nbLine = static_cast<size_t>(getTextRenderArea().getHeight() / getActualFont()->getLineSpacing());
    size_t newline = 0;
    if (nbLine < caretLine)
        newline = caretLine - nbLine;

    setCaretIndex(d_lines[newline].d_startIdx);

    if (select)
        setSelection(d_caretPos, d_selectionEnd);
    else
        clearSelection();

    ensureCaretIsVisible();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::handlePageDown(bool select)
{
    size_t caretLine = getLineNumberFromIndex(d_caretPos);
    size_t nbLine =  static_cast<size_t>(getTextRenderArea().getHeight() / getActualFont()->getLineSpacing());
    size_t newline = caretLine + nbLine;
    if (!d_lines.empty())
        newline = std::min(newline,d_lines.size() - 1);

    setCaretIndex(d_lines[newline].d_startIdx + d_lines[newline].d_length - 1);

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
void MultiLineEditbox::onVertScrollbarModeChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
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
Rectf MultiLineEditbox::getTextRenderArea() const
{
    if (!d_windowRenderer)
        throw InvalidRequestException("This function must be implemented by the window renderer module");

    return static_cast<MultiLineEditboxWindowRenderer*>(d_windowRenderer)->getTextRenderArea();
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setShowVertScrollbar(bool setting)
{
    if (d_forceVertScroll == setting)return;

    d_forceVertScroll = setting;

    configureScrollbars();
    WindowEventArgs args(this);
    onVertScrollbarModeChanged(args);
}

//----------------------------------------------------------------------------//
void MultiLineEditbox::setTextFormatting(HorizontalTextFormatting format)
{
    bool wordWrap = false;
    EditboxBase::setTextFormatting(decomposeHorizontalFormatting(format, wordWrap));
    if (wordWrap)
        setWordWrapEnabled(true);
}

//------------------------------------------------------------------------//
void MultiLineEditbox::setWordWrapEnabled(bool wrap)
{
    if (d_wordWrap == wrap)
        return;

    d_wordWrap = wrap;
    d_renderedText.setWordWrappingEnabled(d_wordWrap);
    invalidate();
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
