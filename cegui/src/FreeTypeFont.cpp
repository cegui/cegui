/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Implements FreeTypeFont class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/FreeTypeFont.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Texture.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/System.h"
#include "CEGUI/Logger.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/Font_xmlHandler.h"
#include "CEGUI/SharedStringStream.h"
#include "CEGUI/FreeTypeFontGlyph.h"

#ifdef CEGUI_USE_RAQM
#include <raqm.h>
#endif

#include <cmath>


namespace CEGUI
{
//----------------------------------------------------------------------------//
// Pixels to put between glyphs
static const int s_glyphPadding = 1;
// A multiplication coefficient to convert FT_Pos values into normal floats
static const float s_conversionMultCoeff =  (1.0f/64.f);
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
int          err_code;                                        
const char*  err_msg;                                         
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
    d_antiAliased(anti_aliased),
    d_fontFace(nullptr)
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
void FreeTypeFont::addFreeTypeFontProperties ()
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
        &FreeTypeFont::setAntiAliased, &FreeTypeFont::isAntiAliased, 0
    );
}

void FreeTypeFont::resizeAndUpdateTexture(Texture* texture, int newSize) const
{
    if(d_lastTextureSize >= newSize)
    {
        InvalidRequestException("Must supply a larger than previous size when "
            "resizing the glyph atlas");
    }

    int oldTextureSize = d_lastTextureSize;
    std::vector<argb_t> oldTextureData(d_lastTextureBuffer);

    Sizef newTextureSize(static_cast<float>(newSize),
                         static_cast<float>(newSize));
    d_lastTextureSize = newSize;

    d_lastTextureBuffer.resize(newSize * newSize);
    std::fill(d_lastTextureBuffer.begin(), d_lastTextureBuffer.end(), 0);

    // Copy our memory buffer into the texture and free it
    updateTextureBufferSubImage(d_lastTextureBuffer.data(),
        oldTextureSize, oldTextureSize, oldTextureData);

    texture->loadFromMemory(d_lastTextureBuffer.data(), newTextureSize, Texture::PixelFormat::Rgba);

    System::getSingleton().getRenderer()->updateGeometryBufferTexCoords(texture,
        oldTextureSize / static_cast<float>(newSize));
}

void FreeTypeFont::createTextureSpaceForGlyphRasterisation(Texture* texture, int glyphWidth, int glyphHeight) const
{
    int maxTextureSize = System::getSingleton().getRenderer()->getMaxTextureSize();
    const int scaleFactor = 2;

    if(glyphWidth > maxTextureSize || glyphHeight > maxTextureSize)
    {
        InvalidRequestException("Can not rasterise a glyph that is larger "
            "than the maximum supported texture size.");
    }
        
    int newSize = d_lastTextureSize * scaleFactor;
    if (newSize > maxTextureSize)
    {
        createGlyphAtlasTexture();
    }
    else
    {
        resizeAndUpdateTexture(texture, newSize);
    }
}

//----------------------------------------------------------------------------//
void FreeTypeFont::rasterise(FreeTypeFontGlyph* glyph) const
{
    if(d_glyphTextures.empty())
    {
        createGlyphAtlasTexture();
    }

    // Retrieve the last texture created
    Texture* texture = d_glyphTextures.back();

    // Go ahead, line by line, top-left to bottom-right
    int glyphWidth = d_fontFace->glyph->bitmap.width;
    int glyphHeight = d_fontFace->glyph->bitmap.rows;

    bool fittingLineWasFound = false;
    size_t fittingLineIndex = -1;

    // Go through the lines and find one that fits
    size_t lineCount =  d_textureGlyphLines.size();
    for(size_t i = 0; i < lineCount && !fittingLineWasFound; ++i)
    {
        const auto& currentGlyphLine = d_textureGlyphLines[i];
        // Check if glyph right margin exceeds texture size
        int curGlyphXEnd = currentGlyphLine.d_lastXPos + glyphWidth;
        int curGlyphYEnd = currentGlyphLine.d_lastYPos + glyphHeight;
        if (curGlyphXEnd < d_lastTextureSize && curGlyphYEnd < d_lastTextureSize)
        {
            //Only the last line can be extended into the y-dimension
            if (i == (lineCount - 1))
            {
                if (curGlyphYEnd > currentGlyphLine.d_maximumExtentY)
                {
                    currentGlyphLine.d_maximumExtentY = curGlyphYEnd;
                }

                fittingLineIndex = i;
                fittingLineWasFound = true;
            }
            else
            {
                if (curGlyphYEnd < currentGlyphLine.d_maximumExtentY)
                {
                    fittingLineIndex = i;
                    fittingLineWasFound = true;
                }
            }
        }
    }

    if(!fittingLineWasFound)
    {
        fittingLineWasFound = addNewLineIfFitting(glyphHeight, fittingLineIndex);
    }

    if(!fittingLineWasFound)
    {
        createTextureSpaceForGlyphRasterisation(texture, glyphWidth, glyphHeight);

        rasterise(glyph);
        return;
    }

    const auto& glyphTexLine = d_textureGlyphLines[fittingLineIndex];

    // Create the data containing the pixels of the glyph
    FT_Bitmap& glyphBitmap = d_fontFace->glyph->bitmap;
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

    const glm::vec2 offset(d_fontFace->glyph->metrics.horiBearingX * static_cast<float>(s_conversionMultCoeff),
        -d_fontFace->glyph->metrics.horiBearingY * static_cast<float>(s_conversionMultCoeff));

    const String name(PropertyHelper<std::uint32_t>::toString(glyph->getCodePoint()));
    BitmapImage* img = new BitmapImage(name, texture, area, offset, AutoScaledMode::Disabled,
            d_nativeResolution);
    d_glyphImages.push_back(img);
    glyph->setImage(img);

    // Advance to next position, add padding
    glyphTexLine.d_lastXPos += glyphWidth + s_glyphPadding;
}

bool FreeTypeFont::addNewLineIfFitting(unsigned int glyphHeight, size_t& fittingLineIndex) const
{
    const auto& lastLine = d_textureGlyphLines.back();

    unsigned int newLinePosY = lastLine.d_maximumExtentY + s_glyphPadding;
    int newMaxYExtent = newLinePosY + glyphHeight;
    if (newMaxYExtent <= d_lastTextureSize)
    {
        // Add the glyph in a new line
        d_textureGlyphLines.push_back(TextureGlyphLine(0, newLinePosY, newMaxYExtent));
        fittingLineIndex = d_textureGlyphLines.size() - 1;
        return true;
    }

    return false;
}

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
    d_textureGlyphLines.clear();
    d_textureGlyphLines.push_back(TextureGlyphLine());
}

//----------------------------------------------------------------------------//
std::vector<argb_t> FreeTypeFont::createGlyphTextureData(FT_Bitmap& glyphBitmap)
{
    unsigned int bitmapHeight = static_cast<unsigned int>(glyphBitmap.rows);
    unsigned int bitmapWidth = static_cast<unsigned int>(glyphBitmap.width);

    std::vector<argb_t> glyphTextureData;
    glyphTextureData.resize(bitmapHeight * bitmapWidth);

    for (unsigned int i = 0; i < bitmapHeight; ++i)
    {
        argb_t* currentRow =  glyphTextureData.data() + i * bitmapWidth;

        for (unsigned int j = 0; j < bitmapWidth; ++j)
        {
            std::uint8_t* src = glyphBitmap.buffer + (i * glyphBitmap.pitch + j);

            switch (glyphBitmap.pixel_mode)
            {
            case FT_PIXEL_MODE_GRAY:
            {
                currentRow[j] = Colour::calculateArgb(*src, 0xFF, 0xFF, 0xFF);
                break;
            }
            case FT_PIXEL_MODE_MONO:
            {
                currentRow[j] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFFFF : 0x00000000;
                break;
            }
            default:
                throw InvalidRequestException(
                    "The glyph could not be drawn because the pixel mode is "
                    "unsupported.");
            }
        }
    }

    return glyphTextureData;
}

//----------------------------------------------------------------------------//
void FreeTypeFont::updateTextureBufferSubImage(argb_t* destTextureData, unsigned int bitmapWidth,
    unsigned int bitmapHeight, const std::vector<argb_t>& subImageData) const
{
    argb_t* curDestPixelLine = destTextureData;

    for (unsigned int i = 0;  i < bitmapHeight;  ++i)
    {
        argb_t* curDestPixel = curDestPixelLine;

        for (unsigned int j = 0; j < bitmapWidth; ++j)
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

    for(auto codePointMapEntry : d_codePointToGlyphMap)
    {
        delete codePointMapEntry.second;
    }

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

void FreeTypeFont::createFreetypeMemoryFace()
{
    // create face using input font
    FT_Error error = FT_New_Memory_Face(s_freetypeLibHandle, d_fontData.getDataPtr(),
        static_cast<FT_Long>(d_fontData.getSize()), 0,
        &d_fontFace);

    if (error != 0)
    {
        findAndThrowFreeTypeError(error, "Failed to create face from font file");
    }
}

void FreeTypeFont::findAndThrowFreeTypeError(
    FT_Error error,
    const String& errorMessageIntro) const
{
    String errorMsg = "Unknown freetype error occurred: " + error;

    for (size_t i = 0; i < freeTypeErrorDescriptions.size(); ++i)
    {
        const FreeTypeErrorDescription& currentFreetypeError = freeTypeErrorDescriptions[i];

        if (currentFreetypeError.err_code == error)
        {
            errorMsg = currentFreetypeError.err_msg;
        }
    }

    throw GenericException(errorMessageIntro + " - '" +
        d_filename + "' error was: " + errorMsg);
}

void FreeTypeFont::checkUnicodeCharMapAvailability()
{
    if (d_fontFace->charmap != nullptr)
    {
        return;
    }
    FT_Done_Face(d_fontFace);
    d_fontFace = nullptr;
    throw GenericException(
        "The font '" + d_name + "' does not have a Unicode charmap, and "
        "cannot be used.");
}

void FreeTypeFont::tryToCreateFontWithClosestFontHeight(
    FT_Error errorResult,
    int requestedFontPixelHeight) const
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
        {
            findAndThrowFreeTypeError(errorResult, "Failed to create Font using "
                "the closest available Font size that was found");
        }
    }

    if (errorResult != 0)
    {
        std::stringstream& sstream = SharedStringstream::GetPreparedStream();
        sstream << d_size;
        throw GenericException("The font '" + d_name + "' requested at height "
            "of " + sstream.str() + " pixels, could not be created and therefore"
            " not used");
    }
}

//----------------------------------------------------------------------------//
void FreeTypeFont::updateFont()
{
    free();

    System::getSingleton().getResourceProvider()->loadRawDataContainer(
        d_filename, d_fontData, d_resourceGroup.empty() ?
            getDefaultResourceGroup() : d_resourceGroup);

    createFreetypeMemoryFace();

    checkUnicodeCharMapAvailability();
    
    float fontScaleFactor = System::getSingleton().getRenderer()->getFontScale();
    if (d_autoScaled != AutoScaledMode::Disabled)
    {
        fontScaleFactor *= d_vertScaling;
    }

    int requestedFontPixelHeight = std::lround(getSizeInPixels() * fontScaleFactor);


    FT_Error errorResult = FT_Set_Pixel_Sizes(d_fontFace, 0, requestedFontPixelHeight);
    if(errorResult != 0)
    {
        // Usually, an error occurs with a fixed-size font format (like FNT or PCF)
        // when trying to set the pixel size to a value that is not listed in the
        // face->fixed_sizes array.For bitmap fonts we can render only at specific
        // point sizes.
        // Try to find Font with closest pixel height and use it instead

        tryToCreateFontWithClosestFontHeight(errorResult, requestedFontPixelHeight);
    }

    if (d_fontFace->face_flags & FT_FACE_FLAG_SCALABLE)
    {
        float y_scale = d_fontFace->size->metrics.y_scale * float(s_conversionMultCoeff) * (1.0f / 65536.0f);
        d_ascender = d_fontFace->ascender * y_scale;
        d_descender = d_fontFace->descender * y_scale;
        d_height = d_fontFace->height * y_scale;
    }
    else
    {
        d_ascender = d_fontFace->size->metrics.ascender * float(s_conversionMultCoeff);
        d_descender = d_fontFace->size->metrics.descender * float(s_conversionMultCoeff);
        d_height = d_fontFace->size->metrics.height * float(s_conversionMultCoeff);
    }

    if (d_specificLineSpacing > 0.0f)
    {
        d_height = d_specificLineSpacing;
    }

    initialiseGlyphMap();
}

//----------------------------------------------------------------------------//
void FreeTypeFont::initialiseGlyphMap()
{
    FT_UInt gindex;
    FT_ULong codepoint = FT_Get_First_Char(d_fontFace, &gindex);

    while (gindex != 0)
    {

        if(d_codePointToGlyphMap.find(codepoint) != d_codePointToGlyphMap.end())
        {
            throw InvalidRequestException("FreeTypeFont::initialiseGlyphMap - Requesting "
                "adding an already added glyph to the codepoint glyph map.");        
        }

        FreeTypeFontGlyph* newFontGlyph = new FreeTypeFontGlyph(this, codepoint);
        d_codePointToGlyphMap[codepoint] = newFontGlyph;
        d_indexToGlyphMap[gindex] = static_cast<char32_t>(codepoint);

        codepoint = FT_Get_Next_Char(d_fontFace, codepoint, &gindex);
    }
}

//----------------------------------------------------------------------------//
void FreeTypeFont::prepareGlyph(FreeTypeFontGlyph* glyph) const
{
    if (glyph->isInitialised())
    {
        return;
    }

    // Load the code point, "rendering" the glyph
    FT_Int32 targetType = d_antiAliased ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO;
    FT_Error error = FT_Load_Char(d_fontFace, glyph->getCodePoint(), FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT |
        targetType);

    glyph->markAsInitialised();

    if (error != 0)
    {
        return;
    }

    bool isRendered = glyph->getImage() != nullptr;
    if (!isRendered)
    {
        rasterise(glyph);
    }

    const float adv =
        d_fontFace->glyph->metrics.horiAdvance * static_cast<float>(s_conversionMultCoeff);
    glyph->setAdvance(adv);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::writeXMLToStream_impl(XMLSerializer& xml_stream) const
{
    xml_stream.attribute(Font_xmlHandler::FontSizeAttribute,
                         PropertyHelper<float>::toString(d_size));
    if (!d_antiAliased)
        xml_stream.attribute(Font_xmlHandler::FontAntiAliasedAttribute, "false");

    if (d_specificLineSpacing > 0.0f)
        xml_stream.attribute(Font_xmlHandler::FontLineSpacingAttribute,
                             PropertyHelper<float>::toString(d_specificLineSpacing));
}

//----------------------------------------------------------------------------//
bool FreeTypeFont::isAntiAliased() const
{
    return d_antiAliased;
}

//----------------------------------------------------------------------------//
void FreeTypeFont::setSize(const float size)
{
    setSizeAndUnit(size, d_sizeUnit);
}

void FreeTypeFont::setSizeUnit(const FontSizeUnit sizeUnit)
{
    setSizeAndUnit(d_size, sizeUnit);
}

void FreeTypeFont::setSizeInPixels(const float pixelSize)
{
    setSizeAndUnit(pixelSize, FontSizeUnit::Pixels);
}

void FreeTypeFont::setSizeInPoints(const float pointSize)
{
    setSizeAndUnit(pointSize, FontSizeUnit::Points);
}

void FreeTypeFont::setSizeAndUnit(const float size, const FontSizeUnit sizeUnit)
{
    if (d_size == size && d_sizeUnit == sizeUnit)
    {
        return;
    }

    d_size = size;
    d_sizeUnit = sizeUnit;

    handleFontSizeOrFontUnitChange();
}

float FreeTypeFont::getSize() const
{
    return d_size;
}

FontSizeUnit FreeTypeFont::getSizeUnit() const
{
    return d_sizeUnit;
}

float FreeTypeFont::getSizeInPixels() const
{
    if (d_sizeUnit == FontSizeUnit::Pixels)
    {
        return d_size;
    }

    if (d_sizeUnit == FontSizeUnit::Points)
    {
        return convertPointsToPixels(d_size, Renderer::ReferenceDpiValue);
    }

    throw InvalidRequestException("FreeTypeFont::getSizeInPixels - Requesting "
        "font size in pixels but the Font size unit is invalid.");
}

float FreeTypeFont::getSizeInPoints() const
{
    if (d_sizeUnit == FontSizeUnit::Pixels)
    {
        return convertPixelsToPoints(d_size, Renderer::ReferenceDpiValue);
    }

    if (d_sizeUnit == FontSizeUnit::Points)
    {
        return d_size;
    }

    throw InvalidRequestException("FreeTypeFont::getSizeInPixels - Requesting "
        "font size in pixels but the Font size unit is invalid.");
}

void FreeTypeFont::handleFontSizeOrFontUnitChange()
{
    updateFont();

    FontEventArgs args(this);
    onRenderSizeChanged(args);
}


//----------------------------------------------------------------------------//
void FreeTypeFont::setAntiAliased(const bool anti_alaised)
{
    if (anti_alaised == d_antiAliased)
        return;

    d_antiAliased = anti_alaised;
    updateFont();

    FontEventArgs args(this);
    onRenderSizeChanged(args);
}

const FT_Face& FreeTypeFont::getFontFace() const
{
    return d_fontFace;
}

#ifdef CEGUI_USE_RAQM
std::vector<GeometryBuffer*> FreeTypeFont::layoutAndCreateGlyphRenderGeometry(
    const String& text,
    const Rectf* clip_rect, const ColourRect& colours,
    const float space_extra, const float x_scale, const float y_scale,
    ImageRenderSettings imgRenderSettings, glm::vec2& glyphPos) const
{
    std::vector<GeometryBuffer*> textGeometryBuffers;

    const float base_y = glyphPos.y + getBaseline(y_scale);


    if (text.empty())
    {
        return textGeometryBuffers;
    }

    raqm_t* raqmObject = raqm_create();

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    std::u32string utf32Text = String::convertUtf8ToUtf32(text);
    const uint32_t* originalTextArray = reinterpret_cast<const std::uint32_t*>(utf32Text.c_str());
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32) 
    const uint32_t* originalTextArray = reinterpret_cast<const std::uint32_t*>(text.c_str());
#endif
    bool wasSuccess = raqm_set_text(raqmObject, originalTextArray, text.length());
    if (!wasSuccess)
    {
        throw InvalidRequestException("Setting raqm text was unsuccessful");
    }

    FT_Face face = getFontFace();
    if (!raqm_set_freetype_face(raqmObject, face))
    {
        throw InvalidRequestException("Could not set the Freetype font Face for "
            "a raqm object");
    }

    if (!raqm_set_par_direction(raqmObject, RAQM_DIRECTION_DEFAULT))
    {
        throw InvalidRequestException("Could not set the parse direction for "
            "a raqm object");
    }

    wasSuccess = raqm_layout(raqmObject);
    if (!wasSuccess)
    {
        throw InvalidRequestException("Layouting raqm text was unsuccessful");
    }

    size_t count = 0;
    raqm_glyph_t* glyphs = raqm_get_glyphs(raqmObject, &count);

    for (size_t i = 0; i < count; i++)
    {
        raqm_glyph_t& currentGlyph = glyphs[i];

        char32_t codePoint;
        auto foundCodePointIter = d_indexToGlyphMap.find(currentGlyph.index);
        if (foundCodePointIter != d_indexToGlyphMap.end())
        {
            codePoint = foundCodePointIter->second;
        }
        else
        {
            // Ignore new line characters
            if(originalTextArray[currentGlyph.cluster] == '\n')
            {
                continue;
            }
            codePoint = UnicodeReplacementCharacter;
        }

        const FreeTypeFontGlyph* glyph = getPreparedGlyph(codePoint);
        if (glyph == nullptr && codePoint != UnicodeReplacementCharacter)
        {
            glyph = getPreparedGlyph(UnicodeReplacementCharacter);
        }

        if (glyph == nullptr)
        {
            continue;
        }

        const Image* const image = glyph->getImage();

        glyphPos.y = base_y - (image->getRenderedOffset().y -
            image->getRenderedOffset().y * y_scale);

        imgRenderSettings.d_destArea =
            Rectf(glyphPos, glyph->getSize(x_scale, y_scale));

        addGlyphRenderGeometry(textGeometryBuffers, image, imgRenderSettings,
            clip_rect, colours);

        glyphPos.x += currentGlyph.x_advance * x_scale * s_conversionMultCoeff;
        // TODO: This is probably wrong because the space was determined without kerning
        if (codePoint == ' ')
        {
            glyphPos.x += space_extra;
        }
    }

    raqm_destroy(raqmObject);

    return textGeometryBuffers;
}
#endif

bool FreeTypeFont::isCodepointAvailable(char32_t codePoint) const
{
    return d_codePointToGlyphMap.find(codePoint) != d_codePointToGlyphMap.end();
}


FreeTypeFontGlyph* FreeTypeFont::getGlyphForCodepoint(const char32_t codepoint) const
{
    CodePointToGlyphMap::const_iterator pos = d_codePointToGlyphMap.find(codepoint);
    if (pos != d_codePointToGlyphMap.end())
    {
        return pos->second;
    }

    return nullptr;
}

int FreeTypeFont::getInitialGlyphAtlasSize() const
{
    return d_initialGlyphAtlasSize;
}

void FreeTypeFont::setInitialGlyphAtlasSize(int val)
{
    d_initialGlyphAtlasSize = val;
}


const FreeTypeFontGlyph* FreeTypeFont::getPreparedGlyph(char32_t currentCodePoint) const
{
    FreeTypeFontGlyph* glyph = getGlyphForCodepoint(currentCodePoint);

    if (glyph != nullptr)
    {
        prepareGlyph(glyph);
    }

    return glyph;
}


} // End of  CEGUI namespace section
