/************************************************************************
    filename: 	CEGUIFont.cpp
    created:	21/2/2004
    author:		Paul D Turner

    purpose:	Implements Font class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGUIFont.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUITexture.h"
#include "CEGUILogger.h"
#include "CEGUITextUtils.h"
#include "CEGUIFont_xmlHandler.h"
#include "CEGUIFont_implData.h"
#include "CEGUIResourceProvider.h"
#include "CEGUIXMLParser.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <sstream>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    static data definitions
*************************************************************************/
const argb_t Font::DefaultColour     = 0xFFFFFFFF;
const uint	Font::InterGlyphPadSpace = 2;
String Font::d_defaultResourceGroup;

// XML related strings
const char	Font::FontSchemaName[]   = "Font.xsd";


/*************************************************************************
    Constructs a new Font object from a font definition file
*************************************************************************/
Font::Font(const String& filename, const String& resourceGroup, FontImplData* dat) :
        d_glyph_images(0),
        d_freetype(false),
        d_lineHeight(0),
        d_lineSpacing(0),
        d_max_bearingY(0),
        d_maxGlyphHeight(0),
        d_impldat(dat),
        d_ptSize(0),
        d_autoScale(false),
        d_horzScaling(1.0f),
        d_vertScaling(1.0f),
        d_nativeHorzRes(DefaultNativeHorzRes),
        d_nativeVertRes(DefaultNativeVertRes),
        d_antiAliased(false)
{
    load(filename, resourceGroup);

    // complete y spacing set-up for bitmap / static fonts
    calculateStaticVertSpacing();
}


/*************************************************************************
    Constructs a new Font object (via FreeType & a true-type font file)
    'glyph-set' describes the set of code points to be available via
    this font
*************************************************************************/
Font::Font(const String& name, const String& fontname, const String& resourceGroup, uint size, uint flags, const String& glyph_set, FontImplData* dat) :
        d_glyph_images(0),
        d_freetype(false),
        d_lineHeight(0),
        d_lineSpacing(0),
        d_max_bearingY(0),
        d_maxGlyphHeight(0),
        d_impldat(dat),
        d_ptSize(0),
        d_autoScale(false),
        d_horzScaling(1.0f),
        d_vertScaling(1.0f),
        d_nativeHorzRes(DefaultNativeHorzRes),
        d_nativeVertRes(DefaultNativeVertRes),
        d_antiAliased(false)
{
    constructor_impl(name,
                     fontname,
                     resourceGroup,
                     size,
                     flags,
                     glyph_set);
}


/*************************************************************************
    Constructs a new Font object (via FreeType & a true-type font file)
    [first_code_point, last_code_point] describes the range of code
    points to be available via this font
*************************************************************************/
Font::Font(const String& name, const String& fontname, const String& resourceGroup, uint size, uint flags, utf32 first_code_point, utf32 last_code_point, FontImplData* dat) :
        d_glyph_images(0),
        d_freetype(false),
        d_lineHeight(0),
        d_lineSpacing(0),
        d_max_bearingY(0),
        d_maxGlyphHeight(0),
        d_impldat(dat),
        d_ptSize(0),
        d_autoScale(false),
        d_horzScaling(1.0f),
        d_vertScaling(1.0f),
        d_nativeHorzRes(DefaultNativeHorzRes),
        d_nativeVertRes(DefaultNativeVertRes),
        d_antiAliased(false)
{
    constructor_impl(name,
                     fontname,
                     resourceGroup,
                     size,
                     flags,
                     getCodepointRangeAsString(first_code_point, last_code_point));
}


/*************************************************************************
    Constructs a new Font object (via FreeType & a true-type font file)
    The font file will provide support for 7-bit ASCII characters only
*************************************************************************/
Font::Font(const String& name, const String& fontname, const String& resourceGroup, uint size, uint flags, FontImplData* dat) :
        d_glyph_images(0),
        d_freetype(false),
        d_lineHeight(0),
        d_lineSpacing(0),
        d_max_bearingY(0),
        d_maxGlyphHeight(0),
        d_impldat(dat),
        d_ptSize(0),
        d_autoScale(false),
        d_horzScaling(1.0f),
        d_vertScaling(1.0f),
        d_nativeHorzRes(DefaultNativeHorzRes),
        d_nativeVertRes(DefaultNativeVertRes),
        d_antiAliased(false)
{
    constructor_impl(name,
                     fontname,
                     resourceGroup,
                     size,
                     flags,
                     getCodepointRangeAsString(32, 127));
}


/*************************************************************************
    Destroys a Font object
*************************************************************************/
Font::~Font(void)
{
    unload();
    delete d_impldat;
}


/*************************************************************************
    Return the pixel width of the specified text if rendered with this Font.
*************************************************************************/
float Font::getTextExtent(const String& text, float x_scale) const
{
    const GlyphDat* glyph;
    float cur_extent = 0, adv_extent = 0, width;

    for (size_t c = 0; c < text.length(); ++c)
    {
        glyph = getGlyphData(text[c]);

        if (glyph)
        {
            width = glyph->getRenderedAdvance(x_scale);

            if (adv_extent + width > cur_extent)
                cur_extent = adv_extent + width;

            adv_extent += glyph->getAdvance(x_scale);
        }
    }

    return ceguimax(adv_extent, cur_extent);
}


/*************************************************************************
    Return the index of the closest text character in String 'text' that
    corresponds to pixel location 'pixel' if the text were rendered.
*************************************************************************/
size_t Font::getCharAtPixel(const String& text, size_t start_char, float pixel, float x_scale) const
{
    const GlyphDat* glyph;
    float cur_extent = 0;
    size_t char_count = text.length();

    // handle simple cases
    if ((pixel <= 0) || (char_count <= start_char))
        return start_char;

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

    return char_count;
}


/*************************************************************************
    Renders text on the display.  Return number of lines output.
*************************************************************************/
size_t Font::drawText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale, float y_scale) const
{
    size_t thisCount;
    size_t lineCount = 0;

    float	y_base = draw_area.d_top + getBaseline(y_scale);

    Rect tmpDrawArea(
        PixelAligned(draw_area.d_left),
        PixelAligned(draw_area.d_top),
        PixelAligned(draw_area.d_right),
        PixelAligned(draw_area.d_bottom)
    );

    size_t lineStart = 0, lineEnd = 0;
    String	currLine;

    while (lineEnd < text.length())
    {
        if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
            lineEnd = text.length();

        currLine = text.substr(lineStart, lineEnd - lineStart);
        lineStart = lineEnd + 1;	// +1 to skip \n char

        switch(fmt)
        {
        case LeftAligned:
            drawTextLine(currLine, Vector3(tmpDrawArea.d_left, y_base, z), clip_rect, colours, x_scale, y_scale);
            thisCount = 1;
            y_base += getLineSpacing(y_scale);
            break;

        case RightAligned:
            drawTextLine(currLine, Vector3(tmpDrawArea.d_right - getTextExtent(currLine, x_scale), y_base, z), clip_rect, colours, x_scale, y_scale);
            thisCount = 1;
            y_base += getLineSpacing(y_scale);
            break;

        case Centred:
            drawTextLine(currLine, Vector3(PixelAligned(tmpDrawArea.d_left + ((tmpDrawArea.getWidth() - getTextExtent(currLine, x_scale)) / 2.0f)), y_base, z), clip_rect, colours, x_scale, y_scale);
            thisCount = 1;
            y_base += getLineSpacing(y_scale);
            break;

        case Justified:
            // new function in order to keep drawTextLine's signature unchanged
            drawTextLineJustified(currLine, draw_area, Vector3(tmpDrawArea.d_left, y_base, z), clip_rect, colours, x_scale, y_scale);
            thisCount = 1;
            y_base += getLineSpacing(y_scale);
            break;

        case WordWrapLeftAligned:
            thisCount = drawWrappedText(currLine, tmpDrawArea, z, clip_rect, LeftAligned, colours, x_scale, y_scale);
            tmpDrawArea.d_top += thisCount * getLineSpacing(y_scale);
            break;

        case WordWrapRightAligned:
            thisCount = drawWrappedText(currLine, tmpDrawArea, z, clip_rect, RightAligned, colours, x_scale, y_scale);
            tmpDrawArea.d_top += thisCount * getLineSpacing(y_scale);
            break;

        case WordWrapCentred:
            thisCount = drawWrappedText(currLine, tmpDrawArea, z, clip_rect, Centred, colours, x_scale, y_scale);
            tmpDrawArea.d_top += thisCount * getLineSpacing(y_scale);
            break;

        case WordWrapJustified:
            // no change needed
            thisCount = drawWrappedText(currLine, tmpDrawArea, z, clip_rect, Justified, colours, x_scale, y_scale);
            tmpDrawArea.d_top += thisCount * getLineSpacing(y_scale);
            break;

        default:
            throw InvalidRequestException("Font::drawText - Unknown or unsupported TextFormatting value specified.");
        }

        lineCount += thisCount;

    }

    // should not return 0
    return ceguimax(lineCount, (size_t)1);
}


/*************************************************************************
    Define the set of glyphs available for this font
*************************************************************************/
void Font::defineFontGlyphs(const String& glyph_set)
{
    d_glyphset = glyph_set;
    defineFontGlyphs_impl();

    Logger::getSingleton().logEvent("Font '" + d_name + "' now has the following glyphs defined: '" + d_glyphset + "'.", Informative);
}


/*************************************************************************
    Return the required texture size required to store imagery for the
    glyphs specified in 'glyph_set'
*************************************************************************/
uint Font::getRequiredTextureSize(const String& glyph_set)
{
    d_maxGlyphHeight = 0;

    uint	texSize = 32;			// start with a texture this size
    uint	width;

    uint	cur_x = 0;
    uint	cur_y = d_maxGlyphHeight;

    size_t glyph_set_length = glyph_set.length();

    for (size_t i = 0; i < glyph_set_length; ++i)
    {
        // load-up required glyph
        if (!loadFreetypeGlyph(glyph_set[i]))
            continue;   // skip errors

        // if this glyph is taller than all others so far, update height and re-calculate cur_y
        if (getLoadedFreetypeGlyphHeight() > d_maxGlyphHeight)
        {
            d_maxGlyphHeight = getLoadedFreetypeGlyphHeight();
            cur_y = static_cast<uint>((i + 1) * d_maxGlyphHeight);
        }

        width = getLoadedFreetypeGlyphWidth();
        cur_x += width;

        // check for fit
        if (cur_x >= texSize)
        {
            cur_x = width;
            cur_y += d_maxGlyphHeight;

            if (cur_y >= texSize)
            {
                // texture is too small, set-up to start again...
                texSize *= 2;
                cur_x = 0;
                cur_y = d_maxGlyphHeight;
                i = (size_t)-1;
            }
        }
    }

    return texSize;
}


/*************************************************************************
    Render a set of glyph images into the given memory buffer.
    pixels will be in A8R8G8B8 format
*************************************************************************/
void Font::createFontGlyphSet(const String& glyph_set, uint size, argb_t* buffer)
{
    String imageName;
    FT_GlyphSlot glyph = d_impldat->fontFace->glyph;

    d_max_bearingY = 0;
    uint cur_x = 0, cur_y = 0, width;
    size_t  glyph_set_length = glyph_set.length();

    for (size_t i = 0; i < glyph_set_length; ++i)
    {
        // load-up required glyph
        if (!loadFreetypeGlyph(glyph_set[i]))
            continue;   // skip errors

        width = getLoadedFreetypeGlyphWidth();

        // see if we need to wrap to next row
        if (cur_x + width >= size)
        {
            cur_x = 0;
            cur_y += d_maxGlyphHeight;
        }

        // calculate destination for glyph imagery data
        argb_t* dest_buff = buffer + (cur_y * size) + cur_x;

        // draw glyph into buffer
        drawGlyphToBuffer(dest_buff, size);

        imageName = glyph_set[i];
        GlyphSlotImpl glyphImpl(glyph);

        // define Image on Imageset for this glyph.
        defineGlyphImage(imageName, glyphImpl, cur_x, cur_y, width, (uint)d_maxGlyphHeight);

        cur_x += width;

        // create entry in code-point to Image map
        createGlyphMapping(glyph_set[i], imageName, glyphImpl);
    }
}


/*************************************************************************
    Render a range of glyph images into the given memory buffer.
    pixels will be in A8R8G8B8 format
*************************************************************************/
void Font::createFontGlyphSet(utf32 first_code_point, utf32 last_code_point, uint size, argb_t* buffer)
{
    createFontGlyphSet(getCodepointRangeAsString(first_code_point,
                                                 last_code_point),
                       size,
                       buffer);
}


/*************************************************************************
    Return the required texture size required to store imagery for the
    glyphs specified in by the inclusive range
    [first_code_point, last_code_point]
*************************************************************************/
uint Font::getRequiredTextureSize(utf32 first_code_point, utf32 last_code_point)
{
    return getRequiredTextureSize(
                getCodepointRangeAsString(first_code_point,
                                          last_code_point));
}


/*************************************************************************
    Define the range of glyphs available for this font
*************************************************************************/
void Font::defineFontGlyphs(utf32 first_code_point, utf32 last_code_point)
{
    defineFontGlyphs(getCodepointRangeAsString(first_code_point,
                                               last_code_point));
}


/*************************************************************************
    Copy the FreeType glyph bitmap into the given memory buffer
*************************************************************************/
void Font::drawGlyphToBuffer(argb_t* buffer, uint buf_width)
{
    FT_Bitmap* glyph_bitmap = &d_impldat->fontFace->glyph->bitmap;

    for (int i = 0; i < glyph_bitmap->rows; ++i)
    {
        for (int j = 0; j < glyph_bitmap->width; ++j)
        {
            switch (glyph_bitmap->pixel_mode)
            {
            case FT_PIXEL_MODE_GRAY:
                {
                    uchar* bytebuff = reinterpret_cast<uchar*>(&buffer[j]);
                    *bytebuff++ = 0xFF;
                    *bytebuff++ = 0xFF;
                    *bytebuff++ = 0xFF;
                    *bytebuff = glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j];
                }
                break;

            case FT_PIXEL_MODE_MONO:
                buffer[j] = ((glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j / 8] << (j % 8)) & 0x80) ? 0xFFFFFFFF : 0x00000000;
                break;

            default:
                throw InvalidRequestException("Font::drawGlyphToBuffer - The glyph could not be drawn because the pixel mode is unsupported.");
                break;
            }

        }

        buffer += buf_width;
    }

}


/*************************************************************************
    draws wrapped text
*************************************************************************/
size_t Font::drawWrappedText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale, float y_scale) const
{
    size_t	line_count = 0;
    Rect	dest_area(draw_area);
    float	wrap_width = draw_area.getWidth();

    String  whitespace = TextUtils::DefaultWhitespace;
    String	thisLine, thisWord;
    size_t	currpos = 0;

    // get first word.
    currpos += getNextWord(text, currpos, thisLine);

    // while there are words left in the string...
    while (String::npos != text.find_first_not_of(whitespace, currpos))
    {
        // get next word of the string...
        currpos += getNextWord(text, currpos, thisWord);

        // if the new word would make the string too long
        if ((getTextExtent(thisLine, x_scale) + getTextExtent(thisWord, x_scale)) > wrap_width)
        {
            // output what we had until this new word
            line_count += drawText(thisLine, dest_area, z, clip_rect, fmt, colours, x_scale, y_scale);

            // remove whitespace from next word - it will form start of next line
            thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));

            // reset for a new line.
            thisLine.clear();

            // update y co-ordinate for next line
            dest_area.d_top += getLineSpacing(y_scale);
        }

        // add the next word to the line
        thisLine += thisWord;
    }

    // Last line is left aligned
    TextFormatting last_fmt = (fmt == Justified ? LeftAligned : fmt);
    // output last bit of string
    line_count += drawText(thisLine, dest_area, z, clip_rect, last_fmt, colours, x_scale, y_scale);

    return line_count;
}


/*************************************************************************
    helper function for renderWrappedText to get next word of a string
*************************************************************************/
size_t Font::getNextWord(const String& in_string, size_t start_idx, String& out_string) const
{
    out_string = TextUtils::getNextWord(in_string, start_idx, TextUtils::DefaultWrapDelimiters);

    return out_string.length();
}


/*************************************************************************
    Draw a line of text.  No formatting is applied.
*************************************************************************/
void Font::drawTextLine(const String& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours, float x_scale, float y_scale) const
{
    Vector3	cur_pos(position);

    const GlyphDat* glyph;
    float base_y = position.d_y;

    for (size_t c = 0; c < text.length(); ++c)
    {
        glyph = getGlyphData(text[c]);

        if (glyph)
        {
            const Image* img = glyph->getImage();
            cur_pos.d_y = base_y - (img->getOffsetY() - img->getOffsetY() * y_scale);
            img->draw(cur_pos, glyph->getSize(x_scale, y_scale), clip_rect, colours);
            cur_pos.d_x += glyph->getAdvance(x_scale);
        }

    }

}


/*************************************************************************
    Draw a justified line of text.
*************************************************************************/
void Font::drawTextLineJustified(const String& text, const Rect& draw_area, const Vector3& position, const Rect& clip_rect, const ColourRect& colours, float x_scale, float y_scale) const
{
    Vector3	cur_pos(position);

    const GlyphDat* glyph;
    float base_y = position.d_y;
    size_t char_count = text.length();

    // Calculate the length difference between the justified text and the same text, left aligned
    // This space has to be shared between the space characters of the line
    float lost_space = getFormattedTextExtent(text, draw_area, Justified, x_scale) - getTextExtent(text, x_scale);

    // The number of spaces and tabs in the current line
    uint space_count = 0;
    size_t c;
    for (c = 0; c < char_count; ++c)
        if ((text[c] == ' ') || (text[c] == '\t'))
            ++space_count;

    // The width that must be added to each space character in order to transform the left aligned text in justified text
    float shared_lost_space = 0.0;
    if (space_count > 0)
        shared_lost_space = lost_space / (float)space_count;

    for (c = 0; c < char_count; ++c)
    {
        glyph = getGlyphData(text[c]);

        if (glyph)
        {
            const Image* img = glyph->getImage();
            cur_pos.d_y = base_y - (img->getOffsetY() - img->getOffsetY() * y_scale);
            img->draw(cur_pos, glyph->getSize(x_scale, y_scale), clip_rect, colours);
            cur_pos.d_x += glyph->getAdvance(x_scale);

            // That's where we adjust the size of each space character
            if ((text[c] == ' ') || (text[c] == '\t'))
                cur_pos.d_x += shared_lost_space;
        }

    }

}


/*************************************************************************
    Function to do real work of constructor
*************************************************************************/
void Font::constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size, uint flags, const String& glyph_set)
{
    ImagesetManager& ismgr	= ImagesetManager::getSingleton();

    // pull a-a setting from flags
    d_antiAliased = (flags == NoAntiAlias) ? false : true;

    // create an blank Imageset
    d_glyph_images = ismgr.createImageset(name + "_auto_glyph_images", System::getSingleton().getRenderer()->createTexture());

    uint		horzdpi		= System::getSingleton().getRenderer()->getHorzScreenDPI();
    uint		vertdpi		= System::getSingleton().getRenderer()->getVertScreenDPI();
    String		errMsg;

    System::getSingleton().getResourceProvider()->loadRawDataContainer(fontname,
        d_impldat->fontData, resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);

    // create face using input font
    if (FT_New_Memory_Face(d_impldat->library, d_impldat->fontData.getDataPtr(),
                            (FT_Long)d_impldat->fontData.getSize(), 0, &d_impldat->fontFace) == 0)
    {
        // check that default Unicode character map is available
        if (d_impldat->fontFace->charmap)
        {
            try
            {
                d_glyphset = glyph_set;
                d_name = name;
                d_freetype = true;
                createFontFromFT_Face(size, horzdpi, vertdpi);
                return;
            }
            catch(...)
            {
                ismgr.destroyImageset(d_glyph_images);
                d_glyph_images = 0;

                FT_Done_Face(d_impldat->fontFace);
                d_freetype = false;

                // re-throw
                throw;
            }

        }
        // missing Unicode character map
        else
        {
            FT_Done_Face(d_impldat->fontFace);
            d_freetype = false;

            errMsg = "Font::constructor_impl - The source font '" + fontname +"' does not have a Unicode charmap, and cannot be used.";
        }

    }
    // failed to create face (a problem with the font file?)
    else
    {
        errMsg = "Font::constructor_impl - An error occurred while trying to create a FreeType face from source font '" + fontname + "'.";
    }

    // cleanup image set we created here
    ismgr.destroyImageset(d_glyph_images);

    throw GenericException(errMsg);
}


/*************************************************************************
    Load and complete construction of 'this' via an XML file
*************************************************************************/
void Font::load(const String& filename, const String& resourceGroup)
{
    // unload old data
    unload();

    if (filename.empty())
    {
        throw InvalidRequestException("Font::load - Filename supplied for Font loading must be valid");
    }

    // create handler object
    Font_xmlHandler handler(this);

    // do parse (which uses handler to create actual data)
    try
    {
        System::getSingleton().getXMLParser()->parseXMLFile(handler,
            filename, FontSchemaName, resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);
    }
    catch(...)
    {
        unload();

        Logger::getSingleton().logEvent("Font::load - loading of Font from file '" + filename +"' failed.", Errors);
        throw;
    }

}


/*************************************************************************
    Unload data associated with the font (font is then useless.
    this is intended for cleanup).
*************************************************************************/
void Font::unload(void)
{
    d_cp_map.clear();

    // cleanup Imageset if it's valid
    if (d_glyph_images)
    {
        ImagesetManager::getSingleton().destroyImageset(d_glyph_images);
        d_glyph_images = 0;
    }

    // cleanup FreeType face if this is a FreeType based font.
    if (d_freetype)
    {
        FT_Done_Face(d_impldat->fontFace);
        d_freetype = false;
    }

    System::getSingleton().getResourceProvider()->unloadRawDataContainer(d_impldat->fontData);
}


/*************************************************************************
    Defines the set of code points on the font. (implementation).
*************************************************************************/
void Font::defineFontGlyphs_impl(void)
{
    // must be a font using the FreeType system
    if (!d_freetype)
        throw InvalidRequestException("Font::defineFontGlyphs_impl - operation not supported on bitmap based fonts.");

    uint texture_size = getRequiredTextureSize(d_glyphset);

    // check renderer can do a texture big enough
    if (texture_size > System::getSingleton().getRenderer()->getMaxTextureSize())
        throw	RendererException("Font::defineFontGlyphs_impl - operation requires a texture larger than the supported maximum.");

    // allocate memory buffer where we will define the imagery
    argb_t* mem_buffer;

    try
    {
        mem_buffer = new argb_t[texture_size * texture_size];
    }
    catch (std::bad_alloc)
    {
        throw	MemoryException("Font::defineFontGlyphs_impl - failed to allocate required memory buffer.");
    }

    // initialise background to transparent black.
    memset(mem_buffer, 0, ((texture_size * texture_size) * sizeof(argb_t)));

    // clear old data about glyphs and images
    d_cp_map.clear();
    d_glyph_images->undefineAllImages();

    // render new glyphs and define Imageset images.
    createFontGlyphSet(d_glyphset, texture_size, mem_buffer);

    // update Imageset texture with new imagery.
    d_glyph_images->getTexture()->loadFromMemory(mem_buffer, texture_size, texture_size);

    delete[] mem_buffer;

    d_lineHeight = (float)d_maxGlyphHeight;

    // calculate spacing and base-line
    d_max_bearingY = ((float)d_impldat->fontFace->ascender / (float)d_impldat->fontFace->units_per_EM) * (float)d_impldat->fontFace->size->metrics.y_ppem;
    d_lineSpacing = ((float)d_impldat->fontFace->height / (float)d_impldat->fontFace->units_per_EM) * (float)d_impldat->fontFace->size->metrics.y_ppem;
}


/*************************************************************************
    Calculate the vertical spacing fields for a static / bitmap font
*************************************************************************/
void Font::calculateStaticVertSpacing(void)
{
    if (!d_freetype)
    {
        float scale = d_autoScale ? d_vertScaling : 1.0f;

        d_lineHeight	= 0;
        d_max_bearingY	= 0;

        CodepointMap::iterator pos = d_cp_map.begin(), end = d_cp_map.end();

        for (;pos != end; ++pos)
        {
            const Image* img = pos->second.getImage();

            if (-img->getOffsetY() > d_max_bearingY)
                d_max_bearingY = -img->getOffsetY();

            if (img->getHeight() > d_lineHeight)
                d_lineHeight = img->getHeight();
        }

        d_max_bearingY *= scale;
        d_lineHeight *= scale;
        d_lineSpacing = d_lineHeight;
        d_maxGlyphHeight = static_cast<uint>(d_lineHeight);
    }

}


/*************************************************************************
    Set the native resolution for this Font
*************************************************************************/
void Font::setNativeResolution(const Size& size)
{
    d_nativeHorzRes = size.d_width;
    d_nativeVertRes = size.d_height;

    // set native resolution for underlying imageset for bitmap fonts
    if ((!d_freetype) && (d_glyph_images != 0))
    {
        d_glyph_images->setNativeResolution(size);
    }

    // re-calculate scaling factors & notify images as required
    notifyScreenResolution(System::getSingleton().getRenderer()->getSize());
}


/*************************************************************************
    Notify the Font of the current (usually new) display resolution.
*************************************************************************/
void Font::notifyScreenResolution(const Size& size)
{
    // notification should come from System which would have notified the Imageset anyway, but
    // in case client code decides to call us, we must pass on notification here too.
    if (d_glyph_images)
    {
        d_glyph_images->notifyScreenResolution(size);
    }

    d_horzScaling = size.d_width / d_nativeHorzRes;
    d_vertScaling = size.d_height / d_nativeVertRes;

    if (d_autoScale)
    {
        updateFontScaling();
    }

}


/*************************************************************************
    Enable or disable auto-scaling for this Font.
*************************************************************************/
void Font::setAutoScalingEnabled(bool setting)
{
    if (setting != d_autoScale)
    {
        if ((!d_freetype) && (d_glyph_images != 0))
        {
            d_glyph_images->setAutoScalingEnabled(setting);
        }

        d_autoScale = setting;
        updateFontScaling();
    }

}


/*************************************************************************
    Update the font as required according to the current scaling
*************************************************************************/
void Font::updateFontScaling(void)
{
    if (d_freetype)
    {
        uint hdpi = System::getSingleton().getRenderer()->getHorzScreenDPI();
        uint vdpi = System::getSingleton().getRenderer()->getVertScreenDPI();

        createFontFromFT_Face(d_ptSize, hdpi, vdpi);
    }
    // bitmapped font
    else
    {
        float hscale = d_autoScale ? d_horzScaling : 1.0f;

        // perform update on font mapping advance values
        CodepointMap::iterator pos = d_cp_map.begin(), end = d_cp_map.end();
        for (; pos != end; ++pos)
        {
            pos->second.setAdvance(PixelAligned(pos->second.getUnscaledAdvance() * hscale));
        }

        // re-calculate height
        calculateStaticVertSpacing();
    }

}


/*************************************************************************
    Set the size of the free-type font (via d_impldat->fontFace which should already
    be setup) and render the glyphs in d_glyphset.
*************************************************************************/
void Font::createFontFromFT_Face(uint size, uint horzDpi, uint vertDpi)
{
    if (d_autoScale)
    {
        horzDpi = (uint)(((float)horzDpi) * d_horzScaling);
        vertDpi = (uint)(((float)vertDpi) * d_vertScaling);
    }

    d_ptSize = size;

    if (FT_Set_Char_Size(d_impldat->fontFace, 0, d_ptSize * 64, horzDpi, vertDpi) == 0)
    {
        defineFontGlyphs_impl();
    }
    // failed to set size for font
    else
    {
        throw GenericException("Font::createFontFromFT_Face - An error occurred while creating a source font with the requested size.");
    }

}


/*************************************************************************
    Return the number of lines the given text would be formatted to.
*************************************************************************/
size_t Font::getFormattedLineCount(const String& text, const Rect& format_area, TextFormatting fmt, float x_scale) const
{
    // handle simple non-wrapped cases.
    if ((fmt == LeftAligned) || (fmt == Centred) || (fmt == RightAligned) || (fmt == Justified))
    {
        return std::count(text.begin(), text.end(), static_cast<utf8>('\n')) + 1;
    }

    // handle wraping cases
    size_t lineStart = 0, lineEnd = 0;
    String	sourceLine;

    float	wrap_width = format_area.getWidth();
    String  whitespace = TextUtils::DefaultWhitespace;
    String	thisLine, thisWord;
    size_t	line_count = 0, currpos = 0;

    while (lineEnd < text.length())
    {
        if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
        {
            lineEnd = text.length();
        }

        sourceLine = text.substr(lineStart, lineEnd - lineStart);
        lineStart = lineEnd + 1;

        // get first word.
        currpos = getNextWord(sourceLine, 0, thisLine);

        // while there are words left in the string...
        while (String::npos != sourceLine.find_first_not_of(whitespace, currpos))
        {
            // get next word of the string...
            currpos += getNextWord(sourceLine, currpos, thisWord);

            // if the new word would make the string too long
            if ((getTextExtent(thisLine, x_scale) + getTextExtent(thisWord, x_scale)) > wrap_width)
            {
                // too long, so that's another line of text
                line_count++;

                // remove whitespace from next word - it will form start of next line
                thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));

                // reset for a new line.
                thisLine.clear();
            }

            // add the next word to the line
            thisLine += thisWord;
        }

        // plus one for final line
        line_count++;
    }

    return line_count;
}


/*************************************************************************
    Return whether this font is anti-aliased or not.
*************************************************************************/
bool Font::isAntiAliased(void) const
{
    return d_freetype ? d_antiAliased : false;
}


/*************************************************************************
    Set whether the font is anti-aliased or not.
*************************************************************************/
void Font::setAntiAliased(bool setting)
{
    if (d_freetype && (d_antiAliased != setting))
    {
        d_antiAliased = setting;

        // regenerate font
        createFontFromFT_Face(d_ptSize, System::getSingleton().getRenderer()->getHorzScreenDPI(), System::getSingleton().getRenderer()->getVertScreenDPI());
    }

}


/*************************************************************************
    Return the horizontal pixel extent given text would be formatted to.
*************************************************************************/
float Font::getFormattedTextExtent(const String& text, const Rect& format_area, TextFormatting fmt, float x_scale) const
{
    float lineWidth;
    float widest = 0;

    size_t lineStart = 0, lineEnd = 0;
    String	currLine;

    while (lineEnd < text.length())
    {
        if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
        {
            lineEnd = text.length();
        }

        currLine = text.substr(lineStart, lineEnd - lineStart);
        lineStart = lineEnd + 1;	// +1 to skip \n char

        switch(fmt)
        {
        case Centred:
        case RightAligned:
        case LeftAligned:
            lineWidth = getTextExtent(currLine, x_scale);
            break;

        case Justified:
            // usually we use the width of the rect but we have to ensure the current line is not wider than that
            lineWidth = ceguimax(format_area.getWidth(), getTextExtent(currLine, x_scale));
            break;

        case WordWrapLeftAligned:
        case WordWrapRightAligned:
        case WordWrapCentred:
            lineWidth = getWrappedTextExtent(currLine, format_area.getWidth(), x_scale);
            break;

        case WordWrapJustified:
            // same as above
            lineWidth = ceguimax(format_area.getWidth(), getWrappedTextExtent(currLine, format_area.getWidth(), x_scale));
            break;

        default:
            throw InvalidRequestException("Font::getFormattedTextExtent - Unknown or unsupported TextFormatting value specified.");
        }

        if (lineWidth > widest)
        {
            widest = lineWidth;
        }

    }

    return widest;
}


/*************************************************************************
    returns extent of widest line of wrapped text.
*************************************************************************/
float Font::getWrappedTextExtent(const String& text, float wrapWidth, float x_scale) const
{
    String  whitespace = TextUtils::DefaultWhitespace;
    String	thisWord;
    size_t	currpos;
    float	lineWidth, wordWidth;
    float	widest = 0;

    // get first word.
    currpos = getNextWord(text, 0, thisWord);
    lineWidth = getTextExtent(thisWord, x_scale);

    // while there are words left in the string...
    while (String::npos != text.find_first_not_of(whitespace, currpos))
    {
        // get next word of the string...
        currpos += getNextWord(text, currpos, thisWord);
        wordWidth = getTextExtent(thisWord, x_scale);

        // if the new word would make the string too long
        if ((lineWidth + wordWidth) > wrapWidth)
        {

            if (lineWidth > widest)
            {
                widest = lineWidth;
            }

            // remove whitespace from next word - it will form start of next line
            thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));
            wordWidth = getTextExtent(thisWord, x_scale);

            // reset for a new line.
            lineWidth = 0;
        }

        // add the next word to the line
        lineWidth += wordWidth;
    }

    if (lineWidth > widest)
    {
        widest = lineWidth;
    }

    return widest;
}


/*************************************************************************
    Return a String object that contains the code-points that the font
    is currently configured to render.
*************************************************************************/
const String& Font::getAvailableGlyphs(void) const
{
    return d_glyphset;
}


/*************************************************************************
    Return the point size of a dynamic (ttf based) font.
*************************************************************************/
uint Font::getPointSize(void) const
{
    if (d_freetype)
    {
        return d_ptSize;
    }
    else
    {
        throw InvalidRequestException("Font::getPointSize - unable to return point size for a static (bitmap based) font.");
    }
}


/*************************************************************************
    Writes an xml representation of this Font to \a out_stream.
*************************************************************************/
void Font::writeXMLToStream(OutStream& out_stream, uint indentLevel) const
{
    String indent(indentLevel, '\t');
    String subindent(indentLevel + 1, '\t');

    // output starting <Font ... > element
    out_stream << indent << "<Font Name=\"" << d_name << "\" Filename=\"" << d_sourceFilename << "\" ";

    if (d_freetype)
        out_stream << "Size=\"" << d_ptSize << "\" ";

    if (d_nativeHorzRes != DefaultNativeHorzRes)
        out_stream << "NativeHorzRes=\"" << static_cast<uint>(d_nativeHorzRes) << "\" ";

    if (d_nativeVertRes != DefaultNativeVertRes)
        out_stream << "NativeVertRes=\"" << static_cast<uint>(d_nativeVertRes) << "\" ";

    if (d_autoScale)
        out_stream << "AutoScaled=\"True\" ";

    out_stream << ">" << std::endl;

    // dynamic font so output defined glyphs
    if (d_freetype)
    {
        size_t start = 0, idx = 0;

        while(start < d_glyphset.length())
        {
            // find end of range
            while ((idx + 1 < d_glyphset.length()) && (d_glyphset[idx] + 1 == d_glyphset[idx + 1]))
                ++idx;

            if (start == idx)
                // if range is a just a single codepoint
                out_stream << subindent << "<Glyph Codepoint=\"" << d_glyphset[start] << "\" />" << std::endl;
            else
                // range contains >1 codepoint
                out_stream << subindent << "<GlyphRange StartCodepoint=\"" << d_glyphset[start] << "\" EndCodepoint=\"" << d_glyphset[idx] << "\" />" << std::endl;

            start = ++idx;
        }
    }
    // static font, so output glyph to imageset mappings
    else
    {
        for (CodepointMap::const_iterator iter = d_cp_map.begin(); iter != d_cp_map.end(); ++iter)
        {
            out_stream << subindent << "<Mapping Codepoint=\"" << (*iter).first << "\" Image=\"" << (*iter).second.getImage()->getName() << "\" ";

            if ((*iter).second.getUnscaledAdvance() != -1)
                out_stream << "HorzAdvance=\"" << (*iter).second.getUnscaledAdvance() << "\" ";

            out_stream << "/>" << std::endl;
        }
    }

    // output closing </Font> element.
    out_stream << indent << "</Font>" << std::endl;
}


/*************************************************************************
    Return a String containing a specified range of codepoint values
*************************************************************************/
String Font::getCodepointRangeAsString(utf32 first_code_point, utf32 last_code_point) const
{
    String tmp;

    for (utf32 cp = first_code_point; cp <= last_code_point; ++cp)
        tmp += cp;

    return tmp;
}

/*************************************************************************
    Load the specified glyph into the internal FreeType state
*************************************************************************/
bool Font::loadFreetypeGlyph(utf32 codepoint)
{
    // load-up required glyph
    bool load_error = 0 != FT_Load_Char(d_impldat->fontFace, codepoint, FT_LOAD_RENDER|FT_LOAD_FORCE_AUTOHINT|
            (d_antiAliased ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO));

    if (load_error)
    {
        std::stringstream err;
        err << "Font::loadFreetypeGlyph - Failed to load glyph for codepoint: ";
        err << static_cast<unsigned int>(codepoint);
        Logger::getSingleton().logEvent(err.str(), Errors);
    }

    return !load_error;
}

/*************************************************************************
    Return Width of currently loaded freetype glyph
*************************************************************************/
uint Font::getLoadedFreetypeGlyphWidth() const
{
    return d_impldat->fontFace->glyph->bitmap.width + InterGlyphPadSpace;
}

/*************************************************************************
    Return Height of currently loaded freetype glyph
*************************************************************************/
uint Font::getLoadedFreetypeGlyphHeight() const
{
    return static_cast<uint>(d_impldat->fontFace->glyph->bitmap.rows) +
                             InterGlyphPadSpace;
}

/*************************************************************************
    Return GlyphDat struct for a codepoint.
*************************************************************************/
const GlyphDat* Font::getGlyphData(utf32 codepoint) const
{
    CodepointMap::const_iterator pos = d_cp_map.find(codepoint);

    if (pos != d_cp_map.end())
        return &pos->second;
    else
        return 0;
}

/*************************************************************************
    define an image, on the associated Imageset, for a glyph.
*************************************************************************/
void Font::defineGlyphImage(const String& name, const GlyphSlotImpl& glyph, uint x, uint y, uint width, uint height)
{
    assert(d_glyph_images != 0);

    // define area
    Rect area(static_cast<float>(x),
              static_cast<float>(y),
              static_cast<float>(x + width - InterGlyphPadSpace),
              static_cast<float>(y + height - InterGlyphPadSpace));

    // define rendering offset
    Point offset(static_cast<float>(glyph.glyphSlot->metrics.horiBearingX >> 6),
                 -static_cast<float>(glyph.glyphSlot->metrics.horiBearingY >> 6));

    // add an entry in the Imageset.
    d_glyph_images->defineImage(name, area, offset);
}

/*************************************************************************
    define an entry in the codepoint map for the given codepoint & Image.
*************************************************************************/
void Font::createGlyphMapping(utf32 codepoint, const String& imageName, const GlyphSlotImpl& glyph)
{
    assert(d_glyph_images != 0);

    // create mapping entry
    GlyphDat dat(&d_glyph_images->getImage(imageName),
                 glyph.glyphSlot->advance.x >> 6);

    // add entry in the codepoint map;
    d_cp_map[codepoint] = dat;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
    GlyphDat member definitions
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
GlyphDat::GlyphDat() :
    d_image(0),
    d_horz_advance(0),
    d_horz_advance_unscaled(0)
{}

GlyphDat::GlyphDat(const GlyphDat& other) :
    d_image(other.d_image),
    d_horz_advance(other.d_horz_advance),
    d_horz_advance_unscaled(other.d_horz_advance_unscaled)
{}

GlyphDat::GlyphDat(const Image* image, int horzAdvance, int unscaledAdvace) :
    d_image(image),
    d_horz_advance(horzAdvance),
    d_horz_advance_unscaled(unscaledAdvace)
{}

const GlyphDat& GlyphDat::operator=(const GlyphDat& other)
{
    d_image = other.d_image;
    d_horz_advance = other.d_horz_advance;
    d_horz_advance_unscaled = other.d_horz_advance_unscaled;

    return *this;
}

const Image* GlyphDat::getImage() const
{
    return d_image;
}

Size GlyphDat::getSize(float x_scale, float y_scale) const
{
    return Size(getWidth(x_scale), getHeight(y_scale));
}

float GlyphDat::getWidth(float x_scale) const
{
    return d_image->getWidth() * x_scale;
}

float GlyphDat::getHeight(float y_scale) const
{
    return d_image->getHeight() * y_scale;
}

float GlyphDat::getRenderedAdvance(float x_scale) const
{
    return (d_image->getWidth() + d_image->getOffsetX()) * x_scale;
}

float GlyphDat::getAdvance(float x_scale) const
{
    return static_cast<float>(d_horz_advance) * x_scale;
}

float GlyphDat::getUnscaledAdvance() const
{
    return static_cast<float>(d_horz_advance_unscaled);
}

void GlyphDat::setAdvance(float advance)
{
    d_horz_advance = static_cast<int>(advance);
}

} // End of  CEGUI namespace section
