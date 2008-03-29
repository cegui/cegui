/***********************************************************************
    filename:   CEGUIFalWidgetLookFeel.cpp
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIWindowManager.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    WidgetLookFeel::WidgetLookFeel(const String& name) :
        d_lookName(name)
    {}

    const StateImagery& WidgetLookFeel::getStateImagery(const CEGUI::String& state) const
    {
        StateList::const_iterator imagery = d_stateImagery.find(state);

        if (imagery == d_stateImagery.end())
        {
            throw UnknownObjectException("WidgetLookFeel::getStateImagery - unknown state '" + state + "' in look '" + d_lookName + "'.");
        }

        return (*imagery).second;
    }


    const ImagerySection& WidgetLookFeel::getImagerySection(const CEGUI::String& section) const
    {
        ImageryList::const_iterator imgSect = d_imagerySections.find(section);

        if (imgSect == d_imagerySections.end())
        {
            throw UnknownObjectException("WidgetLookFeel::getImagerySection - unknown imagery section '" + section +  "' in look '" + d_lookName + "'.");
        }

        return (*imgSect).second;
    }


    const String& WidgetLookFeel::getName() const
    {
        return d_lookName;
    }

    void WidgetLookFeel::addImagerySection(const ImagerySection& section)
    {
        if (d_imagerySections.find(section.getName()) != d_imagerySections.end())
        {
            Logger::getSingleton().logEvent(
                "WidgetLookFeel::addImagerySection - Defintion for imagery section '" + section.getName() + "' already exists.  Replacing previous definition.");
        }

        d_imagerySections[section.getName()] = section;
    }

    void WidgetLookFeel::addWidgetComponent(const WidgetComponent& widget)
    {
        d_childWidgets.push_back(widget);
    }

    void WidgetLookFeel::addStateSpecification(const StateImagery& state)
    {
        if (d_stateImagery.find(state.getName()) != d_stateImagery.end())
        {
            Logger::getSingleton().logEvent(
                "WidgetLookFeel::addStateSpecification - Defintion for state '" + state.getName() + "' already exists.  Replacing previous definition.");
        }

        d_stateImagery[state.getName()] = state;
    }

    void WidgetLookFeel::addPropertyInitialiser(const PropertyInitialiser& initialiser)
    {
        d_properties.push_back(initialiser);
    }

    void WidgetLookFeel::clearImagerySections()
    {
        d_imagerySections.clear();
    }

    void WidgetLookFeel::clearWidgetComponents()
    {
        d_childWidgets.clear();
    }

    void WidgetLookFeel::clearStateSpecifications()
    {
        d_stateImagery.clear();
    }

    void WidgetLookFeel::clearPropertyInitialisers()
    {
        d_properties.clear();
    }

    /*************************************************************************
        Initialise a widget for this look'n'feel
    *************************************************************************/
    void WidgetLookFeel::initialiseWidget(Window& widget) const
    {
        // add required child widgets
        for(WidgetList::const_iterator curr = d_childWidgets.begin(); curr != d_childWidgets.end(); ++curr)
        {
            (*curr).create(widget);
        }

        // add new property definitions
        for(PropertyDefinitionList::iterator propdef = d_propertyDefinitions.begin(); propdef != d_propertyDefinitions.end(); ++propdef)
        {
            // add the property to the window
            widget.addProperty(&(*propdef));
            // write default value to get things set up properly
            widget.setProperty((*propdef).getName(), (*propdef).getDefault(&widget));
        }

        // add new property link definitions
        for(PropertyLinkDefinitionList::iterator linkdef = d_propertyLinkDefinitions.begin(); linkdef != d_propertyLinkDefinitions.end(); ++linkdef)
        {
            // add the property to the window
            widget.addProperty(&(*linkdef));
            // write default value to get things set up properly
            widget.setProperty((*linkdef).getName(), (*linkdef).getDefault(&widget));
        }

        // apply properties to the parent window
        for(PropertyList::const_iterator prop = d_properties.begin(); prop != d_properties.end(); ++prop)
        {
            (*prop).apply(widget);
        }

    }

    /*************************************************************************
        Clean up a widget currently using this look'n'feel
    *************************************************************************/
    void WidgetLookFeel::cleanUpWidget(Window& widget) const
    {
        if (widget.getLookNFeel() != getName())
        {
            throw InvalidRequestException(
                "WidgetLookFeel::cleanUpWidget - The window '"
                + widget.getName() +
                "' does not have this look'n'feel assigned");
        }

        // remove added child widgets
        for(WidgetList::const_iterator curr = d_childWidgets.begin(); curr != d_childWidgets.end(); ++curr)
        {
            WindowManager::getSingleton().destroyWindow(widget.getName() + (*curr).getWidgetNameSuffix());
        }

        // remove added property definitions
        for(PropertyDefinitionList::iterator propdef = d_propertyDefinitions.begin(); propdef != d_propertyDefinitions.end(); ++propdef)
        {
            // remove the property from the window
            widget.removeProperty((*propdef).getName());
        }

        // remove added property link definitions
        for(PropertyLinkDefinitionList::iterator linkdef = d_propertyLinkDefinitions.begin(); linkdef != d_propertyLinkDefinitions.end(); ++linkdef)
        {
            // remove the property from the window
            widget.removeProperty((*linkdef).getName());
        }
    }

    bool WidgetLookFeel::isStateImageryPresent(const String& state) const
    {
        return d_stateImagery.find(state) != d_stateImagery.end();
    }

    void WidgetLookFeel::addNamedArea(const NamedArea& area)
    {
        if (d_namedAreas.find(area.getName()) != d_namedAreas.end())
        {
            Logger::getSingleton().logEvent(
                "WidgetLookFeel::addNamedArea - Defintion for area '" + area.getName() + "' already exists.  Replacing previous definition.");
        }

        d_namedAreas[area.getName()] = area;
    }

    void WidgetLookFeel::clearNamedAreas()
    {
        d_namedAreas.clear();
    }

    const NamedArea& WidgetLookFeel::getNamedArea(const String& name) const
    {
        NamedAreaList::const_iterator area = d_namedAreas.find(name);

        if (area == d_namedAreas.end())
        {
            throw UnknownObjectException("WidgetLookFeel::getNamedArea - unknown named area: '" + name +  "' in look '" + d_lookName + "'.");
        }

        return (*area).second;
    }

    bool WidgetLookFeel::isNamedAreaDefined(const String& name) const
    {
        return d_namedAreas.find(name) != d_namedAreas.end();
    }

    void WidgetLookFeel::layoutChildWidgets(const Window& owner) const
    {
        // apply properties to the parent window
        for(WidgetList::const_iterator wdgt = d_childWidgets.begin(); wdgt != d_childWidgets.end(); ++wdgt)
        {
            (*wdgt).layout(owner);
        }
    }

    void WidgetLookFeel::addPropertyDefinition(const PropertyDefinition& propdef)
    {
        d_propertyDefinitions.push_back(propdef);
    }

    void WidgetLookFeel::clearPropertyDefinitions()
    {
        d_propertyDefinitions.clear();
    }

    void WidgetLookFeel::addPropertyLinkDefinition(const PropertyLinkDefinition& propdef)
    {
        d_propertyLinkDefinitions.push_back(propdef);
    }

    void WidgetLookFeel::clearPropertyLinkDefinitions()
    {
        d_propertyLinkDefinitions.clear();
    }

    void WidgetLookFeel::writeXMLToStream(XMLSerializer& xml_stream) const
    {

        xml_stream.openTag("WidgetLook")
                .attribute("name", d_lookName);

        // These sub-scobes of the loops avoid the "'curr'-already-initialized" compile error on VC6++
        {
          // output property definitions
          for (PropertyDefinitionList::const_iterator curr = d_propertyDefinitions.begin(); curr != d_propertyDefinitions.end(); ++curr)
              (*curr).writeXMLToStream(xml_stream);
        }

        {
          // output property link definitions
          for (PropertyLinkDefinitionList::const_iterator curr = d_propertyLinkDefinitions.begin(); curr != d_propertyLinkDefinitions.end(); ++curr)
              (*curr).writeXMLToStream(xml_stream);
        }

        {
          // output property initialisers.
          for (PropertyList::const_iterator curr = d_properties.begin(); curr != d_properties.end(); ++curr)
              (*curr).writeXMLToStream(xml_stream);
        }

        {
          // output named areas
          for (NamedAreaList::const_iterator curr = d_namedAreas.begin(); curr != d_namedAreas.end(); ++curr)
              (*curr).second.writeXMLToStream(xml_stream);
        }

        {
          // output child widgets
          for (WidgetList::const_iterator curr = d_childWidgets.begin(); curr != d_childWidgets.end(); ++curr)
              (*curr).writeXMLToStream(xml_stream);
        }

        {
          // output imagery sections
          for (ImageryList::const_iterator curr = d_imagerySections.begin(); curr != d_imagerySections.end(); ++curr)
              (*curr).second.writeXMLToStream(xml_stream);
        }

        {
          // output states
          for (StateList::const_iterator curr = d_stateImagery.begin(); curr != d_stateImagery.end(); ++curr)
              (*curr).second.writeXMLToStream(xml_stream);
        }

        xml_stream.closeTag();
    }

    void WidgetLookFeel::renameChildren(const Window& widget, const String& newBaseName) const
    {
        WindowManager& winMgr = WindowManager::getSingleton();

        for(WidgetList::const_iterator curr = d_childWidgets.begin(); curr != d_childWidgets.end(); ++curr)
            winMgr.renameWindow(widget.getName() + (*curr).getWidgetNameSuffix(),
                                newBaseName + (*curr).getWidgetNameSuffix());
    }

    const PropertyInitialiser* WidgetLookFeel::findPropertyInitialiser(const String& propertyName) const
    {
        PropertyList::const_reverse_iterator i = d_properties.rbegin();
        while (i != d_properties.rend())
        {
            if ((*i).getTargetPropertyName() == propertyName)
                return &(*i);
            ++i;
        }
        return 0;
    }

    const WidgetComponent* WidgetLookFeel::findWidgetComponent(const String& nameSuffix) const
    {
        WidgetList::const_iterator i = d_childWidgets.begin();
        while (i != d_childWidgets.end())
        {
            if ((*i).getWidgetNameSuffix() == nameSuffix)
                return &(*i);
            ++i;
        }
        return 0;
    }

} // End of  CEGUI namespace section
