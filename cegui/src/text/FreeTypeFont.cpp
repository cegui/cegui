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

#ifdef CEGUI_USE_RAQM
#include <raqm.h>
#endif

#include <iostream>

namespace
{

#ifdef CEGUI_USE_RAQM
raqm_t* createAndSetupRaqmTextObject(const uint32_t* text, const size_t textLength,
    CEGUI::DefaultParagraphDirection defaultParagraphDir, FT_Face face, FT_Int32 loadFlags)
{
    raqm_t* raqmObject = raqm_create();

    if (!raqm_set_text(raqmObject, text, textLength))
        throw CEGUI::InvalidRequestException("Setting raqm text was unsuccessful");

    if (!raqm_set_freetype_face(raqmObject, face))
        throw CEGUI::InvalidRequestException("Could not set the Freetype font Face for a raqm object");

    if (!raqm_set_freetype_load_flags(raqmObject, loadFlags))
        throw CEGUI::InvalidRequestException("Could not set Freetype load flags for a raqm object");

    raqm_direction_t textDefaultParagraphDirection = RAQM_DIRECTION_LTR;
    switch (defaultParagraphDir)
    {
        case CEGUI::DefaultParagraphDirection::RightToLeft:
            textDefaultParagraphDirection = RAQM_DIRECTION_RTL;
            break;
        case CEGUI::DefaultParagraphDirection::Automatic:
            textDefaultParagraphDirection = RAQM_DIRECTION_DEFAULT;
            break;
    }

    if (!raqm_set_par_direction(raqmObject, textDefaultParagraphDirection))
        throw CEGUI::InvalidRequestException("Could not set the parse direction for a raqm object");

    if (!raqm_layout(raqmObject))
        throw CEGUI::InvalidRequestException("Layouting raqm text was unsuccessful");

    return raqmObject;
}
#endif

}

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
    std::vector<FreeTypeFontLayer> fontLayers,
    const String& resource_group,
    const AutoScaledMode auto_scaled,
    const Sizef& native_res,
    const float specific_line_spacing) :
    Font(font_name, Font_xmlHandler::FontTypeFreeType, font_filename,
         resource_group, auto_scaled, native_res),
    d_specificLineSpacing(specific_line_spacing),
    d_size(size),
    d_sizeUnit(sizeUnit),
    d_antiAliased(anti_aliased),
    d_fontFace(nullptr),
    d_fontLayers(std::move(fontLayers))
{
    if (!s_fontUsageCount++)
        FT_Init_FreeType(&s_freetypeLibHandle);

    addFreeTypeFontProperties();

    FreeTypeFont::updateFont();

    std::stringstream& sstream = SharedStringstream::GetPreparedStream();
    sstream << "Successfully loaded " << d_codePointToGlyphMap.size() << " glyphs";
    Logger::getSingleton().logEvent(sstream.str(), LoggingLevel::Informative);
}

//----------------------------------------------------------------------------//
FreeTypeFont::~FreeTypeFont()
{
    free();

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
void FreeTypeFont::resizeAndUpdateTexture(Texture* texture, uint32_t newSize) const
{
    if (d_lastTextureSize >= newSize)
        throw InvalidRequestException("Must supply a larger than previous size when "
            "resizing the glyph atlas");

    const uint32_t oldTextureSize = d_lastTextureSize;
    std::vector<argb_t> oldTextureData;
    std::swap(oldTextureData, d_lastTextureBuffer);

    const Sizef newTextureSize(static_cast<float>(newSize), static_cast<float>(newSize));
    d_lastTextureSize = newSize;

    d_lastTextureBuffer.resize(newSize * newSize);
    std::fill(d_lastTextureBuffer.begin(), d_lastTextureBuffer.end(), 0);

    // Copy our memory buffer into the texture and free it
    updateTextureBufferSubImage(d_lastTextureBuffer.data(),
        oldTextureSize, oldTextureSize, oldTextureData);

    //TODO: use single channel 8-bit format! Blit/clear with GPU instead of loading from memory?
    texture->loadFromMemory(d_lastTextureBuffer.data(), newTextureSize, Texture::PixelFormat::Rgba);

    System::getSingleton().getRenderer()->updateGeometryBufferTexCoords(texture,
        oldTextureSize / static_cast<float>(newSize));
}

//----------------------------------------------------------------------------//
void FreeTypeFont::createTextureSpaceForGlyphRasterisation(Texture* texture, uint32_t glyphWidth, uint32_t glyphHeight) const
{
    const auto maxTextureSize = static_cast<uint32_t>(System::getSingleton().getRenderer()->getMaxTextureSize());
    const uint32_t scaleFactor = 2;

    if (glyphWidth > maxTextureSize || glyphHeight > maxTextureSize)
        throw InvalidRequestException("Can not rasterise a glyph that is larger "
            "than the maximum supported texture size.");
        
    uint32_t newSize = d_lastTextureSize * scaleFactor;
    if (newSize > maxTextureSize)
        createGlyphAtlasTexture();
    else
        resizeAndUpdateTexture(texture, newSize);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::addRasterisedGlyphToTextureAndSetupGlyphImage(
    FreeTypeFontGlyph* glyph, Texture* texture, FT_Bitmap& glyphBitmap, uint32_t glyphLeft, uint32_t glyphTop,
    uint32_t glyphWidth, uint32_t glyphHeight, uint32_t layer, const TextureGlyphLine& glyphTexLine) const
{
    // Create the data containing the pixels of the glyph
    std::vector<argb_t> subTextureData = createGlyphTextureData(glyphBitmap);

    // Update the cached texture data in memory
    size_t bufferDataGlyphPos = (glyphTexLine.d_lastYPos * d_lastTextureSize) + glyphTexLine.d_lastXPos;
    updateTextureBufferSubImage(d_lastTextureBuffer.data() + bufferDataGlyphPos,
        glyphWidth, glyphHeight, subTextureData);

    // Update the sub-image in the texture on the GPU
    glm::vec2 subImagePos(glyphTexLine.d_lastXPos, glyphTexLine.d_lastYPos);
    Sizef subImageSize(static_cast<float>(glyphWidth), static_cast<float>(glyphHeight));
    Rectf subImageArea(subImagePos, subImageSize);
    texture->blitFromMemory(subTextureData.data(), subImageArea);

    // Create a new image in the imageset
    const Rectf area(static_cast<float>(glyphTexLine.d_lastXPos),
        static_cast<float>(glyphTexLine.d_lastYPos),
        static_cast<float>(glyphTexLine.d_lastXPos + glyphWidth),
        static_cast<float>(glyphTexLine.d_lastYPos + glyphHeight));

    // This is the right bearing for bitmap glyphs, not d_fontFace->glyph->metrics.horiBearingX
    const glm::vec2 offset(glyphLeft, -1.f * glyphTop);

    const String name(std::to_string(glyph->getCodePoint()));

    BitmapImage* img = new BitmapImage(name, texture, area, offset, AutoScaledMode::Disabled, d_nativeResolution);
    d_glyphImages.push_back(img);

    glyph->setImage(img, layer);
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
void FreeTypeFont::rasterise(FreeTypeFontGlyph* glyph, FT_Bitmap& ft_bitmap, uint32_t glyphLeft,
    uint32_t glyphTop, uint32_t glyphWidth, uint32_t glyphHeight, uint32_t layer) const
{
    if (d_glyphTextures.empty())
        createGlyphAtlasTexture();

    size_t fittingLineIndex = findTextureLineWithFittingSpot(glyphWidth, glyphHeight);

    if (fittingLineIndex >= d_textureGlyphLines.size())
        fittingLineIndex = addNewLineIfFitting(glyphHeight, glyphWidth);

    if (fittingLineIndex >= d_textureGlyphLines.size())
    {
        createTextureSpaceForGlyphRasterisation(d_glyphTextures.back(), glyphWidth, glyphHeight);
        rasterise(glyph, ft_bitmap, glyphLeft, glyphTop, glyphWidth, glyphHeight, layer);
        return;
    }

    const TextureGlyphLine& glyphTexLine = d_textureGlyphLines[fittingLineIndex];

    addRasterisedGlyphToTextureAndSetupGlyphImage(glyph, d_glyphTextures.back(), ft_bitmap,
        glyphLeft, glyphTop, glyphWidth, glyphHeight, layer, glyphTexLine);

    // Advance to next position, add padding
    glyphTexLine.d_lastXPos += glyphWidth + s_glyphPadding;
}

//----------------------------------------------------------------------------//
size_t FreeTypeFont::addNewLineIfFitting(uint32_t glyphHeight, uint32_t glyphWidth) const
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
void FreeTypeFont::createGlyphAtlasTexture() const
{
    std::uint32_t newTextureIndex = d_glyphTextures.size();
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
std::vector<argb_t> FreeTypeFont::createGlyphTextureData(FT_Bitmap& glyphBitmap)
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
FT_Stroker_LineCap FreeTypeFont::getLineCap(FreeTypeLineCap line_cap)
{
    switch (line_cap)
    {
        case FreeTypeLineCap::Round:
            return FT_STROKER_LINECAP_ROUND;
        case FreeTypeLineCap::Butt:
            return FT_STROKER_LINECAP_BUTT;
        case FreeTypeLineCap::Square:
            return FT_STROKER_LINECAP_SQUARE;
    }
    return FT_STROKER_LINECAP_SQUARE;
}

//----------------------------------------------------------------------------//
FT_Stroker_LineJoin FreeTypeFont::getLineJoin(FreeTypeLineJoin line_join)
{
    switch (line_join)
    {
        case FreeTypeLineJoin::Round:
            return FT_STROKER_LINEJOIN_ROUND;
        case FreeTypeLineJoin::Bevel:
            return FT_STROKER_LINEJOIN_BEVEL;
        case FreeTypeLineJoin::MiterFixed:
            return FT_STROKER_LINEJOIN_MITER_FIXED;
        case FreeTypeLineJoin::MiterVariable:
            return FT_STROKER_LINEJOIN_MITER_VARIABLE;
    }
    return FT_STROKER_LINEJOIN_ROUND;
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

    d_replacementGlyph = nullptr;

    for (auto codePointMapEntry : d_codePointToGlyphMap)
        delete codePointMapEntry.second;

    d_codePointToGlyphMap.clear();
    d_indexToGlyphMap.clear();

    for (size_t i = 0; i < d_glyphImages.size(); ++i)
        delete d_glyphImages[i];
    d_glyphImages.clear();

    for (size_t i = 0; i < d_glyphTextures.size(); i++)
        System::getSingleton().getRenderer()->destroyTexture(*d_glyphTextures[i]);
    d_glyphTextures.clear();

    FT_Done_Face(d_fontFace);
    d_fontFace = nullptr;
    System::getSingleton().getResourceProvider()->unloadRawDataContainer(d_fontData);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::createFreetypeMemoryFace()
{
    // create face using input font
    FT_Error error = FT_New_Memory_Face(s_freetypeLibHandle, d_fontData.getDataPtr(),
        static_cast<FT_Long>(d_fontData.getSize()), 0, &d_fontFace);

    if (error != 0)
        findAndThrowFreeTypeError(error, "Failed to create face from font file");
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
void FreeTypeFont::checkUnicodeCharMapAvailability()
{
    if (d_fontFace->charmap)
        return;

    FT_Done_Face(d_fontFace);
    d_fontFace = nullptr;
    throw GenericException("The font '" + d_name +
        "' does not have a Unicode charmap, and cannot be used.");
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

    System::getSingleton().getResourceProvider()->loadRawDataContainer(
        d_filename, d_fontData, d_resourceGroup.empty() ? getDefaultResourceGroup() : d_resourceGroup);

    createFreetypeMemoryFace();

    checkUnicodeCharMapAvailability();
    
    float fontScaleFactor = System::getSingleton().getRenderer()->getFontScale();
    if (d_autoScaled != AutoScaledMode::Disabled)
        fontScaleFactor *= d_vertScaling;
    
    uint32_t requestedFontSizeInPixels = static_cast<uint32_t>(
        std::lround(getSizeInPixels() * fontScaleFactor));

    FT_Error errorResult = FT_Set_Pixel_Sizes(d_fontFace, 0, requestedFontSizeInPixels);
    if (errorResult != 0)
    {
        // Usually, an error occurs with a fixed-size font format (like FNT or PCF)
        // when trying to set the pixel size to a value that is not listed in the
        // face->fixed_sizes array. For bitmap fonts we can render only at specific
        // point sizes. Try to find Font with closest pixel height and use it instead.
        tryToCreateFontWithClosestFontHeight(errorResult, requestedFontSizeInPixels);
    }

    if (d_fontFace->face_flags & FT_FACE_FLAG_SCALABLE)
    {
        const float yScale = d_fontFace->size->metrics.y_scale * s_16dot16_toFloat * s_26dot6_toFloat;
        d_ascender = d_fontFace->ascender * yScale;
        d_descender = d_fontFace->descender * yScale;
        d_height = d_fontFace->height * yScale;
    }
    else
    {
        d_ascender = d_fontFace->size->metrics.ascender * s_26dot6_toFloat;
        d_descender = d_fontFace->size->metrics.descender * s_26dot6_toFloat;
        d_height = d_fontFace->size->metrics.height * s_26dot6_toFloat;
    }

    if (d_specificLineSpacing > 0.0f)
        d_height = d_specificLineSpacing;

    initialiseGlyphMap();
}

//----------------------------------------------------------------------------//
void FreeTypeFont::initialiseGlyphMap()
{
    FT_UInt gindex;
    FT_ULong codepoint = FT_Get_First_Char(d_fontFace, &gindex);

    while (gindex != 0)
    {
        if (d_codePointToGlyphMap.find(codepoint) != d_codePointToGlyphMap.end())
        {
            throw InvalidRequestException("FreeTypeFont::initialiseGlyphMap - Requesting "
                "adding an already added glyph to the codepoint glyph map.");        
        }

        //???TODO TEXT: allocate an array of glyph instances, not pointers, and use pointers only in maps?
        FreeTypeFontGlyph* newFontGlyph = new FreeTypeFontGlyph(codepoint, gindex);
        d_codePointToGlyphMap[codepoint] = newFontGlyph;
        d_indexToGlyphMap[gindex] = newFontGlyph;

        codepoint = FT_Get_Next_Char(d_fontFace, codepoint, &gindex);
    }

    d_replacementGlyph = getGlyphForCodepoint(UnicodeReplacementCharacter);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::prepareGlyph(FreeTypeFontGlyph* glyph) const
{
    if (glyph->isInitialised())
        return;

    FT_Glyph ft_glyph;
    FT_Bitmap ft_bitmap;
    FT_Vector position;
    position.x = 0L;
    position.y = 0L;
    int glyphWidth = 0;
    int glyphHeight = 0;
    int glyphLeft = 0;
    int glyphTop = 0;
    FT_Set_Transform(d_fontFace, nullptr, &position);
    FT_UInt glyphIndex = FT_Get_Char_Index(d_fontFace, glyph->getCodePoint());

    // Retrieved from font somehow
    uint32_t layerCount = d_fontLayers.size();

    //layer 0 is the top rendered layer (rendered last over the other layers)
    for (uint32_t layer = 0; layer < layerCount; layer++)
    {
        FontLayerType fontLayerType = d_fontLayers[layer].d_fontLayerType;

        // Load the code point, "rendering" the glyph
        FT_Error error = FT_Load_Glyph(d_fontFace, glyphIndex, getGlyphLoadFlags(layer));

        glyph->markAsInitialised();

        if (error != 0)
            return;

        if (fontLayerType == FontLayerType::Standard)
        {
            ft_bitmap = d_fontFace->glyph->bitmap;
            glyphWidth = d_fontFace->glyph->bitmap.width;
            glyphHeight = d_fontFace->glyph->bitmap.rows;
            glyphTop = d_fontFace->glyph->bitmap_top;
            glyphLeft = d_fontFace->glyph->bitmap_left;
        }
        else
        {
            uint32_t outlinePixels = d_fontLayers[layer].d_outlinePixels; // n * 64 result in n pixels outline
            bool errorFlag = false;

            FT_Stroker stroker;
            FT_Stroker_New(s_freetypeLibHandle, &stroker);
            FT_Stroker_Set(stroker,
                outlinePixels * 64,
                getLineCap(d_fontLayers[layer].d_lineCap),
                getLineJoin(d_fontLayers[layer].d_lineJoin),
                d_fontLayers[layer].d_miterLimit);

            if (FT_Get_Glyph(d_fontFace->glyph, &ft_glyph))
                errorFlag = true;
            if (fontLayerType == FontLayerType::Outline)
              error = FT_Glyph_Stroke(&ft_glyph, stroker, true);
            if (fontLayerType == FontLayerType::Outer)
              error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, false, true);
            if (fontLayerType == FontLayerType::Inner)
              error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, true, true);
            if (error > 0)
                errorFlag = true;
            error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, true);
            if (error > 0)
                errorFlag = true;

            FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(ft_glyph);
            ft_bitmap = bitmapGlyph->bitmap;
            glyphWidth = bitmapGlyph->bitmap.width;
            glyphHeight = bitmapGlyph->bitmap.rows;
            glyphTop = bitmapGlyph->top;
            glyphLeft = bitmapGlyph->left;
            FT_Stroker_Done(stroker);
            if (errorFlag)
            {
                //return;
            }
        }

        if (!glyph->getImage(layer))
        {
            // Rasterise the 0 position glyph
            rasterise(glyph, ft_bitmap, glyphLeft, glyphTop, glyphWidth, glyphHeight, layer);

            glyph->setLsbDelta(d_fontFace->glyph->lsb_delta);
            glyph->setRsbDelta(d_fontFace->glyph->rsb_delta);        
        }

        // FT_Fixed advance;
        // TODO TEXT: update freetype and use FT_Get_Advance(d_fontFace, glyphIndex, loadBitmask, &advance);
        // If scaling is performed (based on the value of `load_flags`), the advance value is in 16.16 format.
        // Otherwise, it is in font units. Zero return value is success.

        float adv;
        if (fontLayerType == FontLayerType::Standard)
        {
            //adv = d_fontFace->glyph->advance.x * static_cast<float>(s_26dot6_toFloat);
            adv = d_fontFace->glyph->metrics.horiAdvance * static_cast<float>(s_26dot6_toFloat);
        }
        else
        {
            adv = d_fontFace->glyph->metrics.horiAdvance * static_cast<float>(s_26dot6_toFloat);
            FT_Done_Glyph(ft_glyph);
        }

        glyph->setAdvance(adv);
    }
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
FT_Int32 FreeTypeFont::getGlyphLoadFlags(uint32_t layer) const
{
    if (layer >= d_fontLayers.size())
        return 0;

    const FontLayerType fontLayerType = d_fontLayers[layer].d_fontLayerType;

    const FT_Int32 targetType = d_antiAliased ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO;
    const FT_Int32 loadType = (fontLayerType == FontLayerType::Standard) ? FT_LOAD_RENDER : FT_LOAD_NO_BITMAP;
    return loadType | FT_LOAD_FORCE_AUTOHINT | targetType;
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
void FreeTypeFont::layoutAndCreateGlyphRenderGeometry(std::vector<GeometryBuffer*>& out,
    const String& text, const float spaceExtra, ImageRenderSettings& imgRenderSettings,
    DefaultParagraphDirection defaultParagraphDir, glm::vec2& penPosition) const
{
#ifdef CEGUI_USE_RAQM
    layoutUsingRaqmAndCreateRenderGeometry(out, text, { imgRenderSettings.d_multiplyColours },
        spaceExtra, imgRenderSettings, defaultParagraphDir, penPosition);
#else
    CEGUI_UNUSED(defaultParagraphDir);
    layoutUsingFreetypeAndCreateRenderGeometry(out, text, { imgRenderSettings.d_multiplyColours },
        spaceExtra, imgRenderSettings, penPosition);
#endif
}

//----------------------------------------------------------------------------//
void FreeTypeFont::layoutUsingFreetypeAndCreateRenderGeometry(std::vector<GeometryBuffer*>& out,
    const String& text, const std::vector<ColourRect>& layerColours,
    const float spaceExtra, ImageRenderSettings& imgRenderSettings, glm::vec2& penPosition) const
{
    if (text.empty())
        return;

    const auto canCombineFromIdx = out.size();
    const CEGUI::ColourRect fallbackColour;

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    std::u32string utf32Text = String::convertUtf8ToUtf32(text.c_str(), text.length());
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32) 
    const std::u32string& utf32Text = text.getString();
#endif

    glm::vec2 penPositionStart = penPosition;
    penPositionStart.y += getBaseline();

    uint32_t layerCount = d_fontLayers.size();
    for (int layerTmp = layerCount - 1; layerTmp >= 0; --layerTmp)
    {
        uint32_t layer = static_cast<uint32_t>(layerTmp);

        // Each layer starts from the same position
        penPosition = penPositionStart;

        const FreeTypeFontGlyph* prevGlyph = nullptr;

        const size_t charCount = utf32Text.size();
        for (size_t i = 0; i < charCount; ++i)
        {
            const char32_t codePoint = utf32Text[i];

            // Ignore new line characters
            if (codePoint == '\n')
                continue;

            // Find glyph and handle missing glyph replacement
            const FreeTypeFontGlyph* glyph = getGlyphForCodepoint(codePoint, true);
            if (!glyph)
            {
                //!!!FIXME TEXT: could cache in a variable to skip search!
                if (codePoint != UnicodeReplacementCharacter)
                    glyph = static_cast<const FreeTypeFontGlyph*>(d_replacementGlyph);

                if (!glyph)
                    continue;
            }

            // Do kerning and FreeType auto-hinting LSB/RSB delta compensation
            penPosition.x += getKerning(prevGlyph, *glyph);
            prevGlyph = glyph;

            // Render glyph
            if (auto image = glyph->getImage(layer))
            {
                imgRenderSettings.d_destArea = Rectf(penPosition, image->getRenderedSize());
                imgRenderSettings.d_multiplyColours = (layer < layerColours.size()) ? layerColours[layer] : fallbackColour;
                image->createRenderGeometry(out, imgRenderSettings, canCombineFromIdx);
            }

            penPosition.x += glyph->getAdvance();

            // TODO: This is for justified text and probably wrong because the space was determined
            // without considering kerning
            if (codePoint == ' ')
                penPosition.x += spaceExtra;
        }
    }
}

#ifdef CEGUI_USE_RAQM
//----------------------------------------------------------------------------//
void FreeTypeFont::layoutUsingRaqmAndCreateRenderGeometry(std::vector<GeometryBuffer*>& out,
    const String& text, const std::vector<ColourRect>& layerColours, 
    const float spaceExtra, ImageRenderSettings& imgRenderSettings, 
    DefaultParagraphDirection defaultParagraphDir, glm::vec2& penPosition) const
{
    if (text.empty())
        return;

    const auto canCombineFromIdx = out.size();
    const CEGUI::ColourRect fallbackColour;

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    std::u32string utf32Text = String::convertUtf8ToUtf32(text.c_str());
    size_t origTextLength = utf32Text.length();
    const uint32_t* originalTextArray = reinterpret_cast<const std::uint32_t*>(utf32Text.c_str());
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32) 
    size_t origTextLength = text.length();
    const uint32_t* originalTextArray = reinterpret_cast<const std::uint32_t*>(text.c_str());
#endif

    const FT_Int32 loadBitmask = getGlyphLoadFlags(0);

    raqm_t* raqmObject = createAndSetupRaqmTextObject(
        originalTextArray, origTextLength, defaultParagraphDir, getFontFace(), loadBitmask);

    glm::vec2 penPositionStart = penPosition;
    penPositionStart.y += getBaseline();

    const std::size_t layerCount = d_fontLayers.size();
    for (int layerTmp = layerCount - 1; layerTmp >= 0; --layerTmp)
    {
        uint32_t layer = static_cast<uint32_t>(layerTmp);

        size_t count = 0;
        raqm_glyph_t* glyphs = raqm_get_glyphs(raqmObject, &count);

        // Each layer starts from the same position
        penPosition = penPositionStart;

        for (size_t i = 0; i < count; ++i)
        {
            raqm_glyph_t& currentGlyph = glyphs[i];

            // Ignore new line characters
            const auto codePoint = originalTextArray[currentGlyph.cluster];
            if (codePoint == '\n')
                continue;

            // Find glyph and handle missing glyph replacement
            const FreeTypeFontGlyph* glyph = getGlyphByIndex(currentGlyph.index, true);
            if (!glyph)
            {
                //!!!FIXME TEXT: could cache in a variable to skip search!
                if (codePoint != UnicodeReplacementCharacter)
                    glyph = static_cast<const FreeTypeFontGlyph*>(d_replacementGlyph);

                if (!glyph)
                    continue;
            }

            // Render glyph
            if (auto image = glyph->getImage(layer))
            {
                //The glyph pos will be rounded to full pixels internally
                const glm::vec2 renderGlyphPos(
                    penPosition.x + currentGlyph.x_offset * s_26dot6_toFloat,
                    penPosition.y + currentGlyph.y_offset * s_26dot6_toFloat);

                imgRenderSettings.d_destArea = Rectf(renderGlyphPos, image->getRenderedSize());
                imgRenderSettings.d_multiplyColours = (layer < layerColours.size()) ? layerColours[layer] : fallbackColour;
                image->createRenderGeometry(out, imgRenderSettings, canCombineFromIdx);
            }

            penPosition.x += currentGlyph.x_advance * s_26dot6_toFloat;

            // TODO: This is for justified text and probably wrong because the space was determined
            // without considering kerning
            if (codePoint == ' ')
                penPosition.x += spaceExtra;
        }

        raqm_destroy(raqmObject);
    }
}
#endif

//----------------------------------------------------------------------------//
bool FreeTypeFont::isCodepointAvailable(char32_t codePoint) const
{
    return d_codePointToGlyphMap.find(codePoint) != d_codePointToGlyphMap.end();
}

//----------------------------------------------------------------------------//
FreeTypeFontGlyph* FreeTypeFont::getGlyphForCodepoint(const char32_t codepoint, bool prepare) const
{
    auto it = d_codePointToGlyphMap.find(codepoint);
    if (it == d_codePointToGlyphMap.end() || !it->second)
        return nullptr;

    if (prepare)
        prepareGlyph(it->second);

    return it->second;
}

//----------------------------------------------------------------------------//
FreeTypeFontGlyph* FreeTypeFont::getGlyphByIndex(uint32_t ftGlyphIndex, bool prepare) const
{
    auto it = d_indexToGlyphMap.find(ftGlyphIndex);
    if (it == d_indexToGlyphMap.end() || !it->second)
        return nullptr;

    if (prepare)
        prepareGlyph(it->second);

    return it->second;
}

}
