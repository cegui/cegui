/***********************************************************************
    filename:   CEGUITinyXMLParser.cpp
    created:    Sun Mar 13 2005
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUITinyXMLParser.h"
#include "CEGUIResourceProvider.h"
#include "CEGUISystem.h"
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUILogger.h"
#include "CEGUIExceptions.h"
#include CEGUI_TINYXML_H

// Start of CEGUI namespace section
namespace CEGUI
{
    class TinyXMLDocument : public CEGUI_TINYXML_NAMESPACE::TiXmlDocument
    {
    public:
        TinyXMLDocument(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup);
        ~TinyXMLDocument()
        {}
    protected:
        void processElement(const CEGUI_TINYXML_NAMESPACE::TiXmlElement* element);

    private:
        XMLHandler* d_handler;
    };

    TinyXMLDocument::TinyXMLDocument(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup)
    {
        d_handler = &handler;

        // use resource provider to load file data
        // Fixed using patch from tracker item #000057
        // cegui_mk2-0.4.1-fixtinyxml.patch
        RawDataContainer rawXMLData;
        System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, rawXMLData, resourceGroup);

        // Create a buffer with extra bytes for a newline and a terminating null
        size_t size = rawXMLData.getSize();
        char* buf = new char[size + 2];
        memcpy(buf, rawXMLData.getDataPtr(), size);
        // PDT: The addition of the newline is a kludge to resolve an issue
        // whereby parse returns 0 if the xml file has no newline at the end but
        // is otherwise well formed.
        buf[size] = '\n';
        buf[size+1] = 0;

        // Parse the document
        CEGUI_TINYXML_NAMESPACE::TiXmlDocument doc;
        if (!doc.Parse((const char*)buf))
        {
            // error detected, cleanup out buffers
            delete[] buf;
            System::getSingleton().getResourceProvider()->
                unloadRawDataContainer(rawXMLData);
            // throw exception
            throw FileIOException("TinyXMLParser: an error occurred while "
                                  "parsing the XML document '" + filename +
                                  "' - check it for potential errors!.");
        }

        const CEGUI_TINYXML_NAMESPACE::TiXmlElement* currElement = doc.RootElement();
        if (currElement)
        {
            try
            {
                // function called recursively to parse xml data
                processElement(currElement);
            }
            catch(...)
            {
                delete [] buf;
                System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
                throw;
            }
        } // if (currElement)

        // Free memory
        delete [] buf;
        System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
    }


    void TinyXMLDocument::processElement(const CEGUI_TINYXML_NAMESPACE::TiXmlElement* element)
    {
        // build attributes block for the element
        XMLAttributes attrs;

        const CEGUI_TINYXML_NAMESPACE::TiXmlAttribute *currAttr = element->FirstAttribute();
        while (currAttr)
        {
            attrs.add((utf8*)currAttr->Name(), (utf8*)currAttr->Value());
            currAttr = currAttr->Next();
        }

        // start element
        d_handler->elementStart((utf8*)element->Value(), attrs);

        // do children
        const CEGUI_TINYXML_NAMESPACE::TiXmlNode* childNode = element->FirstChild();
        while (childNode)
        {
            switch(childNode->Type())
            {
            case CEGUI_TINYXML_NAMESPACE::TiXmlNode::ELEMENT:
                processElement(childNode->ToElement());
                break;
            case CEGUI_TINYXML_NAMESPACE::TiXmlNode::TEXT:
                if (childNode->ToText()->Value() != '\0')
                    d_handler->text((utf8*)childNode->ToText()->Value());
                break;

                // Silently ignore unhandled node type
            };
            childNode = childNode->NextSibling();
        }

        // end element
        d_handler->elementEnd((utf8*)element->Value());
    }

    TinyXMLParser::TinyXMLParser(void)
    {
        // set ID string
        d_identifierString = "CEGUI::TinyXMLParser - Official tinyXML based parser module for CEGUI";
    }

    TinyXMLParser::~TinyXMLParser(void)
    {}

    void TinyXMLParser::parseXMLFile(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup)
    {
      TinyXMLDocument doc(handler, filename, schemaName, resourceGroup);
    }


    bool TinyXMLParser::initialiseImpl(void)
    {
        // This used to prevent deletion of line ending in the middle of a text.
        // WhiteSpace cleaning will be available throught the use of String methods directly
        //CEGUI_TINYXML_NAMESPACE::TiXmlDocument::SetCondenseWhiteSpace(false);
        return true;
    }

    void TinyXMLParser::cleanupImpl(void)
    {}

} // End of  CEGUI namespace section
