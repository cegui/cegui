/***********************************************************************
	filename: 	CEGUIPixmapFont.h
	created:	14/6/2006
	author:		Andrew Zabolotny
	
	purpose:    Implementation of the Font class via static imagesets
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
#ifndef _CEGUIPixmapFont_h_
#define _CEGUIPixmapFont_h_

#include "CEGUIFont.h"
#include "CEGUIImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Forward declarations for font properties
namespace FontProperties
{
    class PixmapImageset;
    class PixmapMapping;
}

/*!
\brief
    Implementation of the Font class interface using static Imageset's.

    To create such a font you must create a Imageset with all the glyphs,
    and then define individual glyphs via the Mapping object property
    (or via the Mapping XML element).
*/
class PixmapFont : public Font
{
protected:
    friend class FontManager;
    friend class FontProperties::PixmapImageset;
    friend class FontProperties::PixmapMapping;

    /// The imageset with the glyphs
    Imageset *d_glyphImages;

    /// Current X scaling for glyph images
    float d_origHorzScaling;

    /// true if we're the owners of the imageset
	bool d_imagesetOwner;

    /*!
    \brief
        Constructs a new semi-complete Font object. It is the responsability
        of the user to define the glyph mapping (via the Mapping property),
        and finally call the load() method which will make font available
        for use (this is not *required* for PixmapFont, though).

        All font parameters that are not initialized are set to sensible
        default values.

    \param name
        The unique name that will be used to identify this Font.

    \param fontname
        The filename of the font file, which contains the font data.
        This can be a TrueType, PostScript, bitmap font etc file.
        If resourceGroup is set to the special value of "*",
        fontname is interpreted as a imageset name and the respective
        Imageset object must be already loaded.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider
        to load the font definition file.

    \exception
        UnknownObjectException Thrown if no Imageset named \a filename is
        present in within the system (when resourceGroup == "*").
    */
    PixmapFont (const String& name, const String& fontname,
                const String& resourceGroup = "");

    /// \copydoc Font::Font
    PixmapFont (const XMLAttributes& attributes);

    /// \copydoc Font::~Font
    virtual ~PixmapFont ();

    /// \copydoc Font::updateFont
    virtual void updateFont ();

    /// \copydoc Font::writeXMLToStream_impl
    virtual void writeXMLToStream_impl (XMLSerializer& xml_stream) const;

    /*!
    \brief
        Define a glyph mapping (handle a <Mapping /> XML element)
    */
    virtual void defineMapping (const XMLAttributes& attributes);

    /*!
    \brief
        Define a single glyph mapping
    */
    void defineMapping (String image_name, utf32 codepoint, float horzAdvance);

    /*!
    \brief
        Register all properties of this class.
    */
    void addPixmapFontProperties ();

    /*!
    \brief
        Initialize the imageset.
    */
    void reinit ();

public:
    /// \copydoc Font::load
    virtual void load ();
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIPixmapFont_h_
