/***********************************************************************
filename: 	CEGUIFont_xmlHandler.h
created:	21/2/2004
author:		Paul D Turner

purpose:	Defines interface for the Font class
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
#ifndef _CEGUIFont_xmlHandler_h_
#define _CEGUIFont_xmlHandler_h_

#include "CEGUIFont.h"
#include "CEGUIXMLHandler.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
Handler class used to parse the Font XML files using SAX2
*/
class Font_xmlHandler : public XMLHandler
{
public:
	/*************************************************************************
	Construction & Destruction
	*************************************************************************/
	/*!
	\brief
	Constructor for Font::xmlHandler objects

	\param font
	Pointer to the Font object creating this xmlHandler object
	*/
	Font_xmlHandler(Font* font) : d_font(font) {}

	/*!
	\brief
	Destructor for Font::xmlHandler objects
	*/
	virtual ~Font_xmlHandler(void) {}

	/*************************************************************************
	SAX2 Handler overrides
	*************************************************************************/
	/*!
	\brief
	document processing (only care about elements, schema validates format)
	*/
    virtual void elementStart(const String& element, const XMLAttributes& attributes);
    virtual void elementEnd(const String& element);

private:
	/*************************************************************************
	Implementation Constants
	*************************************************************************/
	// XML related strings
	static const String FontElement;					//!< Tag name for Font elements.
	static const String MappingElement;				//!< Tag name for Mapping elements.
	static const String FontTypeStatic;				//!< Value used for FontTypeAttribute for a static (bitmapped) font.
	static const String FontTypeDynamic;				//!< Value used for FontTypeAttribute for a dynamic (true-type) font.
	static const String GlyphElement;					//!< Tag name for Glyph elements.
	static const String GlyphRangeElement;			//!< Tag name for GlyphRange elements.
	static const String GlyphSetElement;				//!< Tag name for GlyphSet elements.
	static const String FontNameAttribute;			//!< Attribute name that stores the name of the Font
    static const String FontFilenameAttribute;		//!< Attribute name that stores the filename, this is either an Imageset xml file, or a font file.
    static const String FontResourceGroupAttribute;   //!< Attribute name that stores the resource group identifier used when loading font file.
	static const String FontTypeAttribute;			//!< Attribute name that stores the type of font being defined (either static or dynamic).
	static const String FontSizeAttribute;			//!< Attribute name that stores the point size for a dynamic font.
	static const String FontFirstCodepointAttribute;	//!< Attribute name that stores the first code-point for a dynamic font.
	static const String FontLastCodepointAttribute;	//!< Attribute name that stores the last code-point for a dynamic font.
	static const String FontNativeHorzResAttribute;	//!< Optional attribute that stores 'native' horizontal resolution for the Font.
	static const String FontNativeVertResAttribute;	//!< Optional attribute that stores 'native' vertical resolution for the Font.
	static const String FontAutoScaledAttribute;		//!< Optional attribute that specifies whether the Font should be auto-scaled.
	static const String FontAntiAliasedAttribute;		//!< Optional attribute that specifies whether the TTF based font should be anti-aliased.
	static const String MappingCodepointAttribute;	//!< Attribute name that stores the Unicode code-point for a mapping.
	static const String MappingImageAttribute;		//!< Attribute name that stores the Image name for a mapping.
	static const String MappingHorzAdvanceAttribute;	//!< Attribute name that stores the horizontal advance for a glyph.
	static const String GlyphCodepointAttribute;				//!< Attribute name that stores the U+ codepoint to add to the set.
	static const String GlyphRangeStartCodepointAttribute;	//!< Attribute name that stores the U+ codepoint for the start of a range.
	static const String GlyphRangeEndCodepointAttribute;		//!< Attribute name that stores the U+ codepoint for the end of a range.
	static const String GlyphSetGlyphsAttribute;				//!< Attribute name that stores the UTF8 encoded codepoint set.

	// general constants
	static const int	AutoGenerateHorzAdvance;		//!< Horizontal advance value that tells the parser to auto-calculate some reasonable value.

    /*!
    \brief
        Method that handles the opening Font XML element.
    */
    void elementFontStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the Mapping XML element.
    */
    void elementMappingStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the Glyph XML element.
    */
    void elementGlyphStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the GlyphRange XML element.
    */
    void elementGlyphRangeStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the GlyphSet XML element.
    */
    void elementGlyphSetStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the closing Font XML element.
    */
    void elementFontEnd();

    /*!
    \brief
        Method that completes font creatiion for a dynamic font.
    */
    void createDynamicFont(const XMLAttributes& attributes);

    /*!
    \brief
        Method that completes font creatiion for a static font.
    */
    void createStaticFont(const XMLAttributes& attributes);

    /*!
    \brief
        Method that initialises the auto-scale settings of the font being
        created.
    */
    void initialiseAutoScaling(const XMLAttributes& attributes);

	/*************************************************************************
	Implementation Data
	*************************************************************************/
	Font*	d_font;			//!< Font object that we are helping to build
	String	d_glyphSet;		//!< String holding the set of codepoints to be available.
};

} // End of  CEGUI namespace section

#endif
