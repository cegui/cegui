/************************************************************************
    filename:   CEGUIFalPropertyLinkDefinition.cpp
    created:    Sat Oct 8 2005
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
#include "falagard/CEGUIFalPropertyLinkDefinition.h"
#include "CEGUIWindowManager.h"
#include "CEGUIWindow.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    PropertyLinkDefinition::PropertyLinkDefinition(
                                const String& propertyName,
                                const String& widgetNameSuffix,
                                const String& targetProperty,
                                const String& initialValue,
                                bool redrawOnWrite,
                                bool layoutOnWrite)

        : PropertyDefinitionBase(propertyName,
                                 "Falagard property link definition - links a property on this window to another defined on a child window.",
                                 initialValue,
                                 redrawOnWrite,
                                 layoutOnWrite),

          d_widgetNameSuffix(widgetNameSuffix),
          d_targetProperty(targetProperty)
    {
    }

    String PropertyLinkDefinition::get(const PropertyReceiver* receiver) const
    {
        return getTargetWindow(receiver)->getProperty(d_targetProperty.empty() ? d_name : d_targetProperty);
    }

    void PropertyLinkDefinition::set(PropertyReceiver* receiver, const String& value)
    {
        getTargetWindow(receiver)->setProperty(d_targetProperty.empty() ? d_name : d_targetProperty, value);
        PropertyDefinitionBase::set(receiver, value);
    }

    const Window* PropertyLinkDefinition::getTargetWindow(const PropertyReceiver* receiver) const
    {
        // if no name suffix, we are the target (not very useful, but still...)
        if (d_widgetNameSuffix.empty())
            return static_cast<const Window*>(receiver);

        return WindowManager::getSingleton().getWindow(
                static_cast<const Window*>(receiver)->getName() + d_widgetNameSuffix);
    }

    Window* PropertyLinkDefinition::getTargetWindow(PropertyReceiver* receiver)
    {
        return const_cast<Window*>(static_cast<const PropertyLinkDefinition*>(this)->getTargetWindow(receiver));
    }


    void PropertyLinkDefinition::writeXMLElementType(OutStream& out_stream) const
    {
        out_stream << "PropertyLinkDefinition";
    }

    void PropertyLinkDefinition::writeXMLAttributes(OutStream& out_stream) const
    {
        if (!d_widgetNameSuffix.empty())
            out_stream << "widget=\"" << d_widgetNameSuffix << "\" ";

        if (!d_targetProperty.empty())
            out_stream << "targetProperty=\"" << d_targetProperty << "\" ";
    }

} // End of  CEGUI namespace section
