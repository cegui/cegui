/***********************************************************************
    created:    13/4/2004
    author:     Paul D Turner

    purpose:    Implementation of Editbox base class widget
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
//----------------------------------------------------------------------------//
EditboxWindowRenderer::EditboxWindowRenderer(const String& name) :
    WindowRenderer(name, Editbox::EventNamespace)
{
}

//----------------------------------------------------------------------------//
const String Editbox::EventNamespace("Editbox");
const String Editbox::WidgetTypeName("CEGUI/Editbox");
const String Editbox::EventReadOnlyModeChanged("ReadOnlyModeChanged");
const String Editbox::EventMaskedRenderingModeChanged("MaskedRenderingModeChanged");
const String Editbox::EventMaskCodePointChanged("MaskCodePointChanged");
const String Editbox::EventValidationStringChanged("ValidationStringChanged");
const String Editbox::EventMaximumTextLengthChanged("MaximumTextLengthChanged");
const String Editbox::EventTextValidityChanged("TextValidityChanged");
const String Editbox::EventCaretMoved("CaretMoved");
const String Editbox::EventTextSelectionChanged("TextSelectionChanged");
const String Editbox::EventEditboxFull("EditboxFull");
const String Editbox::EventTextAccepted("TextAccepted");
const String Editbox::ReadOnlyCursorImagePropertyName("ReadOnlyCursorImage");

//----------------------------------------------------------------------------//
Editbox::Editbox(const String& type, const String& name) :
    Window(type, name),
    d_readOnly(false),
    d_readOnlyCursorImage(0),
    d_maskText(false),
    d_maskCodePoint('*'),
    d_maxTextLen(String().max_size()),
    d_caretPos(0),
    d_selectionStart(0),
    d_selectionEnd(0),
    d_validator(System::getSingleton().createRegexMatcher()),
    d_weOwnValidator(true),
    d_dragging(false),
    d_validatorMatchState(RegexMatcher::MS_VALID),
    d_previousValidityChangeResponse(true)
{
    addEditboxProperties();

    // override default and disable text parsing
    d_textParsingEnabled = false;
    // ban the property too, since this being off is not optional.
    banPropertyFromXML("TextParsingEnabled");

    // default to accepting all characters
    if (d_validator)
        setValidationString(".*");
    // set copy of validation string to ".*" so getter returns something valid.
    else
        d_validationString = ".*";

    d_undoHandler = new UndoHandler(this);
}

//----------------------------------------------------------------------------//
Editbox::~Editbox(void)
{
    if (d_weOwnValidator && d_validator)
        System::getSingleton().destroyRegexMatcher(d_validator);

    delete d_undoHandler;
}

//----------------------------------------------------------------------------//
bool Editbox::hasInputFocus(void) const
{
    return isActive();
}

//----------------------------------------------------------------------------//
Editbox::MatchState Editbox::getTextMatchState() const
{
    return d_validatorMatchState;
}

//----------------------------------------------------------------------------//
size_t Editbox::getSelectionStartIndex(void) const
{
    return (d_selectionStart != d_selectionEnd) ? d_selectionStart : d_caretPos;
}

//----------------------------------------------------------------------------//
size_t Editbox::getSelectionEndIndex(void) const
{
    return (d_selectionStart != d_selectionEnd) ? d_selectionEnd : d_caretPos;
}

//----------------------------------------------------------------------------//
size_t Editbox::getSelectionLength(void) const
{
    return d_selectionEnd - d_selectionStart;
}

//----------------------------------------------------------------------------//
void Editbox::setReadOnly(bool setting)
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

//----------------------------------------------------------------------------//
void Editbox::setEnabled(bool enabled)
{
    Window::setEnabled(enabled);

    // Update the mouse cursor according to the read only state.
    if (enabled)
        setCursor(getProperty<Image*>(Window::CursorImagePropertyName));
    else
        setCursor(d_readOnlyCursorImage);
}

//----------------------------------------------------------------------------//
void Editbox::setTextMasked(bool setting)
{
    // if setting is changed
    if (d_maskText != setting)
    {
        d_maskText = setting;
        WindowEventArgs args(this);
        onMaskedRenderingModeChanged(args);
    }

}

//----------------------------------------------------------------------------//
void Editbox::setValidationString(const String& validation_string)
{
    if (validation_string == d_validationString)
        return;

    if (!d_validator)
        CEGUI_THROW(InvalidRequestException(
            "Unable to set validation string on Editbox '" + getNamePath() +
            "' because it does not currently have a RegexMatcher validator."));

    d_validationString = validation_string;
    d_validator->setRegexString(validation_string);

    // notification
    WindowEventArgs args(this);
    onValidationStringChanged(args);

    handleValidityChangeForString(getText());
}

//----------------------------------------------------------------------------//
void Editbox::setCaretIndex(size_t caret_pos)
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

//----------------------------------------------------------------------------//
void Editbox::setSelection(size_t start_pos, size_t end_pos)
{
    // ensure selection start point is within the valid range
    if (start_pos > getText().length())
        start_pos = getText().length();

    // ensure selection end point is within the valid range
    if (end_pos > getText().length())
        end_pos = getText().length();

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
        d_selectionEnd  = end_pos;

        // Trigger "selection changed" event
        WindowEventArgs args(this);
        onTextSelectionChanged(args);
    }

}

//----------------------------------------------------------------------------//
void Editbox::setSelectionStart(size_t start_pos)
{
	this->setSelection(start_pos,start_pos + this->getSelectionLength());
}
//----------------------------------------------------------------------------//
void Editbox::setSelectionLength(size_t length)
{
	this->setSelection(this->getSelectionStartIndex(),this->getSelectionStartIndex() + length);
}


//----------------------------------------------------------------------------//
void Editbox::setMaskCodePoint(String::value_type code_point)
{
    if (code_point != d_maskCodePoint)
    {
        d_maskCodePoint = code_point;

        // Trigger "mask code point changed" event
        WindowEventArgs args(this);
        onMaskCodePointChanged(args);
    }

}

//----------------------------------------------------------------------------//
void Editbox::setMaxTextLength(size_t max_len)
{
    if (d_maxTextLen != max_len)
    {
        d_maxTextLen = max_len;

        // Trigger max length changed event
        WindowEventArgs args(this);
        onMaximumTextLengthChanged(args);

        // trim string
        if (getText().length() > d_maxTextLen)
        {
            String newText = getText();
            newText.resize(d_maxTextLen);
            setText(newText);
            onTextChanged(args);

            const MatchState state = getStringMatchState(getText());
            if (d_validatorMatchState != state)
            {
                RegexMatchStateEventArgs rms_args(this, state);
                onTextValidityChanged(rms_args);
                d_validatorMatchState = state;
            }
        }

    }

}

//----------------------------------------------------------------------------//
void Editbox::clearSelection(void)
{
    // perform action only if required.
    if (getSelectionLength() != 0)
        setSelection(0, 0);
}

//----------------------------------------------------------------------------//
void Editbox::eraseSelectedText(bool modify_text)
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
            undo.d_type = UndoHandler::UAT_DELETE;
            undo.d_startIdx = getSelectionStartIndex();
            undo.d_text = newText.substr(getSelectionStartIndex(), getSelectionLength());
            d_undoHandler->addUndoHistory(undo);

            newText.erase(getSelectionStartIndex(), getSelectionLength());
            setText(newText);

            // trigger notification that text has changed.
            WindowEventArgs args(this);
            onTextChanged(args);
        }

    }

}

//----------------------------------------------------------------------------//
Editbox::MatchState Editbox::getStringMatchState(const String& str) const
{
    return d_validator ? d_validator->getMatchStateOfString(str) :
                         RegexMatcher::MS_VALID;
}

//----------------------------------------------------------------------------//
void Editbox::setValidator(RegexMatcher* validator)
{
    if (d_weOwnValidator && d_validator)
        System::getSingleton().destroyRegexMatcher(d_validator);

	d_validator = validator;

    if (d_validator)
        d_weOwnValidator = false;
    else
    {
        d_validator = System::getSingleton().createRegexMatcher();
        d_weOwnValidator = true;
    }
}

//----------------------------------------------------------------------------//
bool Editbox::performCopy(Clipboard& clipboard)
{
    if (getSelectionLength() == 0)
        return false;

    const String selectedText = getText().substr(
        getSelectionStartIndex(), getSelectionLength());

    clipboard.setText(selectedText);
    return true;
}

//----------------------------------------------------------------------------//
bool Editbox::performCut(Clipboard& clipboard)
{
    if (isReadOnly())
        return false;

    if (!performCopy(clipboard))
        return false;

    handleDelete();
    return true;
}

//----------------------------------------------------------------------------//
bool Editbox::handleValidityChangeForString(const String& str)
{
    const MatchState new_state = getStringMatchState(str);

    if (new_state == d_validatorMatchState)
        return d_previousValidityChangeResponse;

    RegexMatchStateEventArgs args(this, new_state);
    onTextValidityChanged(args);

    const bool response = (args.handled != 0);
    if (response)
    {
        d_validatorMatchState = new_state;
        d_previousValidityChangeResponse = response;
    }

    return response;
}

//----------------------------------------------------------------------------//
bool Editbox::performPaste(Clipboard& clipboard)
{
    if (isReadOnly())
        return false;

    String clipboardText = clipboard.getText();

    if (clipboardText.empty())
        return false;

    // backup current text
    String tmp(getText());

    UndoHandler::UndoAction undoSelection;
    undoSelection.d_type = UndoHandler::UAT_DELETE;
    undoSelection.d_startIdx = getSelectionStartIndex();
    undoSelection.d_text = tmp.substr(getSelectionStartIndex(), getSelectionLength());

    tmp.erase(getSelectionStartIndex(), getSelectionLength());

    // if there is room
    if (tmp.length() < d_maxTextLen)
    {
        UndoHandler::UndoAction undo;
        undo.d_type = UndoHandler::UAT_INSERT;
        undo.d_startIdx = getCaretIndex();
        undo.d_text = clipboardText;

        tmp.insert(getSelectionStartIndex(), clipboardText);

        if (handleValidityChangeForString(tmp))
        {
            // erase selection using mode that does not modify getText()
            // (we just want to update state)
            eraseSelectedText(false);

            // advance caret (done first so we can "do stuff" in event
            // handlers!)
            d_caretPos += clipboardText.length();

            // set text to the newly modified string
            setText(tmp);
            d_undoHandler->addUndoHistory(undo);
            if (getSelectionLength() > 0)
                d_undoHandler->addUndoHistory(undoSelection);

            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------//
void Editbox::onCursorPressHold(CursorInputEventArgs& e)
{
    // base class handling
    Window::onCursorPressHold(e);

    if (e.source == CIS_Left)
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

//----------------------------------------------------------------------------//
void Editbox::onCursorActivate(CursorInputEventArgs& e)
{
    // base class processing
    Window::onCursorActivate(e);

    if (e.source == CIS_Left)
    {
        releaseInput();
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void Editbox::onCursorMove(CursorInputEventArgs& e)
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

//----------------------------------------------------------------------------//
void Editbox::onCaptureLost(WindowEventArgs& e)
{
    d_dragging = false;

    // base class processing
    Window::onCaptureLost(e);

    ++e.handled;
}

//----------------------------------------------------------------------------//
void Editbox::onCharacter(TextEventArgs& e)
{
    // NB: We are not calling the base class handler here because it propagates
    // inputs back up the window hierarchy, whereas, as a consumer of input
    // events, we want such propagation to cease with us regardless of whether
    // we actually handle the event.

    // fire event.
    fireEvent(EventCharacterKey, e, Window::EventNamespace);

    // only need to take notice if we have focus
    if (e.handled == 0 && hasInputFocus() && !isReadOnly() &&
        getFont()->isCodepointAvailable(e.character))
    {
        // backup current text
        String tmp(getText());

        UndoHandler::UndoAction undoSelection;
        undoSelection.d_type = UndoHandler::UAT_DELETE;
        undoSelection.d_startIdx = getSelectionStartIndex();
        undoSelection.d_text = tmp.substr(getSelectionStartIndex(), getSelectionLength());

        tmp.erase(getSelectionStartIndex(), getSelectionLength());

        // if there is room
        if (tmp.length() < d_maxTextLen)
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UAT_INSERT;
            undo.d_startIdx = getSelectionStartIndex();
            undo.d_text = e.character;

            tmp.insert(getSelectionStartIndex(), 1, e.character);

            if (handleValidityChangeForString(tmp))
            {
                // erase selection using mode that does not modify getText()
                // (we just want to update state)
                eraseSelectedText(false);

                // advance caret (done first so we can "do stuff" in event
                // handlers!)
                d_caretPos++;

                // set text to the newly modified string
                setText(tmp);

                // char was accepted into the Editbox - mark event as handled.
                ++e.handled;
                d_undoHandler->addUndoHistory(undo);
                if (getSelectionLength() > 0)
                    d_undoHandler->addUndoHistory(undoSelection);
            }
        }
        else
        {
            // Trigger text box full event
            WindowEventArgs args(this);
            onEditboxFullEvent(args);
        }

    }

    // event was (possibly) not handled
}

//----------------------------------------------------------------------------//
void Editbox::handleBackspace(void)
{
    if (!isReadOnly())
    {
        String tmp(getText());

        if (getSelectionLength() != 0)
        {
            UndoHandler::UndoAction undoSelection;
            undoSelection.d_type = UndoHandler::UAT_DELETE;
            undoSelection.d_startIdx = getSelectionStartIndex();
            undoSelection.d_text = tmp.substr(getSelectionStartIndex(), getSelectionLength());

            tmp.erase(getSelectionStartIndex(), getSelectionLength());

            if (handleValidityChangeForString(tmp))
            {
                // erase selection using mode that does not modify getText()
                // (we just want to update state)
                eraseSelectedText(false);

                // set text to the newly modified string
                setText(tmp);
                d_undoHandler->addUndoHistory(undoSelection);
            }
        }
        else if (getCaretIndex() > 0)
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UAT_DELETE;
            undo.d_startIdx = d_caretPos - 1;
            undo.d_text = tmp.substr(d_caretPos - 1, 1);

            tmp.erase(d_caretPos - 1, 1);

            if (handleValidityChangeForString(tmp))
            {
                setCaretIndex(d_caretPos - 1);

                // set text to the newly modified string
                setText(tmp);
                d_undoHandler->addUndoHistory(undo);
            }
        }

    }

}

//----------------------------------------------------------------------------//
void Editbox::handleDelete(void)
{
    if (!isReadOnly())
    {
        String tmp(getText());

        if (getSelectionLength() != 0)
        {
            UndoHandler::UndoAction undoSelection;
            undoSelection.d_type = UndoHandler::UAT_DELETE;
            undoSelection.d_startIdx = getSelectionStartIndex();
            undoSelection.d_text = tmp.substr(getSelectionStartIndex(), getSelectionLength());

            tmp.erase(getSelectionStartIndex(), getSelectionLength());

            if (handleValidityChangeForString(tmp))
            {
                // erase selection using mode that does not modify getText()
                // (we just want to update state)
                eraseSelectedText(false);

                // set text to the newly modified string
                setText(tmp);
                d_undoHandler->addUndoHistory(undoSelection);
            }
        }
        else if (getCaretIndex() < tmp.length())
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UAT_DELETE;
            undo.d_startIdx = d_caretPos;
            undo.d_text = tmp.substr(d_caretPos, 1);

            tmp.erase(d_caretPos, 1);

            if (handleValidityChangeForString(tmp))
            {
                // set text to the newly modified string
                setText(tmp);
                d_undoHandler->addUndoHistory(undo);
            }
        }

    }

}

//----------------------------------------------------------------------------//
void Editbox::handleCharLeft(bool select)
{
    if (d_caretPos > 0)
        setCaretIndex(d_caretPos - 1);

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleWordLeft(bool select)
{
    if (d_caretPos > 0)
        setCaretIndex(TextUtils::getWordStartIdx(getText(), d_caretPos));

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleCharRight(bool select)
{
    if (d_caretPos < getText().length())
        setCaretIndex(d_caretPos + 1);

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleWordRight(bool select)
{
    if (d_caretPos < getText().length())
        setCaretIndex(TextUtils::getNextWordStartIdx(getText(), d_caretPos));

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleHome(bool select)
{
    if (d_caretPos > 0)
        setCaretIndex(0);

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleEnd(bool select)
{
    if (d_caretPos < getText().length())
        setCaretIndex(getText().length());

    if (select)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
bool Editbox::validateWindowRenderer(const WindowRenderer* renderer) const
{
	return dynamic_cast<const EditboxWindowRenderer*>(renderer) != 0;
}

//----------------------------------------------------------------------------//
void Editbox::onReadOnlyChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventReadOnlyModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onMaskedRenderingModeChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventMaskedRenderingModeChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onMaskCodePointChanged(WindowEventArgs& e)
{
    // if we are in masked mode, trigger a GUI redraw.
    if (isTextMasked())
        invalidate();

    fireEvent(EventMaskCodePointChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onValidationStringChanged(WindowEventArgs& e)
{
    fireEvent(EventValidationStringChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onMaximumTextLengthChanged(WindowEventArgs& e)
{
    fireEvent(EventMaximumTextLengthChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onTextValidityChanged(RegexMatchStateEventArgs& e)
{
    fireEvent(EventTextValidityChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onCaretMoved(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventCaretMoved , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onTextSelectionChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventTextSelectionChanged , e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onEditboxFullEvent(WindowEventArgs& e)
{
    fireEvent(EventEditboxFull, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onTextAcceptedEvent(WindowEventArgs& e)
{
    fireEvent(EventTextAccepted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onTextChanged(WindowEventArgs& e)
{
    // base class processing
    Window::onTextChanged(e);

    // clear selection
    clearSelection();

    // make sure caret is within the text
    if (d_caretPos > getText().length())
        setCaretIndex(getText().length());

    ++e.handled;
}

//----------------------------------------------------------------------------//
void Editbox::addEditboxProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Editbox, bool,
          "ReadOnly","Property to get/set the read-only setting for the Editbox.  Value is either \"true\" or \"false\".",
          &Editbox::setReadOnly, &Editbox::isReadOnly, false
    );

    CEGUI_DEFINE_PROPERTY(Editbox, bool,
          "MaskText","Property to get/set the mask text setting for the Editbox.  Value is either \"true\" or \"false\".",
          &Editbox::setTextMasked, &Editbox::isTextMasked, false /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(Editbox, String::value_type,
          "MaskCodepoint","Property to get/set the utf32 codepoint value used for masking text. "
          "Value is either \"[uint]\" (number = codepoint) if CEGUI is compiled with utf32 string or \"[char]\" (just the symbol) if CEGUI is compiled with std::string.",
          &Editbox::setMaskCodePoint, &Editbox::getMaskCodePoint, 42
    );

    CEGUI_DEFINE_PROPERTY(Editbox, String,
          "ValidationString","Property to get/set the validation string Editbox.  Value is a text string.",
          &Editbox::setValidationString, &Editbox::getValidationString, ".*"
    );

    CEGUI_DEFINE_PROPERTY(Editbox, size_t,
          "CaretIndex","Property to get/set the current caret index.  Value is \"[uint]\".",
          &Editbox::setCaretIndex, &Editbox::getCaretIndex, 0
    );

    CEGUI_DEFINE_PROPERTY(Editbox, size_t,
          "SelectionStart","Property to get/set the zero based index of the selection start position within the text.  Value is \"[uint]\".",
          &Editbox::setSelectionStart, &Editbox::getSelectionStartIndex, 0 /* TODO: getter inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(Editbox, size_t,
          "SelectionLength","Property to get/set the length of the selection (as a count of the number of code points selected).  Value is \"[uint]\".",
          &Editbox::setSelectionLength, &Editbox::getSelectionLength, 0
    );

    CEGUI_DEFINE_PROPERTY(Editbox, size_t,
          "MaxTextLength","Property to get/set the the maximum allowed text length (as a count of code points).  Value is \"[uint]\".",
          &Editbox::setMaxTextLength, &Editbox::getMaxTextLength, String().max_size()
    );
    CEGUI_DEFINE_PROPERTY(Editbox, Image*,
        "ReadOnlyCursorImage", "Property to get/set the mouse cursor image "
        "for the EditBox when in Read-only mode.  Value should be \"imageset/image_name\". "
        "Value is the image to use.",
        &Editbox::setReadOnlyCursorImage, &Editbox::getReadOnlyCursorImage, 0
    );
}

//----------------------------------------------------------------------------//
size_t Editbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
    if (d_windowRenderer != 0)
    {
        EditboxWindowRenderer* wr = (EditboxWindowRenderer*)d_windowRenderer;
        return wr->getTextIndexFromPosition(pt);
    }
    else
    {
        CEGUI_THROW(InvalidRequestException(
            "This function must be implemented by the window renderer"));
    }
}

//----------------------------------------------------------------------------//
size_t Editbox::getCaretIndex(void) const
{
#ifdef CEGUI_BIDI_SUPPORT
    size_t caretPos = d_caretPos;
    if (d_bidiVisualMapping->getL2vMapping().size() > caretPos)
        caretPos = d_bidiVisualMapping->getL2vMapping()[caretPos];
#endif

    return d_caretPos;
}

//----------------------------------------------------------------------------//
void Editbox::onSemanticInputEvent(SemanticEventArgs& e)
{
    if (isDisabled())
        return;

    if (e.d_semanticValue == SV_SelectAll &&
        e.d_payload.source == CIS_Left)
    {
        d_dragAnchorIdx = 0;
        setCaretIndex(getText().length());
        setSelection(d_dragAnchorIdx, d_caretPos);
        ++e.handled;
    }
    else if (e.d_semanticValue == SV_SelectWord &&
        e.d_payload.source == CIS_Left)
    {
        // if masked, set up to select all
        if (isTextMasked())
        {
            d_dragAnchorIdx = 0;
            setCaretIndex(getText().length());
        }
        // not masked, so select the word that was double-clicked.
        else
        {
            d_dragAnchorIdx = TextUtils::getWordStartIdx(getText(),
                (d_caretPos == getText().length()) ? d_caretPos :
                d_caretPos + 1);
            d_caretPos = TextUtils::getNextWordStartIdx(getText(), d_caretPos);
        }

        // perform actual selection operation.
        setSelection(d_dragAnchorIdx, d_caretPos);

        ++e.handled;
    }

    // TODO: migrate common logic with MultiLineEditbox in a base class
    if (e.handled == 0 && hasInputFocus())
    {
        if (isReadOnly())
        {
            Window::onSemanticInputEvent(e);
            return;
        }

        if (getSelectionLength() == 0 && isSelectionSemanticValue(e.d_semanticValue))
            d_dragAnchorIdx = d_caretPos;

        WindowEventArgs args(this);
        switch (e.d_semanticValue)
        {
        case SV_DeletePreviousCharacter:
            handleBackspace();
            break;

        case SV_DeleteNextCharacter:
            handleDelete();
            break;

        case SV_Confirm:
            // Fire 'input accepted' event
            onTextAcceptedEvent(args);
            break;

        case SV_GoToPreviousCharacter:
            handleCharLeft(false);
            break;

        case SV_GoToNextCharacter:
            handleCharRight(false);
            break;

        case SV_SelectPreviousCharacter:
            handleCharLeft(true);
            break;

        case SV_SelectNextCharacter:
            handleCharRight(true);
            break;

        case SV_GoToPreviousWord:
            handleWordLeft(false);
            break;

        case SV_GoToNextWord:
            handleWordRight(false);
            break;

        case SV_SelectPreviousWord:
            handleWordLeft(true);
            break;

        case SV_SelectNextWord:
            handleWordRight(true);
            break;

        case SV_GoToStartOfLine:
            handleHome(false);
            break;

        case SV_GoToEndOfLine:
            handleEnd(false);
            break;

        case SV_SelectToStartOfLine:
            handleHome(true);
            break;

        case SV_SelectToEndOfLine:
            handleEnd(true);
            break;

        default:
            Window::onSemanticInputEvent(e);
            return;
        }

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//

/*************************************************************************
    Undo/redo
*************************************************************************/
bool Editbox::performUndo()
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

//----------------------------------------------------------------------------//
bool Editbox::performRedo()
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

} // End of  CEGUI namespace section
