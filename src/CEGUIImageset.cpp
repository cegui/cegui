/***********************************************************************
	filename: 	CEGUIImageset.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the Imageset class
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
#include "CEGUIImageset.h"
#include "CEGUIExceptions.h"
#include "CEGUITexture.h"
#include "CEGUIRenderer.h"
#include "CEGUISystem.h"
#include "CEGUIImageset_xmlHandler.h"
#include "CEGUILogger.h"
#include "CEGUIDataContainer.h"
#include "CEGUIXMLParser.h"
#include "CEGUIXMLSerializer.h" 
#include "CEGUIPropertyHelper.h" 
#include <iostream>
#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Definition of static /const data for Imageset (and sub-classes)
*************************************************************************/
// Declared in Imageset
const char	Imageset::ImagesetSchemaName[]			= "Imageset.xsd";
String Imageset::d_defaultResourceGroup;


/*************************************************************************
	constructor
*************************************************************************/
Imageset::Imageset(const String& name, Texture* texture) :
	d_name(name),
	d_texture(texture)
{
	if (!d_texture)
	{
		throw NullObjectException("Imageset::Imageset - Texture object supplied for Imageset creation must be valid.");
	}
	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, DefaultNativeVertRes));
}


/*************************************************************************
	construct and initialise Imageset from the specified file.
*************************************************************************/
Imageset::Imageset(const String& filename, const String& resourceGroup)
{
	// defaults for scaling options
	d_autoScale = false;
	setNativeResolution(Size(DefaultNativeHorzRes, DefaultNativeVertRes));

	d_texture = 0;
	load(filename, resourceGroup);
}


Imageset::Imageset(const String& name, const String& filename, const String& resourceGroup) :
    d_name(name)
{
    // try to load the image file using the renderer
    d_texture =
        System::getSingleton().getRenderer()->createTexture(filename,
        resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);

    // store texture filename
    d_textureFilename = filename;
    // TODO: Should we store the resource group too?

    // initialse the auto-scaling for this Imageset
    d_autoScale = true;
    setNativeResolution(
        Size(d_texture->getWidth(), d_texture->getHeight()));

    // define the default image for this Imageset
    defineImage(
        "full_image",
        Rect(0, 0, d_texture->getOriginalWidth(), d_texture->getOriginalHeight()),
        Point(0, 0)
    );
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
	if (!d_texture)
	{
		throw NullObjectException("Imageset::setTexture - Texture object supplied for Imageset creation must be valid.");
	}

	d_texture = texture;
}


/*************************************************************************
	load Imageset data from the specified file
*************************************************************************/
void Imageset::load(const String& filename, const String& resourceGroup)
{
	// unload old data and texture.
	unload();

	if (filename.empty())
	{
		throw InvalidRequestException("Imageset::load - Filename supplied for Imageset loading must be valid");
	}

    // create handler object
    Imageset_xmlHandler handler(this);

	// do parse (which uses handler to create actual data)
	try
	{
        System::getSingleton().getXMLParser()->parseXMLFile(
                handler, filename, ImagesetSchemaName,
                resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);
	}
	catch(...)
	{
		unload();

        Logger::getSingleton().logEvent("Imageset::load - loading of Imageset from file '" + filename +"' failed.", Errors);
        throw;
	}

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
void Imageset::draw(const Rect& source_rect, const Rect& dest_rect, float z, const Rect& clip_rect,const ColourRect& colours, QuadSplitMode quad_split_mode) const
{
	// get the rect area that we will actually draw to (i.e. perform clipping)
	Rect final_rect(dest_rect.getIntersection(clip_rect));

	// check if rect was totally clipped
	if (final_rect.getWidth() != 0)
	{
        // Fix bug #45
        // Obtain correct scale vlaue from the texture
        const float x_scale = d_texture->getXScale();
        const float y_scale = d_texture->getYScale();

		float tex_per_pix_x = source_rect.getWidth() / dest_rect.getWidth();
		float tex_per_pix_y = source_rect.getHeight() / dest_rect.getHeight();

		// calculate final, clipped, texture co-ordinates
		Rect  tex_rect((source_rect.d_left + ((final_rect.d_left - dest_rect.d_left) * tex_per_pix_x)) * x_scale,
			(source_rect.d_top + ((final_rect.d_top - dest_rect.d_top) * tex_per_pix_y)) * y_scale,
			(source_rect.d_right + ((final_rect.d_right - dest_rect.d_right) * tex_per_pix_x)) * x_scale,
			(source_rect.d_bottom + ((final_rect.d_bottom - dest_rect.d_bottom) * tex_per_pix_y)) * y_scale);

		final_rect.d_left	= PixelAligned(final_rect.d_left);
		final_rect.d_right	= PixelAligned(final_rect.d_right);
		final_rect.d_top	= PixelAligned(final_rect.d_top);
		final_rect.d_bottom	= PixelAligned(final_rect.d_bottom);

		// queue a quad to be rendered
		d_texture->getRenderer()->addQuad(final_rect, z, d_texture, tex_rect, colours, quad_split_mode);
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
	d_texture = 0;
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

void Imageset::writeXMLToStream(XMLSerializer& xml_stream) const
{
    // output Imageset tag
    xml_stream.openTag("Imageset")
        .attribute("Name", d_name)
        .attribute("Imagefile", d_textureFilename);

    if (d_nativeHorzRes != DefaultNativeHorzRes)
        xml_stream.attribute("NativeHorzRes", 
          PropertyHelper::uintToString(static_cast<uint>(d_nativeHorzRes)));
    if (d_nativeVertRes != DefaultNativeVertRes)
        xml_stream.attribute("NativeVertRes", 
          PropertyHelper::uintToString(static_cast<uint>(d_nativeVertRes)));

    if (d_autoScale)
        xml_stream.attribute("AutoScaled", "true");
    
    // output images
    ImageIterator image = getIterator();
    while (!image.isAtEnd())
    {
        image.getCurrentValue().writeXMLToStream(xml_stream);
        ++image;
    }

    // output closing tag
    xml_stream.closeTag();
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
