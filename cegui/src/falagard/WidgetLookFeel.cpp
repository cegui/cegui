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
#include "CEGUI/falagard/XMLHandler.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/AnimationInstance.h"
#include <iostream>
#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
//---------------------------------------------------------------------------//
WidgetLookFeel::WidgetLookFeel(const String& name, const String& inheritedLookName) :
    d_lookName(name),
    d_inheritedLookName(inheritedLookName)
{
}

//---------------------------------------------------------------------------//
WidgetLookFeel::WidgetLookFeel(const WidgetLookFeel& other) :
    d_lookName(other.d_lookName),
    d_inheritedLookName(other.d_inheritedLookName),
    d_imagerySectionMap(other.d_imagerySectionMap),
    d_widgetComponentMap(other.d_widgetComponentMap),
    d_stateImageryMap(other.d_stateImageryMap),
    d_propertyInitialiserMap(other.d_propertyInitialiserMap),
    d_namedAreaMap(other.d_namedAreaMap),
    d_animations(other.d_animations),
    d_animationInstances(other.d_animationInstances),
    d_eventLinkDefinitionMap(other.d_eventLinkDefinitionMap)
{
    copyPropertyDefinitionsFrom(other);
    copyPropertyLinkDefinitionsFrom(other);
}

//---------------------------------------------------------------------------//
WidgetLookFeel& WidgetLookFeel::operator=(const WidgetLookFeel& other)
{
    WidgetLookFeel tmp(other);
    swap(tmp);
    return *this;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::swap(WidgetLookFeel& other)
{
    std::swap(d_lookName, other.d_lookName);
    std::swap(d_inheritedLookName, other.d_inheritedLookName);
    std::swap(d_imagerySectionMap, other.d_imagerySectionMap);
    std::swap(d_widgetComponentMap, other.d_widgetComponentMap);
    std::swap(d_stateImageryMap, other.d_stateImageryMap);
    std::swap(d_propertyInitialiserMap, other.d_propertyInitialiserMap);
    std::swap(d_namedAreaMap, other.d_namedAreaMap);
    std::swap(d_propertyDefinitionMap, other.d_propertyDefinitionMap);
    std::swap(d_propertyLinkDefinitionMap, other.d_propertyLinkDefinitionMap);
    std::swap(d_animations, other.d_animations);
    std::swap(d_animationInstances, other.d_animationInstances);
    std::swap(d_eventLinkDefinitionMap, other.d_eventLinkDefinitionMap);
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
    StateImageryMap::const_iterator stateImageryIter = d_stateImageryMap.find(name);

    if (stateImageryIter != d_stateImageryMap.end())
        return stateImageryIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("StateImagery with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getStateImagery(name, true);
}

//---------------------------------------------------------------------------//
const ImagerySection& WidgetLookFeel::getImagerySection(const CEGUI::String& name, bool includeInheritedLook) const
{
    ImagerySectionMap::const_iterator imagerySectIter = d_imagerySectionMap.find(name);

    if (imagerySectIter != d_imagerySectionMap.end())
        return imagerySectIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("ImagerySection with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getImagerySection(name, true);
}

//---------------------------------------------------------------------------//
const NamedArea& WidgetLookFeel::getNamedArea(const String& name, bool includeInheritedLook) const
{
    NamedAreaMap::const_iterator namedAreaIter = d_namedAreaMap.find(name);

    if (namedAreaIter != d_namedAreaMap.end())
        return namedAreaIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("NamedArea with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getNamedArea(name, true);
}

//---------------------------------------------------------------------------//
const PropertyInitialiser& WidgetLookFeel::getPropertyInitialiser(const String& name, bool includeInheritedLook) const
{
    PropertyInitialiserMap::const_iterator propertyInitialiserIter = d_propertyInitialiserMap.find(name);

    if (propertyInitialiserIter != d_propertyInitialiserMap.end())
        return propertyInitialiserIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("PropertyInitialiser with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyInitialiser(name, true);
}

//---------------------------------------------------------------------------//
const PropertyDefinitionBase& WidgetLookFeel::getPropertyDefinition(const String& name, bool includeInheritedLook) const
{
    PropertyDefinitionMap::const_iterator propDefIter = d_propertyDefinitionMap.find(name);

    if (propDefIter != d_propertyDefinitionMap.end())
        return *(propDefIter->second);

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("PropertyDefinition with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyDefinition(name, true);
}

//---------------------------------------------------------------------------//
const PropertyDefinitionBase& WidgetLookFeel::getPropertyLinkDefinition(const String& name, bool includeInheritedLook) const
{
    PropertyLinkDefinitionMap::const_iterator propLinkDefIter = d_propertyLinkDefinitionMap.find(name);

    if (propLinkDefIter != d_propertyLinkDefinitionMap.end())
        return *(propLinkDefIter->second);

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("PropertyLinkDefinition with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getPropertyLinkDefinition(name, true);
}

//---------------------------------------------------------------------------//
const WidgetComponent& WidgetLookFeel::getWidgetComponent(const String& name, bool includeInheritedLook) const
{
    WidgetComponentMap::const_iterator widgetComponentIter = d_widgetComponentMap.find(name);

    if (widgetComponentIter != d_widgetComponentMap.end())
        return widgetComponentIter->second;

    if (!includeInheritedLook || d_inheritedLookName.empty())
        throw UnknownObjectException("WidgetComponent with name '" + name + "' was not found in WidgetLookFeel '" + d_lookName + "'.");

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).getWidgetComponent(name, true);
}

//---------------------------------------------------------------------------//
const EventLinkDefinition& WidgetLookFeel::getEventLinkDefinition(const String& name, bool includeInheritedLook) const
{
    EventLinkDefinitionMap::const_iterator eventLinkDefIter = d_eventLinkDefinitionMap.find(name);

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
void WidgetLookFeel::addImagerySection(const ImagerySection& section)
{
    String name = section.getName();
    ImagerySectionMap::iterator foundIter = d_imagerySectionMap.find(name);

    if(foundIter != d_imagerySectionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addImagerySection - Entry for ImagerySection '" +
                                        name + "' already exists. Replacing previous definition.");
        d_imagerySectionMap.erase(foundIter);
    }

    d_imagerySectionMap.insert(ImagerySectionMap::value_type(name, section));
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::renameImagerySection(const String& oldName, const String& newName)
{
	ImagerySectionMap::iterator oldsection = d_imagerySectionMap.find(oldName);
    if (oldsection == d_imagerySectionMap.end())
        throw UnknownObjectException("unknown imagery section: '" +
            oldName + "' in look '" + d_lookName + "'.");

    if (d_imagerySectionMap.find(newName) != d_imagerySectionMap.end())
        throw UnknownObjectException("imagery section: '" + newName +
            "' already exists in look '" + d_lookName + "'.");

    oldsection->second.setName(newName);
    d_imagerySectionMap[newName] = d_imagerySectionMap[oldName];
    d_imagerySectionMap.erase(oldsection);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addWidgetComponent(const WidgetComponent& widget)
{
    String name = widget.getWidgetName();
    WidgetComponentMap::iterator foundIter = d_widgetComponentMap.find(name);

    if(foundIter != d_widgetComponentMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addWidgetComponent - Entry for WidgetComponent '" +
            name + "' already exists. Replacing previous definition.");
        d_widgetComponentMap.erase(foundIter);
    }

    d_widgetComponentMap.insert(WidgetComponentMap::value_type(name, widget));
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addStateImagery(const StateImagery& state)
{
    String name = state.getName();
    StateImageryMap::iterator foundIter = d_stateImageryMap.find(name);

    if(foundIter != d_stateImageryMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addStateImagery - Entry for StateImagery '" +
            name + "' already exists. Replacing previous definition.");
        d_stateImageryMap.erase(foundIter);
    }

    d_stateImageryMap.insert(StateImageryMap::value_type(name, state));
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyInitialiser(const PropertyInitialiser& initialiser)
{
    String name = initialiser.getTargetPropertyName();
    PropertyInitialiserMap::iterator foundIter = d_propertyInitialiserMap.find(name);

    if(foundIter != d_propertyInitialiserMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addPropertyInitialiser - Entry for PropertyInitialiser '" +
            name + "' already exists. Replacing previous definition.");
        d_propertyInitialiserMap.erase(foundIter);
    }

    d_propertyInitialiserMap.insert(PropertyInitialiserMap::value_type(name, initialiser));
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
    // apply properties to the parent window
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

    // create animation instances
    AnimationNameSet ans;
    appendAnimationNames(ans);
    for (AnimationNameSet::const_iterator ani = ans.begin();
         ani != ans.end();
         ++ani)
    {
        AnimationInstance* instance =
            AnimationManager::getSingleton().instantiateAnimation(*ani);

        d_animationInstances.insert(AnimationInstanceMap::value_type(&widget, instance));
        instance->setTargetWindow(&widget);
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

    // clean up animation instances assoicated wit the window.
    AnimationInstanceMap::iterator anim;
    while ((anim = d_animationInstances.find(&widget)) != d_animationInstances.end())
    {
        AnimationManager::getSingleton().destroyAnimationInstance(anim->second);
        d_animationInstances.erase(anim);
    }
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isStateImageryPresent(const String& name, bool includeInheritedLook) const
{
    StateImageryMap::const_iterator i = d_stateImageryMap.find(name);
    
    if (i != d_stateImageryMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;
 
    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isStateImageryPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isImagerySectionPresent(const String& name, bool includeInheritedLook) const
{
    ImagerySectionMap::const_iterator i = d_imagerySectionMap.find(name);
    
    if (i != d_imagerySectionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;
 
    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isImagerySectionPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isNamedAreaPresent(const String& name, bool includeInheritedLook) const
{
    NamedAreaMap::const_iterator area = d_namedAreaMap.find(name);

    if (area != d_namedAreaMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isNamedAreaPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isWidgetComponentPresent(const String& name, bool includeInheritedLook) const
{
    WidgetComponentMap::const_iterator widgetComponentIter = d_widgetComponentMap.find(name);

    if (widgetComponentIter != d_widgetComponentMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isWidgetComponentPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isPropertyInitialiserPresent(const String& name, bool includeInheritedLook) const
{
    PropertyInitialiserMap::const_iterator propInitialiserIter = d_propertyInitialiserMap.find(name);

    if (propInitialiserIter != d_propertyInitialiserMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isPropertyInitialiserPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isPropertyDefinitionPresent(const String& name, bool includeInheritedLook) const
{
    PropertyDefinitionMap::const_iterator propDefIter = d_propertyDefinitionMap.find(name);

    if (propDefIter != d_propertyDefinitionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isPropertyDefinitionPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isPropertyLinkDefinitionPresent(const String& name, bool includeInheritedLook) const
{
    PropertyLinkDefinitionMap::const_iterator propLinkDefIter = d_propertyLinkDefinitionMap.find(name);

    if (propLinkDefIter != d_propertyLinkDefinitionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isPropertyLinkDefinitionPresent(name, true);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isEventLinkDefinitionPresent(const String& name, bool includeInheritedLook) const
{
    EventLinkDefinitionMap::const_iterator eventLinkDefIter = d_eventLinkDefinitionMap.find(name);

    if (eventLinkDefIter != d_eventLinkDefinitionMap.end())
        return true;

    if (d_inheritedLookName.empty() || !includeInheritedLook)
        return false;

    return WidgetLookManager::getSingleton().getWidgetLook(d_inheritedLookName).isEventLinkDefinitionPresent(name, true);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addNamedArea(const NamedArea& area)
{
    String name = area.getName();
    NamedAreaMap::iterator foundIter = d_namedAreaMap.find(name);

    if(foundIter != d_namedAreaMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addNamedArea - Entry for NamedArea '" +
                                         name + "' already exists. Replacing previous definition.");
        d_namedAreaMap.erase(foundIter);
    }

    d_namedAreaMap.insert(NamedAreaMap::value_type(name, area));
}


//---------------------------------------------------------------------------//
void WidgetLookFeel::renameNamedArea(const String& oldName, const String& newName)
{
    NamedAreaMap::iterator oldarea = d_namedAreaMap.find(oldName);
    NamedAreaMap::const_iterator newarea = d_namedAreaMap.find(newName);
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
void WidgetLookFeel::layoutChildWidgets(const Window& owner) const
{
    WidgetComponentCollator wcc;
    appendChildWidgetComponents(wcc);

    for (WidgetComponentCollator::const_iterator wci = wcc.begin();
         wci != wcc.end();
         ++wci)
    {
        (*wci)->layout(owner);
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyDefinition(PropertyDefinitionBase* propertyDefiniton)
{
    String name = propertyDefiniton->getPropertyName();
    PropertyDefinitionMap::iterator foundIter = d_propertyDefinitionMap.find(name);

    if(foundIter != d_propertyDefinitionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addPropertyDefinition - Entry for PropertyDefinition '" +
            name + "' already exists. Deleting and replacing previous definition.");
        delete( foundIter->second );
        d_propertyDefinitionMap.erase(foundIter);
    }

    d_propertyDefinitionMap.insert(PropertyDefinitionMap::value_type(name, propertyDefiniton));
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyDefinitions()
{
    PropertyDefinitionMap::iterator propDefIter = d_propertyDefinitionMap.begin();
    PropertyDefinitionMap::iterator propDefEnd = d_propertyDefinitionMap.end();
    while (propDefIter != propDefEnd)
    {
        delete (propDefIter->second);
        ++propDefIter;
    }

    d_propertyDefinitionMap.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyLinkDefinition(PropertyDefinitionBase* propertyLinkDefiniton)
{
    String name = propertyLinkDefiniton->getPropertyName();
    PropertyLinkDefinitionMap::iterator foundIter = d_propertyLinkDefinitionMap.find(name);

    if(foundIter != d_propertyLinkDefinitionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addPropertyLinkDefinition - Entry for PropertyLinkDefinition '" +
            name + "' already exists. Deleting and replacing previous definition.");
        delete( foundIter->second );
        d_propertyLinkDefinitionMap.erase(foundIter);
    }

    d_propertyLinkDefinitionMap.insert(PropertyLinkDefinitionMap::value_type(name, propertyLinkDefiniton));
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyLinkDefinitions()
{
    PropertyLinkDefinitionMap::iterator propLinkDefIter = d_propertyLinkDefinitionMap.begin();
    PropertyLinkDefinitionMap::iterator propLinkDefEnd = d_propertyLinkDefinitionMap.end();
    while (propLinkDefIter != propLinkDefEnd)
    {
        delete (propLinkDefIter->second);
        ++propLinkDefIter;
    }

    d_propertyLinkDefinitionMap.clear();
}

//---------------------------------------------------------------------------//
WidgetLookFeel::StringSet WidgetLookFeel::getStateImageryNames(bool includeInheritedLook) const
{
    StringSet result;

    StateImageryMap::const_iterator iter = d_stateImageryMap.begin();
    StateImageryMap::const_iterator iterEnd = d_stateImageryMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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

    ImagerySectionMap::const_iterator iter = d_imagerySectionMap.begin();
    ImagerySectionMap::const_iterator iterEnd = d_imagerySectionMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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

    NamedAreaMap::const_iterator iter = d_namedAreaMap.begin();
    NamedAreaMap::const_iterator iterEnd = d_namedAreaMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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

    WidgetComponentMap::const_iterator iter = d_widgetComponentMap.begin();
    WidgetComponentMap::const_iterator iterEnd = d_widgetComponentMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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

    PropertyDefinitionMap::const_iterator iter = d_propertyDefinitionMap.begin();
    PropertyDefinitionMap::const_iterator iterEnd = d_propertyDefinitionMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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

    PropertyLinkDefinitionMap::const_iterator iter = d_propertyLinkDefinitionMap.begin();
    PropertyLinkDefinitionMap::const_iterator iterEnd = d_propertyLinkDefinitionMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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

    PropertyInitialiserMap::const_iterator iter = d_propertyInitialiserMap.begin();
    PropertyInitialiserMap::const_iterator iterEnd = d_propertyInitialiserMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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

    EventLinkDefinitionMap::const_iterator iter = d_eventLinkDefinitionMap.begin();
    EventLinkDefinitionMap::const_iterator iterEnd = d_eventLinkDefinitionMap.end();
    while(iter != iterEnd)
    {
        result.insert(iter->first);
        ++iter;
    }

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
    AnimationNameSet ans;
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

    // These sub-scopes of the loops avoid the "'curr'-already-initialized"
    // compile error on VC6++
    {
        // output property definitions
        for (PropertyDefinitionMap::const_iterator curr = d_propertyDefinitionMap.begin();
             curr != d_propertyDefinitionMap.end();
             ++curr)
        {
            curr->second->writeDefinitionXMLToStream(xml_stream);
        }
    }

    {
        // output property link definitions
        for (PropertyLinkDefinitionMap::const_iterator curr = d_propertyLinkDefinitionMap.begin();
             curr != d_propertyLinkDefinitionMap.end();
             ++curr)
        {
            curr->second->writeDefinitionXMLToStream(xml_stream);
        }
    }

    {
        // output property initialisers.
        for (PropertyInitialiserMap::const_iterator curr = d_propertyInitialiserMap.begin();
             curr != d_propertyInitialiserMap.end();
             ++curr)
        {
            curr->second.writeXMLToStream(xml_stream);
        }
    }

    {
        // output named areas
        for (NamedAreaMap::const_iterator curr = d_namedAreaMap.begin();
             curr != d_namedAreaMap.end();
             ++curr)
        {
            curr->second.writeXMLToStream(xml_stream);
        }
    }

    {
        // output child widgets
        for (WidgetComponentMap::const_iterator curr = d_widgetComponentMap.begin();
             curr != d_widgetComponentMap.end();
             ++curr)
        {
            curr->second.writeXMLToStream(xml_stream);
        }
    }

    {
        // output imagery sections
        for (ImagerySectionMap::const_iterator curr = d_imagerySectionMap.begin();
             curr != d_imagerySectionMap.end();
             ++curr)
        {
            curr->second.writeXMLToStream(xml_stream);
        }
    }

    {
        // output states
        for (StateImageryMap::const_iterator curr = d_stateImageryMap.begin();
             curr != d_stateImageryMap.end();
             ++curr)
        {
            curr->second.writeXMLToStream(xml_stream);
        }
    }

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
    AnimationList::iterator it = std::find(d_animations.begin(),
                                           d_animations.end(),
                                           anim_name);

    if (it == d_animations.end())
        d_animations.push_back(anim_name);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addEventLinkDefinition(const EventLinkDefinition& evtdef)
{
    String name = evtdef.getName();
    EventLinkDefinitionMap::iterator foundIter = d_eventLinkDefinitionMap.find(name);

    if(foundIter != d_eventLinkDefinitionMap.end())
    {
        Logger::getSingleton().logEvent("WidgetLookFeel::addEventLinkDefinition - Entry for EventLinkDefinition '" +
                                        name + "' already exists. Replacing previous definition.");
        d_eventLinkDefinitionMap.erase(foundIter);
    }

    d_eventLinkDefinitionMap.insert(EventLinkDefinitionMap::value_type(name, evtdef));
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

    for (WidgetComponentMap::const_iterator iter = d_widgetComponentMap.begin();
         iter != d_widgetComponentMap.end();
         ++iter)
    {
        col.set(iter->first, &(iter->second));
    }

}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyDefinitions(PropertyDefinitionCollator& col,
                                               bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyDefinitions(col);

    for (PropertyDefinitionMap::iterator iter = d_propertyDefinitionMap.begin();
         iter != d_propertyDefinitionMap.end();
         ++iter)
    {
        col.set(iter->first, iter->second);
    }

}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyLinkDefinitions(
        PropertyLinkDefinitionCollator& col, bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyLinkDefinitions(col);

    for (PropertyLinkDefinitionMap::iterator iter = d_propertyLinkDefinitionMap.begin();
         iter != d_propertyLinkDefinitionMap.end();
         ++iter)
    {
        col.set(iter->first, iter->second);
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyInitialisers(
        PropertyInitialiserCollator& col, bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyInitialisers(col);

    for (PropertyInitialiserMap::const_iterator iter = d_propertyInitialiserMap.begin();
         iter != d_propertyInitialiserMap.end();
         ++iter)
    {
        col.set(iter->first, &(iter->second));
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendEventLinkDefinitions(
        EventLinkDefinitionCollator& col, bool inherits) const
{
    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendEventLinkDefinitions(col);

    for (EventLinkDefinitionMap::const_iterator iter = d_eventLinkDefinitionMap.begin();
         iter != d_eventLinkDefinitionMap.end();
         ++iter)
    {
        col.set(iter->first, &(iter->second));
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendAnimationNames(AnimationNameSet& set,
                                          bool inherits) const
{
    set.insert(d_animations.begin(),d_animations.end());

    if (!d_inheritedLookName.empty() && inherits)
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendAnimationNames(set);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::handleFontRenderSizeChange(Window& window,
                                                const Font* font) const
{
    bool result = false;

    for(ImagerySectionMap::const_iterator i = d_imagerySectionMap.begin();
        i != d_imagerySectionMap.end();
        ++i)
    {
        result |= i->second.handleFontRenderSizeChange(window, font);
    }

    for(WidgetComponentMap::const_iterator i = d_widgetComponentMap.begin();
        i != d_widgetComponentMap.end();
        ++i)
    {
        result |= i->second.handleFontRenderSizeChange(window, font);
    }

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

    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        StateImageryMap::iterator stateImageryIter = d_stateImageryMap.find(currentElementName);

        if(stateImageryIter == d_stateImageryMap.end())
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

        ++iter;
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::ImagerySectionPointerMap WidgetLookFeel::getImagerySectionMap(bool includeInheritedLook)
{
    ImagerySectionPointerMap pointerMap;
    StringSet nameSet = getImagerySectionNames(includeInheritedLook);
    
    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        ImagerySectionMap::iterator imagerySectionIter = d_imagerySectionMap.find(currentElementName);

        if(imagerySectionIter == d_imagerySectionMap.end())
        {
            throw UnknownObjectException("Error: ImagerySection with name: \"" + currentElementName
                                               + "\" exists in the list of names but a ImagerySection with this name"
                                               + "could not be found in the map");
        }
        else
        {
            ImagerySection* imagerySection = &imagerySectionIter->second;
            pointerMap.insert(ImagerySectionPointerMap::value_type(currentElementName, imagerySection));
        }

        ++iter;
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::NamedAreaPointerMap WidgetLookFeel::getNamedAreaMap(bool includeInheritedLook)
{
    NamedAreaPointerMap pointerMap;
    StringSet nameSet = getNamedAreaNames(includeInheritedLook);

    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        NamedAreaMap::iterator namedAreaIter = d_namedAreaMap.find(currentElementName);

        if(namedAreaIter == d_namedAreaMap.end())
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

        ++iter;
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::WidgetComponentPointerMap WidgetLookFeel::getWidgetComponentMap(bool includeInheritedLook)
{
    WidgetComponentPointerMap pointerMap;
    StringSet nameSet = getWidgetComponentNames(includeInheritedLook);

    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        WidgetComponentMap::iterator widgetComponentIter = d_widgetComponentMap.find(currentElementName);

        if(widgetComponentIter == d_widgetComponentMap.end())
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

        ++iter;
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::PropertyInitialiserPointerMap WidgetLookFeel::getPropertyInitialiserMap(bool includeInheritedLook)
{
    PropertyInitialiserPointerMap pointerMap;
    StringSet nameSet = getPropertyInitialiserNames(includeInheritedLook);

    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        PropertyInitialiserMap::iterator propertyInitIter = d_propertyInitialiserMap.find(currentElementName);

        if(propertyInitIter == d_propertyInitialiserMap.end())
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

        ++iter;
    }

    return pointerMap;
}


//---------------------------------------------------------------------------//
WidgetLookFeel::PropertyDefinitionBasePointerMap WidgetLookFeel::getPropertyDefinitionMap(bool includeInheritedLook)
{
    PropertyDefinitionBasePointerMap pointerMap;
    StringSet nameSet = getPropertyDefinitionNames(includeInheritedLook);

    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        PropertyDefinitionMap::iterator propDefIter = d_propertyDefinitionMap.find(currentElementName);

        if(propDefIter == d_propertyDefinitionMap.end())
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

        ++iter;
    }

    return pointerMap;
}

//---------------------------------------------------------------------------//
WidgetLookFeel::PropertyDefinitionBasePointerMap WidgetLookFeel::getPropertyLinkDefinitionMap(bool includeInheritedLook)
{
    PropertyDefinitionBasePointerMap pointerMap;
    StringSet nameSet = getPropertyLinkDefinitionNames(includeInheritedLook);

    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        PropertyLinkDefinitionMap::iterator propLinkDefIter = d_propertyLinkDefinitionMap.find(currentElementName);

        if(propLinkDefIter == d_propertyLinkDefinitionMap.end())
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

        ++iter;
    }

    return pointerMap;
}

 
//---------------------------------------------------------------------------//
WidgetLookFeel::EventLinkDefinitionPointerMap WidgetLookFeel::getEventLinkDefinitionMap(bool includeInheritedLook)
{
    EventLinkDefinitionPointerMap pointerMap;
    StringSet nameSet = getEventLinkDefinitionNames(includeInheritedLook);

    StringSet::iterator iter = nameSet.begin();
    StringSet::iterator iterEnd = nameSet.end();
    while(iter != iterEnd)
    {
        const CEGUI::String& currentElementName = *iter;
        EventLinkDefinitionMap::iterator eventLinkDefIter = d_eventLinkDefinitionMap.find(currentElementName);

        if(eventLinkDefIter == d_eventLinkDefinitionMap.end())
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

        ++iter;
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
    if(d_inheritedLookName.empty())
        return nullptr;

    WidgetLookManager::WidgetLookPointerMap pointerMap = WidgetLookManager::getSingleton().getWidgetLookPointerMap();
    WidgetLookManager::WidgetLookPointerMap::iterator foundIter = pointerMap.find(d_inheritedLookName);

    if(foundIter == pointerMap.end())
        throw UnknownObjectException("Error: Inherited WidgetLook with name: \"" + d_inheritedLookName
                                           + "\" cannot be found in the WidgetLookManager's map");

    return foundIter->second;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::copyPropertyDefinitionsFrom(const WidgetLookFeel& widgetLook)
{
    PropertyDefinitionMap::iterator iter = widgetLook.d_propertyDefinitionMap.begin();
    PropertyDefinitionMap::iterator iterEnd = widgetLook.d_propertyDefinitionMap.end();

    while(iter != iterEnd)
    {
        PropertyDefinitionMap::value_type insertValue(iter->first,
                                                      dynamic_cast<PropertyDefinitionBase*>(dynamic_cast<Property*>(iter->second)->clone()));
        d_propertyDefinitionMap.insert(insertValue);
            
        ++iter;
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::copyPropertyLinkDefinitionsFrom(const WidgetLookFeel& widgetLook)
{
    PropertyLinkDefinitionMap::iterator iter = widgetLook.d_propertyLinkDefinitionMap.begin();
    PropertyLinkDefinitionMap::iterator iterEnd = widgetLook.d_propertyLinkDefinitionMap.end();

    while(iter != iterEnd)
    {
        PropertyLinkDefinitionMap::value_type insertValue(iter->first,
                                                          dynamic_cast<PropertyDefinitionBase*>(dynamic_cast<Property*>(iter->second)->clone()));
        d_propertyLinkDefinitionMap.insert(insertValue);
            
        ++iter;
    }
}

//---------------------------------------------------------------------------//

} // End of  CEGUI namespace section

