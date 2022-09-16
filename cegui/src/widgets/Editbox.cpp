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
#include "CEGUI/GUIContext.h"
#include <algorithm>

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
    d_renderedText.setWordWrapEnabled(false);
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
bool Editbox::insertString(String&& strToInsert)
{
    if (isReadOnly() || strToInsert.empty())
        return false;

    // TODO TEXT: can implement UTF-8 multibyte string friendly remove_if to avoid copying
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    std::u32string utf32String = String::convertUtf8ToUtf32(strToInsert.c_str());
#else
    String& utf32String = strToInsert;
#endif

    // Erase newline characters from the single line editbox input
    utf32String.erase(std::remove_if(utf32String.begin(), utf32String.end(), [](char32_t ch)
    {
        return TextUtils::UTF32_NEWLINE_CHARACTERS.find(ch) != std::u32string::npos;
    }), utf32String.end());

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    strToInsert = String::convertUtf32ToUtf8(utf32String.c_str());
#endif

    return EditboxBase::insertString(std::move(strToInsert));
}

//----------------------------------------------------------------------------//
void Editbox::updateFormatting()
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    const auto textArea = wr->getTextRenderArea();
    d_renderedText.updateFormatting(textArea.getWidth() - wr->getCaretWidth());

    // Center text vertically inside an area
    d_textOffset.y = (d_renderedText.getExtents().d_height - textArea.getHeight()) * 0.5f;
}

//----------------------------------------------------------------------------//
void Editbox::setTextOffsetX(float value)
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    updateRenderedText();

    const float offset = std::max(0.f,
        std::min(value, d_renderedText.getExtents().d_width - d_renderedText.getAreaWidth()));

    if (d_textOffset.x != offset)
    {
        d_textOffset.x = offset;
        invalidate();
    }
}

//----------------------------------------------------------------------------//
void Editbox::ensureCaretIsVisible()
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    updateRenderedText();

    const float caretOffsetX = wr->getCaretRect().left();
    if (caretOffsetX < d_textOffset.x)
        d_textOffset.x = caretOffsetX;
    else if (caretOffsetX > d_textOffset.x + d_renderedText.getAreaWidth())
        d_textOffset.x = caretOffsetX - d_renderedText.getAreaWidth();
}

//----------------------------------------------------------------------------//
void Editbox::processKeyDownEvent(KeyEventArgs& e)
{
    if (d_guiContext->isInputSemantic(SemanticValue::Confirm, e))
    {
        WindowEventArgs args(this);
        onTextAcceptedEvent(args);
        ++e.handled;
    }
    else
    {
        EditboxBase::processKeyDownEvent(e);
    }
}

//----------------------------------------------------------------------------//
void Editbox::onTextAcceptedEvent(WindowEventArgs& e)
{
    fireEvent(EventTextAccepted, e, EventNamespace);
}

}
