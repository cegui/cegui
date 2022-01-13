/***********************************************************************
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/PropertyDefinition.h"
#include "CEGUI/Logger.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/AnimationInstance.h"
#include "CEGUI/Animation.h"

namespace CEGUI
{

//---------------------------------------------------------------------------//
WidgetLookFeel::WidgetLookFeel(const String& name, const String& inheritedLookName)
    : d_lookName(name)
    , d_inheritedLookName(inheritedLookName)
{
}

//---------------------------------------------------------------------------//
WidgetLookFeel::~WidgetLookFeel()
{
    clearPropertyDefinitions();
    clearPropertyLinkDefinitions();
}

//---------------------------------------------------------------------------//
const StateImagery& WidgetLookFeel::getStateImagery(const CEGUI::String& name, bool includeInheritedLook) const
{
    auto stateImageryIter = d_stateImageryMap.find(name);

    if (stateImageryIter != d_stateImageryMap.end())
        return stateImageryIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("StateImagery with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getStateImagery(name, true);
}

//---------------------------------------------------------------------------//
const ImagerySection& WidgetLookFeel::getImagerySection(const CEGUI::String& name, bool includeInheritedLook) const
{
    auto imagerySectIter = d_imagerySectionMap.find(name);

    if (imagerySectIter != d_imagerySectionMap.end())
        return imagerySectIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("ImagerySection with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getImagerySection(name, true);
}

//---------------------------------------------------------------------------//
const NamedArea& WidgetLookFeel::getNamedArea(const String& name, bool includeInheritedLook) const
{
    auto namedAreaIter = d_namedAreaMap.find(name);

    if (namedAreaIter != d_namedAreaMap.end())
        return namedAreaIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("NamedArea with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getNamedArea(name, true);
}

//---------------------------------------------------------------------------//
const PropertyInitialiser& WidgetLookFeel::getPropertyInitialiser(const String& name, bool includeInheritedLook) const
{
    auto propertyInitialiserIter = d_propertyInitialiserMap.find(name);

    if (propertyInitialiserIter != d_propertyInitialiserMap.end())
        return propertyInitialiserIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("PropertyInitialiser with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyInitialiser(name, true);
}

//---------------------------------------------------------------------------//
const PropertyDefinitionBase& WidgetLookFeel::getPropertyDefinition(const String& name, bool includeInheritedLook) const
{
    auto propDefIter = d_propertyDefinitionMap.find(name);

    if (propDefIter != d_propertyDefinitionMap.end())
        return *(propDefIter->second);

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("PropertyDefinition with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyDefinition(name, true);
}

//---------------------------------------------------------------------------//
const PropertyDefinitionBase& WidgetLookFeel::getPropertyLinkDefinition(const String& name, bool includeInheritedLook) const
{
    auto propLinkDefIter = d_propertyLinkDefinitionMap.find(name);

    if (propLinkDefIter != d_propertyLinkDefinitionMap.end())
        return *(propLinkDefIter->second);

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("PropertyLinkDefinition with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyLinkDefinition(name, true);
}

//---------------------------------------------------------------------------//
const WidgetComponent& WidgetLookFeel::getWidgetComponent(const String& name, bool includeInheritedLook) const
{
    auto widgetComponentIter = d_widgetComponentMap.find(name);

    if (widgetComponentIter != d_widgetComponentMap.end())
        return widgetComponentIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("WidgetComponent with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getWidgetComponent(name, true);
}

//---------------------------------------------------------------------------//
const EventLinkDefinition& WidgetLookFeel::getEventLinkDefinition(const String& name, bool includeInheritedLook) const
{
    auto eventLinkDefIter = d_eventLinkDefinitionMap.find(name);

    if (eventLinkDefIter != d_eventLinkDefinitionMap.end())
        return eventLinkDefIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("WidgetComponent with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getEventLinkDefinition(name, true);
}

//---------------------------------------------------------------------------//
const String& WidgetLookFeel::getName() const
{
    return d_lookName;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addImagerySection(ImagerySection&& section)
{
    auto foundIter = d_imagerySectionMap.find(section.getName());

    if (foundIter != d_imagerySectionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addImagerySection - Entry for ImagerySection '" +
            section.getName() + "' already exists. Replacing previous definition.");
        d_imagerySectionMap.erase(foundIter);
    }

    d_imagerySectionMap.emplace(section.getName(), std::move(section));
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::renameImagerySection(const String& oldName, const String& newName)
{
	auto itOld = d_imagerySectionMap.find(oldName);
    if (itOld == d_imagerySectionMap.end())
        throw UnknownObjectException("unknown imagery section: '" +
            oldName + "' in look '" + d_lookName + "'.");

    if (d_imagerySectionMap.find(newName) != d_imagerySectionMap.end())
        throw UnknownObjectException("imagery section: '" + newName +
            "' already exists in look '" + d_lookName + "'.");

    itOld->second.setName(newName);
    d_imagerySectionMap.emplace(newName, std::move(itOld->second));
    d_imagerySectionMap.erase(itOld);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addWidgetComponent(const WidgetComponent& widget)
{
    auto foundIter = d_widgetComponentMap.find(widget.getWidgetName());

    if (foundIter != d_widgetComponentMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addWidgetComponent - Entry for WidgetComponent '" +
            widget.getWidgetName() + "' already exists. Replacing previous definition.");
        d_widgetComponentMap.erase(foundIter);
    }

    d_widgetComponentMap.emplace(widget.getWidgetName(), widget);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addStateImagery(const StateImagery& state)
{
    auto foundIter = d_stateImageryMap.find(state.getName());

    if (foundIter != d_stateImageryMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addStateImagery - Entry for StateImagery '" +
            state.getName() + "' already exists. Replacing previous definition.");
        d_stateImageryMap.erase(foundIter);
    }

    d_stateImageryMap.emplace(state.getName(), state);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyInitialiser(const PropertyInitialiser& initialiser)
{
    auto foundIter = d_propertyInitialiserMap.find(initialiser.getTargetPropertyName());

    if (foundIter != d_propertyInitialiserMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addPropertyInitialiser - Entry for PropertyInitialiser '" +
            initialiser.getTargetPropertyName() + "' already exists. Replacing previous definition.");
        d_propertyInitialiserMap.erase(foundIter);
    }

    d_propertyInitialiserMap.emplace(initialiser.getTargetPropertyName(), initialiser);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearImagerySections()
{
    d_imagerySectionMap.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearWidgetComponents()
{
    d_widgetComponentMap.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearStateSpecifications()
{
    d_stateImageryMap.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyInitialisers()
{
    d_propertyInitialiserMap.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::initialiseWidget(Window& widget) const
{
    // add new property definitions
    PropertyDefinitionCollator pdc;
    appendPropertyDefinitions(pdc);
    for (PropertyDefinitionCollator::const_iterator pdi = pdc.begin();
         pdi != pdc.end();
         ++pdi)
    {
        PropertyDefinitionBase* propDefBase = *pdi;
        Property* curProperty = dynamic_cast<Property*>(propDefBase);
        // add the property to the window
        widget.addProperty(curProperty);
    }

    // add required child widgets
    WidgetComponentCollator wcc;
    appendChildWidgetComponents(wcc);
    for (WidgetComponentCollator::const_iterator wci = wcc.begin();
         wci != wcc.end();
         ++wci)
    {
        (*wci)->create(widget);
    }

    // add new property link definitions
    PropertyLinkDefinitionCollator pldc;
    appendPropertyLinkDefinitions(pldc);
    for (PropertyLinkDefinitionCollator::const_iterator pldi = pldc.begin();
         pldi != pldc.end();
         ++pldi)
    {
        // add the property to the window
        widget.addProperty(dynamic_cast<Property*>(*pldi));
    }

    // initialize properties with values
    PropertyInitialiserCollator pic;
    appendPropertyInitialisers(pic);
    for (PropertyInitialiserCollator::const_iterator pi = pic.begin();
         pi != pic.end();
         ++pi)
    {
        (*pi)->apply(widget);
    }

    // setup linked events
    EventLinkDefinitionCollator eldc;
    appendEventLinkDefinitions(eldc);
    for (EventLinkDefinitionCollator::const_iterator eldi = eldc.begin();
         eldi != eldc.end();
         ++eldi)
    {
        (*eldi)->initialiseWidget(widget);
    }

    // create animation instances which automatically react on widget events
    std::unordered_set<String> ans;
    appendAnimationNames(ans);
    for (const auto& animName : ans)
    {
        auto animDef = AnimationManager::getSingleton().getAnimation(animName);
        if (animDef && animDef->hasAutoSubscriptions())
        {
            auto instance = AnimationManager::getSingleton().instantiateAnimation(animDef);
            d_animationInstances.emplace(&widget, instance);
            instance->setTargetWindow(&widget);
        }
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::cleanUpWidget(Window& widget) const
{
    if (widget.getLookNFeel() != getName())
    {
        throw InvalidRequestException("The window '" + 
            widget.getNamePath() + "' does not have this WidgetLook assigned");
    }

    // remove added child widgets
    WidgetComponentCollator wcc;
    appendChildWidgetComponents(wcc);
    for (WidgetComponentCollator::const_iterator wci = wcc.begin();
         wci != wcc.end();
         ++wci)
    {
        (*wci)->cleanup(widget);
    }

    // delete added named Events
    EventLinkDefinitionCollator eldc;
    appendEventLinkDefinitions(eldc);
    for (EventLinkDefinitionCollator::const_iterator eldi = eldc.begin();
         eldi != eldc.end();
         ++eldi)
    {
        (*eldi)->cleanUpWidget(widget);
    }

    // remove added property definitions
    PropertyDefinitionCollator pdc;
    appendPropertyDefinitions(pdc);
    for (PropertyDefinitionCollator::const_iterator pdi = pdc.begin();
         pdi != pdc.end();
         ++pdi)
    {
        // remove the property from the window
        widget.removeProperty((*pdi)->getPropertyName());
    }

    // remove added property link definitions
    PropertyLinkDefinitionCollator pldc;
    appendPropertyLinkDefinitions(pldc);
    for (PropertyLinkDefinitionCollator::const_iterator pldi = pldc.begin();
         pldi != pldc.end();
         ++pldi)
    {
        // remove the property from the window
        widget.removeProperty((*pldi)->getPropertyName());
    }

    // clean up animation instances assoicated with the window.
    const auto range = d_animationInstances.equal_range(&widget);
    for (auto it = range.first; it != range.second; ++it)
        AnimationManager::getSingleton().destroyAnimationInstance(it->second);
    d_animationInstances.erase(range.first, range.second);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isStateImageryPresent(const String& name, bool includeInheritedLook) const
{
    auto i = d_stateImageryMap.find(name);
    
    if (i != d_stateImageryMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;
 
    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isStateImageryPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isImagerySectionPresent(const String& name, bool includeInheritedLook) const
{
    if (d_imagerySectionMap.find(name) != d_imagerySectionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;
 
    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isImagerySectionPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isNamedAreaPresent(const String& name, bool includeInheritedLook) const
{
    auto area = d_namedAreaMap.find(name);

    if (area != d_namedAreaMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isNamedAreaPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isWidgetComponentPresent(const String& name, bool includeInheritedLook) const
{
    auto widgetComponentIter = d_widgetComponentMap.find(name);

    if (widgetComponentIter != d_widgetComponentMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isWidgetComponentPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isPropertyInitialiserPresent(const String& name, bool includeInheritedLook) const
{
    auto propInitialiserIter = d_propertyInitialiserMap.find(name);

    if (propInitialiserIter != d_propertyInitialiserMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isPropertyInitialiserPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isPropertyDefinitionPresent(const String& name, bool includeInheritedLook) const
{
    auto propDefIter = d_propertyDefinitionMap.find(name);

    if (propDefIter != d_propertyDefinitionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isPropertyDefinitionPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isPropertyLinkDefinitionPresent(const String& name, bool includeInheritedLook) const
{
    auto propLinkDefIter = d_propertyLinkDefinitionMap.find(name);

    if (propLinkDefIter != d_propertyLinkDefinitionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isPropertyLinkDefinitionPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isEventLinkDefinitionPresent(const String& name, bool includeInheritedLook) const
{
    auto eventLinkDefIter = d_eventLinkDefinitionMap.find(name);

    if (eventLinkDefIter != d_eventLinkDefinitionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isEventLinkDefinitionPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isAnimationPresent(const String& name, bool includeInheritedLook) const
{
    if (name.empty())
        return false;

    if (std::find(d_animations.cbegin(), d_animations.cend(), name) != d_animations.end())
        return true;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isAnimationPresent(name, true);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addNamedArea(const NamedArea& area)
{
    auto foundIter = d_namedAreaMap.find(area.getName());

    if (foundIter != d_namedAreaMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addNamedArea - Entry for NamedArea '" +
            area.getName() + "' already exists. Replacing previous definition.");
        d_namedAreaMap.erase(foundIter);
    }

    d_namedAreaMap.emplace(area.getName(), area);
}


//---------------------------------------------------------------------------//
void WidgetLookFeel::renameNamedArea(const String& oldName, const String& newName)
{
    auto oldarea = d_namedAreaMap.find(oldName);
    auto newarea = d_namedAreaMap.find(newName);
    if (oldarea == d_namedAreaMap.end())
        throw UnknownObjectException("unknown named area: '" + oldName +
            "' in look '" + d_lookName + "'.");

    if (newarea != d_namedAreaMap.end())
        throw UnknownObjectException("named area: '" + newName +
            "' already exists in look '" + d_lookName + "'.");

    oldarea->second.setName(newName);
    d_namedAreaMap[newName] = d_namedAreaMap[oldName];
    d_namedAreaMap.erase(oldarea);
}
//---------------------------------------------------------------------------//
void WidgetLookFeel::clearNamedAreas()
{
    d_namedAreaMap.clear();
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::layoutChildWidgets(const Window& owner) const
{
    WidgetComponentCollator wcc;
    appendChildWidgetComponents(wcc);

    bool changed = false;
    for (WidgetComponentCollator::const_iterator wci = wcc.begin();
         wci != wcc.end();
         ++wci)
    {
        changed |= (*wci)->layout(owner);
    }

    return changed;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyDefinition(PropertyDefinitionBase* propertyDefiniton)
{
    auto foundIter = d_propertyDefinitionMap.find(propertyDefiniton->getPropertyName());

    if(foundIter != d_propertyDefinitionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addPropertyDefinition - Entry for PropertyDefinition '" +
            propertyDefiniton->getPropertyName() + "' already exists. Deleting and replacing previous definition.");
        delete(foundIter->second);
        d_propertyDefinitionMap.erase(foundIter);
    }

    d_propertyDefinitionMap.emplace(propertyDefiniton->getPropertyName(), propertyDefiniton);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyDefinitions()
{
    for (const auto& pair : d_propertyDefinitionMap)
        delete (pair.second);
    d_propertyDefinitionMap.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyLinkDefinition(PropertyDefinitionBase* propertyLinkDefiniton)
{
    auto foundIter = d_propertyLinkDefinitionMap.find(propertyLinkDefiniton->getPropertyName());

    if (foundIter != d_propertyLinkDefinitionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addPropertyLinkDefinition - Entry for PropertyLinkDefinition '" +
            propertyLinkDefiniton->getPropertyName() + "' already exists. Deleting and replacing previous definition.");
        delete foundIter->second;
        d_propertyLinkDefinitionMap.erase(foundIter);
    }

    d_propertyLinkDefinitionMap.emplace(propertyLinkDefiniton->getPropertyName(), propertyLinkDefiniton);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyLinkDefinitions()
{
    for (const auto& pair : d_propertyLinkDefinitionMap)
        delete pair.second;
    d_propertyLinkDefinitionMap.clear();
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getStateImageryNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_stateImageryMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getStateImageryNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getImagerySectionNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_imagerySectionMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getImagerySectionNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getNamedAreaNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_namedAreaMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getNamedAreaNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getWidgetComponentNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_widgetComponentMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getWidgetComponentNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getPropertyDefinitionNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_propertyDefinitionMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyDefinitionNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}


//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getPropertyLinkDefinitionNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_propertyLinkDefinitionMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyLinkDefinitionNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getPropertyInitialiserNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_propertyInitialiserMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyInitialiserNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}


//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getEventLinkDefinitionNames(bool includeInheritedLook) const
{
    StringSet result;
    for (const auto& pair : d_eventLinkDefinitionMap)
        result.insert(pair.first);

    if (!d_inheritedLookName.empty() && includeInheritedLook)
    {
        StringSet inheritedStringSet = WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getEventLinkDefinitionNames(true);
        result.insert(inheritedStringSet.begin(), inheritedStringSet.end());
    }

    return result;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getAnimationNames(bool includeInheritedLook) const
{
    std::unordered_set<String> ans;
    appendAnimationNames(ans, includeInheritedLook);
    return ans;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::writeXMLToStream(XMLSerializer& xml_stream) const
{
    xml_stream.openTag(Falagard_xmlHandler::WidgetLookElement);

    xml_stream.attribute(Falagard_xmlHandler::NameAttribute, d_lookName);

    if(!d_inheritedLookName.empty())
        xml_stream.attribute(Falagard_xmlHandler::InheritsAttribute, d_inheritedLookName);

    // output property definitions
    for (const auto& pair : d_propertyDefinitionMap)
        pair.second->writeDefinitionXMLToStream(xml_stream);

    // output property link definitions
    for (const auto& pair : d_propertyLinkDefinitionMap)
        pair.second->writeDefinitionXMLToStream(xml_stream);

    // output property initialisers.
    for (const auto& pair : d_propertyInitialiserMap)
        pair.second.writeXMLToStream(xml_stream);

    // output named areas
    for (const auto& pair : d_namedAreaMap)
        pair.second.writeXMLToStream(xml_stream);

    // output child widgets
    for (const auto& pair : d_widgetComponentMap)
        pair.second.writeXMLToStream(xml_stream);

    // output imagery sections
    for (const auto& pair : d_imagerySectionMap)
        pair.second.writeXMLToStream(xml_stream);

    // output states
    for (const auto& pair : d_stateImageryMap)
        pair.second.writeXMLToStream(xml_stream);

    xml_stream.closeTag();
}

//---------------------------------------------------------------------------//
const PropertyInitialiser* WidgetLookFeel::findPropertyInitialiser(const String& propertyName) const
{
    PropertyInitialiserCollator pic;
    appendPropertyInitialisers(pic);

    PropertyInitialiserCollator::const_iterator i = pic.find(propertyName);

    if (i == pic.end())
        return nullptr;

    return *i;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addAnimationName(const String& anim_name)
{
    auto it = std::find(d_animations.begin(), d_animations.end(), anim_name);
    if (it == d_animations.end())
        d_animations.push_back(anim_name);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addEventLinkDefinition(const EventLinkDefinition& evtdef)
{
    auto foundIter = d_eventLinkDefinitionMap.find(evtdef.getName());

    if (foundIter != d_eventLinkDefinitionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addEventLinkDefinition - Entry for EventLinkDefinition '" +
            evtdef.getName() + "' already exists. Replacing previous definition.");
        d_eventLinkDefinitionMap.erase(foundIter);
    }

    d_eventLinkDefinitionMap.emplace(evtdef.getName(), evtdef);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearEventLinkDefinitions()
{
    d_eventLinkDefinitionMap.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendChildWidgetComponents(WidgetComponentCollator& col,
                                                 bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendChildWidgetComponents(col);

    for (const auto& pair : d_widgetComponentMap)
        col.set(pair.first, &pair.second);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyDefinitions(PropertyDefinitionCollator& col,
                                               bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyDefinitions(col);

    for (const auto& pair : d_propertyDefinitionMap)
        col.set(pair.first, pair.second);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyLinkDefinitions(
        PropertyLinkDefinitionCollator& col, bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyLinkDefinitions(col);

    for (const auto& pair : d_propertyLinkDefinitionMap)
        col.set(pair.first, pair.second);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyInitialisers(
        PropertyInitialiserCollator& col, bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyInitialisers(col);

    for (const auto& pair : d_propertyInitialiserMap)
        col.set(pair.first, &pair.second);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendEventLinkDefinitions(
        EventLinkDefinitionCollator& col, bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendEventLinkDefinitions(col);

    for (const auto& pair : d_eventLinkDefinitionMap)
        col.set(pair.first, &pair.second);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendAnimationNames(std::unordered_set<String>& set, bool inherits) const
{
    set.insert(d_animations.begin(), d_animations.end());

    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendAnimationNames(set);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::handleFontRenderSizeChange(Window& window, const Font* font) const
{
    bool result = false;

    for (const auto& pair : d_imagerySectionMap)
        result |= pair.second.handleFontRenderSizeChange(window, font);

    for (const auto& pair : d_widgetComponentMap)
        result |= pair.second.handleFontRenderSizeChange(window, font);

    if (!d_inheritedLookName.empty())
        result |= WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).
                handleFontRenderSizeChange(window, font);

    return result;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StateImageryPointerMap WidgetLookFeel::getStateImageryMap(bool includeInheritedLook)
{
    StateImageryPointerMap pointerMap;
    StringSet nameSet = getStateImageryNames(includeInheritedLook);

    for (const String& currentElementName : nameSet)
    {
        auto stateImageryIter = d_stateImageryMap.find(currentElementName);
        if (stateImageryIter == d_stateImageryMap.end())
        {
            throw UnknownObjectException("Error: StateImagery with name: \"" + currentElementName
                + "\" exists in the list of names but a StateImagery with this name"
                + "could not be found in the map");
        }
        else
        {
            StateImagery* stateImagery = &stateImageryIter->second;
            pointerMap.insert(StateImageryPointerMap::value_type(currentElementName, stateImagery));
        }
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::ImagerySectionPointerMap WidgetLookFeel::getImagerySectionMap(bool includeInheritedLook)
{
    ImagerySectionPointerMap pointerMap;
    StringSet nameSet = getImagerySectionNames(includeInheritedLook);
    
    for (const String& currentElementName : nameSet)
    {
        auto imagerySectionIter = d_imagerySectionMap.find(currentElementName);
        if (imagerySectionIter == d_imagerySectionMap.end())
        {
            throw UnknownObjectException("Error: ImagerySection with name: \"" + currentElementName
                                               + "\" exists in the list of names but a ImagerySection with this name"
                                               + "could not be found in the map");
        }
        else
        {
            pointerMap.emplace(currentElementName, &imagerySectionIter->second);
        }
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::NamedAreaPointerMap WidgetLookFeel::getNamedAreaMap(bool includeInheritedLook)
{
    NamedAreaPointerMap pointerMap;
    StringSet nameSet = getNamedAreaNames(includeInheritedLook);

    for (const String& currentElementName : nameSet)
    {
        auto namedAreaIter = d_namedAreaMap.find(currentElementName);
        if (namedAreaIter == d_namedAreaMap.end())
        {
            throw UnknownObjectException("Error: NamedArea with name: \"" + currentElementName
                + "\" exists in the list of names but a NamedArea with this name"
                + "could not be found in the map");
        }
        else
        {
            NamedArea* namedArea = &namedAreaIter->second;
            pointerMap.insert(NamedAreaPointerMap::value_type(currentElementName, namedArea));
        }
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::WidgetComponentPointerMap WidgetLookFeel::getWidgetComponentMap(bool includeInheritedLook)
{
    WidgetComponentPointerMap pointerMap;
    StringSet nameSet = getWidgetComponentNames(includeInheritedLook);

    for (const String& currentElementName : nameSet)
    {
        auto widgetComponentIter = d_widgetComponentMap.find(currentElementName);
        if (widgetComponentIter == d_widgetComponentMap.end())
        {
            throw UnknownObjectException("Error: WidgetComponent with name: \"" + currentElementName
                + "\" exists in the list of names but a WidgetComponent with this name"
                + "could not be found in the map");
        }
        else
        {
            WidgetComponent* widgetComponent = &widgetComponentIter->second;
            pointerMap.insert(WidgetComponentPointerMap::value_type(currentElementName, widgetComponent));
        }
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::PropertyInitialiserPointerMap WidgetLookFeel::getPropertyInitialiserMap(bool includeInheritedLook)
{
    PropertyInitialiserPointerMap pointerMap;
    StringSet nameSet = getPropertyInitialiserNames(includeInheritedLook);

    for (const String& currentElementName : nameSet)
    {
        auto propertyInitIter = d_propertyInitialiserMap.find(currentElementName);
        if (propertyInitIter == d_propertyInitialiserMap.end())
        {
            throw UnknownObjectException("Error: PropertyInitialiser with name: \"" + currentElementName
                + "\" exists in the list of names but a PropertyInitialiser with this name"
                + "could not be found in the map");
        }
        else
        {
            PropertyInitialiser* propInitialiser = &propertyInitIter->second;
            pointerMap.insert(PropertyInitialiserPointerMap::value_type(currentElementName, propInitialiser));
        }
    }

    return pointerMap;
}


//---------------------------------------------------------------------------//
WidgetLookFeel::PropertyDefinitionBasePointerMap WidgetLookFeel::getPropertyDefinitionMap(bool includeInheritedLook)
{
    PropertyDefinitionBasePointerMap pointerMap;
    StringSet nameSet = getPropertyDefinitionNames(includeInheritedLook);

    for (const String& currentElementName : nameSet)
    {
        auto propDefIter = d_propertyDefinitionMap.find(currentElementName);
        if (propDefIter == d_propertyDefinitionMap.end())
        {
            throw UnknownObjectException("Error: PropertyDefinition with name: \"" + currentElementName
                + "\" exists in the list of names but a PropertyDefinition with this name"
                + "could not be found in the map");
        }
        else
        {
            PropertyDefinitionBase* propertyDefinition = propDefIter->second;
            pointerMap.insert(PropertyDefinitionBasePointerMap::value_type(currentElementName, propertyDefinition));
        }
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::PropertyDefinitionBasePointerMap WidgetLookFeel::getPropertyLinkDefinitionMap(bool includeInheritedLook)
{
    PropertyDefinitionBasePointerMap pointerMap;
    StringSet nameSet = getPropertyLinkDefinitionNames(includeInheritedLook);

    for (const String& currentElementName : nameSet)
    {
        auto propLinkDefIter = d_propertyLinkDefinitionMap.find(currentElementName);
        if (propLinkDefIter == d_propertyLinkDefinitionMap.end())
        {
            throw UnknownObjectException("Error: PropertyLinkDefinition with name: \"" + currentElementName
                + "\" exists in the list of names but a PropertyLinkDefinition with this name"
                + "could not be found in the map");
        }
        else
        {        
            PropertyDefinitionBase* propertyLinkDefinition = propLinkDefIter->second;
            pointerMap.insert(PropertyDefinitionBasePointerMap::value_type(currentElementName, propertyLinkDefinition));
        }
    }

    return pointerMap;
}

 
//---------------------------------------------------------------------------//
WidgetLookFeel::EventLinkDefinitionPointerMap WidgetLookFeel::getEventLinkDefinitionMap(bool includeInheritedLook)
{
    EventLinkDefinitionPointerMap pointerMap;
    StringSet nameSet = getEventLinkDefinitionNames(includeInheritedLook);

    for (const String& currentElementName : nameSet)
    {
        auto eventLinkDefIter = d_eventLinkDefinitionMap.find(currentElementName);
        if (eventLinkDefIter == d_eventLinkDefinitionMap.end())
        {
            throw UnknownObjectException("Error: EventLinkDefinition with name: \"" + currentElementName
                + "\" exists in the list of names but a EventLinkDefinition with this name"
                + "could not be found in the map");
        }
        else
        {
            EventLinkDefinition* eventLinkDefinition = &eventLinkDefIter->second;
            pointerMap.insert(EventLinkDefinitionPointerMap::value_type(currentElementName, eventLinkDefinition));
        }
    }

    return pointerMap;
}


//---------------------------------------------------------------------------//
const CEGUI::String& WidgetLookFeel::getInheritedWidgetLookName() const
{
    return d_inheritedLookName;
}

//---------------------------------------------------------------------------//
WidgetLookFeel* WidgetLookFeel::getInheritedWidgetLookFeel()
{
    if (d_inheritedLookName.empty())
        return nullptr;

    WidgetLookManager::WidgetLookPointerMap pointerMap = WidgetLookManager::getSingleton().getWidgetLookPointerMap();
    WidgetLookManager::WidgetLookPointerMap::iterator foundIter = pointerMap.find(d_inheritedLookName);

    if (foundIter == pointerMap.end())
        throw UnknownObjectException("Error: Inherited WidgetLook with name: \"" + d_inheritedLookName
                                           + "\" cannot be found in the WidgetLookManager's map");

    return foundIter->second;
}

}
