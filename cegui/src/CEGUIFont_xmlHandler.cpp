/***********************************************************************
    filename:   CEGUIFont_xmlHandler.cpp
    created:    Sun Jul 19 2009
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUIFont_xmlHandler.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUISystem.h"
#include "CEGUIXMLParser.h"
#include "CEGUIPixmapFont.h"

#ifdef CEGUI_HAS_FREETYPE
#   include "CEGUIFreeTypeFont.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String Font_xmlHandler::FontSchemaName("Font.xsd");
const String Font_xmlHandler::FontElement("Font");
const String Font_xmlHandler::MappingElement("Mapping");
const String Font_xmlHandler::FontTypeAttribute("Type");
const String Font_xmlHandler::FontNameAttribute("Name");
const String Font_xmlHandler::FontFilenameAttribute("Filename");
const String Font_xmlHandler::FontResourceGroupAttribute("ResourceGroup");
const String Font_xmlHandler::FontAutoScaledAttribute("AutoScaled");
const String Font_xmlHandler::FontNativeHorzResAttribute("NativeHorzRes");
const String Font_xmlHandler::FontNativeVertResAttribute("NativeVertRes");
const String Font_xmlHandler::FontLineSpacingAttribute("LineSpacing");
const String Font_xmlHandler::FontSizeAttribute("Size");
const String Font_xmlHandler::FontAntiAliasedAttribute("AntiAlias");
const String Font_xmlHandler::FontTypeFreeType("FreeType");
const String Font_xmlHandler::FontTypePixmap("Pixmap");
const String Font_xmlHandler::MappingCodepointAttribute("Codepoint");
const String Font_xmlHandler::MappingImageAttribute("Image");
const String Font_xmlHandler::MappingHorzAdvanceAttribute("HorzAdvance");

//----------------------------------------------------------------------------//
Font_xmlHandler::Font_xmlHandler():
    d_font(0),
    d_objectRead(false)
{}

//----------------------------------------------------------------------------//
Font_xmlHandler::~Font_xmlHandler()
{
    if (!d_objectRead)
        CEGUI_DELETE_AO d_font;
}

//----------------------------------------------------------------------------//
const String& Font_xmlHandler::getObjectName() const
{
    if (!d_font)
        CEGUI_THROW(InvalidRequestException("Font_xmlHandler::getName: "
            "Attempt to access null object."));

    return d_font->getName();
}

//----------------------------------------------------------------------------//
Font& Font_xmlHandler::getObject() const
{
    if (!d_font)
        CEGUI_THROW(InvalidRequestException("Font_xmlHandler::getObject: "
            "Attempt to access null object."));

    d_objectRead = true;
    return *d_font;
}

//----------------------------------------------------------------------------//
const String& Font_xmlHandler::getSchemaName() const
{
    return FontSchemaName;
}

//----------------------------------------------------------------------------//
const String& Font_xmlHandler::getDefaultResourceGroup() const
{
    return Font::getDefaultResourceGroup();
}

//----------------------------------------------------------------------------//
void Font_xmlHandler::elementStart(const String& element,
                                   const XMLAttributes& attributes)
{
    // handle root Font element
    if (element == FontElement)
        elementFontStart(attributes);
    // handle a Mapping element
    else if (element == MappingElement)
        elementMappingStart(attributes);
    // anything else is a non-fatal error.
    else
        Logger::getSingleton().logEvent("Font_xmlHandler::elementStart: "
            "Unknown element encountered: <" + element + ">", Errors);
}

//----------------------------------------------------------------------------//
void Font_xmlHandler::elementEnd(const String& element)
{
    if (element == FontElement)
        elementFontEnd();
}

//----------------------------------------------------------------------------//
void Font_xmlHandler::elementFontStart(const XMLAttributes& attributes)
{
    // get type of font being created
    const String font_type(attributes.getValueAsString(FontTypeAttribute));

    // log the start of font creation.
    Logger::getSingleton().logEvent(
        "Started creation of Font from XML specification:");

    if (font_type == FontTypeFreeType)
        createFreeTypeFont(attributes);
    else if (font_type == FontTypePixmap)
        createPixmapFont(attributes);
    else
        CEGUI_THROW(InvalidRequestException("Font_xmlHandler::elementFontStart: "
            "Encountered unknown font type of '" + font_type + "'"));
}

//----------------------------------------------------------------------------//
void Font_xmlHandler::elementFontEnd()
{
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(d_font));
    Logger::getSingleton().logEvent("Finished creation of Font '" +
        d_font->getName() + "' via XML file. " + addr_buff, Informative);
}

//----------------------------------------------------------------------------//
void Font_xmlHandler::elementMappingStart(const XMLAttributes& attributes)
{
    if (!d_font)
        CEGUI_THROW(InvalidRequestException(
            "Imageset_xmlHandler::elementMappingStart: Attempt to access null "
            "object."));

    // double-check font type just in case - report issues as 'soft' errors
    if (d_font->getTypeName() != FontTypePixmap)
        Logger::getSingleton().logEvent(
            "Imageset_xmlHandler::elementMappingStart: <Mapping> element is "
            "only valid for Pixmap type fonts.", Errors);
    else
        static_cast<PixmapFont*>(d_font)->defineMapping(
            attributes.getValueAsInteger(MappingCodepointAttribute),
            attributes.getValueAsString(MappingImageAttribute),
            attributes.getValueAsFloat(MappingHorzAdvanceAttribute, -1.0f));
}

//----------------------------------------------------------------------------//
void Font_xmlHandler::createFreeTypeFont(const XMLAttributes& attributes)
{
    const String name(attributes.getValueAsString(FontNameAttribute));
    const String filename(attributes.getValueAsString(FontFilenameAttribute));
    const String resource_group(attributes.getValueAsString(FontResourceGroupAttribute));

#ifdef CEGUI_HAS_FREETYPE
    Logger& logger(Logger::getSingleton());
    logger.logEvent("---- CEGUI font name: " + name);
    logger.logEvent("----       Font type: FreeType");
    logger.logEvent("----     Source file: " + filename +
                    " in resource group: " + (resource_group.empty() ?
                                              "(Default)" : resource_group));
    logger.logEvent("---- Real point size: " +
            attributes.getValueAsString(FontSizeAttribute, "12"));

    d_font = CEGUI_NEW_AO FreeTypeFont(name,
        attributes.getValueAsFloat(FontSizeAttribute, 12.0f),
        attributes.getValueAsBool(FontAntiAliasedAttribute, true),
        filename, resource_group,
        attributes.getValueAsBool(FontAutoScaledAttribute, false),
        attributes.getValueAsFloat(FontNativeHorzResAttribute, 640.0f),
        attributes.getValueAsFloat(FontNativeVertResAttribute, 480.0f),
        attributes.getValueAsFloat(FontLineSpacingAttribute, 0.0f));
#else
    CEGUI_THROW(InvalidRequestException("Font_xmlHandler::createFreeTypeFont: "
        "CEGUI was compiled without freetype support."));
#endif
}

//----------------------------------------------------------------------------//
void Font_xmlHandler::createPixmapFont(const XMLAttributes& attributes)
{
    const String name(attributes.getValueAsString(FontNameAttribute));
    const String filename(attributes.getValueAsString(FontFilenameAttribute));
    const String resource_group(attributes.getValueAsString(FontResourceGroupAttribute));

    Logger& logger(Logger::getSingleton());
    logger.logEvent("---- CEGUI font name: " + name);
    logger.logEvent("----       Font type: Pixmap");
    logger.logEvent("----     Source file: " + filename +
                    " in resource group: " + (resource_group.empty() ?
                                              "(Default)" : resource_group));

    d_font = CEGUI_NEW_AO PixmapFont(name, filename, resource_group,
        attributes.getValueAsBool(FontAutoScaledAttribute, false),
        attributes.getValueAsFloat(FontNativeHorzResAttribute, 640.0f),
        attributes.getValueAsFloat(FontNativeVertResAttribute, 480.0f));
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
