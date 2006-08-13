/***********************************************************************
    filename:   CEGUIFalTextComponent.cpp
    created:    Sun Jun 19 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "falagard/CEGUIFalTextComponent.h"
#include "falagard/CEGUIFalXMLEnumHelper.h"
#include "CEGUIFontManager.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    TextComponent::TextComponent() :
        d_vertFormatting(VTF_TOP_ALIGNED),
        d_horzFormatting(HTF_LEFT_ALIGNED)
    {}

    const String& TextComponent::getText() const
    {
        return d_text;
    }

    void TextComponent::setText(const String& text)
    {
        d_text = text;
    }

    const String& TextComponent::getFont() const
    {
        return d_font;
    }

    void TextComponent::setFont(const String& font)
    {
        d_font = font;
    }

    VerticalTextFormatting TextComponent::getVerticalFormatting() const
    {
        return d_vertFormatting;
    }

    void TextComponent::setVerticalFormatting(VerticalTextFormatting fmt)
    {
        d_vertFormatting = fmt;
    }

    HorizontalTextFormatting TextComponent::getHorizontalFormatting() const
    {
        return d_horzFormatting;
    }

    void TextComponent::setHorizontalFormatting(HorizontalTextFormatting fmt)
    {
        d_horzFormatting = fmt;
    }

    void TextComponent::render_impl(Window& srcWindow, Rect& destRect, float base_z, const CEGUI::ColourRect* modColours, const Rect* clipper, bool clipToDisplay) const
    {
        // get font to use
        Font* font;

        try
        {
            font = d_fontPropertyName.empty() ?
                (d_font.empty() ? srcWindow.getFont() : FontManager::getSingleton().getFont(d_font))
                : FontManager::getSingleton().getFont(srcWindow.getProperty(d_fontPropertyName));
        }
        catch (UnknownObjectException&)
        {
            font = 0;
        }

        // exit if we have no font to use.
        if (!font)
            return;

        HorizontalTextFormatting horzFormatting = d_horzFormatPropertyName.empty() ? d_horzFormatting :
            FalagardXMLHelper::stringToHorzTextFormat(srcWindow.getProperty(d_horzFormatPropertyName));

        VerticalTextFormatting vertFormatting = d_vertFormatPropertyName.empty() ? d_vertFormatting :
            FalagardXMLHelper::stringToVertTextFormat(srcWindow.getProperty(d_vertFormatPropertyName));

        // calculate final colours to be used
        ColourRect finalColours;
        initColoursRect(srcWindow, modColours, finalColours);

        // decide which string to render.
        const String& renderString = d_textPropertyName.empty() ?
            (d_text.empty() ? srcWindow.getText() : d_text)
            : srcWindow.getProperty(d_textPropertyName);

        // calculate height of formatted text
        float textHeight = font->getFormattedLineCount(renderString, destRect, (TextFormatting)horzFormatting) * font->getLineSpacing();

        // handle dest area adjustments for vertical formatting.
        switch(vertFormatting)
        {
        case VTF_CENTRE_ALIGNED:
            destRect.d_top += (destRect.getHeight() - textHeight) * 0.5f;
            break;

        case VTF_BOTTOM_ALIGNED:
            destRect.d_top = destRect.d_bottom - textHeight;
            break;

        default:
            // default is VTF_TOP_ALIGNED, for which we take no action.
            break;
        }

        // offset the font little down so that it's centered within its own spacing
        destRect.d_top += (font->getLineSpacing() - font->getFontHeight()) * 0.5f;
        // add text to the rendering cache for the target window.
        srcWindow.getRenderCache().cacheText(renderString, font, (TextFormatting)horzFormatting, destRect, base_z, finalColours, clipper, clipToDisplay);
    }

    void TextComponent::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // opening tag
        xml_stream.openTag("TextComponent");
        // write out area
        d_area.writeXMLToStream(xml_stream);

        // write text element
        if (!d_font.empty() && !d_text.empty())
        {
            xml_stream.openTag("Text");
            if (!d_font.empty())
                xml_stream.attribute("font", d_font);
            if (!d_text.empty())
                xml_stream.attribute("string", d_text);
            xml_stream.closeTag();
        }

        // write text property element
        if (!d_textPropertyName.empty())
        {
            xml_stream.openTag("TextProperty")
                .attribute("name", d_textPropertyName)
                .closeTag();
        }

        // write font property element
        if (!d_fontPropertyName.empty())
        {
            xml_stream.openTag("FontProperty")
                .attribute("name", d_fontPropertyName)
                .closeTag();
        }

        // get base class to write colours
        writeColoursXML(xml_stream);

        // write vert format, allowing base class to do this for us if a propety is in use
        if (!writeVertFormatXML(xml_stream))
        {
            // was not a property, so write out explicit formatting in use
            xml_stream.openTag("VertFormat")
                .attribute("type", FalagardXMLHelper::vertTextFormatToString(d_vertFormatting))
                .closeTag();
        }

        // write horz format, allowing base class to do this for us if a propety is in use
        if (!writeHorzFormatXML(xml_stream))
        {
            // was not a property, so write out explicit formatting in use
            xml_stream.openTag("HorzFormat")
                .attribute("type", FalagardXMLHelper::horzTextFormatToString(d_horzFormatting))
                .closeTag();
        }

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

} // End of  CEGUI namespace section
