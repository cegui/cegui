/***********************************************************************
    filename:   CEGUIImageset_xmlHandler.cpp
    created:    Sat Jul 18 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIXMLParser.h"
#include "CEGUIImageset.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String Imageset_xmlHandler::ImagesetSchemaName("Imageset.xsd");
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

//----------------------------------------------------------------------------//
Imageset_xmlHandler::Imageset_xmlHandler(const String& filename,
                                         const String& resource_group) :
    d_imageset(0),
    d_objectRead(false)
{
    System::getSingleton().getXMLParser()->parseXMLFile(
            *this, filename, ImagesetSchemaName,
            resource_group.empty() ? Imageset::getDefaultResourceGroup() :
                                     resource_group);
}

//----------------------------------------------------------------------------//
Imageset_xmlHandler::~Imageset_xmlHandler()
{
    if (!d_objectRead)
        CEGUI_DELETE_AO d_imageset;
}

//----------------------------------------------------------------------------//
const String& Imageset_xmlHandler::getObjectName() const
{
    if (!d_imageset)
        CEGUI_THROW(InvalidRequestException("Imageset_xmlHandler::getName: "
            "Attempt to access null object."));

    return d_imageset->getName();
}

//----------------------------------------------------------------------------//
Imageset& Imageset_xmlHandler::getObject() const
{
    if (!d_imageset)
        CEGUI_THROW(InvalidRequestException("Imageset_xmlHandler::getObject: "
            "Attempt to access null object."));

    d_objectRead = true;
    return *d_imageset;
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementStart(const String& element,
                                       const XMLAttributes& attributes)
{
    // handle an Image element
    if (element == ImageElement)
        elementImageStart(attributes);
    // handle root Imageset element
    else if (element == ImagesetElement)
        elementImagesetStart(attributes);
    // anything else is a non-fatal error.
    else
        Logger::getSingleton().logEvent("Imageset_xmlHandler::elementStart: "
            "Unknown element encountered: <" + element + ">", Errors);
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementEnd(const String& element)
{
    if (element == ImagesetElement)
        elementImagesetEnd();
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementImagesetStart(const XMLAttributes& attributes)
{
    // get name of the imageset.
    const String name(attributes.getValueAsString(ImagesetNameAttribute));
    // get texture image filename
    const String filename(
        attributes.getValueAsString(ImagesetImageFileAttribute));
    // get resource group to use for image file.
    const String resource_group(
        attributes.getValueAsString(ImagesetResourceGroupAttribute));

    Logger& logger(Logger::getSingleton());
    logger.logEvent("Started creation of Imageset from XML specification:");
    logger.logEvent("---- CEGUI Imageset name: " + name);
    logger.logEvent("---- Source texture file: " + filename +
                    " in resource group: " +
                    (resource_group.empty() ? "(Default)" : resource_group));

    // Create imageset object from image file
    d_imageset = CEGUI_NEW_AO Imageset(name, filename, resource_group);

    // set native resolution for imageset
    const float native_hres = static_cast<float>(
        attributes.getValueAsInteger(ImagesetNativeHorzResAttribute, 640));
    const float native_vres = static_cast<float>(
        attributes.getValueAsInteger(ImagesetNativeVertResAttribute, 480));
    d_imageset->setNativeResolution(Size(native_hres, native_vres));

    // set auto-scaling as needed
    d_imageset->setAutoScalingEnabled(
        attributes.getValueAsBool(ImagesetAutoScaledAttribute, false));
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementImageStart(const XMLAttributes& attributes)
{
    if (!d_imageset)
        CEGUI_THROW(InvalidRequestException(
            "Imageset_xmlHandler::elementImageStart: "
            "Attempt to access null object."));

    const String name(attributes.getValueAsString(ImageNameAttribute));

    Rect rect;
    rect.d_left =
        static_cast<float>(attributes.getValueAsInteger(ImageXPosAttribute));
    rect.d_top  =
        static_cast<float>(attributes.getValueAsInteger(ImageYPosAttribute));
    rect.setWidth(
        static_cast<float>(attributes.getValueAsInteger(ImageWidthAttribute)));
    rect.setHeight(
        static_cast<float>(attributes.getValueAsInteger(ImageHeightAttribute)));

    const Vector2<> offset(
        static_cast<float>(attributes.getValueAsInteger(ImageXOffsetAttribute, 0)),
        static_cast<float>(attributes.getValueAsInteger(ImageYOffsetAttribute, 0)));

    d_imageset->defineImage(name, rect, offset);
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementImagesetEnd()
{
    if (!d_imageset)
        CEGUI_THROW(InvalidRequestException(
            "Imageset_xmlHandler::elementImagesetEnd: "
            "Attempt to access null object."));

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(d_imageset));
    Logger::getSingleton().logEvent("Finished creation of Imageset '" +
        d_imageset->getName() + "' via XML file. " + addr_buff, Informative);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
