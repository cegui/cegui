/***********************************************************************
    created:    12/3/2005
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
#include "CEGUI/XMLParser.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/System.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/Logger.h"

#include <locale.h>  
#include <cstring>

// Start of CEGUI namespace section
namespace CEGUI
{

    XMLParser::XMLParser(void) :
            d_identifierString("Unknown XML parser (vendor did not set the ID string!)"),
            d_initialised(false)
    {}

    XMLParser::~XMLParser(void)
    {}

    bool XMLParser::initialise(void)
    {
        // do this to ensure only one initialise call is made
        if (!d_initialised)
        {
            d_initialised = initialiseImpl();
        }

        return d_initialised;
    }

    void XMLParser::parseXMLFile(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup, bool allowXmlValidation)
    {
        // Acquire resource using CEGUI ResourceProvider
        RawDataContainer rawXMLData;
        System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, rawXMLData, resourceGroup);

        try
        {
            // The actual parsing action (this is overridden and depends on the specific parser)
            parseXML(handler, rawXMLData, schemaName, allowXmlValidation);
        }
        catch (const Exception&)
        {
            // hint the related file name in the log
            Logger::getSingleton().logEvent("The last thrown exception was related to XML file '" +
                                            filename + "' from resource group '" + resourceGroup + "'.", LoggingLevel::Error);

            // exception safety
            System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);

            throw;
        }

        // Release resource
        System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
    }

    void XMLParser::parseXMLString(XMLHandler& handler, const String& source, const String& schemaName, bool allowXmlValidation)
    {
        // Put the source string into a RawDataContainer
        RawDataContainer rawXMLData;

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
        const char* c_str = source.c_str();
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
        std::string utf8StdStr = String::convertUtf32ToUtf8(source.getString());
        const char* c_str = utf8StdStr.c_str();
#endif
        rawXMLData.setData(reinterpret_cast<std::uint8_t*>(const_cast<char*>(c_str)));
        rawXMLData.setSize(std::strlen(c_str));

        try
        {
        	// The actual parsing action (this is overridden and depends on the specific parser)
        	parseXML(handler, rawXMLData, schemaName, allowXmlValidation);
        }
        catch(...)
        {
        	// make sure we don't allow rawXMLData to release String owned data no matter what!
        	rawXMLData.setData(nullptr);
			rawXMLData.setSize(0);

			throw;
        }

        // !!! We must not allow DataContainer to delete String owned data,
        //     therefore, we set it's data to 0 to avoid double-deletion
        rawXMLData.setData(nullptr);
        rawXMLData.setSize(0);
    }

    void XMLParser::cleanup(void)
    {
        if (d_initialised)
        {
            cleanupImpl();
            d_initialised = false;
        }
    }

    const String& XMLParser::getIdentifierString() const
    {
        return d_identifierString;
    }

} // End of  CEGUI namespace section
