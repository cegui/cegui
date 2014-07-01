/***********************************************************************
    created:    Mar 27 2010
    author:     Jay Sprenkle
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "config.h"
#endif

#include "rapidxml.hpp"

#include "CEGUI/XMLParserModules/RapidXML/XMLParser.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/System.h"
#include "CEGUI/XMLHandler.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
class RapidXMLDocument : public rapidxml::xml_document<>
{
public:
    RapidXMLDocument(XMLHandler& handler, const RawDataContainer& source,
                     const String& schemaName);

    ~RapidXMLDocument()
    {
    }

protected:
    void processElement(const rapidxml::xml_node<>* element);

private:
    XMLHandler* d_handler;
};

//----------------------------------------------------------------------------//
RapidXMLDocument::RapidXMLDocument(XMLHandler& handler,
                                   const RawDataContainer& source,
                                   const String& /*schemaName*/)
{
    d_handler = &handler;

    // Create a buffer with extra bytes for a newline and a terminating null
    size_t size = source.getSize();
    char* buf = new char[size + 2];
    memcpy(buf, source.getDataPtr(), size);
    // PDT: The addition of the newline is a kludge to resolve an issue
    // whereby parse returns 0 if the xml file has no newline at the end but
    // is otherwise well formed.
    buf[size] = '\n';
    buf[size + 1] = 0;

    // Parse the document
    rapidxml::xml_document<> doc;    // character type defaults to char

    try
    {
        doc.parse<0>(buf);          // 0 means default parse flags
    }
    catch (...)
    {
        // error detected, cleanup out buffers
        delete[] buf;
        CEGUI_THROW(FileIOException("an error occurred while "
                              "parsing the XML data - check it for "
                              "potential errors!."));
    }

    rapidxml::xml_node<>* currElement = doc.first_node();

    if (currElement)
    {
        try
        {
            // function called recursively to parse xml data
            processElement(currElement);
        }
        catch (...)
        {
            delete[] buf;
            throw;
        }
    }

    // Free memory
    delete[] buf;
}

//----------------------------------------------------------------------------//
void RapidXMLDocument::processElement(const rapidxml::xml_node<>* element)
{
    // build attributes block for the element
    XMLAttributes attrs;

    rapidxml::xml_attribute<>* currAttr = element->first_attribute(0);

    while (currAttr)
    {
        attrs.add(reinterpret_cast<encoded_char*>(currAttr->name()),
                  reinterpret_cast<encoded_char*>(currAttr->value()));
        currAttr = currAttr->next_attribute();
    }

    // start element
    d_handler->elementStart(reinterpret_cast<encoded_char*>(element->name()),
                            attrs);

    // do children
    rapidxml::xml_node<>* childNode = element->first_node();

    while (childNode)
    {
        switch (childNode->type())
        {
        case rapidxml::node_element:
            processElement(childNode);
            break;

        case rapidxml::node_data:
            if (childNode->value() != '\0')
                d_handler->text(
                    reinterpret_cast<encoded_char*>(childNode->value()));

            break;

            // Silently ignore unhandled node type
        };

        childNode = childNode->next_sibling();
    }


    // end element
    d_handler->elementEnd(reinterpret_cast<encoded_char*>(element->name()));
}

//----------------------------------------------------------------------------//
RapidXMLParser::RapidXMLParser(void)
{
    // set ID string
    d_identifierString = "CEGUI::RapidXMLParser - "
                         "Official RapidXML based parser module for CEGUI";
}

//----------------------------------------------------------------------------//
RapidXMLParser::~RapidXMLParser(void)
{
}

//----------------------------------------------------------------------------//
void RapidXMLParser::parseXML(XMLHandler& handler,
                              const RawDataContainer& source,
                              const String& schemaName)
{
    RapidXMLDocument doc(handler, source, schemaName);
}

//----------------------------------------------------------------------------//
bool RapidXMLParser::initialiseImpl(void)
{
    // This used to prevent deletion of line ending in the middle of a text.
    // WhiteSpace cleaning will be available throught the use of String methods
    // directly: rapidxml::xml_document<>::SetCondenseWhiteSpace(false);
    return true;
}

//----------------------------------------------------------------------------//
void RapidXMLParser::cleanupImpl(void)
{
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

