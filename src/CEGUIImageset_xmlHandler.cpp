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

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Definition of constant data for Imageset (and sub-classes)
*************************************************************************/

	// Declared in Imageset::xmlHandler
const char	Imageset_xmlHandler::ImagesetImageFileAttribute[]		= "Imagefile";
const char	Imageset_xmlHandler::ImagesetNameAttribute[]			= "Name";
const char	Imageset_xmlHandler::ImagesetNativeHorzResAttribute[]	= "NativeHorzRes";
const char	Imageset_xmlHandler::ImagesetNativeVertResAttribute[]	= "NativeVertRes";
const char	Imageset_xmlHandler::ImagesetAutoScaledAttribute[]		= "AutoScaled";
const char	Imageset_xmlHandler::ImagesetElement[]					= "Imageset";
const char	Imageset_xmlHandler::ImageElement[]						= "Image";
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
		std::string element(XMLString::transcode(localname));

	// handle an Image element (extract all element attributes and use data to define an Image for the Imageset)
	if (element == ImageElement)
	{
		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(ImageNameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		String	name((utf8*)val_str.get());

		Rect	rect;
		attr_name.reset(XMLString::transcode(ImageXPosAttribute));
		rect.d_left	= (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		attr_name.reset(XMLString::transcode(ImageYPosAttribute));
		rect.d_top	= (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		attr_name.reset(XMLString::transcode(ImageWidthAttribute));
		rect.setWidth((float)XMLString::parseInt(attrs.getValue(attr_name.get())));

		attr_name.reset(XMLString::transcode(ImageHeightAttribute));
		rect.setHeight((float)XMLString::parseInt(attrs.getValue(attr_name.get())));

		Point	offset;
		attr_name.reset(XMLString::transcode(ImageXOffsetAttribute));
		offset.d_x	= (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		attr_name.reset(XMLString::transcode(ImageYOffsetAttribute));
		offset.d_y	= (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		d_imageset->defineImage(name, rect, offset);
	}
	// handle root Imageset element
	else if (element == ImagesetElement)
	{
		ArrayJanitor<XMLCh>	attr_name(XMLString::transcode(ImagesetNameAttribute));
		ArrayJanitor<char>  val_str(XMLString::transcode(attrs.getValue(attr_name.get())));
		d_imageset->d_name = (utf8*)val_str.get();

		//
		// load auto-scaling configuration
		//
		float hres, vres;

		// get native horizontal resolution
		attr_name.reset(XMLString::transcode(ImagesetNativeHorzResAttribute));
		hres = (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		// get native vertical resolution
		attr_name.reset(XMLString::transcode(ImagesetNativeVertResAttribute));
		vres = (float)XMLString::parseInt(attrs.getValue(attr_name.get()));

		d_imageset->setNativeResolution(Size(hres, vres));

		// get auto-scaling setting
		attr_name.reset(XMLString::transcode(ImagesetAutoScaledAttribute));
		val_str.reset(XMLString::transcode(attrs.getValue(attr_name.get())));
		std::string autoscale = val_str.get();

		// enable / disable auto-scaling for this Imageset according to the setting
		if ((autoscale == "true") || (autoscale == "1"))
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
		attr_name.reset(XMLString::transcode(ImagesetImageFileAttribute));
		val_str.reset(XMLString::transcode(attrs.getValue(attr_name.get())));
		String filename((utf8*)val_str.get());

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
