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
#include "CEGUI/widgets/Editbox.h"
#include "CEGUI/TextUtils.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Font.h"
#include "CEGUI/Clipboard.h"
#include "CEGUI/BidiVisualMapping.h"
#include "CEGUI/UndoHandler.h"

#include <string.h>

// Start of CEGUI namespace section
namespace CEGUI
{

const String EditboxBase::EventNamespace("EditboxBase");
const String EditboxBase::WidgetTypeName("CEGUI/EditboxBase");
const String EditboxBase::EventReadOnlyModeChanged("ReadOnlyModeChanged");
const String EditboxBase::EventTextMaskingEnabledChanged("TextMaskingEnabledChanged");
const String EditboxBase::EventTextMaskingCodepointChanged("TextMaskingCodepointChanged");
const String EditboxBase::EventValidationStringChanged("ValidationStringChanged");
const String EditboxBase::EventMaximumTextLengthChanged("MaximumTextLengthChanged");
const String EditboxBase::EventTextValidityChanged("TextValidityChanged");
const String EditboxBase::EventCaretMoved("CaretMoved");
const String EditboxBase::EventTextSelectionChanged("TextSelectionChanged");
const String EditboxBase::EventEditboxFull("EditboxFull");
const String EditboxBase::EventTextAccepted("TextAccepted");
const String EditboxBase::ReadOnlyCursorImagePropertyName("ReadOnlyCursorImage");


EditboxBase::EditboxBase(const String& type, const String& name) :
    Window(type, name),
    d_readOnly(false),
    d_readOnlyCursorImage(nullptr),
    d_textMaskingEnabled(false),
    d_textMaskingCodepoint('*'),
    d_maxTextLen(String().max_size()),
    d_caretPos(0),
    d_selectionStart(0),
    d_selectionEnd(0),
    d_dragging(false)
{
    addEditboxBaseProperties();

    // override default and disable text parsing
    d_textParsingEnabled = false;
    // ban the property too, since this being off is not optional.
    banPropertyFromXML("TextParsingEnabled");

    d_undoHandler = new UndoHandler(this);
}


EditboxBase::~EditboxBase(void)
{
    delete d_undoHandler;
}


bool EditboxBase::hasInputFocus(void) const
{
    return isActive();
}


size_t EditboxBase::getSelectionStart(void) const
{
    return (d_selectionStart != d_selectionEnd) ? d_selectionStart : d_caretPos;
}


size_t EditboxBase::getSelectionEnd(void) const
{
    return (d_selectionStart != d_selectionEnd) ? d_selectionEnd : d_caretPos;
}


size_t EditboxBase::getSelectionLength(void) const
{
    return d_selectionEnd - d_selectionStart;
}


void EditboxBase::setReadOnly(bool setting)
{
    // if setting is changed
    if (d_readOnly != setting)
    {
        d_readOnly = setting;
        WindowEventArgs args(this);
        onReadOnlyChanged(args);
        
        // Update the cursor according to the read only state.
        if (setting)
            setCursor(d_readOnlyCursorImage);
        else
            setCursor(getProperty<Image*>(Window::CursorImagePropertyName));
    }
}


void EditboxBase::setEnabled(bool enabled)
{
    Window::setEnabled(enabled);

    // Update the mouse cursor according to the read only state.
    if (enabled)
        setCursor(getProperty<Image*>(Window::CursorImagePropertyName));
    else
        setCursor(d_readOnlyCursorImage);
}


void EditboxBase::setTextMaskingEnabled(bool setting)
{
    // if setting is changed
    if (d_textMaskingEnabled != setting)
    {
        d_textMaskingEnabled = setting;
        WindowEventArgs args(this);
        onTextMaskingEnabledChanged(args);
    }

}


void EditboxBase::setCaretIndex(size_t caret_pos)
{
    // make sure new position is valid
    if (caret_pos > getText().length())
        caret_pos = getText().length();

    // if new position is different
    if (d_caretPos != caret_pos)
    {
        d_caretPos = caret_pos;

        // Trigger "caret moved" event
        WindowEventArgs args(this);
        onCaretMoved(args);
    }

}

void EditboxBase::setSelection(size_t start_pos, size_t end_pos)
{
    // ensure selection start point is within the valid range
    if (start_pos > getText().length())
    {
        start_pos = getText().length();
    }

    // ensure selection end point is within the valid range
    if (end_pos > getText().length())
    {
        end_pos = getText().length() ;
    }

    // ensure start is before end
    if (start_pos > end_pos)
    {
        size_t tmp = end_pos;
        end_pos = start_pos;
        start_pos = tmp;
    }

    // only change state if values are different.
    if ((start_pos != d_selectionStart) || (end_pos != d_selectionEnd))
    {
        // setup selection
        d_selectionStart = start_pos;
        d_selectionEnd = end_pos;

        // Trigger "selection changed" event
        WindowEventArgs args(this);
        onTextSelectionChanged(args);
    }
}

void EditboxBase::setSelectionStart(size_t start_pos)
{
	this->setSelection(start_pos, start_pos + this->getSelectionLength());
}

void EditboxBase::setSelectionLength(size_t length)
{
	this->setSelection(this->getSelectionStart(), this->getSelectionStart() + length);
}

void EditboxBase::setTextMaskingCodepoint(std::uint32_t code_point)
{
    if (code_point != d_textMaskingCodepoint)
    {
        d_textMaskingCodepoint = code_point;

        // Trigger "mask code point changed" event
        WindowEventArgs args(this);
        onTextMaskingCodepointChanged(args);
    }

}

void EditboxBase::clearSelection(void)
{
    // perform action only if required.
    if (getSelectionLength() != 0)
    {
        setSelection(0, 0);
    }

}

void EditboxBase::eraseSelectedText(bool modify_text)
{
    if (getSelectionLength() != 0)
    {
        // setup new caret position and remove selection highlight.
        setCaretIndex(d_selectionStart);
        clearSelection();

        // erase the selected characters (if required)
        if (modify_text)
        {
            String newText = getText();
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UndoActionType::Delete;
            undo.d_startIdx = getSelectionStart();
            undo.d_text = newText.substr(getSelectionStart(), getSelectionLength());
            d_undoHandler->addUndoHistory(undo);

            newText.erase(getSelectionStart(), getSelectionLength());
            setText(newText);

            // trigger notification that text has changed.
            WindowEventArgs args(this);
            onTextChanged(args);
        }

    }

}

bool EditboxBase::performCopy(Clipboard& clipboard)
{
    if (getSelectionLength() == 0)
        return false;

    const String selectedText = getText().substr(
        getSelectionStart(), getSelectionLength());

    clipboard.setText(selectedText);
    return true;
}


bool EditboxBase::performCut(Clipboard& clipboard)
{
    if (isReadOnly())
        return false;

    if (!performCopy(clipboard))
        return false;

    handleDelete();
    return true;
}


void EditboxBase::onCursorPressHold(CursorInputEventArgs& e)
{
    // base class handling
    Window::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
    {
        // grab inputs
        if (captureInput())
        {
            // handle cursor press
            clearSelection();
            d_dragging = true;
            d_dragAnchorIdx = getTextIndexFromPosition(e.position);
#ifdef CEGUI_BIDI_SUPPORT
            if (d_bidiVisualMapping->getV2lMapping().size() > d_dragAnchorIdx)
                d_dragAnchorIdx =
                    d_bidiVisualMapping->getV2lMapping()[d_dragAnchorIdx];
#endif
            setCaretIndex(d_dragAnchorIdx);
        }

        ++e.handled;
    }
}


void EditboxBase::onCursorActivate(CursorInputEventArgs& e)
{
    // base class processing
    Window::onCursorActivate(e);

    if (e.source == CursorInputSource::Left)
    {
        releaseInput();
        ++e.handled;
    }
}


void EditboxBase::onCursorMove(CursorInputEventArgs& e)
{
    // base class processing
    Window::onCursorMove(e);

    if (d_dragging)
    {
        size_t anchorIdx = getTextIndexFromPosition(e.position);
#ifdef CEGUI_BIDI_SUPPORT
        if (d_bidiVisualMapping->getV2lMapping().size() > anchorIdx)
            anchorIdx = d_bidiVisualMapping->getV2lMapping()[anchorIdx];
#endif
        setCaretIndex(anchorIdx);

        setSelection(d_caretPos, d_dragAnchorIdx);
    }

    ++e.handled;
}


void EditboxBase::onCaptureLost(WindowEventArgs& e)
{
    d_dragging = false;

    // base class processing
    Window::onCaptureLost(e);

    ++e.handled;
}


void EditboxBase::handleCharLeft(bool select)
{
    if (d_caretPos > 0)
    {
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
        size_t previousCodePointPos = d_caretPos - 1;
#else
        const String& currentText = getText();
        String::codepoint_iterator caretIter(currentText.begin() + d_caretPos,
                                             currentText.begin(), currentText.end());
        --caretIter;

        size_t previousCodePointPos = caretIter.getCodeUnitIndexFromStart();
#endif
        setCaretIndex(previousCodePointPos);
    }

    if (select)
    {
        setSelection(d_caretPos, d_dragAnchorIdx);
    }
    else
    {
        clearSelection();
    }
}


void EditboxBase::handleWordLeft(bool select)
{
    if (d_caretPos > 0)
    {
        setCaretIndex(TextUtils::getWordStartIdx(getText(), getCaretIndex()));
    }

    if (select)
    {
        setSelection(d_caretPos, d_dragAnchorIdx);
    }
    else
    {
        clearSelection();
    }
}


void EditboxBase::handleCharRight(bool select)
{
    if (d_caretPos < getText().length())
    {
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
        size_t codePointSize = 1;
#else
        const CEGUI::String& currentText = getText();
        size_t codePointSize = String::getCodePointSize(currentText[d_caretPos]);
#endif

        setCaretIndex(d_caretPos + codePointSize);
    }

    if (select)
    {
        setSelection(d_caretPos, d_dragAnchorIdx);
    }
    else
    {
        clearSelection();
    }
}


void EditboxBase::handleWordRight(bool select)
{
    if (d_caretPos < getText().length())
        setCaretIndex(TextUtils::getNextWordStartIdx(getText(), d_caretPos));

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}


void EditboxBase::handleHome(bool select)
{
    if (d_caretPos > 0)
    {
        setCaretIndex(0);
    }

    if (select)
    {
        setSelection(d_caretPos, d_dragAnchorIdx);
    }
    else
    {
        clearSelection();
    }
}


void EditboxBase::handleEnd(bool select)
{
    if (d_caretPos < getText().length())
    {
        setCaretIndex(getText().length());
    }

    if (select)
    {
        setSelection(d_caretPos, d_dragAnchorIdx);
    }
    else
    {
        clearSelection();
    }
}


void EditboxBase::handleSelectAll()
{
    setSelection(0, getText().length());
    setCaretIndex(getText().length());
}


bool EditboxBase::validateWindowRenderer(const WindowRenderer* renderer) const
{
	return dynamic_cast<const EditboxWindowRenderer*>(renderer) != nullptr;
}


void EditboxBase::onReadOnlyChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventReadOnlyModeChanged, e, EventNamespace);
}


void EditboxBase::onTextMaskingEnabledChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventTextMaskingEnabledChanged , e, EventNamespace);
}


void EditboxBase::onTextMaskingCodepointChanged(WindowEventArgs& e)
{
    // if we are in masked mode, trigger a GUI redraw.
    if (isTextMaskingEnabled())
        invalidate();

    fireEvent(EventTextMaskingCodepointChanged , e, EventNamespace);
}


void EditboxBase::onMaximumTextLengthChanged(WindowEventArgs& e)
{
    fireEvent(EventMaximumTextLengthChanged , e, EventNamespace);
}


void EditboxBase::onCaretMoved(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventCaretMoved , e, EventNamespace);
}


void EditboxBase::onTextSelectionChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventTextSelectionChanged , e, EventNamespace);
}


void EditboxBase::onEditboxFullEvent(WindowEventArgs& e)
{
    fireEvent(EventEditboxFull, e, EventNamespace);
}


void EditboxBase::addEditboxBaseProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(EditboxBase, bool,
        "ReadOnly", "Property to get/set the read-only setting for the Editbox.  Value is either \"true\" or \"false\".",
        &EditboxBase::setReadOnly, &EditboxBase::isReadOnly, false
    );

    CEGUI_DEFINE_PROPERTY(EditboxBase, bool,
        "TextMaskingEnabled","Property to get/set the mask text setting for the Editbox.  Value is either \"true\" or \"false\".",
        &EditboxBase::setTextMaskingEnabled, &EditboxBase::isTextMaskingEnabled, false
    );

    CEGUI_DEFINE_PROPERTY(EditboxBase, std::uint32_t,
        "TextMaskingCodepoint","Property to get/set the UTF-32 codepoint value used for masking text. "
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
}


size_t EditboxBase::getCaretIndex(void) const
{
#ifdef CEGUI_BIDI_SUPPORT
    size_t caretPos = d_caretPos;
    if (d_bidiVisualMapping->getL2vMapping().size() > caretPos)
        caretPos = d_bidiVisualMapping->getL2vMapping()[caretPos];
#endif

    return d_caretPos;
}


bool EditboxBase::performUndo()
{
    bool result = false;
    if (!isReadOnly())
    {
        clearSelection();

        result = d_undoHandler->undo(d_caretPos);
        WindowEventArgs args(this);
        onTextChanged(args);
    }

    return result;
}


bool EditboxBase::performRedo()
{
    bool result = false;
    if (!isReadOnly())
    {
        clearSelection();

        result = d_undoHandler->redo(d_caretPos);
        WindowEventArgs args(this);
        onTextChanged(args);
    }

    return result;
}


bool EditboxBase::handleBasicSemanticValue(SemanticEventArgs& e)
{
    switch (e.d_semanticValue)
    {
    case SemanticValue::DeletePreviousCharacter:
        handleBackspace();
        break;

    case SemanticValue::DeleteNextCharacter:
        handleDelete();
        break;

    case SemanticValue::GoToPreviousCharacter:
        handleCharLeft(false);
        break;

    case SemanticValue::GoToNextCharacter:
        handleCharRight(false);
        break;

    case SemanticValue::SelectPreviousCharacter:
        handleCharLeft(true);
        break;

    case SemanticValue::SelectNextCharacter:
        handleCharRight(true);
        break;

    case SemanticValue::GoToPreviousWord:
        handleWordLeft(false);
        break;

    case SemanticValue::GoToNextWord:
        handleWordRight(false);
        break;

    case SemanticValue::SelectPreviousWord:
        handleWordLeft(true);
        break;

    case SemanticValue::SelectNextWord:
        handleWordRight(true);
        break;

    case SemanticValue::GoToStartOfDocument:
        handleHome(false);
        break;

    case SemanticValue::GoToEndOfDocument:
        handleEnd(false);
        break;

    case SemanticValue::SelectToStartOfDocument:
        handleHome(true);
        break;

    case SemanticValue::SelectToEndOfDocument:
        handleEnd(true);
        break;

    case SemanticValue::SelectAll:
        handleSelectAll();
        break;

    default:
        return false;
    }

    return true;
}

bool isSelectionSemanticValue(SemanticValue value)
{
    return (value >= SemanticValue::SelectRange && value <= SemanticValue::SelectToEndOfLine) ||
        (value >= SemanticValue::SelectToStartOfDocument && value <= SemanticValue::SelectToPreviousPage);
}

}
