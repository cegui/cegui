/***********************************************************************
    filename:   CEGUIAnimationManager.cpp
    created:    7/8/2010
    author:     Martin Preisler

    purpose:    Implements the AnimationManager class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIAnimationManager.h"
#include "CEGUILogger.h"
#include "CEGUIAnimation.h"
#include "CEGUIAnimationInstance.h"
#include "CEGUITplInterpolators.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIXMLParser.h"
#include "CEGUIAnimation_xmlHandler.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> AnimationManager* Singleton<AnimationManager>::ms_Singleton  = 0;
// Name of the xsd schema file used to validate animation XML files.
const String AnimationManager::XMLSchemaName("Animation.xsd");
// String that holds the default resource group for loading animations
String AnimationManager::s_defaultResourceGroup("");

/*************************************************************************
    Constructor
*************************************************************************/
AnimationManager::AnimationManager(void)
{
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
        "CEGUI::AnimationManager singleton created " + String(addr_buff));

    // todo: is this too dirty?
#   define addBasicInterpolator(i) { Interpolator* in = i; addInterpolator(in); d_basicInterpolators.push_back(in); }

    // create and add basic interpolators shipped with CEGUI
    addBasicInterpolator(CEGUI_NEW_AO TplDiscreteRelativeInterpolator<String>("String"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<float>("float"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<int>("int"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<uint>("uint"));
    addBasicInterpolator(CEGUI_NEW_AO TplDiscreteInterpolator<bool>("bool"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<Sizef >("Sizef"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<Vector2f >("Vector2f"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<Vector3f >("Vector3f"));
    addBasicInterpolator(CEGUI_NEW_AO QuaternionSlerpInterpolator());
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<Rectf >("Rectf"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<Colour>("Colour"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<ColourRect>("ColourRect"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<UDim>("UDim"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<UVector2>("UVector2"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<URect>("URect"));
    addBasicInterpolator(CEGUI_NEW_AO TplLinearInterpolator<UBox>("UBox"));
}


/*************************************************************************
    Destructor
*************************************************************************/
AnimationManager::~AnimationManager(void)
{
    // first we remove & destroy remaining animation instances
    for (AnimationInstanceMap::const_iterator it = d_animationInstances.begin();
         it != d_animationInstances.end(); ++it)
    {
        CEGUI_DELETE_AO it->second;
    }

    d_animationInstances.clear();

    // then we remove & destroy animation definitions
    for (AnimationMap::const_iterator it = d_animations.begin();
         it != d_animations.end(); ++it)
    {
        CEGUI_DELETE_AO it->second;
    }

    d_animations.clear();

    // and lastly, we remove all interpolators, but we don't delete them!
    // it is the creator's responsibility to delete them
    d_interpolators.clear();

    // we only destroy inbuilt interpolators
    for (BasicInterpolatorList::const_iterator it = d_basicInterpolators.begin();
         it != d_basicInterpolators.end(); ++it)
    {
        CEGUI_DELETE_AO *it;
    }

    d_basicInterpolators.clear();

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
        "CEGUI::AnimationManager singleton destroyed " + String(addr_buff));
}

//----------------------------------------------------------------------------//
void AnimationManager::addInterpolator(Interpolator* interpolator)
{
    if (d_interpolators.find(interpolator->getType()) != d_interpolators.end())
    {
        CEGUI_THROW(AlreadyExistsException(
            "AnimationManager::addInterpolator: Interpolator of type '"
            + interpolator->getType() + "' already exists."));
    }

    d_interpolators.insert(
        std::make_pair(interpolator->getType(), interpolator));
}

//----------------------------------------------------------------------------//
void AnimationManager::removeInterpolator(Interpolator* interpolator)
{
    InterpolatorMap::iterator it = d_interpolators.find(interpolator->getType());

    if (it == d_interpolators.end())
    {
        CEGUI_THROW(UnknownObjectException(
            "AnimationManager::removeInterpolator: Interpolator of type '"
            + interpolator->getType() + "' not found."));
    }

    d_interpolators.erase(it);
}

//----------------------------------------------------------------------------//
Interpolator* AnimationManager::getInterpolator(const String& type) const
{
    InterpolatorMap::const_iterator it = d_interpolators.find(type);

    if (it == d_interpolators.end())
    {
        CEGUI_THROW(UnknownObjectException(
            "AnimationManager::getInterpolator: Interpolator of type '" + type +
            "' not found."));
    }

    return it->second;
}

//----------------------------------------------------------------------------//
Animation* AnimationManager::createAnimation(const String& name)
{
    if (d_animations.find(name) != d_animations.end())
    {
        CEGUI_THROW(UnknownObjectException(
            "AnimationManager::createAnimation: Animation with name '"
            + name + "' already exists."));
    }

    Animation* ret = CEGUI_NEW_AO Animation(name);
    d_animations.insert(std::make_pair(name, ret));

    return ret;
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAnimation(Animation* animation)
{
    destroyAnimation(animation->getName());
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAnimation(const String& name)
{
    AnimationMap::iterator it = d_animations.find(name);

    if (it == d_animations.end())
    {
        CEGUI_THROW(UnknownObjectException(
            "AnimationManager::destroyAnimation: Animation with name '" + name
            + "' not found."));
    }

    Animation* animation = it->second;
    destroyAllInstancesOfAnimation(animation);

    d_animations.erase(it);
    CEGUI_DELETE_AO animation;
}

//----------------------------------------------------------------------------//
Animation* AnimationManager::getAnimation(const String& name) const
{
    AnimationMap::const_iterator it = d_animations.find(name);

    if (it == d_animations.end())
    {
        CEGUI_THROW(UnknownObjectException(
            "AnimationManager::getAnimation: Animation with name '" + name
            + "' not found."));
    }

    return it->second;
}

//----------------------------------------------------------------------------//
Animation* AnimationManager::getAnimationAtIdx(size_t index) const
{
    if (index >= d_animations.size())
    {
        CEGUI_THROW(InvalidRequestException(
            "AnimationManager::getAnimationAtIdx: Out of bounds."));
    }

    AnimationMap::const_iterator it = d_animations.begin();
    std::advance(it, index);

    return it->second;
}

//----------------------------------------------------------------------------//
size_t AnimationManager::getNumAnimations() const
{
    return d_animations.size();
}

//----------------------------------------------------------------------------//
AnimationInstance* AnimationManager::instantiateAnimation(Animation* animation)
{
    AnimationInstance* ret = CEGUI_NEW_AO AnimationInstance(animation);
    d_animationInstances.insert(std::make_pair(animation, ret));

    return ret;
}

//----------------------------------------------------------------------------//
AnimationInstance* AnimationManager::instantiateAnimation(const String& name)
{
    return instantiateAnimation(getAnimation(name));
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAnimationInstance(AnimationInstance* instance)
{
    AnimationInstanceMap::iterator it =
        d_animationInstances.find(instance->getDefinition());

    for (; it != d_animationInstances.end(); ++it)
    {
        if (it->second == instance)
        {
            d_animationInstances.erase(it);
            CEGUI_DELETE_AO instance;
            return;
        }
    }

    CEGUI_THROW(InvalidRequestException(
        "AnimationManager::destroyAnimationInstance: Given animation instance "
        "not found."));
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAllInstancesOfAnimation(Animation* animation)
{
    AnimationInstanceMap::iterator it = d_animationInstances.find(animation);

    // the first instance of given animation is now it->second (if there is any)
    while (it != d_animationInstances.end() && it->first == animation)
    {
        AnimationInstanceMap::iterator toErase = it;
        ++it;

        CEGUI_DELETE_AO toErase->second;
        d_animationInstances.erase(toErase);
    }
}

//----------------------------------------------------------------------------//
AnimationInstance* AnimationManager::getAnimationInstanceAtIdx(size_t index) const
{
    if (index >= d_animationInstances.size())
    {
        CEGUI_THROW(InvalidRequestException(
            "AnimationManager::getAnimationInstanceAtIdx: Out of bounds."));
    }

    AnimationInstanceMap::const_iterator it = d_animationInstances.begin();
    std::advance(it, index);

    return it->second;
}

//----------------------------------------------------------------------------//
size_t AnimationManager::getNumAnimationInstances() const
{
    return d_animationInstances.size();
}

//----------------------------------------------------------------------------//
void AnimationManager::stepInstances(float delta)
{
    for (AnimationInstanceMap::const_iterator it = d_animationInstances.begin();
         it != d_animationInstances.end(); ++it)
    {
        it->second->step(delta);
    }
}

//----------------------------------------------------------------------------//
void AnimationManager::loadAnimationsFromXML(const String& filename,
                                             const String& resourceGroup)
{
    if (filename.empty())
        CEGUI_THROW(InvalidRequestException(
            "AnimationManager::loadAnimationsFromXML: "
            "filename supplied for file loading must be valid."));

    Animation_xmlHandler handler;

    // do parse (which uses handler to create actual data)
    CEGUI_TRY
    {
        System::getSingleton().getXMLParser()->
            parseXMLFile(handler, filename, XMLSchemaName,
                         resourceGroup.empty() ? s_defaultResourceGroup :
                                                 resourceGroup);
    }
    CEGUI_CATCH(...)
    {
        Logger::getSingleton().logEvent(
            "AnimationManager::loadAnimationsFromXML: "
            "loading of animations from file '" + filename + "' has failed.",
            Errors);

        CEGUI_RETHROW;
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

