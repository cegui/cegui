/************************************************************************
filename: 	CEGUIScheme_xmlHandler.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements GUI Scheme class
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
#include "CEGUIScheme_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Static Data definitions
*************************************************************************/

// xml file elements and attributes
const char	Scheme_xmlHandler::GUISchemeElement[]			= "GUIScheme";
const char	Scheme_xmlHandler::ImagesetElement[]			= "Imageset";
const char	Scheme_xmlHandler::FontElement[]				= "Font";
const char	Scheme_xmlHandler::WindowSetElement[]			= "WindowSet";
const char	Scheme_xmlHandler::WindowFactoryElement[]		= "WindowFactory";
const char	Scheme_xmlHandler::NameAttribute[]				= "Name";
const char	Scheme_xmlHandler::FilenameAttribute[]			= "Filename";

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void Scheme_xmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
{
	XERCES_CPP_NAMESPACE_USE
		std::string element(XMLString::transcode(localname));

	// handle an Imageset element
	if (element == ImagesetElement)
	{
		Scheme::LoadableUIElement	imageset;

		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(NameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		imageset.name = (utf8*)val_str.get();

		attr_name.reset(XMLString::transcode(FilenameAttribute));
		val_str.reset(XMLString::transcode(attrs.getValue(attr_name.get())));
		imageset.filename = (utf8*)val_str.get();

		d_scheme->d_imagesets.push_back(imageset);
	}
	// handle a font element
	else if (element == FontElement)
	{
		Scheme::LoadableUIElement	font;

		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(NameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		font.name = (utf8*)val_str.get();

		attr_name.reset(XMLString::transcode(FilenameAttribute));
		val_str.reset(XMLString::transcode(attrs.getValue(attr_name.get())));
		font.filename = (utf8*)val_str.get();

		d_scheme->d_fonts.push_back(font);
	}
	// handle a WindowSet element
	else if (element == WindowSetElement)
	{
		Scheme::UIModule	module;
		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(FilenameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		module.name		= (utf8*)val_str.get();
		module.module	= NULL;

		module.factories.clear();
		d_scheme->d_widgetModules.push_back(module);
	}
	// handle a WindowFactory element
	else if (element == WindowFactoryElement)
	{
		Scheme::UIElementFactory factory;

		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(NameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		factory.name = (utf8*)val_str.get();

		d_scheme->d_widgetModules[d_scheme->d_widgetModules.size() - 1].factories.push_back(factory);
	}
	// handle root Scheme element
	else if (element == GUISchemeElement)
	{
		// get name of scheme we are creating
		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(NameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		d_scheme->d_name = (utf8*)val_str.get();

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
