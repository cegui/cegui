/***********************************************************************
    created:    Sat Mar 12 2005
    author:     Paul D Turner
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
#include "CEGUI/XMLParserModules/Xerces/XMLParser.h"
#include "CEGUI/String.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/System.h"
#include "CEGUI/XMLHandler.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/PropertyHelper.h"

#include <xercesc/validators/schema/SchemaValidator.hpp>

#include <iostream> // Debug 
// Start of CEGUI namespace section
namespace CEGUI
{
    // Static data definition for default schema resource group name
    String XercesParser::d_defaultSchemaResourceGroup("");
    // static data definition of the SchemaDefaultResourceGroup property.
    XercesParserProperties::SchemaDefaultResourceGroup
        XercesParser::s_schemaDefaultResourceGroupProperty;

    ////////////////////////////////////////////////////////////////////////////////
    //
    // XercesParser methods
    //
    ////////////////////////////////////////////////////////////////////////////////

    XercesParser::XercesParser(void)
    {
        // set ID string
        d_identifierString = "CEGUI::XercesParser - Official Xerces-C++ based parser module for CEGUI";
        // add property
        addProperty(&s_schemaDefaultResourceGroupProperty);
    }

    XercesParser::~XercesParser(void)
    {}

    void XercesParser::parseXML(XMLHandler& handler, const RawDataContainer& source, const String& schemaName)
    {
        XERCES_CPP_NAMESPACE_USE;

        XercesHandler xercesHandler(handler);

        // create parser
        SAX2XMLReader* reader = createReader(xercesHandler);

        CEGUI_TRY
        {
            // set up schema
            initialiseSchema(reader, schemaName);
            // do parse
            doParse(reader, source);
        }
        CEGUI_CATCH(const XMLException& exc)
        {
            if (exc.getCode() != XMLExcepts::NoError)
            {
                delete reader;

                char* excmsg = XMLString::transcode(exc.getMessage());
                String message("An error occurred at line nr. " + PropertyHelper<uint>::toString((uint)exc.getSrcLine()) + " while parsing XML.  Additional information: ");
                message += excmsg;
                XMLString::release(&excmsg);

                CEGUI_THROW(FileIOException(message));
            }

        }
        CEGUI_CATCH(const SAXParseException& exc)
        {
            delete reader;

            char* excmsg = XMLString::transcode(exc.getMessage());
            String message("An error occurred at line nr. " + PropertyHelper<uint>::toString((uint)exc.getLineNumber()) + " while parsing XML.  Additional information: ");
            message += excmsg;
            XMLString::release(&excmsg);

            CEGUI_THROW(FileIOException(message));
        }
        CEGUI_CATCH(...)
        {
            delete reader;

            Logger::getSingleton().logEvent("An unexpected error occurred while parsing XML", Errors);
            CEGUI_RETHROW;
        }

        // cleanup
        delete reader;
    }

    bool XercesParser::initialiseImpl(void)
    {
        XERCES_CPP_NAMESPACE_USE;

        // initialise Xerces-C XML system
        CEGUI_TRY
        {
            XMLPlatformUtils::Initialize();
        }
        CEGUI_CATCH(XMLException& exc)
        {
            // prepare a message about the failure
            char* excmsg = XMLString::transcode(exc.getMessage());
            String message("An exception occurred while initialising the Xerces-C XML system.  Additional information: ");
            message += excmsg;
            XMLString::release(&excmsg);

            // throw a C string (because it won't try and use logger, which may not be available)
            CEGUI_THROW(message.c_str());
        }

        return true;
    }

    void XercesParser::cleanupImpl(void)
    {
        // cleanup XML stuff
        XERCES_CPP_NAMESPACE_USE;
        XMLPlatformUtils::Terminate();
    }
    
    void XercesParser::populateAttributesBlock(const XERCES_CPP_NAMESPACE::Attributes& src, XMLAttributes& dest)
    {
        XERCES_CPP_NAMESPACE_USE;
        String attributeName;
        String attributeValue;

        for (uint i = 0; i < src.getLength(); ++i)
        {
            // TODO dalfy: Optimize this using temporary value. 
            attributeName = transcodeXmlCharToString(src.getLocalName(i), XMLString::stringLen(src.getLocalName(i)));
            attributeValue = transcodeXmlCharToString(src.getValue(i), XMLString::stringLen(src.getValue(i)));
            dest.add(attributeName, attributeValue);
        }
    }

    String XercesParser::transcodeXmlCharToString(const XMLCh* const xmlch_str, unsigned int inputLength)
    {
        XERCES_CPP_NAMESPACE_USE;

        XMLTransService::Codes  res;
        XMLTranscoder* transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XMLRecognizer::UTF_8, res, 4096, XMLPlatformUtils::fgMemoryManager );

        if (res == XMLTransService::Ok)
        {
            String out;
#if _XERCES_VERSION >= 30000
            XMLByte outBuff[128];
            XMLSize_t outputLength;
            XMLSize_t eaten = 0;
            XMLSize_t offset = 0;
#else /* _XERCES_VERSION >= 30000 */
            utf8 outBuff[128];
            unsigned int outputLength;
            unsigned int eaten = 0;
            unsigned int offset = 0;
#endif /* _XERCES_VERSION >= 30000 */
//            unsigned int inputLength = XMLString::stringLen(xmlch_str); // dalfy caracters node need to transcode but give the size 

            while (inputLength)
            {
                outputLength = transcoder->transcodeTo(xmlch_str + offset, inputLength, outBuff, 128, eaten, XMLTranscoder::UnRep_RepChar);
                out.append((encoded_char*)outBuff, outputLength);
                offset += eaten;
                inputLength -= eaten;
            }

            delete transcoder;

            return out;
        }
        else
        {
            CEGUI_THROW(GenericException("Internal Error: Could not create UTF-8 string transcoder."));
        }

    }

    void XercesParser::initialiseSchema(XERCES_CPP_NAMESPACE::SAX2XMLReader* reader, const String& schemaName)
    {
        XERCES_CPP_NAMESPACE_USE;

        // enable schema use and set validation options
        reader->setFeature(XMLUni::fgXercesSchema, true);
        reader->setFeature(XMLUni::fgSAX2CoreValidation, true);
        reader->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

        // load in the raw schema data
        RawDataContainer rawSchemaData;
        // load the schema from the resource group
        Logger::getSingleton().logEvent("XercesParser::initialiseSchema - Attempting to load schema from file '" + schemaName + "'.");
        System::getSingleton().getResourceProvider()->loadRawDataContainer(schemaName, rawSchemaData, d_defaultSchemaResourceGroup);

        // wrap schema data in a xerces MemBufInputSource object
        MemBufInputSource  schemaData(
            rawSchemaData.getDataPtr(),
            static_cast<const unsigned int>(rawSchemaData.getSize()),
            schemaName.c_str(),
            false);
        reader->loadGrammar(schemaData, Grammar::SchemaGrammarType, true);
        // enable grammar reuse
        reader->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);

        // set schema for usage
        XMLCh* pval = XMLString::transcode(schemaName.c_str());
        reader->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);
        XMLString::release(&pval);
        Logger::getSingleton().logEvent("XercesParser::initialiseSchema - XML schema file '" + schemaName + "' has been initialised.");

        // use resource provider to release loaded schema data (if it supports this)
        System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawSchemaData);
    }

    XERCES_CPP_NAMESPACE::SAX2XMLReader* XercesParser::createReader(XERCES_CPP_NAMESPACE::DefaultHandler& handler)
    {
        XERCES_CPP_NAMESPACE_USE;

        SAX2XMLReader* reader = XMLReaderFactory::createXMLReader();

        // set basic settings we want from parser
        reader->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);

        // set handlers
        reader->setContentHandler(&handler);
        reader->setErrorHandler(&handler);

        return reader;
    }

    void XercesParser::doParse(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser, const RawDataContainer& source)
    {
        XERCES_CPP_NAMESPACE_USE;

        MemBufInputSource  fileData(
            source.getDataPtr(),
            static_cast<const unsigned int>(source.getSize()),
            "Unknown",
            false);

         parser->parse(fileData);
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    //
    // XercesHandler methods
    //
    ////////////////////////////////////////////////////////////////////////////////

    XercesHandler::XercesHandler(XMLHandler& handler) :
            d_handler(handler)
    {}

    XercesHandler::~XercesHandler(void)
    {}

    void XercesHandler::startElement(const XMLCh* const /*uri*/, const XMLCh* const localname, const XMLCh* const /*qname*/, const XERCES_CPP_NAMESPACE::Attributes& attrs)
    {
        XERCES_CPP_NAMESPACE_USE;
        XMLAttributes cegui_attributes;
        XercesParser::populateAttributesBlock(attrs, cegui_attributes);
        String element(XercesParser::transcodeXmlCharToString(localname, XMLString::stringLen(localname)));
        d_handler.elementStart(element, cegui_attributes);
    }

    void XercesHandler::endElement(const XMLCh* const /*uri*/, const XMLCh* const localname, const XMLCh* const /*qname*/)
    {
        XERCES_CPP_NAMESPACE_USE;
        String element(XercesParser::transcodeXmlCharToString(localname,XMLString::stringLen(localname)));
        d_handler.elementEnd(element);
    }

#if _XERCES_VERSION >= 30000
    void XercesHandler::characters(const XMLCh* const chars, const XMLSize_t length)
#else /* _XERCES_VERSION >= 30000 */
    void XercesHandler::characters (const XMLCh *const chars, const unsigned int length)
#endif /* _XERCES_VERSION >= 30000 */
    {
        d_handler.text(XercesParser::transcodeXmlCharToString(chars, length));
    }

    void XercesHandler::warning (const XERCES_CPP_NAMESPACE::SAXParseException &exc)
    {
        XERCES_CPP_NAMESPACE_USE;

        // prepare a message about the warning
        char* excmsg = XMLString::transcode(exc.getMessage());
        String message("Xerces warning: ");
        message += excmsg;
        XMLString::release(&excmsg);
        Logger::getSingleton().logEvent(message);
    }

    void XercesHandler::error (const XERCES_CPP_NAMESPACE::SAXParseException &exc)
    {
        CEGUI_THROW(exc);
    }

    void XercesHandler::fatalError (const XERCES_CPP_NAMESPACE::SAXParseException &exc)
    {
        CEGUI_THROW(exc);
    }

} // End of  CEGUI namespace section
