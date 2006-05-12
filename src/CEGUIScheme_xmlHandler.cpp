/***********************************************************************
filename: 	CEGUIScheme_xmlHandler.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements GUI Scheme class
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
#include "CEGUIScheme_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"
#include "falagard/CEGUIFalWidgetLookManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Static Data definitions
*************************************************************************/

// xml file elements and attributes
const String Scheme_xmlHandler::GUISchemeElement( "GUIScheme" );
const String Scheme_xmlHandler::ImagesetElement( "Imageset" );
const String Scheme_xmlHandler::ImagesetFromImageElement( "ImagesetFromImage" );
const String Scheme_xmlHandler::FontElement( "Font" );
const String Scheme_xmlHandler::WindowSetElement( "WindowSet" );
const String Scheme_xmlHandler::WindowFactoryElement( "WindowFactory" );
const String Scheme_xmlHandler::WindowAliasElement( "WindowAlias" );
const String Scheme_xmlHandler::FalagardMappingElement( "FalagardMapping" );
const String Scheme_xmlHandler::LookNFeelElement( "LookNFeel" );
const String Scheme_xmlHandler::NameAttribute( "Name" );
const String Scheme_xmlHandler::FilenameAttribute( "Filename" );
const String Scheme_xmlHandler::AliasAttribute( "Alias" );
const String Scheme_xmlHandler::TargetAttribute( "Target" );
const String Scheme_xmlHandler::ResourceGroupAttribute( "ResourceGroup" );
const String Scheme_xmlHandler::WindowTypeAttribute( "WindowType" );
const String Scheme_xmlHandler::TargetTypeAttribute( "TargetType" );
const String Scheme_xmlHandler::LookNFeelAttribute( "LookNFeel" );
const String Scheme_xmlHandler::WindowRendererSetElement( "WindowRendererSet" );
const String Scheme_xmlHandler::WindowRendererFactoryElement( "WindowRendererFactory" );
const String Scheme_xmlHandler::WindowRendererAttribute( "Renderer" );

/*************************************************************************
Handler methods
*************************************************************************/
void Scheme_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
{
    // handle alias element
    if (element == WindowAliasElement)
    {
        elementWindowAliasStart(attributes);
    }
    // handle an Imageset element
    else if (element == ImagesetElement)
    {
        elementImagesetStart(attributes);
    }
    // handle an ImagesetFromImage element
    else if (element == ImagesetFromImageElement)
    {
        elementImagesetFromImageStart(attributes);
    }
    // handle a font element
    else if (element == FontElement)
    {
        elementFontStart(attributes);
    }
    // handle a WindowSet element
    else if (element == WindowSetElement)
    {
        elementWindowSetStart(attributes);
    }
    // handle a WindowFactory element
    else if (element == WindowFactoryElement)
    {
        elementWindowFactoryStart(attributes);
    }
    // handle a WindowRendererSet element
    else if (element == WindowRendererSetElement)
    {
        elementWindowRendererSetStart(attributes);
    }
    // handle a WindowRendererFactory element
    else if (element == WindowRendererFactoryElement)
    {
        elementWindowRendererFactoryStart(attributes);
    }
    // handle root Scheme element
    else if (element == GUISchemeElement)
    {
        elementGUISchemeStart(attributes);
    }
    else if (element == FalagardMappingElement)
    {
        elementFalagardMappingStart(attributes);
    }
    else if (element == LookNFeelElement)
    {
        elementLookNFeelStart(attributes);
    }
    // anything else is an error which *should* have already been caught by XML validation
    else
    {
        Logger::getSingleton().logEvent("Scheme::xmlHandler::startElement - Unexpected data was found while parsing the Scheme file: '" + element + "' is unknown.", Errors);
    }
}

void Scheme_xmlHandler::elementEnd(const String& element)
{
    if (element == GUISchemeElement)
    {
        elementGUISchemeEnd();
    }
}

/*************************************************************************
    Method that handles the opening GUIScheme XML element.
*************************************************************************/
void Scheme_xmlHandler::elementGUISchemeStart(const XMLAttributes& attributes)
{
    // get name of scheme we are creating
    d_scheme->d_name = attributes.getValueAsString(NameAttribute);

    Logger::getSingleton().logEvent("Started creation of Scheme '" + d_scheme->d_name + "' via XML file.", Informative);

    if (SchemeManager::getSingleton().isSchemePresent(d_scheme->d_name))
    {
        throw   AlreadyExistsException("A GUI Scheme named '" + d_scheme->d_name + "' is already present in the system.");
    }
}

/*************************************************************************
    Method that handles the Imageset XML element.
*************************************************************************/
void Scheme_xmlHandler::elementImagesetStart(const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement   imageset;

    imageset.name = attributes.getValueAsString(NameAttribute);
    imageset.filename = attributes.getValueAsString(FilenameAttribute);
    imageset.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_imagesets.push_back(imageset);
}

/*************************************************************************
    Method that handles the ImagesetFromImage XML element.
*************************************************************************/
void Scheme_xmlHandler::elementImagesetFromImageStart(const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement   imageset;

    imageset.name = attributes.getValueAsString(NameAttribute);
    imageset.filename = attributes.getValueAsString(FilenameAttribute);
    imageset.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_imagesetsFromImages.push_back(imageset);
}

/*************************************************************************
    Method that handles the Font XML element.
*************************************************************************/
void Scheme_xmlHandler::elementFontStart(const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement   font;

    font.name = attributes.getValueAsString(NameAttribute);
    font.filename = attributes.getValueAsString(FilenameAttribute);
    font.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_fonts.push_back(font);
}

/*************************************************************************
    Method that handles the WindowSet XML element.
*************************************************************************/
void Scheme_xmlHandler::elementWindowSetStart(const XMLAttributes& attributes)
{
    Scheme::UIModule    module;
    module.name     = attributes.getValueAsString(FilenameAttribute);
    module.module   = 0;

    module.factories.clear();
    d_scheme->d_widgetModules.push_back(module);
}

/*************************************************************************
    Method that handles the WindowFactory XML element.
*************************************************************************/
void Scheme_xmlHandler::elementWindowFactoryStart(const XMLAttributes& attributes)
{
    Scheme::UIElementFactory factory;

    factory.name = attributes.getValueAsString(NameAttribute);

    d_scheme->d_widgetModules[d_scheme->d_widgetModules.size() - 1].factories.push_back(factory);
}

/*************************************************************************
    Method that handles the WindowRendererSet XML element.
*************************************************************************/
void Scheme_xmlHandler::elementWindowRendererSetStart(const XMLAttributes& attributes)
{
    Scheme::UIModule    module;
    module.name     = attributes.getValueAsString(FilenameAttribute);
    module.module   = 0;

    module.factories.clear();
    d_scheme->d_windowRendererModules.push_back(module);
}

/*************************************************************************
    Method that handles the WindowFactory XML element.
*************************************************************************/
void Scheme_xmlHandler::elementWindowRendererFactoryStart(const XMLAttributes& attributes)
{
    Scheme::UIElementFactory factory;

    factory.name = attributes.getValueAsString(NameAttribute);

    d_scheme->d_windowRendererModules[d_scheme->d_windowRendererModules.size() - 1].factories.push_back(factory);
}

/*************************************************************************
    Method that handles the WindowAlias XML element.
*************************************************************************/
void Scheme_xmlHandler::elementWindowAliasStart(const XMLAttributes& attributes)
{
    Scheme::AliasMapping    alias;

    alias.aliasName  = attributes.getValueAsString(AliasAttribute);
    alias.targetName = attributes.getValueAsString(TargetAttribute);
    d_scheme->d_aliasMappings.push_back(alias);
}

/*************************************************************************
    Method that handles the FalagardMapping XML element.
*************************************************************************/
void Scheme_xmlHandler::elementFalagardMappingStart(const XMLAttributes& attributes)
{
    Scheme::FalagardMapping fmap;
    fmap.windowName = attributes.getValueAsString(WindowTypeAttribute);
    fmap.targetName = attributes.getValueAsString(TargetTypeAttribute);
    fmap.lookName   = attributes.getValueAsString(LookNFeelAttribute);
    fmap.rendererName = attributes.getValueAsString(WindowRendererAttribute);

    d_scheme->d_falagardMappings.push_back(fmap);
}

/*************************************************************************
    Method that handles the LookNFeel XML element.
*************************************************************************/
void Scheme_xmlHandler::elementLookNFeelStart(const XMLAttributes& attributes)
{
    Scheme::LoadableUIElement lnf;
    lnf.filename      = attributes.getValueAsString(FilenameAttribute);
    lnf.resourceGroup = attributes.getValueAsString(ResourceGroupAttribute);

    d_scheme->d_looknfeels.push_back(lnf);
}

/*************************************************************************
    Method that handles the closing GUIScheme XML element.
*************************************************************************/
void Scheme_xmlHandler::elementGUISchemeEnd()
{
    Logger::getSingleton().logEvent("Finished creation of Scheme '" + d_scheme->d_name + "' via XML file.", Informative);
}

} // End of  CEGUI namespace section
