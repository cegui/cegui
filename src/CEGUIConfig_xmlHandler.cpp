/************************************************************************
	filename: 	CEGUIConfig_xmlHandler.cpp
	created:	17/7/2004
	author:		Paul D Turner
	
	purpose:	Implements configuration file parser
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
#include "CEGUIConfig_xmlHandler.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIXmlHandlerHelper.h"

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Implementation Constants
*************************************************************************/
const String Config_xmlHandler::CEGUIConfigElement( (utf8*)"CEGUIConfig" );
const char	Config_xmlHandler::ConfigLogfileAttribute[]			= "Logfile";
const char	Config_xmlHandler::ConfigSchemeAttribute[]			= "Scheme";
const char	Config_xmlHandler::ConfigLayoutAttribute[]			= "Layout";
const char	Config_xmlHandler::ConfigDefaultFontAttribute[]		= "DefaultFont";
const char	Config_xmlHandler::ConfigInitScriptAttribute[]		= "InitScript";
const char	Config_xmlHandler::ConfigTerminateScriptAttribute[]	= "TerminateScript";

void Config_xmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
{
	XERCES_CPP_NAMESPACE_USE
	String element(XmlHandlerHelper::transcodeXmlCharToString(localname));

	// handle root CEGUIConfig element
	if (element == CEGUIConfigElement)
	{
		d_logFilename			= XmlHandlerHelper::getAttributeValueAsString(attrs, ConfigLogfileAttribute);
		d_schemeFilename		= XmlHandlerHelper::getAttributeValueAsString(attrs, ConfigSchemeAttribute);
		d_layoutFilename		= XmlHandlerHelper::getAttributeValueAsString(attrs, ConfigLayoutAttribute);
		d_initScriptFilename	= XmlHandlerHelper::getAttributeValueAsString(attrs, ConfigInitScriptAttribute);
		d_termScriptFilename	= XmlHandlerHelper::getAttributeValueAsString(attrs, ConfigTerminateScriptAttribute);
		d_defaultFontName		= XmlHandlerHelper::getAttributeValueAsString(attrs, ConfigDefaultFontAttribute);
	}
	// anything else is an error which *should* have already been caught by XML validation
	else
	{
		String message("Config_xmlHandler::startElement - Unexpected data was found while parsing the configuration file: '" + element + "' is unknown.");

		// throw a std::exception (because it won't try and use logger)
		throw message.c_str();
	}

}


void Config_xmlHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Config_xmlHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Config_xmlHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

} // End of  CEGUI namespace section
