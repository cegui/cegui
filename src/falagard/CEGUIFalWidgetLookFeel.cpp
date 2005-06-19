/************************************************************************
    filename:   CEGUIFalWidgetLookFeel.cpp
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"

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
            throw UnknownObjectException("WidgetLookFeel::getStateImagery - unknown state '" + state + "'.");
        }

        return (*imagery).second;
    }


    const ImagerySection& WidgetLookFeel::getImagerySection(const CEGUI::String& section) const
    {
        ImageryList::const_iterator imgSect = d_imagerySections.find(section);

        if (imgSect == d_imagerySections.end())
        {
            throw UnknownObjectException("WidgetLookFeel::getImagerySection - unknown imagery section '" + section + "'.");
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
        if (d_childWidgets.find(widget.getWidgetLookName()) != d_childWidgets.end())
        {
            Logger::getSingleton().logEvent(
                "WidgetLookFeel::addWidgetComponent - Child widget defintion '" + widget.getWidgetLookName() + "' already exists.  Replacing previous definition.");
        }

        d_childWidgets[widget.getWidgetLookName()] = widget;
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

    void WidgetLookFeel::initialiseWidget(Window& widget) const
    {
        // apply properties to the parent window first
        for(PropertyList::const_iterator prop = d_properties.begin(); prop != d_properties.end(); ++prop)
        {
            (*prop).apply(widget);
        }

        // add required child widgets
        for(WidgetList::const_iterator curr = d_childWidgets.begin(); curr != d_childWidgets.end(); ++curr)
        {
            (*curr).second.create(widget);
        }
    }

} // End of  CEGUI namespace section
