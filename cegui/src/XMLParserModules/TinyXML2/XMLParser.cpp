/***********************************************************************
    filename:   XMLParser.cpp
    created:    Fri Sep 02 2016
    author:     Vladimir "Niello" Orlov
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/XMLParserModules/TinyXML2/XMLParser.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/XMLHandler.h"
#include "CEGUI/XMLAttributes.h"
#include <tinyxml2.h>

namespace CEGUI
{
    TinyXML2Parser::TinyXML2Parser()
    {
        d_identifierString = "CEGUI::TinyXML2Parser - Official TinyXML2 based parser module for CEGUI";
    }

    void TinyXML2Parser::parseXML(XMLHandler& handler, const RawDataContainer& source, const String& /*schemaName*/, bool /*allowXmlValidation*/)
    {
        // Parse the document
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError Result = doc.Parse(reinterpret_cast<const char*>(source.getDataPtr()), source.getSize());
        if (Result == tinyxml2::XML_ERROR_EMPTY_DOCUMENT) return;

        // Check if any actual error occurred
        if (Result != tinyxml2::XML_SUCCESS)
        {
            throw FileIOException("TinyXML2Parser: an error occurred while "
                "parsing the XML document '...' - check it for potential errors!");
        }

        // Parse root element recursively
        const tinyxml2::XMLElement* currElement = doc.RootElement();
        if (currElement)
            processElement(handler, currElement);
    }

    void TinyXML2Parser::processElement(XMLHandler& handler, const tinyxml2::XMLElement* element)
    {
        // Build attributes block for the element
        XMLAttributes attrs;

        const tinyxml2::XMLAttribute *currAttr = element->FirstAttribute();
        while (currAttr)
        {
            attrs.add(currAttr->Name(), currAttr->Value());
            currAttr = currAttr->Next();
        }

        handler.elementStart(element->Value(), attrs);

        const tinyxml2::XMLNode* pChildNode = element->FirstChild();
        while (pChildNode)
        {
            if (pChildNode->ToText())
            {
                const char* pValue = pChildNode->ToText()->Value();
                if (pValue && *pValue)
                    handler.text(pValue);
            }
            else if (pChildNode->ToElement())
                processElement(handler, pChildNode->ToElement());
            
            // Silently ignore unhandled node type

            pChildNode = pChildNode->NextSibling();
        }

        handler.elementEnd(element->Value());
    }
    
    bool TinyXML2Parser::initialiseImpl()
    {
        return true;
    }

    void TinyXML2Parser::cleanupImpl()
    {
    }

} // End of CEGUI namespace section
