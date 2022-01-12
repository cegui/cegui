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
#include "CEGUI/falagard/XMLHandler.h"
#include "CEGUI/WindowRenderer.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/System.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/LeftAlignedRenderedString.h"
#include "CEGUI/RightAlignedRenderedString.h"
#include "CEGUI/CentredRenderedString.h"
#include "CEGUI/JustifiedRenderedString.h"
#include "CEGUI/RenderedStringWordWrapper.h"
#include "CEGUI/RenderedStringParser.h"
#if defined (CEGUI_USE_FRIBIDI)
    #include "CEGUI/FribidiVisualMapping.h"
#elif defined (CEGUI_USE_MINIBIDI)
    #include "CEGUI/MinibidiVisualMapping.h"
#endif

namespace CEGUI
{

//----------------------------------------------------------------------------//
TextComponent::TextComponent() :
    d_vertFormatting(VerticalTextFormatting::TopAligned),
    d_horzFormatting(HorizontalTextFormatting::LeftAligned),
    d_paragraphDir(DefaultParagraphDirection::Automatic)
{
}

//----------------------------------------------------------------------------//
TextComponent::TextComponent(const TextComponent& obj) :
    FalagardComponentBase(obj),
    d_text(obj.d_text),
    d_renderedString(obj.d_renderedString),
    d_lastFont(obj.d_lastFont),
    d_lastParser(obj.d_lastParser),
    d_lastText(obj.d_lastText),
    d_font(obj.d_font),
    d_vertFormatting(obj.d_vertFormatting),
    d_horzFormatting(obj.d_horzFormatting),
    d_paragraphDir(obj.d_paragraphDir),
    d_textFromProperty(obj.d_textFromProperty),
    d_fontFromProperty(obj.d_fontFromProperty)
{
    setupStringFormatter(obj.d_lastHorzFormatting);
}

//----------------------------------------------------------------------------//
TextComponent::~TextComponent() = default;

//----------------------------------------------------------------------------//
TextComponent& TextComponent::operator =(const TextComponent& other)
{
    if (this == &other)
        return *this;

    FalagardComponentBase::operator=(other);

    d_text = other.d_text;
    // note we do not assign the BidiVisualMapping object, we just mark our
    // existing one as invalid so it's data gets regenerated next time it's
    // needed.
    d_renderedString = other.d_renderedString;
    d_formatter.reset();
    d_lastFont = other.d_lastFont;
    d_lastParser = other.d_lastParser;
    d_lastText = other.d_lastText;
    d_font = other.d_font;
    d_vertFormatting = other.d_vertFormatting;
    d_horzFormatting = other.d_horzFormatting;
    d_paragraphDir = other.d_paragraphDir;
    d_textFromProperty = other.d_textFromProperty;
    d_fontFromProperty = other.d_fontFromProperty;

    setupStringFormatter(other.d_lastHorzFormatting);

    return *this;
}

//----------------------------------------------------------------------------//
String TextComponent::getEffectiveText(const Window& wnd) const
{
    //!!!FIXME TEXT: retrieve a reference to String from String-typed property!
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
void TextComponent::setupStringFormatter(HorizontalTextFormatting horzFormatting) const
{
    // no formatting change
    if (d_formatter && horzFormatting == d_lastHorzFormatting)
    {
        d_formatter->setRenderedString(d_renderedString);
        return;
    }

    d_lastHorzFormatting = horzFormatting;

    switch (horzFormatting)
    {
        case HorizontalTextFormatting::LeftAligned:
            d_formatter.reset(new LeftAlignedRenderedString(d_renderedString));
            break;

        case HorizontalTextFormatting::CentreAligned:
            d_formatter.reset(new CentredRenderedString(d_renderedString));
            break;

        case HorizontalTextFormatting::RightAligned:
            d_formatter.reset(new RightAlignedRenderedString(d_renderedString));
            break;

        case HorizontalTextFormatting::Justified:
            d_formatter.reset(new JustifiedRenderedString(d_renderedString));
            break;

        case HorizontalTextFormatting::WordWrapLeftAligned:
            d_formatter.reset(new RenderedStringWordWrapper<LeftAlignedRenderedString>(d_renderedString));
            break;

        case HorizontalTextFormatting::WordWrapCentreAligned:
            d_formatter.reset(new RenderedStringWordWrapper<CentredRenderedString>(d_renderedString));
            break;

        case HorizontalTextFormatting::WordWrapRightAligned:
            d_formatter.reset(new RenderedStringWordWrapper<RightAlignedRenderedString>(d_renderedString));
            break;

        case HorizontalTextFormatting::WordWraperJustified:
            d_formatter.reset(new RenderedStringWordWrapper<JustifiedRenderedString>(d_renderedString));
            break;

        default:
            d_formatter.reset();
            break;
    }
}

//----------------------------------------------------------------------------//
void TextComponent::addImageRenderGeometryToWindow_impl(Window& srcWindow, Rectf& destRect,
    const ColourRect* modColours, const Rectf* clipper, bool /*clipToDisplay*/) const
{
    updateFormatting(srcWindow, destRect.getSize());

    // Get total formatted height.
    const float textHeight = d_formatter->getVerticalExtent(&srcWindow);

    // handle dest area adjustments for vertical formatting.
    const auto vertFormatting = d_vertFormatting.get(srcWindow);
    switch (vertFormatting)
    {
        case VerticalTextFormatting::CentreAligned:
            destRect.d_min.y += (destRect.getHeight() - textHeight) * 0.5f;
            break;
        case VerticalTextFormatting::BottomAligned:
            destRect.d_min.y = destRect.d_max.y - textHeight;
            break;
        default:
            // default is VerticalTextFormatting::TOP_ALIGNED, for which we take no action.
            break;
    }

    // calculate final colours to be used
    ColourRect finalColours;
    initColoursRect(srcWindow, modColours, finalColours);

    // add geometry for text to the target window.
    auto geomBuffers = d_formatter->createRenderGeometry(
        &srcWindow,
        destRect.getPosition(),
        &finalColours,
        clipper);

    srcWindow.appendGeometryBuffers(geomBuffers);
}

//----------------------------------------------------------------------------//
const Font* TextComponent::getFontObject(const Window& window) const
{
    try
    {
        return d_fontFromProperty ?
            &FontManager::getSingleton().get(window.getProperty(d_font)) :
            (d_font.empty() ? window.getActualFont() : &FontManager::getSingleton().get(d_font));
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
    d_paragraphDir.writeXMLToStream(xml_stream);

    // closing tag
    xml_stream.closeTag();
}

//!!!TODO TEXT: unify extents!
//----------------------------------------------------------------------------//
float TextComponent::getHorizontalTextExtent(const Window& window) const
{
    updateFormatting(window, d_area.getPixelRect(window).getSize());
    return d_formatter->getHorizontalExtent(&window);
}

//----------------------------------------------------------------------------//
float TextComponent::getVerticalTextExtent(const Window& window) const
{
    updateFormatting(window, d_area.getPixelRect(window).getSize());
    return d_formatter->getVerticalExtent(&window);
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
RenderedStringParser& TextComponent::getRenderedStringParser(const Window& window) const
{
    // if parsing is disabled, we use a DefaultRenderedStringParser that creates
    // a RenderedString to renderi the input text verbatim (i.e. no parsing).
    if (!window.getWindowRenderer() || !window.getWindowRenderer()->isTextParsingEnabled())
        return CEGUI::System::getSingleton().getDefaultRenderedStringParser();

    // Next prefer a custom RenderedStringParser assigned to this Window.
    if (auto renderer = window.getWindowRenderer())
        if (auto parser = renderer->getCustomRenderedStringParser())
            return *parser;

    // Next prefer any globally set RenderedStringParser.
    if (auto parser = CEGUI::System::getSingleton().getDefaultCustomRenderedStringParser())
        return *parser;

    // if parsing is enabled and no custom RenderedStringParser is set anywhere,
    // use the system's BasicRenderedStringParser to do the parsing.
    return CEGUI::System::getSingleton().getBasicRenderedStringParser();
}

//------------------------------------------------------------------------//
void TextComponent::updateRenderedString(const Window& srcWindow, const String& text, const Font* font) const
{
    RenderedStringParser& parser = getRenderedStringParser(srcWindow);

    if (d_lastFont == font && d_lastParser == &parser && d_lastText == text)
        return;

#if defined(CEGUI_BIDI_SUPPORT)
#if defined (CEGUI_USE_FRIBIDI)
    FribidiVisualMapping bidiVisualMapping;
#elif defined (CEGUI_USE_MINIBIDI)
    MinibidiVisualMapping bidiVisualMapping;
#else
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
    bidiVisualMapping.updateVisual(text);
    const String& textVisual = bidiVisualMapping.getTextVisual();
#else
    const String& textVisual = text;
#endif

    d_renderedString = parser.parse(textVisual, font, nullptr, d_paragraphDir.get(srcWindow));

    d_lastFont = font;
    d_lastParser = &parser;
    d_lastText = text;
}

//------------------------------------------------------------------------//
void TextComponent::updateFormatting(const Window& srcWindow, const Sizef& size) const
{
    const Font* font = getFontObject(srcWindow);
    if (!font)
        throw InvalidRequestException("TextComponent > Window doesn't have a font.");

    // NB: made so to pass text by reference where possible
    //!!!FIXME TEXT: retrieve a reference to String from String-typed property!
    if (d_textFromProperty)
        updateRenderedString(srcWindow, srcWindow.getProperty(d_text), font);
    else
        updateRenderedString(srcWindow, d_text.empty() ? srcWindow.getText() : d_text, font);

    setupStringFormatter(d_horzFormatting.get(srcWindow));
    d_formatter->format(&srcWindow, size);   
}

}
