/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "CEGUIString.h"
#include "CEGUIXmlHandlerHelper.h"
#include "CEGUIExceptions.h"

using XERCES_CPP_NAMESPACE::XMLString;
using XERCES_CPP_NAMESPACE::XMLTransService;
using XERCES_CPP_NAMESPACE::XMLPlatformUtils;
using XERCES_CPP_NAMESPACE::XMLTranscoder;
using XERCES_CPP_NAMESPACE::XMLRecognizer;


namespace CEGUI 
{

const XMLCh* XmlHandlerHelper::getAttributeValueAsXmlChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	XMLCh* attr_name = XMLString::transcode(attributeName);
	const XMLCh* val_str = attrs.getValue(attr_name);
	XMLString::release(&attr_name);

	return val_str;
}

char* XmlHandlerHelper::getAttributeValueAsChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	return XMLString::transcode(getAttributeValueAsXmlChar(attrs, attributeName));
}

int XmlHandlerHelper::getAttributeValueAsInteger(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	return XMLString::parseInt(getAttributeValueAsXmlChar(attrs, attributeName));
}

String XmlHandlerHelper::getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
{
	return transcodeXmlCharToString(getAttributeValueAsXmlChar(attrs, attributeName));
}

String XmlHandlerHelper::transcodeXmlCharToString(const XMLCh* const xmlch_str)
{
	XMLTransService::Codes	res;
	XMLTranscoder* transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XMLRecognizer::UTF_8, res, 4096, XMLPlatformUtils::fgMemoryManager );

	if (res == XMLTransService::Ok)
	{
		String out;
		utf8 outBuff[128];
		unsigned int outputLength;
		unsigned int eaten = 0;
		unsigned int offset = 0;
		unsigned int inputLength = XMLString::stringLen(xmlch_str);

		while (inputLength)
		{
			outputLength = transcoder->transcodeTo(xmlch_str + offset, inputLength, outBuff, 128, eaten, XMLTranscoder::UnRep_RepChar);
			out.append(outBuff, outputLength);
			offset += eaten;
			inputLength -= eaten;
		}

		delete transcoder;

		return out;
	}
	else
	{
		throw GenericException((utf8*)"XmlHandlerHelper::transcodeXmlCharToString - Internal Error: Could not create UTF-8 string transcoder.");
	}

}

}
