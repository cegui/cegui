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
#include "CEGUIFont.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUITexture.h"
#include "CEGUILogger.h"
#include "CEGUITextUtils.h"
#include "CEGUIFont_xmlHandler.h"
#include "CEGUIFont_implData.h"
#include "CEGUIResourceProvider.h"

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	static data definitions
*************************************************************************/
const ulong Font::DefaultColour					= 0xFFFFFFFF;
const uint	Font::InterGlyphPadSpace			= 2;

// XML related strings
const char	Font::FontSchemaName[]				= "Font.xsd";


/*************************************************************************
	Constructs a new Font object from a font definition file
*************************************************************************/
Font::Font(const String& filename, FontImplData* dat)
{
	d_antiAliased = false;
	d_impldat = dat;
	d_freetype = false;
	d_glyph_images = NULL;

	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, DefaultNativeVertRes));

	load(filename);

	// complete y spacing set-up for bitmap / static fonts
	calculateStaticVertSpacing();
}


/*************************************************************************
	Constructs a new Font object (via FreeType & a true-type font file)
	'glyph-set' describes the set of code points to be available via
	this font
*************************************************************************/
Font::Font(const String& name, const String& fontname, uint size, uint flags, const String& glyph_set, FontImplData* dat)
{
	d_impldat = dat;
	d_freetype = false;
	d_glyph_images = NULL;

	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, DefaultNativeVertRes));

	constructor_impl(name, fontname, size, flags, glyph_set);
}


/*************************************************************************
	Constructs a new Font object (via FreeType & a true-type font file)
	[first_code_point, last_code_point] describes the range of code
	points to be available via this font
*************************************************************************/
Font::Font(const String& name, const String& fontname, uint size, uint flags, utf32 first_code_point, utf32 last_code_point, FontImplData* dat)
{
	d_impldat = dat;
	d_freetype = false;
	d_glyph_images = NULL;

	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, DefaultNativeVertRes));

	String tmp;

	for (utf32 cp = first_code_point; cp <= last_code_point; ++cp)
	{
		tmp += cp;
	}

	constructor_impl(name, fontname, size, flags, tmp);
}


/*************************************************************************
	Constructs a new Font object (via FreeType & a true-type font file)
	The font file will provide support for 7-bit ASCII characters only
*************************************************************************/
Font::Font(const String& name, const String& fontname, uint size, uint flags, FontImplData* dat)
{
	d_impldat = dat;
	d_freetype = false;
	d_glyph_images = NULL;

	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, DefaultNativeVertRes));

	String tmp;

	for (utf32 cp = 32; cp <= 127; ++cp)
	{
		tmp += cp;
	}

	constructor_impl(name, fontname, size, flags, tmp);
}


/*************************************************************************
	Destroys a Font object
*************************************************************************/
Font::~Font(void)
{
	unload();
	delete d_impldat;
}


/*************************************************************************
	Return the pixel width of the specified text if rendered with this Font.
*************************************************************************/
float Font::getTextExtent(const String& text) const
{
	uint cur_extent = 0;

	uint char_count = text.length();
	CodepointMap::const_iterator	pos, end = d_cp_map.end();

	for (uint c = 0; c < char_count; ++c)
	{
		pos = d_cp_map.find(text[c]);

		if (pos != end)
		{
			cur_extent += pos->second.d_horz_advance;
		}

	}

	return (float)cur_extent;
}


/*************************************************************************
	Return the index of the closest text character in String 'text' that
	corresponds to pixel location 'pixel' if the text were rendered.
*************************************************************************/
uint Font::getCharAtPixel(const String& text, uint start_char, float pixel) const
{
	uint cur_extent = 0;
	uint char_count = text.length();

	// handle simple cases
	if ((pixel <= 0) || (char_count <= start_char))
	{
		return start_char;
	}

	CodepointMap::const_iterator	pos, end = d_cp_map.end();

	for (uint c = start_char; c < char_count; ++c)
	{
		pos = d_cp_map.find(text[c]);

		if (pos != end)
		{
			cur_extent += pos->second.d_horz_advance;

			if (pixel < cur_extent)
			{
				return c;
			}

		}

	}

	return char_count;
}


/*************************************************************************
	Renders text on the display.  Return number of lines output.
*************************************************************************/
uint Font::drawText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours) const
{
	uint thisCount;
	uint lineCount = 0;

	float	y_base = draw_area.d_top + d_max_bearingY;

	Rect tmpDrawArea(
		PixelAligned(draw_area.d_left),
		PixelAligned(draw_area.d_top),
		PixelAligned(draw_area.d_right),
		PixelAligned(draw_area.d_bottom)
		);

	uint lineStart = 0, lineEnd = 0;
	String	currLine;

	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
		{
			lineEnd = text.length();
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case LeftAligned:
			drawTextLine(currLine, Vector3(tmpDrawArea.d_left, y_base, z), clip_rect, colours);
			thisCount = 1;
			y_base += getLineSpacing();
			break;

		case RightAligned:
			drawTextLine(currLine, Vector3(tmpDrawArea.d_right - getTextExtent(currLine), y_base, z), clip_rect, colours);
			thisCount = 1;
			y_base += getLineSpacing();
			break;

		case Centred:
			drawTextLine(currLine, Vector3(PixelAligned(tmpDrawArea.d_left + ((tmpDrawArea.getWidth() - getTextExtent(currLine)) / 2.0f)), y_base, z), clip_rect, colours);
			thisCount = 1;
			y_base += getLineSpacing();
			break;

		case WordWrapLeftAligned:
			thisCount = drawWrappedText(currLine, tmpDrawArea, z, clip_rect, LeftAligned, colours);
			tmpDrawArea.d_top += thisCount * getLineSpacing();
			break;

		case WordWrapRightAligned:
			thisCount = drawWrappedText(currLine, tmpDrawArea, z, clip_rect, RightAligned, colours);
			tmpDrawArea.d_top += thisCount * getLineSpacing();
			break;

		case WordWrapCentred:
			thisCount = drawWrappedText(currLine, tmpDrawArea, z, clip_rect, Centred, colours);
			tmpDrawArea.d_top += thisCount * getLineSpacing();
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::drawText - Unknown or unsupported TextFormatting value specified.");
		}

		lineCount += thisCount;

	}

	return lineCount;
}


/*************************************************************************
	Define the set of glyphs available for this font
*************************************************************************/
void Font::defineFontGlyphs(const String& glyph_set)
{
	d_glyphset = glyph_set;
	defineFontGlyphs_impl();

	Logger::getSingleton().logEvent("Font '" + d_name + "' now has the following glyphs defined: '" + d_glyphset + "'.", Informative);
}


/*************************************************************************
	Return the required texture size required to store imagery for the
	glyphs specified in 'glyph_set'
*************************************************************************/
uint Font::getRequiredTextureSize(const String& glyph_set)
{
	d_maxGlyphHeight = 0;

	uint	texSize = 32;			// start with a texture this size
	uint	width;

	uint	cur_x = 0;
	uint	cur_y = d_maxGlyphHeight;

	uint	glyph_set_length = glyph_set.length();

	for (uint i = 0; i < glyph_set_length; ++i)
	{
		// load-up required glyph
		if (FT_Load_Char(d_impldat->fontFace, glyph_set[i], FT_LOAD_RENDER|(d_antiAliased ? 0 : FT_LOAD_MONOCHROME)))
		{
			// skip errors
			continue;
		}

		// if this glyph is taller than all others so far, update height and re-calculate cur_y
		if ((uint)d_impldat->fontFace->glyph->bitmap.rows + InterGlyphPadSpace > d_maxGlyphHeight)
		{
			d_maxGlyphHeight = d_impldat->fontFace->glyph->bitmap.rows + InterGlyphPadSpace;
			cur_y = (i + 1) * d_maxGlyphHeight;
		}

		width = d_impldat->fontFace->glyph->bitmap.width + InterGlyphPadSpace;
		cur_x += width;

		// check for fit
		if (cur_x >= texSize)
		{
			cur_x = width;
			cur_y += d_maxGlyphHeight;

			if (cur_y >= texSize)
			{
				// texture is too small, set-up to start again...
				texSize *= 2;
				cur_x = 0;
				cur_y = d_maxGlyphHeight;
				i = (uint)-1;
			}

		}

	}

	return texSize;
}


/*************************************************************************
	Render a set of glyph images into the given memory buffer.
	pixels will be in A8R8G8B8 format
*************************************************************************/
void Font::createFontGlyphSet(const String& glyph_set, uint size, ulong* buffer)
{
	String	imageName;
	Rect	rect;
	Point	offset;

	FT_GlyphSlot glyph = d_impldat->fontFace->glyph;

	d_max_bearingY = 0;

	uint	glyph_set_length = glyph_set.length();
	uint	cur_x = 0;
	uint	cur_y = 0;
	uint	width;

	for (uint i = 0; i < glyph_set_length; ++i)
	{
		// load-up required glyph
		if (FT_Load_Char(d_impldat->fontFace, glyph_set[i], FT_LOAD_RENDER|(d_antiAliased ? 0 : FT_LOAD_MONOCHROME)))
		{
			// skip errors
			continue;
		}

		width = glyph->bitmap.width + InterGlyphPadSpace;

		// see if we need to wrap to next row
		if (cur_x + width >= size)
		{
			cur_x = 0;
			cur_y += d_maxGlyphHeight;
		}

		// calculate offset into buffer for this glyph
		ulong* dest_buff = buffer + (cur_y * size) + cur_x;

		// draw glyph into buffer
		drawGlyphToBuffer(dest_buff, size);

		// define Image on Imageset for this glyph to save re-rendering glyph later
		imageName		= glyph_set[i];
		rect.d_left		= (float)cur_x;
		rect.d_top		= (float)cur_y;
		rect.d_right	= (float)(cur_x + width - InterGlyphPadSpace);
		rect.d_bottom	= (float)(cur_y + d_maxGlyphHeight - InterGlyphPadSpace);
		offset.d_x		= (float)(glyph->metrics.horiBearingX >> 6);
		offset.d_y		= -(float)(glyph->metrics.horiBearingY >> 6);

		d_glyph_images->defineImage(imageName, rect, offset);

		cur_x += width;

		// check and update maximum bearingY value
		if ((glyph->metrics.horiBearingY >> 6) > d_max_bearingY)
		{
			d_max_bearingY = glyph->metrics.horiBearingY >> 6;
		}

		// create entry in code-point to Image map
		glyphDat	dat;
		dat.d_image = &d_glyph_images->getImage(imageName);
		dat.d_horz_advance = glyph->advance.x >> 6;
		d_cp_map[glyph_set[i]] = dat;
	}

}


/*************************************************************************
	Render a range of glyph images into the given memory buffer.
	pixels will be in A8R8G8B8 format
*************************************************************************/
void Font::createFontGlyphSet(utf32 first_code_point, utf32 last_code_point, uint size, ulong* buffer)
{
	String tmp;

	for (utf32 cp = first_code_point; cp <=last_code_point; ++cp)
	{
		tmp += cp;
	}

	createFontGlyphSet(tmp, size, buffer);
}


/*************************************************************************
	Return the required texture size required to store imagery for the
	glyphs specified in by the inclusive range
	[first_code_point, last_code_point]
*************************************************************************/
uint Font::getRequiredTextureSize(utf32 first_code_point, utf32 last_code_point)
{
	String tmp;

	for (utf32 cp = first_code_point; cp <=last_code_point; ++cp)
	{
		tmp += cp;
	}

	return getRequiredTextureSize(tmp);
}


/*************************************************************************
	Define the range of glyphs available for this font
*************************************************************************/
void Font::defineFontGlyphs(utf32 first_code_point, utf32 last_code_point)
{
	String tmp;

	for (utf32 cp = first_code_point; cp <=last_code_point; ++cp)
	{
		tmp += cp;
	}

	defineFontGlyphs(tmp);
}


/*************************************************************************
	Copy the FreeType glyph bitmap into the given memory buffer
*************************************************************************/
void Font::drawGlyphToBuffer(ulong* buffer, uint buf_width)
{
	FT_Bitmap* glyph_bitmap = &d_impldat->fontFace->glyph->bitmap;

	for (int i = 0; i < glyph_bitmap->rows; ++i)
	{
		for (int j = 0; j < glyph_bitmap->width; ++j)
		{
			switch (glyph_bitmap->pixel_mode)
			{
			case FT_PIXEL_MODE_GRAY:
				{
					uchar* bytebuff = reinterpret_cast<uchar*>(&buffer[j]);
					*bytebuff++ = 0xFF;
					*bytebuff++ = 0xFF;
					*bytebuff++ = 0xFF;
					*bytebuff = glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j];
				}
				break;

			case FT_PIXEL_MODE_MONO:
				buffer[j] = ((glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j / 8] << (j % 8)) & 0x80) ? 0xFFFFFFFF : 0x00000000;
				break;

			default:
				throw InvalidRequestException((utf8*)"Font::drawGlyphToBuffer - The glyph could not be drawn because the pixel mode is unsupported.");
				break;
			}

		}

		buffer += buf_width;
	}

}


/*************************************************************************
	draws wrapped text
*************************************************************************/
uint Font::drawWrappedText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours) const
{
	uint	line_count = 0;
	Rect	dest_area(draw_area);
	float	wrap_width = draw_area.getWidth();

	String  whitespace = TextUtils::DefaultWhitespace;
	String	thisLine, thisWord;
	uint	currpos = 0;

	// get first word.
	currpos += getNextWord(text, currpos, thisLine);

	// while there are words left in the string...
	while (String::npos != text.find_first_not_of(whitespace, currpos)) {
		// get next word of the string...
		currpos += getNextWord(text, currpos, thisWord);

		// if the new word would make the string too long
		if ((getTextExtent(thisLine) + getTextExtent(thisWord)) > wrap_width) {
			// output what we had until this new word
			line_count += drawText(thisLine, dest_area, z, clip_rect, fmt, colours);

			// remove whitespace from next word - it will form start of next line
			thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));

			// reset for a new line.
			thisLine.clear();

			// update y co-ordinate for next line
			dest_area.d_top += getLineSpacing();
		}

		// add the next word to the line
		thisLine += thisWord;
	}

	// output last bit of string
	line_count += drawText(thisLine, dest_area, z, clip_rect, fmt, colours);

	return line_count;
}


/*************************************************************************
	helper function for renderWrappedText to get next word of a string
*************************************************************************/
uint Font::getNextWord(const String& in_string, uint start_idx, String& out_string) const
{
	out_string = TextUtils::getNextWord(in_string, start_idx, TextUtils::DefaultWrapDelimiters);

	return out_string.length();
}


/*************************************************************************
	Draw a line of text.  No formatting is applied.
*************************************************************************/
void Font::drawTextLine(const String& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours) const
{
	Vector3	cur_pos(position);

	uint char_count = text.length();
	CodepointMap::const_iterator	pos, end = d_cp_map.end();

	for (uint c = 0; c < char_count; ++c)
	{
		pos = d_cp_map.find(text[c]);

		if (pos != end)
		{
			pos->second.d_image->draw(cur_pos, clip_rect, colours);
			cur_pos.d_x += pos->second.d_horz_advance;
		}

	}
}


/*************************************************************************
	Function to do real work of constructor
*************************************************************************/
void Font::constructor_impl(const String& name, const String& fontname, uint size, uint flags, const String& glyph_set)
{
	FontManager&	 fman	= FontManager::getSingleton();
	ImagesetManager& ismgr	= ImagesetManager::getSingleton();

	// pull a-a setting from flags
	d_antiAliased = (flags == NoAntiAlias) ? false : true;

	// create an blank Imageset
	d_glyph_images = ismgr.createImageset(name + "_auto_glyph_images", System::getSingleton().getRenderer()->createTexture());

	uint		horzdpi		= System::getSingleton().getRenderer()->getHorzScreenDPI();
	uint		vertdpi		= System::getSingleton().getRenderer()->getVertScreenDPI();
	String		errMsg;

    System::getSingleton().getResourceProvider()->loadRawDataContainer(fontname, d_impldat->fontData);

	// create face using input font
	if (FT_New_Memory_Face(d_impldat->library, d_impldat->fontData.getDataPtr(), 
                (FT_Long)d_impldat->fontData.getSize(), 0, &d_impldat->fontFace) == 0)
	{
		// check that default Unicode character map is available
		if (d_impldat->fontFace->charmap != NULL)	
		{
			try
			{
				d_glyphset = glyph_set;
				d_name = name;
				d_freetype = true;
				createFontFromFT_Face(size, horzdpi, vertdpi);
				return;
			}
			catch(...)
			{
				ismgr.destroyImageset(d_glyph_images);
				d_glyph_images = NULL;

				FT_Done_Face(d_impldat->fontFace);
				d_freetype = false;

				// re-throw
				throw;
			}

		}
		// missing Unicode character map
		else
		{
			FT_Done_Face(d_impldat->fontFace);
			d_freetype = false;

			errMsg = (utf8*)"Font::constructor_impl - The source font '" + fontname +"' does not have a Unicode charmap, and cannot be used.";
		}

	}
	// failed to create face (a problem with the font file?)
	else
	{
		errMsg = (utf8*)"Font::constructor_impl - An error occurred while trying to create a FreeType face from source font '" + fontname + "'.";
	}

	// cleanup image set we created here
	ismgr.destroyImageset(d_glyph_images);

	throw GenericException(errMsg);
}


/*************************************************************************
	Load and complete construction of 'this' via an XML file
*************************************************************************/
void Font::load(const String& filename)
{
	XERCES_CPP_NAMESPACE_USE

	// unload old data
	unload();

	if (filename.empty() || (filename == (utf8*)""))
	{
		throw InvalidRequestException((utf8*)"Font::load - Filename supplied for Font loading must be valid");
	}

	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

	// set basic settings we want from parser
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
	parser->setFeature(XMLUni::fgXercesSchema, true);
	parser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

    InputSourceContainer fontSchemaData;
    System::getSingleton().getResourceProvider()->loadInputSourceContainer(FontSchemaName, fontSchemaData);
    parser->loadGrammar(*(fontSchemaData.getDataPtr()), Grammar::SchemaGrammarType, true);
    // enable grammar reuse
    parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);

	// setup schema for Font data
	XMLCh* pval = XMLString::transcode(FontSchemaName);
	parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);
	XMLString::release(&pval);

	// setup handler object
	Font_xmlHandler handler(this);
	parser->setContentHandler(&handler);
	parser->setErrorHandler(&handler);

    InputSourceContainer fontData;
    System::getSingleton().getResourceProvider()->loadInputSourceContainer(filename, fontData);

	// do parse (which uses handler to create actual data)
	try
	{
		parser->parse(*(fontData.getDataPtr()));
	}
	catch(const XMLException& exc)
	{
		if (exc.getCode() != XMLExcepts::NoError)
		{
			unload();
			delete parser;

			char* excmsg = XMLString::transcode(exc.getMessage());
			String message((utf8*)"Font::load - An error occurred while parsing Font file '" + filename + "'.  Additional information: ");
			message += excmsg;
			XMLString::release(&excmsg);

			throw FileIOException(message);
		}

	}
	catch(const SAXParseException& exc)
	{
		unload();
		delete parser;

		char* excmsg = XMLString::transcode(exc.getMessage());
		String message((utf8*)"Font::load - An error occurred while parsing Font file '" + filename + "'.  Additional information: ");
		message += excmsg;
		XMLString::release(&excmsg);

		throw FileIOException(message);
	}
	catch(...)
	{
		unload();
		delete parser;

		throw FileIOException((utf8*)"Font::load - An unexpected error occurred while parsing Font file '" + filename + "'.");
	}

	// cleanup
	delete parser;
}


/*************************************************************************
	Unload data associated with the font (font is then useless.
	this is intended for cleanup).
*************************************************************************/
void Font::unload(void)
{
	d_cp_map.clear();

	// cleanup Imageset if it's valid
	if (d_glyph_images != NULL)
	{
		ImagesetManager::getSingleton().destroyImageset(d_glyph_images);
		d_glyph_images = NULL;
	}

	// cleanup FreeType face if this is a FreeType based font.
	if (d_freetype)
	{
		FT_Done_Face(d_impldat->fontFace);
		d_freetype = false;
	}

}


/*************************************************************************
	Defines the set of code points on the font. (implementation).
*************************************************************************/
void Font::defineFontGlyphs_impl(void)
{
	// must be a font useing the FreeType system
	if (!d_freetype)
	{
		throw InvalidRequestException((utf8*)"Font::defineFontGlyphs_impl - operation not supported on bitmap based fonts.");
	}

	uint texture_size = getRequiredTextureSize(d_glyphset);

	// check renderer can do a texture big enough
	if (texture_size > System::getSingleton().getRenderer()->getMaxTextureSize())
	{
		throw	RendererException((utf8*)"Font::defineFontGlyphs_impl - operation requires a texture larger than the supported maximum.");
	}

	// allocate memory buffer where we will define the imagery
	ulong* mem_buffer;

	try
	{
		mem_buffer = new ulong[texture_size * texture_size];
	}
	catch (std::bad_alloc)
	{
		throw	MemoryException((utf8*)"Font::defineFontGlyphs_impl - failed to allocate required memory buffer.");
	}

	// initialise background to transparent black.
	memset(mem_buffer, 0, ((texture_size * texture_size) * sizeof(ulong)));

	// clear old data about glyphs and images
	d_cp_map.clear();
	d_glyph_images->undefineAllImages();

	// render new glyphs and define Imageset images.
	createFontGlyphSet(d_glyphset, texture_size, mem_buffer);

	// update Imageset texture with new imagery.
	d_glyph_images->getTexture()->loadFromMemory(mem_buffer, texture_size, texture_size);

	delete[] mem_buffer;
}


/*************************************************************************
	Calculate the vertical spacing fields for a static / bitmap font
*************************************************************************/
void Font::calculateStaticVertSpacing(void)
{
	if (!d_freetype)
	{
		float scale = d_autoScale ? d_vertScaling : 1.0f;

		d_y_spacing		= 0;
		d_max_bearingY	= 0;

		CodepointMap::iterator pos = d_cp_map.begin(), end = d_cp_map.end();

		for (;pos != end; ++pos)
		{
			const Image* img = pos->second.d_image;

			if (img->getOffsetY() > d_max_bearingY)
				d_max_bearingY = (int)img->getOffsetY();

			if (img->getHeight() > d_y_spacing)
				d_y_spacing = img->getHeight();
		}

		d_y_spacing *= scale;
		d_max_bearingY = (int)(((float)d_max_bearingY) * scale);
	}

}


/*************************************************************************
	Set the native resolution for this Font
*************************************************************************/
void Font::setNativeResolution(const Size& size)
{
	d_nativeHorzRes = size.d_width;
	d_nativeVertRes = size.d_height;

	// set native resolution for underlying imageset for bitmap fonts
	if ((!d_freetype) && (d_glyph_images != NULL))
	{
		d_glyph_images->setNativeResolution(size);
	}

	// re-calculate scaling factors & notify images as required
	notifyScreenResolution(System::getSingleton().getRenderer()->getSize());
}


/*************************************************************************
	Notify the Font of the current (usually new) display resolution.
*************************************************************************/
void Font::notifyScreenResolution(const Size& size)
{
	// notification should come from System which would have notified the Imageset anyway, but
	// in case client code decides to call us, we must pass on notification here too.
	if (d_glyph_images)
	{
		d_glyph_images->notifyScreenResolution(size);
	}

	d_horzScaling = size.d_width / d_nativeHorzRes;
	d_vertScaling = size.d_height / d_nativeVertRes;

	if (d_autoScale)
	{
		updateFontScaling();
	}

}


/*************************************************************************
	Enable or disable auto-scaling for this Font.
*************************************************************************/
void Font::setAutoScalingEnabled(bool setting)
{
	if (setting != d_autoScale)
	{
		if ((!d_freetype) && (d_glyph_images != NULL))
		{
			d_glyph_images->setAutoScalingEnabled(setting);
		}

		d_autoScale = setting;
		updateFontScaling();
	}

}


/*************************************************************************
	Update the font as required according to the current scaling
*************************************************************************/
void Font::updateFontScaling(void)
{
	if (d_freetype)
	{
		uint hdpi = System::getSingleton().getRenderer()->getHorzScreenDPI();
		uint vdpi = System::getSingleton().getRenderer()->getVertScreenDPI();

		createFontFromFT_Face(d_ptSize, hdpi, vdpi);
	}
	// bitmapped font
	else
	{
		float hscale = d_autoScale ? d_horzScaling : 1.0f;

		// perform update on font mapping advance values
		CodepointMap::iterator pos = d_cp_map.begin(), end = d_cp_map.end();
		for (; pos != end; ++pos)
		{
			pos->second.d_horz_advance = (uint)(((float)pos->second.d_horz_advance_unscaled) * hscale);
		}

		// re-calculate height
		calculateStaticVertSpacing();
	}

}


/*************************************************************************
	Set the size of the free-type font (via d_impldat->fontFace which should already
	be setup) and render the glyphs in d_glyphset.
*************************************************************************/
void Font::createFontFromFT_Face(uint size, uint horzDpi, uint vertDpi)
{
	if (d_autoScale)
	{
		horzDpi = (uint)(((float)horzDpi) * d_horzScaling);
		vertDpi = (uint)(((float)vertDpi) * d_vertScaling);
	}

	d_ptSize = size;

	if (FT_Set_Char_Size(d_impldat->fontFace, 0, d_ptSize * 64, horzDpi, vertDpi) == 0)
	{
		d_y_spacing = (float)(d_impldat->fontFace->height / d_impldat->fontFace->units_per_EM) * d_impldat->fontFace->size->metrics.y_ppem;
		defineFontGlyphs_impl();
	}
	// failed to set size for font
	else
	{
		throw GenericException((utf8*)"Font::createFontFromFT_Face - An error occurred while creating a source font with the requested size.");
	}

}


/*************************************************************************
	Return the number of lines the given text would be formatted to.	
*************************************************************************/
uint Font::getFormattedLineCount(const String& text, const Rect& format_area, TextFormatting fmt) const
{
	// handle simple non-wrapped cases.
	if ((fmt == LeftAligned) || (fmt == Centred) || (fmt == RightAligned))
	{
		return std::count(text.begin(), text.end(), '\n') + 1;
	}

	// handle wraping cases
	uint lineStart = 0, lineEnd = 0;
	String	sourceLine;

	float	wrap_width = format_area.getWidth();
	String  whitespace = TextUtils::DefaultWhitespace;
	String	thisLine, thisWord;
	uint	line_count = 0, currpos = 0;

	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
		{
			lineEnd = text.length();
		}

		sourceLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;

		// get first word.
		currpos = getNextWord(sourceLine, 0, thisLine);

		// while there are words left in the string...
		while (String::npos != sourceLine.find_first_not_of(whitespace, currpos))
		{
			// get next word of the string...
			currpos += getNextWord(sourceLine, currpos, thisWord);

			// if the new word would make the string too long
			if ((getTextExtent(thisLine) + getTextExtent(thisWord)) > wrap_width)
			{
				// too long, so that's another line of text
				line_count++;

				// remove whitespace from next word - it will form start of next line
				thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));

				// reset for a new line.
				thisLine.clear();
			}

			// add the next word to the line
			thisLine += thisWord;
		}

		// plus one for final line
		line_count++;
	}

	return line_count;
}


/*************************************************************************
	Return whether this font is anti-aliased or not.
*************************************************************************/
bool Font::isAntiAliased(void) const
{
	return d_freetype ? d_antiAliased : false;
}


/*************************************************************************
	Set whether the font is anti-aliased or not.
*************************************************************************/
void Font::setAntiAliased(bool setting)
{
	if (d_freetype && (d_antiAliased != setting))
	{
		d_antiAliased = setting;

		// regenerate font
		createFontFromFT_Face(d_ptSize, System::getSingleton().getRenderer()->getHorzScreenDPI(), System::getSingleton().getRenderer()->getVertScreenDPI());
	}

}


/*************************************************************************
	Return the horizontal pixel extent given text would be formatted to.	
*************************************************************************/
float Font::getFormattedTextExtent(const String& text, const Rect& format_area, TextFormatting fmt) const
{
	float lineWidth;
	float widest = 0;

	uint lineStart = 0, lineEnd = 0;
	String	currLine;

	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
		{
			lineEnd = text.length();
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case Centred:
		case RightAligned:
		case LeftAligned:
			lineWidth = getTextExtent(currLine);
			break;

		case WordWrapLeftAligned:
		case WordWrapRightAligned:
		case WordWrapCentred:
			lineWidth = getWrappedTextExtent(currLine, format_area.getWidth());
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::getFormattedTextExtent - Unknown or unsupported TextFormatting value specified.");
		}

		if (lineWidth > widest)
		{
			widest = lineWidth;
		}

	}

	return widest;
}


/*************************************************************************
	returns extent of widest line of wrapped text.
*************************************************************************/
float Font::getWrappedTextExtent(const String& text, float wrapWidth) const
{
	String  whitespace = TextUtils::DefaultWhitespace;
	String	thisWord;
	uint	currpos;
	float	lineWidth, wordWidth;
	float	widest = 0;

	// get first word.
	currpos = getNextWord(text, 0, thisWord);
	lineWidth = getTextExtent(thisWord);

	// while there are words left in the string...
	while (String::npos != text.find_first_not_of(whitespace, currpos)) {
		// get next word of the string...
		currpos += getNextWord(text, currpos, thisWord);
		wordWidth = getTextExtent(thisWord);

		// if the new word would make the string too long
		if ((lineWidth + wordWidth) > wrapWidth) {
			
			if (lineWidth > widest)
			{
				widest = lineWidth;
			}

			// remove whitespace from next word - it will form start of next line
			thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));
			wordWidth = getTextExtent(thisWord);

			// reset for a new line.
			lineWidth = 0;
		}

		// add the next word to the line
		lineWidth += wordWidth;
	}

	if (lineWidth > widest)
	{
		widest = lineWidth;
	}

	return widest;
}


/*************************************************************************
	Return a String object that contains the code-points that the font
	is currently configured to render.
*************************************************************************/
const String& Font::getAvailableGlyphs(void) const
{
	return d_glyphset;
}

} // End of  CEGUI namespace section
