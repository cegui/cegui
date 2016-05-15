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
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/System.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/PixmapFont.h"
#include "CEGUI/SharedStringStream.h"

#ifdef CEGUI_HAS_FREETYPE
#   include "CEGUI/FreeTypeFont.h"
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

// singleton instance pointer
template<> FontManager* Singleton<FontManager>::ms_Singleton = nullptr;


FontManager::FontManager() :
    d_resourceType("Font")
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

Font& FontManager::createFreeTypeFont(const String& font_name,
                                      const float point_size,
                                      const bool anti_aliased,
                                      const String& font_filename,
                                      const String& resource_group,
                                      const AutoScaledMode auto_scaled,
                                      const Sizef& native_res,
                                      XMLResourceExistsAction action)
{
#ifdef CEGUI_HAS_FREETYPE
    CEGUI_LOGINSANE("Attempting to create FreeType font '" +
        font_name + "' using font file '" + font_filename + "'.");

    // create new object ahead of time
    Font* newFont = new FreeTypeFont(font_name, point_size, anti_aliased,
                                    font_filename, resource_group, auto_scaled,
                                    native_res);

    // return appropriate object instance (deleting any not required)
    return doExistingObjectAction(newFont, action);

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
                                    XMLResourceExistsAction action)
{
    CEGUI_LOGINSANE("Attempting to create Pixmap font '" +
        font_name + "' using imageset file '" + imageset_filename + "'.");

    // create new object ahead of time
    Font* newFont = new PixmapFont(font_name, imageset_filename, resource_group,
                                  auto_scaled, native_res);

    // return appropriate object instance (deleting any not required)
    return doExistingObjectAction(newFont, action);
}

void FontManager::notifyDisplaySizeChanged(const Sizef& size)
{
    // notify all attached Font objects of the change in resolution
    FontRegistry::iterator pos = d_registeredFonts.begin(), end = d_registeredFonts.end();

    for (; pos != end; ++pos)
        pos->second->notifyDisplaySizeChanged(size);
}
void FontManager::writeFontToStream(const String& name,
                                    OutStream& out_stream) const
{
    XMLSerializer xml(out_stream);
    // output font data
    get(name).writeXMLToStream(xml);
}

FontManager::FontList FontManager::createFromContainer(const RawDataContainer& source, XMLResourceExistsAction action)
{
    Font_xmlHandler xml_loader;

    xml_loader.handleContainer(source);

    FontList& createdFonts = xml_loader.getObjects();
    doExistingObjectsAction(createdFonts, action);


    return createdFonts;
}


FontManager::FontList FontManager::createFromFile(const String& xml_filename,
    const String& resource_group,
    XMLResourceExistsAction action)
{
    Font_xmlHandler xml_loader;

    xml_loader.handleFile(xml_filename, resource_group);

    FontList& createdFonts = xml_loader.getObjects();
    doExistingObjectsAction(createdFonts, action);

    return createdFonts;
}

FontManager::FontList FontManager::createFromString(const String& source, XMLResourceExistsAction action)
{
    Font_xmlHandler xml_loader;

    xml_loader.handleString(source);

    FontList& createdFonts = xml_loader.getObjects();
    doExistingObjectsAction(createdFonts, action);

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
        "No object of type '" + d_resourceType + "' named '" + font_name +
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
    Logger::getSingleton().logEvent("Object of type '" + d_resourceType +
        "' named '" + ob->first + "' has been destroyed. " +
        addressStr, Informative);

    // Set up event args for event notification
    ResourceEventArgs args(d_resourceType, ob->first);

    delete ob->second;
    d_registeredFonts.erase(ob);

    // fire event signalling an object has been destroyed
    fireEvent(EventResourceDestroyed, args, EventNamespace);
}

Font& FontManager::doExistingObjectAction(Font* font_instance, const XMLResourceExistsAction action)
{
    String fontName = font_instance->getName();
    String event_name;

    if (isDefined(fontName))
    {
        switch (action)
        {
        case XREA_RETURN:
            Logger::getSingleton().logEvent("---- Returning existing instance "
                "of " + d_resourceType + " named '" + fontName + "'.");
            // delete any new object we already had created
            delete font_instance;
            // return existing instance of object.
            return *d_registeredFonts[fontName];

        case XREA_REPLACE:
            Logger::getSingleton().logEvent("---- Replacing existing instance "
                "of " + d_resourceType + " named '" + fontName +
                "' (DANGER!).");
            destroy(fontName);
            event_name = EventResourceReplaced;
            break;

        case XREA_THROW:
            delete font_instance;
            throw AlreadyExistsException(
                "an object of type '" + d_resourceType + "' named '" +
                fontName + "' already exists in the collection.");

        default:
            delete font_instance;
            throw InvalidRequestException(
                "Invalid CEGUI::XMLResourceExistsAction was specified.");
        }
    }
    else
        event_name = EventResourceCreated;

    d_registeredFonts[fontName] = font_instance;

    // fire event about this resource change
    ResourceEventArgs args(d_resourceType, fontName);
    fireEvent(event_name, args, EventNamespace);

    return *font_instance;
}

void FontManager::doExistingObjectsAction(FontList& fonts, const XMLResourceExistsAction action)
{
    String event_name;

    FontList::iterator curIter = fonts.begin();

    for (auto iter = fonts.begin(); iter != fonts.end(); ++iter)
    {
        //! We replacing the pointers to the font object, which is necessary in case 
        *iter = &doExistingObjectAction(*iter, action);
    }
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


//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
