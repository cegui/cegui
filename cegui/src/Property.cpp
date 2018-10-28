/***********************************************************************
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements Property base class
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
#include "CEGUI/Property.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String Property::XMLElementName("Property");
const String Property::NameXMLAttributeName("name");
const String Property::ValueXMLAttributeName("value");

//----------------------------------------------------------------------------//
bool Property::isDefault(const PropertyReceiver* receiver) const
{
	return (get(receiver) == getDefault(receiver));
}

//----------------------------------------------------------------------------//
String Property::getDefault(const PropertyReceiver*) const
{
	return d_default;
}

//----------------------------------------------------------------------------//
void Property::writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const
{
	if (d_writeXML)
	{
		xml_stream.openTag(XMLElementName)
			.attribute(NameXMLAttributeName,  d_name);
		// Detect wether it is a long property or not
		// Long property are needed if
		const String& value = get(receiver);
		if (value.find(static_cast<String::value_type>('\n')) != String::npos)
		{
			xml_stream.text(value);
		}
		else
		{
			xml_stream.attribute(ValueXMLAttributeName, get(receiver));
		}
		xml_stream.closeTag();
	}
}

//----------------------------------------------------------------------------//
bool Property::isReadable() const
{
	return true;
}

//----------------------------------------------------------------------------//
bool Property::isWritable() const
{
	return true;
}

//----------------------------------------------------------------------------//
bool Property::doesWriteXML() const
{
    return d_writeXML;
}

} // End of  CEGUI namespace section
