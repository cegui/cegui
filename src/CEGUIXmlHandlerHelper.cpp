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
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include "CEGUIString.h"
#include "CEGUIXmlHandlerHelper.h"
#include "CEGUIExceptions.h"
#include "CEGUIResourceProvider.h"
#include "CEGUISystem.h"
#include "CEGUILogger.h"

using XERCES_CPP_NAMESPACE::XMLString;
using XERCES_CPP_NAMESPACE::XMLTransService;
using XERCES_CPP_NAMESPACE::XMLPlatformUtils;
using XERCES_CPP_NAMESPACE::XMLTranscoder;
using XERCES_CPP_NAMESPACE::XMLRecognizer;


namespace CEGUI 
{

const XMLCh* XmlHandlerHelper::getAttributeValueAsXmlChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	XMLCh* attr_name = XMLString::transcode(attributeName);
	const XMLCh* val_str = attrs.getValue(attr_name);
	XMLString::release(&attr_name);

	return val_str;
}

char* XmlHandlerHelper::getAttributeValueAsChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	return XMLString::transcode(getAttributeValueAsXmlChar(attrs, attributeName));
}

int XmlHandlerHelper::getAttributeValueAsInteger(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	return XMLString::parseInt(getAttributeValueAsXmlChar(attrs, attributeName));
}

String XmlHandlerHelper::getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	return transcodeXmlCharToString(getAttributeValueAsXmlChar(attrs, attributeName));
}

String XmlHandlerHelper::transcodeXmlCharToString(const XMLCh* const xmlch_str)
{
	XMLTransService::Codes	res;
	XMLTranscoder* transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XMLRecognizer::UTF_8, res, 4096, XMLPlatformUtils::fgMemoryManager );

	if (res == XMLTransService::Ok)
	{
		String out;
		utf8 outBuff[128];
		unsigned int outputLength;
		unsigned int eaten = 0;
		unsigned int offset = 0;
		unsigned int inputLength = XMLString::stringLen(xmlch_str);

		while (inputLength)
		{
			outputLength = transcoder->transcodeTo(xmlch_str + offset, inputLength, outBuff, 128, eaten, XMLTranscoder::UnRep_RepChar);
			out.append(outBuff, outputLength);
			offset += eaten;
			inputLength -= eaten;
		}

		delete transcoder;

		return out;
	}
	else
	{
		throw GenericException((utf8*)"XmlHandlerHelper::transcodeXmlCharToString - Internal Error: Could not create UTF-8 string transcoder.");
	}

}

void XmlHandlerHelper::initialiseSchema(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser, const String& schemaName, const String& xmlFilename, const String& resourceGroup)
{
    XERCES_CPP_NAMESPACE_USE;

    // enable schema use and set validation options
    parser->setFeature(XMLUni::fgXercesSchema, true);
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

    // load in the raw schema data
    RawDataContainer rawSchemaData;
    // try base filename first
    try
    {
        Logger::getSingleton().logEvent("XmlHandlerHelper::initialiseSchema - Attempting to load schema from file '" + schemaName + "'.", Informative);
        System::getSingleton().getResourceProvider()->loadRawDataContainer(schemaName, rawSchemaData, resourceGroup);
    }
    // oops, no file.  Try an alternative instead...
    catch(InvalidRequestException)
    {
        // get path from filename
        String schemaFilename;
        size_t pos = xmlFilename.rfind("/");
        if (pos == String::npos) pos = xmlFilename.rfind("\\");
        if (pos != String::npos) schemaFilename.assign(xmlFilename, 0, pos + 1);
        // append schema filename
        schemaFilename += schemaName;
        // re-try the load operation.
        Logger::getSingleton().logEvent("XmlHandlerHelper::initialiseSchema - Attempting to load schema from file '" + schemaFilename + "'.", Informative);
        System::getSingleton().getResourceProvider()->loadRawDataContainer(schemaFilename, rawSchemaData, resourceGroup);
    }
    // wrap schema data in a xerces MemBufInputSource object
    MemBufInputSource  schemaData(
        rawSchemaData.getDataPtr(),
        static_cast<const unsigned int>(rawSchemaData.getSize()),
        schemaName.c_str(),
        false);
    parser->loadGrammar(schemaData, Grammar::SchemaGrammarType, true);
    // enable grammar reuse
    parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);

    // set schema for usage
    XMLCh* pval = XMLString::transcode(schemaName.c_str());
    parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);
    XMLString::release(&pval);
    Logger::getSingleton().logEvent("XmlHandlerHelper::initialiseSchema - XML schema file '" + schemaName + "' has been initialised.", Informative);
}

XERCES_CPP_NAMESPACE::SAX2XMLReader* XmlHandlerHelper::createParser(XERCES_CPP_NAMESPACE::DefaultHandler& handler)
{
    XERCES_CPP_NAMESPACE_USE;

    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

    // set basic settings we want from parser
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);

    // set handlers
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler);

    return parser;
}

void XmlHandlerHelper::parseXMLFile(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser, const String& xmlFilename, const String& resourceGroup)
{
    XERCES_CPP_NAMESPACE_USE;

    // use resource provider to load file data
    RawDataContainer rawXMLData;
    System::getSingleton().getResourceProvider()->loadRawDataContainer(xmlFilename, rawXMLData, resourceGroup);
    MemBufInputSource  fileData(
        rawXMLData.getDataPtr(),
        static_cast<const unsigned int>(rawXMLData.getSize()),
        xmlFilename.c_str(),
        false);

    // perform parse
    parser->parse(fileData);
}

void XmlHandlerHelper::parseXMLFile(XERCES_CPP_NAMESPACE::DefaultHandler& handler, const String& schemaName, const String& xmlFilename, const String& resourceGroup)
{
    XERCES_CPP_NAMESPACE_USE;

    // create parser
    SAX2XMLReader* parser = XmlHandlerHelper::createParser(handler);
    // set up schema
    XmlHandlerHelper::initialiseSchema(parser, schemaName, xmlFilename, resourceGroup);

    // do parse
    try
    {
        XmlHandlerHelper::parseXMLFile(parser, xmlFilename, resourceGroup);
    }
    catch(const XMLException& exc)
    {
        if (exc.getCode() != XMLExcepts::NoError)
        {
            delete parser;

            char* excmsg = XMLString::transcode(exc.getMessage());
            String message((utf8*)"XmlHandlerHelper::parse - An error occurred while parsing XML file '" + xmlFilename + "'.  Additional information: ");
            message += excmsg;
            XMLString::release(&excmsg);

            throw FileIOException(message);
        }

    }
    catch(const SAXParseException& exc)
    {
        delete parser;

        char* excmsg = XMLString::transcode(exc.getMessage());
        String message((utf8*)"XmlHandlerHelper::parse - An error occurred while parsing XML file '" + xmlFilename + "'.  Additional information: ");
        message += excmsg;
        XMLString::release(&excmsg);

        throw FileIOException(message);
    }
    catch(...)
    {
        delete parser;

        Logger::getSingleton().logEvent("XmlHandlerHelper::parse - An unexpected error occurred while parsing XML file '" + xmlFilename + "'.", Errors);
        throw;
    }

    // cleanup
    delete parser;
}

}
