/***********************************************************************
filename: 	CEGUIImageset_xmlHandler.cpp
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
#include "CEGUIImageset_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Definition of constant data for Imageset (and sub-classes)
*************************************************************************/
// Declared in Imageset::xmlHandler
const String Imageset_xmlHandler::ImagesetElement( "Imageset" );
const String Imageset_xmlHandler::ImageElement( "Image" );
const String Imageset_xmlHandler::ImagesetImageFileAttribute( "Imagefile" );
const String Imageset_xmlHandler::ImagesetResourceGroupAttribute( "ResourceGroup" );
const String Imageset_xmlHandler::ImagesetNameAttribute( "Name" );
const String Imageset_xmlHandler::ImagesetNativeHorzResAttribute( "NativeHorzRes" );
const String Imageset_xmlHandler::ImagesetNativeVertResAttribute( "NativeVertRes" );
const String Imageset_xmlHandler::ImagesetAutoScaledAttribute( "AutoScaled" );
const String Imageset_xmlHandler::ImageNameAttribute( "Name" );
const String Imageset_xmlHandler::ImageXPosAttribute( "XPos" );
const String Imageset_xmlHandler::ImageYPosAttribute( "YPos" );
const String Imageset_xmlHandler::ImageWidthAttribute( "Width" );
const String Imageset_xmlHandler::ImageHeightAttribute( "Height" );
const String Imageset_xmlHandler::ImageXOffsetAttribute( "XOffset" );
const String Imageset_xmlHandler::ImageYOffsetAttribute( "YOffset" );

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void Imageset_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
{
    // handle an Image element (extract all element attributes and use data to define an Image for the Imageset)
    if (element == ImageElement)
    {
        elementImageStart(attributes);
    }
    // handle root Imageset element
    else if (element == ImagesetElement)
    {
        elementImagesetStart(attributes);
    }
    // anything else is an error which *should* have already been caught by XML validation
    else
    {
        Logger::getSingleton().logEvent("Imageset::xmlHandler::startElement - Unexpected data was found while parsing the Imageset file: '" + element + "' is unknown.", Errors);
    }
}

void Imageset_xmlHandler::elementEnd(const String& element)
{
    if (element == ImagesetElement)
    {
        elementImagesetEnd();
    }
}

/*************************************************************************
    Method that handles the opening Imageset XML element.
*************************************************************************/
void Imageset_xmlHandler::elementImagesetStart(const XMLAttributes& attributes)
{
    d_imageset->d_name = attributes.getValueAsString(ImagesetNameAttribute);

    // get native horizontal resolution
    float hres = static_cast<float>(attributes.getValueAsInteger(ImagesetNativeHorzResAttribute, 640));
    // get native vertical resolution
    float vres = static_cast<float>(attributes.getValueAsInteger(ImagesetNativeVertResAttribute, 480));
    // set native resolution for imageset 
    d_imageset->setNativeResolution(Size(hres, vres));
    // enable / disable auto-scaling for this Imageset according to the setting
    d_imageset->setAutoScalingEnabled(attributes.getValueAsBool(ImagesetAutoScaledAttribute, false));
    // get texture image filename
    String filename(attributes.getValueAsString(ImagesetImageFileAttribute));
    // get resource group to use.
    String resourceGroup(attributes.getValueAsString(ImagesetResourceGroupAttribute));

    Logger::getSingleton().logEvent("Started creation of Imageset from XML specification:");
    Logger::getSingleton().logEvent("---- CEGUI Imageset name: " + d_imageset->d_name);
    Logger::getSingleton().logEvent("---- Source texture file: " + filename +
                                    " in resource group: " + (resourceGroup.empty() ? "(Default)" : resourceGroup));

    // Create a Texture object via the specified filename, and set it as the texture for the Imageset
    try
    {
        d_imageset->d_texture = System::getSingleton().getRenderer()->createTexture(
            filename, resourceGroup.empty() ? Imageset::getDefaultResourceGroup() : resourceGroup);
    }
    catch(...)
    {
        throw RendererException("Imageset::xmlHandler::startElement - An unexpected error occurred while creating a Texture object from file '" + filename + "'");
    }

    d_imageset->d_textureFilename = filename;
}

/*************************************************************************
    Method that handles the Image XML element.
*************************************************************************/
void Imageset_xmlHandler::elementImageStart(const XMLAttributes& attributes)
{
    String  name(attributes.getValueAsString(ImageNameAttribute));

    Rect    rect;
    rect.d_left = static_cast<float>(attributes.getValueAsInteger(ImageXPosAttribute));
    rect.d_top  = static_cast<float>(attributes.getValueAsInteger(ImageYPosAttribute));
    rect.setWidth(static_cast<float>(attributes.getValueAsInteger(ImageWidthAttribute)));
    rect.setHeight(static_cast<float>(attributes.getValueAsInteger(ImageHeightAttribute)));

    Point   offset;
    offset.d_x  = static_cast<float>(attributes.getValueAsInteger(ImageXOffsetAttribute, 0));
    offset.d_y  = static_cast<float>(attributes.getValueAsInteger(ImageYOffsetAttribute, 0));

    d_imageset->defineImage(name, rect, offset);
}

/*************************************************************************
    Method that handles the closing Imageset XML element.
*************************************************************************/
void Imageset_xmlHandler::elementImagesetEnd()
{
    Logger::getSingleton().logEvent("Finished creation of Imageset '" + d_imageset->d_name + "' via XML file.", Informative);
}

} // End of  CEGUI namespace section
