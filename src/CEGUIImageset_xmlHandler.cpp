/************************************************************************
filename: 	CEGUIImageset_xmlHandler.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements the Imageset class
*************************************************************************/
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
#include "CEGUIImageset_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUILogger.h"
#include "CEGUIXmlHandlerHelper.h"

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Definition of constant data for Imageset (and sub-classes)
*************************************************************************/
// Declared in Imageset::xmlHandler
const utf8	Imageset_xmlHandler::ImagesetElement[]					= "Imageset";
const utf8	Imageset_xmlHandler::ImageElement[]						= "Image";
const char	Imageset_xmlHandler::ImagesetImageFileAttribute[]		= "Imagefile";
const char	Imageset_xmlHandler::ImagesetNameAttribute[]			= "Name";
const char	Imageset_xmlHandler::ImagesetNativeHorzResAttribute[]	= "NativeHorzRes";
const char	Imageset_xmlHandler::ImagesetNativeVertResAttribute[]	= "NativeVertRes";
const char	Imageset_xmlHandler::ImagesetAutoScaledAttribute[]		= "AutoScaled";
const char	Imageset_xmlHandler::ImageNameAttribute[]				= "Name";
const char	Imageset_xmlHandler::ImageXPosAttribute[]				= "XPos";
const char	Imageset_xmlHandler::ImageYPosAttribute[]				= "YPos";
const char	Imageset_xmlHandler::ImageWidthAttribute[]				= "Width";
const char	Imageset_xmlHandler::ImageHeightAttribute[]				= "Height";
const char	Imageset_xmlHandler::ImageXOffsetAttribute[]			= "XOffset";
const char	Imageset_xmlHandler::ImageYOffsetAttribute[]			= "YOffset";

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void Imageset_xmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
{
	XERCES_CPP_NAMESPACE_USE
	String element(XmlHandlerHelper::transcodeXmlCharToString(localname));

	// handle an Image element (extract all element attributes and use data to define an Image for the Imageset)
	if (element == ImageElement)
	{
		String	name(XmlHandlerHelper::getAttributeValueAsString(attrs, ImageNameAttribute));

		Rect	rect;
		rect.d_left	= (float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImageXPosAttribute);
		rect.d_top	= (float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImageYPosAttribute);
		rect.setWidth((float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImageWidthAttribute));
		rect.setHeight((float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImageHeightAttribute));

		Point	offset;
		offset.d_x	= (float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImageXOffsetAttribute);
		offset.d_y	= (float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImageYOffsetAttribute);

		d_imageset->defineImage(name, rect, offset);
	}
	// handle root Imageset element
	else if (element == ImagesetElement)
	{
		d_imageset->d_name = XmlHandlerHelper::getAttributeValueAsString(attrs, ImagesetNameAttribute);

		Logger::getSingleton().logEvent("Started creation of Imageset '" + d_imageset->d_name + "' via XML file.", Informative);

		//
		// load auto-scaling configuration
		//
		float hres, vres;

		// get native horizontal resolution
		hres = (float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImagesetNativeHorzResAttribute);

		// get native vertical resolution
		vres = (float)XmlHandlerHelper::getAttributeValueAsInteger(attrs, ImagesetNativeVertResAttribute);

		d_imageset->setNativeResolution(Size(hres, vres));

		// get auto-scaling setting
		String autoscale(XmlHandlerHelper::getAttributeValueAsString(attrs, ImagesetAutoScaledAttribute));

		// enable / disable auto-scaling for this Imageset according to the setting
		if ((autoscale == (utf8*)"true") || (autoscale == (utf8*)"1"))
		{
			d_imageset->setAutoScalingEnabled(true);
		}
		else
		{
			d_imageset->setAutoScalingEnabled(false);
		}

		//
		// Create a Texture object via the specified filename, and set it as the texture for the Imageset
		//
		String filename(XmlHandlerHelper::getAttributeValueAsString(attrs, ImagesetImageFileAttribute));

		try
		{
			d_imageset->d_texture = System::getSingleton().getRenderer()->createTexture(filename);
		}
		catch(...)
		{
			throw RendererException((utf8*)"Imageset::xmlHandler::startElement - An unexpected error occurred while creating a Texture object from file '" + filename + "'");
		}

	}
	// anything else is an error which *should* have already been caught by XML validation
	else
	{
		throw FileIOException("Imageset::xmlHandler::startElement - Unexpected data was found while parsing the Imageset file: '" + element + "' is unknown.");
	}

}

void Imageset_xmlHandler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
{
	XERCES_CPP_NAMESPACE_USE
	String element(XmlHandlerHelper::transcodeXmlCharToString(localname));

	if (element == ImagesetElement)
	{
		Logger::getSingleton().logEvent("Finished creation of Imageset '" + d_imageset->d_name + "' via XML file.", Informative);
	}
}


void Imageset_xmlHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Imageset_xmlHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Imageset_xmlHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

} // End of  CEGUI namespace section
