/************************************************************************
filename: 	CEGUIFont_xmlHandler.h
created:	21/2/2004
author:		Paul D Turner

purpose:	Defines interface for the Font class
*************************************************************************/
/*************************************************************************
Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#ifndef _CEGUIFont_xmlHandler_h_
#define _CEGUIFont_xmlHandler_h_

#include "CEGUIFont.h"

#include "xercesc/sax2/DefaultHandler.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
Handler class used to parse the Font XML files using SAX2
*/
class Font_xmlHandler : public XERCES_CPP_NAMESPACE::DefaultHandler
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
	virtual void	startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs);

	/*!
	\brief
	error processing
	*/
	virtual void  warning (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
	virtual void  error (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
	virtual void  fatalError (const XERCES_CPP_NAMESPACE::SAXParseException &exc);

private:
	/*************************************************************************
	Implementation Constants
	*************************************************************************/
	// XML related strings
	static const utf8	FontElement[];					//!< Tag name for Font elements.
	static const utf8	MappingElement[];				//!< Tag name for Mapping elements.
	static const utf8	FontTypeStatic[];				//!< Value used for FontTypeAttribute for a static (bitmapped) font.
	static const utf8	FontTypeDynamic[];				//!< Value used for FontTypeAttribute for a dynamic (true-type) font.
	static const char	FontNameAttribute[];			//!< Attribute name that stores the name of the Font
	static const char	FontFilenameAttribute[];		//!< Attribute name that stores the filename, this is either an Imageset xml file, or a font file.
	static const char	FontTypeAttribute[];			//!< Attribute name that stores the type of font being defined (either static or dynamic).
	static const char	FontSizeAttribute[];			//!< Attribute name that stores the point size for a dynamic font.
	static const char	FontFirstCodepointAttribute[];	//!< Attribute name that stores the first code-point for a dynamic font.
	static const char	FontLastCodepointAttribute[];	//!< Attribute name that stores the last code-point for a dynamic font.
	static const char	FontNativeHorzResAttribute[];	//!< Optional attribute that stores 'native' horizontal resolution for the Font.
	static const char	FontNativeVertResAttribute[];	//!< Optional attribute that stores 'native' vertical resolution for the Font.
	static const char	FontAutoScaledAttribute[];	//!< Optional attribute that specifies whether the Font should be auto-scaled.
	static const char	MappingCodepointAttribute[];	//!< Attribute name that stores the Unicode code-point for a mapping.
	static const char	MappingImageAttribute[];		//!< Attribute name that stores the Image name for a mapping.
	static const char	MappingHorzAdvanceAttribute[];	//!< Attribute name that stores the horizontal advance for a glyph.

	// general constants
	static const int	AutoGenerateHorzAdvance;		//!< Horizontal advance value that tells the parser to auto-calculate some reasonable value.

	/*************************************************************************
	Implementation Data
	*************************************************************************/
	Font* d_font;			//!< Font object that we are helping to build
};

} // End of  CEGUI namespace section

#endif
