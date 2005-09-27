/************************************************************************
filename: 	CEGUIFont.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements Font class
*************************************************************************/
/*************************************************************************
Crazy Eddie's GUI System (http://www.cegui.org.uk)
Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"

#include <ft2build.h>
#include FT_FREETYPE_H


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
static data definitions
*************************************************************************/

// XML related strings
const String Font_xmlHandler::FontElement( "Font" );
const String Font_xmlHandler::MappingElement( "Mapping" );
const String Font_xmlHandler::FontTypeStatic( "Static" );
const String Font_xmlHandler::FontTypeDynamic( "Dynamic" );
const String Font_xmlHandler::GlyphElement( "Glyph" );
const String Font_xmlHandler::GlyphRangeElement( "GlyphRange" );
const String Font_xmlHandler::GlyphSetElement( "GlyphSet" );
const String Font_xmlHandler::FontNameAttribute( "Name" );
const String Font_xmlHandler::FontFilenameAttribute( "Filename" );
const String Font_xmlHandler::FontResourceGroupAttribute( "ResourceGroup" );
const String Font_xmlHandler::FontTypeAttribute( "Type" );
const String Font_xmlHandler::FontSizeAttribute( "Size" );
const String Font_xmlHandler::FontFirstCodepointAttribute( "FirstCodepoint" );
const String Font_xmlHandler::FontLastCodepointAttribute( "LastCodepoint" );
const String Font_xmlHandler::FontNativeHorzResAttribute( "NativeHorzRes" );
const String Font_xmlHandler::FontNativeVertResAttribute( "NativeVertRes" );
const String Font_xmlHandler::FontAutoScaledAttribute( "AutoScaled" );
const String Font_xmlHandler::FontAntiAliasedAttribute( "AntiAlias" );
const String Font_xmlHandler::MappingCodepointAttribute( "Codepoint" );
const String Font_xmlHandler::MappingImageAttribute( "Image" );
const String Font_xmlHandler::MappingHorzAdvanceAttribute( "HorzAdvance" );
const String Font_xmlHandler::GlyphCodepointAttribute( "Codepoint" );
const String Font_xmlHandler::GlyphRangeStartCodepointAttribute( "StartCodepoint" );
const String Font_xmlHandler::GlyphRangeEndCodepointAttribute( "EndCodepoint" );
const String Font_xmlHandler::GlyphSetGlyphsAttribute( "Glyphs" );

// General constants
const int	Font_xmlHandler::AutoGenerateHorzAdvance		= -1;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
    Main element-start despatcher
*************************************************************************/
void Font_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
{
    // handle a Mapping element
    if (element == MappingElement)
    {
        elementMappingStart(attributes);
    }
    // handle root Font element
    else if (element == FontElement)
    {
        elementFontStart(attributes);
    }
    // Glyph element
    else if (element == GlyphElement)
    {
        elementGlyphStart(attributes);
    }
    // GlyphRange element
    else if (element == GlyphRangeElement)
    {
        elementGlyphRangeStart(attributes);
    }
    // GlyphSet element
    else if (element == GlyphSetElement)
    {
        elementGlyphSetStart(attributes);
    }
    // anything else is really an error which *should* have already been caught by XML validation
    else
    {
        Logger::getSingleton().logEvent("Font::xmlHandler::startElement - Unexpected data was found while parsing the Font file: '" + element + "' is unknown.", Errors);
    }
}

/*************************************************************************
    Main element-end despatcher
*************************************************************************/
void Font_xmlHandler::elementEnd(const String& element)
{
    if (element == FontElement)
    {
        elementFontEnd();
    }
}

/*************************************************************************
    Method that handles the opening Font XML element.
*************************************************************************/
void Font_xmlHandler::elementFontStart(const XMLAttributes& attributes)
{
    // get type of font being created
    String  font_type(attributes.getValueAsString(FontTypeAttribute));

    // dynamic (ttf) font
    if (font_type == FontTypeDynamic)
    {
        createDynamicFont(attributes);
    }
    // static (Imageset based) font
    else if (font_type == FontTypeStatic)
    {
        createStaticFont(attributes);
    }
    // error (should never happen)
    else
    {
        throw FileIOException("Font::xmlHandler::startElement - The unknown Font:Type attribute value '" + font_type + "' was encountered while processing the Font file.");
    }
}

/*************************************************************************
    Method that handles the Mapping XML element.
*************************************************************************/
void Font_xmlHandler::elementMappingStart(const XMLAttributes& attributes)
{
    if (!d_font->d_freetype)
    {
        String  image_name(attributes.getValueAsString(MappingImageAttribute));
        utf32 codepoint = (utf32)attributes.getValueAsInteger(MappingCodepointAttribute);
        int horzAdvance = attributes.getValueAsInteger(MappingHorzAdvanceAttribute, -1);

        const Image* image = &d_font->d_glyph_images->getImage(image_name);

        // calculate advance width if it was not specified
        if (horzAdvance == AutoGenerateHorzAdvance)
            horzAdvance = (int)(image->getWidth() + image->getOffsetX());

        GlyphDat mapDat(image,
                        (uint)(((float)horzAdvance) * (d_font->d_autoScale ? d_font->d_horzScaling : 1.0f)),
                        horzAdvance);

        d_font->d_cp_map[codepoint] = mapDat;
    }
    else
    {
        Logger::getSingleton().logEvent("Mapping element encountered.  This element is invalid for dynamic fonts.", Informative);
    }
}

/*************************************************************************
    Method that handles the Glyph XML element.
*************************************************************************/
void Font_xmlHandler::elementGlyphStart(const XMLAttributes& attributes)
{
    if (d_font->d_freetype)
    {
        utf32 codepoint = (utf32)attributes.getValueAsInteger(GlyphCodepointAttribute);

        if (d_glyphSet.find(codepoint) == String::npos)
        {
            d_glyphSet.append(1, codepoint);
        }
    }
    else
    {
        Logger::getSingleton().logEvent("Glyph element encountered.  This element is invalid for static fonts.", Informative);
    }
}

/*************************************************************************
    Method that handles the GlyphRange XML element.
*************************************************************************/
void Font_xmlHandler::elementGlyphRangeStart(const XMLAttributes& attributes)
{
    if (d_font->d_freetype)
    {
        utf32 start = (utf32)attributes.getValueAsInteger(GlyphRangeStartCodepointAttribute);
        utf32 end   = (utf32)attributes.getValueAsInteger(GlyphRangeEndCodepointAttribute);

        for (utf32 codepoint = start; codepoint <= end; ++codepoint)
        {
            if (d_glyphSet.find(codepoint) == String::npos)
            {
                d_glyphSet.append(1, codepoint);
            }
        }
    }
    else
    {
        Logger::getSingleton().logEvent("GlyphRange element encountered.  This element is invalid for static fonts.", Informative);
    }
}

/*************************************************************************
    Method that handles the GlyphSet XML element.
*************************************************************************/
void Font_xmlHandler::elementGlyphSetStart(const XMLAttributes& attributes)
{
    if (d_font->d_freetype)
    {
        String glyphs(attributes.getValueAsString(GlyphSetGlyphsAttribute));

        for (String::size_type i = 0; i < glyphs.length(); ++i)
        {
            utf32 codepoint = glyphs[i];

            if (d_glyphSet.find(codepoint) == String::npos)
            {
                d_glyphSet.append(1, codepoint);
            }
        }
    }
    else
    {
        Logger::getSingleton().logEvent("GlyphSet element encountered.  This element is invalid for static fonts.", Informative);
    }
}

/*************************************************************************
    Method that handles the closing Font XML element.
*************************************************************************/
void Font_xmlHandler::elementFontEnd()
{
    // if this is a freetype based font, perform glyph definition
    if (d_font->d_freetype)
        d_font->defineFontGlyphs(d_glyphSet);

    Logger::getSingleton().logEvent("Finished creation of Font '" + d_font->d_name + "' via XML file.", Informative);
}

/*************************************************************************
    Method that completes font creatiion for a dynamic font.
*************************************************************************/
void Font_xmlHandler::createDynamicFont(const XMLAttributes& attributes)
{
    // get name of font we are creating
    String font_name(attributes.getValueAsString(FontNameAttribute));
    // get filename for the font
    String filename(attributes.getValueAsString(FontFilenameAttribute));
    // get resource group for font file.
    String resourceGroup(attributes.getValueAsString(FontResourceGroupAttribute));
    // get size of font
    uint size = (uint)attributes.getValueAsInteger(FontSizeAttribute, 12);

    // log the start of font creation.
    Logger::getSingleton().logEvent("Started creation of dynamic Font from XML specification:");
    Logger::getSingleton().logEvent("---- CEGUI font name: " + font_name);
    Logger::getSingleton().logEvent("---- Source TTF file: " + filename +
                                    " in resource group: " + (resourceGroup.empty() ? "(Default)" : resourceGroup));
    Logger::getSingleton().logEvent("---- Real point size: " + attributes.getValueAsString(FontSizeAttribute, "12"));

    // extract codepoint range
    utf32 first_codepoint = (utf32)attributes.getValueAsInteger(FontFirstCodepointAttribute, 32);
    utf32 last_codepoint = (utf32)attributes.getValueAsInteger(FontLastCodepointAttribute, 127);

    // build string containing the required code-points.
    for (;first_codepoint <= last_codepoint; ++first_codepoint)
        d_glyphSet += first_codepoint;

    uint flags = attributes.getValueAsBool(FontAntiAliasedAttribute, true) ? 0 : NoAntiAlias;

    // perform pre-initialisation
    initialiseAutoScaling(attributes);

    // Finalise construction of font without glyphs.
    // Glyphs will defined after we know which ones we need.
    d_font->constructor_impl(font_name, filename, resourceGroup, size, flags, String(""));
    d_font->d_sourceFilename = filename;
}

/*************************************************************************
    Method that completes font creatiion for a static font.
*************************************************************************/
void Font_xmlHandler::createStaticFont(const XMLAttributes& attributes)
{
    // get name of font we are creating
    String font_name(attributes.getValueAsString(FontNameAttribute));
    // get filename for the font
    String filename(attributes.getValueAsString(FontFilenameAttribute));
    // get resource group for font file.
    String resourceGroup(attributes.getValueAsString(FontResourceGroupAttribute));

    // log the start of font creation.
    Logger::getSingleton().logEvent("Started creation of static (bitmapped) Font from XML specification:");
    Logger::getSingleton().logEvent("---- CEGUI font name: " + font_name);
    Logger::getSingleton().logEvent("---- Source Imageset file: " + filename +
                                    " in resource group: " + (resourceGroup.empty() ? "(Default)" : resourceGroup));

    d_font->d_name = font_name;
    d_font->d_freetype = false;
    d_font->d_sourceFilename = filename;

    // load the Imageset
    d_font->d_glyph_images = ImagesetManager::getSingleton().createImageset(filename, resourceGroup);

    initialiseAutoScaling(attributes);
}

/*************************************************************************
    Method that initialises the auto-scale settings of the font being
    created.
*************************************************************************/
void Font_xmlHandler::initialiseAutoScaling(const XMLAttributes& attributes)
{
    // set autoscale options on the font being created.
    d_font->setNativeResolution(
        Size(static_cast<float>(attributes.getValueAsInteger(FontNativeHorzResAttribute, 640)),
             static_cast<float>(attributes.getValueAsInteger(FontNativeVertResAttribute, 480))));

    d_font->setAutoScalingEnabled(
        attributes.getValueAsBool(FontAutoScaledAttribute, false));
}

} // End of  CEGUI namespace section
