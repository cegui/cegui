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
#if defined (CEGUI_USE_FRIBIDI)
    d_bidiVisualMapping(new FribidiVisualMapping),
#elif defined (CEGUI_USE_MINIBIDI)
    d_bidiVisualMapping(new MinibidiVisualMapping),
#elif defined (CEGUI_BIDI_SUPPORT)
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
    d_formatter(new LeftAlignedRenderedString(d_renderedString)),
    d_lastHorzFormatting(HorizontalTextFormatting::LeftAligned),
    d_vertFormatting(VerticalTextFormatting::TopAligned),
    d_horzFormatting(HorizontalTextFormatting::LeftAligned),
    d_paragraphDir(DefaultParagraphDirection::Automatic)
{
}

//----------------------------------------------------------------------------//
TextComponent::TextComponent(const TextComponent& obj) :
    FalagardComponentBase(obj),
    d_text(obj.d_text),
#if defined (CEGUI_USE_FRIBIDI)
    d_bidiVisualMapping(new FribidiVisualMapping),
#elif defined (CEGUI_USE_MINIBIDI)
    d_bidiVisualMapping(new MinibidiVisualMapping),
#elif defined (CEGUI_BIDI_SUPPORT)
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
    d_renderedString(obj.d_renderedString),
    d_formatter(obj.d_formatter),
    d_lastHorzFormatting(obj.d_lastHorzFormatting),
    d_font(obj.d_font),
    d_vertFormatting(obj.d_vertFormatting),
    d_horzFormatting(obj.d_horzFormatting),
    d_paragraphDir(obj.d_paragraphDir),
    d_ownText(obj.d_ownText),
    d_textFromProperty(obj.d_textFromProperty),
    d_fontFromProperty(obj.d_fontFromProperty)
{
}

//----------------------------------------------------------------------------//
TextComponent::~TextComponent()
{
#ifdef CEGUI_BIDI_SUPPORT
    delete d_bidiVisualMapping;
#endif
}

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
#ifdef CEGUI_BIDI_SUPPORT
    d_bidiDataValid = false;
#endif
    d_renderedString = other.d_renderedString;
    d_formatter = other.d_formatter;
    d_lastHorzFormatting = other.d_lastHorzFormatting;
    d_font = other.d_font;
    d_vertFormatting = other.d_vertFormatting;
    d_horzFormatting = other.d_horzFormatting;
    d_paragraphDir = other.d_paragraphDir;
    d_ownText = other.d_ownText;
    d_textFromProperty = other.d_textFromProperty;
    d_fontFromProperty = other.d_fontFromProperty;

    return *this;
}

//----------------------------------------------------------------------------//
void TextComponent::setText(const String& text)
{
    d_ownText = !text.empty();

#ifdef CEGUI_BIDI_SUPPORT
    d_bidiDataValid = false;
#endif

    if (d_ownText)
    {
        d_text = text;
        d_textFromProperty = false;
    }
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
void TextComponent::setupStringFormatter(const Window& window, const RenderedString& rendered_string) const
{
    const HorizontalTextFormatting horzFormatting = d_horzFormatting.get(window);

    // no formatting change
    if (horzFormatting == d_lastHorzFormatting)
    {
        d_formatter->setRenderedString(rendered_string);
        return;
    }

    d_lastHorzFormatting = horzFormatting;

    switch (horzFormatting)
    {
        case HorizontalTextFormatting::LeftAligned:
            d_formatter.reset(new LeftAlignedRenderedString(rendered_string));
            break;

        case HorizontalTextFormatting::CentreAligned:
            d_formatter.reset(new CentredRenderedString(rendered_string));
            break;

        case HorizontalTextFormatting::RightAligned:
            d_formatter.reset(new RightAlignedRenderedString(rendered_string));
            break;

        case HorizontalTextFormatting::Justified:
            d_formatter.reset(new JustifiedRenderedString(rendered_string));
            break;

        case HorizontalTextFormatting::WordWrapLeftAligned:
            d_formatter.reset(new RenderedStringWordWrapper<LeftAlignedRenderedString>(rendered_string));
            break;

        case HorizontalTextFormatting::WordWrapCentreAligned:
            d_formatter.reset(new RenderedStringWordWrapper<CentredRenderedString>(rendered_string));
            break;

        case HorizontalTextFormatting::WordWrapRightAligned:
            d_formatter.reset(new RenderedStringWordWrapper<RightAlignedRenderedString>(rendered_string));
            break;

        case HorizontalTextFormatting::WordWraperJustified:
            d_formatter.reset(new RenderedStringWordWrapper<JustifiedRenderedString>(rendered_string));
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
    const VerticalTextFormatting vertFormatting = d_vertFormatting.get(srcWindow);

    switch(vertFormatting)
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

    if (d_textFromProperty)
    {
        // write text property element
        xml_stream.openTag(Falagard_xmlHandler::TextPropertyElement)
            .attribute(Falagard_xmlHandler::NameAttribute, d_text)
            .closeTag();
    }
    else
    {
        // write text element
        if (d_ownText && (!d_font.empty() || !d_text.empty()))
        {
            xml_stream.openTag(Falagard_xmlHandler::TextElement);
            if (!d_fontFromProperty && !d_font.empty())
                xml_stream.attribute(Falagard_xmlHandler::FontAttribute, d_font);
            if (!d_text.empty())
                xml_stream.attribute(Falagard_xmlHandler::StringAttribute, d_text);
            xml_stream.closeTag();
        }
    }

    // write font property element
    if (d_fontFromProperty)
    {
        xml_stream.openTag(Falagard_xmlHandler::FontPropertyElement)
            .attribute(Falagard_xmlHandler::NameAttribute, d_font)
            .closeTag();
    }

    // get base class to write colours
    writeColoursXML(xml_stream);

    d_vertFormatting.writeXMLToStream(xml_stream);
    d_horzFormatting.writeXMLToStream(xml_stream);
    d_paragraphDir.writeXMLToStream(xml_stream);

    // closing tag
    xml_stream.closeTag();
}

//----------------------------------------------------------------------------//
const String& TextComponent::getTextVisual() const
{
#if defined(CEGUI_BIDI_SUPPORT)
    // no bidi support
    if (!d_bidiVisualMapping)
        return d_text;

    if (!d_bidiDataValid)
    {
        d_bidiVisualMapping->updateVisual(d_text);
        d_bidiDataValid = true;
    }

    return d_bidiVisualMapping->getTextVisual();
#else
    return d_text;
#endif
}

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
    if (!window.isTextParsingEnabled())
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

#if defined(CEGUI_BIDI_SUPPORT)
    if (d_bidiVisualMapping && !d_bidiDataValid)
    {
        d_bidiVisualMapping->updateVisual(text);
        d_bidiDataValid = true;
    }

    const String& textVisual = d_bidiVisualMapping ? d_bidiVisualMapping->getTextVisual() : text;
#else
    const String& textVisual = text;
#endif

    if (font != d_lastFont || &parser != d_lastParser)
    {
    }

// do we fetch text from a property
    if (d_textFromProperty)
        d_renderedString = parser.parse(getEffectiveVisualText(srcWindow), font, nullptr);
    // do we use a static text string from the looknfeel
    else if (!d_text.empty())
        d_renderedString = parser.parse(getTextVisual(), font, nullptr);
    // do we have to override the font?
    else if (font != srcWindow.getActualFont())
        d_renderedString = parser.parse(srcWindow.getTextVisual(), font, nullptr);
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
    else if (d_ownText)
        updateRenderedString(srcWindow, d_text, font);
    else
        updateRenderedString(srcWindow, srcWindow.getText(), font);

    setupStringFormatter(srcWindow, d_renderedString);
    d_formatter->format(&srcWindow, size);   
}


}
