/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Implements FreeTypeFont class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2017 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/text/FreeTypeFont.h"
#include "CEGUI/text/Font_xmlHandler.h"
#include "CEGUI/Texture.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/Logger.h"
#include "CEGUI/BitmapImage.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/SharedStringStream.h"
#include <freetype/tttables.h>

namespace CEGUI
{
//----------------------------------------------------------------------------//
// Pixels to put between glyphs
static const int s_glyphPadding = 1;
// A multiplication coefficient to convert FT_Pos values into normal floats
static const float s_26dot6_toFloat = (1.0f / 64.f);
// A multiplication coefficient to convert 16.16 fixed point values into normal floats
static const float s_16dot16_toFloat = (1.0f / 65536.0f);
// Font objects usage count
static int s_fontUsageCount = 0;
// A handle to the FreeType library
static FT_Library s_freetypeLibHandle;

//----------------------------------------------------------------------------//

#undef __FTERRORS_H__                                           
#define FT_ERRORDEF( e, v, s )  { e, s },                       
#define FT_ERROR_START_LIST     {                               
#define FT_ERROR_END_LIST       { 0, 0 } };                     
                                                                         
struct FreeTypeErrorDescription                                                 
{                                                               
    int         err_code;                                        
    const char* err_msg;                                         
};

static const FreeTypeErrorDescription ftErrorDescs[] =
#include FT_ERRORS_H 

static const std::vector<FreeTypeErrorDescription> freeTypeErrorDescriptions
    (ftErrorDescs, ftErrorDescs + sizeof(ftErrorDescs) / sizeof(FreeTypeErrorDescription) );

//----------------------------------------------------------------------------//
FreeTypeFont::FreeTypeFont(
    const String& font_name,
    const float size,
    const FontSizeUnit sizeUnit,
    const bool anti_aliased, const String& font_filename,
    const String& resource_group,
    const AutoScaledMode auto_scaled,
    const Sizef& native_res,
    const float specific_line_spacing) :
    Font(font_name, Font_xmlHandler::FontTypeFreeType, font_filename,
         resource_group, auto_scaled, native_res),
    d_specificLineSpacing(specific_line_spacing),
    d_size(size),
    d_sizeUnit(sizeUnit),
    d_antiAliased(anti_aliased)
{
    addFreeTypeFontProperties();

    if (!s_fontUsageCount++)
        FT_Init_FreeType(&s_freetypeLibHandle);

    System::getSingleton().getResourceProvider()->loadRawDataContainer(
        d_filename, d_fontData, d_resourceGroup.empty() ? getDefaultResourceGroup() : d_resourceGroup);

    // create face using input font
    FT_Error error = FT_New_Memory_Face(s_freetypeLibHandle, d_fontData.getDataPtr(),
        static_cast<FT_Long>(d_fontData.getSize()), 0, &d_fontFace);

    if (error != 0)
        findAndThrowFreeTypeError(error, "Failed to create face from font file");

    // Check unicode character map availability
    if (!d_fontFace->charmap)
    {
        FT_Done_Face(d_fontFace);
        d_fontFace = nullptr;
        throw GenericException("The font '" + d_name +
            "' does not have a Unicode charmap, and cannot be used.");
    }

    FT_Stroker_New(s_freetypeLibHandle, &d_stroker);

    FreeTypeFont::updateFont();

    std::stringstream& sstream = SharedStringstream::GetPreparedStream();
    sstream << "Successfully loaded " << d_codePointToGlyphMap.size() << " glyphs";
    Logger::getSingleton().logEvent(sstream.str(), LoggingLevel::Informative);
}

//----------------------------------------------------------------------------//
FreeTypeFont::~FreeTypeFont()
{
    free();

    FT_Stroker_Done(d_stroker);
    d_stroker = nullptr;

    FT_Done_Face(d_fontFace);
    d_fontFace = nullptr;
    System::getSingleton().getResourceProvider()->unloadRawDataContainer(d_fontData);

    if (!--s_fontUsageCount)
        FT_Done_FreeType(s_freetypeLibHandle);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::addFreeTypeFontProperties()
{
    const String propertyOrigin("FreeTypeFont");

    CEGUI_DEFINE_PROPERTY(FreeTypeFont, float,
        "Size", "This is the size of the font.",
        &FreeTypeFont::setSize, &FreeTypeFont::getSize, 0
    );

    CEGUI_DEFINE_PROPERTY(FreeTypeFont, FontSizeUnit,
        "SizeUnit", "This is the point size of the font.",
        &FreeTypeFont::setSizeUnit, &FreeTypeFont::getSizeUnit, FontSizeUnit::Pixels
    );

    CEGUI_DEFINE_PROPERTY(FreeTypeFont, bool,
        "Antialiased", "This is a flag indicating whenever to render antialiased font or not. "
        "Value is either true or false.",
        &FreeTypeFont::setAntiAliased, &FreeTypeFont::isAntiAliased, false
    );
}

//----------------------------------------------------------------------------//
void FreeTypeFont::resizeAndUpdateTexture(Texture* texture, uint32_t newSize)
{
    if (d_lastTextureSize >= newSize)
        throw InvalidRequestException("Must supply a larger than previous size when "
            "resizing the glyph atlas");

    const uint32_t oldTexSize = d_lastTextureSize;
    std::vector<argb_t> oldTextureData;
    std::swap(oldTextureData, d_lastTextureBuffer);

    const float newTexSize = static_cast<float>(newSize);
    d_lastTextureSize = newSize;

    d_lastTextureBuffer.resize(newSize * newSize);
    std::fill(d_lastTextureBuffer.begin(), d_lastTextureBuffer.end(), 0);

    // Copy our memory buffer into the texture and free it
    updateTextureBufferSubImage(d_lastTextureBuffer.data(), oldTexSize, oldTexSize, oldTextureData);

    // TODO: use single channel 8-bit format! Blit/clear with GPU instead of loading from memory?
    texture->loadFromMemory(d_lastTextureBuffer.data(), Sizef(newTexSize, newTexSize), Texture::PixelFormat::Rgba);

    // Fix glyph texcoords in existing text geometry buffers
    System::getSingleton().getRenderer()->updateGeometryBufferTexCoords(texture, oldTexSize / newTexSize);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::createTextureSpaceForGlyphRasterisation(Texture* texture, uint32_t glyphWidth, uint32_t glyphHeight)
{
    const auto maxTextureSize = static_cast<uint32_t>(System::getSingleton().getRenderer()->getMaxTextureSize());

    if (glyphWidth > maxTextureSize || glyphHeight > maxTextureSize)
        throw InvalidRequestException("Can not rasterise a glyph that is larger "
            "than the maximum supported texture size.");
        
    constexpr uint32_t TEXTURE_SIZE_GROW_FACTOR = 2;
    const uint32_t newSize = d_lastTextureSize * TEXTURE_SIZE_GROW_FACTOR;
    if (newSize > maxTextureSize)
        createGlyphAtlasTexture();
    else
        resizeAndUpdateTexture(texture, newSize);
}

//----------------------------------------------------------------------------//
size_t FreeTypeFont::findTextureLineWithFittingSpot(uint32_t glyphWidth, uint32_t glyphHeight) const
{
    // Go through the lines and find one that fits
    const size_t lineCount =  d_textureGlyphLines.size();
    for (size_t i = 0; i < lineCount; ++i)
    {
        const auto& currentGlyphLine = d_textureGlyphLines[i];

        const bool isLastLine = (i == (lineCount - 1));

        // Check if glyph right margin exceeds texture size
        uint32_t curGlyphXEnd = currentGlyphLine.d_lastXPos + glyphWidth;
        uint32_t curGlyphYEnd = currentGlyphLine.d_lastYPos + glyphHeight;
        if (curGlyphXEnd < d_lastTextureSize && curGlyphYEnd < d_lastTextureSize)
        {
            //Only the last line can be extended into the y-dimension
            if (isLastLine)
            {
                if (curGlyphYEnd > currentGlyphLine.d_maximumExtentY)
                    currentGlyphLine.d_maximumExtentY = curGlyphYEnd;

                return i;
            }
            else
            {
                if (curGlyphYEnd < currentGlyphLine.d_maximumExtentY)
                    return i;
            }
        }
    }

    return lineCount;
}

//----------------------------------------------------------------------------//
BitmapImage* FreeTypeFont::rasterise(const String& name, const FT_Bitmap& ft_bitmap, int32_t glyphLeft,
    int32_t glyphTop, uint32_t glyphWidth, uint32_t glyphHeight)
{
    if (d_glyphTextures.empty())
        createGlyphAtlasTexture();

    size_t fittingLineIndex = findTextureLineWithFittingSpot(glyphWidth, glyphHeight);

    if (fittingLineIndex >= d_textureGlyphLines.size())
        fittingLineIndex = addNewLineIfFitting(glyphHeight, glyphWidth);

    if (fittingLineIndex >= d_textureGlyphLines.size())
    {
        createTextureSpaceForGlyphRasterisation(d_glyphTextures.back(), glyphWidth, glyphHeight);
        return rasterise(name, ft_bitmap, glyphLeft, glyphTop, glyphWidth, glyphHeight);
    }

    const TextureGlyphLine& glyphTexLine = d_textureGlyphLines[fittingLineIndex];

    // Create the data containing the pixels of the glyph
    std::vector<argb_t> subTextureData = createGlyphTextureData(ft_bitmap);

    // Update the cached texture data in memory
    size_t bufferDataGlyphPos = (glyphTexLine.d_lastYPos * d_lastTextureSize) + glyphTexLine.d_lastXPos;
    updateTextureBufferSubImage(d_lastTextureBuffer.data() + bufferDataGlyphPos,
        glyphWidth, glyphHeight, subTextureData);

    // Update the sub-image in the texture on the GPU
    const Rectf imageArea(
        glm::vec2(glyphTexLine.d_lastXPos, glyphTexLine.d_lastYPos),
        Sizef(static_cast<float>(glyphWidth), static_cast<float>(glyphHeight)));
    d_glyphTextures.back()->blitFromMemory(subTextureData.data(), imageArea);

    // Advance to next position, add padding
    glyphTexLine.d_lastXPos += glyphWidth + s_glyphPadding;

    // This is the right bearing for bitmap glyphs, not d_fontFace->glyph->metrics.horiBearingX
    const glm::vec2 offset(glyphLeft, -1.f * glyphTop);
    return new BitmapImage(name, d_glyphTextures.back(), imageArea, offset, AutoScaledMode::Disabled, d_nativeResolution);
}

//----------------------------------------------------------------------------//
size_t FreeTypeFont::addNewLineIfFitting(uint32_t glyphHeight, uint32_t glyphWidth)
{
    const auto& lastLine = d_textureGlyphLines.back();

    uint32_t newLinePosY = lastLine.d_maximumExtentY + s_glyphPadding;
    uint32_t newMaxYExtent = newLinePosY + glyphHeight;
    
    // Also skip if the texture isn't wide enough    
    if (newMaxYExtent <= d_lastTextureSize && glyphWidth < d_lastTextureSize)
    {
        // Add the glyph in a new line
        d_textureGlyphLines.push_back(TextureGlyphLine(0, newLinePosY, newMaxYExtent));
        return d_textureGlyphLines.size() - 1;
    }

    return d_textureGlyphLines.size();
}

//----------------------------------------------------------------------------//
void FreeTypeFont::createGlyphAtlasTexture()
{
    const auto newTextureIndex = static_cast<uint32_t>(d_glyphTextures.size());
    const String texture_name(d_name + "_auto_glyph_images_texture_" +
        PropertyHelper<std::uint32_t>::toString(newTextureIndex));

    d_lastTextureSize = d_initialGlyphAtlasSize;
    Sizef newTextureSize(static_cast<float>(d_initialGlyphAtlasSize),
        static_cast<float>(d_initialGlyphAtlasSize));
    Texture& texture = System::getSingleton().getRenderer()->createTexture(
        texture_name, newTextureSize);
    d_glyphTextures.push_back(&texture);

    d_lastTextureBuffer = std::vector<argb_t>(d_lastTextureSize * d_lastTextureSize, 0);

    // Clear texture
    // TODO: use single channel 8-bit format, clear with GPU clear op instead of loading from memory!
    texture.blitFromMemory(d_lastTextureBuffer.data(), Rectf(glm::vec2(0.f, 0.f), newTextureSize));

    d_textureGlyphLines.clear();
    d_textureGlyphLines.push_back(TextureGlyphLine());
}

//----------------------------------------------------------------------------//
std::vector<argb_t> FreeTypeFont::createGlyphTextureData(const FT_Bitmap& glyphBitmap)
{
    uint32_t bitmapHeight = static_cast<uint32_t>(glyphBitmap.rows);
    uint32_t bitmapWidth = static_cast<uint32_t>(glyphBitmap.width);

    std::vector<argb_t> glyphTextureData;
    glyphTextureData.resize(bitmapHeight * bitmapWidth);

    for (uint32_t i = 0; i < bitmapHeight; ++i)
    {
        argb_t* currentRow =  glyphTextureData.data() + i * bitmapWidth;

        for (uint32_t j = 0; j < bitmapWidth; ++j)
        {
            std::uint8_t* src = glyphBitmap.buffer + (i * glyphBitmap.pitch + j);

            switch (glyphBitmap.pixel_mode)
            {
                case FT_PIXEL_MODE_GRAY:
                    currentRow[j] = Colour::calculateArgb(*src, 0xFF, 0xFF, 0xFF);
                    break;
                case FT_PIXEL_MODE_MONO:
                    currentRow[j] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFFFF : 0x00000000;
                    break;
                default:
                    throw InvalidRequestException("The glyph could not be drawn because the pixel mode is unsupported.");
            }
        }
    }

    return glyphTextureData;
}

//----------------------------------------------------------------------------//
void FreeTypeFont::updateTextureBufferSubImage(argb_t* destTextureData, uint32_t bitmapWidth,
    uint32_t bitmapHeight, const std::vector<argb_t>& subImageData) const
{
    argb_t* curDestPixelLine = destTextureData;

    for (uint32_t i = 0;  i < bitmapHeight;  ++i)
    {
        argb_t* curDestPixel = curDestPixelLine;

        for (uint32_t j = 0; j < bitmapWidth; ++j)
        {
            *curDestPixel = subImageData[bitmapWidth * i + j];
            ++curDestPixel;
        }

        curDestPixelLine += d_lastTextureSize;
    }
}

//----------------------------------------------------------------------------//
void FreeTypeFont::free()
{
    if (!d_fontFace)
        return;

    d_outlines.clear();

    for (const auto& glyph : d_glyphs)
        delete glyph.getImage();

    for (size_t i = 0; i < d_glyphTextures.size(); i++)
        System::getSingleton().getRenderer()->destroyTexture(*d_glyphTextures[i]);
    d_glyphTextures.clear();

    d_replacementGlyphIdx = std::numeric_limits<uint32_t>().max();

    d_codePointToGlyphMap.clear();
    d_indexToGlyphMap.clear();
    d_glyphLoadStatus.clear();
    d_glyphs.clear();
}

//----------------------------------------------------------------------------//
void FreeTypeFont::findAndThrowFreeTypeError(FT_Error error, const String& errorMessageIntro) const
{
    String errorMsg = "Unknown freetype error occurred: " + error;

    for (size_t i = 0; i < freeTypeErrorDescriptions.size(); ++i)
    {
        const FreeTypeErrorDescription& currentFreetypeError = freeTypeErrorDescriptions[i];
        if (currentFreetypeError.err_code == error)
            errorMsg = currentFreetypeError.err_msg;
    }

    throw GenericException(errorMessageIntro + " - '" + d_filename + "' error was: " + errorMsg);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::tryToCreateFontWithClosestFontHeight(
    FT_Error errorResult, int requestedFontPixelHeight) const
{
    FT_Short closestAvailableFontHeight = -1;
    int closestHeightDelta = std::numeric_limits<int>::max();

    FT_Int fixedSizesCount = d_fontFace->num_fixed_sizes;
    for (FT_Int i = 0; i < fixedSizesCount; i++)
    {
        FT_Short currentFontHeight = d_fontFace->available_sizes[i].height;
        int currentFontHeightDelta = std::abs(requestedFontPixelHeight - currentFontHeight);
        if(currentFontHeightDelta < closestHeightDelta)
        {
            closestAvailableFontHeight = currentFontHeight;
            closestHeightDelta = currentFontHeightDelta;
        }
    }

    if (closestAvailableFontHeight > 0)
    {
        errorResult = FT_Set_Pixel_Sizes(d_fontFace, 0, closestAvailableFontHeight);
        if (errorResult != 0)
            findAndThrowFreeTypeError(errorResult, "Failed to create Font using "
                "the closest available Font size that was found");
    }

    if (errorResult != 0)
        throw GenericException("The font '" + d_name + "' requested at height "
            "of " + std::to_string(d_size) + " pixels, could not be created and therefore"
            " not used");
}

//----------------------------------------------------------------------------//
void FreeTypeFont::updateFont()
{
    free();

    float fontScaleFactor = System::getSingleton().getRenderer()->getFontScale();
    if (d_autoScaled != AutoScaledMode::Disabled)
        fontScaleFactor *= d_vertScaling;
    
    const uint32_t fontSize = static_cast<uint32_t>(std::lround(getSizeInPixels() * fontScaleFactor));
    FT_Error errorResult = FT_Set_Pixel_Sizes(d_fontFace, 0, fontSize);
    if (errorResult != FT_Err_Ok)
    {
        // Usually, an error occurs with a fixed-size font format (like FNT or PCF)
        // when trying to set the pixel size to a value that is not listed in the
        // face->fixed_sizes array. For bitmap fonts we can render only at specific
        // point sizes. Try to find Font with closest pixel height and use it instead.
        tryToCreateFontWithClosestFontHeight(errorResult, fontSize);
    }

    bool initStrikeout = false;
    if (d_fontFace->face_flags & FT_FACE_FLAG_SCALABLE)
    {
        const float yScale = d_fontFace->size->metrics.y_scale * s_16dot16_toFloat * s_26dot6_toFloat;
        d_ascender = d_fontFace->ascender * yScale;
        d_descender = d_fontFace->descender * yScale;
        d_height = d_fontFace->height * yScale;
        d_underlineThickness = d_fontFace->underline_thickness * yScale;
        d_underlineTop = std::round(d_fontFace->underline_position * yScale - 0.5f * d_underlineThickness);

        if (auto os2 = static_cast<const TT_OS2*>(FT_Get_Sfnt_Table(d_fontFace, FT_SFNT_OS2)))
        {
            d_strikeoutThickness = os2->yStrikeoutSize * yScale;
            d_strikeoutTop = std::round(os2->yStrikeoutPosition * yScale - 0.5f * d_strikeoutThickness);
            initStrikeout = false;
        }
    }
    else
    {
        d_ascender = d_fontFace->size->metrics.ascender * s_26dot6_toFloat;
        d_descender = d_fontFace->size->metrics.descender * s_26dot6_toFloat;
        d_height = d_fontFace->size->metrics.height * s_26dot6_toFloat;
        d_underlineThickness = 1.f;
        d_underlineTop = 0.f;
    }

    if (initStrikeout)
    {
        d_strikeoutThickness = d_underlineThickness;
        d_strikeoutTop = std::round(d_underlineTop + (d_ascender - d_descender) * 0.4f);
    }

    if (d_specificLineSpacing > 0.0f)
        d_height = d_specificLineSpacing;

    // Initialise glyph map

    FT_UInt gindex;
    FT_ULong codepoint = FT_Get_First_Char(d_fontFace, &gindex);
    while (gindex != 0)
    {
        if (d_codePointToGlyphMap.find(codepoint) != d_codePointToGlyphMap.cend())
            throw InvalidRequestException("FreeTypeFont::initialiseGlyphMap - Requesting "
                "adding an already added glyph to the codepoint glyph map.");

        d_glyphs.emplace_back(codepoint, gindex);
        d_codePointToGlyphMap[codepoint] = static_cast<uint32_t>(d_glyphs.size()) - 1;
        d_indexToGlyphMap[gindex] = static_cast<uint32_t>(d_glyphs.size()) - 1;

        codepoint = FT_Get_Next_Char(d_fontFace, codepoint, &gindex);
    }

    d_glyphLoadStatus.resize(d_glyphs.size(), false);

    d_replacementGlyphIdx = getGlyphIndexForCodepoint(UnicodeReplacementCharacter);
}

//----------------------------------------------------------------------------//
Image* FreeTypeFont::renderOutline(uint32_t index, FT_Fixed thickness)
{
    if (!d_stroker || index >= d_glyphs.size())
        return nullptr;

    FT_Set_Transform(d_fontFace, nullptr, nullptr);

    if (FT_Load_Glyph(d_fontFace, d_glyphs[index].getGlyphIndex(), getGlyphLoadFlags() | FT_LOAD_NO_BITMAP))
        return nullptr;

    if (d_fontFace->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
        return nullptr;

    FT_Glyph ftGlyph;
    FT_Get_Glyph(d_fontFace->glyph, &ftGlyph);
    FT_Stroker_Set(d_stroker, thickness, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
    FT_Glyph_Stroke(&ftGlyph, d_stroker, true); // can also use FT_Glyph_StrokeBorder
    FT_Glyph_To_Bitmap(&ftGlyph, FT_RENDER_MODE_NORMAL, 0, true);

    const FT_BitmapGlyph& bitmapGlyph = reinterpret_cast<const FT_BitmapGlyph&>(ftGlyph);

    auto it = d_outlines.find(thickness);
    if (it == d_outlines.cend())
        it = d_outlines.emplace(thickness, std::vector<std::unique_ptr<BitmapImage>>(d_glyphs.size())).first;

    const String name(std::to_string(d_glyphs[index].getCodePoint()) + "_ol_" + std::to_string(thickness));
    it->second[index].reset(rasterise(name, bitmapGlyph->bitmap, bitmapGlyph->left, bitmapGlyph->top, bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows));

    FT_Done_Glyph(ftGlyph);

    return it->second[index].get();
}

//----------------------------------------------------------------------------//
void FreeTypeFont::writeXMLToStream_impl(XMLSerializer& xml_stream) const
{
    xml_stream.attribute(Font_xmlHandler::FontSizeAttribute,
                         PropertyHelper<float>::toString(d_size));

    if (d_sizeUnit != FontSizeUnit::Points)
        xml_stream.attribute(Font_xmlHandler::FontSizeUnitAttribute,
                             PropertyHelper<FontSizeUnit>::toString(d_sizeUnit));
    if (!d_antiAliased)
        xml_stream.attribute(Font_xmlHandler::FontAntiAliasedAttribute, "false");

    if (d_specificLineSpacing > 0.0f)
        xml_stream.attribute(Font_xmlHandler::FontLineSpacingAttribute,
                             PropertyHelper<float>::toString(d_specificLineSpacing));
}

//----------------------------------------------------------------------------//
void FreeTypeFont::setSizeAndUnit(const float size, const FontSizeUnit sizeUnit)
{
    if (d_size == size && d_sizeUnit == sizeUnit)
        return;

    d_size = size;
    d_sizeUnit = sizeUnit;

    handleFontSizeOrFontUnitChange();
}

//----------------------------------------------------------------------------//
float FreeTypeFont::getSizeInPixels() const
{
    if (d_sizeUnit == FontSizeUnit::Pixels)
        return d_size;

    if (d_sizeUnit == FontSizeUnit::Points)
        return convertPointsToPixels(d_size, Renderer::ReferenceDpiValue);

    throw InvalidRequestException("FreeTypeFont::getSizeInPixels - Requesting "
        "font size in pixels but the Font size unit is invalid.");
}

//----------------------------------------------------------------------------//
float FreeTypeFont::getSizeInPoints() const
{
    if (d_sizeUnit == FontSizeUnit::Pixels)
        return convertPixelsToPoints(d_size, Renderer::ReferenceDpiValue);

    if (d_sizeUnit == FontSizeUnit::Points)
        return d_size;

    throw InvalidRequestException("FreeTypeFont::getSizeInPixels - Requesting "
        "font size in pixels but the Font size unit is invalid.");
}

//----------------------------------------------------------------------------//
void FreeTypeFont::handleFontSizeOrFontUnitChange()
{
    updateFont();

    FontEventArgs args(this);
    onRenderSizeChanged(args);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::setAntiAliased(bool antiAliased)
{
    if (antiAliased == d_antiAliased)
        return;

    d_antiAliased = antiAliased;
    updateFont();

    FontEventArgs args(this);
    onRenderSizeChanged(args);
}

//----------------------------------------------------------------------------//
FT_Int32 FreeTypeFont::getGlyphLoadFlags() const
{
    return FT_LOAD_FORCE_AUTOHINT | (d_antiAliased ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO);
}

//----------------------------------------------------------------------------//
float FreeTypeFont::getKerning(const FontGlyph* prev, const FontGlyph& curr) const
{
    auto ftPrev = static_cast<const FreeTypeFontGlyph*>(prev);
    auto ftCurr = static_cast<const FreeTypeFontGlyph*>(&curr);

    // Get freetype kerning in 1/64th of the pixel
    int32_t kerningX = 0;

    if (ftPrev)
    {
        // Do kerning
        FT_Vector kerning;
        FT_Get_Kerning(d_fontFace, ftPrev->getGlyphIndex(), ftCurr->getGlyphIndex(), FT_KERNING_DEFAULT, &kerning);
        kerningX = kerning.x;

        // Adjust for side bearing changes due to FreeType auto-hinting
        kerningX -= ftPrev->getRsbDelta();
    }

    // Adjust for side bearing changes due to FreeType auto-hinting
    kerningX += ftCurr->getLsbDelta();

    // Return value in pixels
    return kerningX * s_26dot6_toFloat;
}

//----------------------------------------------------------------------------//
FreeTypeFontGlyph* FreeTypeFont::loadGlyph(uint32_t index)
{
    if (index >= d_glyphs.size())
        return nullptr;

    auto glyph = &d_glyphs[index];

    if (d_glyphLoadStatus[index])
        return glyph;

    // Mark as loaded immediately to avoid retrying on error
    d_glyphLoadStatus[index] = true;

    FT_Set_Transform(d_fontFace, nullptr, nullptr);

    // Non-zero result is an error
    if (FT_Load_Glyph(d_fontFace, glyph->getGlyphIndex(), getGlyphLoadFlags() | FT_LOAD_RENDER))
        return nullptr;

    // NB: FontGlyph doesn't own an image (see PixmapFont), so we have to delete an image manually in free()
    const String name(std::to_string(glyph->getCodePoint()));
    const auto& ft_bitmap = d_fontFace->glyph->bitmap;
    auto image = rasterise(name, ft_bitmap, d_fontFace->glyph->bitmap_left, d_fontFace->glyph->bitmap_top, ft_bitmap.width, ft_bitmap.rows);
    glyph->setImage(image);
    glyph->setAdvance(d_fontFace->glyph->advance.x * static_cast<float>(s_26dot6_toFloat));
    glyph->setLsbDelta(d_fontFace->glyph->lsb_delta);
    glyph->setRsbDelta(d_fontFace->glyph->rsb_delta);

    return glyph;
}

//----------------------------------------------------------------------------//
const FreeTypeFontGlyph* FreeTypeFont::getGlyph(uint32_t index) const
{
    return (index < d_glyphs.size()) ? &d_glyphs[index] : nullptr;
}

//----------------------------------------------------------------------------//
Image* FreeTypeFont::getOutline(uint32_t index, float thickness)
{
    if (index >= d_glyphs.size())
        return nullptr;

    const auto outlineThickness = static_cast<FT_Fixed>(std::round(thickness * 64.f));
    if (outlineThickness < 1)
        return nullptr;

    //!!!TODO TEXT: need to make distinction between not yet rendered and failed outline to avoid repeated recreation on failure!
    auto it = d_outlines.find(outlineThickness);
    if (it != d_outlines.cend() && it->second[index])
        return it->second[index].get();

    return renderOutline(index, outlineThickness);
}

//----------------------------------------------------------------------------//
uint32_t FreeTypeFont::getGlyphIndexByFreetypeIndex(FT_UInt ftGlyphIndex) const
{
    auto it = d_indexToGlyphMap.find(ftGlyphIndex);
    return (it != d_indexToGlyphMap.end()) ? it->second : d_replacementGlyphIdx;
}

}
