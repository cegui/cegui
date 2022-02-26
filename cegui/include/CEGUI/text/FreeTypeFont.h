/***********************************************************************
	created:	3/6/2006
	author:		Andrew Zabolotny

	purpose:    Implementation of the Font class via the FreeType library
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
#ifndef _CEGUIFreeTypeFont_h_
#define _CEGUIFreeTypeFont_h_

#include "CEGUI/text/Font.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/text/FreeTypeFontGlyph.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251) // STL classes in API
#endif

namespace CEGUI
{
class BitmapImage;

/*!
\brief
    Implementation of the Font class interface using the FreeType library.

    This implementation tries to provide maximal support for any kind of
    fonts supported by FreeType. It has been tested on outline font formats
    like TTF and PS as well as on bitmap font formats like PCF and FON.

    Glyphs are rendered dynamically on demand, so a large font with lots
    of glyphs won't slow application startup time.
*/
class CEGUIEXPORT FreeTypeFont : public Font
{
public:

    /*!
    \brief
        Constructor for FreeTypeFont based fonts.
    
    \param font_name
        The name that the font will use within the CEGUI system.
    
    \param size
        Specifies the point size that the font is to be rendered at.
    
    \param sizeUnit
        Specifies the size unit of the Font size.
    
    \param anti_aliased
        Specifies whether the font should be rendered using anti aliasing.
    
    \param font_filename
        The filename of an font file that will be used as the source for
        glyph images for this font.
    
    \param resource_group
        The resource group identifier to use when loading the font file
        specified by \a font_filename.
    
    \param auto_scaled
        Specifies whether the font imagery should be automatically scaled to
        maintain the same physical size (which is calculated by using the
        native resolution setting).
    
    \param native_res
        The native resolution value.  This is only significant when
        auto scaling is enabled.
    
    \param specific_line_spacing
        If specified (non-zero), this will be the line spacing that we will
        report for this font, regardless of what is mentioned in the font file
        itself.
    */
    FreeTypeFont(const String& font_name, const float size,
                 const FontSizeUnit sizeUnit,
                 const bool anti_aliased, const String& font_filename,
                 const String& resource_group = "",
                 const AutoScaledMode auto_scaled = AutoScaledMode::Disabled,
                 const Sizef& native_res = Sizef(640.0f, 480.0f),
                 const float specific_line_spacing = 0.0f);

    virtual ~FreeTypeFont() override;
    
    void updateFont() override;
    float getKerning(const FontGlyph* prev, const FontGlyph& curr) const override;
    FreeTypeFontGlyph* loadGlyph(uint32_t index) override;
    const FreeTypeFontGlyph* getGlyph(uint32_t index) const override;
    uint32_t getGlyphIndexByFreetypeIndex(FT_UInt ftGlyphIndex) const;
    Image* getOutline(uint32_t index, float thickness = 1.f) override;

    //! \brief Sets the Font size of this font.
    void setSize(float size) { setSizeAndUnit(size, d_sizeUnit); }
    //! \brief Returns the Font size of this font.
    float getSize() const { return d_size; }
    //! \brief Sets the Font size unit of this font.
    void setSizeUnit(FontSizeUnit sizeUnit) { setSizeAndUnit(d_size, sizeUnit); }
    //! \brief Returns the Font size unit of this font.
    FontSizeUnit getSizeUnit() const { return d_sizeUnit; }

    /*!
    \brief
        Sets the Font size in pixels. This internally
        sets both the Font size and the Font unit accordingly.
    \param pixelSize
        The font size in pixels.
    */
    void setSizeInPixels(float pixelSize) { setSizeAndUnit(pixelSize, FontSizeUnit::Pixels); }

    /*!
    \brief
        Sets the Font size in points. This internally
        sets both the Font size and the Font unit accordingly.
    \param pointSize
        The font size in points.
    */
    void setSizeInPoints(float pointSize) { setSizeAndUnit(pointSize, FontSizeUnit::Points); }

    /*!
    \brief
        Sets the Font size in and Font size unit.
    \param size
        The font size in the specified unit.
    \param sizeUnit
        The font size unit.
    */
    void setSizeAndUnit(float size, const FontSizeUnit sizeUnit);

    //! \brief Calculates and returns the Font size of this font in pixels.
    float getSizeInPixels() const;
    //! \brief Calculates and returns the Font size of this font in points.
    float getSizeInPoints() const;

    //! \brief Sets whether this font should be rendered anti-aliased or not.
    void setAntiAliased(bool anti_alaised);
    //! Returns whether the Freetype font is rendered anti-aliased or not.
    bool isAntiAliased() const { return d_antiAliased; }

    //! Returns the Freetype font face
    const FT_Face& getFontFace() const { return d_fontFace; }
    //! Returns the FreeType load flags
    FT_Int32 getGlyphLoadFlags() const;

    //! Returns the initial size to be used for any new glyph atlas texture.
    int getInitialGlyphAtlasSize() const { return d_initialGlyphAtlasSize; }
    //! Sets the initial size to be used for any new glyph atlas texture.
    void setInitialGlyphAtlasSize(int val) { d_initialGlyphAtlasSize = val; }

protected:

    /*!
        A data structure containing info about one horizontal line inside
        a glyph atlas. The data is used when deciding where to place a new glyph.
    */
    struct TextureGlyphLine
    {
        TextureGlyphLine() = default;

        TextureGlyphLine(uint32_t lastPosX, uint32_t lastPosY, uint32_t maximumExtentY)
            : d_lastXPos(lastPosX)
            , d_lastYPos(lastPosY)
            , d_maximumExtentY(maximumExtentY)
        {
        }

        //! The x position where we last ended drawing (including padding)
        mutable uint32_t d_lastXPos = 0;
        //! The y position where we last ended drawing (including padding)
        mutable uint32_t d_lastYPos = 0;
        //! The maximum extent that a glyph of this line can take in the y-dimension
        //! If the line is the last line, this value may increase while rendering to the line
        mutable uint32_t d_maximumExtentY = 0;
    };

    /*!
    \brief
        Updates a part of the buffer data, which equates to a sub-image inside the 
        texture the buffer data represents, based on the supplied subimage data.

    \param bitmapWidth
        Width of the source subimage

    \param bitmapHeight
        Height of the source subimage

    \param subImageData
        The pixel data of the subimage 
    */
    void updateTextureBufferSubImage(argb_t* buffer, uint32_t bitmapWidth,
        uint32_t bitmapHeight, const std::vector<argb_t>& subImageData) const;

    /*
    \brief
        Changes the size of the texture and updates the data in it based on the old
        texture's data. The size must be larger than the original one.
     */
    void resizeAndUpdateTexture(Texture* texture, uint32_t newSize);
    void createTextureSpaceForGlyphRasterisation(Texture* texture, uint32_t glyphWidth, uint32_t glyphHeight);
   //! Register all properties of this class.
    void addFreeTypeFontProperties();
    //! Free all allocated font data.
    void free();

    void findAndThrowFreeTypeError(FT_Error error, const CEGUI::String& errorMessageIntro) const;

    void tryToCreateFontWithClosestFontHeight(FT_Error errorResult, int requestedFontPixelHeight) const;
    Image* renderOutline(uint32_t index, FT_Fixed thickness);

    void handleFontSizeOrFontUnitChange();

    //! Rasterises the glyph and adds it into a glyph atlas texture
    BitmapImage* rasterise(const String& name, const FT_Bitmap& ft_bitmap,
        int32_t glyphLeft, int32_t glyphTop, uint32_t glyphWidth, uint32_t glyphHeight);

    size_t findTextureLineWithFittingSpot(uint32_t glyphWidth, uint32_t glyphHeight) const;
    size_t addNewLineIfFitting(uint32_t glyphHeight, uint32_t glyphWidth);

    void createGlyphAtlasTexture();
    static std::vector<argb_t> createGlyphTextureData(const FT_Bitmap& glyph_bitmap);

    void writeXMLToStream_impl(XMLSerializer& xml_stream) const override;

    //! If non-zero, the overridden line spacing that we're to report.
    float d_specificLineSpacing;
    //! Specified font size for this font.
    float d_size;
    //! The specified size unit of the Font size.
    FontSizeUnit d_sizeUnit;
    //! True if the font should be rendered as anti-aliased by freeType.
    bool d_antiAliased;
    //! FreeType-specific font handle
    FT_Face d_fontFace = nullptr;
    FT_Stroker d_stroker = nullptr;
    //! Font file in memory, must be accessible during the font face lifetime
    RawDataContainer d_fontData;

    std::vector<FreeTypeFontGlyph> d_glyphs;
    std::vector<bool> d_glyphLoadStatus;

    //! Contains mappings from freetype indices to Font glyphs
    std::unordered_map<FT_UInt, uint32_t> d_indexToGlyphMap;

    std::map<FT_Fixed, std::vector<std::unique_ptr<BitmapImage>>> d_outlines;

    //! The size with which new texture atlases for glyphs are going to be initialised
    uint32_t d_initialGlyphAtlasSize = 32;

    //! The size of the last texture that has been created
    uint32_t d_lastTextureSize = 0;
    //! Textures that hold the glyph imagery for this font.
    std::vector<Texture*> d_glyphTextures;
    //! Memory buffer for rendering the glyphs, this contains the data of the latest texture
    std::vector<argb_t> d_lastTextureBuffer;
    //! Contains information about the extents of each line of glyphs of the latest texture
    std::vector<TextureGlyphLine> d_textureGlyphLines;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
