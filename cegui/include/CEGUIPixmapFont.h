/***********************************************************************
    filename:   CEGUIPixmapFont.h
    created:    14/6/2006
    author:     Andrew Zabolotny
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIPixmapFont_h_
#define _CEGUIPixmapFont_h_

#include "CEGUIFont.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Implementation of the Font class interface using static Imageset's.

    To create such a font you must create a Imageset with all the glyphs,
    and then define individual glyphs via defineMapping.
*/
class PixmapFont : public Font
{
public:
    /*!
    \brief
        Constructor for Pixmap type fonts.

    \param font_name
        The name that the font will use within the CEGUI system.

    \param imageset_filename
        The filename of an imageset to load that will be used as the source for
        glyph images for this font.  If \a resource_group is the special value
        of "*", this parameter may instead refer to the name of an already
        loaded Imagset.

    \param resource_group
        The resource group identifier to use when loading the imageset file
        specified by \a imageset_filename.  If this group is set to the special
        value of "*", then \a imageset_filename instead will refer to the name
        of an existing Imageset.

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
    */
    PixmapFont(const String& font_name, const String& imageset_filename,
               const String& resource_group = "",
               const bool auto_scaled = false,
               const float native_horz_res = 640.0f,
               const float native_vert_res = 480.0f);

    //! Destructor.
    ~PixmapFont();

    void defineMapping(const utf32 codepoint, const String& image_name, 
                       const float horz_advance);

    //! Return the name of the imageset the font is using for it's glyphs.
    const String& getImageset() const;

    /*!
    \brief
        Set Imageset the font is using for it's glyphs.

        This will potentially cause an existing imageset to be unloaded (if it
        was created specifically by, and for, this Font).  The new Imageset
        must already exist within the system.

    \param imageset
        Name ofan existing image set to be used as the glyph source for this
        Font.

    \exception UnknownObjectException
        thrown if \a imageset is not known in the system.
    */
    void setImageset(const String& imageset);

protected:
    //! Initialize the imageset.
    void reinit();
    //! Register all properties of this class.
    void addPixmapFontProperties();

    // override of functions in Font base class.
    void updateFont ();
    void writeXMLToStream_impl (XMLSerializer& xml_stream) const;

    //! The imageset with the glyphs
    Imageset* d_glyphImages;
    //! Current X scaling for glyph images
    float d_origHorzScaling;
    //! true if we own the imageset
    bool d_imagesetOwner;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIPixmapFont_h_
