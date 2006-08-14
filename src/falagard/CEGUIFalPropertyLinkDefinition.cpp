/***********************************************************************
    filename:   CEGUIFalPropertyLinkDefinition.cpp
    created:    Sat Oct 8 2005
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


    void PropertyLinkDefinition::writeXMLElementType(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("PropertyLinkDefinition");
    }

    void PropertyLinkDefinition::writeXMLAttributes(XMLSerializer& xml_stream) const
    {
        PropertyDefinitionBase::writeXMLAttributes(xml_stream);

        if (!d_widgetNameSuffix.empty())
            xml_stream.attribute("widget", d_widgetNameSuffix);

        if (!d_targetProperty.empty())
            xml_stream.attribute("targetProperty",  d_targetProperty);
    }

} // End of  CEGUI namespace section
