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

//----------------------------------------------------------------------------//
unsigned int FreeTypeFont::getTextureSize(CodePointToGlyphMap::const_iterator s,
                                  CodePointToGlyphMap::const_iterator e) const
{
    unsigned int texsize = 32; // start with 32x32
    unsigned int max_texsize = System::getSingleton().getRenderer()->getMaxTextureSize();
    unsigned int glyph_count = 0;

    // Compute approximatively the optimal texture size for font
    while (texsize < max_texsize)
    {
        unsigned int x = s_glyphPadding, y = s_glyphPadding;
        unsigned int yb = s_glyphPadding;
        for (CodePointToGlyphMap::const_iterator c = s; c != e; ++c)
        {
            // skip glyphs that are already rendered
            if (c->second->getImage())
                continue;

            // load glyph metrics (don't render)
            if (FT_Load_Char(d_fontFace, c->first,
                             FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT))
                continue;

            unsigned int glyph_w = int (ceil(d_fontFace->glyph->metrics.width * s_conversionMultCoeff)) +
                           s_glyphPadding;
            unsigned int glyph_h = int (ceil(d_fontFace->glyph->metrics.height * s_conversionMultCoeff)) +
                           s_glyphPadding;

            x += glyph_w;
            if (x > texsize)
            {
                x = s_glyphPadding;
                y = yb;
            }
            unsigned int yy = y + glyph_h;
            if (yy > texsize)
                goto too_small;

            if (yy > yb)
                yb = yy;

            ++glyph_count;
        }
        // Okay, the texture size is enough for holding our glyphs
        break;

    too_small:
        texsize *= 2;
    }

    return glyph_count ? texsize : 0;
}

//----------------------------------------------------------------------------//
void FreeTypeFont::rasterise(FreeTypeFontGlyph* glyph) const
{
    if(d_glyphTextures.empty())
    {
        createNewTexture();
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
        auto& currentGlyphLine = d_textureGlyphLines[i];
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
        //Bigger sized texture needed
        return;
    }

    const auto& glyphTexLine = d_textureGlyphLines[fittingLineIndex];

    // Copy rendered glyph to memory buffer in RGBA format
    size_t bufferDataGlyphPos = (glyphTexLine.d_lastYPos * d_lastTextureSize) + glyphTexLine.d_lastXPos;
    drawGlyphToBuffer(d_lastTextureBuffer.data() + bufferDataGlyphPos, d_lastTextureSize);

    // Create a new image in the imageset
    const Rectf area(static_cast<float>(glyphTexLine.d_lastXPos),
        static_cast<float>(glyphTexLine.d_lastYPos),
        static_cast<float>(glyphTexLine.d_lastXPos + glyphWidth),
        static_cast<float>(glyphTexLine.d_lastYPos + glyphHeight));

     //TODO: Is this needed ?
    const glm::vec2 offset(d_fontFace->glyph->metrics.horiBearingX * static_cast<float>(s_conversionMultCoeff),
        -d_fontFace->glyph->metrics.horiBearingY * static_cast<float>(s_conversionMultCoeff));

    const String name(PropertyHelper<std::uint32_t>::toString(glyph->getCodePoint()));
    BitmapImage* img =
        new BitmapImage(name, texture, area, offset, AutoScaledMode::Disabled,
            d_nativeResolution);
    d_glyphImages.push_back(img);
    glyph->setImage(img);

    // Advance to next position, add padding
    glyphTexLine.d_lastXPos += glyphWidth + s_glyphPadding;

    // Copy our memory buffer into the texture and free it
    Sizef textureSize(static_cast<float>(d_lastTextureSize), static_cast<float>(d_lastTextureSize));
    texture->loadFromMemory(d_lastTextureBuffer.data(), textureSize, Texture::PixelFormat::Rgba);
}

//----------------------------------------------------------------------------//
void FreeTypeFont::rasterise(char32_t startCodePoint, char32_t endCodePoint) const
{
    CodePointToGlyphMap::iterator s = d_codePointToGlyphMap.lower_bound(startCodePoint);
    if (s == d_codePointToGlyphMap.end())
        return;

    CodePointToGlyphMap::iterator orig_s = s;
    CodePointToGlyphMap::iterator e = d_codePointToGlyphMap.upper_bound(endCodePoint);
    while (true)
    {
        // Create a new Imageset for glyphs
        unsigned int texsize = getTextureSize(s, e);
        // If all glyphs were already rendered, do nothing
        if (!texsize)
            break;

        const String texture_name(d_name + "_auto_glyph_images_" +
                                   PropertyHelper<std::uint32_t>::toString(s->first));
        Texture& texture = System::getSingleton().getRenderer()->createTexture(
            texture_name, Sizef(static_cast<float>(texsize), static_cast<float>(texsize)));
        d_glyphTextures.push_back(&texture);

        // Create a memory buffer where we will render our glyphs
        std::vector<argb_t> mem_buffer(texsize * texsize, 0);

        // Go ahead, line by line, top-left to bottom-right
        unsigned int x = s_glyphPadding, y = s_glyphPadding;
        unsigned int yb = s_glyphPadding;

        // Set to true when we finish rendering all glyphs we were asked to
        bool finished = false;
        // Set to false when we reach d_codePointToGlyphMap.end() and we start going backward
        bool forward = true;

        /* To conserve texture space we will render more glyphs than asked,
         * but never less than asked. First we render all glyphs from s to e
         * and after that we render glyphs until we reach d_codePointToGlyphMap.end(),
         * and if there's still free texture space we will go backward
         * from s until we hit d_codePointToGlyphMap.begin().
         */
        while (s != d_codePointToGlyphMap.end())
        {
            // Check if we finished rendering all the required glyphs
            finished |= (s == e);

            // Check if glyph already rendered
            if (!s->second->getImage())
            {
                // Render the glyph
                if (FT_Load_Char(d_fontFace, s->first, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT |
                                 (d_antiAliased ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO)))
                {
                    std::stringstream err;
                    err << "Font::loadFreetypeGlyph - Failed to load glyph for codepoint: ";
                    err << static_cast<unsigned int>(s->first);
                    err << ".  Will use an empty image for this glyph!";
                    Logger::getSingleton().logEvent(err.str().c_str(), LoggingLevel::Error);

                    // Create a 'null' image for this glyph so we do not seg later
                    const Rectf area(0, 0, 0, 0);
                    const glm::vec2 offset(0, 0);
                    const String name(PropertyHelper<std::uint32_t>::toString(s->first));
                    BitmapImage* img =
                        new BitmapImage(name, &texture, area, offset, AutoScaledMode::Disabled,
                                       d_nativeResolution);
                    d_glyphImages.push_back(img);
                    s->second->setImage(img);
                }
                else
                {
                    unsigned int glyph_w = d_fontFace->glyph->bitmap.width + s_glyphPadding;
                    unsigned int glyph_h = d_fontFace->glyph->bitmap.rows + s_glyphPadding;

                    // Check if glyph right margin does not exceed texture size
                    unsigned int x_next = x + glyph_w;
                    if (x_next > texsize)
                    {
                        x = s_glyphPadding;
                        x_next = x + glyph_w;
                        y = yb;
                    }

                    // Check if glyph bottom margin does not exceed texture size
                    unsigned int y_bot = y + glyph_h;
                    if (y_bot > texsize)
                        break;

                    // Copy rendered glyph to memory buffer in RGBA format
                    drawGlyphToBuffer(&mem_buffer[0] + (y * texsize) + x, texsize);

                    // Create a new image in the imageset
                    const Rectf area(static_cast<float>(x),
                                     static_cast<float>(y),
                                     static_cast<float>(x + glyph_w - s_glyphPadding),
                                     static_cast<float>(y + glyph_h - s_glyphPadding));

                    const glm::vec2 offset(
                        d_fontFace->glyph->metrics.horiBearingX * static_cast<float>(s_conversionMultCoeff),
                        -d_fontFace->glyph->metrics.horiBearingY * static_cast<float>(s_conversionMultCoeff));

                    const String name(PropertyHelper<std::uint32_t>::toString(s->first));
                    BitmapImage* img =
                        new BitmapImage(name, &texture, area, offset, AutoScaledMode::Disabled,
                                       d_nativeResolution);
                    d_glyphImages.push_back(img);
                    s->second->setImage(img);

                    // Advance to next position
                    x = x_next;
                    if (y_bot > yb)
                    {
                        yb = y_bot;
                    }
                }
            }

            // Go to next glyph, if we are going forward
            if (forward)
                if (++s == d_codePointToGlyphMap.end())
                {
                    finished = true;
                    forward = false;
                    s = orig_s;
                }
            // Go to previous glyph, if we are going backward
            if (!forward)
                if ((s == d_codePointToGlyphMap.begin()) || (--s == d_codePointToGlyphMap.begin()))
                    break;
        }

        // Copy our memory buffer into the texture and free it
        Sizef textureSize(static_cast<float>(texsize), static_cast<float>(texsize));
        texture.loadFromMemory(&mem_buffer[0], textureSize, Texture::PixelFormat::Rgba);

        if (finished)
            break;
    }
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

void FreeTypeFont::createNewTexture() const
{
    const String texture_name(d_name + "_auto_glyph_images_texture_" +
        PropertyHelper<std::uint32_t>::toString(d_glyphTextures.size() - 1));

    d_lastTextureSize = d_initialGlyphAtlasSize;
    Sizef textureSize(static_cast<float>(d_lastTextureSize),
        static_cast<float>(d_lastTextureSize));
    Texture& texture = System::getSingleton().getRenderer()->createTexture(
        texture_name, textureSize);
    d_glyphTextures.push_back(&texture);

    d_lastTextureBuffer = std::vector<argb_t>(d_lastTextureSize * d_lastTextureSize, 0);
    d_textureGlyphLines.push_back(TextureGlyphLine());
}

//----------------------------------------------------------------------------//
void FreeTypeFont::drawGlyphToBuffer(argb_t *buffer, unsigned int buf_width) const
{
    FT_Bitmap *glyph_bitmap = &d_fontFace->glyph->bitmap;

    unsigned int glyph_bitmap_height =
      static_cast<unsigned int>(glyph_bitmap->rows);
    unsigned int glyph_bitmap_width =
      static_cast<unsigned int>(glyph_bitmap->width);
    for (unsigned int i = 0;  i < glyph_bitmap_height;  ++i)
    {
        std::uint8_t *src = glyph_bitmap->buffer + (i * glyph_bitmap->pitch);
        switch (glyph_bitmap->pixel_mode)
        {
        case FT_PIXEL_MODE_GRAY:
        {
            std::uint8_t *dst = reinterpret_cast<std::uint8_t*>(buffer);
            for (unsigned int j = 0;  j < glyph_bitmap_width;  ++j)
            {
                // RGBA
                *dst++ = 0xFF;
                *dst++ = 0xFF;
                *dst++ = 0xFF;
                *dst++ = *src++;
            }
        }
        break;

        case FT_PIXEL_MODE_MONO:
            for (unsigned int j = 0;  j < glyph_bitmap_width;  ++j)
                buffer [j] = (src [j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFFFF : 0x00000000;
            break;

        default:
            throw InvalidRequestException(
                "The glyph could not be drawn because the pixel mode is "
                "unsupported.");
            break;
        }

        buffer += buf_width;
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
        /*
        TODO: rectangle replacement

        std::stringstream err;
        err << "Font::loadFreetypeGlyph - Failed to load glyph for codepoint: ";
        err << static_cast<unsigned int>(s->first);
        err << ".  Will use an empty image for this glyph!";
        Logger::getSingleton().logEvent(err.str().c_str(), LoggingLevel::Error);

        // Create a 'null' image for this glyph so we do not seg later
        const Rectf area(0, 0, 0, 0);
        const glm::vec2 offset(0, 0);
        const String name(PropertyHelper<std::uint32_t>::toString(s->first));
        BitmapImage* img =
            new BitmapImage(name, &texture, area, offset, AutoScaledMode::Disabled,
                d_nativeResolution);
        d_glyphImages.push_back(img);
        s->second->setImage(img);
        */

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
void FreeTypeFont::layoutAndRenderGlyphs(const String& text,
    const glm::vec2& position, const Rectf* clip_rect,
    const ColourRect& colours, const float space_extra,
    const float x_scale, const float y_scale,
    ImageRenderSettings imgRenderSettings, glm::vec2& glyph_pos,
    GeometryBuffer*& textGeometryBuffer) const
{
    textGeometryBuffer = nullptr;

    if (text.empty())
    {
        return;
    }

    const float base_y = position.y + getBaseline(y_scale);
    glyph_pos = position;

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

        if (currentGlyph.index == 0)
        {
            glyph_pos.x += currentGlyph.x_advance * x_scale * s_conversionMultCoeff;
            glyph_pos.x = std::roundf(glyph_pos.x);
            continue;
        }

        char32_t codePoint;
        auto foundCodePointIter = d_indexToGlyphMap.find(currentGlyph.index);
        if (foundCodePointIter == d_indexToGlyphMap.end())
        {
            codePoint = UnicodeReplacementCharacter;
        }
        else
        {
            codePoint = foundCodePointIter->second;
        }

        const FreeTypeFontGlyph* glyph = getPreparedGlyph(codePoint);
        if (glyph == nullptr)
        {
            //TODO: Show rectangle glyph
            continue;
        }

        const Image* const img = glyph->getImage();

        glyph_pos.y = base_y - (img->getRenderedOffset().y -
            img->getRenderedOffset().y * y_scale);

        imgRenderSettings.d_destArea =
            Rectf(glyph_pos, glyph->getSize(x_scale, y_scale));

        // We only fully create the first GeometryBuffer
        if (textGeometryBuffer == nullptr)
        {
            std::vector<GeometryBuffer*> currentGeombuffs =
                img->createRenderGeometry(imgRenderSettings);

            assert(currentGeombuffs.size() <= 1 && "Glyphs are expected to "
                "be built from a single GeometryBuffer (or none)");

            if (currentGeombuffs.size() == 1)
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

        glyph_pos.x += glyph->getAdvance(x_scale);//currentGlyph.x_advance * x_scale * s_conversionMultCoeff;
        glyph_pos.x = std::roundf(glyph_pos.x);
    }


    raqm_destroy(raqmObject);
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
