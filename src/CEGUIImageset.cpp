/************************************************************************
	filename: 	CEGUIImageset.cpp
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
#include "CEGUIImageset.h"
#include "CEGUIExceptions.h"
#include "CEGUITexture.h"
#include "CEGUIRenderer.h"
#include "CEGUISystem.h"
#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"

#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Definition of constant data for Imageset (and sub-classes)
*************************************************************************/
// Declared in Imageset
const char	Imageset::ImagesetSchemaName[]			= "Imageset.xsd";

// Declared in Imageset::xmlHandler
const char	Imageset::xmlHandler::ImagesetImageFileAttribute[]	= "Imagefile";
const char	Imageset::xmlHandler::ImagesetNameAttribute[]		= "Name";
const char	Imageset::xmlHandler::ImagesetNativeHorzResAttribute[]	= "NativeHorzRes";
const char	Imageset::xmlHandler::ImagesetNativeVertResAttribute[]	= "NativeVertRes";
const char	Imageset::xmlHandler::ImagesetAutoScaledAttribute[]		= "AutoScaled";
const char	Imageset::xmlHandler::ImagesetElement[]				= "Imageset";
const char	Imageset::xmlHandler::ImageElement[]				= "Image";
const char	Imageset::xmlHandler::ImageNameAttribute[]			= "Name";
const char	Imageset::xmlHandler::ImageXPosAttribute[]			= "XPos";
const char	Imageset::xmlHandler::ImageYPosAttribute[]			= "YPos";
const char	Imageset::xmlHandler::ImageWidthAttribute[]			= "Width";
const char	Imageset::xmlHandler::ImageHeightAttribute[]		= "Height";
const char	Imageset::xmlHandler::ImageXOffsetAttribute[]		= "XOffset";
const char	Imageset::xmlHandler::ImageYOffsetAttribute[]		= "YOffset";


/*************************************************************************
	constructor
*************************************************************************/
Imageset::Imageset(const String& name, Texture* texture) :
	d_name(name),
	d_texture(texture)
{
	if (d_texture == NULL)
	{
		throw NullObjectException((utf8*)"Imageset::Imageset - Texture object supplied for Imageset creation must not be NULL");
	}

	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, d_nativeVertRes));
}


/*************************************************************************
	construct and initialise Imageset from the specified file.
*************************************************************************/
Imageset::Imageset(const String& filename)
{
	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, d_nativeVertRes));

	d_texture = NULL;
	load(filename);
}


/*************************************************************************
	destructor
*************************************************************************/
Imageset::~Imageset(void)
{
	unload();
}


/*************************************************************************
	Set texture for use by this imageset object
*************************************************************************/
void Imageset::setTexture(Texture* texture)
{
	if (d_texture == NULL)
	{
		throw NullObjectException((utf8*)"Imageset::setTexture - Texture object supplied for Imageset creation must not be NULL");
	}

	d_texture = texture;
}


/*************************************************************************
	load Imageset data from the specified file
*************************************************************************/
void Imageset::load(const String& filename)
{
	XERCES_CPP_NAMESPACE_USE

	// unload old data and texture.
	unload();

	if (filename.empty() || (filename == (utf8*)""))
	{
		throw InvalidRequestException((utf8*)"Imageset::load - Filename supplied for Imageset loading must be valid");
	}

	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

	// set basic settings we want from parser
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
	parser->setFeature(XMLUni::fgXercesSchema, true);
	parser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

	// setup schema for Imageset data
	XMLCh* pval = XMLString::transcode(ImagesetSchemaName);
	ArrayJanitor<XMLCh>	arrayJanitor(pval);
	parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);

	// setup handler object
	xmlHandler handler(this);
	parser->setContentHandler(&handler);
	parser->setErrorHandler(&handler);

	// do parse (which uses handler to create actual data)
	try
	{
		parser->parse(filename.c_str());
	}
	catch(const XMLException& exc)
	{
		if (exc.getCode() != XMLExcepts::NoError)
		{
			unload();
			delete parser;

			ArrayJanitor<char> excmsg(XMLString::transcode(exc.getMessage()));
			String message((utf8*)"Imageset::load - An error occurred while parsing Imageset file '" + filename + "'.  Additional information: ");
			message += (utf8*)excmsg.get();

			throw FileIOException(message);
		}

	}
	catch(const SAXParseException& exc)
	{
		unload();
		delete parser;

		ArrayJanitor<char> excmsg(XMLString::transcode(exc.getMessage()));
		String message((utf8*)"Imageset::load - An error occurred while parsing Imageset file '" + filename + "'.  Additional information: ");
		message += (utf8*)excmsg.get();

		throw FileIOException(message);
	}
	catch(...)
	{
		unload();
		delete parser;

		throw FileIOException((utf8*)"Imageset::load - An unexpected error occurred while parsing Imageset file '" + filename + "'.");
	}

	// cleanup
	delete parser;
}

 
/*************************************************************************
	return the Image object for the named image
*************************************************************************/
const Image& Imageset::getImage(const String& name) const
{
	ImageRegistry::const_iterator	pos = d_images.find(name);

	if (pos == d_images.end())
	{
		throw	UnknownObjectException("Imageset::getImage - The Image named '" + name + "' could not be found in Imageset '" + d_name + "'.");
	}

	return pos->second;
}


/*************************************************************************
	defines a new Image.
*************************************************************************/
void Imageset::defineImage(const String& name, const Rect& image_rect, const Point& render_offset)
{
	if (isImageDefined(name))
	{
		throw AlreadyExistsException("Imageset::defineImage - An image with the name '" + name + "' already exists in Imageset '" + d_name + "'.");
	}

	// get scaling factors
	float hscale = d_autoScale ? d_horzScaling : 1.0f;
	float vscale = d_autoScale ? d_vertScaling : 1.0f;

	// add the Image definition
	d_images[name] = Image(this, image_rect, render_offset, hscale, vscale);
}


/*************************************************************************
	Queues an area of the associated Texture the be drawn on the screen.
	Low-level routine not normally used!
*************************************************************************/
void Imageset::draw(const Rect& source_rect, const Rect& dest_rect, float z, const Rect& clip_rect,const ColourRect& colours) const
{
	// get the rect area that we will actually draw to (i.e. perform clipping)
	Rect final_rect(dest_rect.getIntersection(clip_rect));

	// check if rect was totally clipped
	if (final_rect.getWidth() != 0)
	{
		float x_scale = 1.0f / (float)d_texture->getWidth();
		float y_scale = 1.0f / (float)d_texture->getHeight();

		float tex_per_pix_x = source_rect.getWidth() / dest_rect.getWidth();
		float tex_per_pix_y = source_rect.getHeight() / dest_rect.getHeight();

		// calculate final, clipped, texture co-ordinates
		Rect  tex_rect((source_rect.d_left + ((final_rect.d_left - dest_rect.d_left) * tex_per_pix_x)) * x_scale,
			(source_rect.d_top + ((final_rect.d_top - dest_rect.d_top) * tex_per_pix_y)) * y_scale,
			(source_rect.d_right + ((final_rect.d_right - dest_rect.d_right) * tex_per_pix_x)) * x_scale,
			(source_rect.d_bottom + ((final_rect.d_bottom - dest_rect.d_bottom) * tex_per_pix_y)) * y_scale);

		// queue a quad to be rendered
		d_texture->getRenderer()->addQuad(final_rect, z, d_texture, tex_rect, colours);
	}

}

/*************************************************************************
	Unload all data, leaving Imageset in a clean (but unusable) state
*************************************************************************/
void Imageset::unload(void)
{
	undefineAllImages();

	// cleanup texture
	System::getSingleton().getRenderer()->destroyTexture(d_texture);
	d_texture = NULL;
}


/*************************************************************************
	Sets the scaling factor for all Images that are a part of this Imageset.
*************************************************************************/
void Imageset::updateImageScalingFactors(void)
{
	float hscale, vscale;

	if (d_autoScale)
	{
		hscale = d_horzScaling;
		vscale = d_vertScaling;
	}
	else
	{
		hscale = vscale = 1.0f;
	}

	ImageRegistry::iterator pos = d_images.begin(), end = d_images.end();
	for(; pos != end; ++pos)
	{
		pos->second.setHorzScaling(hscale);
		pos->second.setVertScaling(vscale);
	}

}


/*************************************************************************
	Enable or disable auto-scaling for this Imageset.
*************************************************************************/
void Imageset::setAutoScalingEnabled(bool setting)
{
	if (setting != d_autoScale)
	{
		d_autoScale = setting;
		updateImageScalingFactors();
	}

}


/*************************************************************************
	Set the native resolution for this Imageset
*************************************************************************/
void Imageset::setNativeResolution(const Size& size)
{
	d_nativeHorzRes = size.d_width;
	d_nativeVertRes = size.d_height;

	// re-calculate scaling factors & notify images as required
	notifyScreenResolution(System::getSingleton().getRenderer()->getSize());
}


/*************************************************************************
	Notify the Imageset of the current (usually new) display resolution.
*************************************************************************/
void Imageset::notifyScreenResolution(const Size& size)
{
	d_horzScaling = size.d_width / d_nativeHorzRes;
	d_vertScaling = size.d_height / d_nativeVertRes;

	if (d_autoScale)
	{
		updateImageScalingFactors();
	}

}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	SAX2 Handler methods
*************************************************************************/
void Imageset::xmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
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

void Imageset::xmlHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Imageset::xmlHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

void Imageset::xmlHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
{
	throw(exc);
}

} // End of  CEGUI namespace section
