/***********************************************************************
    filename:   CEGUIFalWidgetLookFeel.cpp
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
WidgetLookFeel::WidgetLookFeel(const String& name, const String& inherits) :
    d_lookName(name),
    d_inheritedLookName(inherits)
{
}

//---------------------------------------------------------------------------//
const StateImagery& WidgetLookFeel::getStateImagery(
                                        const CEGUI::String& state) const
{
    StateList::const_iterator imagery = d_stateImagery.find(state);

    if (imagery != d_stateImagery.end())
        return (*imagery).second;

    if (d_inheritedLookName.empty())
        CEGUI_THROW(UnknownObjectException(
            "WidgetLookFeel::getStateImagery - unknown state '" + state +
            "' in look '" + d_lookName + "'."));

    return WidgetLookManager::getSingleton().
        getWidgetLook(d_inheritedLookName).getStateImagery(state);
}

//---------------------------------------------------------------------------//
const ImagerySection& WidgetLookFeel::getImagerySection(
                                        const CEGUI::String& section) const
{
    ImageryList::const_iterator imgSect = d_imagerySections.find(section);

    if (imgSect != d_imagerySections.end())
        return (*imgSect).second;

    if (d_inheritedLookName.empty())
        CEGUI_THROW(UnknownObjectException(
            "WidgetLookFeel::getImagerySection - unknown imagery section '" +
            section +  "' in look '" + d_lookName + "'."));

    return WidgetLookManager::getSingleton().
        getWidgetLook(d_inheritedLookName).getImagerySection(section);
}

//---------------------------------------------------------------------------//
const String& WidgetLookFeel::getName() const
{
    return d_lookName;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addImagerySection(const ImagerySection& section)
{
    if (d_imagerySections.find(section.getName()) != d_imagerySections.end())
        Logger::getSingleton().logEvent(
            "WidgetLookFeel::addImagerySection - Defintion for imagery "
            "section '" + section.getName() + "' already exists.  "
            "Replacing previous definition.");

    d_imagerySections[section.getName()] = section;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addWidgetComponent(const WidgetComponent& widget)
{
    d_childWidgets.push_back(widget);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addStateSpecification(const StateImagery& state)
{
    if (d_stateImagery.find(state.getName()) != d_stateImagery.end())
        Logger::getSingleton().logEvent(
            "WidgetLookFeel::addStateSpecification - Defintion for state '" +
            state.getName() + "' already exists.  Replacing previous "
            "definition.");

    d_stateImagery[state.getName()] = state;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyInitialiser(
                        const PropertyInitialiser& initialiser)
{
    d_properties.push_back(initialiser);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearImagerySections()
{
    d_imagerySections.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearWidgetComponents()
{
    d_childWidgets.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearStateSpecifications()
{
    d_stateImagery.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyInitialisers()
{
    d_properties.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::initialiseWidget(Window& widget) const
{
    // add required child widgets
    WidgetComponentPtrMap wcm;
    appendChildWidgetComponents(wcm);
    for (WidgetComponentPtrMap::const_iterator wci = wcm.begin();
         wci != wcm.end();
         ++wci)
    {
        wci->second->create(widget);
    }

    // add new property definitions
    PropertyDefinitionPtrMap pdm;
    appendPropertyDefinitions(pdm);
    for (PropertyDefinitionPtrMap::const_iterator pdi = pdm.begin();
         pdi != pdm.end();
         ++pdi)
    {
        // add the property to the window
        widget.addProperty(pdi->second);
        // write default value to get things set up properly
        widget.setProperty(pdi->first,
                           pdi->second->getDefault(&widget));
    }

    // add new property link definitions
    PropertyLinkDefinitionPtrMap pldm;
    appendPropertyLinkDefinitions(pldm);
    for (PropertyLinkDefinitionPtrMap::const_iterator pldi = pldm.begin();
         pldi != pldm.end();
         ++pldi)
    {
        // add the property to the window
        widget.addProperty(pldi->second);
        // write default value to get things set up properly
        widget.setProperty(pldi->first,
                           pldi->second->getDefault(&widget));
    }

    // apply properties to the parent window
    PropertyInitialiserPtrMap pim;
    appendPropertyInitialisers(pim);
    for (PropertyInitialiserPtrMap::const_iterator pi = pim.begin();
         pi != pim.end();
         ++pi)
    {
        pi->second->apply(widget);
    }

    // setup linked events
    EventLinkDefinitionPtrMap eldm;
    appendEventLinkDefinitions(eldm);
    for (EventLinkDefinitionPtrMap::const_iterator eldi = eldm.begin();
         eldi != eldm.end();
         ++eldi)
    {
        eldi->second->initialiseWidget(widget);
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

        d_animationInstances.insert(std::make_pair(&widget, instance));
        instance->setTargetWindow(&widget);
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::cleanUpWidget(Window& widget) const
{
    if (widget.getLookNFeel() != getName())
    {
        CEGUI_THROW(InvalidRequestException(
            "WidgetLookFeel::cleanUpWidget - The window '"
            + widget.getName() +
            "' does not have this look'n'feel assigned"));
    }

    // remove added child widgets
    WidgetComponentPtrMap wcl;
    appendChildWidgetComponents(wcl);
    for (WidgetComponentPtrMap::const_iterator wci = wcl.begin();
         wci != wcl.end();
         ++wci)
    {
        if (widget.isChild(wci->second->getWidgetName()))
            widget.destroyChild(wci->second->getWidgetName());
    }

    // delete added named Events
    EventLinkDefinitionPtrMap eldm;
    appendEventLinkDefinitions(eldm);
    for (EventLinkDefinitionPtrMap::const_iterator eldi = eldm.begin();
         eldi != eldm.end();
         ++eldi)
    {
        eldi->second->cleanUpWidget(widget);
    }

    // remove added property definitions
    PropertyDefinitionPtrMap pdm;
    appendPropertyDefinitions(pdm);
    for (PropertyDefinitionPtrMap::const_iterator pdi = pdm.begin();
         pdi != pdm.end();
         ++pdi)
    {
        // remove the property from the window
        widget.removeProperty(pdi->first);
    }

    // remove added property link definitions
    PropertyLinkDefinitionPtrMap pldm;
    appendPropertyLinkDefinitions(pldm);
    for (PropertyLinkDefinitionPtrMap::const_iterator pldi = pldm.begin();
         pldi != pldm.end();
         ++pldi)
    {
        // remove the property from the window
        widget.removeProperty(pldi->first);
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
bool WidgetLookFeel::isStateImageryPresent(const String& state) const
{
    StateList::const_iterator i = d_stateImagery.find(state);
    
    if (i != d_stateImagery.end())
        return true;

    if (d_inheritedLookName.empty())
        return false;
 
    return WidgetLookManager::getSingleton().
        getWidgetLook(d_inheritedLookName).isStateImageryPresent(state);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addNamedArea(const NamedArea& area)
{
    if (d_namedAreas.find(area.getName()) != d_namedAreas.end())
        Logger::getSingleton().logEvent(
            "WidgetLookFeel::addNamedArea - Defintion for area '" +
            area.getName() + "' already exists.  Replacing previous "
            "definition.");

    d_namedAreas[area.getName()] = area;
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearNamedAreas()
{
    d_namedAreas.clear();
}

//---------------------------------------------------------------------------//
const NamedArea& WidgetLookFeel::getNamedArea(const String& name) const
{
    NamedAreaList::const_iterator area = d_namedAreas.find(name);

    if (area != d_namedAreas.end())
        return (*area).second;

    if (d_inheritedLookName.empty())
        CEGUI_THROW(UnknownObjectException(
            "WidgetLookFeel::getNamedArea - unknown named area: '" + name +
            "' in look '" + d_lookName + "'."));

    return WidgetLookManager::getSingleton().
        getWidgetLook(d_inheritedLookName).getNamedArea(name);
}

//---------------------------------------------------------------------------//
bool WidgetLookFeel::isNamedAreaDefined(const String& name) const
{
    NamedAreaList::const_iterator area = d_namedAreas.find(name);

    if (area != d_namedAreas.end())
        return true;

    if (d_inheritedLookName.empty())
        return false;

    return WidgetLookManager::getSingleton().
        getWidgetLook(d_inheritedLookName).isNamedAreaDefined(name);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::layoutChildWidgets(const Window& owner) const
{
    WidgetComponentPtrMap wcl;
    appendChildWidgetComponents(wcl);

    for (WidgetComponentPtrMap::const_iterator wci = wcl.begin();
         wci != wcl.end();
         ++wci)
    {
        wci->second->layout(owner);
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyDefinition(const PropertyDefinition& propdef)
{
    d_propertyDefinitions.push_back(propdef);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyDefinitions()
{
    d_propertyDefinitions.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::addPropertyLinkDefinition(
                            const PropertyLinkDefinition& propdef)
{
    d_propertyLinkDefinitions.push_back(propdef);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearPropertyLinkDefinitions()
{
    d_propertyLinkDefinitions.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::writeXMLToStream(XMLSerializer& xml_stream) const
{
    xml_stream.openTag("WidgetLook")
    .attribute("name", d_lookName)
    .attribute("inherits", d_inheritedLookName);

    // These sub-scopes of the loops avoid the "'curr'-already-initialized"
    // compile error on VC6++
    {
        // output property definitions
        for (PropertyDefinitionList::const_iterator curr = d_propertyDefinitions.begin();
             curr != d_propertyDefinitions.end();
             ++curr)
        {
            (*curr).writeXMLToStream(0, xml_stream);
        }
    }

    {
        // output property link definitions
        for (PropertyLinkDefinitionList::const_iterator curr = d_propertyLinkDefinitions.begin();
             curr != d_propertyLinkDefinitions.end();
             ++curr)
        {
            (*curr).writeXMLToStream(0, xml_stream);
        }
    }

    {
        // output property initialisers.
        for (PropertyList::const_iterator curr = d_properties.begin();
             curr != d_properties.end();
             ++curr)
        {
            (*curr).writeXMLToStream(xml_stream);
        }
    }

    {
        // output named areas
        for (NamedAreaList::const_iterator curr = d_namedAreas.begin();
             curr != d_namedAreas.end();
             ++curr)
        {
            (*curr).second.writeXMLToStream(xml_stream);
        }
    }

    {
        // output child widgets
        for (WidgetList::const_iterator curr = d_childWidgets.begin();
             curr != d_childWidgets.end();
             ++curr)
        {
            (*curr).writeXMLToStream(xml_stream);
        }
    }

    {
        // output imagery sections
        for (ImageryList::const_iterator curr = d_imagerySections.begin();
             curr != d_imagerySections.end();
             ++curr)
        {
            (*curr).second.writeXMLToStream(xml_stream);
        }
    }

    {
        // output states
        for (StateList::const_iterator curr = d_stateImagery.begin();
             curr != d_stateImagery.end();
             ++curr)
        {
            (*curr).second.writeXMLToStream(xml_stream);
        }
    }

    xml_stream.closeTag();
}

//---------------------------------------------------------------------------//
const PropertyInitialiser* WidgetLookFeel::findPropertyInitialiser(
                                            const String& propertyName) const
{
    PropertyInitialiserPtrMap pim;
    appendPropertyInitialisers(pim);

    PropertyInitialiserPtrMap::const_iterator i = pim.find(propertyName);

    if (i == pim.end())
        return 0;

    return i->second;
}

//---------------------------------------------------------------------------//
const WidgetComponent* WidgetLookFeel::findWidgetComponent(
                                            const String& name) const
{
    WidgetComponentPtrMap wcl;
    appendChildWidgetComponents(wcl);

    WidgetComponentPtrMap::const_iterator wci = wcl.find(name);

    if (wci == wcl.end())
        return 0;

    return wci->second;
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
    d_eventLinkDefinitions.push_back(evtdef);
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::clearEventLinkDefinitions()
{
    d_eventLinkDefinitions.clear();
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendChildWidgetComponents(WidgetComponentPtrMap& map) const
{
    if (!d_inheritedLookName.empty())
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendChildWidgetComponents(map);

    for (WidgetList::const_iterator i = d_childWidgets.begin();
         i != d_childWidgets.end();
         ++i)
    {
        map[(*i).getWidgetName()] = &(*i);
    }

}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyDefinitions(PropertyDefinitionPtrMap& map) const
{
    if (!d_inheritedLookName.empty())
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyDefinitions(map);

    for (PropertyDefinitionList::iterator i = d_propertyDefinitions.begin();
         i != d_propertyDefinitions.end();
         ++i)
    {
        map[(*i).getName()] = &(*i);
    }

}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyLinkDefinitions(PropertyLinkDefinitionPtrMap& map) const
{
    if (!d_inheritedLookName.empty())
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyLinkDefinitions(map);

    for (PropertyLinkDefinitionList::iterator i = d_propertyLinkDefinitions.begin();
         i != d_propertyLinkDefinitions.end();
         ++i)
    {
        map[(*i).getName()] = &(*i);
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendPropertyInitialisers(PropertyInitialiserPtrMap& map) const
{
    if (!d_inheritedLookName.empty())
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendPropertyInitialisers(map);

    for (PropertyList::const_iterator i = d_properties.begin();
         i != d_properties.end();
         ++i)
    {
        map[(*i).getTargetPropertyName()] = &(*i);
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendEventLinkDefinitions(EventLinkDefinitionPtrMap& map) const
{
    if (!d_inheritedLookName.empty())
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendEventLinkDefinitions(map);

    for (EventLinkDefinitionList::const_iterator i = d_eventLinkDefinitions.begin();
         i != d_eventLinkDefinitions.end();
         ++i)
    {
        map[(*i).getName()] = &(*i);
    }
}

//---------------------------------------------------------------------------//
void WidgetLookFeel::appendAnimationNames(AnimationNameSet& set) const
{
    for (AnimationList::const_iterator i = d_animations.begin();
         i != d_animations.end();
         ++i)
    {
        if (set.find(*i) == set.end())
            set.insert(*i);
    }

    if (!d_inheritedLookName.empty())
        WidgetLookManager::getSingleton().
            getWidgetLook(d_inheritedLookName).appendAnimationNames(set);
}

//---------------------------------------------------------------------------//

} // End of  CEGUI namespace section

