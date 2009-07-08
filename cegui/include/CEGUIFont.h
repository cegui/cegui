/***********************************************************************
    filename:   CEGUIFont.h
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Defines interface for the Font class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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

#include "CEGUIBase.h"
#include "CEGUIPropertySet.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIVector.h"
#include "CEGUIColourRect.h"
#include "CEGUIXMLSerializer.h"
#include "CEGUIImage.h"

#include <map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

// Forward declarations for font properties
namespace FontProperties
{
class NativeRes;
class Name;
class FileName;
class ResourceGroup;
class AutoScaled;
}

/*!
\brief
    internal class representing a single font glyph.

    For TrueType fonts initially all FontGlyph's are empty
    (getImage() will return NULL), but they are filled by demand.
*/
class FontGlyph
{
private:
    /// The image which will be rendered.
    const Image* d_image;
    /// Amount to advance the pen after rendering this glyph
    float d_advance;

public:
    /*!
    \brief
        construct an empty uninitialized FontGlyph
    */
    FontGlyph()
    { }

    /*!
    \brief
        FontGlyph constructor.
    */
    FontGlyph(float advance) : d_image(0), d_advance(advance)
    { }

    /*!
    \brief
        A better :-) FontGlyph constructor.
    */
    FontGlyph(float advance, const Image *image) : d_image(image), d_advance(advance)
    { }

    /*!
    \brief
        Return the CEGUI::Image object rendered for this glyph.
    */
    const Image* getImage() const
    { return d_image; }

    /*!
    \brief
        Return the parent CEGUI::Imageset object for this glyph.
    */
    const Imageset* getImageset() const
    { return d_image->getImageset(); }

    /*!
    \brief
        Return the scaled pixel size of the glyph.
    */
    Size getSize(float x_scale, float y_scale) const
    { return Size(getWidth(x_scale), getHeight(y_scale)); }

    /*!
    \brief
        Return the scaled widht of the glyph.
    */
    float getWidth(float x_scale) const
    { return d_image->getWidth() * x_scale; }

    /*!
    \brief
        Return the scaled height of the glyph.
    */
    float getHeight(float y_scale) const
    { return d_image->getHeight() * y_scale; }

    /*!
    \brief
        Return the rendered advance value for this glyph.

        The rendered advance value is the total number of pixels from the
        current pen position that will be occupied by this glyph when rendered.
    */
    float getRenderedAdvance(float x_scale) const
    { return (d_image->getWidth() + d_image->getOffsetX()) * x_scale; }

    /*!
    \brief
        Return the horizontal advance value for the glyph.

        The returned value is the number of pixels the pen should move
        horizontally to position itself ready to render the next glyph.  This
        is not always the same as the glyph image width or rendererd advance,
        since it allows for horizontal overhangs.
    */
    float getAdvance(float x_scale = 1.0) const
    { return d_advance * x_scale; }

    /*!
    \brief
        Set the horizontal advance value for the glyph.
    */
    void setAdvance(float advance)
    { d_advance = advance; }

    /*!
    \brief
        Set the CEGUI::Image object rendered for this glyph.
    */
    void setImage(const Image* image)
    { d_image = image; }
};


/*!
\brief
    Class that encapsulates text rendering functionality for a typeface

    A Font object is created for each unique typeface required.
    The Font class provides methods for loading typefaces from various sources,
    and then for outputting text via the Renderer object.

    This class is not specific to any font renderer, it just provides the
    basic interfaces needed to manage fonts.
*/
class CEGUIEXPORT Font : public PropertySet
{
protected:
    /*************************************************************************
        Friends so that only FontManager can create and destroy font objects
    *************************************************************************/
    friend class FontManager;
    friend class Font_xmlHandler;
    friend class FontProperties::NativeRes;
    friend class FontProperties::Name;
    friend class FontProperties::FileName;
    friend class FontProperties::ResourceGroup;
    friend class FontProperties::AutoScaled;

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    typedef std::map<utf32, FontGlyph> CodepointMap;
    /// Contains mappings from code points to Image objects
    CodepointMap d_cp_map;

    /// Name of this font.
    String d_name;
    /// Holds the name of the file used to create this font (either font file or imagset)
    String d_fileName;
    /// Holds the name of the font file's resource group
    String d_resourceGroup;
    /// hold default resource group for font loading.
    static String d_defaultResourceGroup;

    /// maximal font ascender (pixels above the baseline)
    float d_ascender;
    /// maximal font descender (negative pixels below the baseline)
    float d_descender;
    /// (ascender - descender) + linegap
    float d_height;

    /// true when auto-scaling is enabled.
    bool d_autoScale;
    /// current horizontal scaling factor.
    float d_horzScaling;
    /// current vertical scaling factor.
    float d_vertScaling;
    /// native horizontal resolution for this Imageset.
    float d_nativeHorzRes;
    /// native vertical resolution for this Imageset.
    float d_nativeVertRes;

    /// Maximal codepoint for font glyphs
    utf32 d_maxCodepoint;

    /*!
    \brief
        This bitmap holds information about loaded 'pages' of glyphs.
        A glyph page is a set of 256 codepoints, starting at 256-multiples.
        For example, the 1st glyph page is 0-255, fourth is 1024-1279 etc.
        When a specific glyph is required for painting, the corresponding
        bit is checked to see if the respective page has been rasterized.
        If not, the rasterize() method is invoked, which prepares the
        glyphs from the respective glyph page for being painted.

        This array is big enough to hold at least max_codepoint bits.
        If this member is NULL, all glyphs are considered pre-rasterized.
    */
    uint *d_glyphPageLoaded;

    /*************************************************************************
        Construction & Destruction
    *************************************************************************/
    /*!
    \brief
        Constructs a new semi-complete Font object. It is the
        responsability of the user to set up all remaining font
        parameters after constructing the Font object, and finally
        calling the load() method which will make font available for use.
        All font parameters that are not initialized are set to sensible
        default values.

    \param name
        The unique name that will be used to identify this Font.

    \param fontname
        The filename of the font file, which contains the font data.
        This can be a TrueType, PostScript, bitmap font etc file.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider
        to load the font definition file.
    */
    Font(const String& name, const String& fontname,
         const String& resourceGroup = "");

    /*!
    \brief
        Constructs a new Font object and instantly loads it.
        The font is ready for use right after creation, there is no
        need to load() it. All data required by this font is loaded
        from the provided XMLAttributes object.

    \param attributes
        The XML attributes attached to this Font.

    \exception  FileIOException
        thrown if there was some problem accessing or parsing the file
        \a filename
    \exception  InvalidRequestException
        thrown if an invalid filename was provided.
    \exception  AlreadyExistsException
        thrown if a Font Imageset clashes with one already defined in the
        system.
    \exception  GenericException
        thrown if something goes wrong while accessing a true-type font
        referenced in file \a filename.
    \exception  RendererException
        thrown if the Renderer can't support a texture large enough to
        hold the requested glyph imagery.
    \exception  MemoryException
        thrown if allocation of imagery construction buffer fails.
    */
    Font(const XMLAttributes& attributes);

    /*!
    \brief
        Destroys a Font object
    */
    virtual ~Font();

    /*!
    \brief
        Define a glyph mapping (handle a <Mapping /> XML element)
    */
    virtual void defineMapping(const XMLAttributes& attributes);

    /*!
    \brief
        Update the font as required according to the current parameters
    */
    virtual void updateFont() = 0;

    /*!
    \brief
        Return a pointer to the glyphDat struct for the given codepoint,
        or 0 if the codepoint does not have a glyph defined.

    \param codepoint
        utf32 codepoint to return the glyphDat structure for.

    \return
        Pointer to the glyphDat struct for \a codepoint, or 0 if no glyph
        is defined for \a codepoint.
    */
#ifdef CEGUI_BIDI_SUPPORT
public:
#endif
    const FontGlyph* getGlyphData(utf32 codepoint);
#ifdef CEGUI_BIDI_SUPPORT
protected:
#endif
    /*!
    \brief
        Set the maximal glyph index. This reserves the respective
        number of bits in the d_glyphPageLoaded array.
    */
    void setMaxCodepoint(utf32 codepoint);

    /*!
    \brief
        This function prepares a certain range of glyphs to be ready for
        displaying. This means that after returning from this function
        glyphs from d_cp_map[start_codepoint] to d_cp_map[end_codepoint]
        should have their d_image member set. If there is an error
        during rasterization of some glyph, it's okay to leave the
        d_image field set to NULL, in which case such glyphs will
        be skipped from display.
    \param start_codepoint
        The lowest codepoint that should be rasterized
    \param end_codepoint
        The highest codepoint that should be rasterized
    */
    virtual void rasterize(utf32 start_codepoint, utf32 end_codepoint);

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
        Same as writeXMLToStream() but called from inside writeXMLToStream()
        so that derived classes may add their own attributes to stream.

    \param xml_stream
        Stream where xml data should be output.
    */
    virtual void writeXMLToStream_impl(XMLSerializer& xml_stream) const = 0;

    /*!
    \brief
        Register all properties of this class.
    */
    void addFontProperties();

public:
    /// Colour value used whenever a colour is not specified.
    static const argb_t DefaultColour;

    /*!
    \brief
        Complete font loading. If you create the font from an XML file,
        this method is invoked automatically after reading all the required
        data from the XMLAttributes object. If you create the font manually,
        it is your responsability to call this function as soon as you
        set up all the appropiate fields of the Font object.
    */
    virtual void load() = 0;

    /*!
    \brief
        Return whether this Font can draw the specified code-point

    \param cp
        utf32 code point that is the subject of the query.

    \return
        true if the font contains a mapping for code point \a cp,
        false if it does not contain a mapping for \a cp.
    */
    bool isCodepointAvailable(utf32 cp) const
    { return (d_cp_map.find(cp) != d_cp_map.end()); }

    /*************************************************************************
        Text drawing methods
    *************************************************************************/
    /*!
    \brief
        Draw text into a specified area of the display.

    \param buffer
        GeometryBuffer object where the geometry for the text be queued.

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

    \param x_scale
        Scaling factor to be applied to each glyph's x axis, where 1.0f is
        considered to be 'normal'.

    \param y_scale
        Scaling factor to be applied to each glyph's y axis, where 1.0f is
        considered to be 'normal'.

    \return
        Nothing.
    */
    void drawText(GeometryBuffer& buffer, const String& text,
                  const Vector2& position, const Rect* clip_rect,
                  const ColourRect& colours, const float space_extra = 0.0f,
                  const float x_scale = 1.0f, const float y_scale = 1.0f);

    /*!
    \brief
        Set the native resolution for this Font

    \param size
        Size object describing the new native screen resolution for this Font.

    \return
        Nothing
    */
    virtual void setNativeResolution(const Size& size);

    /*!
    \brief
        Notify the Font that the display size may have changed.

    \param size
        Size object describing the display resolution
    */
    virtual void notifyDisplaySizeChanged(const Size& size);

    /*************************************************************************
        Informational methods
    *************************************************************************/
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

    \param x_scale
        Scaling factor to be applied to each glyph's x axis when
        measuring the extent, where 1.0f is considered to be 'normal'.

    \return
        Number of pixels that \a text will occupy when rendered with
        this Font.
    */
    float getTextExtent(const String& text, float x_scale = 1.0f);

    /*!
    \brief
        Return the index of the closest text character in String \a text
        that corresponds to pixel location \a pixel if the text were rendered.

    \param text
        String object containing the text.

    \param pixel
        Specifies the (horizontal) pixel offset to return the character
        index for.

    \param x_scale
        Scaling factor to be applied to each glyph's x axis when measuring
        the text extent, where 1.0f is considered to be 'normal'.

    \return
        Returns a character index into String \a text for the character that
        would be rendered closest to horizontal pixel offset \a pixel if the
        text were to be rendered via this Font.  Range of the return is from
        0 to text.length(), so may actually return an index past the end of
        the string, which indicates \a pixel was beyond the last character.
    */
    size_t getCharAtPixel(const String& text, float pixel, float x_scale = 1.0f)
    { return getCharAtPixel(text, 0, pixel, x_scale); }

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

    \param x_scale
        Scaling factor to be applied to each glyph's x axis when measuring
        the text extent, where 1.0f is considered to be 'normal'.

    \return
        Returns a character index into String \a text for the character that
        would be rendered closest to horizontal pixel offset \a pixel if the
        text were to be rendered via this Font.  Range of the return is from
        0 to text.length(), so may actually return an index past the end of
        the string, which indicates \a pixel was beyond the last character.
    */
    size_t getCharAtPixel(const String& text, size_t start_char, float pixel, float x_scale = 1.0f);

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
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif


#endif  // end of guard _CEGUIFont_h_
