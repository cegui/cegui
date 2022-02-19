/***********************************************************************
    created:    2nd July 2015
    author:     Lukas Meindl

    purpose:    Implementation of EditboxBase class widget
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2015 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/EditboxBase.h"
#include "CEGUI/falagard/XMLEnumHelper.h" // for DefaultParagraphDirection property helper
#include "CEGUI/text/TextUtils.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/Clipboard.h"
#include "CEGUI/UndoHandler.h"
#include "CEGUI/CoordConverter.h"
#if defined(CEGUI_HAS_PCRE_REGEX)
#   include "CEGUI/PCRERegexMatcher.h"
#elif defined(CEGUI_HAS_STD11_REGEX)
#   include "CEGUI/StdRegexMatcher.h"
#endif
#include <string.h>

namespace CEGUI
{
const String EditboxBase::EventNamespace("EditboxBase");
const String EditboxBase::WidgetTypeName("CEGUI/EditboxBase");
const String EditboxBase::EventReadOnlyModeChanged("ReadOnlyModeChanged");
const String EditboxBase::EventTextMaskingEnabledChanged("TextMaskingEnabledChanged");
const String EditboxBase::EventTextMaskingCodepointChanged("TextMaskingCodepointChanged");
const String EditboxBase::EventMaximumTextLengthChanged("MaximumTextLengthChanged");
const String EditboxBase::EventDefaultParagraphDirectionChanged("DefaultParagraphDirectionChanged");
const String EditboxBase::EventCaretMoved("CaretMoved");
const String EditboxBase::EventTextSelectionChanged("TextSelectionChanged");
const String EditboxBase::EventEditboxFull("EditboxFull");
const String EditboxBase::EventValidationStringChanged("ValidationStringChanged");
const String EditboxBase::EventTextValidityChanged("TextValidityChanged");

//----------------------------------------------------------------------------//
static RegexMatcher* createRegexMatcher()
{
#if defined(CEGUI_HAS_PCRE_REGEX)
    return new PCRERegexMatcher();
#elif defined(CEGUI_HAS_STD11_REGEX)
    return new StdRegexMatcher();
#else
    return nullptr;
#endif
}

//----------------------------------------------------------------------------//
EditboxBase::EditboxBase(const String& type, const String& name)
    : Window(type, name)
    , d_maxTextLen(String().max_size())
    , d_undoHandler(new UndoHandler(this))
{
    d_renderedText.setHorizontalFormatting(HorizontalTextFormatting::LeftAligned);
    addEditboxBaseProperties();
}

//----------------------------------------------------------------------------//
EditboxBase::~EditboxBase()
{
    if (d_weOwnValidator && d_validator)
        delete d_validator;
}

//----------------------------------------------------------------------------//
RenderedText& EditboxBase::getRenderedText()
{
    updateRenderedText();
    return d_renderedText;
}

//----------------------------------------------------------------------------//
void EditboxBase::updateRenderedText()
{
    if (d_renderedTextDirty)
    {
        if (d_textMaskingEnabled)
        {
            const String maskedText(getText().size(), static_cast<char32_t>(d_textMaskingCodepoint));
            d_renderedText.renderText(maskedText, nullptr, getActualFont(), d_defaultParagraphDirection);
        }
        else
        {
            d_renderedText.renderText(getText(), nullptr, getActualFont(), d_defaultParagraphDirection);
        }

        d_renderedTextDirty = false;
        d_formattingDirty = true;
    }

    if (d_formattingDirty)
    {
        updateFormatting();
        invalidate();
        d_formattingDirty = false;
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::setReadOnly(bool setting)
{
    if (d_readOnly == setting)
        return;

    d_readOnly = setting;
    WindowEventArgs args(this);
    onReadOnlyChanged(args);
        
    // Update the cursor according to the read only state.
    if (setting)
        setCursor(d_readOnlyCursorImage);
    else
        setCursor(getProperty<Image*>(Window::CursorImagePropertyName));
}

//----------------------------------------------------------------------------//
void EditboxBase::setEnabled(bool enabled)
{
    Window::setEnabled(enabled);

    // Update the mouse cursor according to the read only state.
    if (enabled)
        setCursor(getProperty<Image*>(Window::CursorImagePropertyName));
    else
        setCursor(d_readOnlyCursorImage);
}

//----------------------------------------------------------------------------//
void EditboxBase::setTextMaskingEnabled(bool setting)
{
    if (d_textMaskingEnabled == setting)
        return;

    d_textMaskingEnabled = setting;

    d_renderedTextDirty = false;
    invalidate();

    WindowEventArgs args(this);
    onTextMaskingEnabledChanged(args);
}

//----------------------------------------------------------------------------//
void EditboxBase::setTextMaskingCodepoint(std::uint32_t codePoint)
{
    if (codePoint == d_textMaskingCodepoint)
        return;

    d_textMaskingCodepoint = codePoint;

    if (d_textMaskingEnabled)
    {
        d_renderedTextDirty = false;
        invalidate();
    }

    WindowEventArgs args(this);
    onTextMaskingCodepointChanged(args);
}

//----------------------------------------------------------------------------//
void EditboxBase::setMaxTextLength(size_t maxLen)
{
    if (d_maxTextLen == maxLen)
        return;

    d_maxTextLen = maxLen;

    // Trigger max length changed event
    WindowEventArgs args(this);
    onMaximumTextLengthChanged(args);

    // Trim string
    const auto& text = getText();
    if (text.size() > d_maxTextLen)
    {
        setText(text.substr(0, d_maxTextLen));
        d_undoHandler->clearUndoHistory();
        handleValidityChangeForString(getText());
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::setTextFormatting(HorizontalTextFormatting format)
{
    if (d_renderedText.getHorizontalFormatting() == format)
        return;

    d_renderedText.setHorizontalFormatting(decomposeHorizontalFormatting(format));
    d_formattingDirty = true;
    invalidate();
}

//----------------------------------------------------------------------------//
HorizontalTextFormatting EditboxBase::getTextFormatting() const
{
    return d_renderedText.getHorizontalFormatting();
}

//----------------------------------------------------------------------------//
void EditboxBase::setDefaultParagraphDirection(DefaultParagraphDirection defaultParagraphDirection)
{
    if (defaultParagraphDirection == d_defaultParagraphDirection)
        return;

    d_defaultParagraphDirection = defaultParagraphDirection;
    d_renderedTextDirty = true;

    WindowEventArgs eventArgs(this);
    fireEvent(EventDefaultParagraphDirectionChanged, eventArgs, EventNamespace);
}

//----------------------------------------------------------------------------//
RegexMatchState EditboxBase::getStringMatchState(const String& str) const
{
    return d_validator ? d_validator->getMatchStateOfString(str) : RegexMatchState::Valid;
}

//----------------------------------------------------------------------------//
void EditboxBase::setValidator(RegexMatcher* validator)
{
    if (d_validator == validator)
        return;

    if (d_weOwnValidator && d_validator)
        delete d_validator;

    d_validator = validator;

    if (d_validator)
        d_weOwnValidator = false;
    else
    {
        d_validator = createRegexMatcher();
        d_weOwnValidator = true;
    }

    if (d_validator)
    {
        if (d_validationString.empty())
            setValidationString(".*"); // Default to accepting all characters
        else
            d_validator->setRegexString(d_validationString);
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::setValidationString(const String& validationString)
{
    if (validationString == d_validationString)
        return;

    if (!d_validator)
        d_validator = createRegexMatcher();

    if (d_validator)
        d_validator->setRegexString(validationString);

    d_validationString = validationString;

    WindowEventArgs args(this);
    onValidationStringChanged(args);

    handleValidityChangeForString(getText());
}

//----------------------------------------------------------------------------//
void EditboxBase::setCaretIndex(size_t caretPos)
{
    // Make sure new position is valid
    const auto textLen = getText().size();
    if (caretPos > textLen)
        caretPos = textLen;

    if (d_caretPos == caretPos)
        return;

    d_caretPos = caretPos;

    WindowEventArgs args(this);
    onCaretMoved(args);
}

//----------------------------------------------------------------------------//
void EditboxBase::setSelection(size_t startPos, size_t endPos)
{
    const auto textLen = getText().size();

    // Ensure selection start point is within the valid range
    if (startPos > textLen)
        startPos = textLen;

    // Ensure selection end point is within the valid range
    if (endPos > textLen)
        endPos = textLen;

    // Ensure start is before end
    if (startPos > endPos)
        std::swap(startPos, endPos);

    if (startPos != d_selectionStart || endPos != d_selectionEnd)
    {
        d_selectionStart = startPos;
        d_selectionEnd = endPos;

        WindowEventArgs args(this);
        onTextSelectionChanged(args);
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::setSelectionStart(size_t start_pos)
{
    setSelection(start_pos, start_pos + getSelectionLength());
}

//----------------------------------------------------------------------------//
void EditboxBase::setSelectionLength(size_t length)
{
    const auto start = getSelectionStart();
    setSelection(start, start + length);
}

//----------------------------------------------------------------------------//
void EditboxBase::clearSelection()
{
    if (getSelectionLength())
        setSelection(0, 0);
}

//----------------------------------------------------------------------------//
size_t EditboxBase::getPrevTextIndex(size_t idx) const
{
    if (!idx)
        return 0;

    const String& text = getText();
    if (idx > text.size())
        idx = text.size();

    String::codepoint_iterator caretIter(text.begin() + idx, text.begin(), text.end());
    --caretIter;

    return caretIter.getCodeUnitIndexFromStart();
}

//----------------------------------------------------------------------------//
bool EditboxBase::insertString(String&& strToInsert)
{
    //!!!TODO TEXT: for the single line editbox need to remove all \n and \r!

    if (isReadOnly() || strToInsert.empty())
        return false;

    String tmp = getText();
    if (tmp.size() + strToInsert.size() - getSelectionLength() > d_maxTextLen)
    {
        WindowEventArgs args(this);
        onEditboxFullEvent(args);
        return false;
    }

    UndoHandler::UndoAction undoDeleteSelection;
    const auto selLength = getSelectionLength();
    if (selLength)
    {
        const auto selStart = getSelectionStart();

        undoDeleteSelection.d_type = UndoHandler::UndoActionType::Delete;
        undoDeleteSelection.d_startIdx = selStart;
        undoDeleteSelection.d_text = tmp.substr(selStart, selLength);

        tmp.erase(selStart, selLength);
    }

    tmp.insert(d_caretPos, strToInsert);

    if (!handleValidityChangeForString(tmp))
        return false;

    UndoHandler::UndoAction undoInsert;
    undoInsert.d_type = UndoHandler::UndoActionType::Insert;
    undoInsert.d_startIdx = d_caretPos;
    undoInsert.d_text = std::move(strToInsert);

    setText(tmp);

    d_undoHandler->addUndoHistory(undoInsert);
    if (selLength)
        d_undoHandler->addUndoHistory(undoDeleteSelection);

    clearSelection();
    setCaretIndex(d_caretPos + undoInsert.d_text.size());
    ensureCaretIsVisible();

    return true;
}

//----------------------------------------------------------------------------//
void EditboxBase::deleteRange(size_t start, size_t length)
{
    if (isReadOnly() || !length)
        return;

    const auto& text = getText();
    if (start >= text.size())
        return;

    UndoHandler::UndoAction undoDelete;
    undoDelete.d_type = UndoHandler::UndoActionType::Delete;
    undoDelete.d_startIdx = start;
    undoDelete.d_text = text.substr(start, length);

    const auto end = start + length;

    String tmp;
    tmp.reserve(text.size() - length);
    if (start)
        tmp += text.substr(0, start);
    if (end < text.size())
        tmp += text.substr(end);

    if (!handleValidityChangeForString(tmp))
        return;

    setCaretIndex(start);
    clearSelection();
    ensureCaretIsVisible();

    setText(tmp);

    d_undoHandler->addUndoHistory(undoDelete);
}

//----------------------------------------------------------------------------//
void EditboxBase::handleCaretMovement(size_t newIndex, bool select)
{
    if (select && !getSelectionLength())
        d_dragAnchorIdx = d_caretPos;

    setCaretIndex(newIndex);
    ensureCaretIsVisible();

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
bool EditboxBase::performCopy(Clipboard& clipboard)
{
    if (!getSelectionLength())
        return false;

    clipboard.setText(getText().substr(getSelectionStart(), getSelectionLength()));
    return true;
}

//----------------------------------------------------------------------------//
bool EditboxBase::performCut(Clipboard& clipboard)
{
    if (isReadOnly())
        return false;

    if (!performCopy(clipboard))
        return false;

    handleDelete();
    return true;
}

//----------------------------------------------------------------------------//
bool EditboxBase::performPaste(Clipboard& clipboard)
{
    return insertString(clipboard.getText());
}

//----------------------------------------------------------------------------//
void EditboxBase::onCursorPressHold(CursorInputEventArgs& e)
{
    Window::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
    {
        if (captureInput())
        {
            d_dragging = true;
            d_dragAnchorIdx = getTextIndexFromPosition(e.position);
            clearSelection();
            setCaretIndex(d_dragAnchorIdx);
            ensureCaretIsVisible();
        }

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::onCursorActivate(CursorInputEventArgs& e)
{
    Window::onCursorActivate(e);

    if (e.source == CursorInputSource::Left)
    {
        releaseInput();
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::onCursorMove(CursorInputEventArgs& e)
{
    Window::onCursorMove(e);

    if (d_dragging)
    {
        setCaretIndex(getTextIndexFromPosition(e.position));
        setSelection(d_caretPos, d_dragAnchorIdx);
        ensureCaretIsVisible();
    }

    ++e.handled;
}

//----------------------------------------------------------------------------//
void EditboxBase::onCaptureLost(WindowEventArgs& e)
{
    d_dragging = false;
    Window::onCaptureLost(e);
    ++e.handled;
}

//----------------------------------------------------------------------------//
void EditboxBase::onSelectWord(CursorInputEventArgs& e)
{
    const bool byLMB = e.source == CursorInputSource::Left;
    if (byLMB || !isReadOnly())
    {
        const auto& text = getText();
        if (d_textMaskingEnabled)
        {
            handleSelectAll();
        }
        else
        {
            const auto start = TextUtils::getWordStartIndex(text, d_caretPos);
            const auto end = byLMB ?
                TextUtils::getWordEndIndex(text, d_caretPos) :
                TextUtils::getNextWordStartIndex(text, d_caretPos);
            setCaretIndex(end);
            setSelection(start, end);
        }

        ++e.handled;
    }

    Window::onSelectWord(e);
}

//----------------------------------------------------------------------------//
void EditboxBase::onSelectAll(CursorInputEventArgs& e)
{
    if (e.source == CursorInputSource::Left)
    {
        // LMB selects only the current paragraph
        const auto& text = getText();
        if (d_textMaskingEnabled)
        {
            handleSelectAll();
        }
        else
        {
            updateRenderedText();
            const auto start = d_renderedText.paragraphStartTextIndex(d_caretPos);
            setCaretIndex(d_renderedText.paragraphEndTextIndex(d_caretPos));
            setSelection(start, d_caretPos);
        }

        ++e.handled;
    }
    else
    {
        // Regular 'select all'
        handleSelectAll();
        ++e.handled;
    }

    Window::onSelectAll(e);
}

//----------------------------------------------------------------------------//
void EditboxBase::onSized(ElementEventArgs& e)
{
    Window::onSized(e);
    d_formattingDirty = true;
}

//----------------------------------------------------------------------------//
void EditboxBase::onFontChanged(WindowEventArgs& e)
{
    Window::onFontChanged(e);

    if (d_renderedText.getDefaultFont() != getActualFont())
    {
        d_renderedTextDirty = true;
        invalidate();
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::onTextChanged(WindowEventArgs& e)
{
    Window::onTextChanged(e);

    clearSelection();

    // Make sure the caret is within the text
    const auto textLen = getText().size();
    if (d_caretPos > textLen)
        setCaretIndex(textLen);

    d_renderedTextDirty = true;
    invalidate();

    ++e.handled;
}

//----------------------------------------------------------------------------//
void EditboxBase::onCharacter(TextEventArgs& e)
{
    // NB: We are not calling the base class handler here because it propagates
    // inputs back up the window hierarchy, whereas, as a consumer of input
    // events, we want such propagation to cease with us regardless of whether
    // we actually handle the event.

    fireEvent(EventCharacterKey, e, Window::EventNamespace);

    if (e.handled || !hasInputFocus() || !getActualFont()->isCodepointAvailable(e.d_character))
        return;

    if (insertString(String(1, e.d_character)))
        ++e.handled;
}

//----------------------------------------------------------------------------//
void EditboxBase::onSemanticInputEvent(SemanticEventArgs& e)
{
    if (!isEffectiveDisabled() &&
        !isReadOnly() &&
        hasInputFocus() &&
        processSemanticInputEvent(e))
    {
        ++e.handled;
        return;
    }

    Window::onSemanticInputEvent(e);
}

//----------------------------------------------------------------------------//
bool EditboxBase::processSemanticInputEvent(const SemanticEventArgs& e)
{
    switch (e.d_semanticValue)
    {
        case SemanticValue::DeletePreviousCharacter:
            handleBackspace();
            return true;

        case SemanticValue::DeleteNextCharacter:
            handleDelete();
            return true;

        case SemanticValue::GoToPreviousCharacter:
            handleCharLeft(false);
            return true;

        case SemanticValue::GoToNextCharacter:
            handleCharRight(false);
            return true;

        case SemanticValue::SelectPreviousCharacter:
            handleCharLeft(true);
            return true;

        case SemanticValue::SelectNextCharacter:
            handleCharRight(true);
            return true;

        case SemanticValue::GoToPreviousWord:
            handleWordLeft(false);
            return true;

        case SemanticValue::GoToNextWord:
            handleWordRight(false);
            return true;

        case SemanticValue::SelectPreviousWord:
            handleWordLeft(true);
            return true;

        case SemanticValue::SelectNextWord:
            handleWordRight(true);
            return true;

        case SemanticValue::GoToStartOfDocument:
            handleHome(false, false);
            return true;

        case SemanticValue::GoToEndOfDocument:
            handleEnd(false, false);
            return true;

        case SemanticValue::SelectToStartOfDocument:
            handleHome(true, false);
            return true;

        case SemanticValue::SelectToEndOfDocument:
            handleEnd(true, false);
            return true;

        case SemanticValue::GoToStartOfLine:
            handleHome(false, true);
            return true;

        case SemanticValue::GoToEndOfLine:
            handleEnd(false, true);
            return true;

        case SemanticValue::SelectToStartOfLine:
            handleHome(true, true);
            return true;

        case SemanticValue::SelectToEndOfLine:
            handleEnd(true, true);
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
void EditboxBase::handleCharLeft(bool select)
{
    size_t previousCodePointPos = 0;
    if (d_caretPos > 0)
    {
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
        previousCodePointPos = d_caretPos - 1;
#else
        previousCodePointPos = getPrevTextIndex(d_caretPos);
#endif
    }

    handleCaretMovement(previousCodePointPos, select);
}

//----------------------------------------------------------------------------//
void EditboxBase::handleWordLeft(bool select)
{
    handleCaretMovement(TextUtils::getWordStartIndex(getText(), d_caretPos), select);
}

//----------------------------------------------------------------------------//
void EditboxBase::handleCharRight(bool select)
{
    size_t codePointSize = 0;
    const auto& text = getText();
    if (d_caretPos < text.size())
    {
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
        codePointSize = 1;
#else
        codePointSize = String::getCodePointSize(text[d_caretPos]);
#endif
    }

    handleCaretMovement(d_caretPos + codePointSize, select);
}

//----------------------------------------------------------------------------//
void EditboxBase::handleWordRight(bool select)
{
    handleCaretMovement(TextUtils::getNextWordStartIndex(getText(), d_caretPos), select);
}

//----------------------------------------------------------------------------//
void EditboxBase::handleHome(bool select, bool lineOnly)
{
    updateRenderedText();
    handleCaretMovement(lineOnly ? d_renderedText.lineStartTextIndex(d_caretPos) : 0, select);
}

//----------------------------------------------------------------------------//
void EditboxBase::handleEnd(bool select, bool lineOnly)
{
    updateRenderedText();
    handleCaretMovement(lineOnly ? d_renderedText.lineEndTextIndex(d_caretPos) : d_renderedText.endTextIndex(), select);
}

//----------------------------------------------------------------------------//
void EditboxBase::handleSelectAll()
{
    d_dragAnchorIdx = 0;
    const auto textLen = getText().size();
    setSelection(0, textLen);
    setCaretIndex(textLen);
    ensureCaretIsVisible();
}

//----------------------------------------------------------------------------//
void EditboxBase::handleBackspace()
{
    if (isReadOnly())
        return;

    if (getSelectionLength())
    {
        deleteRange(getSelectionStart(), getSelectionLength());
    }
    else if (d_caretPos > 0)
    {
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
        deleteRange(d_caretPos - 1, 1);
#else
        const size_t deleteStartPos = getPrevTextIndex(d_caretPos);
        deleteRange(deleteStartPos, d_caretPos - deleteStartPos);
#endif
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::handleDelete()
{
    if (isReadOnly())
        return;

    const auto& text = getText();
    if (getSelectionLength())
    {
        deleteRange(getSelectionStart(), getSelectionLength());
    }
    else if (d_caretPos < text.size())
    {
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
        deleteRange(d_caretPos, 1);
#else
        deleteRange(d_caretPos, String::getCodePointSize(text[d_caretPos]));
#endif
    }
}

//----------------------------------------------------------------------------//
void EditboxBase::onReadOnlyChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventReadOnlyModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onTextMaskingEnabledChanged(WindowEventArgs& e)
{
    fireEvent(EventTextMaskingEnabledChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onTextMaskingCodepointChanged(WindowEventArgs& e)
{
    fireEvent(EventTextMaskingCodepointChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onMaximumTextLengthChanged(WindowEventArgs& e)
{
    fireEvent(EventMaximumTextLengthChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onCaretMoved(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventCaretMoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onTextSelectionChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventTextSelectionChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onEditboxFullEvent(WindowEventArgs& e)
{
    fireEvent(EventEditboxFull, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onValidationStringChanged(WindowEventArgs& e)
{
    fireEvent(EventValidationStringChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void EditboxBase::onTextValidityChanged(RegexMatchStateEventArgs& e)
{
    fireEvent(EventTextValidityChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool EditboxBase::performUndo()
{
    if (isReadOnly())
        return false;

    clearSelection();
    return d_undoHandler->undo(d_caretPos);
}

//----------------------------------------------------------------------------//
bool EditboxBase::performRedo()
{
    if (isReadOnly())
        return false;

    clearSelection();
    return d_undoHandler->redo(d_caretPos);
}

//----------------------------------------------------------------------------//
bool EditboxBase::handleFontRenderSizeChange(const Font& font)
{
    const bool res = Window::handleFontRenderSizeChange(font);

    if (getActualFont() == &font)
    {
        d_renderedTextDirty = true;
        invalidate();
        return true;
    }

    return res;
}

//----------------------------------------------------------------------------//
bool EditboxBase::handleValidityChangeForString(const String& str)
{
    const RegexMatchState newState = getStringMatchState(str);

    if (newState == d_validatorMatchState)
        return d_previousValidityChangeResponse;

    RegexMatchStateEventArgs args(this, newState);
    onTextValidityChanged(args);

    d_previousValidityChangeResponse = !!args.handled;
    d_validatorMatchState = newState;

    return d_previousValidityChangeResponse;
}

//----------------------------------------------------------------------------//
void EditboxBase::addEditboxBaseProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(EditboxBase, bool,
        "ReadOnly", "Property to get/set the read-only setting for the Editbox.  Value is either \"true\" or \"false\".",
        &EditboxBase::setReadOnly, &EditboxBase::isReadOnly, false
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, bool,
        "TextMaskingEnabled", "Property to get/set the mask text setting for the Editbox.  Value is either \"true\" or \"false\".",
        &EditboxBase::setTextMaskingEnabled, &EditboxBase::isTextMaskingEnabled, false
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, std::uint32_t,
        "TextMaskingCodepoint", "Property to get/set the UTF-32 codepoint value used for masking text. "
        "Value is 32 bit unsigned integer representing an UTF-32 codepoint.",
        &EditboxBase::setTextMaskingCodepoint, &EditboxBase::getTextMaskingCodepoint,
        42
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, size_t,
        "CaretIndex", "Property to get/set the current caret index.  Value is \"[uint]\".",
        &EditboxBase::setCaretIndex, &EditboxBase::getCaretIndex, 0
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, size_t,
        "SelectionStart", "Property to get/set the zero based index of the selection start position within the text.  Value is \"[uint]\".",
        &EditboxBase::setSelectionStart, &EditboxBase::getSelectionStart, 0
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, size_t,
        "SelectionLength", "Property to get/set the length of the selection (as a count of the number of code points selected).  Value is \"[uint]\".",
        &EditboxBase::setSelectionLength, &EditboxBase::getSelectionLength, 0
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, size_t,
        "MaxTextLength", "Property to get/set the the maximum allowed text length (as a count of code points).  Value is \"[uint]\".",
        &EditboxBase::setMaxTextLength, &EditboxBase::getMaxTextLength, String().max_size()
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, Image*,
        "ReadOnlyCursorImage", "Property to get/set the mouse cursor image "
        "for the EditBox when in Read-only mode.  Value should be \"imageset/image_name\". "
        "Value is the image to use.",
        &EditboxBase::setReadOnlyCursorImage, &EditboxBase::getReadOnlyCursorImage, nullptr
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, HorizontalTextFormatting,
        "TextFormatting", "Property to get/set the horizontal formatting mode. "
        "Value is one of HorizontalTextFormatting enum",
        &EditboxBase::setTextFormatting, &EditboxBase::getTextFormatting,
        HorizontalTextFormatting::LeftAligned);
    CEGUI_DEFINE_PROPERTY(EditboxBase, DefaultParagraphDirection,
        "DefaultParagraphDirection", "Property to get/set the default paragraph direction. "
        "This is only in effect if raqm is linked and activated. It sets the default order of the "
        "words in a paragraph, which is relevant when having sentences in a RightToLeft language that "
        "may start with a word (or to be specific: first character of a word) from a LeftToRight language. "
        "Example: If the mode is set to Automatic and the first word of a paragraph in Hebrew is a German "
        "company name, written in German alphabet, the German word will end up left, whereas the rest of "
        "the Hebrew sentences starts from the righ, continuing towards the left. With the setting RightToLeft "
        "the sentence will start on the very right with the German word, as would be expected in a mainly "
        "RightToLeft written paragraph. If the language of the UI user is known, then either LeftToRight "
        "or RightToLeft should be chosen for the paragraphs. Default is LeftToRight."
        "Value is one of \"LeftToRight\", \"RightToLeft\" or \"Automatic\".",
        &EditboxBase::setDefaultParagraphDirection, &EditboxBase::getDefaultParagraphDirection,
        DefaultParagraphDirection::LeftToRight
    );
    CEGUI_DEFINE_PROPERTY(EditboxBase, String,
        "ValidationString", "Property to get/set the validation string Editbox.  Value is a text string.",
        &EditboxBase::setValidationString, &EditboxBase::getValidationString, ".*"
    );
}

}
