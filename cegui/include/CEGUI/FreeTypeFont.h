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

#include "CEGUI/Font.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/BitmapImage.h"
#include "CEGUI/FontSizeUnit.h"
#include "CEGUI/FreeTypeFontGlyph.h"

#include <ft2build.h>
#include FT_FREETYPE_H


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Implementation of the Font class interface using the FreeType library.

    This implementation tries to provide maximal support for any kind of
    fonts supported by FreeType. It has been tested on outline font formats
    like TTF and PS as well as on bitmap font formats like PCF and FON.

    Glyphs are rendered dynamically on demand, so a large font with lots
    of glyphs won't slow application startup time.
*/
class FreeTypeFont : public Font
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
    
        \param native_horz_res
            The horizontal native resolution value.  This is only significant when
            auto scaling is enabled.
    
        \param native_vert_res
            The vertical native resolution value.  This is only significant when
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

    //! Destructor.
    virtual ~FreeTypeFont();
    
    void updateFont() override;
    bool isCodepointAvailable(char32_t codePoint) const override;
    FreeTypeFontGlyph* getGlyphForCodepoint(const char32_t codePoint) const override;

    /*!
    \brief
        Sets the Font size of this font.
    \param size
        The font size.
    */
    void setSize(const float size);

    /*!
    \brief
        Returns the Font size of this font.
    \return
        The font size.
    */
    float getSize() const;

    /*!
    \brief
        Sets the Font size unit of this font.
    \param size
        The font size unit.
    */
    void setSizeUnit(FontSizeUnit sizeUnit);

    /*!
    \brief
        Returns the Font size unit of this font.
    \return
        The font size unit.
    */
    FontSizeUnit getSizeUnit() const;

    /*!
    \brief
        Sets the Font size in pixels. This internally
        sets both the Font size and the Font unit accordingly.
    \param pixelSize
        The font size in pixels.
    */
    void setSizeInPixels(const float pixelSize);

    /*!
    \brief
        Sets the Font size in points. This internally
        sets both the Font size and the Font unit accordingly.
    \param pointSize
        The font size in points.
    */
    void setSizeInPoints(const float pointSize);

    /*!
    \brief
        Sets the Font size in and Font size unit.
    \param size
        The font size in the specified unit.
    \param sizeUnit
        The font size unit.
    */
    void setSizeAndUnit(const float size, const FontSizeUnit sizeUnit);

    /*!
    \brief
        Calculates and returns the Font size of this font in pixels.
    \return
        The font size in pixels.
    */
    float getSizeInPixels() const;

    /*!
    \brief
        Calculates and returns the Font size of this font in points.
    \return
        The font size in points.
    */
    float getSizeInPoints() const;

    /*!
    \brief
        Sets whether the Freetype font should be rendered anti-aliased
        or not.
    */
    void setAntiAliased(const bool anti_alaised);

    //! Returns whether the Freetype font is rendered anti-aliased or not.
    bool isAntiAliased() const;

    //! Returns the Freetype font face
    const FT_Face& getFontFace() const;

    //! Returns the initial size to be used for any new glyph atlas texture.
    int getInitialGlyphAtlasSize() const;

    //! Sets the initial size to be used for any new glyph atlas texture.
    void setInitialGlyphAtlasSize(int val);

protected:
    /*!
        A data structure containing info about one horizontal line inside
        a glyph atlas. The data is used when deciding where to place a new glyph.
    */
    struct TextureGlyphLine
    {
        TextureGlyphLine()
        {
        }

        TextureGlyphLine(int lastPosX, int lastPosY, int maximumExtentY)
            : d_lastXPos(lastPosX)
            , d_lastYPos(lastPosY)
            , d_maximumExtentY(maximumExtentY)
        {
        }

        //! The x position where we last ended drawing (including padding)
        mutable int d_lastXPos = 0;
        //! The y position where we last ended drawing (including padding)
        mutable int d_lastYPos = 0;
        //! The maximum extent that a glyph of this line can take in the y-dimension
        //! If the line is the last line, this value may increase while rendering to the line
        mutable int d_maximumExtentY = 0;
    };

    //! Type for mapping codepoints to the corresponding Freetype Font glyphs
    typedef std::unordered_map<char32_t, FreeTypeFontGlyph*> CodePointToGlyphMap;
    //! Type for mapping Freetype indices to the corresponding Freetype Font glyphs
    typedef std::unordered_map<FT_UInt, char32_t> IndexToCodePointMap;

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
    void updateTextureBufferSubImage(argb_t* buffer, unsigned int bitmapWidth,
        unsigned int bitmapHeight, const std::vector<argb_t>& subImageData) const;

    /*
    \brief
        Changes the size of the texture and updates the data in it based on the old
        texture's data. The size must be larger than the original one.
     */
    void resizeAndUpdateTexture(Texture* texture, int newSize) const;
    void createTextureSpaceForGlyphRasterisation(Texture* texture, int glyphWidth, int glyphHeight) const;
   //! Register all properties of this class.
    void addFreeTypeFontProperties();
    //! Free all allocated font data.
    void free();
    void createFreetypeMemoryFace();

    void findAndThrowFreeTypeError(
        FT_Error error, const CEGUI::String& errorMessageIntro) const;

    void checkUnicodeCharMapAvailability();
    void tryToCreateFontWithClosestFontHeight(
        FT_Error errorResult, int requestedFontPixelHeight) const;
    //! initialise FontGlyph for given codepoint.
    void prepareGlyph(FreeTypeFontGlyph* glyph) const;

    void initialiseGlyphMap();

    void handleFontSizeOrFontUnitChange();

    //! Rasterises the glyph and adds it into a glyph atlas texture
    void rasterise(FreeTypeFontGlyph* glyph) const;
    
    //! Helper functions for rasterisation
    void addRasterisedGlyphToTextureAndSetupGlyphImage(
        FreeTypeFontGlyph* glyph, Texture* texture, int glyphWidth, int glyphHeight,
        const TextureGlyphLine& glyphTexLine) const;

    void findFittingSpotInGlyphTextureLines(int glyphWidth, int glyphHeight,
        bool &fittingLineWasFound, size_t &fittingLineIndex) const;

    //! Adds a new glyph atlas line if the glyph would fit into there.
    bool addNewLineIfFitting(unsigned int glyphHeight, unsigned int glyphWidth,
        size_t & fittingLineIndex) const;

    void createGlyphAtlasTexture() const;
    static std::vector<argb_t> createGlyphTextureData(FT_Bitmap& glyph_bitmap);

    const FreeTypeFontGlyph* getPreparedGlyph(char32_t currentCodePoint) const override;
    void writeXMLToStream_impl(XMLSerializer& xml_stream) const override;

    std::vector<GeometryBuffer*> layoutAndCreateGlyphRenderGeometry(
        const String& text, const Rectf* clip_rect,
        const ColourRect& colours, const float space_extra,
        ImageRenderSettings imgRenderSettings, DefaultParagraphDirection defaultParagraphDir,
        glm::vec2& penPosition) const override;

#ifdef CEGUI_USE_RAQM
    std::vector<GeometryBuffer*> layoutUsingRaqmAndCreateRenderGeometry(
        const String& text, const Rectf* clip_rect, const ColourRect& colours,
        const float space_extra, ImageRenderSettings imgRenderSettings,
        DefaultParagraphDirection defaultParagraphDir, glm::vec2& penPosition) const;
#endif

    std::vector<GeometryBuffer*> layoutUsingFreetypeAndCreateRenderGeometry(
        const String& text, const Rectf* clip_rect, const ColourRect& colours,
        const float space_extra, ImageRenderSettings imgRenderSettings,
        glm::vec2& penPosition) const;

    //! If non-zero, the overridden line spacing that we're to report.
    float d_specificLineSpacing;
    //! Specified font size for this font.
    float d_size;
    //! The specified size unit of the Font size.
    FontSizeUnit d_sizeUnit;
    //! True if the font should be rendered as anti-aliased by freeType.
    bool d_antiAliased;
    //! FreeType-specific font handle
    FT_Face d_fontFace;
    //! Font file data
    RawDataContainer d_fontData;
    //! Type definition for TextureVector.
    typedef std::vector<Texture*> TextureVector;

    typedef std::vector<BitmapImage*> ImageVector;
    //! collection of images defined for this font.
    mutable ImageVector d_glyphImages;

    //! Textures that hold the glyph imagery for this font.
    mutable TextureVector d_glyphTextures;

    //! Contains mappings from code points to Font glyphs
    mutable CodePointToGlyphMap d_codePointToGlyphMap;
    //! Contains mappings from freetype indices to Font glyphs
    mutable IndexToCodePointMap d_indexToGlyphMap;

    //! The size with which new texture atlases for glyphs are going to be initialised
    int d_initialGlyphAtlasSize = 32;
    //! The size of the last texture that has been created
    mutable int d_lastTextureSize = 0;
    //! Memory buffer for rendering the glyphs, this contains the data of the latest texture
    mutable std::vector<argb_t> d_lastTextureBuffer;
    //! Contains information about the extents of each line of glyphs of the latest texture
    mutable std::vector<TextureGlyphLine> d_textureGlyphLines;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIFreeTypeFont_h_
