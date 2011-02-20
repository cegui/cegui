/***********************************************************************
    filename:   CEGUIImageManager.cpp
    created:    Wed Feb 16 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIImageManager.h"
#include "CEGUILogger.h"
#include "CEGUIExceptions.h"

// for the XML parsing part.
#include "CEGUIXMLParser.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUISystem.h"
#include "CEGUITexture.h"
#include "CEGUIBasicImage.h"

#include <cstdio>
#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// singleton instance pointer
template<> ImageManager* Singleton<ImageManager>::ms_Singleton = 0;

//----------------------------------------------------------------------------//
String ImageManager::d_imagesetDefaultResourceGroup;

//----------------------------------------------------------------------------//
// predicate functor class to match items using a given prefix string.
class ImagePrefixMatchPred
{
    const String d_prefix;

public:
    ImagePrefixMatchPred(const String& prefix) : d_prefix(prefix + '/') {}

    bool operator()(const std::pair<String, Image*>& v)
    { return v.first.find(d_prefix) == 0; }
};

//----------------------------------------------------------------------------//
// Internal Strings holding XML element and attribute names
const String ImagesetSchemaName("Imageset.xsd");
const String ImagesetElement( "Imageset" );
const String ImageElement( "Image" );
const String ImagesetImageFileAttribute( "Imagefile" );
const String ImagesetResourceGroupAttribute( "ResourceGroup" );
const String ImagesetNameAttribute( "Name" );
const String ImagesetNativeHorzResAttribute( "NativeHorzRes" );
const String ImagesetNativeVertResAttribute( "NativeVertRes" );
const String ImagesetAutoScaledAttribute( "AutoScaled" );
const String ImageNameAttribute( "Name" );
const String ImageXPosAttribute( "XPos" );
const String ImageYPosAttribute( "YPos" );
const String ImageWidthAttribute( "Width" );
const String ImageHeightAttribute( "Height" );
const String ImageXOffsetAttribute( "XOffset" );
const String ImageYOffsetAttribute( "YOffset" );

//----------------------------------------------------------------------------//
// Internal variables used when parsing XML
static Texture* s_texture = 0;
static bool s_autoscaled = false;
static Size<> s_nativeResolution(640.0f, 480.0f);

//----------------------------------------------------------------------------//
ImageManager::ImageManager()
{
    char addr_buff[32];
    std::sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
        "[CEGUI::ImageManager] Singleton created " + String(addr_buff));
}

//----------------------------------------------------------------------------//
ImageManager::~ImageManager()
{
    destroyAll();

    char addr_buff[32];
    std::sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
       "[CEGUI::ImageManager] Singleton destroyed " + String(addr_buff));
}

//----------------------------------------------------------------------------//
void ImageManager::add(const Image& image)
{
    if (d_images.find(image.getName()) != d_images.end())
        CEGUI_THROW(AlreadyExistsException(
            "[ImageManager] Image already exists: " + image.getName()));

    d_images[image.getName()] = &image.clone();

    Logger::getSingleton().logEvent(
        "[ImageManager] Added image: " + image.getName());
}

//----------------------------------------------------------------------------//
void ImageManager::destroy(Image& image)
{
    destroy(image.getName());
}

//----------------------------------------------------------------------------//
void ImageManager::destroy(const String& name)
{
    ImageMap::iterator i = d_images.find(name);

    if (d_images.end() != i)
        destroy(i);
}

//----------------------------------------------------------------------------//
void ImageManager::destroy(ImageMap::iterator& iter)
{
    Logger::getSingleton().logEvent(
        "[ImageManager] Deleted image: " + iter->first);
    delete iter->second;
    d_images.erase(iter);
}

//----------------------------------------------------------------------------//
void ImageManager::destroyAll()
{
    while (!d_images.empty())
        destroy(d_images.begin()->first);
}

//----------------------------------------------------------------------------//
Image& ImageManager::get(const String& name) const
{
    ImageMap::const_iterator i = d_images.find(name);
    
    if (i == d_images.end())
        CEGUI_THROW(UnknownObjectException(
            "[ImageManager] Image not defined: " + name));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool ImageManager::isDefined(const String& name) const
{
    return d_images.find(name) != d_images.end();
}

//----------------------------------------------------------------------------//
uint ImageManager::getImageCount() const
{
    return static_cast<uint>(d_images.size());
}

//----------------------------------------------------------------------------//
void ImageManager::loadImageset(const String& filename,
                                const String& resource_group)
{
    System::getSingleton().getXMLParser()->parseXMLFile(
            *this, filename, ImagesetSchemaName,
            resource_group.empty() ? d_imagesetDefaultResourceGroup : resource_group);
}

//----------------------------------------------------------------------------//
void ImageManager::destroyImageCollection(const String& prefix,
                                          const bool delete_texture)
{
    Logger::getSingleton().logEvent(
        "[ImageManager] Destroying image collection with prefix: " + prefix);

    ImagePrefixMatchPred p(prefix);

    ImageMap::iterator i;
    while ((i = std::find_if(d_images.begin(), d_images.end(), p)) != d_images.end())
        destroy(i);

    if (delete_texture)
        System::getSingleton().getRenderer()->destroyTexture(prefix);
}

//----------------------------------------------------------------------------//
void ImageManager::addFromImageFile(const String& name, const String& filename,
                                    const String& resource_group)
{
    // create texture from image
    Texture* tex = &System::getSingleton().getRenderer()->
        createTexture(name, filename,
            resource_group.empty() ? d_imagesetDefaultResourceGroup : resource_group);

    const Rect<> rect(Vector2<>(0.0f, 0.0f), tex->getOriginalDataSize());

    BasicImage image(name, tex, rect,
                     Vector2<>(0, 0), false, Size<>(0, 0));
    add(image);
}

//----------------------------------------------------------------------------//
void ImageManager::notifyDisplaySizeChanged(const Size<>& size)
{
    for (ImageMap::iterator i = d_images.begin() ; i != d_images.end(); ++i)
        i->second->notifyDisplaySizeChanged(size);
}

//----------------------------------------------------------------------------//
void ImageManager::elementStart(const String& element,
                                const XMLAttributes& attributes)
{
    if (element == ImageElement)
        elementImageStart(attributes);
    else if (element == ImagesetElement)
        elementImagesetStart(attributes);
    else
        Logger::getSingleton().logEvent(
            "[ImageManager] Unknown XML element encountered: <" +
            element + ">", Errors);
}

//----------------------------------------------------------------------------//
void ImageManager::elementImagesetStart(const XMLAttributes& attributes)
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
    logger.logEvent("[ImageManager] Started creation of Imageset from XML specification:");
    logger.logEvent("[ImageManager] ---- CEGUI Imageset name: " + name);
    logger.logEvent("[ImageManager] ---- Source texture file: " + filename);
    logger.logEvent("[ImageManager] ---- Source texture resource group: " +
                    (resource_group.empty() ? "(Default)" : resource_group));

    // create texture from image
    s_texture = &System::getSingleton().getRenderer()->
        createTexture(name, filename,
            resource_group.empty() ? d_imagesetDefaultResourceGroup : resource_group);

    // set native resolution for imageset
    s_nativeResolution = Size<>(
        attributes.getValueAsFloat(ImagesetNativeHorzResAttribute, 640),
        attributes.getValueAsFloat(ImagesetNativeVertResAttribute, 480));

    // set auto-scaling as needed
    s_autoscaled =
        attributes.getValueAsBool(ImagesetAutoScaledAttribute, false);
}

//----------------------------------------------------------------------------//
void ImageManager::elementImageStart(const XMLAttributes& attributes)
{
    const String name(attributes.getValueAsString(ImageNameAttribute));

    Rect<> rect;
    rect.left(
        attributes.getValueAsInteger(ImageXPosAttribute));
    rect.top(
        attributes.getValueAsInteger(ImageYPosAttribute));
    rect.setWidth(
        attributes.getValueAsInteger(ImageWidthAttribute));
    rect.setHeight(
        attributes.getValueAsInteger(ImageHeightAttribute));

    const Vector2<> offset(
        attributes.getValueAsInteger(ImageXOffsetAttribute, 0),
        attributes.getValueAsInteger(ImageYOffsetAttribute, 0));

    BasicImage image(s_texture->getName() + '/' + name, s_texture,
            rect, offset, s_autoscaled, s_nativeResolution);

    add(image);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

