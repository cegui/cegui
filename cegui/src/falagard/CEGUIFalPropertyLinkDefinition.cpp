/***********************************************************************
    filename:   CEGUIFalPropertyLinkDefinition.cpp
    created:    Sat Oct 8 2005
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
#include "falagard/CEGUIFalPropertyLinkDefinition.h"
#include "CEGUIWindowManager.h"
#include "CEGUIWindow.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    // Static string holding parent link identifier
    static const String S_parentIdentifier("__parent__");


    PropertyLinkDefinition::PropertyLinkDefinition(
                                const String& propertyName,
                                const String& widgetName,
                                const String& targetProperty,
                                const String& initialValue,
                                bool redrawOnWrite,
                                bool layoutOnWrite)

        : PropertyDefinitionBase(propertyName,
                                 "Falagard property link definition - links a "
                                     "property on this window to properties "
                                     "defined on one or more child windows, or "
                                     "the parent window.",
                                 initialValue,
                                 redrawOnWrite,
                                 layoutOnWrite)
    {
        // add initial target if it was specified via constructor
        // (typically meaning it came via XML attributes)
        if (!widgetName.empty() || !targetProperty.empty())
            addLinkTarget(widgetName, targetProperty);
    }

    String PropertyLinkDefinition::get(const PropertyReceiver* receiver) const
    {
        const LinkTargetCollection::const_iterator i(d_targets.begin());

        const Window* const target_wnd =
            getTargetWindow(receiver, (*i).d_widgetName);

        // if no target, or target (currently) invalid, return the default value
        if (d_targets.empty() || !target_wnd)
            return d_default;

        // otherwise return the value of the property for first target, since
        // this is considered the 'master' target for get operations.
        return target_wnd->getProperty((*i).d_targetProperty.empty() ?
                                                d_name :
                                                (*i).d_targetProperty);
    }

    void PropertyLinkDefinition::set(PropertyReceiver* receiver, const String& value)
    {
        LinkTargetCollection::iterator i = d_targets.begin();
        for ( ; i != d_targets.end(); ++i)
        {
            Window* target_wnd = getTargetWindow(receiver,
                                                 (*i).d_widgetName);

            // only try to set property if target is currently valid.
            if (target_wnd)
                target_wnd->setProperty((*i).d_targetProperty.empty() ?
                                                d_name :
                                                (*i).d_targetProperty, value);
        }

        // base handles things like ensuring redraws and such happen
        PropertyDefinitionBase::set(receiver, value);
    }

    const Window* PropertyLinkDefinition::getTargetWindow(const PropertyReceiver* receiver) const
    {
        if (d_targets.empty())
            return static_cast<const Window*>(receiver);

        return getTargetWindow(receiver,
                               (*d_targets.begin()).d_widgetName);
    }

    Window* PropertyLinkDefinition::getTargetWindow(PropertyReceiver* receiver)
    {
        return const_cast<Window*>(
            static_cast<const PropertyLinkDefinition*>(this)->
                getTargetWindow(receiver));
    }

    void PropertyLinkDefinition::addLinkTarget(const String& widget,
                                               const String& property)
    {
        const LinkTarget t = { widget, property };
        d_targets.push_back(t);
    }

    void PropertyLinkDefinition::clearLinkTargets()
    {
        d_targets.clear();
    }

    const Window* PropertyLinkDefinition::getTargetWindow(
                                            const PropertyReceiver* receiver,
                                            const String& name) const
    {
        if (name.empty())
            return static_cast<const Window*>(receiver);

        // handle link back to parent.  Return receiver if no parent.
        if (name== S_parentIdentifier)
            return static_cast<const Window*>(receiver)->getParent();

        return static_cast<const Window*>(receiver)->getChild(name);
    }

    Window* PropertyLinkDefinition::getTargetWindow(PropertyReceiver* receiver,
                                                    const String& name)
    {
        return const_cast<Window*>(
            static_cast<const PropertyLinkDefinition*>(this)->
                getTargetWindow(receiver, name));
    }

    void PropertyLinkDefinition::writeXMLElementType(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("PropertyLinkDefinition");
    }

    void PropertyLinkDefinition::writeXMLAttributes(XMLSerializer& xml_stream) const
    {
        PropertyDefinitionBase::writeXMLAttributes(xml_stream);

        // HACK: Here we abuse some intimate knowledge in that we know it's
        // safe to write our sub-elements out although the function is named
        // for writing attributes.  The alternative was to repeat code from the
        // base class, also demonstrating intimate knowledge ;)

        LinkTargetCollection::const_iterator i(d_targets.begin());

        // if there is one target only, write it out as attributes
        if (d_targets.size() == 1)
        {
            if (!(*i).d_widgetName.empty())
                xml_stream.attribute("widget", (*i).d_widgetName);

            if (!(*i).d_targetProperty.empty())
                xml_stream.attribute("targetProperty", (*i).d_targetProperty);
        }
        // we have multiple targets, so write them as PropertyLinkTarget tags
        else
        {
            for ( ; i != d_targets.end(); ++i)
            {
                xml_stream.openTag("PropertyLinkTarget");

                if (!(*i).d_widgetName.empty())
                    xml_stream.attribute("widget", (*i).d_widgetName);

                if (!(*i).d_targetProperty.empty())
                    xml_stream.attribute("property", (*i).d_targetProperty);

                xml_stream.closeTag();
            }
        }
    }

} // End of  CEGUI namespace section
