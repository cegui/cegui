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
                 const AutoScaledMode auto_scaled = ASM_Disabled,
                 const Sizef& native_res = Sizef(640.0f, 480.0f),
                 const float specific_line_spacing = 0.0f);

    //! Destructor.
    virtual ~FreeTypeFont();

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
protected:
    /*!
    \brief
        Copy the current glyph data into \a buffer, which has a width of
        \a buf_width pixels (not bytes).

    \param buffer
        Memory buffer large enough to receive the imagery for the currently
        loaded glyph.

    \param buf_width
        Width of \a buffer in pixels (where each pixel is a argb_t).

    \return
        Nothing.
    */
    void drawGlyphToBuffer(argb_t* buffer, unsigned int buf_width) const;

    /*!
    \brief
        Return the required texture size required to store imagery for the
        glyphs from s to e
    \param s
        The first glyph in set
    \param e
        The last glyph in set
    */
    unsigned int getTextureSize(CodepointMap::const_iterator s,
                        CodepointMap::const_iterator e) const;

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
    void initialiseFontGlyph(CodepointMap::iterator cp) const;

    void initialiseGlyphMap();

    void handleFontSizeOrFontUnitChange();

    // overrides of functions in Font base class.
    const FontGlyph* findFontGlyph(const char32_t codepoint) const override;
    void rasterise(char32_t start_codepoint, char32_t end_codepoint) const override;
    void updateFont() override;
    void writeXMLToStream_impl (XMLSerializer& xml_stream) const override;
#ifdef CEGUI_USE_RAQM
    //! The recommended way of rendering a glyph
    void layoutAndRenderGlyphs(const String& text, const glm::vec2& position,
        const Rectf* clip_rect, const ColourRect& colours,
        const float space_extra, const float x_scale,
        const float y_scale, ImageRenderSettings imgRenderSettings,
        glm::vec2& glyph_pos, GeometryBuffer*& textGeometryBuffer) const override;

#endif

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
    //! Textures that hold the glyph imagery for this font.
    mutable TextureVector d_glyphTextures;
    typedef std::vector<BitmapImage*> ImageVector;
    //! collection of images defined for this font.
    mutable ImageVector d_glyphImages;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIFreeTypeFont_h_
