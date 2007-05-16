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
        // Fixed using patch from tracker item #000057
        // cegui_mk2-0.4.1-fixtinyxml.patch
        RawDataContainer rawXMLData;
        System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, rawXMLData, resourceGroup);
        
        // Create a buffer with the missing extra byte 
        size_t size = rawXMLData.getSize();
        char* buf = new char[size + 1];
        memcpy(buf, rawXMLData.getDataPtr(), size);
        buf[size] = 0; 

		try 
		{
			// Parse the document 
			CEGUITinyXML::TiXmlDocument doc;
			doc.Parse((const char*)buf);
			const CEGUITinyXML::TiXmlElement* currElement = doc.RootElement();
			if (currElement)
			{
				// function called recursively to parse xml data
				processElement(currElement);
			} // if (currElement)
		}
		catch(...)
		{
			delete [] buf;
			System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
			throw;
		}
		// Free memory 
        delete [] buf;
        System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
    }
    

    void TinyXMLDocument::processElement(const CEGUITinyXML::TiXmlElement* element)
    {
        // build attributes block for the element
        XMLAttributes attrs;
        
        const CEGUITinyXML::TiXmlAttribute *currAttr = element->FirstAttribute();
        while (currAttr)
        {
            attrs.add((utf8*)currAttr->Name(), (utf8*)currAttr->Value());
            currAttr = currAttr->Next();
        }
        
        // start element
        d_handler->elementStart((utf8*)element->Value(), attrs);

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
        //CEGUITinyXML::TiXmlDocument::SetCondenseWhiteSpace(false);
        return true;
    }

    void TinyXMLParser::cleanupImpl(void)
    {}

} // End of  CEGUI namespace section
