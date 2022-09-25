/***********************************************************************
    created:    Sun Jul 19 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUI/FontManager.h"
#include "CEGUI/text/Font_xmlHandler.h"
#include "CEGUI/text/PixmapFont.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/Logger.h"
#include "CEGUI/System.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/SharedStringStream.h"

#ifdef CEGUI_USE_FREETYPE
#   include "CEGUI/text/FreeTypeFont.h"
#endif

namespace CEGUI
{

// singleton instance pointer
template<> FontManager* Singleton<FontManager>::ms_Singleton = nullptr;

const String FontManager::ResourceTypeName = "Font";


FontManager::FontManager()
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::FontManager Singleton created. (" + addressStr + ")");
}

FontManager::~FontManager()
{
    Logger::getSingleton().logEvent(
        "---- Beginning cleanup of Font system ----");

    destroyAll();

    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::FontManager singleton destroyed. (" + addressStr + ")");
}

Font* FontManager::handleResourceExistsAction(
    const String& font_name,
    XmlResourceExistsAction resourceExistsAction,
    String& event_name)
{
    bool isAlreadyDefined = isDefined(font_name);

    if (!isAlreadyDefined)
    {
        event_name = EventResourceCreated;
        return nullptr;
    }

    switch (resourceExistsAction)
    {
        case XmlResourceExistsAction::Return:
        {
            Logger::getSingleton().logEvent("---- Using existing instance "
                "of " + ResourceTypeName + " named '" + font_name + "'.");

            return &get(font_name);
        }
        case XmlResourceExistsAction::Replace:
        {
            Logger::getSingleton().logEvent("---- Replacing existing instance "
                "of " + ResourceTypeName + " named '" + font_name +
                "' (DANGER!).");
            destroy(font_name);

            event_name = EventResourceReplaced;
            return nullptr;
        }
        case XmlResourceExistsAction::Throw:
        {
            destroy(font_name);
            throw AlreadyExistsException(
                "an object of type '" + FontManager::ResourceTypeName + "' named '" +
                font_name + "' already exists in the collection.");
        }
        default:
        {
            destroy(font_name);
            throw InvalidRequestException(
                "Invalid CEGUI::XmlResourceExistsAction was specified.");
        }
    }
}

Font& FontManager::createFreeTypeFont(
    const String& font_name,
    const float size,
    const FontSizeUnit sizeUnit,
    const bool anti_aliased,
    const String& font_filename,
    const String& resource_group,
    const AutoScaledMode auto_scaled,
    const Sizef& native_res,
    float specificLineSpacing,
    XmlResourceExistsAction resourceExistsAction)
{
#ifdef CEGUI_USE_FREETYPE
    CEGUI_LOGINSANE("Attempting to create FreeType font '" +
        font_name + "' using font file '" + font_filename + "'.");

    String event_name;
    Font* fontObject = handleResourceExistsAction(
        font_name, resourceExistsAction, event_name);
    
    if(fontObject != nullptr)
    {
        return *fontObject;
    }

    fontObject = new FreeTypeFont(
        font_name, size, sizeUnit,
        anti_aliased, font_filename,
        resource_group, auto_scaled,
        native_res, specificLineSpacing);

    d_registeredFonts[font_name] = fontObject;

    // fire event about this resource change
    ResourceEventArgs args(ResourceTypeName, font_name);
    fireEvent(event_name, args, EventNamespace);

    return *fontObject;


#else
    throw InvalidRequestException(
        "CEGUI was compiled without freetype support.");
#endif
}

Font& FontManager::createPixmapFont(const String& font_name,
                                    const String& imageset_filename,
                                    const String& resource_group,
                                    const AutoScaledMode auto_scaled,
                                    const Sizef& native_res,
                                    XmlResourceExistsAction resourceExistsAction)
{
    CEGUI_LOGINSANE("Attempting to create Pixmap font '" +
        font_name + "' using imageset file '" + imageset_filename + "'.");

    String event_name;
    Font* fontObject = handleResourceExistsAction(
        font_name, resourceExistsAction, event_name);

    if (fontObject != nullptr)
    {
        return *fontObject;
    }

    fontObject = new PixmapFont(font_name, imageset_filename, resource_group,
                                  auto_scaled, native_res);

    d_registeredFonts[font_name] = fontObject;

    // fire event about this resource change
    ResourceEventArgs args(ResourceTypeName, font_name);
    fireEvent(event_name, args, EventNamespace);

    // return appropriate object instance (deleting any not required)
    return *fontObject;
}

void FontManager::notifyDisplaySizeChanged(const Sizef& size)
{
    // notify all attached Font objects of the change in resolution
    for (const auto& pair : d_registeredFonts)
        pair.second->notifyDisplaySizeChanged(size);
}
void FontManager::writeFontToStream(const String& name,
                                    OutStream& out_stream) const
{
    XMLSerializer xml(out_stream);
    // output font data
    get(name).writeXMLToStream(xml);
}

FontManager::FontList FontManager::createFromContainer(const RawDataContainer& source,
    XmlResourceExistsAction resourceExistsAction)
{
    Font_xmlHandler xml_loader;
    xml_loader.handleContainer(source, resourceExistsAction);

    FontList& createdFonts = xml_loader.getObjects();
    return createdFonts;
}


FontManager::FontList FontManager::createFromFile(const String& xml_filename,
    const String& resource_group,
    XmlResourceExistsAction resourceExistsAction)
{
    Font_xmlHandler xml_loader;
    xml_loader.handleFile(xml_filename, resource_group, resourceExistsAction);

    FontList& createdFonts = xml_loader.getObjects();
    return createdFonts;
}

FontManager::FontList FontManager::createFromString(const String& source,
    XmlResourceExistsAction resourceExistsAction)
{
    Font_xmlHandler xml_loader;
    xml_loader.handleString(source, resourceExistsAction);

    FontList& createdFonts = xml_loader.getObjects();
    return createdFonts;
}

void FontManager::destroy(const String& font_name)
{
    FontRegistry::iterator i(d_registeredFonts.find(font_name));

    // exit if no such object.
    if (i == d_registeredFonts.end())
        return;

    destroyObject(i);
}

void FontManager::destroy(const Font& object)
{
    // don't want to force a 'getName' function on T here, so we'll look for the
    // object the hard way.
    FontRegistry::iterator i(d_registeredFonts.begin());
    for (; i != d_registeredFonts.end(); ++i)
        if (i->second == &object)
        {
        destroyObject(i);
        return;
        }
}

void FontManager::destroyAll()
{
    while (!d_registeredFonts.empty())
        destroyObject(d_registeredFonts.begin());
}


Font& FontManager::get(const String& font_name) const
{
    FontRegistry::const_iterator i(d_registeredFonts.find(font_name));

    if (i == d_registeredFonts.end())
        throw UnknownObjectException(
        "No object of type '" + ResourceTypeName + "' named '" + font_name +
        "' is present in the collection.");

    return *i->second;
}

bool FontManager::isDefined(const String& font_name) const
{
    return d_registeredFonts.find(font_name) != d_registeredFonts.end();
}

void FontManager::destroyObject(
    FontRegistry::iterator ob)
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(ob->second);
    Logger::getSingleton().logEvent("Object of type '" + ResourceTypeName +
        "' named '" + ob->first + "' has been destroyed. " +
        addressStr, LoggingLevel::Informative);

    // Set up event args for event notification
    ResourceEventArgs args(ResourceTypeName, ob->first);

    delete ob->second;
    d_registeredFonts.erase(ob);

    // fire event signaling an object has been destroyed
    fireEvent(EventResourceDestroyed, args, EventNamespace);
}

void FontManager::createAll(const String& pattern,
    const String& resource_group)
{
    std::vector<String> names;
    const size_t num = System::getSingleton().getResourceProvider()->
        getResourceGroupFileNames(names, pattern, resource_group);

    for (size_t i = 0; i < num; ++i)
        createFromFile(names[i], resource_group);
}

const FontManager::FontRegistry& FontManager::getRegisteredFonts() const
{
    return d_registeredFonts;
}

void FontManager::updateAllFonts()
{
    for(auto& currentFont : d_registeredFonts)
    {
        currentFont.second->updateFont();
    }
}


//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
