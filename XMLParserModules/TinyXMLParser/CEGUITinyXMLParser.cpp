/************************************************************************
    filename:   CEGUITinyXMLParser.cpp
    created:    Sun Mar 13 2005
    author:     Paul D Turner
*************************************************************************/
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
#include "CEGUITinyXMLParser.h"
#include "CEGUIResourceProvider.h"
#include "CEGUISystem.h"
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUILogger.h"
#include "ceguitinyxml/tinyxml.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    class TinyXMLDocument : public CEGUITinyXML::TiXmlDocument
    {
    public:
        TinyXMLDocument(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup);
        ~TinyXMLDocument()
        {}
    protected:
        void processElement(const CEGUITinyXML::TiXmlElement* element);

    private:
        XMLHandler* d_handler;
    };

    TinyXMLDocument::TinyXMLDocument(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup)
    {
        d_handler = &handler;

        // use resource provider to load file data
        // TODO: Fix null termination issue.
        RawDataContainer rawXMLData;
        System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, rawXMLData, resourceGroup);

        CEGUITinyXML::TiXmlDocument doc;
        doc.Parse((const char*)rawXMLData.getDataPtr());

        const CEGUITinyXML::TiXmlElement* currElement = doc.RootElement();

        if (currElement)
        {
            // function called recursively to parse xml data
            processElement(currElement);
        }

        System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
    }
    

    void TinyXMLDocument::processElement(const CEGUITinyXML::TiXmlElement* element)
    {
        // build attributes block for the element
        XMLAttributes attrs;
        
        const CEGUITinyXML::TiXmlAttribute *currAttr = element->FirstAttribute();
        while (currAttr)
        {
            attrs.add(currAttr->Name(), currAttr->Value());
            currAttr = currAttr->Next();
        }
        
        // start element
        d_handler->elementStart(element->Value(), attrs);

        // do children
        const CEGUITinyXML::TiXmlNode* childNode = element->FirstChild();
        while (childNode)
        {
            switch(childNode->Type())
            {
            case CEGUITinyXML::TiXmlNode::ELEMENT:
                processElement(childNode->ToElement());
                break;
            case CEGUITinyXML::TiXmlNode::TEXT:
                if (childNode->ToText()->Value() != '\0')
                    d_handler->text((utf8*)childNode->ToText()->Value());
                break;
                
                // Silently ignore unhandled node type 
            };
            childNode = childNode->NextSibling();
        }

        // end element
        d_handler->elementEnd(element->Value());
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
        //CEGUITinyXML::TiXmlDocument::SetCondenseWhiteSpace(false);
        return true;
    }

    void TinyXMLParser::cleanupImpl(void)
    {}

} // End of  CEGUI namespace section
