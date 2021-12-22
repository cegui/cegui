/***********************************************************************
    created:    Wed Dec 23 2009
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
#include "CEGUI/RenderEffectManager.h"
#include "CEGUI/SharedStringStream.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

namespace CEGUI
{
//---------------------------------------------------------------------------//
template<>
RenderEffectManager* Singleton<RenderEffectManager>::ms_Singleton = nullptr;

//---------------------------------------------------------------------------//
RenderEffectManager::RenderEffectManager()
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::RenderEffectManager Singleton created. (" + addressStr + ")");
}

//---------------------------------------------------------------------------//
RenderEffectManager::~RenderEffectManager()
{
    // Destroy any RenderEffect objects we created that still exist.
    while (!d_effects.empty())
        destroy(*d_effects.begin()->first);

    // Remove (destroy) all the RenderEffectFactory objects.
    while (!d_effectRegistry.empty())
        removeEffect(d_effectRegistry.begin()->first);

    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::RenderEffectManager singleton destroyed (" + addressStr + ")");
}

//---------------------------------------------------------------------------//
void RenderEffectManager::removeEffect(const String& name)
{
    RenderEffectRegistry::iterator i(d_effectRegistry.find(name));

    // exit if no factory exists for this type
    if (i == d_effectRegistry.end())
        return;

    Logger::getSingleton().logEvent(
        "Unregistered RenderEffect named '" + name + "'");

    delete i->second;
	d_effectRegistry.erase(name);
}

//---------------------------------------------------------------------------//
bool RenderEffectManager::isEffectAvailable(const String& name) const
{
    return d_effectRegistry.find(name) != d_effectRegistry.end();
}

//---------------------------------------------------------------------------//
RenderEffect& RenderEffectManager::create(const String& name, Window* window)
{
    RenderEffectRegistry::iterator i(d_effectRegistry.find(name));

    // throw if no factory exists for this type
    if (i == d_effectRegistry.end())
        throw UnknownObjectException(
            "No RenderEffect has been registered with the name '" + name + "'");

    RenderEffect& effect = i->second->create(window);

    // here we keep track of the factory used to create the effect object.
    d_effects[&effect] = i->second;

    String addressStr = SharedStringstream::GetPointerAddressAsString(&effect);

    Logger::getSingleton().logEvent("RenderEffectManager::create: Created "
        "instance of effect '" + name + "' at (" + addressStr + ")");

    return effect;
}

//---------------------------------------------------------------------------//
void RenderEffectManager::destroy(RenderEffect& effect)
{
    EffectCreatorMap::iterator i(d_effects.find(&effect));

    // We will only destroy effects that we created (and throw otherwise)
    if (i == d_effects.end())
        throw InvalidRequestException(
            "The given RenderEffect was not created by the "
            "RenderEffectManager - perhaps you created it directly?");

    // Get string of object address before we delete it.
    String addressStr = SharedStringstream::GetPointerAddressAsString(&effect);

    // use the same factory to delete the RenderEffect as what created it
    i->second->destroy(effect);

    // erase record of the object now it's gone
    d_effects.erase(i);

    Logger::getSingleton().logEvent("RenderEffectManager::destroy: Destroyed "
        "RenderEffect object at (" + addressStr + ")");
}

//---------------------------------------------------------------------------//

} // End of  CEGUI namespace section

