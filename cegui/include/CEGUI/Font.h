/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIFont_h_
#define _CEGUIFont_h_

#include "CEGUI/Base.h"
#include "CEGUI/PropertySet.h"
#include "CEGUI/EventSet.h"
#include "CEGUI/String.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/FontGlyph.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class that encapsulates a typeface.

    A Font object is created for each unique typeface required.
    The Font class provides methods for loading typefaces from various sources,
    and then for outputting text via the Renderer object.

    This class is not specific to any font renderer, it just provides the
    basic interfaces needed to manage fonts.
*/
class CEGUIEXPORT Font :
    public PropertySet,
    public EventSet
{
public:
    //! Colour value used whenever a colour is not specified.
    static const argb_t DefaultColour;

    //! Event namespace for font events
    static const String EventNamespace;
    /** Event fired when the font internal state has changed such that the
     * rendered size of they glyphs is different.
     * Handlers are passed a const FontEventArgs reference with
     * FontEventArgs::font set to the Font whose rendered size has changed.
     */
    static const String EventRenderSizeChanged;

    /*! The code point of the replacement character defined by the Unicode standard.
        This is typically rendered as rectangle or question-mark inside a box and
        is used whenever a unicode code point can not be rendered because it is not
        present in the Font or otherwise unsupported.
    */
    static const char32_t UnicodeReplacementCharacter = 0xFFFD;

    //! Destructor.
    virtual ~Font();

    /*!
    \brief
        Helper function that converts point sizes of Fonts into 
        pixel sizes. Points are a physical unit. 1 point is
        equal to 1/72th of an inch. The function takes the dots per
        inch as well as the pixel size. 

    \param pointSize
        The point size to be converted.

    \param dotsPerInch
        The dots per inch resolution to be used for the conversion.

    \return
        Returns the calculated Font size in pixels.

    \see Render::ReferenceDpiValue
    \see Font::convertPixelsToPoints
    */
    static float convertPointsToPixels(const float pointSize, const int dotsPerInch);

        /*!
    \brief
        Helper function that converts pixel sizes of Fonts into 
        point sizes. Points are a physical unit. 1 point is
        equal to 1/72th of an inch. The function takes the dots per
        inch as well as the pixel size. 

    \param pixelSize
        The pixel size to be converted.

    \param dotsPerInch
        The dots per inch resolution to be used for the conversion.

    \return
        Returns the calculated Font size in points.

    \see Render::ReferenceDpiValue
    \see Font::convertPointsToPixels
    */
    static float convertPixelsToPoints(const float pixelSize, const int dotsPerInch);

    //! Return the string holding the font name.
    const String& getName() const;

    //! Return the type of the font.
    const String& getTypeName() const;

    //! Return the filename of the used font.
    const String& getFileName() const;
    
    /*!
    \brief
        Return whether this Font can draw the specified code-point

    \param cp
        char32_t code point that is the subject of the query.

    \return
        true if the font contains a mapping for code point \a cp,
        false if it does not contain a mapping for \a cp.
    */
    virtual bool isCodepointAvailable(char32_t codePoint) const = 0;

    /*!
    \brief
        Create render geometry for the text that should be rendered into a
        specified area of the display.

    \param text
        String object containing the text to be drawn.

    \param nextPenPosX
        The x-coordinate where the pen is positioned after rendering

    \param position
        Reference to a Vector2 object describing the location at which the text
        is to be drawn.

    \param clip_rect
        Rect object describing the clipping area for the drawing.
        No drawing will occur outside this Rect.

    \param colours
        ColourRect object describing the colours to be applied when drawing the
        text.  NB: The colours specified in here are applied to each glyph,
        rather than the text as a whole.

    \param space_extra
        Number of additional pixels of spacing to be added to space characters.

    \return
        Returns a list of GeometryBuffers representing the render geometry of
        the text.
    */
    std::vector<GeometryBuffer*> createTextRenderGeometry(
        const String& text, float& nextPenPosX,
        const glm::vec2& position, const Rectf* clip_rect,
        const bool clipping_enabled, const ColourRect& colours, const DefaultParagraphDirection defaultParagraphDir,
        const float space_extra = 0.0f) const;

        /*!
    \brief
        Create render geometry for the text that should be rendered into a
        specified area of the display.

    \param text
        String object containing the text to be drawn.

    \param position
        Reference to a Vector2 object describing the location at which the text
        is to be drawn.

    \param clip_rect
        Rect object describing the clipping area for the drawing.
        No drawing will occur outside this Rect.

    \param colours
        ColourRect object describing the colours to be applied when drawing the
        text.  NB: The colours specified in here are applied to each glyph,
        rather than the text as a whole.

    \param space_extra
        Number of additional pixels of spacing to be added to space characters.

    \return
        Returns a list of GeometryBuffers representing the render geometry of
        the text.
    */
    std::vector<GeometryBuffer*> createTextRenderGeometry(
        const String& text,
        const glm::vec2& position, const Rectf* clip_rect,
        const bool clipping_enabled, const ColourRect& colours, const DefaultParagraphDirection defaultParagraphDir,
        const float space_extra = 0.0f) const;
  
    /*!
      \brief
          Set the native resolution for this Font
  
      \param size
          Size object describing the new native screen resolution for this Font.
      */
    void setNativeResolution(const Sizef& size);

    /*!
    \brief
        Return the native display size for this Font.  This is only relevant if
        the Font is being auto-scaled.

    \return
        Size object describing the native display size for this Font.
    */
    const Sizef& getNativeResolution() const;

    /*!
    \brief
        Enable or disable auto-scaling for this Font.

    \param auto_scaled
        AutoScaledMode describing how this font should be auto scaled

    \see AutoScaledMode
    */
    void setAutoScaled(const AutoScaledMode auto_scaled);

    /*!
    \brief
        Checks whether this font is being auto-scaled and how.

    \return
        AutoScaledMode describing how this font should be auto scaled
    */
    AutoScaledMode getAutoScaled() const;

    /*!
    \brief
        Notify the Font that the display size may have changed.

    \param size
        Size object describing the display resolution
    */
    virtual void notifyDisplaySizeChanged(const Sizef& size);

    /*!
    \brief
        Return the pixel line spacing value for.

    \param y_scale
        Scaling factor to be applied to the line spacing, where 1.0f
        is considered to be 'normal'.

    \return
        Number of pixels between vertical base lines, i.e. The minimum
        pixel space between two lines of text.
    */
    float getLineSpacing(float y_scale = 1.0f) const
    { return d_height * y_scale; }

    /*!
    \brief
        return the exact pixel height of the font.

    \param y_scale
        Scaling factor to be applied to the height, where 1.0f
        is considered to be 'normal'.

    \return
        float value describing the pixel height of the font without
        any additional padding.
    */
    float getFontHeight(float y_scale = 1.0f) const
    { return (d_ascender - d_descender) * y_scale; }

    /*!
    \brief
        Return the number of pixels from the top of the highest glyph
        to the baseline

    \param y_scale
        Scaling factor to be applied to the baseline distance, where 1.0f
        is considered to be 'normal'.

    \return
        pixel spacing from top of front glyphs to baseline
    */
    float getBaseline(float y_scale = 1.0f) const
    { return d_ascender * y_scale; }

    /*!
    \brief
        Return the pixel width of the specified text if rendered with
        this Font.

    \param text
        String object containing the text to return the rendered pixel
        width for.

    \return
        Number of pixels that \a text will occupy when rendered with
        this Font.

    \note
        The difference between the advance and the extent of a text string is
        important for numerous reasons. Picture some scenario where a glyph
        has a swash which extends way beyond the subsequent glyph - the text
        extent of those two glyphs is to the end of the swash on the first glyph
        whereas the advance of those two glyphs is to the start of a theoretical
        third glyph - still beneath the swash of the first glyph.
        The difference can basically be summarised as follows:
        - the extent is the total rendered width of all glyphs in the string.
        - the advance is the width to the point where the next character would
          have been drawn.

    \see getTextAdvance
    */
    float getTextExtent(const String& text) const;

    /*!
    \brief
        Calculates and returns the size this glyph takes up if it is the last character, or 
        the size the glyph takes until the next character begins (considering kerning).
    */
    void getGlyphExtents(
        char32_t currentCodePoint,
        float& cur_extent,
        float& adv_extent) const;

    /*!
    \brief
        Return pixel advance of the specified text when rendered with this Font.

    \param text
        String object containing the text to return the pixel advance for.

    \return
        pixel advance of \a text when rendered with this Font.

    \note
        The difference between the advance and the extent of a text string is
        important for numerous reasons. Picture some scenario where a glyph
        has a swash which extends way beyond the subsequent glyph - the text
        extent of those two glyphs is to the end of the swash on the first glyph
        whereas the advance of those two glyphs is to the start of a theoretical
        third glyph - still beneath the swash of the first glyph.
        The difference can basically be summarised as follows:
        - the extent is the total rendered width of all glyphs in the string.
        - the advance is the width to the point where the next character would
          have been drawn.

    \see getTextExtent
    */
    float getTextAdvance(const String& text) const;

    /*!
    \brief
        Return the index of the closest text character in String \a text
        that corresponds to pixel location \a pixel if the text were rendered.

    \param text
        String object containing the text.

    \param pixel
        Specifies the (horizontal) pixel offset to return the character
        index for.

    \return
        Returns a character index into String \a text for the character that
        would be rendered closest to horizontal pixel offset \a pixel if the
        text were to be rendered via this Font.  Range of the return is from
        0 to text.length(), so may actually return an index past the end of
        the string, which indicates \a pixel was beyond the last character.
    */
    size_t getCharAtPixel(const String& text, float pixel) const
    { return getCharAtPixel(text, 0, pixel); }

    /*!
    \brief
        Return the index of the closest text character in String \a text,
        starting at character index \a start_char, that corresponds
        to pixel location \a pixel if the text were to be rendered.

    \param text
        String object containing the text.

    \param start_char
        index of the first character to consider.  This is the lowest
        value that will be returned from the call.

    \param pixel
        Specifies the (horizontal) pixel offset to return the character
        index for.

    \return
        Returns a character index into String \a text for the character that
        would be rendered closest to horizontal pixel offset \a pixel if the
        text were to be rendered via this Font.  Range of the return is from
        0 to text.length(), so may actually return an index past the end of
        the string, which indicates \a pixel was beyond the last character.
    */
    size_t getCharAtPixel(const String& text, size_t start_char, float pixel) const;

    /*!
    \brief
        Sets the default resource group to be used when loading font data

    \param resourceGroup
        String describing the default resource group identifier to be used.

    \return
        Nothing.
    */
    static void setDefaultResourceGroup(const String& resourceGroup)
    { d_defaultResourceGroup = resourceGroup; }

    /*!
    \brief
        Returns the default resource group currently set for Fonts.

    \return
        String describing the default resource group identifier that will be
        used when loading font data.
    */
    static const String& getDefaultResourceGroup()
    { return d_defaultResourceGroup; }

    /*!
    \brief
        Writes an xml representation of this Font to \a out_stream.

    \param xml_stream
        Stream where xml data should be output.

    \return
        Nothing.
    */
    void writeXMLToStream(XMLSerializer& xml_stream) const;

    /*!
    \brief
        Update the font as needed, according to the current parameters.
        This should only be called if really necessary and will only 
        internally update the Font, without firing events or updating
        any Windows using this Font.
    */
    virtual void updateFont() = 0;

protected:
    //! Constructor.
    Font(const String& name, const String& type_name, const String& filename,
         const String& resource_group, const AutoScaledMode auto_scaled,
         const Sizef& native_res);

    //! implementation version of writeXMLToStream.
    virtual void writeXMLToStream_impl(XMLSerializer& xml_stream) const = 0;

    //! Register all properties of this class.
    void addFontProperties();

    //! event trigger function for when the font rendering size changes.
    virtual void onRenderSizeChanged(FontEventArgs& args);

    //! Returns the FontGlyph corresponding to the codepoint or 0 if it can't be found.
    virtual FontGlyph* getGlyphForCodepoint(const char32_t codePoint) const = 0;

    //! The old way of rendering glyphs, without kerning and extended layouting
    virtual std::vector<GeometryBuffer*> layoutUsingFallbackAndCreateGlyphGeometry(const String& text,
        const Rectf* clip_rect, const ColourRect& colours,
        const float space_extra,
        ImageRenderSettings imgRenderSettings,
        glm::vec2& glyph_pos) const;

    /*! 
    \brief
        Adds the render geometry data to the supplied vector. A new GeometryBuffer
        might be added if necessary or data might be added to an existing one.
    */
    void addGlyphRenderGeometry(std::vector<GeometryBuffer*> &textGeometryBuffers,
                                const Image* image, ImageRenderSettings &imgRenderSettings,
                                const Rectf* clip_rect, const ColourRect& colours) const;

    //! Manages the glyph layout and and creates the RenderGeometry for the text.
    virtual std::vector<GeometryBuffer*> layoutAndCreateGlyphRenderGeometry(
        const String& text, const Rectf* clip_rect,
        const ColourRect& colours, const float space_extra,
        ImageRenderSettings imgRenderSettings, DefaultParagraphDirection /*defaultParagraphDir*/,
        glm::vec2& glyphPos) const
    {
        return layoutUsingFallbackAndCreateGlyphGeometry(text, clip_rect,
            colours, space_extra, imgRenderSettings, glyphPos);
    }

    /*!
    \brief
        Checks if the supplied GeometryBuffers contain a GeometryBuffer using the same Image.
        If this is the case, the first found Geometrybuffer will be returned, otherwise 
        a nullptr will be returned.
    */
    static GeometryBuffer* findCombinableBuffer(const std::vector<GeometryBuffer*>& geomBuffers,
        const Image* image);

    /*!
    \brief
        Tries to find the FontGlyph for the supplied codepoint. Before returning it,
        extra steps might be taking, such as initialising and rasterising the glyph
        if necessary.
    */
    virtual const FontGlyph* getPreparedGlyph(char32_t currentCodePoint) const;

    //! Name of this font.
    String d_name;
    //! Type name string for this font (not used internally)
    String d_type;
    //! Name of the file used to create this font (font file or imagset)
    String d_filename;
    //! Name of the font file's resource group.
    String d_resourceGroup;
    //! Holds default resource group for font loading.
    static String d_defaultResourceGroup;

    //! maximal font ascender (pixels above the baseline)
    float d_ascender;
    //! maximal font descender (negative pixels below the baseline)
    float d_descender;
    //! (ascender - descender) + linegap
    float d_height;

    //! which mode should we use for auto-scaling
    AutoScaledMode d_autoScaled;
    //! native resolution for this Font.
    Sizef d_nativeResolution;
    //! current horizontal scaling factor.
    float d_horzScaling;
    //! current vertical scaling factor.
    float d_vertScaling;
};



} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif


#endif  // end of guard _CEGUIFont_h_
