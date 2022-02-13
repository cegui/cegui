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
void Editbox::updateFormatting()
{
    if (auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer))
        d_renderedText.updateFormatting(wr->getTextRenderArea().getWidth());
}

//----------------------------------------------------------------------------//
void Editbox::ensureCaretIsVisible()
{
    updateRenderedText();

    //!!!TODO TEXT: get caret rect from renderer? The same code as in geometry generation!
    Rectf caretGlyphRect;
    if (!d_renderedText.getTextIndexBounds(getCaretIndex(), caretGlyphRect))
        return;

    const float caretWidth = 5.f; //!!!FIXME TEXT!
    const float areaWidth = d_renderedText.getAreaWidth() - caretWidth;
    const float caretOffsetX = caretGlyphRect.left();

    if (caretOffsetX < d_textOffset)
        d_textOffset = caretOffsetX;
    else if (caretOffsetX > d_textOffset + areaWidth)
        d_textOffset = caretOffsetX - areaWidth;
}

//----------------------------------------------------------------------------//
size_t Editbox::getTextIndexFromPosition(const glm::vec2& pt)
{
    const auto& text = getText();
    if (text.empty())
        return 0;

    auto wr = static_cast<const EditboxWindowRenderer*>(d_windowRenderer);
    if (!wr)
        return 0;

    updateRenderedText();

    //???FIXME TEXT: move to renderer? Should not rely on the same calculations in different places!
    const auto textArea = wr->getTextRenderArea();
    glm::vec2 localPt = CoordConverter::screenToWindow(*this, pt) - textArea.d_min;
    localPt.x += d_textOffset;
    localPt.y -= (textArea.getHeight() - d_renderedText.getExtents().d_height) * 0.5f;

    return d_renderedText.getTextIndexAtPoint(localPt);
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
