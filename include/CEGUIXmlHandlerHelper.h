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
#ifndef _CEGUIXmlHandlerHelper_h_
#define _CEGUIXmlHandlerHelper_h_

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include "xercesc/sax2/DefaultHandler.hpp"

namespace CEGUI 
{
/*!
\brief
	Internal helper class to extract attribute values from xerces-c attribute blocks.
*/
class XmlHandlerHelper
{
public:
    /*!
    \brief
        Parses a file using the given handler object.

    \param handler
        Xerces-C++ DefaultHandler based object which will process the parsed xml data.

    \param schemaName
        String holding the name of the .xsd schema which should be used for validation of the
        xml data.

    \param xmlFilename
        String holding the name of the xml file which is to be parsed.

    \param resourceGroup
        String holding the resource group identifer which will be passed to the ResourceProvider
        when loading the xml and schema files.

    \return
        Nothing.
    */
    static void parseXMLFile(XERCES_CPP_NAMESPACE::DefaultHandler& handler, const String& schemaName, const String& xmlFilename, const String& resourceGroup);

    /*!
    \brief
        Parses a file using the given, pre initialised, SAX2XMLReader object.

    \param parser
        Pointer to a correctly initialised Xerces-C++ SAX2XMLReader object which will
        control the xml parse operation.

    \param xmlFilename
        String holding the name of the xml file which is to be parsed.

    \param resourceGroup
        String holding the resource group identifer which will be passed to the ResourceProvider
        when loading the xml file.

    \return
        Nothing.
    */
    static void parseXMLFile(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser, const String& xmlFilename, const String& resourceGroup);

    /*!
    \brief
        Creates and initialises a xerces SAX2XMLReader for use by the xml parsing methods
        in CEGUI.

    \param handler
        Xerces-C++ DefaultHandler based object which will process the parsed xml data.

    \return
        Pointer to a SAX2XMLReader initialised to use the given handler object.
    */
    static XERCES_CPP_NAMESPACE::SAX2XMLReader* createParser(XERCES_CPP_NAMESPACE::DefaultHandler& handler);

    /*!
    brief
        Loads and initialises the schema to be used for the next parse.

    \param parser
        Pointer to a Xerces-C++ SAX2XMLReader object which will have its schema initialised.

    \param schemaName
        String holding the name of the .xsd schema which is to be initialised for use with
        the parser.

    \param xmlFilename
        String holding the name of the xml file which is to be parsed.  This is technically
        optional and is used to extract a possible alternate path where the .xsd schema file
        may be located.

    \param resourceGroup
        String holding the resource group identifer which will be passed to the ResourceProvider
        when loading the schema file.

    \return
        Nothing.
    */
    static void initialiseSchema(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser, const String& schemaName, const String& xmlFilename, const String& resourceGroup);
    
    /*!
	\brief
		Return a pointer to a character array that contains the value of the requested attribute from the given attributes collection.

	\param attrs
		Xerces-c Attributes collection.

	\param attributeName
		character array holding the name of the attribute to be accessed.

	\return
		pointer to a character array holding the string value of \a attributeName.  Note that the returned pointer is
		owned by xerces-c and should be released by using XMLString::release when it is no longer needed.
	*/
	static char* getAttributeValueAsChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);


	/*!
	\brief
		Return a pointer to a XMLCh array that contains the value of the requested attribute from the given attributes collection.

	\param attrs
		Xerces-c Attributes collection.

	\param attributeName
		character array holding the name of the attribute to be accessed.

	\return
		pointer to a XMLCh array holding the string value of \a attributeName.  Note that the returned pointer is
		owned by xerces-c and should be released by using XMLString::release when it is no longer needed.
	*/
	static const XMLCh* getAttributeValueAsXmlChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);


	/*!
	\brief
		Return an integer equal to the value of the requested attribute from the given attributes collection.

	\param attrs
		Xerces-c Attributes collection.

	\param attributeName
		character array holding the name of the attribute to be accessed.

	\return
		integer value of the requested attribute.
	*/
	static int getAttributeValueAsInteger(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);


	/*!
	\brief
		Return a String object that contains the value of the requested attribute from the given attributes collection.

	\param attrs
		Xerces-c Attributes collection.

	\param attributeName
		character array holding the name of the attribute to be accessed.

	\return
		String object containing the text of the attribute value requested.
	*/
	static String getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);


	/*!
	\brief
		Return a string object containing data transcoded from the given XMLCh array

	\param xmlch_str
		Pointer to an XMLCh array holding the source string to be transcoded.

	\return
		String object containing the transcoded text string.
	*/
	static String transcodeXmlCharToString(const XMLCh* const xmlch_str);
};

}

#endif
