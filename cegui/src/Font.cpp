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
#include "CEGUI/Font.h"
#include "CEGUI/Font_xmlHandler.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/FontGlyph.h"
#include "CEGUI/BitmapImage.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/InputEvent.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
String Font::d_defaultResourceGroup;
const String Font::EventNamespace("Font");
const String Font::EventRenderSizeChanged("RenderSizeChanged");
const argb_t Font::DefaultColour = 0xFFFFFFFF;
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
Font::~Font() = default;

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
float Font::getTextExtent(const String& text) const
{
    float cur_extent = 0.0f;
    float adv_extent = 0.0f;

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (size_t c = 0; c < text.length(); ++c)
    {
        const char32_t currentCodePoint = text[c];

        getGlyphExtents(currentCodePoint, cur_extent, adv_extent);
    }
#else
    String::codepoint_iterator codePointIter(text.begin(), text.begin(), text.end());

    while (!codePointIter.isAtEnd())
    {
        const char32_t currentCodePoint = *codePointIter;

        getGlyphExtents(currentCodePoint, cur_extent, adv_extent);

        ++codePointIter;
    }
#endif


    return std::max(adv_extent, cur_extent);
}

//----------------------------------------------------------------------------//
void Font::getGlyphExtents(char32_t currentCodePoint, float& cur_extent, float& adv_extent) const
{
    if (const FontGlyph* currentGlyph = getGlyphForCodepoint(currentCodePoint, true))
    {
        float width = currentGlyph->getRenderedAdvance();

        if (adv_extent + width > cur_extent)
            cur_extent = adv_extent + width;

        adv_extent += currentGlyph->getAdvance();
    }
}

//----------------------------------------------------------------------------//
float Font::getTextAdvance(const String& text) const
{
    float advance = 0.0f;

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (size_t c = 0; c < text.length(); ++c)
    {
        if (const FontGlyph* glyph = getPreparedGlyph(text[c]))
        {
            advance += glyph->getAdvance();
        }
    }
#else
String::codepoint_iterator currentCodePointIter(text.begin(), text.begin(), text.end());
while (!currentCodePointIter.isAtEnd())
{
    char32_t currentCodePoint = *currentCodePointIter;
    if (const FontGlyph* glyph = getGlyphForCodepoint(currentCodePoint, true))
    {
        advance += glyph->getAdvance();
    }

    ++currentCodePointIter;
}
#endif

    return advance;
}

//----------------------------------------------------------------------------//
size_t Font::getCharAtPixel(const String& text, size_t start_char, float pixel) const
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
        glyph = getGlyphForCodepoint(text[c]);

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
        glyph = getGlyphForCodepoint(currentCodePoint);

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
// The old way of rendering glyphs, without kerning and extended layouting
void Font::layoutAndCreateGlyphRenderGeometry(std::vector<GeometryBuffer*>& out,
    const String& text, float spaceExtra, ImageRenderSettings& imgRenderSettings,
    DefaultParagraphDirection /*defaultParagraphDir*/, glm::vec2& penPosition) const
{
    if (text.empty())
        return;

    penPosition.y += getBaseline();
    const auto canCombineFromIdx = out.size();

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (size_t c = 0; c < text.length(); ++c)
    {
        const char32_t& currentCodePoint = text[c];
#else
    String::codepoint_iterator currentCodePointIter(text.begin(), text.begin(), text.end());
    while (!currentCodePointIter.isAtEnd())
    {
        char32_t currentCodePoint = *currentCodePointIter;
#endif
        if (auto glyph = getGlyphForCodepoint(currentCodePoint, true))
        {  
            if (auto image = glyph->getImage())
            {
                imgRenderSettings.d_destArea = Rectf(penPosition, image->getRenderedSize());
                addGlyphRenderGeometry(out, canCombineFromIdx, image, imgRenderSettings);
            }

            penPosition.x += glyph->getAdvance();
            if (currentCodePoint == ' ')
                penPosition.x += spaceExtra;
        }
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
         ++currentCodePointIter;
#endif
    }
}

//----------------------------------------------------------------------------//
void Font::setNativeResolution(const Sizef& size)
{
    d_nativeResolution = size;

    // re-calculate scaling factors & notify images as required
    notifyDisplaySizeChanged(
        System::getSingleton().getRenderer()->getDisplaySize());
}

//----------------------------------------------------------------------------//
void Font::setAutoScaled(const AutoScaledMode auto_scaled)
{
    if (auto_scaled == d_autoScaled)
        return;

    d_autoScaled = auto_scaled;
    updateFont();

    FontEventArgs args(this);
    onRenderSizeChanged(args);
}

//----------------------------------------------------------------------------//
void Font::notifyDisplaySizeChanged(const Sizef& size)
{
    Image::computeScalingFactors(d_autoScaled, size, d_nativeResolution,
        d_horzScaling, d_vertScaling);

    if (d_autoScaled != AutoScaledMode::Disabled)
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

//----------------------------------------------------------------------------//
void Font::addGlyphRenderGeometry(std::vector<GeometryBuffer*>& textGeometryBuffers,
    size_t canCombineFromIdx, const Image* image, ImageRenderSettings& imgRenderSettings) const
{
    if (!image)
        return;

    //!!!FIXME TEXT: image is implied to be a BitmapImage here, otherwise a crash is possible!

    // We only create a new GeometryBuffer if no existing one is found that we can
    // combine this one with. Render order is irrelevant since glyphs should never overlap.
    auto it = std::find_if(textGeometryBuffers.begin() + canCombineFromIdx, textGeometryBuffers.end(),
        [tex = static_cast<const BitmapImage*>(image)->getTexture()](const GeometryBuffer* buffer)
    {
        return tex == buffer->getTexture("texture0");
    });

    if (it != textGeometryBuffers.end())
        image->addToRenderGeometry(*(*it), imgRenderSettings);
    else
        image->createRenderGeometry(textGeometryBuffers, imgRenderSettings);
}

}
