/***********************************************************************
    filename:   CEGUILibxmlParser.cpp
    created:    Mon Mar 6 2006
    author:     Paul D Turner <paul@cegui.org.uk>
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
        attrs.add((utf8*)currAttr->name, (utf8*)val);
        xmlFree(val);
        currAttr = currAttr->next;
    }

    // element start processing
    handler.elementStart((utf8*)node->name, attrs);

    for (xmlNode* cur_node = node->children; cur_node; cur_node = cur_node->next)
    {
        switch(cur_node->type)
        {
        case XML_ELEMENT_NODE:
            processXMLElement(handler, cur_node);
            break;

        case XML_TEXT_NODE:
            if (cur_node->content != 0 && *cur_node->content!= '\0')
                handler.text((utf8*)cur_node->content);
            break;
        }
    }

    // element end processing
    handler.elementEnd((utf8*)node->name);
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
