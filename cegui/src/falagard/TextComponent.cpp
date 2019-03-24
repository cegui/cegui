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
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/falagard/XMLHandler.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Font.h"
#include "CEGUI/LeftAlignedRenderedString.h"
#include "CEGUI/RightAlignedRenderedString.h"
#include "CEGUI/CentredRenderedString.h"
#include "CEGUI/JustifiedRenderedString.h"
#include "CEGUI/RenderedStringWordWrapper.h"
#if defined (CEGUI_USE_FRIBIDI)
    #include "CEGUI/FribidiVisualMapping.h"
#elif defined (CEGUI_USE_MINIBIDI)
    #include "CEGUI/MinibidiVisualMapping.h"
#endif
#if defined(CEGUI_USE_RAQM)
    #include "CEGUI/RaqmTextData.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    TextComponent::TextComponent() :
#if defined (CEGUI_USE_FRIBIDI)
        d_bidiVisualMapping(new FribidiVisualMapping),
        d_bidiDataValid(false),
#elif defined (CEGUI_USE_MINIBIDI)
        d_bidiVisualMapping(new MinibidiVisualMapping),
        d_bidiDataValid(false),
#elif defined (CEGUI_BIDI_SUPPORT)
        #error "BIDI Configuration is inconsistant, check your config!"
#endif
#ifdef CEGUI_USE_RAQM
        d_raqmTextData(nullptr),
        d_raqmTextNeedsUpdate(true),
#endif 
        d_formattedRenderedString(new LeftAlignedRenderedString(d_renderedString)),
        d_lastHorzFormatting(HorizontalTextFormatting::LeftAligned),
        d_vertFormatting(VerticalTextFormatting::TopAligned),
        d_horzFormatting(HorizontalTextFormatting::LeftAligned)
    {
#ifdef CEGUI_USE_RAQM
        d_raqmTextData = new RaqmTextData();
#endif        
    }

    TextComponent::~TextComponent()
    {
#ifdef CEGUI_BIDI_SUPPORT
        delete d_bidiVisualMapping;
#endif
    }

    TextComponent::TextComponent(const TextComponent& obj) :
        FalagardComponentBase(obj),
        d_textLogical(obj.d_textLogical),
#if defined (CEGUI_USE_FRIBIDI)
        d_bidiVisualMapping(new FribidiVisualMapping),
        d_bidiDataValid(false),
#elif defined (CEGUI_USE_MINIBIDI)
        d_bidiVisualMapping(new MinibidiVisualMapping),
        d_bidiDataValid(false),
#elif defined (CEGUI_BIDI_SUPPORT)
        #error "BIDI Configuration is inconsistant, check your config!"
#endif
        
        d_renderedString(obj.d_renderedString),
        d_formattedRenderedString(obj.d_formattedRenderedString),
        d_lastHorzFormatting(obj.d_lastHorzFormatting),
        d_font(obj.d_font),
        d_vertFormatting(obj.d_vertFormatting),
        d_horzFormatting(obj.d_horzFormatting),
        d_textPropertyName(obj.d_textPropertyName),
        d_fontPropertyName(obj.d_fontPropertyName)
    {
    }

    TextComponent& TextComponent::operator=(const TextComponent& other)
    {
        if (this == &other)
            return *this;

        FalagardComponentBase::operator=(other);

        d_textLogical = other.d_textLogical;
        // note we do not assign the BidiVisualMapping object, we just mark our
        // existing one as invalid so it's data gets regenerated next time it's
        // needed.
#ifdef CEGUI_BIDI_SUPPORT
        d_bidiDataValid = false;
#endif
        d_renderedString = other.d_renderedString;
        d_formattedRenderedString = other.d_formattedRenderedString;
        d_lastHorzFormatting = other.d_lastHorzFormatting;
        d_font = other.d_font;
        d_vertFormatting = other.d_vertFormatting;
        d_horzFormatting = other.d_horzFormatting;
        d_textPropertyName = other.d_textPropertyName;
        d_fontPropertyName = other.d_fontPropertyName;

        return *this;
    }

    const String& TextComponent::getText() const
    {
        return d_textLogical;
    }

    void TextComponent::setText(const String& text)
    {
        d_textLogical = text;
#ifdef CEGUI_BIDI_SUPPORT
        d_bidiDataValid = false;
#endif
    }

    const String& TextComponent::getFont() const
    {
        return d_font;
    }

    void TextComponent::setFont(const String& font)
    {
        d_font = font;
    }

    VerticalTextFormatting TextComponent::getVerticalFormatting(const Window& wnd) const
    {
        return d_vertFormatting.get(wnd);
    }

    VerticalTextFormatting TextComponent::getVerticalFormattingFromComponent() const
    {
        return d_vertFormatting.getValue();
    }

    void TextComponent::setVerticalFormatting(VerticalTextFormatting fmt)
    {
        d_vertFormatting.set(fmt);
    }

    HorizontalTextFormatting TextComponent::getHorizontalFormatting(const Window& wnd) const
    {
        return d_horzFormatting.get(wnd);
    }

    HorizontalTextFormatting TextComponent::getHorizontalFormattingFromComponent() const
    {
        return d_horzFormatting.getValue();
    }

    void TextComponent::setHorizontalFormatting(HorizontalTextFormatting fmt)
    {
        d_horzFormatting.set(fmt);
    }

    const String& TextComponent::getHorizontalFormattingPropertySource() const
    {
        return d_horzFormatting.getPropertySource();
    }

    void TextComponent::setHorizontalFormattingPropertySource(
                                                const String& property_name)
    {
        d_horzFormatting.setPropertySource(property_name);
    }

    const String& TextComponent::getVerticalFormattingPropertySource() const
    {
        return d_vertFormatting.getPropertySource();
    }

    void TextComponent::setVerticalFormattingPropertySource(
                                                const String& property_name)
    {
        d_vertFormatting.setPropertySource(property_name);
    }

    void TextComponent::setupStringFormatter(const Window& window,
                                             const RenderedString& rendered_string) const
    {
        const HorizontalTextFormatting horzFormatting = d_horzFormatting.get(window);

        // no formatting change
        if (horzFormatting == d_lastHorzFormatting)
        {
            d_formattedRenderedString->setRenderedString(rendered_string);
            return;
        }

        d_lastHorzFormatting = horzFormatting;

        switch(horzFormatting)
        {
        case HorizontalTextFormatting::LeftAligned:
            d_formattedRenderedString =
                new LeftAlignedRenderedString(rendered_string);
            break;

        case HorizontalTextFormatting::CentreAligned:
            d_formattedRenderedString =
                new CentredRenderedString(rendered_string);
            break;

        case HorizontalTextFormatting::RightAligned:
            d_formattedRenderedString =
                new RightAlignedRenderedString(rendered_string);
            break;

        case HorizontalTextFormatting::Justified:
            d_formattedRenderedString =
                new JustifiedRenderedString(rendered_string);
            break;

        case HorizontalTextFormatting::WordWrapLeftAligned:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <LeftAlignedRenderedString>(rendered_string);
            break;

        case HorizontalTextFormatting::WordWrapCentreAligned:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <CentredRenderedString>(rendered_string);
            break;

        case HorizontalTextFormatting::WordWrapRightAligned:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <RightAlignedRenderedString>(rendered_string);
            break;

        case HorizontalTextFormatting::WordWraperJustified:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <JustifiedRenderedString>(rendered_string);
            break;
        }
    }

    void TextComponent::addImageRenderGeometryToWindow_impl(Window& srcWindow, Rectf& destRect,
      const ColourRect* modColours, const Rectf* clipper,
      bool /*clipToDisplay*/) const
    {
        updateFormatting(srcWindow, destRect.getSize());

        // Get total formatted height.
        const float textHeight = d_formattedRenderedString->getVerticalExtent(&srcWindow);

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
        auto geomBuffers = d_formattedRenderedString->createRenderGeometry(
            &srcWindow,
            destRect.getPosition(),
            &finalColours,
            clipper);

        srcWindow.appendGeometryBuffers(geomBuffers);
    }

    const Font* TextComponent::getFontObject(const Window& window) const
    {
        try
        {
            return d_fontPropertyName.empty() ?
                (d_font.empty() ? window.getFont() : &FontManager::getSingleton().get(d_font))
                : &FontManager::getSingleton().get(window.getProperty(d_fontPropertyName));
        }
        catch (UnknownObjectException&)
        {
            return nullptr;
        }
    }

    void TextComponent::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // opening tag
        xml_stream.openTag(Falagard_xmlHandler::TextComponentElement);
        // write out area
        d_area.writeXMLToStream(xml_stream);

        // write text element
        if (!d_font.empty() || !getText().empty())
        {
            xml_stream.openTag(Falagard_xmlHandler::TextElement);
            if (!d_font.empty())
                xml_stream.attribute(Falagard_xmlHandler::FontAttribute, d_font);
            if (!getText().empty())
                xml_stream.attribute(Falagard_xmlHandler::StringAttribute, getText());
            xml_stream.closeTag();
        }

        // write text property element
        if (!d_textPropertyName.empty())
        {
            xml_stream.openTag(Falagard_xmlHandler::TextPropertyElement)
                .attribute(Falagard_xmlHandler::NameAttribute, d_textPropertyName)
                .closeTag();
        }

        // write font property element
        if (!d_fontPropertyName.empty())
        {
            xml_stream.openTag(Falagard_xmlHandler::FontPropertyElement)
                .attribute(Falagard_xmlHandler::NameAttribute, d_fontPropertyName)
                .closeTag();
        }

        // get base class to write colours
        writeColoursXML(xml_stream);

        d_vertFormatting.writeXMLToStream(xml_stream);
        d_horzFormatting.writeXMLToStream(xml_stream);

        // closing tag
        xml_stream.closeTag();
    }

    bool TextComponent::isTextFetchedFromProperty() const
    {
        return !d_textPropertyName.empty();
    }

    const String& TextComponent::getTextPropertySource() const
    {
        return d_textPropertyName;
    }

    void TextComponent::setTextPropertySource(const String& property)
    {
        d_textPropertyName = property;
    }

    bool TextComponent::isFontFetchedFromProperty() const
    {
        return !d_fontPropertyName.empty();
    }

    const String& TextComponent::getFontPropertySource() const
    {
        return d_fontPropertyName;
    }

    void TextComponent::setFontPropertySource(const String& property)
    {
        d_fontPropertyName = property;
    }

    const String& TextComponent::getTextVisual() const
    {
#if defined(CEGUI_BIDI_SUPPORT)
        // no bidi support
        if (!d_bidiVisualMapping)
            return d_textLogical;

        if (!d_bidiDataValid)
        {
            d_bidiVisualMapping->updateVisual(d_textLogical);
            d_bidiDataValid = true;
        }

        return d_bidiVisualMapping->getTextVisual();
#else
        return d_textLogical;
#endif
    }

    float TextComponent::getHorizontalTextExtent(const Window& window) const
    {
        updateFormatting(window);
        return d_formattedRenderedString->getHorizontalExtent(&window);
    }

    float TextComponent::getVerticalTextExtent(const Window& window) const
    {
        updateFormatting(window);
        return d_formattedRenderedString->getVerticalExtent(&window);
    }

    bool TextComponent::handleFontRenderSizeChange(Window& window,
                                                   const Font* font) const
    {
        const bool res = 
            FalagardComponentBase::handleFontRenderSizeChange(window, font);

        if (font == getFontObject(window))
        {
            window.invalidate();
            return true;
        }

        return res;
    }

    //------------------------------------------------------------------------//
    void TextComponent::updateFormatting(const Window& srcWindow) const
    {
        updateFormatting(srcWindow, d_area.getPixelRect(srcWindow).getSize());
    }

    //------------------------------------------------------------------------//
    void TextComponent::updateFormatting(
      const Window& srcWindow, const Sizef& size) const
    {

        const Font* font = getFontObject(srcWindow);

        // exit if we have no font to use.
        if (!font)
            throw InvalidRequestException("Window doesn't have a font.");

        const RenderedString* rs = &d_renderedString;
        // do we fetch text from a property
        if (!d_textPropertyName.empty())
        {
            // fetch text & do bi-directional reordering as needed
            String vis;
            #ifdef CEGUI_BIDI_SUPPORT
                BidiVisualMapping::StrIndexList l2v, v2l;
                d_bidiVisualMapping->reorderFromLogicalToVisual(
                    srcWindow.getProperty(d_textPropertyName), vis, l2v, v2l);
            #else
                vis = srcWindow.getProperty(d_textPropertyName);
            #endif
            // parse string using parser from Window.
            d_renderedString =
                srcWindow.getRenderedStringParser().parse(vis, font, nullptr);
        }
        // do we use a static text string from the looknfeel
        else if (!getTextVisual().empty())
            // parse string using parser from Window.
            d_renderedString = srcWindow.getRenderedStringParser().
                parse(getTextVisual(), font, nullptr);
        // do we have to override the font?
        else if (font != srcWindow.getFont())
            d_renderedString = srcWindow.getRenderedStringParser().
                parse(srcWindow.getTextVisual(), font, nullptr);
        // use ready-made RenderedString from the Window itself
        else
            rs = &srcWindow.getRenderedString();

        setupStringFormatter(srcWindow, *rs);
        d_formattedRenderedString->format(&srcWindow, size);
    
    }

//----------------------------------------------------------------------------//
String TextComponent::getEffectiveText(const Window& wnd) const
{
    if (!d_textPropertyName.empty())
        return wnd.getProperty(d_textPropertyName);
    else if (d_textLogical.empty())
        return wnd.getText();
    else
        return d_textLogical;
}

//----------------------------------------------------------------------------//
String TextComponent::getEffectiveVisualText(const Window& wnd) const
{
#ifndef CEGUI_BIDI_SUPPORT
    return getEffectiveText(wnd);
#else
    if (!d_textPropertyName.empty())
    {
        String visual;
        BidiVisualMapping::StrIndexList l2v, v2l;
        d_bidiVisualMapping->reorderFromLogicalToVisual(
            wnd.getProperty(d_textPropertyName), visual, l2v, v2l);

        return visual;
    }
    // do we use a static text string from the looknfeel
    else if (d_textLogical.empty())
        return wnd.getTextVisual();
    else
        return getTextVisual();
#endif
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
