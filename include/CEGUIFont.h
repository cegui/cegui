/************************************************************************
	filename: 	CEGUIFont.h
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
#ifndef _CEGUIFont_h_
#define _CEGUIFont_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIVector.h"
#include "CEGUIColourRect.h"
#include "CEGUIFontManager.h"

#include "xercesc/sax2/DefaultHandler.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

// Start of CEGUI namespace section
namespace CEGUI
{


/*!
\brief
	Enumerated type that contains the valid flags that can be passed to createFont when creating a new font.
*/
enum CEGUIBASE_API FontFlag
{
	Default,			//!< Default / None.
};


/*!
\brief
	Enumerated type that contains valid formatting types that can be specified when rendering text into a Rect area (the formatting Rect).
*/
enum CEGUIBASE_API TextFormatting
{
	LeftAligned,			//!< All text is printed on a single line.  The left-most character is aligned with the left edge of the formatting Rect.
	RightAligned,			//!< All text is printed on a single line.  The right-most character is aligned with the right edge of the formatting Rect.
	Centred,				//!< All text is printed on a single line.  The text is centred horizontally in the formatting Rect.
	WordWrapLeftAligned,	//!< Text is broken into multiple lines no wider than the formatting Rect.  The left-most character of each line is aligned with the left edge of the formatting Rect.
	WordWrapRightAligned,	//!< Text is broken into multiple lines no wider than the formatting Rect.  The right-most character of each line is aligned with the right edge of the formatting Rect.
	WordWrapCentred,		//!< Text is broken into multiple lines no wider than the formatting Rect.  Each line is centred horizontally in the formatting Rect.
};

/*!
\brief
	Class that encapsulates text rendering functionality for a typeface

	A Font object is created for each unique typeface required.  The Font class provides
	methods for loading typefaces from various sources, and then for outputting text via
	the Renderer object.
*/
class CEGUIBASE_API Font
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	static const ulong		DefaultColour;			//!< Colour value used whenever a colour is not specified.


	/*************************************************************************
		Text drawing methods
	*************************************************************************/
	/*!
	\brief
		Draw text into a specified area of the display.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is not clipped to this Rect, but is formatted
		using this Rect depending upon the option specified in \a fmt.

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param colours
		ColourRect object describing the colours to be applied when drawing the text.  NB: The colours specified in here are applied to each glyph,
		rather than the text as a whole.

	\return
		The number of lines output.  NB: This does not consider clipping, so if all text was clipped, this would still return >=1.
	*/
	uint	drawText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours) const;


	/*!
	\brief
		Draw text into a specified area of the display using default colours.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is not clipped to this Rect, but is formatted
		using this Rect depending upon the option specified in \a fmt.

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\return
		The number of lines output.  NB: This does not consider clipping, so if all text was clipped, this would still return >=1.
	*/
	uint	drawText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt) const
	{ return drawText(text, draw_area, z, clip_rect, fmt, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour)); }


	/*!
	\brief
		Draw text into a specified area of the display with default colours and default formatting (LeftAligned).

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is not clipped to this Rect, but is formatted
		using this Rect depending upon the option specified in \a fmt.

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\return
		Nothing.
	*/
	void	drawText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect) const
	{ drawText(text, draw_area, z, clip_rect, LeftAligned, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour)); }


	/*!
	\brief
		Draw text into a specified area of the display.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is formatted using this Rect depending
		upon the option specified in \a fmt.  Additionally, the drawn text is clipped to be within this Rect (applies to non-word wrapped formatting
		where the text may otherwise have fallen outside this Rect).

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param colours
		ColourRect object describing the colours to be applied when drawing the text.  NB: The colours specified in here are applied to each glyph,
		rather than the text as a whole.

	\return
		The number of lines output.  NB: This does not consider clipping, so if all text was clipped, this would still return >=1.
	*/
	uint	drawText(const String& text, const Rect& draw_area, float z, TextFormatting fmt, const ColourRect& colours) const
	{ return drawText(text, draw_area, z, draw_area, fmt, colours); }


	/*!
	\brief
		Draw text into a specified area of the display with default colours.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is formatted using this Rect depending
		upon the option specified in \a fmt.  Additionally, the drawn text is clipped to be within this Rect (applies to non-word wrapped formatting
		where the text may otherwise have fallen outside this Rect).

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\return
		The number of lines output.  NB: This does not consider clipping, so if all text was clipped, this would still return >=1.
	*/
	uint	drawText(const String& text, const Rect& draw_area, float z, TextFormatting fmt) const
	{ return drawText(text, draw_area, z, draw_area, fmt, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour)); }


	/*!
	\brief
		Draw text into a specified area of the display with default colours and default formatting (LeftAligned).

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is formatted using this Rect depending
		upon the option specified in \a fmt.  Additionally, the drawn text is clipped to be within this Rect (applies to non-word wrapped formatting
		where the text may otherwise have fallen outside this Rect).

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\return
		Nothing.
	*/
	void	drawText(const String& text, const Rect& draw_area, float z) const
	{ drawText(text, draw_area, z, draw_area, LeftAligned, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour)); }


	/*!
	\brief
		Draw text at the specified location.

	\param text
		String object containing the text to be drawn.

	\param position
		Vector3 object describing the location for the text.  NB: The position specified here corresponds to the text baseline and not the
		top of any glyph.  The baseline spacing required can be retrieved by calling getBaseline().
		
	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param colours
		ColourRect object describing the colours to be applied when drawing the text.  NB: The colours specified in here are applied to each glyph,
		rather than the text as a whole.

	\return
		Nothing.
	*/
	void	drawText(const String& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours) const
	{ drawText(text, Rect(position.d_x, position.d_y, position.d_x, position.d_y), position.d_z, clip_rect, LeftAligned, colours); }


	/*!
	\brief
		Draw text at the specified location with default colours.

	\param text
		String object containing the text to be drawn.

	\param position
		Vector3 object describing the location for the text.  NB: The position specified here corresponds to the text baseline and not the
		top of any glyph.  The baseline spacing required can be retrieved by calling getBaseline().
		
	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\return
		Nothing.
	*/
	void	drawText(const String& text, const Vector3& position, const Rect& clip_rect) const
	{ drawText(text, Rect(position.d_x, position.d_y, position.d_x, position.d_y), position.d_z, clip_rect, LeftAligned, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour)); }


	/*************************************************************************
		Methods to define available glyphs (truetype fonts only)
	*************************************************************************/
	/*!
	\brief
		Define the set of code points to be renderable by the font.

	\note
		This function can take some time to execute depending upon the size of the code point set, and the size of the
		font being operated upon.

	\param glyph_set
		String object describing all the code points that will be renderable by this font

	\return
		Nothing

	\exception	InvalidRequestException		thrown if the font is based on a bitmap rather than a true-type font.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	void	defineFontGlyphs(const String& glyph_set);


	/*!
	\brief
		Define the range of code points to be renderable by the font.

	\note
		This function can take some time to execute depending upon the size of the code point set, and the size of the
		font being operated upon.

	\note
		The code point arguments must satisfy the following:  \a first_code_point <= \a last_code_point, otherwise results are undefined

	\param first_code_point
		utf32 value describing the first code point that will be renderable by this font.

	\param last_code_point
		utf32 value describing the last code point that will be renderable by this font.

	\return
		Nothing

	\exception	InvalidRequestException		thrown if the font is based on a bitmap rather than a true-type font.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	void	defineFontGlyphs(utf32 first_code_point, utf32 last_code_point);


	/*!
	\brief
		Set the native resolution for this Font

	\param size
		Size object describing the new native screen resolution for this Font.

	\return
		Nothing
	*/
	void	setNativeResolution(const Size& size);


	/*!
	\brief
		Notify the Font of the current (usually new) display resolution.

	\param size
		Size object describing the display resolution

	\return
		Nothing
	*/
	void	notifyScreenResolution(const Size& size);


	/*!
	\brief
		Enable or disable auto-scaling for this Font.

	\param setting
		true to enable auto-scaling, false to disable auto-scaling.

	\return
		Nothing.
	*/
	void	setAutoScalingEnabled(bool setting);


	/*************************************************************************
		Informational methods
	*************************************************************************/
	/*!
	\brief
		Return the pixel width of the specified text if rendered with this Font.

	\param text
		String object containing the text to return the rendered pixel width for.

	\return
		Number of pixels that \a text will occupy when rendered with this Font.
	*/
	float	getTextExtent(const String& text) const;


	/*!
	\brief
		Return the pixel height for this Font.  This value is to be used for line spacing.

	\return
		Number of pixels between vertical base lines, i.e. The minimum pixel space between two lines of text.
	*/
	float	getLineSpacing(void) const		{return d_y_spacing;}


	/*!
	\brief
		Return the number of pixels from the top of the highest glyph to the baseline

	\return
		pixel spacing from top of front glyphs to baseline
	*/
	float	getBaseline(void) const			{return (float)d_max_bearingY;}


	/*!
	\brief
		Return the index of the closest text character in String \a text that corresponds to pixel location \a pixel if the text were rendered.

	\param text
		String object containing the text.

	\param pixel
		Specifies the (horizontal) pixel offset to return the character index for.

	\return
		Returns a character index into String \a text for the character that would be rendered closest to horizontal pixel offset \a pixel if the
		text were to be rendered via this Font.  Range of the return is from 0 to text.length(), so may actually return an index past the end of
		the string, which indicates \a pixel was beyond the last character.
	*/
	uint	getCharAtPixel(const String& text, float pixel) const		{return getCharAtPixel(text, 0, pixel);}


	/*!
	\brief
		Return the index of the closest text character in String \a text, starting at character index \a start_char, that corresponds
		to pixel location \a pixel if the text were to be rendered.

	\param text
		String object containing the text.

	\param start_char
		index of the first character to consider.  This is the lowest value that will be returned from the call.

	\param pixel
		Specifies the (horizontal) pixel offset to return the character index for.

	\return
		Returns a character index into String \a text for the character that would be rendered closest to horizontal pixel offset \a pixel if the
		text were to be rendered via this Font.  Range of the return is from 0 to text.length(), so may actually return an index past the end of
		the string, which indicates \a pixel was beyond the last character.
	*/
	uint	getCharAtPixel(const String& text, uint start_char, float pixel) const;


	/*!
	\brief
		Return the name of this font.

	\return
		String object holding the name of this font.
	*/
	const String&	getName(void) const		{return	d_name;}


	/*!
	\brief
		Return the native display size for this Font.  This is only relevant if the Font is being auto-scaled.

	\return
		Size object describing the native display size for this Font.
	*/
	Size	getNativeResolution(void) const	{return Size(d_nativeHorzRes, d_nativeVertRes);}


	/*!
	\brief
		Return whether this Font is auto-scaled.

	\return
		true if the Font is auto-scaled, false if not.
	*/
	bool	isAutoScaled(void) const		{return d_autoScale;}


private:
	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const char	FontSchemaName[];			//!< Filename of the XML schema used for validating Font files.


	/*************************************************************************
		Friends so that only FontManager can create and destroy font objects
	*************************************************************************/
	friend	Font* FontManager::createFont(const String& filename);
	friend	Font* FontManager::createFont(const String& name, const String& fontname, uint size, uint flags);
	friend	void FontManager::destroyFont(const String& name);


	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	/*!
	\brief
		Constructs a new Font object

	\param filename
		The filename of the "font definition file" to be used in creating this font.

	\exception	FileIOException				thrown if there was some problem accessing or parsing the file \a filename
	\exception	InvalidRequestException		thrown if an invalid filename was provided.
	\exception	AlreadyExistsException		thrown if a Font Imageset clashes with one already defined in the system.
	\exception	GenericException			thrown if something goes wrong while accessing a true-type font referenced in file \a filename.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	Font(const String& filename);


	/*!
	\brief
		Constructs a new Font object, with 7-bit ASCII glyphs

	\param name
		The unique name that will be used to identify this Font.

	\param fontname
		filename of the true-type font to be loaded.

	\param size
		Point size of the new font.

	\param flags
		Combination of the FontFlag enumerated values specifying required options for creating this Font.

	\exception	AlreadyExistsException		thrown if an auto-generated Imageset clashes with one already defined in the system.
	\exception	GenericException			thrown if something goes wrong while accessing the true-type font \a fontname.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	Font(const String& name, const String& fontname, uint size, uint flags);


	/*!
	\brief
		Constructs a new Font object, with the specified set of glyphs

	\param name
		The unique name that will be used to identify this Font.

	\param fontname
		filename of the true-type font to be loaded.

	\param size
		Point size of the new font.

	\param flags
		Combination of the FontFlag enumerated values specifying required options for creating this Font.

	\param glyph_set
		String containing the set of glyphs to have available in this font.

	\exception	AlreadyExistsException		thrown if an auto-generated Imageset clashes with one already defined in the system.
	\exception	GenericException			thrown if something goes wrong while accessing the true-type font \a fontname.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	Font(const String& name, const String& fontname, uint size, uint flags, const String& glyph_set);


	/*!
	\brief
		Constructs a new Font object, with a specified range of glyphs

	\param name
		The unique name that will be used to identify this Font.

	\param fontname
		filename of the true-type font to be loaded.

	\param size
		Point size of the new font.

	\param flags
		Combination of the FontFlag enumerated values specifying required options for creating this Font.

	\param first_code_point
		Specifies the utf32 code point of the first glyph to be available on this font

	\param last_code_point
		Specifies the utf32 code point of the last glyph to be available on this font

	\exception	AlreadyExistsException		thrown if an auto-generated Imageset clashes with one already defined in the system.
	\exception	GenericException			thrown if something goes wrong while accessing the true-type font \a fontname.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	Font(const String& name, const String& fontname, uint size, uint flags, utf32 first_code_point, utf32 last_code_point);


	/*!
	\brief
		Destroys a Font object
	*/
	~Font(void);


	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		Load and complete construction of 'this' via an XML file

	\param filename
		String object holding the name of the XML file that holds details about the font to create.

	\return
		Nothing.
	*/
	void	load(const String& filename);


	/*!
	\brief
		Unloads data associated with the font (font is then useless.  this is intended for cleanup).

	\return
		Nothing.
	*/
	void	unload(void);


	/*!
	\brief
		Return the required texture size required to store imagery for the
		glyphs specified in \a glyph_set

	\param glyph_set
		String object describing the set of code points who's glyph imagery is to be measured.

	\return
		Required pixel size for a square texture large enough to hold glyph imagery for the set of code points in \a glyph_set
	*/
	uint	getRequiredTextureSize(const String& glyph_set);


	/*!
	\brief
		Return the required texture size required to store imagery for the
		glyphs specified in by the inclusive range [first_code_point, last_code_point]

	\param first_code_point
		utf32 code point of the first character in the range to be measured

	\param last_code_point
		utf32 code point of the last character in the range to be measured

	\return
		Required pixel size for a square texture large enough to hold glyph imagery for the specified range of code points.
	*/
	uint	getRequiredTextureSize(utf32 first_code_point, utf32 last_code_point);


	/*!
	\brief
		Paint the set of glyphs required for all code points in \a glyph_set into the buffer \a buffer,
		which has a size of \a size pixels (not bytes) square.  This also defines an Image for each
		glyph in the Imageset for this font, and creates an entry in the code point to Image map.

	\param glyph_set
		String object containing the set of code points whos glyphs are to be loaded into the buffer.

	\param size
		Width of \a buffer in pixels (not bytes).

	\param buffer
		Pointer to a memory buffer large enough to receive the glyph image data.

	\return
		Nothing.
	*/
	void	createFontGlyphSet(const String& glyph_set, uint size, ulong* buffer);


	/*!
	\brief
		Paint the set of glyphs required for all code points in the range [\a first_code_point, \a last_code_point]
		into the buffer \a buffer, which has a size of \a size pixels (not bytes) square.  This also defines an
		Image for each glyph in the Imageset for this font, and creates an entry in the code point to Image map.

	\param first_code_point
		utf32 code point that is the first code point in a range whos glyph are to be loaded into the buffer.

	\param last_code_point
		utf32 code point that is the last code point in a range whos glyph are to be loaded into the buffer.

	\param size
		Width of \a buffer in pixels (not bytes).

	\param buffer
		Pointer to a memory buffer large enough to receive the glyph image data.

	\return
		Nothing.
	*/
	void	createFontGlyphSet(utf32 first_code_point, utf32 last_code_point, uint size, ulong* buffer);


	/*!
	\brief
		Copy the current glyph data into \a buffer, which has a width of \a buf_width pixels (not bytes).

	\param buffer
		Memory buffer large enough to receive the imagery for the currently loaded glyph.

	\param buf_width
		Width of \a buffer in pixels (where each pixel is a ulong).

	\return
		Nothing.
	*/
	void	drawGlyphToBuffer(ulong* buffer, uint buf_width);


	/*!
	\brief
		draws wrapped text.  returns number of lines output.
	*/
	uint	drawWrappedText(const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours) const;


	/*!
	\brief
		helper function for renderWrappedText to get next word of a string
	*/
	uint	getNextWord(const String& in_string, uint start_idx, String& out_string) const;


	/*!
	\brief
		Draw a line of text.  No formatting is applied.
	*/
	void	drawTextLine(const String& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours) const;


	/*!
	\brief
		Set the size of the free-type font (via d_face which should already be setup) and render the glyphs in d_glyphset.
	*/
	void	createFontFromFT_Face(uint size, uint horzDpi, uint vertDpi);


	/*!
	\brief
		Update the font as required according to the current scaling
	*/
	void	updateFontScaling(void);


	/*!
	\brief
		Calculate the vertical spacing fields for a static / bitmap font
	*/
	void	calculateStaticVertSpacing(void);

	/*!
	\brief
		Function to do real work of constructor.  Used to save duplication in the various constructor overloads.
	*/
	void	constructor_impl(const String& name, const String& fontname, uint size, uint flags, const String& glyph_set);


	/*!
	\brief
		Defines the set of code points on the font. (implementation).

	\note
		This function can take some time to execute depending upon the size of the code point set, and the size of the
		font being operated upon.

	\return
		Nothing

	\exception	InvalidRequestException		thrown if the font is based on a bitmap rather than a true-type font.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	void	defineFontGlyphs_impl(void);

	/*************************************************************************
		Implementation structs
	*************************************************************************/
	/*!
	\brief
		struct to hold extra details about a glyph (required for proper rendering)
	*/
	struct glyphDat
	{
		const Image*	d_image;				//!< The image which will be rendered.
		uint			d_horz_advance;			//!< Amount to advance the pen after rendering this glyph
		uint			d_horz_advance_unscaled;	//!< original unscaled advance value (only used with static / bitmap fonts).
	};


	/*************************************************************************
		Implementation Classes
	*************************************************************************/
	/*!
	\brief
		Handler class used to parse the Font XML files using SAX2
	*/
	class xmlHandler : public XERCES_CPP_NAMESPACE::DefaultHandler
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
		xmlHandler(Font* font) : d_font(font) {}

		/*!
		\brief
			Destructor for Font::xmlHandler objects
		*/
		virtual ~xmlHandler(void) {}

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
		static const char	FontElement[];					//!< Tag name for Font elements.
		static const char	MappingElement[];				//!< Tag name for Mapping elements.
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
		static const char	FontTypeStatic[];				//!< Value used for FontTypeAttribute for a static (bitmapped) font.
		static const char	FontTypeDynamic[];				//!< Value used for FontTypeAttribute for a dynamic (true-type) font.

		// general constants
		static const int	AutoGenerateHorzAdvance;		//!< Horizontal advance value that tells the parser to auto-calculate some reasonable value.

		/*************************************************************************
			Implementation Data
		*************************************************************************/
		Font* d_font;			//!< Font object that we are helping to build
	};


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef		std::map<utf32, glyphDat>		CodepointMap;
	CodepointMap	d_cp_map;	//!< Contains mappings from code points to Image objects

	String		d_name;			//!< Name of this font.
	Imageset*	d_glyph_images;	//!< Imageset that holds the glyphs for this font.

	bool	d_freetype;			//!< true when the font is a FreeType based font
	float	d_y_spacing;		//!< Height of font in pixels, a.k.a Line spacing.
	int		d_max_bearingY;		//!< Maximum bearingY value (gives required spacing down to baseline).

	// FreeType2 related
	FT_Face		d_face;			//!< FreeType 2 Face structure
	uint		d_ptSize;		//!< Point size of font.
	String		d_glyphset;		//!< set of glyphs for the dynamic font.

	// auto-scaling fields
	bool	d_autoScale;			//!< true when auto-scaling is enabled.
	float	d_horzScaling;			//!< current horizontal scaling factor.
	float	d_vertScaling;			//!< current vertical scaling factor.
	float	d_nativeHorzRes;		//!< native horizontal resolution for this Imageset.
	float	d_nativeVertRes;		//!< native vertical resolution for this Imageset.
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIFont_h_
