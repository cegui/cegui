/************************************************************************
    filename:   CEGUIFalWidgetComponent.cpp
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
#include "falagard/CEGUIFalWidgetComponent.h"
#include "CEGUIWindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    WidgetComponent::WidgetComponent(const String& type, const String& look, const String& suffix) :
        d_baseType(type),
        d_imageryName(look),
        d_nameSuffix(suffix),
        d_vertAlign(VA_TOP),
        d_horzAlign(HA_LEFT)
    {}

    void WidgetComponent::create(Window& parent) const
    {
        // build final name and create widget.
        String widgetName = parent.getName() + d_nameSuffix;
        Window* widget = WindowManager::getSingleton().createWindow(d_baseType, widgetName);

        // TODO: Code to set the widget look

        // add the new widget to its parent
        parent.addChildWindow(widget);

        // initialise properties
        for(PropertiesList::const_iterator curr = d_properties.begin(); curr != d_properties.end(); ++curr)
        {
            (*curr).apply(*widget);
        }
    }

    const ComponentArea& WidgetComponent::getComponentArea() const
    {
        return d_area;
    }

    void WidgetComponent::setComponentArea(const ComponentArea& area)
    {
        d_area = area;
    }

    const String& WidgetComponent::getBaseWidgetType() const
    {
        return d_baseType;
    }

    void WidgetComponent::setBaseWidgetType(const String& type)
    {
        d_baseType = type;
    }

    const String& WidgetComponent::getWidgetLookName() const
    {
        return d_imageryName;
    }

    void WidgetComponent::setWidgetLookName(const String& look)
    {
        d_imageryName = look;
    }

    const String& WidgetComponent::getWidgetNameSuffix() const
    {
        return d_nameSuffix;
    }

    void WidgetComponent::setWidgetNameSuffix(const String& suffix)
    {
        d_nameSuffix = suffix;
    }

    VerticalAlignment WidgetComponent::getVerticalWidgetAlignemnt() const
    {
        return d_vertAlign;
    }

    void WidgetComponent::setVerticalWidgetAlignment(VerticalAlignment alignment)
    {
        d_vertAlign = alignment;
    }

    HorizontalAlignment WidgetComponent::getHorizontalWidgetAlignemnt() const
    {
        return d_horzAlign;
    }

    void WidgetComponent::setHorizontalWidgetAlignemnt(HorizontalAlignment alignment)
    {
        d_horzAlign = alignment;
    }

    void WidgetComponent::addPropertyInitialiser(const PropertyInitialiser& initialiser)
    {
        d_properties.push_back(initialiser);
    }

    void WidgetComponent::clearPropertyInitialisers()
    {
        d_properties.clear();
    }

} // End of  CEGUI namespace section
