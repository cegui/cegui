/***********************************************************************
    created:    13/4/2004
    author:     Paul D Turner

    purpose:    Implementation of Editbox class widget
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
#include "CEGUI/widgets/Editbox.h"
#include "CEGUI/TextUtils.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Font.h"
#include "CEGUI/Clipboard.h"
#include "CEGUI/BidiVisualMapping.h"
#include "CEGUI/UndoHandler.h"

#include <string.h>

namespace CEGUI
{

EditboxWindowRenderer::EditboxWindowRenderer(const String& name) :
    WindowRenderer(name, Editbox::EventNamespace)
{
}


const String Editbox::EventNamespace("Editbox");
const String Editbox::WidgetTypeName("CEGUI/Editbox");
const String Editbox::EventValidationStringChanged("ValidationStringChanged");
const String Editbox::EventTextValidityChanged("TextValidityChanged");
const String Editbox::ReadOnlyMouseCursorImagePropertyName("ReadOnlyMouseCursorImage");


Editbox::Editbox(const String& type, const String& name) :
    EditboxBase(type, name),
    d_readOnlyMouseCursorImage(nullptr),
    d_validator(System::getSingleton().createRegexMatcher()),
    d_weOwnValidator(true),
    d_validatorMatchState(RegexMatcher::MatchState::Valid),
    d_previousValidityChangeResponse(true)
{
    addEditboxProperties();

    // default to accepting all characters
    if (d_validator)
        setValidationString(".*");
    // set copy of validation string to ".*" so getter returns something valid.
    else
        d_validationString = ".*";
}


Editbox::~Editbox(void)
{
    if (d_weOwnValidator && d_validator)
        System::getSingleton().destroyRegexMatcher(d_validator);
}


Editbox::MatchState Editbox::getTextMatchState() const
{
    return d_validatorMatchState;
}


void Editbox::setValidationString(const String& validation_string)
{
    if (validation_string == d_validationString)
        return;

    if (!d_validator)
        throw InvalidRequestException(
            "Unable to set validation string on Editbox '" + getNamePath() +
            "' because it does not currently have a RegexMatcher validator.");

    d_validationString = validation_string;
    d_validator->setRegexString(validation_string);

    // notification
    WindowEventArgs args(this);
    onValidationStringChanged(args);

    handleValidityChangeForString(getText());
}


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
            d_undoHandler->clearUndoHistory();

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


Editbox::MatchState Editbox::getStringMatchState(const String& str) const
{
    return d_validator ? d_validator->getMatchStateOfString(str) :
                         RegexMatcher::MatchState::Valid;
}


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
    undoSelection.d_type = UndoHandler::UndoActionType::Delete;
    undoSelection.d_startIdx = getSelectionStart();
    undoSelection.d_text = tmp.substr(getSelectionStart(), getSelectionLength());

    tmp.erase(getSelectionStart(), getSelectionLength());

    // if there is room
    if (tmp.length() < d_maxTextLen)
    {
        UndoHandler::UndoAction undo;
        undo.d_type = UndoHandler::UndoActionType::Insert;
        undo.d_startIdx = getCaretIndex();
        undo.d_text = clipboardText;

        tmp.insert(getSelectionStart(), clipboardText);

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
        getFont()->isCodepointAvailable(e.d_character))
    {
        // backup current text
        String tmp(getText());

        UndoHandler::UndoAction undoSelection;
        undoSelection.d_type = UndoHandler::UndoActionType::Delete;
        undoSelection.d_startIdx = getSelectionStart();
        undoSelection.d_text = tmp.substr(getSelectionStart(), getSelectionLength());

        tmp.erase(getSelectionStart(), getSelectionLength());

        // if there is room
        if (tmp.length() < d_maxTextLen)
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UndoActionType::Insert;
            undo.d_startIdx = getSelectionStart();
            undo.d_text = e.d_character;

            const auto oldSize = tmp.size();
            
            tmp.insert(getSelectionStart(), 1, e.d_character);

            const auto insertedCount = tmp.size() - oldSize;

            if (handleValidityChangeForString(tmp))
            {
                // erase selection using mode that does not modify getText()
                // (we just want to update state)
                eraseSelectedText(false);

                // advance caret (done first so we can "do stuff" in event
                // handlers!)
                // In case multiple code point elements are included we need
                // to jump past all of them
                d_caretPos += insertedCount;

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


void Editbox::handleBackspace(void)
{
    if (!isReadOnly())
    {
        String tmp(getText());

        if (getSelectionLength() != 0)
        {
            UndoHandler::UndoAction undoSelection;
            undoSelection.d_type = UndoHandler::UndoActionType::Delete;
            undoSelection.d_startIdx = getSelectionStart();
            undoSelection.d_text = tmp.substr(getSelectionStart(), getSelectionLength());

            tmp.erase(getSelectionStart(), getSelectionLength());

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
            undo.d_type = UndoHandler::UndoActionType::Delete;

#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
            size_t deleteStartPos = d_caretPos - 1;
            size_t deleteLength = 1;
#else
            String::codepoint_iterator caretIter(tmp.begin() + d_caretPos,
                                                 tmp.begin(), tmp.end());
            --caretIter;

            size_t deleteStartPos = caretIter.getCodeUnitIndexFromStart();
            size_t deleteLength = d_caretPos - deleteStartPos;
#endif

            undo.d_startIdx = deleteStartPos;
            undo.d_text = tmp.substr(deleteStartPos, deleteLength);

            tmp.erase(deleteStartPos, deleteLength);

            if (handleValidityChangeForString(tmp))
            {
                setCaretIndex(deleteStartPos);

                // set text to the newly modified string
                setText(tmp);
                d_undoHandler->addUndoHistory(undo);
            }
        }

    }

}


void Editbox::handleDelete(void)
{
    if (!isReadOnly())
    {
        String tmp(getText());

        if (getSelectionLength() != 0)
        {
            UndoHandler::UndoAction undoSelection;
            undoSelection.d_type = UndoHandler::UndoActionType::Delete;
            undoSelection.d_startIdx = getSelectionStart();
            undoSelection.d_text = tmp.substr(getSelectionStart(), getSelectionLength());

            tmp.erase(getSelectionStart(), getSelectionLength());

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
            undo.d_type = UndoHandler::UndoActionType::Delete;
            undo.d_startIdx = d_caretPos;

#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
            size_t eraseLength = 1;
#else
            size_t eraseLength = String::getCodePointSize(tmp[d_caretPos]);
#endif

            undo.d_text = tmp.substr(d_caretPos, eraseLength);

            tmp.erase(d_caretPos, eraseLength);

            if (handleValidityChangeForString(tmp))
            {
                // set text to the newly modified string
                setText(tmp);
                d_undoHandler->addUndoHistory(undo);
            }
        }

    }

}


bool Editbox::validateWindowRenderer(const WindowRenderer* renderer) const
{
	return dynamic_cast<const EditboxWindowRenderer*>(renderer) != nullptr;
}


void Editbox::onValidationStringChanged(WindowEventArgs& e)
{
    fireEvent(EventValidationStringChanged , e, EventNamespace);
}


void Editbox::onTextValidityChanged(RegexMatchStateEventArgs& e)
{
    fireEvent(EventTextValidityChanged, e, EventNamespace);
}


void Editbox::onTextAcceptedEvent(WindowEventArgs& e)
{
    fireEvent(EventTextAccepted, e, EventNamespace);
}


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


void Editbox::addEditboxProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Editbox, String,
          "ValidationString","Property to get/set the validation string Editbox.  Value is a text string.",
          &Editbox::setValidationString, &Editbox::getValidationString, ".*"
    );
}


size_t Editbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
    if (d_windowRenderer != nullptr)
    {
        EditboxWindowRenderer* wr = static_cast<EditboxWindowRenderer*>(d_windowRenderer);
        return wr->getTextIndexFromPosition(pt);
    }
    else
    {
        throw InvalidRequestException(
            "This function must be implemented by the window renderer");
    }
}

void Editbox::onSemanticInputEvent(SemanticEventArgs& e)
{
    if (isDisabled())
        return;

    if (e.d_semanticValue == SemanticValue::SelectAll && e.d_payload.source == CursorInputSource::Left)
    {
        d_dragAnchorIdx = 0;
        setCaretIndex(getText().length());
        setSelection(d_dragAnchorIdx, d_caretPos);
        ++e.handled;
    }
    else if (e.d_semanticValue == SemanticValue::SelectWord && e.d_payload.source == CursorInputSource::Left)
    {
        // if masked, set up to select all
        if (isTextMaskingEnabled())
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

    if (e.handled == 0 && hasInputFocus())
    {
        if (isReadOnly())
        {
            Window::onSemanticInputEvent(e);
            return;
        }

        if (getSelectionLength() == 0 && isSelectionSemanticValue(e.d_semanticValue))
            d_dragAnchorIdx = d_caretPos;

        // Check if the semantic value to be handled is of a general type and can thus be
        // handled via common EditboxBase handlers
        bool isSemanticValueHandled = handleBasicSemanticValue(e);

        // If the semantic value was not handled, check for specific values
        if (!isSemanticValueHandled)
        {
            // We assume it will be handled now, if not it will be set to false in default-case
            isSemanticValueHandled = true;

            switch (e.d_semanticValue)
            {
            case SemanticValue::Confirm:
            {
                WindowEventArgs args(this);
                // Fire 'input accepted' event
                onTextAcceptedEvent(args);
                break;
            }

            case SemanticValue::GoToStartOfLine:
                handleHome(false);
                break;

            case SemanticValue::GoToEndOfLine:
                handleEnd(false);
                break;

            case SemanticValue::SelectToStartOfLine:
                handleHome(true);
                break;

            case SemanticValue::SelectToEndOfLine:
                handleEnd(true);
                break;

            default:
                Window::onSemanticInputEvent(e);
                isSemanticValueHandled = false;
            }
        }

        if (isSemanticValueHandled)
            ++e.handled;
    }
}

}
