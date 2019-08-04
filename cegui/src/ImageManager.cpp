/***********************************************************************
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
#include "CEGUI/ImageManager.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/SharedStringStream.h"

// for the XML parsing part.
#include "CEGUI/XMLParser.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/System.h"
#include "CEGUI/Texture.h"
#include "CEGUI/BitmapImage.h"
#include "CEGUI/svg/SVGImage.h"
#include "CEGUI/svg/SVGData.h"
#include "CEGUI/svg/SVGDataManager.h"

#include <cstdio>
#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// singleton instance pointer
template<> ImageManager* Singleton<ImageManager>::ms_Singleton = nullptr;

//----------------------------------------------------------------------------//
String ImageManager::d_imagesetDefaultResourceGroup;

//----------------------------------------------------------------------------//
// predicate functor class to match items using a given prefix string.
class ImagePrefixMatchPred
{
    const String d_prefix;

public:
    ImagePrefixMatchPred(const String& prefix) : d_prefix(prefix + '/') {}

    bool operator()(const std::pair<String, std::pair<Image*, ImageFactory*> >& v)
    { return v.first.find(d_prefix) == 0; }
};

//----------------------------------------------------------------------------//
// Internal Strings holding XML element and attribute names
const String ImagesetSchemaName("Imageset.xsd");
const String ImagesetElement( "Imageset" );
const String ImageElement( "Image" );
const String ImagesetImageFileAttribute( "imagefile" );
const String ImagesetResourceGroupAttribute( "resourceGroup" );
const String ImagesetNameAttribute( "name" );
const String ImagesetTypeAttribute( "type" );
const String ImagesetNativeHorzResAttribute( "nativeHorzRes" );
const String ImagesetNativeVertResAttribute( "nativeVertRes" );
const String ImagesetAutoScaledAttribute( "autoScaled" );
const String ImageTextureAttribute( "texture" );
const String ImageSVGDataAttribute( "SVGData" );
const String ImageNameAttribute( "name" );
const String ImagesetVersionAttribute( "version" );
// Internal Strings holding XML element and attribute defaults
const String ImageTypeAttributeDefault( "BitmapImage" );

//----------------------------------------------------------------------------//
// note: The assets' versions aren't usually the same as CEGUI version, they
// are versioned from version 1 onwards!
//
// previous versions (though not specified in files until 2)
// 1 - CEGUI up to and including 0.7.x
// 2 - CEGUI version 1.x.x (Custom Image support,
//                          changed case of attr names, added version support)
const String NativeVersion( "2" );

//----------------------------------------------------------------------------//
// Internal variables used when parsing XML
static Texture* s_texture = nullptr;
static SVGData* s_SVGData = nullptr;
static CEGUI::String s_imagesetType = "";
static AutoScaledMode s_autoScaled = AutoScaledMode::Disabled;
static Sizef s_nativeResolution(640.0f, 480.0f);

//----------------------------------------------------------------------------//
ImageManager::ImageManager()
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::ImageManager Singleton created. (" + addressStr + ")");

    // self-register the built in 'BitmapImage' type.
    addImageType<BitmapImage>("BitmapImage");
    // self-register the built in 'SVGImage' type.
    addImageType<SVGImage>("SVGImage");
}

//----------------------------------------------------------------------------//
ImageManager::~ImageManager()
{
    destroyAll();

    while (!d_factories.empty())
        removeImageType(d_factories.begin()->first);

    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
       "CEGUI::ImageManager Singleton destroyed (" + addressStr + ")");
}

//----------------------------------------------------------------------------//
void ImageManager::removeImageType(const String& name)
{
    ImageFactoryRegistry::iterator i(d_factories.find(name));

    if (i == d_factories.end())
        return;

    Logger::getSingleton().logEvent(
        "[ImageManager] Unregistered Image type: " + name);

    delete i->second;
	d_factories.erase(name);
}

//----------------------------------------------------------------------------//
bool ImageManager::isImageTypeAvailable(const String& name) const
{
    return d_factories.find(name) != d_factories.end();
}

//----------------------------------------------------------------------------//
Image& ImageManager::create(const String& type, const String& name)
{
    if (d_images.find(name) != d_images.end())
        throw AlreadyExistsException(
            "Image already exists: " + name);

    ImageFactoryRegistry::iterator i(d_factories.find(type));

    if (i == d_factories.end())
        throw UnknownObjectException(
            "Unknown Image type: " + type);

    ImageFactory* factory = i->second;
    Image& image = factory->create(name);
    d_images[name] = std::make_pair(&image, factory);

        String addressStr = SharedStringstream::GetPointerAddressAsString(&image);

    Logger::getSingleton().logEvent(
        "[ImageManager] Created image: '" + name + "' (" + addressStr + 
        ") of type: " + type);

    return image;
}

//----------------------------------------------------------------------------//
Image& ImageManager::create(const XMLAttributes& attributes)
{
    const String name = attributes.getValueAsString(ImageNameAttribute);

    if (name.empty())
        throw InvalidRequestException(
            "Invalid (empty) image name passed to create.");

    if (d_images.find(name) != d_images.end())
        throw AlreadyExistsException(
            "Image already exists: " + name);

    ImageFactoryRegistry::iterator i(d_factories.find(s_imagesetType));

    if (i == d_factories.end())
        throw UnknownObjectException(
            "Unknown Image type: " + s_imagesetType);

    ImageFactory* factory = i->second;
    Image& image = factory->create(attributes);

    // sanity check that the created image uses the name specified in the
    // attributes block
    if (image.getName() != name)
    {
        const String message(
            "Factory for type: " + s_imagesetType + " created Image named: " +
            image.getName() + ".  Was expecting name: " + name);
            
        factory->destroy(image);

        throw InvalidRequestException(message);
    }

    d_images[name] = std::make_pair(&image, factory);

    String addressStr = SharedStringstream::GetPointerAddressAsString(&image);
    Logger::getSingleton().logEvent(
        "[ImageManager] Created image: '" + name + "' (" + addressStr + 
        ") of type: " + s_imagesetType);

    return image;
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

    // use the stored factory to destroy the image it created.
    iter->second.second->destroy(*iter->second.first);

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
        throw UnknownObjectException(
            "Image not defined: " + name);

    return *i->second.first;
}

//----------------------------------------------------------------------------//
bool ImageManager::isDefined(const String& name) const
{
    return d_images.find(name) != d_images.end();
}

//----------------------------------------------------------------------------//
unsigned int ImageManager::getImageCount() const
{
    return static_cast<unsigned int>(d_images.size());
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
void ImageManager::loadImagesetFromString(const String& source)
{
    System::getSingleton().getXMLParser()->parseXMLString(
            *this, source, ImagesetSchemaName);
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
void ImageManager::addBitmapImageFromFile(const String& name, const String& filename,
                                    const String& resource_group)
{
    // create texture from image
    Texture* tex = &System::getSingleton().getRenderer()->
        createTexture(name, filename,
            resource_group.empty() ? d_imagesetDefaultResourceGroup : resource_group);

    BitmapImage& image = static_cast<BitmapImage&>(create("BitmapImage", name));
    image.setTexture(tex);
    const Rectf rect(glm::vec2(0.0f, 0.0f), tex->getOriginalDataSize());
    image.setImageArea(rect);
}

//----------------------------------------------------------------------------//
void ImageManager::notifyDisplaySizeChanged(const Sizef& size)
{
    for (ImageMap::iterator i = d_images.begin() ; i != d_images.end(); ++i)
        i->second.first->notifyDisplaySizeChanged(size);
}

//----------------------------------------------------------------------------//
const String& ImageManager::getSchemaName() const
{
    return ImagesetSchemaName;
}

//----------------------------------------------------------------------------//
const String& ImageManager::getDefaultResourceGroup() const
{
    return d_imagesetDefaultResourceGroup;
}

//----------------------------------------------------------------------------//
ImageManager::ImageIterator ImageManager::getIterator() const
{
    return ImageIterator(d_images.begin(), d_images.end());
}

//----------------------------------------------------------------------------//
void ImageManager::elementStartLocal(const String& element,
                                     const XMLAttributes& attributes)
{
    if (element == ImageElement)
        elementImageStart(attributes);
    else if (element == ImagesetElement)
        elementImagesetStart(attributes);
    else
        Logger::getSingleton().logEvent(
            "[ImageManager] Unknown XML element encountered: <" +
            element + ">", LoggingLevel::Error);
}

//----------------------------------------------------------------------------//
void ImageManager::elementEndLocal(const String& element)
{
    // ensure that everything is reset to default values when the Imageset ends
    if (element == ImagesetElement)
    {
        s_texture = nullptr;
        s_SVGData = nullptr;
        s_imagesetType = "";
    }
}

//----------------------------------------------------------------------------//
void ImageManager::elementImagesetStart(const XMLAttributes& attributes)
{
    // get name of the imageset.
    const String name(attributes.getValueAsString(ImagesetNameAttribute));
    // get name of the imageset.
    s_imagesetType = attributes.getValueAsString(ImagesetTypeAttribute,
                                                 "BitmapImage");
    
    // get texture image filename
    const String filename(
        attributes.getValueAsString(ImagesetImageFileAttribute));
    // get resource group to use for image file.
    const String resource_group(
        attributes.getValueAsString(ImagesetResourceGroupAttribute));

    Logger& logger(Logger::getSingleton());
    logger.logEvent("[ImageManager] Started creation of Imageset from XML specification:");
    logger.logEvent("[ImageManager] ---- CEGUI Imageset name: " + name);
    logger.logEvent("[ImageManager] ---- Source image file: " + filename);
    logger.logEvent("[ImageManager] ---- Source resource group: " +
                    (resource_group.empty() ? "(Default)" : resource_group));

    validateImagesetFileVersion(attributes);

    if(s_imagesetType == "BitmapImage")
        retrieveImagesetTexture(name, filename, resource_group);
    else if(s_imagesetType == "SVGImage")
        retrieveImagesetSVGData(name, filename, resource_group);
    else
    {
        CEGUI::String message = "Imageset type: \"" + s_imagesetType + "\" is unknown.";
        throw UnknownObjectException(message);
    }
            

    // set native resolution for imageset
    s_nativeResolution = Sizef(
        attributes.getValueAsFloat(ImagesetNativeHorzResAttribute, 640),
        attributes.getValueAsFloat(ImagesetNativeVertResAttribute, 480));

    // set auto-scaling as needed
    s_autoScaled = PropertyHelper<AutoScaledMode>::fromString(
                attributes.getValueAsString(ImagesetAutoScaledAttribute, "false"));
}

//----------------------------------------------------------------------------//
void ImageManager::validateImagesetFileVersion(const XMLAttributes& attrs)
{
    const String version(attrs.getValueAsString(ImagesetVersionAttribute,
                                                "unknown"));

    if (version == NativeVersion)
        return;

    throw InvalidRequestException(
        "You are attempting to load an imageset of version '" + version +
        "' but this CEGUI version is only meant to load imagesets of version '" +
        NativeVersion + "'. Consider using the migrate.py script bundled with "
        "CEGUI Unified Editor to migrate your data.");
}

//----------------------------------------------------------------------------//
void ImageManager::elementImageStart(const XMLAttributes& attributes)
{
    String image_data_name = "";

    if (s_imagesetType == "BitmapImage")
        image_data_name = s_texture->getName();
    else if (s_imagesetType == "SVGImage")
        image_data_name = s_SVGData->getName();

    const String image_name(image_data_name + '/' +
        attributes.getValueAsString(ImageNameAttribute));

    if (isDefined(image_name))
    {
        Logger::getSingleton().logEvent(
            "[ImageManager] WARNING: Using existing image :" + image_name);
        return;
    }

    XMLAttributes rw_attrs(attributes);

    // rewrite the name attribute to include the underlying image data's (Texture's or
    // SVGData's) name
    rw_attrs.add(ImageNameAttribute, image_name);

    if (s_imagesetType == "BitmapImage")
    {
        if (!rw_attrs.exists(ImageTextureAttribute))
            rw_attrs.add(ImageTextureAttribute, image_data_name);
    }
    else if (s_imagesetType == "SVGImage")
    {
        if (!rw_attrs.exists(ImageSVGDataAttribute))
            rw_attrs.add(ImageSVGDataAttribute, image_data_name);
    }

    if (!rw_attrs.exists(ImagesetAutoScaledAttribute))
        rw_attrs.add(ImagesetAutoScaledAttribute,
                     PropertyHelper<AutoScaledMode>::toString(s_autoScaled));

    if (!rw_attrs.exists(ImagesetNativeHorzResAttribute))
        rw_attrs.add(ImagesetNativeHorzResAttribute,
                     PropertyHelper<std::uint32_t>::toString(static_cast<std::uint32_t>(s_nativeResolution.d_width)));

    if (!rw_attrs.exists(ImagesetNativeVertResAttribute))
        rw_attrs.add(ImagesetNativeVertResAttribute,
                     PropertyHelper<std::uint32_t>::toString(static_cast<std::uint32_t>(s_nativeResolution.d_height)));

    d_deleteChainedHandler = false;
    d_chainedHandler = &create(rw_attrs);
}

//----------------------------------------------------------------------------//
void ImageManager::retrieveImagesetTexture(const String& name, const String& filename, const String &resource_group)
{
    Renderer* const renderer = System::getSingleton().getRenderer();

    // if the texture already exists
    if (renderer->isTextureDefined(name))
    {
        Logger::getSingleton().logEvent(
            "[ImageManager] WARNING: Using existing texture: " + name);
        s_texture = &renderer->getTexture(name);
    }
    else
    {
        // create texture from image
        s_texture = &renderer->createTexture(name, filename,
            resource_group.empty() ? d_imagesetDefaultResourceGroup :
            resource_group);
    }
}

//----------------------------------------------------------------------------//
void ImageManager::retrieveImagesetSVGData(const String& name, const String& filename, const String &resource_group)
{
    SVGDataManager& svgDataManager = SVGDataManager::getSingleton();

    if (svgDataManager.isSVGDataDefined(name))
    {
        Logger::getSingleton().logEvent(
            "[ImageManager] WARNING: Using existing SVGData: " + name);
        s_SVGData = &svgDataManager.getSVGData(name);
    }
    else
    {
        // Create SVGData from the specified file
        s_SVGData = &svgDataManager.create(name, filename,
            resource_group.empty() ? d_imagesetDefaultResourceGroup :
            resource_group);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

