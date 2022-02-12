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
#include "CEGUI/text/Font.h"
#include "CEGUI/CoordConverter.h"

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
void Editbox::updateFormatting()
{
    if (auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer))
        d_renderedText.updateFormatting(wr->getTextRenderArea().getWidth());
}

//----------------------------------------------------------------------------//
void Editbox::ensureCaretIsVisible()
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return;

    updateRenderedText();

    //!!!TODO TEXT: IMPLEMENT! Delegate ensureCaretIsVisible to the renderer because it knows
    // the size of the caret itself? or get caret width from EditboxBaseRenderer?
    const float caretWidth = 10.f;

    const float extentToCaretVisual = d_renderedText.getCodepointBounds(getCaretIndex()).left();
    float extentToCaretLogical = extentToCaretVisual;
    switch (d_textFormatting)
    {
        case HorizontalTextFormatting::CentreAligned:
            extentToCaretLogical = (d_renderedText.getExtents().d_height - caretWidth) / 2.f;
            break;
        case HorizontalTextFormatting::RightAligned:
            extentToCaretLogical = extentToCaretVisual - caretWidth;
            break;
    }

    // Update text offset if caret is to the left or to the right of the box
    if ((d_textOffset + extentToCaretLogical) < 0)
        d_textOffset = -extentToCaretLogical;
    else if ((d_textOffset + extentToCaretLogical) >= (d_renderedText.getAreaWidth() - caretWidth))
        d_textOffset = d_renderedText.getAreaWidth() - extentToCaretLogical - caretWidth;
}

//----------------------------------------------------------------------------//
size_t Editbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return getText().size();

    const Font* font = getActualFont();
    if (!font)
        return getText().length();

    String visualText;
    if (d_textMaskingEnabled)
        visualText.assign(getText().length(), static_cast<char32_t>(d_textMaskingCodepoint));
    else
        visualText.assign(getText());

    float textOffset = d_textOffset;
    switch (d_textFormatting)
    {
        case HorizontalTextFormatting::CentreAligned:
            textOffset = (wr->getTextRenderArea().getWidth() - d_renderedText.getExtents().d_height) / 2.f;
            break;
        case HorizontalTextFormatting::RightAligned:
            textOffset = wr->getTextRenderArea().getWidth() - d_textOffset - d_renderedText.getExtents().d_height;
            break;
    }

    const float wndx = CoordConverter::screenToWindowX(*this, pt.x) - textOffset;
    return font->getCharAtPixel(visualText, wndx);

}

//----------------------------------------------------------------------------//
bool Editbox::processSemanticInputEvent(const SemanticEventArgs& e)
{
    switch (e.d_semanticValue)
    {
        case SemanticValue::Confirm:
        {
            WindowEventArgs args(this);
            onTextAcceptedEvent(args);
            return true;
        }
    }

    return EditboxBase::processSemanticInputEvent(e);
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
