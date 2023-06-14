/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/text/Font.h"
#include "CEGUI/text/Font_xmlHandler.h"
#include "CEGUI/text/FontGlyph.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/InputEvent.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
String Font::d_defaultResourceGroup;
const String Font::EventNamespace("Font");
const String Font::EventRenderSizeChanged("RenderSizeChanged");
const char32_t Font::UnicodeReplacementCharacter = 0xFFFD;

//----------------------------------------------------------------------------//
Font::Font(const String& name, const String& type_name, const String& filename,
           const String& resource_group, AutoScaledMode auto_scaled,
           const Sizef& native_res):
    d_name(name),
    d_type(type_name),
    d_filename(filename),
    d_resourceGroup(resource_group),
    d_autoScaled(auto_scaled),
    d_nativeResolution(native_res)
{
    addFontProperties();

    const Sizef size(System::getSingleton().getRenderer()->getDisplaySize());
    Image::computeScalingFactors(d_autoScaled, size, d_nativeResolution,
                                 d_horzScaling, d_vertScaling);
}

//----------------------------------------------------------------------------//
float Font::convertPointsToPixels(float pointSize, int dotsPerInch)
{
    return pointSize * dotsPerInch / 72.f;
}

//----------------------------------------------------------------------------//
float Font::convertPixelsToPoints(float pixelSize, int dotsPerInch)
{
    return pixelSize * 72.f / static_cast<float>(dotsPerInch);
}

//----------------------------------------------------------------------------//
void Font::addFontProperties()
{
    const String propertyOrigin("Font");

    CEGUI_DEFINE_PROPERTY(Font, Sizef,
        "NativeRes", "Native screen resolution for this font."
        "Value uses the 'w:# h:#' format.",
        &Font::setNativeResolution, &Font::getNativeResolution, Sizef::zero()
    );

    CEGUI_DEFINE_PROPERTY(Font, String,
        "Name", "This is font name.  Value is a string.",
        nullptr, &Font::getName, ""
    );

    CEGUI_DEFINE_PROPERTY(Font, AutoScaledMode,
        "AutoScaled", "This indicating whether and how to autoscale font depending on "
        "resolution.  Value can be 'false', 'vertical', 'horizontal' or 'true'.",
        &Font::setAutoScaled, &Font::getAutoScaled, AutoScaledMode::Disabled
    );
}

//----------------------------------------------------------------------------//
uint32_t Font::getGlyphIndexForCodepoint(char32_t codePoint) const
{
    auto it = d_codePointToGlyphMap.find(codePoint);
    return (it != d_codePointToGlyphMap.end()) ? it->second : d_replacementGlyphIdx;
}

//----------------------------------------------------------------------------//
bool Font::isCodepointAvailable(char32_t codePoint) const
{
    return d_codePointToGlyphMap.find(codePoint) != d_codePointToGlyphMap.end();
}

//----------------------------------------------------------------------------//
float Font::getTextExtent(const String& text)
{
    float curExtent = 0.0f;
    float advExtent = 0.0f;

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (const char32_t currentCodePoint : text)
    {
#else
    String::codepoint_iterator currentCodePointIter(text.begin(), text.begin(), text.end());
    while (!currentCodePointIter.isAtEnd())
    {
        const char32_t currentCodePoint = *currentCodePointIter;
#endif
        if (const FontGlyph* currentGlyph = loadGlyph(getGlyphIndexForCodepoint(currentCodePoint)))
        {
            const float width = currentGlyph->getRenderedAdvance();
            if (curExtent < advExtent + width)
                curExtent = advExtent + width;

            advExtent += currentGlyph->getAdvance();
        }
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
        ++currentCodePointIter;
#endif
    }

    return std::max(advExtent, curExtent);
}

//----------------------------------------------------------------------------//
float Font::getTextAdvance(const String& text)
{
    float advance = 0.0f;

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (const char32_t currentCodePoint : text)
    {
#else
    String::codepoint_iterator currentCodePointIter(text.begin(), text.begin(), text.end());
    while (!currentCodePointIter.isAtEnd())
    {
        const char32_t currentCodePoint = *currentCodePointIter;
#endif
        if (const FontGlyph* glyph = loadGlyph(getGlyphIndexForCodepoint(currentCodePoint)))
            advance += glyph->getAdvance();
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
        ++currentCodePointIter;
#endif
    }

    return advance;
}

//----------------------------------------------------------------------------//
size_t Font::getCharAtPixel(const String& text, size_t start_char, float pixel)
{
    const FontGlyph* glyph;
    float cur_extent = 0;
    size_t char_count = text.length();

    // handle simple cases
    if ((pixel <= 0) || (char_count <= start_char))
        return start_char;

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (size_t c = start_char; c < char_count; ++c)
    {
        glyph = loadGlyph(getGlyphIndexForCodepoint(text[c]));

        if (glyph)
        {
            cur_extent += glyph->getAdvance();

            if (pixel < cur_extent)
                return c;
        }
    }
#else
    String::codepoint_iterator currentCodePointIter(text.begin(), text.begin(), text.end());
    currentCodePointIter.increment(start_char);

    while (!currentCodePointIter.isAtEnd())
    {
        char32_t currentCodePoint = *currentCodePointIter;
        glyph = loadGlyph(getGlyphIndexForCodepoint(currentCodePoint));

        if (glyph)
        {
            cur_extent += glyph->getAdvance();

            if (pixel < cur_extent)
                return currentCodePointIter.getCodeUnitIndexFromStart();
        }

        ++currentCodePointIter;
    }
#endif

    return char_count;
}

//----------------------------------------------------------------------------//
void Font::setNativeResolution(const Sizef& size)
{
    if (d_nativeResolution == size)
        return;

    d_nativeResolution = size;

    // re-calculate scaling factors & notify images as required
    notifyDisplaySizeChanged(System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void Font::setAutoScaled(const AutoScaledMode autoScaled)
{
    if (d_autoScaled == autoScaled)
        return;

    d_autoScaled = autoScaled;

    // re-calculate scaling factors & notify images as required
    notifyDisplaySizeChanged(System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void Font::notifyDisplaySizeChanged(const Sizef& size)
{
    const auto oldHorz = d_horzScaling;
    const auto oldVert = d_vertScaling;

    Image::computeScalingFactors(d_autoScaled, size, d_nativeResolution, d_horzScaling, d_vertScaling);

    if (oldHorz != d_horzScaling || oldVert != d_vertScaling)
    {
        updateFont();

        FontEventArgs args(this);
        onRenderSizeChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Font::writeXMLToStream(XMLSerializer& xml_stream) const
{
    // output starting <Font ... > element
    xml_stream.openTag(Font_xmlHandler::FontElement)
        .attribute(Font_xmlHandler::FontNameAttribute, d_name)
        .attribute(Font_xmlHandler::FontVersionAttribute, Font_xmlHandler::NativeVersion)
        .attribute(Font_xmlHandler::FontFilenameAttribute, d_filename)
        .attribute(Font_xmlHandler::FontTypeAttribute, d_type);

    if (!d_resourceGroup.empty())
        xml_stream.attribute(Font_xmlHandler::FontResourceGroupAttribute,
                             d_resourceGroup);

    if (d_nativeResolution.d_width != DefaultNativeHorzRes)
        xml_stream.attribute(Font_xmlHandler::FontNativeHorzResAttribute,
            PropertyHelper<std::uint32_t>::toString(static_cast<std::uint32_t>(d_nativeResolution.d_width)));

    if (d_nativeResolution.d_height != DefaultNativeVertRes)
        xml_stream.attribute(Font_xmlHandler::FontNativeVertResAttribute,
            PropertyHelper<std::uint32_t>::toString(static_cast<std::uint32_t>(d_nativeResolution.d_height)));

    if (d_autoScaled != AutoScaledMode::Disabled)
        xml_stream.attribute(Font_xmlHandler::FontAutoScaledAttribute,
            PropertyHelper<AutoScaledMode>::toString(d_autoScaled));

    writeXMLToStream_impl(xml_stream);

    // output closing </Font> element.
    xml_stream.closeTag();
}

//----------------------------------------------------------------------------//
void Font::onRenderSizeChanged(FontEventArgs& e)
{
    fireEvent(EventRenderSizeChanged, e, EventNamespace);
}

}
