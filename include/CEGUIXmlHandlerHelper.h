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
