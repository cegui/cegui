/************************************************************************
filename: 	CEGUIScheme_xmlHandler.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements GUI Scheme class
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
#include "CEGUIScheme_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"
#include "CEGUILogger.h"
#include "CEGUIXmlHandlerHelper.h"

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Static Data definitions
*************************************************************************/

// xml file elements and attributes
const String Scheme_xmlHandler::GUISchemeElement( (utf8*)"GUIScheme" );
const String Scheme_xmlHandler::ImagesetElement( (utf8*)"Imageset" );
const String Scheme_xmlHandler::FontElement( (utf8*)"Font" );
const String Scheme_xmlHandler::WindowSetElement( (utf8*)"WindowSet" );
const String Scheme_xmlHandler::WindowFactoryElement( (utf8*)"WindowFactory" );
const String Scheme_xmlHandler::WindowAliasElement( (utf8*)"WindowAlias" );
const char	Scheme_xmlHandler::NameAttribute[]				= "Name";
const char	Scheme_xmlHandler::FilenameAttribute[]			= "Filename";
const char	Scheme_xmlHandler::AliasAttribute[]				= "Alias";
const char	Scheme_xmlHandler::TargetAttribute[]			= "Target";

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void Scheme_xmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
{
	XERCES_CPP_NAMESPACE_USE
	String element(XmlHandlerHelper::transcodeXmlCharToString(localname));

	// handle alias element
	if (element == WindowAliasElement)
	{
		Scheme::AliasMapping	alias;

		alias.aliasName	 = XmlHandlerHelper::getAttributeValueAsString(attrs, AliasAttribute);
		alias.targetName = XmlHandlerHelper::getAttributeValueAsString(attrs, TargetAttribute);
		d_scheme->d_aliasMappings.push_back(alias);
	}
	// handle an Imageset element
	else if (element == ImagesetElement)
	{
		Scheme::LoadableUIElement	imageset;

		imageset.name = XmlHandlerHelper::getAttributeValueAsString(attrs, NameAttribute);
		imageset.filename = XmlHandlerHelper::getAttributeValueAsString(attrs, FilenameAttribute);

		d_scheme->d_imagesets.push_back(imageset);
	}
	// handle a font element
	else if (element == FontElement)
	{
		Scheme::LoadableUIElement	font;

		font.name = XmlHandlerHelper::getAttributeValueAsString(attrs, NameAttribute);
		font.filename = XmlHandlerHelper::getAttributeValueAsString(attrs, FilenameAttribute);

		d_scheme->d_fonts.push_back(font);
	}
	// handle a WindowSet element
	else if (element == WindowSetElement)
	{
		Scheme::UIModule	module;
		module.name		= XmlHandlerHelper::getAttributeValueAsString(attrs, FilenameAttribute);
		module.module	= NULL;

		module.factories.clear();
		d_scheme->d_widgetModules.push_back(module);
	}
	// handle a WindowFactory element
	else if (element == WindowFactoryElement)
	{
		Scheme::UIElementFactory factory;

		factory.name = XmlHandlerHelper::getAttributeValueAsString(attrs, NameAttribute);

		d_scheme->d_widgetModules[d_scheme->d_widgetModules.size() - 1].factories.push_back(factory);
	}
	// handle root Scheme element
	else if (element == GUISchemeElement)
	{
		// get name of scheme we are creating
		d_scheme->d_name = XmlHandlerHelper::getAttributeValueAsString(attrs, NameAttribute);

		Logger::getSingleton().logEvent("Started creation of Scheme '" + d_scheme->d_name + "' via XML file.", Informative);

		if (SchemeManager::getSingleton().isSchemePresent(d_scheme->d_name))
		{
			throw	AlreadyExistsException((utf8*)"A GUI Scheme named '" + d_scheme->d_name + "' is already present in the system.");
		}

	}
	// anything else is an error which *should* have already been caught by XML validation
	else
	{
		throw FileIOException("Scheme::xmlHandler::startElement - Unexpected data was found while parsing the Scheme file: '" + element + "' is unknown.");
	}

}

void Scheme_xmlHandler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
{
	XERCES_CPP_NAMESPACE_USE
	String element(XmlHandlerHelper::transcodeXmlCharToString(localname));

	if (element == GUISchemeElement)
	{
		Logger::getSingleton().logEvent("Finished creation of Scheme '" + d_scheme->d_name + "' via XML file.", Informative);
	}

}


void Scheme_xmlHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Scheme_xmlHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Scheme_xmlHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

} // End of  CEGUI namespace section
