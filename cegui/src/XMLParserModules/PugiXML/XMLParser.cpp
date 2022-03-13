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
#include "CEGUI/XMLParserModules/PugiXML/XMLParser.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/XMLHandler.h"
#include <pugixml.hpp>

namespace CEGUI
{

//----------------------------------------------------------------------------//
PugiXMLParser::PugiXMLParser()
{
    d_identifierString = "CEGUI::PugiXMLParser - Official PugiXML based parser module for CEGUI";
}

//----------------------------------------------------------------------------//
static void processNode(XMLHandler& handler, pugi::xml_node node)
{
    const String nodeName = node.name();

    {
        XMLAttributes attrs;
        for (auto attr : node.attributes())
            attrs.add(attr.name(), attr.value());

        handler.elementStart(nodeName, attrs);
    }

    for (auto childNode : node.children())
    {
        switch (childNode.type())
        {
            case pugi::xml_node_type::node_element:
                processNode(handler, childNode);
                break;
            case pugi::xml_node_type::node_pcdata:
                if (*childNode.value())
                    handler.text(childNode.value());
                break;
        }
    }

    handler.elementEnd(nodeName);
}

//----------------------------------------------------------------------------//
void PugiXMLParser::parseXML(XMLHandler& handler,
                              const RawDataContainer& source,
                              const String& /*schemaName*/,
							  bool /*allowXmlValidation*/)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(source.getDataPtr(), source.getSize());
    if (!result)
        throw FileIOException("PugiXMLParser: an error occurred while "
            "parsing the XML document: " + String(result.description()));

    if (doc.root())
        processNode(handler, doc.root());
}

//----------------------------------------------------------------------------//
bool PugiXMLParser::initialiseImpl()
{
    // This used to prevent deletion of line ending in the middle of a text.
    return true;
}

//----------------------------------------------------------------------------//
void PugiXMLParser::cleanupImpl()
{
}

}
