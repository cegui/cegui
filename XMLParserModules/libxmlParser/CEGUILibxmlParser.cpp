/************************************************************************
    filename:   CEGUILibxmlParser.cpp
    created:    Mon Mar 6 2006
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)

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
#include "CEGUILibxmlParser.h"
#include "CEGUISystem.h"
#include "CEGUIResourceProvider.h"
#include "CEGUIExceptions.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIXMLHandler.h"
#include "CEGUIPropertyHelper.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlerror.h>

// Start of CEGUI namespace section
namespace CEGUI
{
// internal helper function to process elements
void processXMLElement(XMLHandler& handler, xmlNode* node)
{
    // build attributes block for the element
    XMLAttributes attrs;

    xmlAttrPtr currAttr = node->properties;
    while (currAttr)
    {
        xmlChar* val = xmlGetProp(node, currAttr->name);
        attrs.add(currAttr->name, val);
        xmlFree(val);
        currAttr = currAttr->next;
    }

    // element start processing
    handler.elementStart(node->name, attrs);

    for (xmlNode* cur_node = node->children; cur_node; cur_node = cur_node->next)
    {
        switch(cur_node->type)
        {
        case XML_ELEMENT_NODE:
            processXMLElement(handler, cur_node);
            break;

        case XML_TEXT_NODE:
            if (cur_node->content != 0 && *cur_node->content!= '\0')
                handler.text(cur_node->content);
            break;
        }
    }

    // element end processing
    handler.elementEnd(node->name);
}

LibxmlParser::LibxmlParser(void)
{
    d_identifierString = "CEGUI::LibxmlParser - Official libxml2 based parser module for CEGUI";
}

LibxmlParser::~LibxmlParser(void)
{
}

void LibxmlParser::parseXMLFile(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup)
{
    // load xml file data into buffer using resource provider
    CEGUI::RawDataContainer rawXMLData;
    CEGUI::System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, rawXMLData, resourceGroup);

    xmlDocPtr doc = xmlParseMemory(
        reinterpret_cast<char*>(rawXMLData.getDataPtr()),
        rawXMLData.getSize());

    // release loaded xml data.
    System::getSingleton().getResourceProvider()->
        unloadRawDataContainer(rawXMLData);

    if (!doc)
    {
        xmlError* err = xmlGetLastError();

        throw GenericException(
            String("LibxmlParser::parseXMLFile - xmlParseMemory failed in file: '") +
            err->file + "' at line number" +
            PropertyHelper::uintToString(err->line) + ".  Error is:" +
            err->message);
    }

    // get root element
    xmlNode* root = xmlDocGetRootElement(doc);

    // process all elements from root to end of doc
    processXMLElement(handler, root);

    // release the xmlDoc 
    xmlFreeDoc(doc);
}

bool LibxmlParser::initialiseImpl(void)
{
    LIBXML_TEST_VERSION;
    return true;
}

void LibxmlParser::cleanupImpl(void)
{
    xmlCleanupParser();
}

} // End of  CEGUI namespace section
