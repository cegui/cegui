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
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/System.h"
#include "CEGUI/Image.h"
#include "CEGUI/BitmapImage.h"

#include <iterator>

namespace CEGUI
{
//----------------------------------------------------------------------------//
// amount of bits in a uint
#define BITS_PER_UINT   (sizeof (unsigned int) * 8)
// must be a power of two
#define GLYPHS_PER_PAGE 256

//----------------------------------------------------------------------------//
const argb_t Font::DefaultColour = 0xFFFFFFFF;
String Font::d_defaultResourceGroup;

//----------------------------------------------------------------------------//
const String Font::EventNamespace("Font");
const String Font::EventRenderSizeChanged("RenderSizeChanged");

//----------------------------------------------------------------------------//
Font::Font(const String& name, const String& type_name, const String& filename,
           const String& resource_group, const AutoScaledMode auto_scaled,
           const Sizef& native_res):
    d_name(name),
    d_type(type_name),
    d_filename(filename),
    d_resourceGroup(resource_group),
    d_ascender(0),
    d_descender(0),
    d_height(0),
    d_autoScaled(auto_scaled),
    d_nativeResolution(native_res),
    d_maxCodepoint(0),
    d_loadedGlyphPages(0)
{
    addFontProperties();

    const Sizef size(System::getSingleton().getRenderer()->getDisplaySize());
    Image::computeScalingFactors(d_autoScaled, size, d_nativeResolution,
                                 d_horzScaling, d_vertScaling);
}

//----------------------------------------------------------------------------//
Font::~Font()
{
}

//----------------------------------------------------------------------------//
const String& Font::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const String& Font::getTypeName() const
{
    return d_type;
}

//----------------------------------------------------------------------------//
const String& Font::getFileName() const
{
    return d_filename;
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
        0, &Font::getName, ""
    );

    CEGUI_DEFINE_PROPERTY(Font, AutoScaledMode,
        "AutoScaled", "This indicating whether and how to autoscale font depending on "
        "resolution.  Value can be 'false', 'vertical', 'horizontal' or 'true'.",
        &Font::setAutoScaled, &Font::getAutoScaled, ASM_Disabled
    );
}

//----------------------------------------------------------------------------//
void Font::setMaxCodepoint(char32_t codepoint)
{
    d_maxCodepoint = codepoint;

    const unsigned int npages = (codepoint + GLYPHS_PER_PAGE) / GLYPHS_PER_PAGE;
    const unsigned int size = (npages + BITS_PER_UINT - 1) / BITS_PER_UINT;

    d_loadedGlyphPages.resize(size);
    std::fill(d_loadedGlyphPages.begin(), d_loadedGlyphPages.end(), 0);
}

//----------------------------------------------------------------------------//
const FontGlyph* Font::getGlyphData(char32_t codepoint) const
{
    if (codepoint > d_maxCodepoint)
        return nullptr;

    const FontGlyph* const glyph = findFontGlyph(codepoint);

    if (!d_loadedGlyphPages.empty())
    {
        // Check if glyph page has been rasterised
        unsigned int page = codepoint / GLYPHS_PER_PAGE;
        unsigned int mask = 1 << (page & (BITS_PER_UINT - 1));
        if (!(d_loadedGlyphPages[page / BITS_PER_UINT] & mask))
        {
            d_loadedGlyphPages[page / BITS_PER_UINT] |= mask;
            rasterise(codepoint & ~(GLYPHS_PER_PAGE - 1),
                      codepoint | (GLYPHS_PER_PAGE - 1));
        }
    }

    return glyph;
}

//----------------------------------------------------------------------------//
const FontGlyph* Font::findFontGlyph(const char32_t codepoint) const
{
    CodepointMap::const_iterator pos = d_cp_map.find(codepoint);
    if (pos != d_cp_map.end())
    {
        return pos->second;
    }

    return nullptr;
}



float Font::getTextExtent(const String& text, float x_scale) const
{
    float cur_extent = 0.0f;
    float adv_extent = 0.0f;

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (size_t c = 0; c < text.length(); ++c)
    {
        char32_t currentCodePoint = text[c];
        size_t nextIndex = c + 1;
        char32_t nextCodePoint = -1;
        bool isFollowedByAnotherCharacter = false;

        if(nextIndex < text.length())
        {
            nextCodePoint = text[nextIndex];
            isFollowedByAnotherCharacter = true;
        }

        getGlyphExtents(currentCodePoint, nextCodePoint, isFollowedByAnotherCharacter,
            cur_extent, adv_extent, x_scale);
    }
#else
    String::codepoint_iterator codePointIter(text.begin(), text.begin(), text.end());

    if (!codePointIter.isAtEnd())
    {
        // Start "behind" the first code point
        char32_t currentCodePoint = -1;
        char32_t nextCodePoint = *codePointIter;

        // Check the next code point
        while (!codePointIter.isAtEnd())
        {
            nextCodePoint = *codePointIter;
            getGlyphExtents(currentCodePoint, nextCodePoint, true, cur_extent, adv_extent, x_scale);

            ++codePointIter;
            currentCodePoint = nextCodePoint;
        }


        //Add the final character
        getGlyphExtents(nextCodePoint, -1, false, cur_extent, adv_extent, x_scale);
    }
#endif


    return std::max(adv_extent, cur_extent);
}

void Font::getGlyphExtents(
    char32_t currentCodePoint,
    char32_t nextCodePoint,
    bool isFollowedByAnotherCharacter,
    float& cur_extent,
    float& adv_extent,
    float x_scale) const
{
    const FontGlyph* currentGlyph = getGlyphData(currentCodePoint);

    if (currentGlyph != nullptr)
    {
        const FontGlyph* nextGlyph = nullptr;
        
        if(isFollowedByAnotherCharacter)
        {
            nextGlyph = getGlyphData(nextCodePoint);
        }

        float width = currentGlyph->getRenderedAdvance(nextGlyph, x_scale);

        if (adv_extent + width > cur_extent)
        {
            cur_extent = adv_extent + width;
        }

        adv_extent += currentGlyph->getAdvance(x_scale);
    }
}

//----------------------------------------------------------------------------//
float Font::getTextAdvance(const String& text, float x_scale) const
{
    float advance = 0.0f;

#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
    for (size_t c = 0; c < text.length(); ++c)
    {
        if (const FontGlyph* glyph = getGlyphData(text[c]))
        {
            advance += glyph->getAdvance(x_scale);
        }
    }
#else
String::codepoint_iterator currentCodePointIter(text.begin(), text.begin(), text.end());
while (!currentCodePointIter.isAtEnd())
{
    char32_t currentCodePoint = *currentCodePointIter;
    if (const FontGlyph* glyph = getGlyphData(currentCodePoint))
    {
        advance += glyph->getAdvance(x_scale);
    }

    ++currentCodePointIter;
}
#endif

    return advance;
}

//----------------------------------------------------------------------------//
size_t Font::getCharAtPixel(const String& text, size_t start_char, float pixel,
                            float x_scale) const
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
        glyph = getGlyphData(text[c]);

        if (glyph)
        {
            cur_extent += glyph->getAdvance(x_scale);

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
        glyph = getGlyphData(currentCodePoint);

        if (glyph)
        {
            cur_extent += glyph->getAdvance(x_scale);

            if (pixel < cur_extent)
                return currentCodePointIter.getCodeUnitIndexFromStart();
        }

        ++currentCodePointIter;
    }
#endif

    return char_count;
}

std::vector<GeometryBuffer*> Font::createRenderGeometryForText(
    const String& text, const glm::vec2& position,
    const Rectf* clip_rect, const bool clipping_enabled,
    const ColourRect& colours, const float space_extra,
    const float x_scale, const float y_scale) const
{
    float nextGlyphPos = 0.0f;

    return createRenderGeometryForText(
        text, nextGlyphPos, position, clip_rect, clipping_enabled,
        colours, space_extra, x_scale, y_scale);
}

std::vector<GeometryBuffer*> Font::createRenderGeometryForText(
    const String& text, float& nextGlyphPosX, const glm::vec2& position,
    const Rectf* clip_rect, const bool clipping_enabled,
    const ColourRect& colours, const float space_extra,
    const float x_scale, const float y_scale) const
{
    const float base_y = position.y + getBaseline(y_scale);
    glm::vec2 glyph_pos(position);
    std::vector<GeometryBuffer*> geomBuffers;
    GeometryBuffer* textGeometryBuffer = nullptr;

    ImageRenderSettings imgRenderSettings(
        Rectf(), clip_rect,
        clipping_enabled, colours);
    
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
        const FontGlyph* glyph = getGlyphData(currentCodePoint);
        if (glyph != nullptr)
        {  
            const Image* const img = glyph->getImage();

            glyph_pos.y = base_y - (img->getRenderedOffset().y - 
                img->getRenderedOffset().y * y_scale);

            imgRenderSettings.d_destArea =
                Rectf(glyph_pos, glyph->getSize(x_scale, y_scale));

            // We only fully create the first GeometryBuffer
            if(textGeometryBuffer == nullptr)
            {
                std::vector<GeometryBuffer*> currentGeombuffs =
                    img->createRenderGeometry(imgRenderSettings);

                assert(currentGeombuffs.size() <= 1 && "Glyphs are expected to "
                    "be built from a single GeometryBuffer (or none)");

                if(currentGeombuffs.size() == 1)
                {
                    textGeometryBuffer = currentGeombuffs.back();
                }
            }
            else
            {
                // Else we add geometry to the rendering batch of the existing geometry
                img->addToRenderGeometry(*textGeometryBuffer, imgRenderSettings.d_destArea,
                    clip_rect, colours);
            }

            glyph_pos.x += glyph->getAdvance(x_scale);
            // apply extra spacing to space chars
            if (currentCodePoint == ' ')
            {
                glyph_pos.x += space_extra;
            }
        }
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
         ++currentCodePointIter;
#endif
    }

    if (textGeometryBuffer != nullptr)
    {
        geomBuffers.push_back(textGeometryBuffer);
    }

    nextGlyphPosX = glyph_pos.x;

    // Adding a single geometry buffer containing the batched glyphs
    return geomBuffers;
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
const Sizef& Font::getNativeResolution() const
{
    return d_nativeResolution;
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
AutoScaledMode Font::getAutoScaled() const
{
    return d_autoScaled;
}

//----------------------------------------------------------------------------//
void Font::notifyDisplaySizeChanged(const Sizef& size)
{
    Image::computeScalingFactors(d_autoScaled, size, d_nativeResolution,
                                 d_horzScaling, d_vertScaling);

    if (d_autoScaled != ASM_Disabled)
    {
        updateFont();

        FontEventArgs args(this);
        onRenderSizeChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Font::rasterise(char32_t, char32_t) const
{
    // do nothing by default
}

//----------------------------------------------------------------------------//
void Font::writeXMLToStream(XMLSerializer& xml_stream) const
{
    // output starting <Font ... > element
    xml_stream.openTag("Font")
        .attribute(Font_xmlHandler::FontNameAttribute, d_name)
        .attribute(Font_xmlHandler::FontFilenameAttribute, d_filename);

    if (!d_resourceGroup.empty())
        xml_stream.attribute(Font_xmlHandler::FontResourceGroupAttribute,
                             d_resourceGroup);

    if (d_nativeResolution.d_width != DefaultNativeHorzRes)
        xml_stream.attribute(Font_xmlHandler::FontNativeHorzResAttribute,
            PropertyHelper<std::uint32_t>::toString(static_cast<std::uint32_t>(d_nativeResolution.d_width)));

    if (d_nativeResolution.d_height != DefaultNativeVertRes)
        xml_stream.attribute(Font_xmlHandler::FontNativeVertResAttribute,
            PropertyHelper<std::uint32_t>::toString(static_cast<std::uint32_t>(d_nativeResolution.d_height)));

    if (d_autoScaled != ASM_Disabled)
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

} // End of  CEGUI namespace section
