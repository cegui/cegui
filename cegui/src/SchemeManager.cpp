/***********************************************************************
    created:    Mon Jul 20 2015
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
#include "CEGUI/SchemeManager.h"
#include "CEGUI/Scheme.h"
#include "CEGUI/Scheme_xmlHandler.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/System.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/Logger.h"
#include "CEGUI/SharedStringStream.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
template<> SchemeManager* Singleton<SchemeManager>::ms_Singleton = nullptr;

//----------------------------------------------------------------------------//
SchemeManager::SchemeManager() :
    d_resourceType("Scheme"),
    d_autoLoadResources(true)
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::SchemeManager Singleton created. (" + addressStr + ")");
}

//----------------------------------------------------------------------------//
SchemeManager::~SchemeManager()
{
    Logger::getSingleton().logEvent(
        "---- Beginning cleanup of GUI Scheme system ----");

    destroyAll();

    String addressStr = SharedStringstream::GetPointerAddressAsString(this);
    Logger::getSingleton().logEvent(
        "CEGUI::SchemeManager Singleton destroyed. (" + addressStr + ")");
}

//----------------------------------------------------------------------------//
void SchemeManager::setAutoLoadResources(bool enabled)
{
    d_autoLoadResources = enabled;
}

//----------------------------------------------------------------------------//
bool SchemeManager::getAutoLoadResources() const
{
    return d_autoLoadResources;
}

//----------------------------------------------------------------------------//
void SchemeManager::doPostObjectAdditionAction(Scheme& scheme)
{
    if (d_autoLoadResources)
    {
        scheme.loadResources();
    }
}

Scheme& SchemeManager::createFromContainer(const RawDataContainer& source,
    XmlResourceExistsAction resourceExistsAction)
{
    Scheme_xmlHandler xml_loader;

    xml_loader.handleContainer(source, resourceExistsAction);
    return doExistingObjectAction(xml_loader.getObjectName(),
        &xml_loader.getObject(), resourceExistsAction);
}


Scheme& SchemeManager::createFromFile(const String& xml_filename,
    const String& resource_group,
    XmlResourceExistsAction resourceExistsAction)
{
    Scheme_xmlHandler xml_loader;

    xml_loader.handleFile(xml_filename, resource_group, resourceExistsAction);
    return doExistingObjectAction(xml_loader.getObjectName(),
        &xml_loader.getObject(), resourceExistsAction);
}


Scheme& SchemeManager::createFromString(const String& source,
    XmlResourceExistsAction resourceExistsAction)
{
    Scheme_xmlHandler xml_loader;

    xml_loader.handleString(source, resourceExistsAction);
    return doExistingObjectAction(xml_loader.getObjectName(),
        &xml_loader.getObject(), resourceExistsAction);
}


void SchemeManager::destroy(const String& object_name)
{
    SchemeRegistry::iterator i(d_registeredSchemes.find(object_name));

    // exit if no such object.
    if (i == d_registeredSchemes.end())
        return;

    destroyObject(i);
}

void SchemeManager::destroy(const Scheme& object)
{
    // don't want to force a 'getName' function on T here, so we'll look for the
    // object the hard way.
    SchemeRegistry::iterator i(d_registeredSchemes.begin());
    for (; i != d_registeredSchemes.end(); ++i)
        if (i->second == &object)
        {
        destroyObject(i);
        return;
        }
}


void SchemeManager::destroyAll()
{
    while (!d_registeredSchemes.empty())
        destroyObject(d_registeredSchemes.begin());
}


Scheme& SchemeManager::get(const String& object_name) const
{
    SchemeRegistry::const_iterator i(d_registeredSchemes.find(object_name));

    if (i == d_registeredSchemes.end())
        throw UnknownObjectException(
        "No object of type '" + d_resourceType + "' named '" + object_name +
        "' is present in the collection.");

    return *i->second;
}


bool SchemeManager::isDefined(const String& object_name) const
{
    return d_registeredSchemes.find(object_name) != d_registeredSchemes.end();
}


void SchemeManager::destroyObject(
    SchemeRegistry::iterator ob)
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(ob->second);
    Logger::getSingleton().logEvent("Object of type '" + d_resourceType +
        "' named '" + ob->first + "' has been destroyed. " +
        addressStr, LoggingLevel::Informative);

    // Set up event args for event notification
    ResourceEventArgs args(d_resourceType, ob->first);

    delete ob->second;
    d_registeredSchemes.erase(ob);

    // fire event signaling an object has been destroyed
    fireEvent(EventResourceDestroyed, args, EventNamespace);
}


Scheme& SchemeManager::doExistingObjectAction(
    const String object_name,
    Scheme* object,
    const XmlResourceExistsAction resourceExistsAction)
{
    String event_name;

    if (isDefined(object_name))
    {
        switch (resourceExistsAction)
        {
        case XmlResourceExistsAction::Return:
            Logger::getSingleton().logEvent("---- Returning existing instance "
                "of " + d_resourceType + " named '" + object_name + "'.");
            // delete any new object we already had created
            delete object;
            // return existing instance of object.
            return *d_registeredSchemes[object_name];

        case XmlResourceExistsAction::Replace:
            Logger::getSingleton().logEvent("---- Replacing existing instance "
                "of " + d_resourceType + " named '" + object_name +
                "' (DANGER!).");
            destroy(object_name);
            event_name = EventResourceReplaced;
            break;

        case XmlResourceExistsAction::Throw:
            delete object;
            throw AlreadyExistsException(
                "an object of type '" + d_resourceType + "' named '" +
                object_name + "' already exists in the collection.");

        default:
            delete object;
            throw InvalidRequestException(
                "Invalid CEGUI::XmlResourceExistsAction was specified.");
        }
    }
    else
        event_name = EventResourceCreated;

    d_registeredSchemes[object_name] = object;
    doPostObjectAdditionAction(*object);

    // fire event about this resource change
    ResourceEventArgs args(d_resourceType, object_name);
    fireEvent(event_name, args, EventNamespace);

    return *object;
}

void SchemeManager::createAll(const String& pattern,
    const String& resource_group)
{
    std::vector<String> names;
    const size_t num = System::getSingleton().getResourceProvider()->
        getResourceGroupFileNames(names, pattern, resource_group);

    for (size_t i = 0; i < num; ++i)
        createFromFile(names[i], resource_group);
}

const SchemeManager::SchemeRegistry& SchemeManager::getRegisteredSchemes() const
{
    return d_registeredSchemes;
}


}
