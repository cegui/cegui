/***********************************************************************
    filename:   CEGUIEditbox.cpp
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "elements/CEGUIEditbox.h"
#include "CEGUITextUtils.h"
#include "CEGUIExceptions.h"
#include "CEGUIFont.h"
#ifdef CEGUI_HAS_PCRE_REGEX
#   include "CEGUIPCRERegexMatcher.h"
#else
#   include "CEGUIRegexMatcher.h"
#endif
#include "CEGUIBidiVisualMapping.h"
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
EditboxProperties::ReadOnly         Editbox::d_readOnlyProperty;
EditboxProperties::MaskText         Editbox::d_maskTextProperty;
EditboxProperties::MaskCodepoint    Editbox::d_maskCodepointProperty;
EditboxProperties::ValidationString Editbox::d_validationStringProperty;
EditboxProperties::CaretIndex       Editbox::d_caretIndexProperty;
EditboxProperties::SelectionStart   Editbox::d_selectionStartProperty;
EditboxProperties::SelectionLength  Editbox::d_selectionLengthProperty;
EditboxProperties::MaxTextLength    Editbox::d_maxTextLengthProperty;

//----------------------------------------------------------------------------//
const String Editbox::EventNamespace("Editbox");
const String Editbox::WidgetTypeName("CEGUI/Editbox");
const String Editbox::EventReadOnlyModeChanged( "ReadOnlyModeChanged" );
const String Editbox::EventMaskedRenderingModeChanged( "MaskedRenderingModeChanged" );
const String Editbox::EventMaskCodePointChanged( "MaskCodePointChanged" );
const String Editbox::EventValidationStringChanged( "ValidationStringChanged" );
const String Editbox::EventMaximumTextLengthChanged( "MaximumTextLengthChanged" );
const String Editbox::EventTextInvalidated("TextInvalidated");
const String Editbox::EventInvalidEntryAttempted( "InvalidEntryAttempted" );
const String Editbox::EventCaretMoved( "CaretMoved" );
const String Editbox::EventTextSelectionChanged( "TextSelectionChanged" );
const String Editbox::EventEditboxFull("EditboxFull");
const String Editbox::EventTextAccepted("TextAccepted");

//----------------------------------------------------------------------------//
Editbox::Editbox(const String& type, const String& name) :
    Window(type, name),
    d_readOnly(false),
    d_maskText(false),
    d_maskCodePoint('*'),
    d_maxTextLen(String::max_size()),
    d_caretPos(0),
    d_selectionStart(0),
    d_selectionEnd(0),
    d_validator(0),
    d_dragging(false)
{
    addEditboxProperties();

    // override default and disable text parsing
    d_textParsingEnabled = false;

#ifdef CEGUI_HAS_PCRE_REGEX
    d_validator = CEGUI_NEW_AO PCRERegexMatcher;
    // default to accepting all characters
    setValidationString(".*");
#else
    // set copy of validation string to ".*" so getter returns something valid.
    d_validationString = ".*";
#endif
}

//----------------------------------------------------------------------------//
Editbox::~Editbox(void)
{
    delete d_validator;
}

//----------------------------------------------------------------------------//
bool Editbox::hasInputFocus(void) const
{
    return isActive();
}

//----------------------------------------------------------------------------//
bool Editbox::isTextValid(void) const
{
    return isStringValid(getText());
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
    }

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
#ifdef CEGUI_HAS_PCRE_REGEX
    if (validation_string == d_validationString)
        return;

    d_validationString = validation_string;
    d_validator->setRegexString(validation_string);

    // notification
    WindowEventArgs args(this);
    onValidationStringChanged(args);

    // also notify if text is now invalid.
    if (!isTextValid())
    {
        args.handled = 0;
        onTextInvalidatedEvent(args);
    }
#else
    CEGUI_THROW(InvalidRequestException("Editbox::setValidationString: Unable "
        "to set validation string because CEGUI was compiled without regular "
        "expression support"));
#endif
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
void Editbox::setMaskCodePoint(utf32 code_point)
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

            // see if new text is valid
            if (!isTextValid())
            {
                // Trigger Text is invalid event.
                onTextInvalidatedEvent(args);
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
            newText.erase(getSelectionStartIndex(), getSelectionLength());
            setText(newText);

            // trigger notification that text has changed.
            WindowEventArgs args(this);
            onTextChanged(args);
        }

    }

}

//----------------------------------------------------------------------------//
bool Editbox::isStringValid(const String& str) const
{
    return d_validator ? d_validator->matchRegex(str) : true;
}

//----------------------------------------------------------------------------//
void Editbox::onMouseButtonDown(MouseEventArgs& e)
{
    // base class handling
    Window::onMouseButtonDown(e);

    if (e.button == LeftButton)
    {
        // grab inputs
        if (captureInput())
        {
            // handle mouse down
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
void Editbox::onMouseButtonUp(MouseEventArgs& e)
{
    // base class processing
    Window::onMouseButtonUp(e);

    if (e.button == LeftButton)
    {
        releaseInput();
        ++e.handled;
    }

}

//----------------------------------------------------------------------------//
void Editbox::onMouseDoubleClicked(MouseEventArgs& e)
{
    // base class processing
    Window::onMouseDoubleClicked(e);

    if (e.button == LeftButton)
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

}

//----------------------------------------------------------------------------//
void Editbox::onMouseTripleClicked(MouseEventArgs& e)
{
    // base class processing
    Window::onMouseTripleClicked(e);

    if (e.button == LeftButton)
    {
        d_dragAnchorIdx = 0;
        setCaretIndex(getText().length());
        setSelection(d_dragAnchorIdx, d_caretPos);
        ++e.handled;
    }

}

//----------------------------------------------------------------------------//
void Editbox::onMouseMove(MouseEventArgs& e)
{
    // base class processing
    Window::onMouseMove(e);

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
void Editbox::onCharacter(KeyEventArgs& e)
{
    // NB: We are not calling the base class handler here because it propogates
    // inputs back up the window hierarchy, whereas, as a consumer of key
    // events, we want such propogation to cease with us regardless of whether
    // we actually handle the event.

    // fire event.
    fireEvent(EventCharacterKey, e, Window::EventNamespace);

    // only need to take notice if we have focus
    if (e.handled == 0 && hasInputFocus() && !isReadOnly() &&
        getFont()->isCodepointAvailable(e.codepoint))
    {
        // backup current text
        String tmp(getText());
        tmp.erase(getSelectionStartIndex(), getSelectionLength());

        // if there is room
        if (tmp.length() < d_maxTextLen)
        {
            tmp.insert(getSelectionStartIndex(), 1, e.codepoint);

            if (isStringValid(tmp))
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
            }
            else
            {
                // Trigger invalid modification attempted event.
                WindowEventArgs args(this);
                onInvalidEntryAttempted(args);
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
void Editbox::onKeyDown(KeyEventArgs& e)
{
    // NB: We are not calling the base class handler here because it propogates
    // inputs back up the window hierarchy, whereas, as a consumer of key
    // events, we want such propogation to cease with us regardless of whether
    // we actually handle the event.

    // fire event.
    fireEvent(EventKeyDown, e, Window::EventNamespace);

    if (e.handled == 0 && hasInputFocus() && !isReadOnly())
    {
        WindowEventArgs args(this);
        switch (e.scancode)
        {
        case Key::LeftShift:
        case Key::RightShift:
            if (getSelectionLength() == 0)
                d_dragAnchorIdx = d_caretPos;
            break;

        case Key::Backspace:
            handleBackspace();
            break;

        case Key::Delete:
            handleDelete();
            break;

        case Key::Tab:
        case Key::Return:
        case Key::NumpadEnter:
            // Fire 'input accepted' event
            onTextAcceptedEvent(args);
            break;

        case Key::ArrowLeft:
            if (e.sysKeys & Control)
                handleWordLeft(e.sysKeys);
            else
                handleCharLeft(e.sysKeys);
            break;

        case Key::ArrowRight:
            if (e.sysKeys & Control)
                handleWordRight(e.sysKeys);
            else
                handleCharRight(e.sysKeys);
            break;

        case Key::Home:
            handleHome(e.sysKeys);
            break;

        case Key::End:
            handleEnd(e.sysKeys);
            break;

            // default case is now to leave event as (possibly) unhandled.
        default:
            return;
        }

        ++e.handled;
    }

}

//----------------------------------------------------------------------------//
void Editbox::handleBackspace(void)
{
    if (!isReadOnly())
    {
        String tmp(getText());

        if (getSelectionLength() != 0)
        {
            tmp.erase(getSelectionStartIndex(), getSelectionLength());

            if (isStringValid(tmp))
            {
                // erase selection using mode that does not modify getText()
                // (we just want to update state)
                eraseSelectedText(false);

                // set text to the newly modified string
                setText(tmp);
            }
            else
            {
                // Trigger invalid modification attempted event.
                WindowEventArgs args(this);
                onInvalidEntryAttempted(args);
            }

        }
        else if (getCaretIndex() > 0)
        {
            tmp.erase(d_caretPos - 1, 1);

            if (isStringValid(tmp))
            {
                setCaretIndex(d_caretPos - 1);

                // set text to the newly modified string
                setText(tmp);
            }
            else
            {
                // Trigger invalid modification attempted event.
                WindowEventArgs args(this);
                onInvalidEntryAttempted(args);
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
            tmp.erase(getSelectionStartIndex(), getSelectionLength());

            if (isStringValid(tmp))
            {
                // erase selection using mode that does not modify getText()
                // (we just want to update state)
                eraseSelectedText(false);

                // set text to the newly modified string
                setText(tmp);
            }
            else
            {
                // Trigger invalid modification attempted event.
                WindowEventArgs args(this);
                onInvalidEntryAttempted(args);
            }

        }
        else if (getCaretIndex() < tmp.length())
        {
            tmp.erase(d_caretPos, 1);

            if (isStringValid(tmp))
            {
                // set text to the newly modified string
                setText(tmp);
            }
            else
            {
                // Trigger invalid modification attempted event.
                WindowEventArgs args(this);
                onInvalidEntryAttempted(args);
            }

        }

    }

}

//----------------------------------------------------------------------------//
void Editbox::handleCharLeft(uint sysKeys)
{
    if (d_caretPos > 0)
        setCaretIndex(d_caretPos - 1);

    if (sysKeys & Shift)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleWordLeft(uint sysKeys)
{
    if (d_caretPos > 0)
        setCaretIndex(TextUtils::getWordStartIdx(getText(), d_caretPos));

    if (sysKeys & Shift)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleCharRight(uint sysKeys)
{
    if (d_caretPos < getText().length())
        setCaretIndex(d_caretPos + 1);

    if (sysKeys & Shift)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleWordRight(uint sysKeys)
{
    if (d_caretPos < getText().length())
        setCaretIndex(TextUtils::getNextWordStartIdx(getText(), d_caretPos));

    if (sysKeys & Shift)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleHome(uint sysKeys)
{
    if (d_caretPos > 0)
        setCaretIndex(0);

    if (sysKeys & Shift)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
}

//----------------------------------------------------------------------------//
void Editbox::handleEnd(uint sysKeys)
{
    if (d_caretPos < getText().length())
        setCaretIndex(getText().length());

    if (sysKeys & Shift)
        setSelection(d_caretPos, d_dragAnchorIdx);
    else
        clearSelection();
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
void Editbox::onTextInvalidatedEvent(WindowEventArgs& e)
{
    fireEvent(EventTextInvalidated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Editbox::onInvalidEntryAttempted(WindowEventArgs& e)
{
    fireEvent(EventInvalidEntryAttempted , e, EventNamespace);
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
    addProperty(&d_readOnlyProperty);
    addProperty(&d_maskTextProperty);
    addProperty(&d_maskCodepointProperty);
    addProperty(&d_validationStringProperty);
    addProperty(&d_caretIndexProperty);
    addProperty(&d_selectionStartProperty);
    addProperty(&d_selectionLengthProperty);
    addProperty(&d_maxTextLengthProperty);
}

//----------------------------------------------------------------------------//
size_t Editbox::getTextIndexFromPosition(const Vector2& pt) const
{
    if (d_windowRenderer != 0)
    {
        EditboxWindowRenderer* wr = (EditboxWindowRenderer*)d_windowRenderer;
        return wr->getTextIndexFromPosition(pt);
    }
    else
    {
        CEGUI_THROW(InvalidRequestException("Editbox::getTextIndexFromPosition: "
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

} // End of  CEGUI namespace section
