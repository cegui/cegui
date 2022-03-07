/***********************************************************************
    created:    14/6/2006
    author:     Andrew Zabolotny
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/text/PixmapFont.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/BitmapImage.h"
#include "CEGUI/text/Font_xmlHandler.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/text/FontGlyph.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/Logger.h"
#include "CEGUI/SharedStringStream.h"

namespace CEGUI
{
static const String BuiltInResourceGroup("*");

//----------------------------------------------------------------------------//
PixmapFont::PixmapFont(const String& font_name, const String& imageset_filename,
                       const String& resource_group,
                       const AutoScaledMode auto_scaled,
                       const Sizef& native_res):
    Font(font_name, Font_xmlHandler::FontTypePixmap, imageset_filename,
         resource_group, auto_scaled, native_res),
    d_origHorzScaling(1.0f),
    d_imagesetOwner(false)
{
    addPixmapFontProperties();

    reinit();
    PixmapFont::updateFont();
}

//----------------------------------------------------------------------------//
PixmapFont::~PixmapFont()
{
    if (d_imagesetOwner)
        ImageManager::getSingleton().destroyImageCollection(d_imageNamePrefix);
}

//----------------------------------------------------------------------------//
void PixmapFont::addPixmapFontProperties()
{
    const String propertyOrigin("PixmapFont");

    CEGUI_DEFINE_PROPERTY(PixmapFont, String,
        "ImageNamePrefix",
        "This is the name prefix used by the images that contain the glyph "
        "imagery for this font.",
        &PixmapFont::setImageNamePrefix, &PixmapFont::getImageNamePrefix, ""
    );

    CEGUI_DEFINE_PROPERTY(PixmapFont, String,
        "Mapping",
        "This is the glyph-to-image mapping font property. It cannot be read. "
        "Format is: codepoint,advance,imagename",
        &PixmapFont::defineMapping, 0, ""
    );
}

//----------------------------------------------------------------------------//
void PixmapFont::reinit()
{
    if (d_imagesetOwner)
        ImageManager::getSingleton().destroyImageCollection(d_imageNamePrefix);

    if (d_resourceGroup == BuiltInResourceGroup)
    {
        d_imageNamePrefix = d_filename;
        d_imagesetOwner = false;
    }
    else
    {
        ImageManager::getSingleton().loadImageset(d_filename, d_resourceGroup);
        // here we assume the imageset name will match the font name
        d_imageNamePrefix = d_name; 
        d_imagesetOwner = true;
    }
}

//----------------------------------------------------------------------------//
void PixmapFont::updateFont()
{
    const float factor = (d_autoScaled != AutoScaledMode::Disabled ? d_horzScaling : 1.0f) / d_origHorzScaling;

    d_ascender = 0;
    d_descender = 0;
    d_height = 0;

    for (const auto& pair : d_codePointToGlyphMap)
    {
        auto& glyph = d_glyphs[pair.second];
        glyph.setAdvance(glyph.getAdvance() * factor);

        Image* img = glyph.getImage();
        if (!img)
            continue;

        if (BitmapImage* bi = dynamic_cast<BitmapImage*>(img))
        {
            bi->setAutoScaled(d_autoScaled);
            bi->setNativeResolution(d_nativeResolution);
        }

        if (img->getRenderedOffset().y < d_ascender)
            d_ascender = img->getRenderedOffset().y;
        if (img->getRenderedSize().d_height + img->getRenderedOffset().y > d_descender)
            d_descender = img->getRenderedSize().d_height + img->getRenderedOffset().y;
    }

    d_ascender = -d_ascender;
    d_descender = -d_descender;
    d_height = d_ascender - d_descender;

    d_origHorzScaling = (d_autoScaled != AutoScaledMode::Disabled) ? d_horzScaling : 1.0f;
}

//----------------------------------------------------------------------------//
FontGlyph* PixmapFont::loadGlyph(uint32_t index)
{
    return (index < d_glyphs.size()) ? const_cast<FontGlyph*>(&d_glyphs[index]) : nullptr;
}

//----------------------------------------------------------------------------//
const FontGlyph* PixmapFont::getGlyph(uint32_t index) const
{
    return (index < d_glyphs.size()) ? const_cast<FontGlyph*>(&d_glyphs[index]) : nullptr;
}

//----------------------------------------------------------------------------//
void PixmapFont::writeXMLToStream_impl (XMLSerializer& xml_stream) const
{
    const float advscale = 1.0f / d_origHorzScaling;
    for (const auto& pair : d_codePointToGlyphMap)
    {
        const auto& glyph = d_glyphs[pair.second];
        xml_stream.openTag("Mapping")
            .attribute(Font_xmlHandler::MappingCodepointAttribute,
                       PropertyHelper<std::uint32_t>::toString(pair.first))
            .attribute(Font_xmlHandler::MappingHorzAdvanceAttribute,
                       PropertyHelper<float>::toString(glyph.getAdvance() * advscale))
            .attribute(Font_xmlHandler::MappingImageAttribute,
                glyph.getImage()->getName());

        xml_stream.closeTag();
    }
}

//----------------------------------------------------------------------------//
void PixmapFont::defineMapping(char32_t codePoint, const String& imageName, float horzAdvance)
{
    if (d_codePointToGlyphMap.find(codePoint) != d_codePointToGlyphMap.end())
        throw InvalidRequestException("PixmapFont::defineMapping - Requesting "
            "adding an already added glyph to the codepoint glyph map.");

    Image& image = ImageManager::getSingleton().get(d_imageNamePrefix + '/' + imageName);

    float adv = (horzAdvance == -1.0f) ?
        std::floor(image.getRenderedSize().d_width + image.getRenderedOffset().x) :
        horzAdvance;

    if (d_autoScaled != AutoScaledMode::Disabled)
        adv *= d_origHorzScaling;

    d_glyphs.emplace_back(codePoint, adv, &image);

    if (image.getRenderedOffset().y < -d_ascender)
        d_ascender = -image.getRenderedOffset().y;
    if (image.getRenderedSize().d_height + image.getRenderedOffset().y > -d_descender)
        d_descender = -(image.getRenderedSize().d_height + image.getRenderedOffset().y);

    d_height = d_ascender - d_descender;

    d_codePointToGlyphMap[codePoint] = static_cast<uint32_t>(d_glyphs.size()) - 1;

    if (codePoint == UnicodeReplacementCharacter)
        d_replacementGlyphIdx = static_cast<uint32_t>(d_glyphs.size()) - 1;
}

//----------------------------------------------------------------------------//
void PixmapFont::defineMapping(const String& value)
{
    CEGUI::String imageName;
    float adv;
    unsigned int codePoint;

    std::stringstream& sstream = SharedStringstream::GetPreparedStream();
    sstream << value;

    sstream >> codePoint;
    if (sstream.fail())
        throw InvalidRequestException("Glyph Mapping does not begin with a "
                                      "code point number: " + value);
    sstream >> mandatoryChar<','>;
    if (sstream.fail())
        throw InvalidRequestException("Glyph Mapping lacks comma separator: " +
                                      value);

    sstream >> adv;
    if (sstream.fail())
        throw InvalidRequestException("Glyph Mapping lacks float value as "
                                       " second parameter : "+ value);
    sstream >> mandatoryChar<','>;

    imageName = sstream.str();

    //" %u , %g , %32s", &codepoint, &adv, img
        throw InvalidRequestException(
            "Bad glyph Mapping specified: " + value);
    
    defineMapping(codePoint, imageName, adv);
}

//----------------------------------------------------------------------------//
void PixmapFont::setImageNamePrefix(const String& name_prefix)
{
    d_resourceGroup = BuiltInResourceGroup;
    d_imageNamePrefix = name_prefix;
    reinit();
}

}
