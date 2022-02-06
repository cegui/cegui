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
#include "CEGUI/text/TextUtils.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/Clipboard.h"
#include "CEGUI/UndoHandler.h"

namespace CEGUI
{
const String Editbox::EventNamespace("Editbox");
const String Editbox::WidgetTypeName("CEGUI/Editbox");
const String Editbox::EventTextAccepted("TextAccepted");

//----------------------------------------------------------------------------//
EditboxWindowRenderer::EditboxWindowRenderer(const String& name) :
    WindowRenderer(name, Editbox::EventNamespace)
{
}

//----------------------------------------------------------------------------//
Editbox::Editbox(const String& type, const String& name)
    : EditboxBase(type, name)
{
    d_renderedText.setWordWrappingEnabled(false);
}

//----------------------------------------------------------------------------//
void Editbox::setTextFormatting(HorizontalTextFormatting format)
{
    if (format != HorizontalTextFormatting::LeftAligned &&
        format != HorizontalTextFormatting::RightAligned &&
        format != HorizontalTextFormatting::CentreAligned)
    {
        throw InvalidRequestException(
            "currently only HorizontalTextFormatting::LeftAligned, "
            "HorizontalTextFormatting::RightAligned and "
            "HorizontalTextFormatting::CentreAligned are accepted for Editbox formatting");
    }

    EditboxBase::setTextFormatting(format);
}

//----------------------------------------------------------------------------//
void Editbox::ensureCaretIsVisible()
{
    //!!!TODO TEXT: IMPLEMENT! Delegate to the renderer because it knows the size of the caret itself?
    FIXME;
}

//----------------------------------------------------------------------------//
size_t Editbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
    if (!d_windowRenderer)
        throw InvalidRequestException("This function must be implemented by the window renderer");

    return static_cast<EditboxWindowRenderer*>(d_windowRenderer)->getTextIndexFromPosition(pt);
}

//----------------------------------------------------------------------------//
void Editbox::onSemanticInputEvent(SemanticEventArgs& e)
{
    if (isDisabled())
        return;

    if (e.d_semanticValue == SemanticValue::SelectAll && e.d_payload.source == CursorInputSource::Left)
    {
        handleSelectAll();
        ++e.handled;
    }
    else if (e.d_semanticValue == SemanticValue::SelectWord && e.d_payload.source == CursorInputSource::Left)
    {
        // if masked, set up to select all
        if (isTextMaskingEnabled())
        {
            d_dragAnchorIdx = 0;
            setCaretIndex(getText().size());
        }
        // not masked, so select the word that was double-clicked.
        else
        {
            d_dragAnchorIdx = TextUtils::getWordStartIndex(getText(),
                (d_caretPos == getText().size()) ? d_caretPos : d_caretPos + 1);
            d_caretPos = TextUtils::getNextWordStartIndex(getText(), d_caretPos);
        }

        // perform actual selection operation.
        setSelection(d_dragAnchorIdx, d_caretPos);

        ++e.handled;
    }

    if (e.handled || !hasInputFocus())
        return;

    if (isReadOnly())
    {
        Window::onSemanticInputEvent(e);
        return;
    }

    if (!getSelectionLength() && isSelectionSemanticValue(e.d_semanticValue))
        d_dragAnchorIdx = d_caretPos;

    // Check if the semantic value to be handled is of a general type and can thus be
    // handled via common EditboxBase handlers
    if (handleBasicSemanticValue(e))
    {
        ++e.handled;
        return;
    }

    // If the semantic value was not handled, check for specific values
    switch (e.d_semanticValue)
    {
        case SemanticValue::Confirm:
        {
            WindowEventArgs args(this);
            onTextAcceptedEvent(args);
            break;
        }

        default:
            Window::onSemanticInputEvent(e);
            return;
    }

    ++e.handled;
}

//----------------------------------------------------------------------------//
void Editbox::onTextAcceptedEvent(WindowEventArgs& e)
{
    fireEvent(EventTextAccepted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool Editbox::validateWindowRenderer(const WindowRenderer* renderer) const
{
    return dynamic_cast<const EditboxWindowRenderer*>(renderer) != nullptr;
}

}
