/************************************************************************
filename: 	CEGUIFont.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements Font class
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
#include "CEGUIFont_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
static data definitions
*************************************************************************/

// XML related strings
const char	Font_xmlHandler::FontElement[]					= "Font";
const char	Font_xmlHandler::MappingElement[]				= "Mapping";
const char	Font_xmlHandler::FontNameAttribute[]			= "Name";
const char	Font_xmlHandler::FontFilenameAttribute[]		= "Filename";
const char	Font_xmlHandler::FontTypeAttribute[]			= "Type";
const char	Font_xmlHandler::FontSizeAttribute[]			= "Size";
const char	Font_xmlHandler::FontFirstCodepointAttribute[]	= "FirstCodepoint";
const char	Font_xmlHandler::FontLastCodepointAttribute[]	= "LastCodepoint";
const char	Font_xmlHandler::FontNativeHorzResAttribute[]	= "NativeHorzRes";
const char	Font_xmlHandler::FontNativeVertResAttribute[]	= "NativeVertRes";
const char	Font_xmlHandler::FontAutoScaledAttribute[]		= "AutoScaled";
const char	Font_xmlHandler::MappingCodepointAttribute[]	= "Codepoint";
const char	Font_xmlHandler::MappingImageAttribute[]		= "Image";
const char	Font_xmlHandler::MappingHorzAdvanceAttribute[]	= "HorzAdvance";
const char	Font_xmlHandler::FontTypeStatic[]				= "Static";
const char	Font_xmlHandler::FontTypeDynamic[]				= "Dynamic";

// General constants
const int	Font_xmlHandler::AutoGenerateHorzAdvance		= -1;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void Font_xmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
{
	XERCES_CPP_NAMESPACE_USE
		std::string element(XMLString::transcode(localname));

	// handle a Mapping element
	if ((element == MappingElement) && !d_font->d_freetype)
	{
		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(MappingImageAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		String	image_name((utf8*)val_str.get());

		attr_name.reset(XMLString::transcode(MappingCodepointAttribute));
		utf32 codepoint = (utf32)XMLString::parseInt(attrs.getValue(attr_name.get()));

		attr_name.reset(XMLString::transcode(MappingHorzAdvanceAttribute));
		int horzAdvance = XMLString::parseInt(attrs.getValue(attr_name.get()));

		Font::glyphDat	mapDat;
		mapDat.d_image = &d_font->d_glyph_images->getImage(image_name);

		// calculate advance width if it was not specified
		if (horzAdvance == AutoGenerateHorzAdvance)
		{
			horzAdvance = (int)(mapDat.d_image->getWidth() + mapDat.d_image->getOffsetX());
		}

		mapDat.d_horz_advance_unscaled = horzAdvance;
		mapDat.d_horz_advance = (uint)(((float)horzAdvance) * d_font->d_horzScaling);
		d_font->d_cp_map[codepoint] = mapDat;
	}
	// handle root Font element
	else if (element == FontElement)
	{
		// get name of font we are creating
		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(FontNameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		String font_name = (utf8*)val_str.get();

		// get filename for the font
		attr_name.reset(XMLString::transcode(FontFilenameAttribute));
		val_str.reset(XMLString::transcode(attrs.getValue(attr_name.get())));
		String filename((utf8*)val_str.get());

		//
		// load auto-scaling configuration
		//
		float hres, vres;
		bool auto_scale;

		// get native horizontal resolution
		attr_name.reset(XMLString::transcode(FontNativeHorzResAttribute));
		hres = (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		// get native vertical resolution
		attr_name.reset(XMLString::transcode(FontNativeVertResAttribute));
		vres = (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		// get auto-scaling setting
		attr_name.reset(XMLString::transcode(FontAutoScaledAttribute));
		val_str.reset(XMLString::transcode(attrs.getValue(attr_name.get())));
		std::string autoscaleval = val_str.get();

		auto_scale = ((autoscaleval == "true") || (autoscaleval == "1")) ? true : false;

		//
		// get type of font
		//
		attr_name.reset(XMLString::transcode(FontTypeAttribute));
		val_str.reset(XMLString::transcode(attrs.getValue(attr_name.get())));
		std::string	font_type = val_str.get();

		// dynamic (ttf) font
		if (font_type == FontTypeDynamic)
		{
			// get size of font
			attr_name.reset(XMLString::transcode(FontSizeAttribute));
			uint size = (uint)XMLString::parseInt(attrs.getValue(attr_name.get()));

			// extract codepoint range
			attr_name.reset(XMLString::transcode(FontFirstCodepointAttribute));
			utf32 first_codepoint = (utf32)XMLString::parseInt(attrs.getValue(attr_name.get()));
			attr_name.reset(XMLString::transcode(FontLastCodepointAttribute));
			utf32 last_codepoint = (utf32)XMLString::parseInt(attrs.getValue(attr_name.get()));

			// build string containing the required code-points.
			String glyph_set;
			for (;first_codepoint <= last_codepoint; ++first_codepoint)
				glyph_set += first_codepoint;

			// perform construction
			d_font->setNativeResolution(Size(hres, vres));
			d_font->setAutoScalingEnabled(auto_scale);
			d_font->constructor_impl(font_name, filename, size, 0, glyph_set);
		}
		// static (Imageset based) font
		else if (font_type == FontTypeStatic)
		{
			d_font->d_name = font_name;
			d_font->d_freetype = false;

			// load the Imageset
			d_font->d_glyph_images = ImagesetManager::getSingleton().createImageset(filename);

			d_font->setNativeResolution(Size(hres, vres));
			d_font->setAutoScalingEnabled(auto_scale);
		}
		// error (should never happen)
		else
		{
			throw FileIOException("Font::xmlHandler::startElement - The unknown Font:Type attribute value '" + font_type + "' was encountered while processing the Font file.");
		}

	}
	// anything else is an error which *should* have already been caught by XML validation
	else
	{
		throw FileIOException("Font::xmlHandler::startElement - Unexpected data was found while parsing the Font file: '" + element + "' is unknown.");
	}

}

void Font_xmlHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Font_xmlHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Font_xmlHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

} // End of  CEGUI namespace section