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

#include "CEGUI/PropertySet.h"
#include "CEGUI/EventSet.h"
#include "CEGUI/Image.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

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
class CEGUIEXPORT Font : public PropertySet, public EventSet
{
public:

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
    static const char32_t UnicodeReplacementCharacter;

    virtual ~Font() = default;

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
    static float convertPointsToPixels(float pointSize, int dotsPerInch);

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
    static float convertPixelsToPoints(float pixelSize, int dotsPerInch);

    //! Return the string holding the font name.
    const String& getName() const { return d_name; }

    //! Return the type of the font.
    const String& getTypeName() const { return d_type; }

    //! Return the filename of the used font.
    const String& getFileName() const { return d_filename; }
  
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
    const Sizef& getNativeResolution() const { return d_nativeResolution; }

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
    AutoScaledMode getAutoScaled() const { return d_autoScaled; }

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

    \return
        Number of pixels between vertical base lines, i.e. The minimum
        pixel space between two lines of text.
    */
    float getLineSpacing() const { return d_height; }

    /*!
    \brief
        return the exact pixel height of the font.

    \return
        float value describing the pixel height of the font without
        any additional padding.
    */
    float getFontHeight() const { return (d_ascender - d_descender); }

    /*!
    \brief
        Return the number of pixels from the top of the highest glyph
        to the baseline

    \return
        pixel spacing from top of front glyphs to baseline
    */
    float getBaseline() const { return d_ascender; }

    //! Return the height of the underline top edge relative to the baseline (negative below)
    float getUnderlineTop() const { return d_underlineTop; }
    //! Return the height of the underline top edge relative to the baseline (negative below)
    float getStrikeoutTop() const { return d_strikeoutTop; }
    //! Get the thickness of the underline in pixels
    float getUnderlineThickness() const { return d_underlineThickness; }
    //! Get the thickness of the strikeout in pixels
    float getStrikeoutThickness() const { return d_strikeoutThickness; }

    //! Returns the index of the glyph corresponding to the codepoint or invalid idx if it can't be found.
    uint32_t getGlyphIndexForCodepoint(char32_t codePoint) const;
    //! \brief Returns whether this Font can draw the specified code-point
    bool isCodepointAvailable(char32_t codePoint) const;
    //! Returns cached index of the glyph used for replacing unknown glyphs
    uint32_t getReplacementGlyphIndex() const { return d_replacementGlyphIdx; }

    //! Loads the glyph object at the given index and returns its pointer or nullptr on failure.
    virtual FontGlyph* loadGlyph(uint32_t index) = 0;
    //! Returns the glyph object at the given index or nullptr if no such glyph is loaded or exists.
    virtual const FontGlyph* getGlyph(uint32_t index) const = 0;
    //! Returns an outline image for the given glyph and thickness.
    virtual Image* getOutline(uint32_t index, float thickness = 1.f) = 0;
    //! \brief Calculates and returns kerning between two glyphs (in pixels, not rounded)
    virtual float getKerning(const FontGlyph* /*prev*/, const FontGlyph& /*curr*/) const { return 0.f; }


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
    float getTextExtent(const String& text);

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
    float getTextAdvance(const String& text);

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
    size_t getCharAtPixel(const String& text, float pixel) { return getCharAtPixel(text, 0, pixel); }

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
    size_t getCharAtPixel(const String& text, size_t start_char, float pixel);

    /*!
    \brief
        Sets the default resource group to be used when loading font data

    \param resourceGroup
        String describing the default resource group identifier to be used.

    \return
        Nothing.
    */
    static void setDefaultResourceGroup(const String& resourceGroup) { d_defaultResourceGroup = resourceGroup; }

    /*!
    \brief
        Returns the default resource group currently set for Fonts.

    \return
        String describing the default resource group identifier that will be
        used when loading font data.
    */
    static const String& getDefaultResourceGroup() { return d_defaultResourceGroup; }

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

    Font(const String& name, const String& type_name, const String& filename,
         const String& resource_group, AutoScaledMode auto_scaled,
         const Sizef& native_res);

    //! implementation version of writeXMLToStream.
    virtual void writeXMLToStream_impl(XMLSerializer& xml_stream) const = 0;

    //! Register all properties of this class.
    void addFontProperties();

    //! event trigger function for when the font rendering size changes.
    virtual void onRenderSizeChanged(FontEventArgs& args);

    std::unordered_map<char32_t, uint32_t> d_codePointToGlyphMap;

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

    uint32_t d_replacementGlyphIdx = std::numeric_limits<uint32_t>().max();

    //! maximal font ascender (pixels above the baseline)
    float d_ascender = 0.f;
    //! maximal font descender (negative pixels below the baseline)
    float d_descender = 0.f;
    //! (ascender - descender) + linegap
    float d_height = 0.f;
    //! Height of the underline top edge relative to the baseline
    float d_underlineTop = 0.f;
    //! Height of the strikeout top edge relative to the baseline
    float d_strikeoutTop = 0.f;
    //! Thickness of the underline in pixels
    float d_underlineThickness = 1.f;
    //! Thickness of the strikeout in pixels
    float d_strikeoutThickness = 1.f;

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
