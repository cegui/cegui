/***********************************************************************
    created:    Sun Jun 19 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/falagard/TextComponent.h"
#include "CEGUI/WindowRenderer.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/System.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
TextComponent::TextComponent() = default;
TextComponent::~TextComponent() = default;
TextComponent::TextComponent(TextComponent&& obj) noexcept = default;
TextComponent& TextComponent::operator =(TextComponent && other) noexcept = default;

//----------------------------------------------------------------------------//
String TextComponent::getEffectiveText(const Window& wnd) const
{
    // TODO: retrieve a reference to String from String-typed property!
    if (d_textFromProperty)
        return wnd.getProperty(d_text);
    else
        return d_text.empty() ? wnd.getText() : d_text;
}

//----------------------------------------------------------------------------//
void TextComponent::setTextPropertySource(const String& property)
{
    d_textFromProperty = !property.empty();
    if (d_textFromProperty)
        d_text = property;
}

//----------------------------------------------------------------------------//
void TextComponent::setFontPropertySource(const String& property)
{
    d_fontFromProperty = !property.empty();
    if (d_fontFromProperty)
        d_font = property;
}

//----------------------------------------------------------------------------//
void TextComponent::addImageRenderGeometryToWindow_impl(Window& srcWindow, Rectf& destRect,
    const ColourRect* modColours, const Rectf* clipper) const
{
    updateRenderedText(srcWindow, destRect.getSize());

    // Get total formatted height.
    const float textHeight = d_renderedText.getExtents().d_height;

    // Handle dest area adjustments for vertical formatting.
    // Default is VerticalTextFormatting::TopAligned, for which we take no action.
    const auto vertFormatting = d_vertFormatting.get(srcWindow);
    switch (vertFormatting)
    {
        case VerticalTextFormatting::CentreAligned:
            destRect.d_min.y += CoordConverter::alignToPixels(destRect.getHeight() - textHeight) * 0.5f;
            break;
        case VerticalTextFormatting::BottomAligned:
            destRect.d_min.y = destRect.d_max.y - textHeight;
            break;
    }

    // calculate final colours to be used
    ColourRect finalColours;
    initColoursRect(srcWindow, modColours, finalColours);

    // add geometry for text to the target window.
    d_renderedText.createRenderGeometry(srcWindow.getGeometryBuffers(), destRect.getPosition(), &finalColours, clipper);
}

//----------------------------------------------------------------------------//
Font* TextComponent::getFontObject(const Window& window) const
{
    try
    {
        return d_fontFromProperty ?
            &FontManager::getSingleton().get(window.getProperty(d_font)) :
            (d_font.empty() ? window.getEffectiveFont() : &FontManager::getSingleton().get(d_font));
    }
    catch (UnknownObjectException&)
    {
        return nullptr;
    }
}

//----------------------------------------------------------------------------//
void TextComponent::writeXMLToStream(XMLSerializer& xml_stream) const
{
    // opening tag
    xml_stream.openTag(Falagard_xmlHandler::TextComponentElement);
    // write out area
    d_area.writeXMLToStream(xml_stream);

    // write font property element
    if (d_fontFromProperty)
    {
        xml_stream.openTag(Falagard_xmlHandler::FontPropertyElement)
            .attribute(Falagard_xmlHandler::NameAttribute, d_font)
            .closeTag();
    }

    // write text property element
    if (d_textFromProperty)
    {
        xml_stream.openTag(Falagard_xmlHandler::TextPropertyElement)
            .attribute(Falagard_xmlHandler::NameAttribute, d_text)
            .closeTag();
    }

    // write text element
    const bool writeFont = !d_fontFromProperty && !d_font.empty();
    const bool writeText = !d_textFromProperty && !d_text.empty();
    if (writeFont || writeText)
    {
        xml_stream.openTag(Falagard_xmlHandler::TextElement);
        if (writeFont)
            xml_stream.attribute(Falagard_xmlHandler::FontAttribute, d_font);
        if (writeText)
            xml_stream.attribute(Falagard_xmlHandler::StringAttribute, d_text);
        xml_stream.closeTag();
    }

    // get base class to write colours
    writeColoursXML(xml_stream);

    d_vertFormatting.writeXMLToStream(xml_stream);
    d_horzFormatting.writeXMLToStream(xml_stream);

    // Write WordWrap in a new format
    // TODO: refactor all falagard prop/values to the same format!
    {
        xml_stream.openTag(Falagard_xmlHandler::WordWrapElement);
        if (d_wordWrapProperty.empty())
            xml_stream.attribute(Falagard_xmlHandler::ValueAttribute, PropertyHelper<bool>::toString(d_wordWrap));
        else
            xml_stream.attribute(Falagard_xmlHandler::PropertyAttribute, d_wordWrapProperty);
        xml_stream.closeTag();
    }

    d_paragraphDir.writeXMLToStream(xml_stream);

    // closing tag
    xml_stream.closeTag();
}

//----------------------------------------------------------------------------//
const Sizef& TextComponent::getTextExtent(const Window& window) const
{
    updateRenderedText(window, d_area.getPixelRect(window).getSize());
    return d_renderedText.getExtents();
}

//----------------------------------------------------------------------------//
bool TextComponent::handleFontRenderSizeChange(Window& window, const Font* font) const
{
    const bool res = FalagardComponentBase::handleFontRenderSizeChange(window, font);

    if (font == getFontObject(window))
    {
        window.invalidate();
        return true;
    }

    return res;
}

//----------------------------------------------------------------------------//
TextParser* TextComponent::getTextParser(const Window& window) const
{
    if (auto renderer = window.getWindowRenderer())
    {
        // Don't parse text if it is explicitly disabled
        if (!renderer->isTextParsingEnabled())
            return nullptr;

        // Prefer a custom parser assigned to this Window
        if (auto parser = renderer->getTextParser())
            return parser;
    }

    // Otherwise use a global default parser
    return CEGUI::System::getSingleton().getDefaultTextParser();
}

//------------------------------------------------------------------------//
void TextComponent::updateRenderedText(const Window& srcWindow, const Sizef& size) const
{
    Font* font = getFontObject(srcWindow);
    if (!font)
        throw InvalidRequestException("TextComponent > Window doesn't have a font.");

    // TODO: get const String& for string properties!
    //???TODO TEXT: detect text change in the window?! Can avoid copying and comparison here!
    String text = getEffectiveText(srcWindow);
    TextParser* parser = getTextParser(srcWindow);
    auto bidiDir = d_paragraphDir.get(srcWindow);
    if (d_lastFont != font || d_lastParser != parser || d_lastBidiDir != bidiDir || d_lastText != text)
    {
        d_renderedText.renderText(text, parser, font, bidiDir);

        d_lastFont = font;
        d_lastParser = parser;
        d_lastBidiDir = bidiDir;
        d_lastText = std::move(text);
    }

    bool wordWrap = false;
    auto hfmt = decomposeHorizontalFormatting(d_horzFormatting.get(srcWindow), &wordWrap);
    if (!wordWrap)
    {
        if (d_wordWrapProperty.empty())
            wordWrap = d_wordWrap;
        else
            wordWrap = srcWindow.getProperty<bool>(d_wordWrapProperty);
    }

    d_renderedText.setHorizontalFormatting(hfmt);
    d_renderedText.setWordWrapEnabled(wordWrap);
    d_renderedText.updateDynamicObjectExtents(&srcWindow);
    d_renderedText.updateFormatting(size.d_width);
}

}
