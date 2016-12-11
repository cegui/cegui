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
#include <cstdio>


namespace CEGUI
{
//----------------------------------------------------------------------------//
// Pixels to put between glyphs
#define INTER_GLYPH_PAD_SPACE 2
// A multiplication coefficient to convert FT_Pos values into normal floats
#define FT_POS_COEF  (1.0/64.0)

//----------------------------------------------------------------------------//
// Font objects usage count
static int ft_usage_count = 0;
// A handle to the FreeType library
static FT_Library ft_lib;

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
    if (!ft_usage_count++)
        FT_Init_FreeType(&ft_lib);

    addFreeTypeFontProperties();

    FreeTypeFont::updateFont();

    std::stringstream& sstream = SharedStringstream::GetPreparedStream();
    sstream << "Successfully loaded " << d_cp_map.size() << " glyphs";
    Logger::getSingleton().logEvent(sstream.str(), LoggingLevel::INFORMATIVE);
}

//----------------------------------------------------------------------------//
FreeTypeFont::~FreeTypeFont()
{
    free();

    if (!--ft_usage_count)
        FT_Done_FreeType(ft_lib);
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
        &FreeTypeFont::setSizeUnit, &FreeTypeFont::getSizeUnit, FontSizeUnit::FSU_PIXELS
    );

    CEGUI_DEFINE_PROPERTY(FreeTypeFont, bool,
        "Antialiased", "This is a flag indicating whenever to render antialiased font or not. "
        "Value is either true or false.",
        &FreeTypeFont::setAntiAliased, &FreeTypeFont::isAntiAliased, 0
    );
}

//----------------------------------------------------------------------------//
unsigned int FreeTypeFont::getTextureSize(CodepointMap::const_iterator s,
                                  CodepointMap::const_iterator e) const
{
    unsigned int texsize = 32; // start with 32x32
    unsigned int max_texsize = System::getSingleton().getRenderer()->getMaxTextureSize();
    unsigned int glyph_count = 0;

    // Compute approximatively the optimal texture size for font
    while (texsize < max_texsize)
    {
        unsigned int x = INTER_GLYPH_PAD_SPACE, y = INTER_GLYPH_PAD_SPACE;
        unsigned int yb = INTER_GLYPH_PAD_SPACE;
        for (CodepointMap::const_iterator c = s; c != e; ++c)
        {
            // skip glyphs that are already rendered
            if (c->second->getImage())
                continue;

            // load glyph metrics (don't render)
            if (FT_Load_Char(d_fontFace, c->first,
                             FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT))
                continue;

            unsigned int glyph_w = int (ceil(d_fontFace->glyph->metrics.width * FT_POS_COEF)) +
                           INTER_GLYPH_PAD_SPACE;
            unsigned int glyph_h = int (ceil(d_fontFace->glyph->metrics.height * FT_POS_COEF)) +
                           INTER_GLYPH_PAD_SPACE;

            x += glyph_w;
            if (x > texsize)
            {
                x = INTER_GLYPH_PAD_SPACE;
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
void FreeTypeFont::rasterise(char32_t start_codepoint, char32_t end_codepoint) const
{
    CodepointMap::iterator s = d_cp_map.lower_bound(start_codepoint);
    if (s == d_cp_map.end())
        return;

    CodepointMap::iterator orig_s = s;
    CodepointMap::iterator e = d_cp_map.upper_bound(end_codepoint);
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
        unsigned int x = INTER_GLYPH_PAD_SPACE, y = INTER_GLYPH_PAD_SPACE;
        unsigned int yb = INTER_GLYPH_PAD_SPACE;

        // Set to true when we finish rendering all glyphs we were asked to
        bool finished = false;
        // Set to false when we reach d_cp_map.end() and we start going backward
        bool forward = true;

        /* To conserve texture space we will render more glyphs than asked,
         * but never less than asked. First we render all glyphs from s to e
         * and after that we render glyphs until we reach d_cp_map.end(),
         * and if there's still free texture space we will go backward
         * from s until we hit d_cp_map.begin().
         */
        while (s != d_cp_map.end())
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
                    Logger::getSingleton().logEvent(err.str().c_str(), LoggingLevel::ERROR_LEVEL);

                    // Create a 'null' image for this glyph so we do not seg later
                    const Rectf area(0, 0, 0, 0);
                    const glm::vec2 offset(0, 0);
                    const String name(PropertyHelper<std::uint32_t>::toString(s->first));
                    BitmapImage* img =
                        new BitmapImage(name, &texture, area, offset, AutoScaledMode::DISABLED,
                                       d_nativeResolution);
                    d_glyphImages.push_back(img);
                    s->second->setImage(img);
                }
                else
                {
                    unsigned int glyph_w = d_fontFace->glyph->bitmap.width + INTER_GLYPH_PAD_SPACE;
                    unsigned int glyph_h = d_fontFace->glyph->bitmap.rows + INTER_GLYPH_PAD_SPACE;

                    // Check if glyph right margin does not exceed texture size
                    unsigned int x_next = x + glyph_w;
                    if (x_next > texsize)
                    {
                        x = INTER_GLYPH_PAD_SPACE;
                        x_next = x + glyph_w;
                        y = yb;
                    }

                    // Check if glyph bottom margine does not exceed texture size
                    unsigned int y_bot = y + glyph_h;
                    if (y_bot > texsize)
                        break;

                    // Copy rendered glyph to memory buffer in RGBA format
                    drawGlyphToBuffer(&mem_buffer[0] + (y * texsize) + x, texsize);

                    // Create a new image in the imageset
                    const Rectf area(static_cast<float>(x),
                                     static_cast<float>(y),
                                     static_cast<float>(x + glyph_w - INTER_GLYPH_PAD_SPACE),
                                     static_cast<float>(y + glyph_h - INTER_GLYPH_PAD_SPACE));

                    const glm::vec2 offset(
                        d_fontFace->glyph->metrics.horiBearingX * static_cast<float>(FT_POS_COEF),
                        -d_fontFace->glyph->metrics.horiBearingY * static_cast<float>(FT_POS_COEF));

                    const String name(PropertyHelper<std::uint32_t>::toString(s->first));
                    BitmapImage* img =
                        new BitmapImage(name, &texture, area, offset, AutoScaledMode::DISABLED,
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
                if (++s == d_cp_map.end())
                {
                    finished = true;
                    forward = false;
                    s = orig_s;
                }
            // Go to previous glyph, if we are going backward
            if (!forward)
                if ((s == d_cp_map.begin()) || (--s == d_cp_map.begin()))
                    break;
        }

        // Copy our memory buffer into the texture and free it
        texture.loadFromMemory(&mem_buffer[0], Sizef(static_cast<float>(texsize), static_cast<float>(texsize)), Texture::PixelFormat::RGBA);

        if (finished)
            break;
    }
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

    for(auto codePointMapEntry : d_cp_map)
    {
        delete codePointMapEntry.second;
    }

    d_cp_map.clear();

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
    FT_Error error = FT_New_Memory_Face(ft_lib, d_fontData.getDataPtr(),
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
    if (d_autoScaled != AutoScaledMode::DISABLED)
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
        float y_scale = d_fontFace->size->metrics.y_scale * float(FT_POS_COEF) * (1.0f / 65536.0f);
        d_ascender = d_fontFace->ascender * y_scale;
        d_descender = d_fontFace->descender * y_scale;
        d_height = d_fontFace->height * y_scale;
    }
    else
    {
        d_ascender = d_fontFace->size->metrics.ascender * float(FT_POS_COEF);
        d_descender = d_fontFace->size->metrics.descender * float(FT_POS_COEF);
        d_height = d_fontFace->size->metrics.height * float(FT_POS_COEF);
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
    FT_ULong max_codepoint = codepoint;

    while (gindex != 0)
    {
        if (max_codepoint < codepoint)
            max_codepoint = codepoint;

        if(d_cp_map.find(codepoint) != d_cp_map.end())
        {
            throw InvalidRequestException("FreeTypeFont::initialiseGlyphMap - Requesting "
                "adding an already added glyph to the codepoint glyph map.");        
        }

        d_cp_map[codepoint] = new FreeTypeFontGlyph(this, gindex);

        codepoint = FT_Get_Next_Char(d_fontFace, codepoint, &gindex);
    }

    setMaxCodepoint(max_codepoint);
}

//----------------------------------------------------------------------------//
const FontGlyph* FreeTypeFont::findFontGlyph(const char32_t codepoint) const
{
    CodepointMap::iterator pos = d_cp_map.find(codepoint);

    if (pos == d_cp_map.end())
        return nullptr;

    if (!pos->second->isValid())
        initialiseFontGlyph(pos);

    return pos->second;
}

//----------------------------------------------------------------------------//
void FreeTypeFont::initialiseFontGlyph(CodepointMap::iterator cp) const
{
    // load-up required glyph metrics (don't render)
    if (FT_Load_Char(d_fontFace, cp->first,
                     FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT))
        return;

    const float adv =
        d_fontFace->glyph->metrics.horiAdvance * static_cast<float>(FT_POS_COEF);

    cp->second->setAdvance(adv);
    cp->second->setValid(true);
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
    setSizeAndUnit(pixelSize, FontSizeUnit::FSU_PIXELS);
}

void FreeTypeFont::setSizeInPoints(const float pointSize)
{
    setSizeAndUnit(pointSize, FontSizeUnit::FSU_POINTS);
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
    if (d_sizeUnit == FontSizeUnit::FSU_PIXELS)
    {
        return d_size;
    }

    if (d_sizeUnit == FontSizeUnit::FSU_POINTS)
    {
        return convertPointsToPixels(d_size, Renderer::ReferenceDpiValue);
    }

    throw InvalidRequestException("FreeTypeFont::getSizeInPixels - Requesting "
        "font size in pixels but the Font size unit is invalid.");
}

float FreeTypeFont::getSizeInPoints() const
{
    if (d_sizeUnit == FontSizeUnit::FSU_PIXELS)
    {
        return convertPixelsToPoints(d_size, Renderer::ReferenceDpiValue);
    }

    if (d_sizeUnit == FontSizeUnit::FSU_POINTS)
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
    if (glyphs == nullptr)
    {
        throw InvalidRequestException("Layouting raqm text was unsuccessful");
    }

    for (size_t i = 0; i < count; i++)
    {
        raqm_glyph_t& currentGlyph = glyphs[i];

        uint32_t& originalArrayIndex = currentGlyph.cluster;
        const char32_t curCodePoint = originalTextArray[originalArrayIndex];
        const FontGlyph* glyph = getGlyphData(curCodePoint);
        if (glyph == nullptr)
        {
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

        glyph_pos.x += glyph->getAdvance(x_scale);//currentGlyph.x_advance * x_scale * FT_POS_COEF;
        glyph_pos.x = std::roundf(glyph_pos.x);
        // apply extra spacing to space chars
        if (curCodePoint == ' ')
        {
            glyph_pos.x += space_extra;
        }
    }

    raqm_destroy(raqmObject);
}
#endif


} // End of  CEGUI namespace section
