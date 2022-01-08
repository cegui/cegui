/***********************************************************************
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
#include "CEGUI/AnimationManager.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/Rectf.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Animation.h"
#include "CEGUI/AnimationInstance.h"
#include "CEGUI/TplInterpolators.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/XMLSerializer.h"
#include "CEGUI/Animation_xmlHandler.h"
#include "CEGUI/Quaternion.h"
#include "CEGUI/SharedStringStream.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> AnimationManager* Singleton<AnimationManager>::ms_Singleton  = nullptr;
// Name of the xsd schema file used to validate animation XML files.
const String AnimationManager::XMLSchemaName("Animation.xsd");
// String that holds the default resource group for loading animations
String AnimationManager::s_defaultResourceGroup("");
const String AnimationManager::GeneratedAnimationNameBase("__ceanim_uid_");

/*************************************************************************
    Constructor
*************************************************************************/
AnimationManager::AnimationManager()
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::AnimationManager Singleton created. (" + addressStr + ")");

    // todo: is this too dirty?
#   define addBasicInterpolator(i) { Interpolator* in = i; addInterpolator(in); d_basicInterpolators.push_back(in); }

    // create and add basic interpolators shipped with CEGUI
    addBasicInterpolator(new TplDiscreteRelativeInterpolator<String>("String"));
    addBasicInterpolator(new TplLinearInterpolator<float>("float"));
    addBasicInterpolator(new TplLinearInterpolator<std::int32_t>("int32"));
    addBasicInterpolator(new TplLinearInterpolator<std::uint32_t>("uint32"));
    addBasicInterpolator(new TplDiscreteInterpolator<bool>("bool"));
    addBasicInterpolator(new TplLinearInterpolator<Sizef>("Sizef"));
    addBasicInterpolator(new TplLinearInterpolator<glm::vec2>("vec2"));
    addBasicInterpolator(new TplLinearInterpolator<glm::vec3>("vec3"));
    addBasicInterpolator(new QuaternionSlerpInterpolator());
    addBasicInterpolator(new TplLinearInterpolator<Rectf>("Rectf"));
    addBasicInterpolator(new TplLinearInterpolator<Colour>("Colour"));
    addBasicInterpolator(new TplLinearInterpolator<ColourRect>("ColourRect"));
    addBasicInterpolator(new TplLinearInterpolator<UDim>("UDim"));
    addBasicInterpolator(new TplLinearInterpolator<UVector2>("UVector2"));
    addBasicInterpolator(new TplLinearInterpolator<URect>("URect"));
    addBasicInterpolator(new TplLinearInterpolator<UBox>("UBox"));
    addBasicInterpolator(new TplLinearInterpolator<USize>("USize"));
}


/*************************************************************************
    Destructor
*************************************************************************/
AnimationManager::~AnimationManager()
{
    // by destroying all animations their instances also get deleted
    destroyAllAnimations();

    // and lastly, we remove all interpolators, but we don't delete them!
    // it is the creator's responsibility to delete them
    d_interpolators.clear();

    // we only destroy inbuilt interpolators
    for (Interpolator* i : d_basicInterpolators)
        delete i;

    d_basicInterpolators.clear();

    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "CEGUI::AnimationManager singleton destroyed " + addressStr);
}

//----------------------------------------------------------------------------//
void AnimationManager::addInterpolator(Interpolator* interpolator)
{
    if (d_interpolators.find(interpolator->getType()) != d_interpolators.end())
    {
        throw AlreadyExistsException("Interpolator of type '"
            + interpolator->getType() + "' already exists.");
    }

    d_interpolators.insert(
        std::make_pair(interpolator->getType(), interpolator));
}

//----------------------------------------------------------------------------//
void AnimationManager::removeInterpolator(Interpolator* interpolator)
{
    auto it = d_interpolators.find(interpolator->getType());

    if (it == d_interpolators.end())
    {
        throw UnknownObjectException("Interpolator of type '"
            + interpolator->getType() + "' not found.");
    }

    d_interpolators.erase(it);
}

//----------------------------------------------------------------------------//
Interpolator* AnimationManager::getInterpolator(const String& type) const
{
    auto it = d_interpolators.find(type);

    if (it == d_interpolators.end())
    {
        throw UnknownObjectException("Interpolator of type '" + type +
            "' not found.");
    }

    return it->second;
}

//----------------------------------------------------------------------------//
Animation* AnimationManager::createAnimation(const String& name)
{
    if (isAnimationPresent(name))
    {
        throw UnknownObjectException("Animation with name '"
            + name + "' already exists.");
    }

    String finalName(name.empty() ? generateUniqueAnimationName() : name);

    Animation* ret = new Animation(finalName);
    d_animations.insert(std::make_pair(finalName, ret));

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
    auto it = d_animations.find(name);

    if (it == d_animations.end())
    {
        throw UnknownObjectException("Animation with name '" + name
            + "' not found.");
    }

    Animation* animation = it->second;
    destroyAllInstancesOfAnimation(animation);

    d_animations.erase(it);
    delete animation;
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAllAnimations()
{
    // we have to destroy all instances to avoid dangling pointers
    // destroying all instances now is also faster than doing that for each
    // animation that is being destroyed
    destroyAllAnimationInstances();
    
    for (const auto& pair : d_animations)
        delete pair.second;

    d_animations.clear();
}

//----------------------------------------------------------------------------//
Animation* AnimationManager::getAnimation(const String& name) const
{
    auto it = d_animations.find(name);

    if (it == d_animations.end())
    {
        throw UnknownObjectException("Animation with name '" + name
            + "' not found.");
    }

    return it->second;
}

//----------------------------------------------------------------------------//
bool AnimationManager::isAnimationPresent(const String& name) const
{
    return (d_animations.find(name) != d_animations.end());
}

//----------------------------------------------------------------------------//
Animation* AnimationManager::getAnimationAtIndex(size_t index) const
{
    if (index >= d_animations.size())
    {
        throw InvalidRequestException("Out of bounds.");
    }

    auto it = d_animations.begin();
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
	if (!animation)
	{
		throw InvalidRequestException("I refuse to instantiate NULL "
            "animation, please provide a valid pointer.");
	}

    AnimationInstance* ret = new AnimationInstance(animation);
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
    const auto range = d_animationInstances.equal_range(instance->getDefinition());
    for (auto it = range.first; it != range.second; ++it)
    {
        if (it->second == instance)
        {
            d_animationInstances.erase(it);
            delete instance;
            return;
        }
    }

    throw InvalidRequestException("Given animation instance not found.");
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAnimationInstances(PropertySet* target)
{
    for (auto it = d_animationInstances.begin(); it != d_animationInstances.end(); /**/)
    {
        if (it->second->getTarget() == target)
        {
            delete it->second;
            it = d_animationInstances.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAllInstancesOfAnimation(Animation* animation)
{
    const auto range = d_animationInstances.equal_range(animation);
    for (auto it = range.first; it != range.second; ++it)
        delete it->second;
    d_animationInstances.erase(range.first, range.second);
}

//----------------------------------------------------------------------------//
void AnimationManager::destroyAllAnimationInstances()
{
    for (const auto& pair : d_animationInstances)
        delete pair.second;
    d_animationInstances.clear();
}

//----------------------------------------------------------------------------//
AnimationInstance* AnimationManager::getAnimationInstanceAtIndex(size_t index) const
{
    if (index >= d_animationInstances.size())
        throw InvalidRequestException("Out of bounds.");

    auto it = d_animationInstances.begin();
    std::advance(it, index);

    return it->second;
}

//----------------------------------------------------------------------------//
AnimationInstance* AnimationManager::getAnimationInstance(Animation* animation, PropertySet* target) const
{
    if (!animation || !target)
        return nullptr;

    const auto range = d_animationInstances.equal_range(animation);
    for (auto it = range.first; it != range.second; ++it)
        if (it->second->getTarget() == target)
            return it->second;

    return nullptr;
}

//----------------------------------------------------------------------------//
size_t AnimationManager::getNumAnimationInstances() const
{
    return d_animationInstances.size();
}

//----------------------------------------------------------------------------//
void AnimationManager::autoStepInstances(float delta)
{
    for (const auto& pair : d_animationInstances)
    	if (pair.second->isAutoSteppingEnabled())
            pair.second->step(delta);
}

//----------------------------------------------------------------------------//
void AnimationManager::loadAnimationsFromXML(const String& filename,
                                             const String& resourceGroup)
{
    if (filename.empty())
        throw InvalidRequestException(
            "filename supplied for file loading must be valid.");

    Animation_xmlHandler handler;

    // do parse (which uses handler to create actual data)
    try
    {
        System::getSingleton().getXMLParser()->
            parseXMLFile(handler, filename, XMLSchemaName,
                         resourceGroup.empty() ? s_defaultResourceGroup :
                                                 resourceGroup);
    }
    catch (...)
    {
        Logger::getSingleton().logEvent(
            "AnimationManager::loadAnimationsFromXML: "
            "loading of animations from file '" + filename + "' has failed.",
            LoggingLevel::Error);

        throw;
    }
}

void AnimationManager::loadAnimationsFromString(const String& source)
{
    Animation_xmlHandler handler;

    // do parse (which uses handler to create actual data)
    try
    {
        System::getSingleton().getXMLParser()->parseXMLString(handler, source, XMLSchemaName);
    }
    catch (...)
    {
        Logger::getSingleton().logEvent("AnimationManager::loadAnimationsFromString - loading of animations from string failed.", LoggingLevel::Error);
        throw;
    }
}

//---------------------------------------------------------------------------//
void AnimationManager::writeAnimationDefinitionToStream(const Animation& animation, OutStream& out_stream) const
{
    XMLSerializer xml(out_stream);

    animation.writeXMLToStream(xml);
}

//---------------------------------------------------------------------------//
String AnimationManager::getAnimationDefinitionAsString(const Animation& animation) const
{
    std::ostringstream str;
    writeAnimationDefinitionToStream(animation, str);

    return String(reinterpret_cast<const String::value_type*>(str.str().c_str()));
}

//---------------------------------------------------------------------------//
String AnimationManager::generateUniqueAnimationName()
{
    const String ret = GeneratedAnimationNameBase +
        PropertyHelper<std::uint32_t>::toString(d_uid_counter);

    // update counter for next time
    unsigned long old_uid = d_uid_counter;
    ++d_uid_counter;

    // log if we ever wrap-around (which should be pretty unlikely)
    if (d_uid_counter < old_uid)
        Logger::getSingleton().logEvent("UID counter for generated Animation "
            "names has wrapped around - the fun shall now commence!");

    return ret;
}

} // End of  CEGUI namespace section
