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
#include "CEGUIImageset_xmlHandler.h"
#include "CEGUILogger.h"

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
	parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);
	XMLString::release(&pval);

	// setup handler object
	Imageset_xmlHandler handler(this);
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

			char* excmsg = XMLString::transcode(exc.getMessage());
			String message((utf8*)"Imageset::load - An error occurred while parsing Imageset file '" + filename + "'.  Additional information: ");
			message += (utf8*)excmsg;
			XMLString::release(&excmsg);

			throw FileIOException(message);
		}

	}
	catch(const SAXParseException& exc)
	{
		unload();
		delete parser;

		char* excmsg = XMLString::transcode(exc.getMessage());
		String message((utf8*)"Imageset::load - An error occurred while parsing Imageset file '" + filename + "'.  Additional information: ");
		message += (utf8*)excmsg;
		XMLString::release(&excmsg);

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
	d_images[name] = Image(this, name, image_rect, render_offset, hscale, vscale);

	CEGUI_LOGINSANE("Image '" + name + "' has been defined for Imageset '" + d_name + "'.")
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


/*************************************************************************
	Return an iterator object that can be used to iterate over the Image
	objects in the Imageset.
*************************************************************************/
Imageset::ImageIterator Imageset::getIterator(void) const
{
	return ImageIterator(d_images.begin(), d_images.end());
}


void Imageset::undefineImage(const String& name)
{
	d_images.erase(name);

	CEGUI_LOGINSANE("Image '" + name + "' has been removed from Imageset '" + d_name + "'.")
}


void Imageset::undefineAllImages(void)
{
	d_images.clear();

	CEGUI_LOGINSANE("All images have been removed from Imageset '" + d_name + "'.")
}

} // End of  CEGUI namespace section
