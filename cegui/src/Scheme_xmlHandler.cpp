/***********************************************************************
    filename:   CEGUIScheme_xmlHandler.cpp
    created:    Mon Jul 20 2009
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
#include "CEGUI/Scheme_xmlHandler.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Scheme.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/falagard/WidgetLookManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String Scheme_xmlHandler::GUISchemeSchemaName("GUIScheme.xsd");
const String Scheme_xmlHandler::GUISchemeElement("GUIScheme");
const String Scheme_xmlHandler::ImagesetElement("Imageset");
const String Scheme_xmlHandler::ImagesetFromImageElement("ImagesetFromImage");
const String Scheme_xmlHandler::FontElement("Font");
const String Scheme_xmlHandler::WindowSetElement("WindowSet");
const String Scheme_xmlHandler::WindowFactoryElement("WindowFactory");
const String Scheme_xmlHandler::WindowAliasElement("WindowAlias");
const String Scheme_xmlHandler::FalagardMappingElement("FalagardMapping");
const String Scheme_xmlHandler::LookNFeelElement("LookNFeel");
const String Scheme_xmlHandler::NameAttribute("Name");
const String Scheme_xmlHandler::FilenameAttribute("Filename");
const String Scheme_xmlHandler::AliasAttribute("Alias");
const String Scheme_xmlHandler::TargetAttribute("Target");
const String Scheme_xmlHandler::ResourceGroupAttribute("ResourceGroup");
const String Scheme_xmlHandler::WindowTypeAttribute("WindowType");
const String Scheme_xmlHandler::TargetTypeAttribute("TargetType");
const String Scheme_xmlHandler::LookNFeelAttribute("LookNFeel");
const String Scheme_xmlHandler::WindowRendererSetElement("WindowRendererSet");
const String Scheme_xmlHandler::WindowRendererFactoryElement("WindowRendererFactory");
const String Scheme_xmlHandler::WindowRendererAttribute("Renderer");
const String Scheme_xmlHandler::RenderEffectAttribute("RenderEffect");

//----------------------------------------------------------------------------//
Scheme_xmlHandler::Scheme_xmlHandler():
    d_scheme(0),
    d_objectRead(false)
{}

//----------------------------------------------------------------------------//
Scheme_xmlHandler::~Scheme_xmlHandler()
{
    if (!d_objectRead)
        CEGUI_DELETE_AO d_scheme;
}

//----------------------------------------------------------------------------//
const String& Scheme_xmlHandler::getObjectName() const
{
    if (!d_scheme)
        CEGUI_THROW(InvalidRequestException("Scheme_xmlHandler::getName: "
            "Attempt to access null object."));

    return d_scheme->getName();
}

//----------------------------------------------------------------------------//
Scheme& Scheme_xmlHandler::getObject() const
{
    if (!d_scheme)
        CEGUI_THROW(InvalidRequestException("Scheme_xmlHandler::getObject: "
            "Attempt to access null object."));

    d_objectRead = true;
    return *d_scheme;
}

//----------------------------------------------------------------------------//
const String& Scheme_xmlHandler::getSchemaName() const
{
    return GUISchemeSchemaName;
}

//----------------------------------------------------------------------------//
const String& Scheme_xmlHandler::getDefaultResourceGroup() const
{
    return Scheme::getDefaultResourceGroup();
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementStart(const String& element,
                                     const XMLAttributes& attributes)
{
    if (element == WindowAliasElement)
        elementWindowAliasStart(attributes);
    else if (element == ImagesetElement)
        elementImagesetStart(attributes);
    else if (element == ImagesetFromImageElement)
        elementImagesetFromImageStart(attributes);
    else if (element == FontElement)
        elementFontStart(attributes);
    else if (element == WindowSetElement)
        elementWindowSetStart(attributes);
    else if (element == WindowFactoryElement)
        elementWindowFactoryStart(attributes);
    else if (element == WindowRendererSetElement)
        elementWindowRendererSetStart(attributes);
    else if (element == WindowRendererFactoryElement)
        elementWindowRendererFactoryStart(attributes);
    else if (element == GUISchemeElement)
        elementGUISchemeStart(attributes);
    else if (element == FalagardMappingElement)
        elementFalagardMappingStart(attributes);
    else if (element == LookNFeelElement)
        elementLookNFeelStart(attributes);
    // anything else is a non-fatal error.
    else
        Logger::getSingleton().logEvent("Scheme_xmlHandler::elementStart: "
            "Unknown element encountered: <" + element + ">", Errors);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementEnd(const String& element)
{
    if (element == GUISchemeElement)
        elementGUISchemeEnd();
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementGUISchemeStart(const XMLAttributes& attributes)
{
    const String name(attributes.getValueAsString(NameAttribute));
    Logger& logger(Logger::getSingleton());
    logger.logEvent("Started creation of Scheme from XML specification:");
    logger.logEvent("---- CEGUI GUIScheme name: " + name);

    // create empty scheme with desired name
    d_scheme = CEGUI_NEW_AO Scheme(name);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementImagesetStart(const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement   imageset;

    imageset.name = attributes.getValueAsString(NameAttribute);
    imageset.filename = attributes.getValueAsString(FilenameAttribute);
    imageset.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_imagesets.push_back(imageset);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementImagesetFromImageStart(
    const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement   imageset;

    imageset.filename = attributes.getValueAsString(FilenameAttribute);
    imageset.name = attributes.getValueAsString(NameAttribute, imageset.filename);
    imageset.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_imagesetsFromImages.push_back(imageset);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementFontStart(const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement   font;

    font.name = attributes.getValueAsString(NameAttribute);
    font.filename = attributes.getValueAsString(FilenameAttribute);
    font.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_fonts.push_back(font);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementWindowSetStart(const XMLAttributes& attributes)
{
    Scheme::UIModule module;
    module.name = attributes.getValueAsString(FilenameAttribute);
    module.dynamicModule = 0;
    module.factoryModule = 0;

    d_scheme->d_widgetModules.push_back(module);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementWindowFactoryStart(
    const XMLAttributes& attributes)
{
    d_scheme->d_widgetModules[d_scheme->
        d_widgetModules.size() - 1].types.push_back(
            attributes.getValueAsString(NameAttribute));
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementWindowRendererSetStart(
    const XMLAttributes& attributes)
{
    Scheme::UIModule module;
    module.name = attributes.getValueAsString(FilenameAttribute);
    module.dynamicModule = 0;
    module.factoryModule = 0;

    d_scheme->d_windowRendererModules.push_back(module);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementWindowRendererFactoryStart(
    const XMLAttributes& attributes)
{
    d_scheme->
        d_windowRendererModules[d_scheme->d_windowRendererModules.size() - 1].
            types.push_back(attributes.getValueAsString(NameAttribute));
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementWindowAliasStart(const XMLAttributes& attributes)
{
    Scheme::AliasMapping    alias;

    alias.aliasName  = attributes.getValueAsString(AliasAttribute);
    alias.targetName = attributes.getValueAsString(TargetAttribute);
    d_scheme->d_aliasMappings.push_back(alias);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementFalagardMappingStart(
    const XMLAttributes& attributes)
{
    Scheme::FalagardMapping fmap;
    fmap.windowName = attributes.getValueAsString(WindowTypeAttribute);
    fmap.targetName = attributes.getValueAsString(TargetTypeAttribute);
    fmap.lookName   = attributes.getValueAsString(LookNFeelAttribute);
    fmap.rendererName = attributes.getValueAsString(WindowRendererAttribute);
    fmap.effectName = attributes.getValueAsString(RenderEffectAttribute);

    d_scheme->d_falagardMappings.push_back(fmap);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementLookNFeelStart(const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement lnf;
    lnf.filename      = attributes.getValueAsString(FilenameAttribute);
    lnf.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_looknfeels.push_back(lnf);
}

//----------------------------------------------------------------------------//
void Scheme_xmlHandler::elementGUISchemeEnd()
{
    if (!d_scheme)
        CEGUI_THROW(InvalidRequestException(
            "Scheme_xmlHandler::elementGUISchemeEnd: "
            "Attempt to access null object."));

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(d_scheme));
    Logger::getSingleton().logEvent("Finished creation of GUIScheme '" +
        d_scheme->getName() + "' via XML file. " + addr_buff, Informative);
}

} // End of  CEGUI namespace section
