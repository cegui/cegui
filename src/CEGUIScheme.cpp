/************************************************************************
	filename: 	CEGUIScheme.cpp
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
#include "CEGUIScheme.h"
#include "CEGUIExceptions.h"
#include "CEGUISchemeManager.h"
#include "CEGUILogger.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIFactoryModule.h"

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Static Data definitions
*************************************************************************/
// name of the xml schema for GUIScheme files
const char Scheme::GUISchemeSchemaName[]					= "GUIScheme.xsd";

// xml file elements and attributes
const char	Scheme::xmlHandler::GUISchemeElement[]			= "GUIScheme";
const char	Scheme::xmlHandler::ImagesetElement[]			= "Imageset";
const char	Scheme::xmlHandler::FontElement[]				= "Font";
const char	Scheme::xmlHandler::WindowSetElement[]			= "WindowSet";
const char	Scheme::xmlHandler::WindowFactoryElement[]		= "WindowFactory";
const char	Scheme::xmlHandler::NameAttribute[]				= "Name";
const char	Scheme::xmlHandler::FilenameAttribute[]			= "Filename";


/*************************************************************************
	Constructor for scheme objects
*************************************************************************/
Scheme::Scheme(const String& filename)
{
	XERCES_CPP_NAMESPACE_USE

	if (filename.empty() || (filename == (utf8*)""))
	{
		throw InvalidRequestException((utf8*)"Scheme::Scheme - Filename supplied for Scheme loading must be valid");
	}

	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

	// set basic settings we want from parser
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
	parser->setFeature(XMLUni::fgXercesSchema, true);
	parser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

	// setup schema for Scheme data
	XMLCh* pval = XMLString::transcode(GUISchemeSchemaName);
	ArrayJanitor<XMLCh>	arrayJanitor(pval);
	parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);

	// setup handler object
	xmlHandler handler(this);
	parser->setContentHandler(&handler);
	parser->setErrorHandler(&handler);

	// do parse (which uses handler to create actual data)
	try
	{
		parser->parse(filename.c_str());
	}
	catch(const XMLException& exc)
	{
		if (exc.getCode() != XMLExcepts::NoError)
		{
			delete parser;

			ArrayJanitor<char> excmsg(XMLString::transcode(exc.getMessage()));
			String message((utf8*)"Scheme::Scheme - An error occurred while parsing Scheme file '" + filename + "'.  Additional information: ");
			message += excmsg.get();

			throw FileIOException(message);
		}

	}
	catch(const SAXParseException& exc)
	{
		delete parser;

		ArrayJanitor<char> excmsg(XMLString::transcode(exc.getMessage()));
		String message((utf8*)"Scheme::Scheme - An error occurred while parsing Scheme file '" + filename + "'.  Additional information: ");
		message += excmsg.get();

		throw FileIOException(message);
	}
	catch(...)
	{
		delete parser;

		throw FileIOException((utf8*)"Scheme::Scheme - An unexpected error occurred while parsing Scheme file '" + filename + "'.");
	}

	// cleanup
	delete parser;

	Logger::getSingleton().logEvent((utf8*)"Loaded GUI scheme '" + d_name + "' from data in file '" + filename + "'.", Informative);

	// attempt to load in resources
	loadResources();
}


/*************************************************************************
	Destructor for scheme objects
*************************************************************************/
Scheme::~Scheme(void)
{
	unloadResources();

	Logger::getSingleton().logEvent((utf8*)"GUI scheme '" + d_name + "' has been unloaded.", Informative);
}


/*************************************************************************
	Load all resources for this scheme
*************************************************************************/
void Scheme::loadResources(void)
{
	Logger::getSingleton().logEvent((utf8*)"---- Begining resource loading for GUI scheme '" + d_name + "' ----", Informative);

	ImagesetManager& ismgr		= ImagesetManager::getSingleton();
	FontManager& fntmgr			= FontManager::getSingleton();
	WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();

	std::vector<LoadableUIElement>::const_iterator	pos;

	// check imagesets
	for (pos = d_imagesets.begin(); pos != d_imagesets.end(); ++pos)
	{
		if (!ismgr.isImagesetPresent((*pos).name))
		{
			Imageset* iset = ismgr.createImageset((*pos).filename);

			// check for wrong imageset for specified name
			String realname = iset->getName();

			if (realname != (*pos).name)
			{
				ismgr.destroyImageset(iset);
				throw InvalidRequestException((utf8*)"Scheme::loadResources - The Imageset created by file '" + 
					(*pos).filename + "' is named '" + realname + "', not '" + (*pos).name + "' as required by Scheme '" + d_name + "'.");
			}

		}

	}

	// check fonts
	for (pos = d_fonts.begin(); pos != d_fonts.end(); ++pos)
	{
		if (!fntmgr.isFontPresent((*pos).name))
		{
			Font* font = fntmgr.createFont((*pos).filename);

			// check for wrong font for specified name
			String realname = font->getName();

			if (realname != (*pos).name)
			{
				fntmgr.destroyFont(font);
				throw InvalidRequestException((utf8*)"Scheme::loadResources - The Font created by file '" + 
					(*pos).filename + "' is named '" + realname + "', not '" + (*pos).name + "' as required by Scheme '" + d_name + "'.");
			}

		}

	}

	// check factories
	std::vector<UIModule>::iterator	cmod = d_widgetModules.begin();
	for (;cmod != d_widgetModules.end(); ++cmod)
	{
		// create and load dynamic module as required
		if ((*cmod).module == NULL)
		{
			(*cmod).module = new FactoryModule((*cmod).name);
		}

		std::vector<UIElementFactory>::const_iterator	elem = (*cmod).factories.begin();
		for (; elem != (*cmod).factories.end(); ++elem)
		{
			if (!wfmgr.isFactoryPresent((*elem).name))
			{
				(*cmod).module->registerFactory((*elem).name);
			}

		}

	}

	Logger::getSingleton().logEvent((utf8*)"---- Resource loading for GUI scheme '" + d_name + "' completed ----", Informative);
}


/*************************************************************************
	Unload all resources for this scheme
*************************************************************************/
void Scheme::unloadResources(void)
{
	Logger::getSingleton().logEvent((utf8*)"---- Begining resource cleanup for GUI scheme '" + d_name + "' ----", Informative);

	ImagesetManager& ismgr		= ImagesetManager::getSingleton();
	FontManager& fntmgr			= FontManager::getSingleton();
	WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();

	std::vector<LoadableUIElement>::const_iterator	pos;

	// check fonts
	for (pos = d_fonts.begin(); pos != d_fonts.end(); ++pos)
	{
		fntmgr.destroyFont((*pos).name);
	}

	// check Imagesets
	for (pos = d_imagesets.begin(); pos != d_imagesets.end(); ++pos)
	{
		ismgr.destroyImageset((*pos).name);
	}

	// check factories
	std::vector<UIModule>::iterator	cmod = d_widgetModules.begin();
	for (;cmod != d_widgetModules.end(); ++cmod)
	{
		std::vector<UIElementFactory>::const_iterator	elem = (*cmod).factories.begin();
		for (; elem != (*cmod).factories.end(); ++elem)
		{
			wfmgr.removeFactory((*elem).name);
		}

		// unload dynamic module as required
		if ((*cmod).module != NULL)
		{
			delete (*cmod).module;
			(*cmod).module = NULL;
		}

	}

	Logger::getSingleton().logEvent((utf8*)"---- Resource cleanup for GUI scheme '" + d_name + "' completed ----", Informative);
}


/*************************************************************************
	Check if all resources for this Scheme are loaded
*************************************************************************/
bool Scheme::resourcesLoaded(void) const
{
	ImagesetManager& ismgr		= ImagesetManager::getSingleton();
	FontManager& fntmgr			= FontManager::getSingleton();
	WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();

	std::vector<LoadableUIElement>::const_iterator	pos;

	// check imagesets
	for (pos = d_imagesets.begin(); pos != d_imagesets.end(); ++pos)
	{
		if (!ismgr.isImagesetPresent((*pos).name))
		{
			return false;
		}

	}

	// check fonts
	for (pos = d_fonts.begin(); pos != d_fonts.end(); ++pos)
	{
		if (!fntmgr.isFontPresent((*pos).name))
		{
			return false;
		}

	}

	// check factories
	std::vector<UIModule>::const_iterator	cmod = d_widgetModules.begin();
	for (;cmod != d_widgetModules.end(); ++cmod)
	{
		std::vector<UIElementFactory>::const_iterator	elem = (*cmod).factories.begin();

		for (; elem != (*cmod).factories.end(); ++elem)
		{
			if (!wfmgr.isFactoryPresent((*elem).name))
			{
				return false;
			}

		}

	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	SAX2 Handler methods
*************************************************************************/
void Scheme::xmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
{
	XERCES_CPP_NAMESPACE_USE
	std::string element(XMLString::transcode(localname));

	// handle an Imageset element
	if (element == ImagesetElement)
	{
		LoadableUIElement	imageset;

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
		LoadableUIElement	font;

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
		UIModule	module;
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
		UIElementFactory factory;

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

void Scheme::xmlHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Scheme::xmlHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Scheme::xmlHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

} // End of  CEGUI namespace section
